/* 鏈結串列 */
// * 資料結構如果追蹤到底就是兩個極端的議題：1. 陣列 (array, vector) 2. 串列 (linked list)
// * 絕大多數的資料型態都是由陣列以及串列所組合綜合出來的。
// * 陣列有以下兩個問題：
//     1. 陣列在起始開頭端元素的操作(e.g. push_front)其花費成本較高(通常跟陣列大小成正比: O(N))。 
//        => 鏈結串列 linked list
//     2. 陣列的索引值只能是整數，不能是其他種型態，如：字串。
//        => 雜湊表 hash table
// * 鏈結串列：
//   1. 每個資料節點(node)都記得下一個資料節點(node)所在的記憶體位址。
//   2. 用一個額外變數記得第一個資料節點的記憶體位址。
//   3. 以空指標(0, nullptr, NULL)表示已經沒有下一個節點了(即結尾)。
// * 運算複雜度
//  * 在鏈結串列的開頭端插入(刪除)元素，運算時間與串列大小無關。 => O(1)
//  * 對陣列做複製、在串列的開頭端插入元素(push_front())，運算時間會與陣列的大小成正比。 => O(n)
// * LeetCode NO: 876, 1290, 83, 206, 237, 21, 160
// * 在標準庫中，singly linked list的名稱又被叫做forward_list，doubly linked list的名稱又被叫做list。
#include <iostream>
#include <memory>

template<typename T>
struct ListNode{     // * struct 就是存取限制預設為public的class語法。
    ListNode(T x) : val(x), next(nullptr) {}   // * Direct Constructor.
    T val{};
    ListNode* next {};  // * 需要自行管理(刪除)節點ListNode的生命週期
};

struct ListNodeAuto{  
    // * 透過shared_ptr來自動管理節點ListNodeAuto的生命週期，
    //   但是若在此種情境下使用會導致效率變差，因為這樣子會多包一層物件
    //   去記錄目前有多少物件正在共用內部資源(如果沒有人在用(i.e.沒有實例物件
    //   儲存該內部資源)就會刪除該內部資源)，也因此較少開發者會在此情況下採用
    //   shared_ptr，在此情況下(實作底層邏輯時)大多數時候都是直接使用new或是
    //   delete進行直接管理效率較高，執行速度較快。
    ListNodeAuto(int x) : val(x), next(nullptr) {}
    int val;
    std::shared_ptr<ListNodeAuto> next;
};

template<typename T>
class ForwardList{
  public:
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const ForwardList<U>& lst);
    ForwardList() = default;
    ForwardList(const std::initializer_list<T>& values) { // * 初始化列表建構子
        ListNode<T> tmp = T{};               // * 新建立一個暫時的媒介節點，並透過該節點新增
        auto tmpptr = &tmp;                  //   從values複製過來的節點資料。
        for(auto& v: values) {
            tmpptr->next = new ListNode{v};
            tmpptr = tmpptr->next;
        }
        head = tmp.next;
    }
    ForwardList(const ForwardList& rhs) {    // * 因為有了自定義解構子，所以大三(五)法則也要重新自定義
                                             // * 利用雙指標演算法實作
                                             // * 目標：將鏈結串列完整地複製一次
        ListNode<T>* source_cur = rhs.head;
        ListNode<T>* target_cur;
        while(source_cur != nullptr) {       // * 確認當前資料來源節點為非空節點(最後一個)後再進去該節點
                                             //   拿資料or進行只限於該節點的其他操作。
            if(source_cur == rhs.head) {
                target_cur = new ListNode<T>{source_cur->val};
                head = target_cur;
                source_cur = source_cur->next;  // * 在要進入下個node之前做好其他的操作。
                continue;
            }
            target_cur->next = new ListNode<T>{source_cur->val};
            target_cur = target_cur->next;            
            source_cur = source_cur->next;
        }

        // ListNode<T> tmp{T{}};    // * 此為另一種寫法(透過暫時的媒介節點來複製資料)
        // auto insertAt = &tmp;
        // for(auto copyFrom = rhs.head; copyFrom != nullptr; copyFrom = copyFrom->next) {
        //     insertAt->next = new ListNode<T>{copyFrom->val};
        //     insertAt = insertAt->next;
        // }
        // head = tmp->next;
    }
    ForwardList& operator=(const ForwardList& rhs) {
        if(this == &rhs) return *this;
        ForwardList newList = rhs;      // * Use the copy constructor to copy the original ForwardList.
        std::swap(head, newList.head);  // * The destructor of the newList will help to 
                                        //   delete the old data.
        return *this;
    }
    T& operator[](int k) {  // * 自定義operator[]運算子
        auto tmp = head;
        for(int i = 0; i < k; i++) {
            tmp = tmp->next;
        }
        return tmp->val;
    }

    void PushFront(T ele) {
        ListNode<T>* tmp = new ListNode<T>{ele};
        tmp->next = head;
        head = tmp;
    }
    bool IsEmpty() const {
        return head == nullptr;
    }
    void PopFront() {
        auto tmp = head;
        head = head->next;
        delete tmp;
    }
    ~ForwardList() {
        while(!IsEmpty()) {     // * 透過PopFront()與IsEmpty()實現解構子。
            PopFront();
        }
    }
  private:
    ListNode<T>* head {nullptr};
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const ForwardList<T>& lst) {
    os << "[";
    for(ListNode<T>* curr = lst.head; curr != nullptr; curr = curr->next) {
        if(curr != lst.head) {
            os << ", ";
        }
        os << curr->val;
    }
    os << "]";
    return os;
}

int main() {
    /* 自行新增節點(從近的開始新增) */
    ListNode<int>* head   = new ListNode<int>{1};
    head->next            = new ListNode<int>{2};
    head->next->next      = new ListNode<int>{3};

    /* 循序存取鏈結串列 (迭帶器, Iterator 的觀念) */
    // * 當curr走到nullptr就代表已經走到鏈結串列的結尾節點了(終止條件)。
    for(ListNode<int>* curr = head; curr != nullptr; curr = curr->next) {
        std::cout << curr->val << std::endl;
    }

    /* 在開頭端新增節點(PushFront) */
    ListNode<int>* tmp = head;
    head = new ListNode<int>{4};
    head->next = tmp;

    /* 在開頭端刪除節點(PopFront) */
    tmp = head;
    head = head->next;
    delete tmp;

    /* 在中間(在第一個ListNode後方)的地方新增節點 */
    tmp = head->next;
    head->next = new ListNode<int>{4};
    head->next->next = tmp;

    /* 自行刪除節點(從遠的開始刪除) */
    delete head->next->next->next;
    delete head->next->next;
    delete head->next;
    delete head;
    // * head變數會由程式自行回收。

    /* 使用shared_ptr來進行節點的自動管理(新增&刪除) => 會有額外效能消耗 */
    auto head2        = std::make_shared<ListNodeAuto>(1);  // * auto <=> std::shared_ptr<ListNodeAuto>
    head2->next       = std::make_shared<ListNodeAuto>(2);
    head2->next->next = std::make_shared<ListNodeAuto>(3);

    for(auto curr = head2; curr != nullptr; curr = curr->next) {
        std::cout << curr->val << std::endl;
    }

    /* Class Template */
    ForwardList<int> l;
    std::cout << l << std::endl;   // []
    l.PushFront(10);            
    std::cout << l << std::endl;   // [10]
    l.PushFront(20);            
    std::cout << l << std::endl;   // [20, 10]
    l.PushFront(30);            
    std::cout << l << std::endl;   // [30, 20, 10]

    ForwardList<int> ll = {12, 34, 56, 78, 90};
    std::cout << ll << std::endl;  // [12, 34, 56, 78, 90]

    ForwardList<int> ll2 = ll;
    std::cout << ll2 << std::endl;  // [12, 34, 56, 78, 90]

    ForwardList<int> ll3;
    ll[3] = 888;
    ll3 = ll;
    std::cout << ll3 << std::endl;  // [12, 34, 56, 78, 90]

    return 0;
}