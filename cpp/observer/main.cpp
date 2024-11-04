#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Observer
{
public:
    Observer() = default;
    ~Observer() = default;
    virtual void Update(float temperature, float humidity, int pressure) = 0;
};

class Subject
{
public:
    Subject() = default;
    virtual ~Subject() = default;

    virtual void RegisterObserver(Observer *observer) = 0;
    virtual void RemoveObserver(Observer *observer) = 0;
    virtual void NotifyObserver() = 0;
};

class WeatherData : public Subject
{
public:
    WeatherData()
        : humidity_(0.0f)
        , pressure_(0)
        , temperature_(0.0f)
    {
    }
    ~WeatherData() override = default;

    void SetMeasureChanged(float humidity, int pressure, float temperature)
    {
        humidity_ = humidity;
        pressure_ = pressure;
        temperature_ = temperature;
        NotifyObserver();
    }

    void RegisterObserver(Observer *observer) override
    {
        observers_.push_back(observer);
    }

    void RemoveObserver(Observer *observer) override
    {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }

private:
    void NotifyObserver() override
    {
        for (const auto &observer : observers_)
        {
            observer->Update(temperature_, humidity_, pressure_);
        }
    }

    float humidity_;
    int pressure_;
    float temperature_;
    std::vector<Observer *> observers_;
};

class ConditionObserver : public Observer
{
public:
    ConditionObserver(std::shared_ptr<WeatherData> weatherData)
        : weather_data_(weatherData)
        , temperature_(0.0f)
        , humidity_(0.0f)
        , pressure_(0)
    {
        weather_data_->RegisterObserver(this);
    }
    ~ConditionObserver() = default;

    void Update(float temperature, float humidity, int pressure) override
    {
        temperature_ = temperature;
        humidity_ = humidity;
        pressure_ = pressure;
        Display();
    }
    void Display() const
    {
        std::cout << "현재 압력 : " << pressure_ << "\n";
        std::cout << "현재 온도 : " << temperature_ << "\n";
        std::cout << "현재 습도 : " << humidity_ << "\n";
    }

private:
    std::shared_ptr<WeatherData> weather_data_;
    float temperature_;
    float humidity_;
    int pressure_;
};

int main()
{
    auto weather = std::make_shared<WeatherData>();
    auto condition = std::make_unique<ConditionObserver>(weather);
    auto condition2 = std::make_unique<ConditionObserver>(weather);

    weather->RegisterObserver(condition.get());
    weather->SetMeasureChanged(7.0f, 2, 15.0f);

    std::cout << "Observer 삭제" << std::endl;
    weather->RemoveObserver(condition.get());

    weather->SetMeasureChanged(5.0f, 1, 13.0f);

    std::cout << "Observer 추가" << std::endl;
    condition.reset(std::make_unique<ConditionObserver>(weather).release());

    weather->SetMeasureChanged(5.0f, 1, 13.0f);

    return 0;
}