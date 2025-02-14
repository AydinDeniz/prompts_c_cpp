#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

template <typename T>
class MyVector {
private:
    T* data;
    size_t sz;
    size_t cap;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        std::copy(data, data + sz, new_data);
        delete[] data;
        data = new_data;
        cap = new_capacity;
    }

public:
    MyVector() : data(nullptr), sz(0), cap(0) {}

    ~MyVector() { delete[] data; }

    void push_back(const T& value) {
        if (sz == cap) {
            size_t new_capacity = (cap == 0) ? 1 : cap * 2;
            reallocate(new_capacity);
        }
        data[sz++] = value;
    }

    void pop_back() {
        if (sz == 0) throw std::out_of_range("pop_back() on empty vector");
        --sz;
    }

    size_t size() const { return sz; }

    size_t capacity() const { return cap; }

    bool empty() const { return sz == 0; }

    T& operator[](size_t index) {
        if (index >= sz) throw std::out_of_range("index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= sz) throw std::out_of_range("index out of range");
        return data[index];
    }

    T* begin() { return data; }

    T* end() { return data + sz; }

    const T* begin() const { return data; }

    const T* end() const { return data + sz; }
};

int main() {
    MyVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "Vector size: " << vec.size() << ", capacity: " << vec.capacity() << '\n';
    std::cout << "Elements: ";
    for (const auto& elem : vec) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    vec.pop_back();
    std::cout << "After pop_back, size: " << vec.size() << '\n';

    return 0;
}