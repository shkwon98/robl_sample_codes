#include <iostream>
#include <memory>

class Person
{
public:
    Person(const std::string &name, int age)
        : name_(name)
        , age_(age)
    {
    }

    std::string GetProfile() const
    {
        return "name = " + name_ + ", age = " + std::to_string(age_);
    }

private:
    std::string name_;
    int age_;
};

class PersonProfile
{
public:
    PersonProfile(const Person *person_ptr)
        : person_ptr_(person_ptr)
    {
    }

    ~PersonProfile()
    {
        if (person_ptr_)
        {
            std::cout << person_ptr_->GetProfile() << std::endl;
        }
    }

private:
    const Person *person_ptr_;
};

int main(void)
{
    Person *person_ptr = nullptr;
    {
        auto person = Person("John", 30);
        person_ptr = &person;
    }

    {
        PersonProfile profile(person_ptr);
    }

    // person = Person("Alice", 20);
    // {
    //     PersonProfile profile(&person);
    //     person = Person("Bob", 25);
    //     person = Person("Charlie", 35);
    //     {
    //         person = Person("David", 40);
    //     }
    // }

    return 0;
}
