#include <any>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <typeindex>
#include <unordered_map>

template <typename M>
using Callback = std::function<void(M)>;
template <typename M>
using Condition = std::function<bool(M)>;

class BaseExecutor
{
public:
    virtual ~BaseExecutor() = default;
    virtual void Execute(const std::any &message) const = 0;
};

template <typename M>
class CallbackExecutor : public BaseExecutor
{
public:
    CallbackExecutor(Callback<M> cb, Condition<M> cond)
        : callback_(cb)
        , condition_(cond)
    {
    }

    void Execute(const std::any &message) const override
    {
        try
        {
            // Try to cast to the expected type
            M value = static_cast<M>(std::any_cast<M>(message));
            if (condition_(value))
            {
                callback_(value);
            }
        }
        catch (const std::bad_any_cast &)
        {
            // Do nothing
        }
    }

private:
    Callback<M> callback_;
    Condition<M> condition_;
};

class MidListener
{
public:
    template <typename M>
    auto RegisterCallback(uint32_t mid, Condition<M> condition, Callback<M> callback) noexcept
    {
        const auto key = mid;
        auto callback_executor = std::make_shared<CallbackExecutor<M>>(std::move(callback), std::move(condition));

        std::scoped_lock lock(mutex_);
        return callback_multimap_.emplace(key, std::move(callback_executor));
    }

    void CancelCallback(std::unordered_multimap<uint32_t, std::shared_ptr<BaseExecutor>>::iterator it) noexcept
    {
        std::scoped_lock lock(mutex_);
        if (it != callback_multimap_.end())
        {
            callback_multimap_.erase(it);
        }
    }

    void Notify(uint32_t mid, int message) noexcept
    {
        std::scoped_lock lock(mutex_);
        auto range = callback_multimap_.equal_range(mid);
        for (auto it = range.first; it != range.second; ++it)
        {
            auto callback_executor = it->second;
            if (callback_executor)
            {
                [[maybe_unused]] auto future =
                    std::async(std::launch::async, [callback_executor, message]() { callback_executor->Execute(message); });
            }
        }
    }

private:
    std::mutex mutex_;
    std::unordered_multimap<uint32_t, std::shared_ptr<BaseExecutor>> callback_multimap_;
};

int main()
{
    MidListener listener;

    auto condition_0 = [](int msg) { return static_cast<int>(msg) == 1; };
    auto condition_1 = [](int msg) { return static_cast<float>(msg) == 1.0f; };
    auto callback = [](int msg) { std::cout << static_cast<int>(msg) << std::endl; };

    auto it_0 = listener.RegisterCallback<int>(1, condition_0, callback);
    auto it_1 = listener.RegisterCallback<int>(1, condition_1, callback);

    listener.Notify(1, int(1));

    listener.CancelCallback(it_0);
    listener.CancelCallback(it_1);

    return 0;
}
