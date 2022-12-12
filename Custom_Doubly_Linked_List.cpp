#include <iostream>
template<typename T>
struct ListNode{
    ListNode() = default;
    ListNode(T val): val(val), prev(nullptr), next(nullptr) {}
    T val{};           // zero initialization 零初始化 (0)
    ListNode* prev{};  // zero initialization 零初始化 (nullptr)
    ListNode* next{};  // zero initialization 零初始化 (nullptr)
};

template<typename T>
class List{
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const List<U>& lst);
  public:
    List() = default;
    ~List() {
        while(!IsEmpty()){
            PopFront();
        }
        tail_ = nullptr;
    }
    List(const List& other) {
        ListNode<T> tmp{};
        ListNode<T>* target_ptr = &tmp;
        for(ListNode<T>* source_ptr = other.head_; 
            source_ptr != nullptr;
            source_ptr = source_ptr->next) 
        {
            target_ptr->next = new ListNode<T>{source_ptr->val};
            target_ptr->next->prev = target_ptr;
            target_ptr = target_ptr->next;          // * 在要進入下個node之前做好其他的操作。
        }
        head_ = tmp.next;
        head_->prev = nullptr;
        tail_ = target_ptr;
    }
    List(const std::initializer_list<T>& values) {
        ListNode<T> tmp{};
        ListNode<T>* ptr = &tmp;
        for(auto& v: values) {
            ptr->next = new ListNode<T>{v};
            ptr->next->prev = ptr;
            ptr = ptr->next;
        }
        head_ = tmp.next;
        head_->prev = nullptr;
        tail_ = ptr;
    }
    List& operator=(const List& other) {
        if(&other == this) return *this;
        List newList = other;
        std::swap(newList.head_, head_);
        std::swap(newList.tail_, tail_);
        return *this;
    }
    bool IsEmpty() {
        return head_ == nullptr;
    }
    void PopFront() {
        ListNode<T>* tmp = head_;
        if(head_->next == nullptr) {
            tail_ = nullptr;
        } else {
            head_->next->prev = nullptr;
        }
        head_ = head_->next;
        delete tmp;
    }
    void PushFront(const T& value) {
        ListNode<T>* tmp = new ListNode<T>{value};
        tmp->next = head_;
        if(head_ == nullptr) {
            tail_ = tmp;
        } else {
            head_->prev = tmp;
        }
        head_ = tmp;
    }
    T& operator[](int i) {
        ListNode<T>* ptr = head_;
        for(int j = 0; j < i; j++) {
            ptr = ptr->next;
        }
        return ptr->val;
    }
  private:
    ListNode<T>* head_ {};
    ListNode<T>* tail_ {};
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& lst) {
    os << "[";
    for(auto cur = lst.head_; cur != nullptr; cur = cur->next) {
        if(cur != lst.head_) os << ", ";
        os << cur->val;
    }
    os << "]";
    return os;
}

int main() {

    List<int> l = {10, 20, 30, 40, 50};
    std::cout << l << std::endl;

    List<int> l2 = l;
    l2[3] = 99;
    std::cout << l2 << std::endl;
    std::cout << l  << std::endl;

    l2.PushFront(100);
    std::cout << l2 << std::endl;
    l2.PopFront();
    std::cout << l2 << std::endl;

    List<int> l3;
    l3 = l2;
    l3[4] = 0;
    std::cout << l3 << std::endl;

    return 0;
}