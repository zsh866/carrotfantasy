#include "GameDataManager.h"
#include "cocos2d.h"

USING_NS_CC;

bool GameDataManager::init()
{
    if (_initialized)
        return true;

    std::string dbPath = FileUtils::getInstance()->getWritablePath() + "gamedata.db";
    bool const isNewDatabase = !FileUtils::getInstance()->isFileExist(dbPath);

    sqlite3* raw_db;
    int result = sqlite3_open(dbPath.c_str(), &raw_db);
    if (result != SQLITE_OK) {
        CCLOG("Failed to open database: %s", sqlite3_errmsg(raw_db));
        return false;
    }

    _db.reset(raw_db); // 将原始指针交给智能指针管理

    if (isNewDatabase) {
        // 创建关卡进度表
        const char* sql_level = "CREATE TABLE IF NOT EXISTS level_progress ("
                                "level_type INTEGER, "
                                "map_index INTEGER, "
                                "stars INTEGER DEFAULT 0, "
                                "unlocked INTEGER DEFAULT 0, "
                                "PRIMARY KEY (level_type, map_index))";

        // 创建游戏状态表
        const char* sql_state = "CREATE TABLE IF NOT EXISTS game_state ("
                                "level_type INTEGER, "
                                "map_index INTEGER, "
                                "state TEXT, "
                                "update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                                "PRIMARY KEY (level_type, map_index))";

        // 创建设置表
        const char* sql_settings = "CREATE TABLE IF NOT EXISTS settings ("
                                   "key TEXT PRIMARY KEY, "
                                   "value TEXT)";

        char* errMsg = nullptr;
        result = sqlite3_exec(_db.get(), sql_level, nullptr, nullptr, &errMsg);
        result |= sqlite3_exec(_db.get(), sql_state, nullptr, nullptr, &errMsg);
        result |= sqlite3_exec(_db.get(), sql_settings, nullptr, nullptr, &errMsg);

        if (result != SQLITE_OK) {
            CCLOG("Failed to create tables: %s", errMsg);
            sqlite3_free(errMsg);
            return false;
        }
    } else {
        // 验证表是否存在
        const char* sql_verify = "SELECT name FROM sqlite_master WHERE type='table' AND "
                                 "(name='level_progress' OR name='game_state' OR name='settings')";

        sqlite3_stmt* stmt;
        result = sqlite3_prepare_v2(_db.get(), sql_verify, -1, &stmt, nullptr);
        if (result != SQLITE_OK)
            return false;

        int tableCount = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            tableCount++;
        }
        sqlite3_finalize(stmt);

        if (tableCount != 3) {
            CCLOG("Database structure is incomplete");
            return false;
        }
    }

    _initialized = true;
    return true;
}

bool GameDataManager::saveLevelProgress(int levelType, int mapIndex, int stars)
{
    if (!_initialized)
        return false;

    const char* sql = "INSERT OR REPLACE INTO level_progress (level_type, map_index, stars, unlocked) "
                      "VALUES (?, ?, ?, 1)";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, levelType);
    sqlite3_bind_int(stmt, 2, mapIndex);
    sqlite3_bind_int(stmt, 3, stars);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // 解锁下一关
    if (result == SQLITE_DONE) {
        const char* sql_unlock = "INSERT OR REPLACE INTO level_progress (level_type, map_index, unlocked) "
                                 "VALUES (?, ?, 1) "
                                 "ON CONFLICT DO NOTHING";

        result = sqlite3_prepare_v2(_db.get(), sql_unlock, -1, &stmt, nullptr);
        if (result == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, levelType);
            sqlite3_bind_int(stmt, 2, mapIndex + 1);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    return result == SQLITE_DONE;
}

bool GameDataManager::isLevelUnlocked(int levelType, int mapIndex)
{
    if (!_initialized)
        return false;
    if (mapIndex == 0)
        return true; // 第一关总是解锁的

    const char* sql = "SELECT unlocked FROM level_progress "
                      "WHERE level_type = ? AND map_index = ?";

    sqlite3_stmt* stmt;
    const int result = sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, levelType);
    sqlite3_bind_int(stmt, 2, mapIndex);

    bool unlocked = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        unlocked = sqlite3_column_int(stmt, 0) == 1;
    }

    sqlite3_finalize(stmt);
    return unlocked;
}

bool GameDataManager::saveGameState(int levelType, int mapIndex, const std::string& gameState)
{
    if (!_initialized)
        return false;

    const char* sql = "INSERT OR REPLACE INTO game_state (level_type, map_index, state) "
                      "VALUES (?, ?, ?)";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, levelType);
    sqlite3_bind_int(stmt, 2, mapIndex);
    sqlite3_bind_text(stmt, 3, gameState.c_str(), -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

std::string GameDataManager::loadGameState(int levelType, int mapIndex)
{
    if (!_initialized)
        return "";

    const char* sql = "SELECT state FROM game_state "
                      "WHERE level_type = ? AND map_index = ?";

    sqlite3_stmt* stmt;
    const int result = sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
        return "";

    sqlite3_bind_int(stmt, 1, levelType);
    sqlite3_bind_int(stmt, 2, mapIndex);

    std::string state;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* text = (const char*)sqlite3_column_text(stmt, 0);
        if (text) {
            state = text;
        }
    }

    sqlite3_finalize(stmt);
    return state;
}

bool GameDataManager::saveSoundSettings(bool bgmEnabled, bool sfxEnabled,
    float bgmVolume, float sfxVolume)
{
    if (!_initialized)
        return false;

    const char* sql = "INSERT OR REPLACE INTO settings (key, value) VALUES (?, ?)";
    sqlite3_stmt* stmt;
    bool success = true;

    // 保存BGM开关状态
    if (sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, "bgm_enabled", -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, bgmEnabled ? "1" : "0", -1, SQLITE_STATIC);
        success &= (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
    }

    // 保存音效开关状态
    if (sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, "sfx_enabled", -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, sfxEnabled ? "1" : "0", -1, SQLITE_STATIC);
        success &= (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
    }

    // 保存BGM音量
    if (sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, "bgm_volume", -1, SQLITE_STATIC);
        std::string volumeStr = std::to_string(bgmVolume);
        CCLOG("Saving bgm_volume: %s", volumeStr.c_str());

        if (sqlite3_bind_text(stmt, 2, volumeStr.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            CCLOG("Failed to bind BGM volume: %s", sqlite3_errmsg(_db.get()));
        }

        const int stepResult = sqlite3_step(stmt);
        if (stepResult != SQLITE_DONE) {
            CCLOG("Failed to save BGM volume: %s", sqlite3_errmsg(_db.get()));
        }

        success &= (stepResult == SQLITE_DONE);
        sqlite3_finalize(stmt);
    }

    // 保存音效音量
    if (sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, "sfx_volume", -1, SQLITE_STATIC);
        std::string volumeStr = std::to_string(sfxVolume);
        CCLOG("Saving sfx_volume: %s", volumeStr.c_str());

        if (sqlite3_bind_text(stmt, 2, volumeStr.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
            CCLOG("Failed to bind SFX volume: %s", sqlite3_errmsg(_db.get()));
        }

        const int stepResult = sqlite3_step(stmt);
        if (stepResult != SQLITE_DONE) {
            CCLOG("Failed to save SFX volume: %s", sqlite3_errmsg(_db.get()));
        }
        success &= (stepResult == SQLITE_DONE);
        sqlite3_finalize(stmt);
    }

    return success;
}

// 获取设置的辅助函数
std::string getSettingValue(sqlite3* db, const char* key, const char* defaultValue)
{
    const char* sql = "SELECT value FROM settings WHERE key = ?";
    sqlite3_stmt* stmt;
    std::string value = defaultValue;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* text = (const char*)sqlite3_column_text(stmt, 0);
            if (text)
                value = text;
        }
        sqlite3_finalize(stmt);
    }
    return value;
}

bool GameDataManager::getBgmEnabled()
{
    return getSettingValue(_db.get(), "bgm_enabled", "1") == "1";
}

bool GameDataManager::getSfxEnabled()
{
    return getSettingValue(_db.get(), "sfx_enabled", "1") == "1";
}

float GameDataManager::getBgmVolume()
{
    auto ret = getSettingValue(_db.get(), "bgm_volume", "50.0");
    return std::stof(ret);
}

float GameDataManager::getSfxVolume()
{
    auto ret = getSettingValue(_db.get(), "sfx_volume", "50.0");
    return std::stof(ret);
}

int GameDataManager::getLevelStars(int levelType, int mapIndex)
{
    if (!_initialized)
        return 0;

    const char* sql = "SELECT stars FROM level_progress "
                      "WHERE level_type = ? AND map_index = ?";

    sqlite3_stmt* stmt;
    const int result = sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
        return 0;

    sqlite3_bind_int(stmt, 1, levelType);
    sqlite3_bind_int(stmt, 2, mapIndex);

    int stars = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stars = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return stars;
}

void GameDataManager::clearGameState(int levelType, int mapIndex)
{
    if (!_initialized)
        return;

    const char* sql = "DELETE FROM game_state "
                      "WHERE level_type = ? AND map_index = ?";

    sqlite3_stmt* stmt;
    int const result = sqlite3_prepare_v2(_db.get(), sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK)
        return;

    sqlite3_bind_int(stmt, 1, levelType);
    sqlite3_bind_int(stmt, 2, mapIndex);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void GameDataManager::cleanup()
{
    if (_db) {
        sqlite3_close(_db.get());
        _db = nullptr;
    }
    _initialized = false;
}