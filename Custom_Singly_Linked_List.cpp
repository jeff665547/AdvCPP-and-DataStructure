#include <iostream>
namespace STD{
template<typename T>
struct ListNode{
    T val{};
    ListNode* next{};
};

template<typename T>
class ForwardList{
  public:
    template<typename U> friend std::ostream& operator<<(std::ostream& os, const ForwardList<U>& lst);
    ForwardList() = default;
    ForwardList(const std::initializer_list<T>& lst) {
        ListNode<T> tmp {};
        ListNode<T>* cur;
        cur = &tmp;
        for(const T& ele: lst) {
            cur->next = new ListNode<T> {ele, nullptr};
            cur = cur->next;
            size_++;
        }
        Data = tmp.next;
    }
    ForwardList(const ForwardList& lst) {
        ListNode<T> tmp {};
        ListNode<T>* cur = &tmp;
        for(ListNode<T>* src = lst.Data; src != nullptr; src = src->next) {
            cur->next = new ListNode<T> {src->val, nullptr};
            cur = cur->next;
            size_++;
        }
        Data = tmp.next;
    }
    ForwardList& operator=(const ForwardList& lst) {
        if(&lst == this) return *this;
        ForwardList tmp = lst;
        std::swap(Data, tmp.Data);
        std::swap(size_, tmp.size_);
        return *this;
    }
    ForwardList(ForwardList&& lst) {
        Data = lst.Data;
        size_ = lst.size_;
        lst.Data = nullptr;
        lst.size_ = 0;
    }
    ForwardList& operator=(ForwardList&& lst) {
        if(&lst == this) return *this;
        ForwardList tmp = std::move(lst);
        std::swap(Data, tmp.Data);
        std::swap(size_, tmp.size_);
        return *this;
    }
    ~ForwardList() {
        while(!IsEmpty()) {
            PopFront();
        }
    }
    T& operator[](int id) {
        ListNode<T>* cur = Data;
        int i = 0;
        // if(!IsEmpty()) {
        while(id != i) {
            cur = cur->next;
            i++;
        }
        return cur->val;
        // }
    }
    const T& operator[](int id) const {
        ListNode<T>* cur = Data;
        int i = 0;
        // if(!IsEmpty()) {
        while(id != i) {
            cur = cur->next;
            i++;
        }
        return cur->val;
        // }
    }
    T& Front() {
        return (*this)[0];
    }
    const T& Front() const {
        return (*this)[0];
    }
    T& Back() {
        return (*this)[size_-1];
    }
    const T& Back() const {
        return (*this)[size_-1];
    }
    int Size() const {
        return size_;
    }
    bool IsEmpty() {
        return Data == nullptr;
    }
    void PushBack(const T& val) {
        if(!IsEmpty()) {
            ListNode<T>* cur = Data;
            while(cur->next) {
                cur = cur->next;
            }
            cur->next = new ListNode<T> {val, nullptr};
        } else {
            Data = new ListNode<T> {val, nullptr};
        }
        size_++;
    }
    void PushFront(const T& val) {
        Data = new ListNode<T> {val, Data};
        size_++;
    }
    void PopBack() {
        if(!IsEmpty()) {
            ListNode<T>* cur = Data;
            ListNode<T>* pre;
            if(!cur->next) {
                Data = nullptr;
            }
            while(cur->next) {
                pre = cur;
                cur = cur->next;
            }
            pre->next = nullptr;
            delete cur;
            size_--;
        }
    }
    void PopFront() {
        if(!IsEmpty()) {
            ListNode<T>* tmp;
            tmp = Data;
            Data = Data->next;
            delete tmp;
            size_--;
        }
    }
    class Iterator{
      public:
        Iterator(const ListNode<T>* ptr) : ptr(ptr) {}
        bool operator!=(const Iterator& other) const {
            return ptr!=other.ptr;
        }
        const T& operator*() const {
            return ptr->val;
        }
        Iterator operator++(int) {
            const ListNode<T>* tmp = ptr;
            if(ptr) ptr = ptr->next;
            return tmp;
        }
      private:
        const ListNode<T>* ptr;
    };
    Iterator Begin() const {
        return Data;
    }
    Iterator End() const {
        return nullptr;
    }
  private:
    ListNode<T>* Data{};
    int size_{};
};

template<typename T>
typename ForwardList<T>::Iterator Begin(const ForwardList<T>& lst) {
    return lst.Begin();
}
template<typename T>
typename ForwardList<T>::Iterator End(const ForwardList<T>& lst) {
    return lst.End();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ForwardList<T>& lst) {
    os << "[";
    for(auto cur = lst.Begin(); cur != lst.End(); cur++) {
        if(cur != lst.Begin()) os << ", ";
        os << *cur;
    }
    // for(ListNode<T>* cur = lst.Data; cur != nullptr; cur = cur->next) {
    //     if(cur != lst.Data) os << ", ";
    //     os << cur->val;
    // }
    os << "]";
    return os;
}
}

int main() {

    /* 在中間(在第一個ListNode後方)的地方新增節點 */
    // tmp = head->next;
    // head->next = new ListNode<int>{4};
    // head->next->next = tmp;

    /* 使用shared_ptr來進行節點的自動管理(新增&刪除) => 會有額外效能消耗 */
    // auto head2        = std::make_shared<ListNodeAuto>(1);  // * auto <=> std::shared_ptr<ListNodeAuto>
    // head2->next       = std::make_shared<ListNodeAuto>(2);
    // head2->next->next = std::make_shared<ListNodeAuto>(3);


    /* Class Template */
    STD::ForwardList<int> l;
    std::cout << l << std::endl;         // []
    l.PushFront(10);            
    std::cout << l << std::endl;         // [10]
    l.PushBack(20);            
    std::cout << l << std::endl;         // [10, 20]
    l.PushFront(30);            
    std::cout << l << std::endl;         // [30, 10, 20]
    l.PopBack();
    std::cout << l << std::endl;         // [30, 10]
    l.PopFront();
    std::cout << l << std::endl;         // [10]
    l.PopBack();
    std::cout << l << std::endl;         // []
    l.PushBack(10);            
    std::cout << l << std::endl;         // [10]
    l.PushFront(20);            
    std::cout << l << std::endl;         // [20, 10]
    std::cout << l.Size() << std::endl;  // 2
    l.PushFront(30);            
    std::cout << l << std::endl;         // [30, 20, 10]

    STD::ForwardList<int> ll = {12, 34, 56, 78, 90};
    std::cout << ll << std::endl;  // [12, 34, 56, 78, 90]

    const STD::ForwardList<int> ll2 = ll;
    std::cout << ll2.Front() << std::endl;  // 12
    std::cout << ll2.Back() << std::endl;   // 90
    std::cout << ll2 << std::endl;          // [12, 34, 56, 78, 90]
    std::cout << ll2.Size() << std::endl;   // 5

    STD::ForwardList<int> ll3;
    ll[3] = 888;
    ll3 = ll;
    std::cout << ll3 << std::endl;  // [12, 34, 56, 888, 90]

    STD::ForwardList<int> ll4;
    ll4 = std::move(ll3);
    std::cout << ll3 << std::endl;  // []
    std::cout << ll4 << std::endl;  // [12, 34, 56, 888, 90]

    return 0;
}