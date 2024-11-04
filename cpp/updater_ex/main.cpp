#include <algorithm>
#include <future> // For async tasks
#include <iostream>
#include <map>
#include <memory>
#include <mutex> // For mutex locking
#include <string>
#include <vector>

class Device; // Forward declaration

/*================================================================================*/

// Interface for UpdateStrategy
class UpdateStrategy
{
public:
    virtual void Update(std::shared_ptr<Device> device) = 0;
    virtual ~UpdateStrategy() = default;
};

/*--------------------------------------------------------------------------------*/

class Device
{
public:
    Device(const std::string &device_name)
        : name_(device_name)
        , updated_(false)
    {
    }

    // Set update strategy (from the Mediator)
    void SetUpdateStrategy(std::shared_ptr<UpdateStrategy> strategy)
    {
        update_strategy_ = strategy;
    }

    // Check if the device has already been updated
    bool IsUpdated() const
    {
        return updated_;
    }

    // Mark the device as updated
    void MarkUpdated()
    {
        updated_ = true;
    }

    // Device-specific implementation for mount-based update
    virtual void MountBasedUpdate()
    {
        std::cout << name_ << ": Performing device-specific MountBased update." << std::endl;
        // TODO: Add device-specific logic here
    }

    // Device-specific implementation for API-based update
    virtual void ApiBasedUpdate()
    {
        std::cout << name_ << ": Performing device-specific ApiBased update." << std::endl;
        // TODO: Add device-specific logic here
    }

    std::string GetName() const
    {
        return name_;
    }

private:
    std::string name_;
    std::shared_ptr<UpdateStrategy> update_strategy_; // Strategy is assigned externally
    bool updated_;                                    // Track if the device has already been updated
    mutable std::mutex device_mutex_;                 // Mutex to protect device state

public:
    // Mutex getter for external synchronization
    std::mutex &GetMutex() const
    {
        return device_mutex_;
    }
};

/*================================================================================*/

// Concrete Strategy: MountBased
class MountBasedUpdate : public UpdateStrategy
{
public:
    void Update(std::shared_ptr<Device> device) override
    {
        std::cout << "MountBased update process started." << std::endl;
        device->MountBasedUpdate(); // Call the device-specific update logic
    }
};

// Concrete Strategy: ApiBased
class ApiBasedUpdate : public UpdateStrategy
{
public:
    void Update(std::shared_ptr<Device> device) override
    {
        std::cout << "ApiBased update process started." << std::endl;
        device->ApiBasedUpdate(); // Call the device-specific update logic
    }
};

/*--------------------------------------------------------------------------------*/

// Concrete Device: ORU
class ORUDevice : public Device
{
public:
    ORUDevice()
        : Device("ORU")
    {
    }

    void MountBasedUpdate() override
    {
        std::cout << GetName() << ": Custom ORU mount-based update logic." << std::endl;
        // Custom logic for ORU device
    }

    void ApiBasedUpdate() override
    {
        std::cout << GetName() << ": Custom ORU API-based update logic." << std::endl;
        // Custom logic for ORU device
    }
};

// Concrete Device: CCU
class CCUDevice : public Device
{
public:
    CCUDevice()
        : Device("CCU")
    {
    }

    void MountBasedUpdate() override
    {
        std::cout << GetName() << ": Custom CCU mount-based update logic." << std::endl;
        // Custom logic for CCU device
    }

    void ApiBasedUpdate() override
    {
        std::cout << GetName() << ": Custom CCU API-based update logic." << std::endl;
        // Custom logic for CCU device
    }
};

/*================================================================================*/

class UpdateMediator
{
public:
    void AddDevice(std::shared_ptr<Device> device)
    {
        devices.push_back(device);
    }

    void SetDependency(std::shared_ptr<Device> from, std::shared_ptr<Device> to)
    {
        dependencies[from].push_back(to);
    }

    // Set global update strategy (MountBased or ApiBased)
    void SetUpdateStrategy(std::shared_ptr<UpdateStrategy> strategy)
    {
        update_strategy_ = strategy;
    }

    void UpdateDevice(std::shared_ptr<Device> device)
    {
        // Lock device's mutex before accessing or modifying its state
        std::scoped_lock lock(device->GetMutex());

        // Skip if the device has already been updated or is not in the list
        if (device->IsUpdated() || std::find(devices.begin(), devices.end(), device) == devices.end())
        {
            return;
        }

        // First, update dependent devices
        if (dependencies.find(device) != dependencies.end())
        {
            for (auto &dep : dependencies[device])
            {
                UpdateDevice(dep); // Recursively update dependencies
            }
        }

        // Update the current device using the global strategy
        update_strategy_->Update(device);

        // Mark the device as updated
        device->MarkUpdated();
    }

    void UpdateAll()
    {
        // Store future tasks for parallel execution
        std::vector<std::future<void>> futures;

        for (auto &device : devices)
        {
            futures.push_back(std::async(std::launch::async, [this, device]() {
                UpdateDevice(device); // Each device update is handled in a separate async task
            }));
        }

        // Wait for all updates to complete
        for (auto &future : futures)
        {
            future.get(); // Blocks until each async task is complete
        }
    }

private:
    std::vector<std::shared_ptr<Device>> devices;
    std::map<std::shared_ptr<Device>, std::vector<std::shared_ptr<Device>>> dependencies;
    std::shared_ptr<UpdateStrategy> update_strategy_;
};

/*================================================================================*/

int main()
{
    // Create specific devices
    auto oru = std::make_shared<ORUDevice>();
    auto ccu = std::make_shared<CCUDevice>();
    auto fvm = std::make_shared<Device>("FVM"); // FVM can use base class behavior
    auto eiu = std::make_shared<Device>("EIU"); // EIU can use base class behavior

    // Create mediator and add devices
    UpdateMediator update_mediator;
    update_mediator.AddDevice(oru);
    update_mediator.AddDevice(fvm);
    update_mediator.AddDevice(ccu);
    update_mediator.AddDevice(eiu);

    // Set up dependencies (e.g., FVM depends on CCU)
    update_mediator.SetDependency(oru, fvm);
    update_mediator.SetDependency(oru, ccu);
    update_mediator.SetDependency(oru, eiu);

    // Select and apply the global update strategy for this run
    update_mediator.SetUpdateStrategy(std::make_shared<MountBasedUpdate>());
    // OR
    // mediator.SetUpdateStrategy(std::make_shared<ApiBasedUpdate>());

    // Update all devices (with dependencies respected)
    update_mediator.UpdateAll();

    return 0;
}
