#pragma once
#include "cocos2d.h"
#include <sqlite3.h>

class GameDataManager {
private:
    static GameDataManager* _instance;
    sqlite3* _db;
    bool _initialized;

    GameDataManager()
        : _db(nullptr)
        , _initialized(false)
    {
    }

public:
    static GameDataManager& getInstance()
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
};