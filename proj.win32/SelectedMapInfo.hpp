#pragma once
enum class LevelType {
    NONE = -1,
    SKY = 0, // 天空主题
    DESERT = 1, // 沙漠主题
    FOREST = 2, // 森林主题
};

class SelectedMapInfo {
private:
    // 单例模式，构造函数私有
    SelectedMapInfo() noexcept
        : _currentMapLevel(LevelType::NONE)
        , _currentMapIndex(-1)
    {
    }
    LevelType _currentMapLevel; // 当前选择的地图等级
    int _currentMapIndex; // 当前选择的地图索引
public:
    static SelectedMapInfo& getInstance() noexcept
    {
        static SelectedMapInfo instance;
        return instance;
    }

    // 设置当前选择的地图等级
    void setmapLevel(LevelType level) noexcept
    {
        _currentMapLevel = level;
    }

    // 获取当前选择的地图等级
    LevelType getMapLevel() const noexcept
    {
        return _currentMapLevel;
    }

    // 设置当前选择的地图索引
    void setMapIndex(int index) noexcept
    {
        _currentMapIndex = index;
    }

    // 获取当前选择的地图索引
    int getMapIndex() const noexcept
    {
        return _currentMapIndex;
    }

    SelectedMapInfo(const SelectedMapInfo&) = delete;
    SelectedMapInfo& operator=(const SelectedMapInfo&) = delete;
};