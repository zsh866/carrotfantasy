#pragma once
enum class LevelType {
    NONE = -1,
    SKY = 0, // �������
    DESERT = 1, // ɳĮ����
    FOREST = 2, // ɭ������
};

class SelectedMapInfo {
private:
    // ����ģʽ�����캯��˽��
    SelectedMapInfo() noexcept
        : _currentMapLevel(LevelType::NONE)
        , _currentMapIndex(0)
    {
    }
    LevelType _currentMapLevel; // ��ǰѡ��ĵ�ͼ�ȼ�
    int _currentMapIndex; // ��ǰѡ��ĵ�ͼ����
public:
    static SelectedMapInfo& getInstance() noexcept
    {
        static SelectedMapInfo instance;
        return instance;
    }

    // ���õ�ǰѡ��ĵ�ͼ�ȼ�
    void setmapLevel(LevelType level) noexcept
    {
        _currentMapLevel = level;
    }

    // ��ȡ��ǰѡ��ĵ�ͼ�ȼ�
    LevelType getMapLevel() const noexcept
    {
        return _currentMapLevel;
    }

    // ���õ�ǰѡ��ĵ�ͼ����
    void setMapIndex(int index) noexcept
    {
        _currentMapIndex = index;
    }

    // ��ȡ��ǰѡ��ĵ�ͼ����
    int getMapIndex() const noexcept
    {
        return _currentMapIndex;
    }

    SelectedMapInfo(const SelectedMapInfo&) = delete;
    SelectedMapInfo& operator=(const SelectedMapInfo&) = delete;
};