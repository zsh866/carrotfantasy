#pragma once
#include "cocos2d.h"
#include <sqlite3.h>

class GameDataManager {
private:
    struct SQLiteDeleter {
        void operator()(sqlite3* db)
        {
            if (db){
                sqlite3_close(db);
                CCLOG("Database closed");
            }
        }
    };
    std::unique_ptr<sqlite3, SQLiteDeleter> _db;
    bool _initialized { false };

    GameDataManager() = default;

public:
    static GameDataManager& getInstance() noexcept
    {
        static GameDataManager instance;
        return instance;
    }

    // 初始化数据库
    bool init();

    // 关卡数据相关
    bool saveLevelProgress(int levelType, int mapIndex, int stars);
    int getLevelStars(int levelType, int mapIndex);
    bool isLevelUnlocked(int levelType, int mapIndex);

    // 游戏进度保存与恢复
    bool saveGameState(int levelType, int mapIndex, const std::string& gameState);
    std::string loadGameState(int levelType, int mapIndex);
    void clearGameState(int levelType, int mapIndex);

    // 音频设置
    bool saveSoundSettings(bool bgmEnabled, bool sfxEnabled, float bgmVolume, float sfxVolume);
    bool getBgmEnabled();
    bool getSfxEnabled();
    float getBgmVolume();
    float getSfxVolume();

    // 清理资源
    void cleanup();

    // 禁止拷贝
    GameDataManager(const GameDataManager&) = delete;
    GameDataManager& operator=(const GameDataManager&) = delete;

    // 析构函数
    ~GameDataManager() = default;
};