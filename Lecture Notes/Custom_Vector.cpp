/* 動態陣列 */
// * 因為C風格陣列以及C++中的Array他們的大小一定要在編譯完之後就確定了。因此，
//   若想在執行期去(動態地)變更動態陣列的大小(生成C風格陣列)，在C++中必須得透
//   過"new[]"運算子(在C中是使用malloc())去動態配置C風格陣列物件(i.e.在
//   執行時再決定C風格陣列物件的大小)，並且該運算子會回傳一個(沒有名字的)陣列
//   的第一個元素的記憶體位址，讓擁有該記憶體位址的變數(指標)使用起來如同回傳
//   的陣列一樣，要注意的是，這裡的變數不是該陣列的名字，變數只是存著一個記憶
//   體位址而已。
// * 使用"new[]"運算子所產生出來的物件，因為其沒有名字，所以在離開可視範圍時
//   不會自動回收，因此必須要使用"delete[]"運算子來回收該物件，也就是說要使用
//   "delete[]"運算子來刪除動態配置(new[])的C風格陣列物件，否則會發生記憶體
//   洩漏(memory leak)，進而消耗記憶體空間。
// * new[] & delete[] example:
//   int* data = new int[size] {};  // * 這邊的data儲存的是new所產生的無名
//                                  //   C風格陣列物件的第一個元素記憶體位址。
//                                  //   int代表的是陣列物件的元素型態。
//                                  //   size代表陣列大小，為執行期的變數。
//                                  //   {}代表陣列中的元素預設會歸零(依據陣
//                                  //   列物件的元素型態)，{}不一定要有，如
//                                  //   果沒有{}代表不會去預設陣列中的元素值。
//   delete[] data;   // * 這邊的delete意思是去刪除data指標所指向的無名陣列
//                    //   物件(意即data指標仍然指向同一個記憶體位址，但是原本
//                    //   住在該記憶體位址的無名陣列已經被刪除了)。至於data本
//                    //   身這個指標物件，因為其有名字，所以它在程式結束時會自
//                    //   動回收。
// * 以動態的方式(在執行期)讓陣列變大：
//   1. 需要變大的時候產生一個較大的新陣列(new[])。
//   2. 將舊有的資料複製到新的陣列(複製完後新陣列有多餘的位址要歸零，或是一開始
//                               就將新陣列的元素全部先歸零)。
//   3. 刪除舊的C風格陣列(delete[])。
//   4. 用新陣列取代舊陣列(指向新陣列)。
// * 要使用 RAII 管理new[]以及delete[]，有new[]就要有delete[]，自行管理，RAII
//   (Resource Acquisition Is Initialization)的策略是在建構的時候配置資源(e.g. 
//   new[])，並在解構時去釋放建構時所配置的資源(e.g. delete[])，但要注意大五法則
//   (The rule of five)的問題(e.g. 在複製建構Vector與解構Vector時會不會造成
//   Vector資料成員重複刪除的問題 => 未定義行為)。
// * 大五法則(Rule of Five): 以下五種特殊函式如果其中一個被明確定義行為(客製化)
//   則其他的通常也都要(客製化)實作：
//   1. 解構子: ~T() (e.g. 寫了解構子以後就不能用以下函式的預設版本，需再額外
//                        自行定義(e.g. 強制刪除(= delete;))，否則會出現錯誤。)
//   2. 複製建構子: T(const T&)
//   3. 複製賦值運算子: T& operator=(const T&) 
//   4. 移動建構子: T(T&&)
//   5. 移動賦值運算子: T& operator=(T&&) 
// * 運算複雜度
//  * 在陣列的尾端插入元素、存取陣列元素(operator[] -> 只是做位址的+-*/計算)，
//    運算時間會與陣列大小無關。 => O(1)
//  * 對陣列做複製、在陣列的開頭端插入元素(push_front())，運算時間會與陣列大小成正比。 => O(n)
#include <iostream>
template<typename T>
class Vector{
  public:
    using ConstReference = const T&;
    using SizeType = size_t;
    Vector() = default;
    Vector(SizeType n) {
        data_ = new T[n] {};
        size_ = n;
    }
    Vector(const Vector& rhs) {  // * 客製化複製建構子，這邊採取的策略是實際地複製另一份陣列。
                                 //   另一種策略是單純複製data_指標儲存的位址，這樣的話結果
                                 //   是會去和rhs共享內部的C風格陣列，如果採用此種策略要注意
                                 //   解構子的實作，最後解構的物件才能去delete[]內部的C風格
                                 //   陣列。否則會發生重複delete[]內部的C風格陣列，進而發生
                                 //   未定義行為。另一個方法則是採用智慧指標來管理內部的C風
                                 //   格陣列。
        data_ = new T[rhs.size_] {};
        size_ = rhs.size_;
        for(SizeType i = 0; i < size_; i++) {
            data_[i] = rhs.data_[i];
        }
    }
    Vector& operator= (const Vector& rhs) {  // * 客製化複製賦值運算子，注意需先刪除data_
                                             //   原來指向的C風格陣列，再配置新的C風格陣列。
                                             //   否則會產生memory leak。        
        if(this == &rhs) return *this;       // * 注意：需要考慮到自我複製賦值的情況，
                                             //   如：a = a; 若遇到此狀況，不處理，直接回傳。
                                             //   如果沒有檢查此條件會使得a物件會去複製一個
                                             //   被刪除成員資料的自己。
        delete[] data_;                      // * 避免memory leak的問題。
        data_ = new T[rhs.size_] {};
        size_ = rhs.size_;
        for(SizeType i = 0; i < size_; i++) {
            data_[i] = rhs.data_[i];
        }
        return *this;
    }
    Vector(Vector&& rhs) {
        data_ = rhs.data_;
        rhs.data_ = nullptr;
        size_ = rhs.size;

    }
    Vector& operator= (Vector&& rhs) {

    }
    ~Vector() {          // * 自定義解構子，啟動大五法則。
        delete[] data_;  // * data_指標所指向的陣列因為是使用new[]產生的，必須要自行手動刪除。
                         //   然而data_這個指標物件本身則會和Vector類別物件的生命週期一樣，一
                         //   起生死。
    }
    void Resize(SizeType n) {
        // * 策略：
        //   1. 如果新的n(size)大小比現有陣列容量capacity小的話，不需要重新配置記憶體。
        //   2. 如果現有陣列容量capacity已經滿了而且還需要新增元素的話，就要重新配置記憶體，
        //      並且給大一點的陣列(通常是原本的capacity x 2)，降低重新配置記憶體的機會，
        //      才不會動不動一直在複製，進而影響程式執行效率。
        if(n <= capacity_) {
            for(SizeType i = size_; i < n; i++) {
                data_[i] = T{};    // * 若Resize是由小變大，則將新位址的值歸零(依據元素型別)。
            }
            size_ = n;
            return;
        }

        capacity_ = 2 * (capacity_+1);     // * 這邊要加1的原因是可能容量一開始是0，0*2還是0。

        T* tmpdata_ = new T[capacity_] {}; // * {}代表將新位址的值歸零(依據元素型別)。
        for(SizeType i = 0; i < size_; i++) {
            tmpdata_[i] = data_[i];
        }
        delete[] data_;
        data_ = tmpdata_;
        size_ = n;
    };                             // * tmpdata_這個指標變數是區域變數，會在這行結束時跟著
                                   //   自動結束其生命週期。
    T& operator[](SizeType id) {
        return data_[id];
    }
    ConstReference operator[](SizeType id) const {
        return data_[id];
    }
    SizeType Size() const {
        return size_;
    }
    void PushBack(T val) {
        Resize(Size()+1);        // * 先長大，
        (*this)[Size()-1] = val; //   再呼叫operator[]改最後一格的值，
                                 //   這行的Size()是上面那行長大後的結果。
    }
  private:
    SizeType size_ = 0;     // * size_表示目前使用到的長度。
    SizeType capacity_ = 0; // * capacity_表示當前的C風格陣列(data_所指向的物件)的空間大小
                            //   (意即最大能夠到多少(上限))。 (size_ <= capacity_)
    T* data_;
    // T data_[1000] = {};  // * 在新版的C++，可以直接在這裡對資料成員進行初始化。
};

template<typename T>
std::ostream& operator<< (std::ostream& os, const Vector<T>& data) {
    // * 這裡的回傳值使用std::ostream&原因有二：
    //   1. std::ostream本身就不能被複製，只有唯一一份(複製建構子被刪除了)，所以只能使用參考。
    //   2. 回傳參考同時也可以實現串接，也就是 os << "]" << std::endl; 這樣的語法，一個接一個。
    os << "[";
    for(size_t i = 0; i < data.Size(); i++){
        if(i != 0) {
            os << ", ";
        }
        os << data[i];
    }
    os << "]";
    return os;
}

int main() {
    Vector<int> v;                // * 用起來像陣列的物件，長度為0。
    std::cout << v << std::endl;  // []
    v.Resize(5);
    v[0] = 1;
    v[3] = 7;
    std::cout << v << std::endl;  // [1, 0, 0, 7, 0]
    v.Resize(3);
    v.Resize(5);
    std::cout << v << std::endl;  // [1, 0, 0, 0, 0]
    for(int i = 0; i < 5; i++) {
        v.PushBack(i);
        std::cout << v << std::endl; // [1, 0, 0, 0, 0, 0, 1, 2, 3, 4]
    }
    Vector<int> va{5};
    Vector<int> vb = va;
    va[2] = 3;
    std::cout << va << std::endl; // [0, 0, 0, 0, 0]
    std::cout << vb << std::endl; // [0, 0, 3, 0, 0]

    return 0;
}