#pragma once
#include <functional>
#include<vector>
class StateObserver {
public:
    using Observer = std::function<void(bool)>;

    void setBgmEnabled(bool enabled) {
        _bgmEnabled = enabled;
        for (const auto& observer : _bgmObservers) {
            observer(enabled);
        }
    }

    void setSfxEnabled(bool enabled) {
        _sfxEnabled = enabled;
        for (const auto& observer : _sfxObservers) {
            observer(enabled);
        }
    }

    bool getBgmEnabled() const noexcept{ return _bgmEnabled; }
    bool getSfxEnabled() const noexcept { return _sfxEnabled; }

    void addBgmObserver(Observer observer) {
        _bgmObservers.push_back(observer);
    }

    void addSfxObserver(Observer observer) {
        _sfxObservers.push_back(observer);
    }

private:
    bool _bgmEnabled{ true };
    bool _sfxEnabled{ true };
    std::vector<Observer> _bgmObservers;
    std::vector<Observer> _sfxObservers;
};