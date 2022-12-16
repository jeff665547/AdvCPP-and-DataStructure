/* 資源擁有權 (Ownership) */
// * C++的使用場景：  (資源有限時使用(e.g. 執行時間有限(效率)、記憶體容量(空間)有限)。)
//   1. 硬體業
//   2. 遊戲業
//   3. 桌上型應用軟體業
// * 在C++中，沒必要就不要去複製資源。
// * 動態資源管理：
//   1. 複製成本很高的資源：e.g. 大型陣列。
//   2. 不能被複製的資源：e.g. 執行緒std::thread。
// * 因為對位址的操作(e.g. 複製、在函式間傳遞)很便宜，因此可用來取代直接對物件進行複製，
//   特別是針對那些複製成本很高以及不能夠被複製的物件，並藉由指標來對該物件進行操作。
// * 動態資源可能出現的位置：類別成員指標變數所指向的物件(RAII)、函式回傳的指標所指向的物
//   件(注意：不能夠回傳函式內部區域變數的位址(以及參考)(因為區域變數的生命週期在離開函式
//   時就已經結束了，如果使用該位址(參考)會發生問題)，只能夠回傳函式內部區域變數的複製品(針
//   對複製成本很低的物件)，或是將其擁有權給轉移出來(針對複製成本很高的物件)
//   。但若是回傳函式內部new出來物件的位址，則不會有問題，因為new出來的物件其生命週期必須
//   要由開發人員自行管理)。
// * 資源擁有權可以分為幾種情況：
//   1. 可以進行複製的資源(e.g. std::vector) => 實作 copy constructor, copy assignment operator
//   2. 不能複製也不能被多物件共用(只有唯一一份)的資源 (e.g. std::ostream, std::fstream, std::thread)
//      => unique_ptr (delete copy constructor, copy assignment operator，並在destructor直接清理資源)
//   3. 不能真的複製但可以被多物件共用的資源(儲存共用資源的位址) i.e. 複製實際上就是去共用資源 
//      (e.g. Python的變數binding機制)
//      => shared_ptr (在第一個物件使用時建構資源，在後續物件建構時去增加共享資源的計數器，
//                     在物件解構時去減少共享資源的計數器，直到最後一個物件解構時才去清理資源)
// * new (new[]) 運算子(動態配置)的特性：
//   1. new 所產生的物件是沒有名字的(只會拿到該無名物件的位址)。
//   2. new 所產生的物件其生命週期需要開發人員自行去做管理(沒事不會結束其生命週期，有名字的物件在名字不能使用
//      時就會死掉)。
//   3. new 會回傳該無名字物件的位址。
// * new 與 delete 運算子 (動態配置C風格陣列以及刪除C風格陣列是使用new[]以及delete[]運算子，
//                        []代表陣列、連續空間的意思)
//   T* ptr = new T;     // * 代表以動態方式配置一個T類型大小的無名記憶體空間(Default Initialization)，
//                       //   並回傳該無名空間的起始位址(以ptr儲存)。
//   T* ptr = new T{};   // * 以動態方式配置一個T類型大小的無名記憶體空間(Value Initialization)，
//                       //   並回傳該無名空間的起始位址(以ptr儲存)。
//   T* ptr = new T{7};  // * 以動態方式配置一個T類型大小的無名記憶體空間(Direct Initialization初始值為7)，
//                       //   並回傳該無名空間的起始位址(以ptr儲存)。
//   delete ptr;         // * 刪除ptr所指向的無名物件(在delete完後，ptr仍然指向(儲存)同樣的記憶體位址，不會
//                       //   變成nullptr，但是原先住在該記憶體位址的T物件已經被delete語法給解構刪除，並且不
//                       //   能再重新透過ptr指標去賦予該位址新的T物件)。
// * 一般不到必要的情況就避免使用new (new[])運算子，否則容易出現memory leak(只有new，沒有delete)，
//   或是程式當掉(new一次，但是重複delete)的問題，真的不得以的情況請使用智慧指標(unique_ptr, shared_ptr, 
//   etc.)，讓它來協助管理以及釋放內部new出來的物件。但有一種情況例外，就是極度講求效率，認為透過多一層物件
//   來管控new出來的資源是無意義的，而且是沒有效率的，一般這種情況是在寫最底層的程式邏輯(e.g. 資料結構)時會
//   遇到，在這種時會才會直接去new以及delete。否則在大多數的應用層都會使用智慧指標來做資源管理。
// * 智慧指標的缺點：
//   * 需要有一個專門的物件去包裝指標，負責管理new出來的資源 => 需要花費額外時間消耗。
// * 右值參考(rvalue reference)(T&&)，用來參考右值表示式(如：暫時物件、字面常數、函式回傳)的參考。
//   (右值不能被一般的(左值)參考所參考)
// * std::move() 做了兩件事情：
//   1. 將參數(T)轉型成右值參考(rvalue reference, T&&)的型態
//   2. std::move()會將lvalue expression變成是rvalue expression
// * 基礎型態(fundamental types, e.g. int, float, double, char, bool)不會有移動建構子(move constructor)
//   以及移動賦值運算子(move assignment operator)。也因此，當基礎型態需要使用這些函式時會自動降級成複製建構子
//   (copy constructor)以及複製賦值運算子(copy assignment opreator)，也就是說會使用複製的方式而非移動的方式
//   去建構物件或是賦值。
#include <iostream>
#include <memory>  // * For loading the unique_ptr, shared_ptr, make_unique(), make_shared().

template<typename T>
class UniquePointer{
  public:
    UniquePointer(T* ptr) {
        ptr_ = ptr;
    }
    ~UniquePointer() {
        delete ptr_;  // * 解構此類別的物件時自動刪除(釋放)ptr_所指向的無名物件(建構時傳入的動態配置資源)。
    }
    UniquePointer(const UniquePointer& rhs) = delete;            // * 不允許此類別的物件複製，因為在解構
    UniquePointer& operator=(const UniquePointer& rhs) = delete; //   時會導致此類別的不同物件重複刪除
                                                                 //   ptr_所指向的無名物件，造成程式當掉。
    
    UniquePointer(UniquePointer&& rhs) {                         // * 允許此類別的物件移動(將管理的資源擁
                                                                 //   有權轉移，也就是負責進行釋放(delete)
                                                                 //   管理資源的物件變了rhs => *this)。
                                                                 // * 物件移動也可以輔助實現NRVO(透過實作
                                                                 //   移動建構子來啟動NRVO)。
        ptr_ = rhs.ptr_;
        rhs.ptr_ = nullptr;
    }
    UniquePointer& operator=(UniquePointer&& rhs) {
        if(this == &rhs) return *this;  // * rhs is an lvalue expression (having address), 
                                        //   its type is a rvalue reference.
                                        // * 檢查if(this == &rhs)是為了要解決自我賦值的問題。
        
        delete ptr_;                    // * 避免原來指向的物件殘留導致記憶體洩漏(處理舊有資源)。
        ptr_ = rhs.ptr_;
        rhs.ptr_ = nullptr;
        return *this;
    }
    T* operator->() { return ptr_; }  // * 規定：C++規定operator->實作時一定要回傳一般指標(否則無法接續呼叫->運算子)。
                                      // * 透過實作operator->可以讓UniquePointer用起來跟一般指標一樣，但是
                                      //   不可以複製 => 確保只有一個物件可以擁有資源，解構時就可以安心釋放。
    T* get() {return ptr_; }          // * 可以透過get()拿到裸指標。
  private:
    T* ptr_;
};

class LargeObject{
  public:
    LargeObject() { std::cout << "Default Construction" << std::endl; }
    LargeObject( const LargeObject& ) { std::cout << "Copy Construction" << std::endl; }
    ~LargeObject() { std::cout << "Destruction" << std::endl; }
    void Process() { std::cout << "Do Something" << std::endl; }
  private:
    int garbage[1000];
};

// * 透過一個幫助函式(Helper Function)來簡化跟明確化使用的方式，使程式碼更易讀。
template <typename T>
UniquePointer<T> MakeUnique() {
    // * 就語意而言，此處是回傳一個無名的暫時物件(UniquePointer<T> {new T{}})，也因此在
    //   caller端會優先觸發移動建構子來產生MakeUnique()的回傳結果，也就是說MakeUnique()
    //   的結果(為一暫時物件)是透過移動建構子所建構出來的。此時，該移動建構子的參數為
    //   UniquePointer<T> {new T{}}，也為一個暫時物件。但在實務上，因為此格式型態是
    //   URVO，為compiler可以優化的情況，因此會直接觸發複製省略(Copy Elision)的優化。
    // * URVO (此處條件為複製建構子和移動建構子都不需要實作就可以保證複製省略) 
    //   => Copy Elision.
    return UniquePointer<T> {new T{}};
}

// * Helper Function
UniquePointer<LargeObject> CreateLargeObject() {
    // * 就語意而言，此處是回傳一個變數，也因此在caller端會觸發複製建構子來產生
    //   CreateLargeObject()的回傳結果，也就是說CreateLargeObject()的結果(為一暫時
    //   物件)是透過複製建構子所建構出來的。此時，該複製建構子的參數為objPtr，為一個有
    //   名字的物件。但在實務上，因為此格式型態是NRVO的一般情況，compiler可以在有實作
    //   複製建構子或是移動建構子的情況下進行優化，進而觸發複製省略(Copy Elision)，
    //   若是屬於NRVO中較複雜的格式型態，則編譯器不會進行複製省略的優化，但會優先呼叫
    //   移動建構子(不需要寫std::move())來進行CreateLargeObject()回傳結果的建構，若
    //   移動建構子沒有實作，才會自動去呼叫複製建構子來進行CreateLargeObject()回傳結
    //   果的建構。
    // * NRVO (Either copy or moving constructor is needed to trigger the copy 
    //   elision) => Copy Elision.
    auto objPtr = MakeUnique<LargeObject>();
    auto objPtrA = std::move(objPtr);  // * 這邊的std::move(objPtr)其實就是
                                       //   static_cast<UniquePointer<LargeObject>&&>(objPtr);
                                       //   也就是將 objPtr 強制轉型成該型態的 rvalue reference。與此
                                       //   同時，也將原本的lvalue expression (objPtr變數) 轉成rvalue 
                                       //   expression (xvalue, expiring value)，並藉著建構子的引數 
                                       //   是一個rvalue expression來觸發objPtrA的移動建構(移動建構子
                                       //   的引數必須是rvalue expression才能被rvalue reference (T&&)
                                       //   型態的參數所參考)。
    return objPtrA;
}

std::unique_ptr<LargeObject> CreateLargeObjectUsingCpp() {
    // * make_unique()內可以放LargeObject的建構子參數用來建構LargeObject。
    auto objPtrA = std::make_unique<LargeObject>();
    return objPtrA;
}

/* 共用(享)資源 => Reference Counting Technique */
// * Reference Counting 的風險是會有循環參照的問題 (a = b; b = a;) => 導致memory leak的問題。
template<typename T>
class SharedPointer{
  public:
    SharedPointer(T* ptr) {
        dataPtr_ = ptr;
        countPtr_ = new int{1};  // * 創建時次數為1。
                                 // * 參照計數 Reference Counting 技巧。
                                 // * 因為所有共用相同資源的SharedPointer也要共用
                                 //   同一個計數器，所以計數器也要動態配置。
    }
    ~SharedPointer() {
        (*countPtr_)--;        // * 當SharedPointer物件解構時會將計數器減1。
        if(*countPtr_ == 0) {  // * 最後一個解構的SharedPointer物件去釋放資源。
            delete dataPtr_;
            delete countPtr_;
        }
    }
    SharedPointer(const SharedPointer& rhs) {
        dataPtr_ = rhs.dataPtr_;
        countPtr_ = rhs.countPtr_;
        (*countPtr_)++;        // * 複製建構SharedPointer物件時將計數器加1。
    }
    SharedPointer& operator=(const SharedPointer& rhs) {
        if(this == &rhs) return *this;   // * 處理自我賦值的問題。

        (*countPtr_)--;                  // * 處理舊有資源的問題(告訴舊有資源準備要管理新資源
                                         //   => 在舊有資源計數器上減1)。
        if(*countPtr_ == 0) {            // * 如果當前物件是最後一個解構的SharedPointer物件，
                                         //   則去釋放舊有資源。
            delete dataPtr_;
            delete countPtr_;
        }
        dataPtr_ = rhs.dataPtr_;         // * 新資源擁有權配置。
        countPtr_ = rhs.countPtr_; 
        (*countPtr_)++;                  // * 在新的資源計數器上加1。
        return *this;
    }
    T* operator->() {
        return dataPtr_;
    }
  private:
    T* dataPtr_;
    int* countPtr_;     // * 使用一個整數物件(countPtr_)當計數器，統計有多少
                        //   SharedPointer共用這個資源(dataPtr_)。
};

template<typename T>
SharedPointer<T> MakeShared() {
    return SharedPointer<T> {new T{}};
}

SharedPointer<LargeObject> CreateLargeObjectBySharedPtr() {
    auto obj = MakeShared<LargeObject>();
    return obj;
}

int main() {
    
    auto objPtr = CreateLargeObject();  // * 就語意而言，此處因為CreateLargeObject()
                                        //   的結果為一暫時物件，所以會優先採用移動建構子
                                        //   來對main中的objPtr進行建構。但在實務上，因為
                                        //   此格式為C++ compiler可以進行複製省略的格式，
                                        //   且是在不需要實作複製建構子以及移動建構子的條件
                                        //   下就會啟動，因此結果為複製省略(copy elision)
                                        //   的結果。
                                        // * Copy Elision (此處條件為複製建構子和移動建構
                                        //   子都不需要實作就可以保證複製省略)
    // auto objPtr = MakeUnique<LargeObject>();  // UniquePointer<LargeObject> objPtr = MakeUnique<LargeObject>();
    // UniquePointer<LargeObject> objPtr{new LargeObject{}};
    objPtr->Process(); // * 左邊會自動呼叫 objPtr.operator->()->Process(); 也就是 objPtr.ptr_->Process(); 
                       //   算是一個自動轉送機制。
    
    std::cout << "In cpp header: " << std::endl;
    std::unique_ptr<LargeObject> up = std::make_unique<LargeObject>();
    std::unique_ptr<LargeObject> uq = std::move(up);  // * unique_ptr物件不能被複製，其內部資源也不能被多個
                                                      //   unique_ptr物件所共享。
    uq->Process();
    std::cout << "======================================" << std::endl;

    auto shrobjPtrA = CreateLargeObjectBySharedPtr();
    auto shrobjPtrB = shrobjPtrA;
    shrobjPtrA->Process();
    shrobjPtrB->Process();
    // * SharedPointer使用參照計數(reference counting)實現多指標共用同一物件，並在沒有指標繼續使用該物件時自動釋放資源。

    std::cout << "In cpp header: " << std::endl;
    std::shared_ptr<LargeObject> sp = std::make_shared<LargeObject>();
    std::shared_ptr<LargeObject> sq = sp;   // * shared_ptr物件的複製實際上並不是真的去複製而是讓內部資源被多個
                                            //   shared_ptr物件所共享。
    sp->Process();
    sq->Process();
    std::cout << "======================================" << std::endl;

    return 0;
}