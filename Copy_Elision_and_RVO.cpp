/* Copy Elision 複製省略 & RVO */
// * 為了增加效率，從C++11開始，明確定義了在某些條件下
//   複製or移動建構子的呼叫會被強制(保證)省略(也不需要
//   有定義)。主要有：
//   1. 在回傳時建構： T f() {return T{};}   又稱URVO (Unnamed Return Value 
//                                          Optimization) 為回傳同型態的匿名物件。
//   2. 暫時物件建構： T var = T{T{}};  T var = T{};  var的初始值為同類型(T)的暫時物件。
// * 複製省略有兩個方向，一個是C++標準規定的省略，另一個
//   則是編譯器在經過分析程式碼以後決定去省略，後者又稱為
//   RVO (Return Value Optimization)，要關掉後者，需要
//   在編譯時加入--no-elide-constructors的參數。
// * RVO 有兩者，分別為URVO(Unnamed Return Value Optimization)以及NRVO(Named Return 
//   Value Optimization)，即T f() { T t; return t;}。在C++17以後，URVO保證會複製省略
//   (而且是在不需要定義移動&複製建構子的情況下)，但NRVO只有在部分情況會有複製省略發生，
//   如：T f() { T t; return t;} (需要在有定義移動or複製建構子的情況下)，
//   而 T f(T par) { return par; } (當回傳的物件為函式吃進來的參數時) 這樣的格式則不
//   會發生複製省略。針對NRVO的複雜情況，為了確保良好的運行效率，實作移動建構子(moving 
//   constructor)以及移動賦值運算子(move assignment operator)能優化程式碼，因為在沒有
//   其他特殊的優化情況下，Compiler在函式回傳時會試著優先使用移動建構子來取代複製建構子。
#include <iostream>

template<typename T>
class Vector {
public:
    using ValueType = T;
    using Reference = T&;
    using ConstReference = const T&;
    using SizeType = size_t;
    
    Vector() = default;
    explicit Vector(SizeType n) {    // 備註: 加上 explicit 避免發生隱性轉型
        elems_ = new ValueType[n]{}; // 備註: {} 預設會歸零(依據ValueType)
        size_ = n;
    }
    ~Vector() {
        delete[] elems_;
    }
    Vector(const Vector& rhs) {
        std::cout << "Copy constructor" << std::endl;
        elems_ = new ValueType[rhs.size_];
        size_ = rhs.size_;
        for (SizeType i = 0; i < size_; i++) {
            elems_[i] = rhs.elems_[i];
        }
    }
    Vector& operator=(const Vector& rhs) {
        if (this == &rhs) return *this;   // * 當自己賦值給自己時不處理直接回傳
        delete[] elems_;                  // * 把舊有的管理物件刪掉為了避免記憶體洩漏
        elems_ = new ValueType[rhs.size_];
        size_ = rhs.size_;
        for (SizeType i = 0; i < size_; i++) {
            elems_[i] = rhs.elems_[i];
        }
        return *this;
    }    
    void Resize(SizeType n) {
         if (n <= capacity_) {
            for (SizeType i = size_; i < n; i++) {
                elems_[i] = T{};
            }
            size_ = n;
            return;
        }
        
        capacity_ = 2 * (capacity_+1);
        
        ValueType* newElems = new ValueType[capacity_]{};
        for (SizeType i = 0; i < size_; i++) {
            newElems[i] = elems_[i];
        }
        delete[] elems_;
        elems_ = newElems;
        size_ = n;        
    }  
    void PushBack(ConstReference val) {
        Resize(Size()+1);
        (*this)[Size()-1] = val;
    }
    Reference operator[](SizeType n) {
        return elems_[n];
    }      
    ConstReference operator[](SizeType n) const {
        return elems_[n];
    }  
    SizeType Size() const {
        return size_;
    }  
    SizeType Capacity() const {
        return capacity_;
    }
private:
    SizeType size_ = 0;
    SizeType capacity_ = 0;
    ValueType* elems_ = nullptr;
};
// * 在此例子中，就語意而言，CreateRange函式中的v會先
//   複製一次到該函式的回傳值，變成暫時物件。接著，在
//   main中的v便會拿該函式的回傳值(暫時物件)進行複製建構，
//   也就是說語意理論上是複製了兩次。但是，因為C++的版本
//   標準(C++14以後)以及C++編譯器的RVO機制，使得編譯完後，
//   編譯器會自動把函式中的v視為是main中的v，並且直接將函
//   式裡對v的操作直接作用於main中的v，也因此實際執行時，
//   並不會進行任何的複製。
Vector<int> CreateRange(int n) {
    Vector<int> v;
    for (int i = 1; i <= n; i++) {
        v.PushBack(i);
    }
    // * NRVO (Either the copy constructor or move constructor 
    //         is needed to trigger the copy elision).
    return v;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
    if (v.Size() == 0) return os << "[]";
    os << '[' << v[0];
    for (typename Vector<T>::SizeType i = 1; i < v.Size(); i++) {
        os << ", " << v[i];
    }
    return os << ']';
}

int main() {
    Vector<int> v = CreateRange(5);   // * C++17 spec.
    std::cout << v << std::endl;
}
