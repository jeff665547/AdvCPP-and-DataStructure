#include <iostream>
// * 類別天生就會有預設建構子、複製建構子、複製賦值運算子。
// * 有定義直接建構子的話，就不會有內建的預設建構子。
// * 陣列的長度(大小)在宣告定義(編譯完之後)時就決定了，後續
//   (執行時, e.g. 隨機抽籤)不能再做更動。
// * 在STL的array中，at成員函式和operator[]之間的區別在於，
//   at成員函式每次都會去檢查input parameter的值使否合理，
//   也就是說id是否超過陣列的大小範圍。如果超過陣列物件邊界，
//   operator[]不會有任何的反應(導致未定義行為)，但是at成員
//   函式則會馬上丟出例外(exception)，並強制終止程式，也因此
//   使用at成員函式，它的效率會較operator[]來的差，但較安全。
// * using的用途
//   * 在類別中替型態取別名 (早期的C++是使用typedef)，同時也提供
//     型別查詢的功能，如:using ConstReference = const T&;
//     在類別中替型態取別名，此別名就會成為此類別的成員，若需要在
//     類別模板外面存取此類別成員型態別名，除了要寫清楚類別的名稱範圍
//     (XXX<xx>::)外，還需要在類別名稱前方加上typename，
//     (typename XXX<xx>::Alias)才能使用在該類別中的型態別名，
//     某種程度來說，typename是去告訴編譯器後面的那一串是一種型別，
//     而不是其它種可能(因為編譯器在有模板時會不清楚後面那串是甚麼東
//     西)，e.g. 模板裡的變數、模板裡的成員函式之類的。
template<typename T, int N>
class Array {
  public:
    T& operator[] (int id) {
        return elem[id];
    }
    const T& operator[] (int id) const {
        return elem[id];
    }
    T& Front() {
        return elem[0];
    }
    T& Back() {
        return elem[N-1];
     }
    T Size() const {
        return N;
    }
    T elem[N];
};

int main() {
    Array<int, 5> a {5, 4, 3, 2, 1};  // * 公開的成員可以直接被大括弧{}給初始化，意即在這裡
                                      //   使用{5, 4, 3, 2, 1}來初始化elem，理論上應該要寫成
                                      //   {{5, 4, 3, 2, 1}}較為合理，但C++規定可以省略外面
                                      //   的大括弧(在初始化內容為陣列的情況下)。
    const Array<int, 5> b = a;   // * 透過自定義類別將陣列給包裝起來，因為自定義類別物件可以被複製，
                                 //   當物件被複製時也就順便複製物件內的成員(陣列)了(間接複製陣列)。
    // * b.elem = a.elem -> compile error. 陣列不能直接複製。

    a[0] = 9;  // a.operator[](0) = 9;  =>  a.elem[0] = 9;
    a.Front() = 3;

    for(int i = 0; i < b.Size(); i++) {
        std::cout << b[i] << std::endl;   // 5, 4, 3, 2, 1
    }

    return 0;
}