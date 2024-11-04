#include <iostream>
#include <memory>
#include <string>
#include <vector>

class UpdateMediator
{
public:
    virtual bool IsUpdatable() = 0;
    virtual void SetUpdateAvailability(bool status) = 0;
};

class UpdateControlTower : public UpdateMediator
{
public:
    bool IsUpdatable() override
    {
        return is_updatable_;
    }

    void SetUpdateAvailability(bool status) override
    {
        is_updatable_ = status;
    }

private:
    bool is_updatable_ = true;
};

class UpdateAgent
{
public:
    UpdateAgent(std::shared_ptr<UpdateMediator> mediator, std::string name)
        : mediator_(mediator)
        , name_(name)
    {
    }

    void Update()
    {
        if (mediator_->IsUpdatable())
        {
            std::cout << "UpdateAgent " << name_ << " is updating" << std::endl;
            mediator_->SetUpdateAvailability(false);
        }
        else
        {
            std::cout << "UpdateAgent " << name_ << " is waiting for update" << std::endl;
        }
    }

    void Clear()
    {
        std::cout << "UpdateAgent " << name_ << " is cleared" << std::endl;
        mediator_->SetUpdateAvailability(true);
    }

private:
    std::shared_ptr<UpdateMediator> mediator_;
    std::string name_;
};

int main()
{
    auto control_tower = std::make_shared<UpdateControlTower>();
    UpdateAgent agent1(control_tower, "Agent1");
    UpdateAgent agent2(control_tower, "Agent2");

    agent1.Update();
    agent2.Update();
    agent1.Clear();
    agent2.Update();

    return 0;
}