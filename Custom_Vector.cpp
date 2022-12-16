#include <iostream>
namespace STD{
template<typename T>
class Vector{
    template<typename U> friend std::ostream& operator<<(std::ostream& os, const Vector<U>& vec);
  public:
    Vector() = default;
    Vector(int size): size_(size) {
        Resize(size);
    }
    Vector(const std::initializer_list<T> lst) {
        size_ = lst.size();
        capacity_ = size_;
        data_ = new T[capacity_] {};
        int i = 0;
        for(const T& val: lst) {
            data_[i] = val;
            i++;
        }
    }
    Vector(const Vector& other) {
        data_ = new T[other.size_] {};
        for(int i = 0; i < other.size_; i++) {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
        capacity_ = other.capacity_;
    }
    Vector& operator=(const Vector& other) {
        if(&other == this) return *this;
        if(data_) delete [] data_;
        data_ = new T[other.capacity_] {};
        for(int i = 0; i < other.size_; i++){
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
        capacity_ = other.size_;
        return *this;
    }
    Vector(Vector&& other) {
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    Vector& operator=(Vector&& other) {
        if(&other == this) return *this;
        if(data_) delete [] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }
    ~Vector() {
        delete [] data_;
    }
    T& operator[](int id) {
        return data_[id];
    }
    const T& operator[](int id) const {
        return data_[id];
    }
    void Resize(int new_size) {
        if(new_size > capacity_) {
            capacity_ = 2*(new_size+1);
            T* tmp = new T[capacity_] {};
            if(data_) {
                for(int i = 0; i < size_; i++) {
                    tmp[i] = data_[i];
                }
                delete [] data_;
            }
            data_ = tmp;
            size_ = new_size;
            return;
        }
        if(new_size < size_) {
            for(int i = new_size; i < size_; i++) {
                data_[i] = T{};
            }
        }
        size_ = new_size;
    }
    void PushBack(const T& val) {
        int insert_id = size_;
        Resize(size_+1);
        data_[insert_id] = val;
    }
    void PushFront(const T& val) {
        int new_size = 2*(size_+1);
        T* tmp = new T[new_size] {};
        tmp[0] = val;
        if(data_) {
            for(int i = 0; i < size_; i++){
                tmp[i+1] = data_[i];
            }
            delete [] data_;
        }
        data_ = tmp;
        size_++;
        capacity_ = new_size;
    }
    T& Back() {
        return data_[size_-1];
    }
    const T& Back() const {
        return data_[size_-1];
    }
    T& Front() {
        return data_[0];
    }
    const T& Front() const {
        return data_[0];
    }
    bool Empty() const {
        return size_ == 0;
    }
    void Fill(const T& val) {
        for(int i = 0; i < size_; i++) {
            data_[i] = val;
        }
    }
    void Swap(Vector& other) {
        std::swap(other, *this);                // * 在C++11中，若待交換的類別有支援移動建構子以及移動賦值運算子(有實作)的話，
                                                //   std::swap()就會使用移動的方式，如果沒有時做上述兩者的話，則會觸發複製建
                                                //   構子以及複製賦值運算子，交換的結果會比較耗時。

        // std::swap(other.data_, data_);         // * 如果Vector沒有實作移動建構子以及移動賦值運算子時要採取此種寫法來移動內
        // std::swap(other.size_, size_);         //   部資源，避免std::swap去針對整個Vector做複製建構以及複製賦值運算，花費
        // std::swap(other.capacity_, capacity_); //   大量的時間。
    }
    class ConstIterator{
      public:
        ConstIterator(const T* ptr): ptr_(ptr) {}
        bool operator!=(const ConstIterator& other) {
            return ptr_ != other.ptr_;
        }
        const T& operator*() {
            return *ptr_;
        }
        const T* operator++(int) {
            return ptr_++;
        }
      private:
        const T* ptr_;
    };
    ConstIterator Begin() const {
        return {&data_[0]};
    }
    ConstIterator End() const {
        return {&data_[size_]};
    }
  private:
    int size_{};
    int capacity_{};
    T* data_{};
};

template<typename T>
typename Vector<T>::ConstIterator Begin(const Vector<T>& vec) {
    return vec.Begin();
}
template<typename T>
typename Vector<T>::ConstIterator End(const Vector<T>& vec) {
    return vec.End();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    os << "[";
    // for(int i = 0; i < vec.size_; i++) {
    //     if(i != 0) os << ", ";
    //     os << vec[i];
    // }
    for(typename Vector<T>::ConstIterator it = Begin(vec); it != End(vec); it++) {
        if(it != Begin(vec)) os << ", ";
        os << *it;
    }
    os << "]";
    return os;
}
}

template<typename T>
void Check(const STD::Vector<T>& v) {
    if(v.Empty()) std::cout << "Empty" << std::endl;
}

int main() {
    STD::Vector<int> v;
    v.PushFront(1);
    std::cout << v << std::endl;  // [1]
    v.Resize(5);
    v[3] = 7;
    std::cout << v << std::endl;  // [1, 0, 0, 7, 0]
    v.Resize(3);
    v.Resize(5);
    std::cout << v << std::endl;  // [1, 0, 0, 0, 0]
    v.PushBack(666);
    for(int i = 1; i <= 5; i++) {
        v.PushFront(i);
        std::cout << v << std::endl; // [5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 666]
    }
    STD::Vector<int> va(5);
    STD::Vector<int> vb = va;
    vb[2] = 3;
    vb.Front() = 10;
    vb.Back() = 100;
    std::cout << va << std::endl; // [0, 0, 0, 0, 0]
    std::cout << vb << std::endl; // [10, 0, 3, 0, 100]
    STD::Vector<int> vc;
    vc = std::move(vb);
    std::cout << vb << std::endl; // []
    Check(vb);                    // Empty
    std::cout << vc << std::endl; // [10, 0, 3, 0, 100]
    Check(vc);                    // 
    STD::Vector<int> vd {1, 2, 3, 4, 5};
    std::cout << vd << std::endl; // [1, 2, 3, 4, 5]
    vd.Resize(2);
    vd.Fill(1);
    vd.Resize(5);
    std::cout << vd << std::endl; // [1, 1, 0, 0, 0]
    vd.Fill(5);
    std::cout << vd << std::endl; // [5, 5, 5, 5, 5]
    vd.Swap(v);
    std::cout << vd << std::endl; // [5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 666]
    std::cout << v << std::endl;  // [5, 5, 5, 5, 5]
    vd.Swap(v);
    std::cout << vd << std::endl; // [5, 5, 5, 5, 5]
    std::cout << v << std::endl;  // [5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 666]
    return 0;
}