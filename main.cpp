#include <iostream>

template<class T>
class MyUnique
{
    T * p=nullptr;
public:
    MyUnique(T *p)
    {
        this->p = p;
    }

    ~MyUnique()
    {
        delete this->p;
    }

    MyUnique(const MyUnique& pointer) = delete;

    MyUnique(MyUnique&& other) noexcept
    {
        this->p = other.p;
        other.p = nullptr;
    }

    MyUnique& operator=(MyUnique&& other) noexcept
    {
        if (this != &other)
        {
            delete this->p;
            this->p = other.p;
            other.p = nullptr;
        }
        return *this;
    }

    MyUnique& operator=(const MyUnique&) = delete;

    T* get() const { return this->p; }
    T& operator*(){ return *(this->p); }
    T* operator->() { return this->p; }
};


template<class T>
class MyShared
{
    T * p=nullptr;
    int* copies = nullptr;
public:
    MyShared(T *p)
    {
        this->p = p;
        this->copies = new int(1);
    }

    ~MyShared()
    {
        if (--(*copies) == 0)
        {
            delete this->p;
            delete this->copies;
        }
    }

    MyShared(const MyShared& other)
    {
        this->p = other.p;
        this->copies = other.copies;
        ++(*copies);
    }

    MyShared(MyShared&& other) noexcept
    {
        this->p = other.p;
        this->copies = other.copies;
        other.p = nullptr;
        other.copies = nullptr;
    }

    MyShared& operator=(MyShared&& other) noexcept
    {
        if (this != &other)
        {
            if (copies && --(*copies) == 0)
            {
                delete p;
                delete copies;
            }
            p = other.p;
            copies = other.copies;
            other.p = nullptr;
            other.copies = nullptr;
        }
        return *this;
    }

    MyShared& operator=(const MyShared& other)
    {
        if (this != &other)
        {
            if (--(*copies) == 0)
            {
                delete p;
                delete copies;
            }
            p = other.p;
            copies = other.copies;
            ++(*copies);
        }
        return *this;
    }

    T* get() const { return this->p; }
    T& operator*(){ return *(this->p); }
    T* operator->() { return this->p; }
};

int main() {
    std::cout << "MyUnique test" << std::endl;
    MyUnique<int> unique1(new int(1830));
    std::cout << "unique1: " << *unique1 << std::endl;

    MyUnique<int*> unique_ptr(new int*(new int(2026)));
    std::cout << "unique_ptr: " << **unique_ptr << std::endl;

    std::cout << "move test" << std::endl;
    MyUnique<int> unique2 = std::move(unique1);
    std::cout << "unique2: " << *unique2 << std::endl;
    std::cout << "unique1: " << unique1.get() << std::endl;

    MyUnique<int> unique3(new int(1997));
    std::cout << "unique3: " << *unique3 << std::endl;
    unique3 = std::move(unique2);
    std::cout << "unique3 after move: " << *unique3 << std::endl;
    std::cout << "unique2: " << unique2.get() << std::endl;

    std::cout << "MyShared test" << std::endl;
    MyShared<int> shared1(new int(1111));
    std::cout << "shared1: " << *shared1 << std::endl;

    std::cout << "copy test" << std::endl;
    MyShared<int> shared2 = shared1;
    std::cout << "shared2: " << *shared2 << std::endl;

    MyShared<int> shared3(new int(3333));
    std::cout << "shared3: " << *shared3 << std::endl;
    shared3 = shared2;
    std::cout << "shared3 after copy: " << *shared3 << std::endl;

    std::cout << "shared1 = " << *shared1 << ", shared2 = " << *shared2 << ", shared3 = " << *shared3 << std::endl;

    std::cout << "move test" << std::endl;
    MyShared<int> shared4 = std::move(shared3);
    std::cout << "shared4: " << *shared4 << std::endl;
    std::cout << "shared3: " << shared3.get() << std::endl;

    MyShared<int> shared5(new int(123));
    MyShared<int> shared6(new int(456));
    std::cout << "shared5: " << *shared5 << std::endl;
    std::cout << "shared6: " << *shared6 << std::endl;

    shared6 = std::move(shared5);
    std::cout << "shared6 after move: " << *shared6 << std::endl;
    std::cout << "shared5: " << shared5.get() << std::endl;

    MyShared<int*> shared_ptr(new int*(new int(789)));
    std::cout << "shared_ptr: " << **shared_ptr << std::endl;

    return 0;
}