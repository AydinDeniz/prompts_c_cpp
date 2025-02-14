#include <iostream>

template <typename T>
class SmartPointer {
private:
    T* ptr;
    unsigned int* refCount;

    void remove() {
        if (--(*refCount) == 0) {
            delete ptr;
            delete refCount;
            ptr = nullptr;
            refCount = nullptr;
        }
    }

public:
    SmartPointer() : ptr(nullptr), refCount(new unsigned int(0)) {}

    explicit SmartPointer(T* p) : ptr(p), refCount(new unsigned int(1)) {}

    SmartPointer(const SmartPointer<T>& sp) {
        ptr = sp.ptr;
        refCount = sp.refCount;
        ++(*refCount);
    }

    SmartPointer<T>& operator=(const SmartPointer<T>& sp) {
        if (this != &sp) {
            remove();
            ptr = sp.ptr;
            refCount = sp.refCount;
            ++(*refCount);
        }
        return *this;
    }

    T& operator*() {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    ~SmartPointer() {
        remove();
    }

    unsigned int use_count() const {
        return *refCount;
    }
};

class Resource {
public:
    Resource() {
        std::cout << "Resource acquired\n";
    }
    ~Resource() {
        std::cout << "Resource destroyed\n";
    }
    void greet() {
        std::cout << "Hello from Resource\n";
    }
};

int main() {
    {
        SmartPointer<Resource> sp1(new Resource());
        std::cout << "Reference Count: " << sp1.use_count() << '\n';

        {
            SmartPointer<Resource> sp2 = sp1;
            std::cout << "Reference Count: " << sp1.use_count() << '\n';
            sp2->greet();
        }

        std::cout << "Reference Count after sp2 out of scope: " << sp1.use_count() << '\n';
    }
    std::cout << "End of main\n";

    return 0;
}