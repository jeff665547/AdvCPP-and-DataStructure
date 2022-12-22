#include <iostream>
namespace STD{
template<typename T>
struct ListNode{
    T val{};
    ListNode* next{};
    ListNode* prev{};
};

template<typename T>
class List{
  public:
    template<typename U> friend std::ostream& operator<<(std::ostream& os, const List<U>& lst);
    List() = default;
    List(const std::initializer_list<T>& lst) {
        ListNode<T> tmp {};
        ListNode<T>* cur;
        cur = &tmp;
        for(const T& ele: lst) {
            cur->next = new ListNode<T> {ele, nullptr, cur};
            cur = cur->next;
            size_++;
        }
        Head = tmp.next;
        Head->prev = nullptr;
        Tail = cur;
    }
    List(const List& lst) {
        ListNode<T> tmp {};
        ListNode<T>* cur = &tmp;
        for(ListNode<T>* src = lst.Head; src != nullptr; src = src->next) {
            cur->next = new ListNode<T> {src->val, nullptr, cur};
            cur = cur->next;
            size_++;
        }
        Head = tmp.next;
        Head->prev = nullptr;
        Tail = cur;
    }
    List& operator=(const List& lst) {
        if(&lst == this) return *this;
        List tmp = lst;
        std::swap(Head, tmp.Head);
        std::swap(Tail, tmp.Tail);        
        std::swap(size_, tmp.size_);
        return *this;
    }
    List(List&& lst) {
        Head = lst.Head;
        Tail = lst.Tail;
        size_ = lst.size_;
        lst.Head = nullptr;
        lst.Tail = nullptr;
        lst.size_ = 0;
    }
    List& operator=(List&& lst) {
        if(&lst == this) return *this;
        List tmp = std::move(lst);
        std::swap(Head, tmp.Head);
        std::swap(Tail, tmp.Tail);
        std::swap(size_, tmp.size_);
        return *this;
    }
    ~List() {
        while(!IsEmpty()) {
            PopFront();
        }
    }
    T& operator[](int id) {
        ListNode<T>* cur = Head;
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
        ListNode<T>* cur = Head;
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
        return Head->val;
    }
    const T& Front() const {
        return Head->val;
    }
    T& Back() {
        return Tail->val;
    }
    const T& Back() const {
        return Tail->val;
    }
    int Size() const {
        return size_;
    }
    bool IsEmpty() {
        return Head == nullptr;
    }
    void PushBack(const T& val) {
        if(!IsEmpty()) {
            Tail->next = new ListNode<T> {val, nullptr, Tail};
            Tail = Tail->next;
        } else {
            Head = new ListNode<T> {val, nullptr, nullptr};
            Tail = Head;
        }
        size_++;
    }
    void PushFront(const T& val) {
        if(!IsEmpty()) {
            Head->prev = new ListNode<T> {val, Head, nullptr};
            Head = Head->prev;
        } else {
            Head = new ListNode<T> {val, nullptr, nullptr};
            Tail = Head;
        }
        size_++;
    }
    void PopBack() {
        if(!IsEmpty()) {
            ListNode<T>* tmp = Tail;
            Tail = Tail->prev;
            if(Tail) {
                Tail->next = nullptr;
            }else{
                Head = nullptr;
            }
            size_--;
        }
    }
    void PopFront() {
        if(!IsEmpty()) {
            ListNode<T>* tmp = Head;
            Head = Head->next;
            if(Head) {
                Head->prev = nullptr;
            }else{
                Tail = nullptr;
            }
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
        Iterator operator--(int) {
            const ListNode<T>* tmp = ptr;
            if(ptr) ptr = ptr->prev;
            return tmp;
        }
      private:
        const ListNode<T>* ptr;
    };
    Iterator Begin() const {
        return Head;
    }
    Iterator End() const {
        return nullptr;
    }
  private:
    ListNode<T>* Head{};
    ListNode<T>* Tail{};
    int size_{};
};

template<typename T>
typename List<T>::Iterator Begin(const List<T>& lst) {
    return lst.Begin();
}
template<typename T>
typename List<T>::Iterator End(const List<T>& lst) {
    return lst.End();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& lst) {
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

    /* Class Template */
    STD::List<int> l;
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

    STD::List<int> ll = {12, 34, 56, 78, 90};
    std::cout << ll << std::endl;  // [12, 34, 56, 78, 90]

    const STD::List<int> ll2 = ll;
    std::cout << ll2.Front() << std::endl;  // 12
    std::cout << ll2.Back() << std::endl;   // 90
    std::cout << ll2 << std::endl;          // [12, 34, 56, 78, 90]
    std::cout << ll2.Size() << std::endl;   // 5

    STD::List<int> ll3;
    ll[3] = 888;
    ll3 = ll;
    std::cout << ll3 << std::endl;  // [12, 34, 56, 888, 90]

    STD::List<int> ll4;
    ll4 = std::move(ll3);
    std::cout << ll3 << std::endl;  // []
    std::cout << ll4 << std::endl;  // [12, 34, 56, 888, 90]

    return 0;
}