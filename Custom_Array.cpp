#include <iostream>
namespace STD{
template<typename T, int N>
class Array{
    template<typename U, int UN> friend std::ostream& operator<< (std::ostream& os, const Array<U, UN>& Arr);
  public:
    Array() = default;
    Array(const std::initializer_list<T> lst) {
        int i = 0;
        for(auto& val: lst) {
            arr[i] = val;
            i++;
        }
    }
    Array(const Array& other) {
        for(int i = 0; i < N; i++) {
            arr[i] = other.arr[i];
        }
    }
    Array& operator=(const Array& other) {
        if(&other == this) return *this;
        for(int i = 0; i < N; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }
    Array(Array&& other) {
        for(int i = 0; i < N; i++) {
            arr[i] = std::move(other.arr[i]);
        }
    }
    Array& operator=(Array&& other) {
        if(&other == this) return *this;
        for(int i = 0; i < N; i++) {
            arr[i] = std::move(other.arr[i]);
        }
        return *this;
    }
    ~Array() = default;
    T& operator[] (int id) {
        return arr[id];
    }
    const T& operator[] (int id) const {
        return arr[id];
    }
    T& At(int id) {
        if(id >= N) throw std::out_of_range("Array Index Out of Bound");
        return arr[id];
    }
    const T& At(int id) const {
        if(id >= N) throw std::out_of_range("Array Index Out of Bound");
        return arr[id];
    }
    T& Front() {
        return arr[0];
    }
    T& Back() {
        return arr[N-1];
    }
    void Fill(const T& val) {
        for(int i = 0; i < N; i++) {
            arr[i] = val;
        }
    }
    void Swap(Array& other) {
        for(int i = 0; i < N; i++) {
            std::swap(arr[i], other.arr[i]);
        }
    }
    int Size() const {
        return N;
    }
    class Iterator {
      public:
        Iterator(T* ptr): ptr_(ptr) {}
        bool operator!=(const Iterator& other) const {
            return ptr_ != other.ptr_;
        }
        Iterator operator++(int) {
            return ptr_++;
        }
        const T& operator*() const {
            return *ptr_;
        }
      private:
        T* ptr_;
    };
    class ConstIterator {
      public:
        ConstIterator(const T* ptr): ptr_(ptr) {}
        bool operator!=(const ConstIterator& other) const {
            return ptr_ != other.ptr_;
        }
        ConstIterator operator++(int) {
            return ptr_++;  // * 因為const T* ptr的意思是有一個名字叫ptr(向右沒有東西，在向左碰到*代表指標)的指標(在向左碰到const T)，
                            //   指向const T，因此ptr指向的T物件不能被更改，但是ptr本身可以指向另一個不能被更動的T物件，因此ptr++是可以的。
        }
        const T& operator*() const {
            return *ptr_;
        }
      private:
        const T* ptr_;
    };
    ConstIterator Begin() const {
        return {&arr[0]};
    }
    Iterator Begin() {
        return {&arr[0]};
    }
    ConstIterator End() const {
        return {&arr[N]};
    }
    Iterator End() {
        return {&arr[N]};
    }
  private:
    T arr[N];
};
template<typename Container>
const typename Container::ConstIterator Begin(const Container& con) {
    return con.Begin();
}
template<typename Container>
typename Container::Iterator Begin(Container& con) {
    return con.Begin();
}
template<typename Container>
const typename Container::ConstIterator End(const Container& con) {
    return con.End();
}
template<typename Container>
typename Container::Iterator End(Container& con) {
    return con.End();
}
template<typename T, int N>
std::ostream& operator<< (std::ostream& os, const Array<T, N>& Arr) {
    os << "[";
    for(typename Array<T, N>::ConstIterator it = Begin(Arr); it != End(Arr); it++) {
        if(it != Begin(Arr)) {
            os << ", ";
        }
        os << *it;
    }
    os << "]";
    return os;
} // (*) Why should this define in the namespace? How std::cout << b call this function? (::operator<<, STD::operator<<)

}

int main() {
    STD::Array<int, 5> a {5, 4, 3, 2, 1};
    STD::Array<int, 5> b = a;
    const STD::Array<std::string, 5> c {"I", "am", "not", "movable", "!"};
    STD::Array<std::string, 5> d {"Wow", "I", "told", "you", "guys!"};
    a[0] = 9;
    for(int i = 0; i < a.Size(); i++) {
        std::cout << a[i] << " ";  // 9, 4, 3, 2, 1
    }
    std::cout << std::endl;
    a.Front() = 3;
    std::cout << a.Back() << std::endl;
    for(int i = 0; i < b.Size(); i++) {
        std::cout << b[i] << " ";  // 5, 4, 3, 2, 1
    }
    std::cout << std::endl;
    for(int i = 0; i < a.Size(); i++) {
        std::cout << a[i] << " ";  // 3, 4, 3, 2, 1
    }
    std::cout << std::endl;
    a.Fill(6);
    std::cout << a << std::endl;   // [6, 6, 6, 6, 6]
    a.Swap(b);
    std::cout << a << std::endl;   // [5, 4, 3, 2, 1]
    std::cout << b << std::endl;   // [6, 6, 6, 6, 6]
    STD::Array<std::string, 5> e = std::move(d);
    std::cout << d << std::endl;   // [, , , , ]
    d = std::move(c);
    std::cout << c << std::endl;   // [I, am, not, movable, !]
    std::cout << d << std::endl;   // [I, am, not, movable, !]
    std::cout << e << std::endl;   // [Wow, I, told, you, guys!]

    // * No need the "typename" keyword in the following for-loop, since 
    //   T and N are known to be std::string and 5 respectively, and the 
    //   compiler knows the full definition of STD::Array<std::string, 5> 
    //   (so the compiler knows the Iterator is a member type of the given
    //   class), which is not a class template.
    for(STD::Array<std::string, 5>::Iterator it = STD::Begin(e); 
        it != STD::End(e); 
        it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}