#pragma once
#include <functional>
#include <vector>

template <typename T>
class Property {
public:
    using Observer = std::function<void(const T&)>;

    Property(T value = T())
        : _value(value)
    {
    }

    T get() const { return _value; }

    void set(const T& value)
    {
        if (_value != value) {
            _value = value;
            notify();
        }
    }

    Property& addObserver(Observer observer, bool notify = true)
    {
        _observers.push_back(observer);
        if (notify) {
            observer(_value); // 立即触发一次
        }
        return *this;
    }

private:
    T _value;
    std::vector<Observer> _observers;

    void notify()
    {
        for (const auto& observer : _observers) {
            observer(_value);
        }
    }
};