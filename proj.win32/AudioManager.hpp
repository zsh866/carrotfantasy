#pragma once
#include "GameDataManager.h"
#include <SimpleAudioEngine.h>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

class AudioManager {
public:
    // 禁止拷贝和赋值
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // 获取单例实例
    static AudioManager& getInstance() noexcept
    {
        static AudioManager instance;
        return instance;
    }

    // 初始化音频管理器
    bool initialize(const std::string& bgmPath = "")
    {
        if (!_audio)
            return false;

        if (!bgmPath.empty()) {
            _bgmPath = bgmPath;
            _audio->preloadBackgroundMusic(_bgmPath.c_str());
            CCLOG("initialize done: %s", _bgmPath.c_str());
        }
        return true;
    }
    void initializeWithConfig()
    {
        auto& gameData = GameDataManager::getInstance();
        setBackgroundMusicVolume(gameData.getBgmVolume());
        setEffectsVolume(gameData.getSfxVolume());

        if (gameData.getBgmEnabled()) {
            if (isBackgroundMusicPlaying()) {
                resumeBackgroundMusic();
            }
            else {
                playBackgroundMusic();
            }
        } else {
            pauseBackgroundMusic();
        }
    }

    // 预加载音效
    void preloadEffect(const std::string& name, const std::string& path)
    {
        if (_effectPaths.find(name) == _effectPaths.end()) {
            _effectPaths[name] = path;
            _audio->preloadEffect(path.c_str());
        }
    }

    // 播放背景音乐
    void playBackgroundMusic(bool loop = true)
    {
        CCLOG("playBackgroundMusic: %s", _bgmPath.c_str());
        if (!_bgmPath.empty()) {
            CCLOG("playBackgroundMusic");
            _audio->playBackgroundMusic(_bgmPath.c_str(), loop);
        }
    }

    // 播放音效
    unsigned int playEffect(const std::string& name, bool loop = false)
    {
        auto it = _effectPaths.find(name);
        if (it != _effectPaths.end()) {
            return _audio->playEffect(it->second.c_str(), loop);
        }
        return 0;
    }

    // 音量控制
    void setBackgroundMusicVolume(float volume)
    {
        _audio->setBackgroundMusicVolume(clamp(volume, 0.0f, 1.0f));
    }

    void setEffectsVolume(float volume)
    {
        _audio->setEffectsVolume(clamp(volume, 0.0f, 1.0f));
    }

    // 背景音乐控制
    void pauseBackgroundMusic() { _audio->pauseBackgroundMusic(); }
    void resumeBackgroundMusic() { _audio->resumeBackgroundMusic(); }
    void stopBackgroundMusic(bool releaseData = false) { _audio->stopBackgroundMusic(releaseData); }
    bool isBackgroundMusicPlaying() const { return _audio->isBackgroundMusicPlaying(); }
    void applyBackgroundMusicSettings()
    {
        auto& gameData = GameDataManager::getInstance();
        const bool enabled = gameData.getBgmEnabled();
        const float volume = gameData.getBgmVolume();

        setBackgroundMusicVolume(volume);
        if (enabled) {
            playBackgroundMusic();
        } else {
            stopBackgroundMusic(true);
        }
    }

    // 音效控制
    void pauseEffect(unsigned int soundId) { _audio->pauseEffect(soundId); }
    void resumeEffect(unsigned int soundId) { _audio->resumeEffect(soundId); }
    void stopEffect(unsigned int soundId) { _audio->stopEffect(soundId); }
    void pauseAllEffects() { _audio->pauseAllEffects(); }
    void resumeAllEffects() { _audio->resumeAllEffects(); }
    void stopAllEffects() { _audio->stopAllEffects(); }
    void applySoundEffectSettings()
    {
        auto& gameData = GameDataManager::getInstance();
        const bool enabled = gameData.getSfxEnabled();
        const float volume = gameData.getSfxVolume();

        setEffectsVolume(enabled ? volume : 0.0f);
    }

    // 资源管理
    void unloadEffect(const std::string& name)
    {
        auto it = _effectPaths.find(name);
        if (it != _effectPaths.end()) {
            _audio->unloadEffect(it->second.c_str());
            _effectPaths.erase(it);
        }
    }

    void unloadAllEffects()
    {
        // 先停止所有音效
        _audio->stopAllEffects();

        // 遍历所有音效并逐个卸载
        for (const auto& effect : _effectPaths) {
            _audio->unloadEffect(effect.second.c_str());
        }

        // 清空音效路径映射
        _effectPaths.clear();
    }

private:
    AudioManager()
        : _audio(CocosDenshion::SimpleAudioEngine::getInstance())
    {
    }
    ~AudioManager() { unloadAllEffects(); }

    CocosDenshion::SimpleAudioEngine* _audio;
    std::string _bgmPath;
    std::unordered_map<std::string, std::string> _effectPaths;
    static constexpr float clamp(float value, float min, float max)
    {
        return std::min(std::max(value, min), max);
    }
};