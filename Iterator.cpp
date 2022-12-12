/* 迭代器 Iterator */
// * 迭代器是一種設計模式(design pattern)，大多數程式語言都有，但
//   不同程式語言有不同的實作方式。其主要要解決的問題為在不了解容器
//   資料結構的實作細節下去拜訪容器內的元素。(只要求能夠一個一個元
//   素拜訪一次)
// * 容器(Container)是一種用來表示物件集合(collection)的物件。
// * 容器物件負責管理其內部每個元素的生命週期。=> 容器死掉時，其內部
//   元素也要跟著死掉。
// * C++中的常見的資料結構又被稱為是容器(如：array(固定大小陣列), 
//   vector((動態)可變大小的陣列), forward_list(單向鏈結串列), 
//   list(雙向鏈結串列), deque(array和list的複合體的一種), 
//   unorder_set, unorder_map(unorder系列的都是雜湊表，將array
//   和list混合在一起使用，也算是複合體的一種))。
// * 概念：陣列 (std::array, std::vector) => 每個元素都在彼此的旁邊。
//        優勢：快速存取、快速查找。
//        串列 (std::forward_list, std::list) => 每個元素都不在彼此的旁邊(位置隨機)。
//        優勢：快速新增或刪除元素。
//        陣列與串列的複合體 (std::deque, std::unorder_map, etc.)
//        優勢：在不同的場合及時機使用不同的特性來達到不同的優勢。
// * 陣列與串列的複合物：
//   * 雙端佇列(double-ended queue)                   -- std::deque
//   * 堆疊(stack)                                    -- std::stack
//   * 佇列(queue)                                    -- std::queue
//   * 堆積(heap)                                     -- std::priority_queue
//   * 平衡二元搜尋樹(balanced binary search tree)     -- std::set, std::map, std::multiset, std::multimap
//   * 雜湊表(hash table)                             -- std::unordered_set, std::unordered_multiset, 
//                                                      std::unordered_map, std::unordered_multimap
// * 迭代器實際上就是一種位址，用來指向容器內的其中一個元素，可以用來存
//   取容器內的元素，故指標在很多時候(如果指標所指向的對象是容器的內容)
//   就算是一種迭代器，如：C風格陣列中元素的指標就是一種迭代器。
// * 使用迭代器來避免使用裸指標(一般指標, raw pointer)。
// * 在C++中絕大多數的標準容器都有迭代器(Iterator)，只有少數容器例外
//   (e.g. queue)。
// * 在迭代器前方加上*就可以取得它儲存的位址所代表的資料(透過實作
//   operator*()) or 取得該資料的成員(透過實作operator->())。
// * std::end(s)其所代表的位址是陣列中最後一個元素的下一個元素位址(
//   但是住在該位址的元素並不存在，因此不能對std::end(s)取資料，也就是
//   *std::end(s) 是錯誤的，其概念也就是linked list中的nullptr)。而
//   此概念也被拿來用在拜訪容器元素順序的終止，當迭代器走到end()時，就
//   代表已經走過頭了(到底了)。
// * 迭代器的分類：
//   1. 單向存取迭代器 Forward Iterator: 只能做++運算(向前移動一個元素) 
//      e.g. std::forward_list (Singly Linked List)。
//           std::unordered_set, std::unordered_map (Hash Table)。
//   2. 雙向存取迭代器 Biodirectional Iterator: 只支援++以及--運算(向前&向後移動一個元素) 
//      e.g. std::list (Doubly Linked List), 
//           std::map, std::set (Binary Search Tree)。
//   3. 隨機存取迭代器 Random Access Iterator: 除了基本的++以及--運算，也支援比大小(因為連續
//      儲存，大的在後小的在前)以及一次向前或向後移動多個元素 => i.e. 隨機存取元素   
//      e.g. std::array (Array), std::vector (Dynamic Array)。
// * 迭帶器之間的關係：由迭代器的分類可以發現，單向存取迭代器支援的功能最少，隨機存取迭代器支援
//   的功能最多。因此，能滿足單向存取迭代器的泛型(泛用)演算法也可以給雙向存取迭代器以及隨機存取
//   迭代器使用。同理，能滿足雙向存取迭代器的泛型(泛用)演算法也可以給隨機存取迭代器使用，但反之
//   則不成立。
// * 迭代器最起碼需要支援(operator*, operator->, operator++, operator==, operator!=)運算。
// * 不同種類的迭代器支援的移動方式會不一樣。
// * 迭代器應用(Application)
//   * 可以當成不同容器的共同規格來實作泛型演算法。
//   * 在algorithm中，可以觀察迭代器的種類來了解該演算法可以套用在哪種容器身上，
//     例如：觀察到std::sort()中RandomAccessIterator就代表支援std::vector, 
//     std::array, std::string，但不支援std::forward_list以及std::list。
//     與此同時，這些不支援該演算法(std::sort())的容器通常會有專門屬於該容器自
//     己特製版本的該種演算法(如：l.sort()，可在該容器(資料結構)的成員函式頁面
//     中找到)。
// * 泛型(泛用)演算法： 除了演算法泛用(如：count計數的目標元素型別泛用)外，連同被操作的對象也
//                    泛用(如：在哪種容器型別上count(尋找目標)也泛用)。
// * 泛型演算法：利用迭代器的規格介面(operator!=, operator++, operator*)來實作演算法。
//   count(first, last, val)        =>    統計val出現的次數。    =>    最低支援單向迭代器
//   find(first, last, val)         =>    尋找val出現的位置。    =>    最低支援單向迭代器
//   reverse(first, last)           =>    反轉順序。            =>    最低支援雙向迭代器
//   sort(first, last)              =>    排序。                =>    最低支援隨機迭代器
//   max_element(first, last)       =>    找值最大的元素。       =>    最低支援單向迭代器
//   lower_bound(first, last, val)  =>    用二元搜尋找val。      =>    最低支援單向迭代器
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>  // * for 泛型(泛用)演算法: std::sort()。

/* 自定義類別的迭代器 */
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// * 自定義迭代器給自定義類別使用
class ListIterator{
  public:
    ListIterator(ListNode* ptr): ptr_(ptr) {}
    bool operator!=(const ListIterator& other) const {
        return ptr_ != other.ptr_;
    }
    ListIterator operator++(int) { // * 此為 postfix increment operator overloading (後置++運算子
                                   //   (例如：p++)，需要吃一個int參數，在使用此運算子時，後置版本的 
                                   //   int 參數會傳入 0，但實際上沒有作用，只是用來識別前置(prefix,
                                   //   ++p)或後置(postfix, p++)，為一種固定格式)。如果是前置版本的
                                   //   話則為 ListIterator& operator++() {}。在實作面，p++代表回傳
                                   //   做加法之前的結果，++p代表回傳做加法之後的結果。
        ListNode* tmp = ptr_;
        ptr_ = ptr_->next;
        return tmp;
    }
    int& operator*() {
        return ptr_->val;
    }
  private:
    ListNode* ptr_{};
};

// * 自定義類別
class ForwardList {  // * For focusing on the demo of the design of the iterator, fix the size
                     //   and the content of the ForwardList, and delete the copy constructor 
                     //   and the copy assignment operator.
  public:
    ForwardList() {
        head_ = new ListNode{1};
        head_->next = new ListNode{2};
        head_->next->next = new ListNode{3};
    }
    ~ForwardList() {
        delete head_->next->next;
        delete head_->next;
        delete head_;
    }
    ForwardList(const ForwardList& other) = delete;
    ForwardList& operator=(const ForwardList& other) = delete;
    ListIterator Begin() const {
        return head_;
    }
    ListIterator End() const {
        return nullptr;
    }
  private:
    ListNode* head_ {};
};

// * 自定義Begin()以及End()函式(定義迭代器的共同介面，
//   比起直接去呼叫容器物件的Begin()，此函式會更泛用)。
ListIterator Begin(const ForwardList& lst) {
    return lst.Begin();
}

ListIterator End(const ForwardList& lst) {
    return lst.End();
}

/* 自定義C風格陣列的迭代器 */
// * 自定義迭代器模板給C風格陣列使用
template<typename T>
class ArrayIterator{
  public:
    ArrayIterator(T* ptr): ptr_(ptr) {}
    bool operator!=(const ArrayIterator& rhs) const {
        return ptr_ != rhs.ptr_;
    }
    ArrayIterator operator++(int) {  // * C++規定的後置++運算子多載格式。
        T* tmp = ptr_;
        ptr_ = ptr_+1;
        return tmp;  // or return ptr++;
    }
    T& operator*() {
        return *ptr_;
    }
  private:
    T* ptr_{};
};

// * 自定義Begin()以及End()函式模板(定義迭代器的共同介面)
template<typename T, int Size>
ArrayIterator<T> Begin(T (&v)[Size]) {
    return v;
}

template<typename T, int Size>
ArrayIterator<T> End(T (&v)[Size]) {
    return v + Size;
}

/* 泛型演算法：Count() */
// * 泛型演算法：利用迭代器的規格介面(operator!=, operator++, operator*)來實作演算法。
template<typename Iterator, typename T>
int Count(Iterator first, Iterator last, const T& target) {
    int ct{};
    for(Iterator p = first; p != last; p++) {
        if(*p == target) ct++;
    }
    return ct;
}

int main() {
    std::string s = "hello";
    std::cout << s[0] << std::endl;
    std::cout << *s.begin() << std::endl;   // * s.begin()回傳一個迭代器，並且指向
                                            //   第一個元素(儲存第一個元素的位址)，
                                            //   *s.begin()可以取得該元素的值('h')。
    std::cout << *s.cbegin() << std::endl;  // * + c 代表唯讀的意思 => 唯讀的迭代器
    std::cout << *s.rbegin() << std::endl;  // * + r 代表reverse(反向)的意思
    std::sort(std::begin(s), std::end(s));  // * 把s重新排列，透過迭代器的方式。
                                            // * std::begin(s) 會呼叫 s.begin();
    std::cout << s << std::endl;

    std::vector<int> v1 {1, 2, 6, 4, 7};
    std::cout << v1[2] << std::endl;
    std::cout << *(std::begin(v1) + 2) << std::endl;
    std::cout << *(std::end(v1) - 3) << std::endl;

    std::list<int> l1 {1, 2, 6, 4, 7};
    auto p = std::begin(l1);   // * Biodirectional Iterator，僅支援++以及--運算。
    for(size_t t = 0; t < 2; t++) {   // * std::advance()在做的事情。
        p++;                          // * ++運算其實就是 cur = cur->next;
                                      // * 對迭代器使用++運算子會得到下一個元素的迭代器。
    }
    std::cout << *p << std::endl;
    p = std::begin(l1);
    std::advance(p, 2);   // * advance 就是Iterator走路(前進)的意思。
    std::cout << *p << std::endl;
    p = std::end(l1);
    std::advance(p, -3);
    std::cout << *p << std::endl;

    /* 自定義C風格陣列的迭代器 */
    int v[5] = {10, 20, 30, 40, 50};
    for(ArrayIterator<int> p = Begin(v); p != End(v); p++) {
        std::cout << *p << " ";
    }
    std::cout << std::endl;

    /* 自定義類別的迭代器 */
    ForwardList l;
    for(ListIterator p = Begin(l); p != End(l); p++) {
        std::cout << *p << " ";
    }
    // * 迭代器的實作概念
    // for(ListNode* curr = l.head_; curr != nullptr; curr = curr->next) {
    //     std::cout << curr->val << " ";
    // }
    std::cout << std::endl;


    /* 自定義泛型演算法 */
    std::vector<int> va = {1, 9, 5, 7, 4, 6, 7, 8};
    std::cout << Count(std::begin(va), std::end(va), 3) << std::endl;
    std::cout << Count(std::begin(va), std::end(va), 7) << std::endl;

    return 0;
}