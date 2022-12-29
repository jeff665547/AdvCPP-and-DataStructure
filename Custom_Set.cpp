#include <iostream>
#include <vector>
#include <forward_list>
#include <functional>

namespace STD{
// * Generic Algorithm
template<typename Iterator, typename T>
Iterator Find(const Iterator& begin, const Iterator& end, const T& val) {
    for(Iterator it = begin; it != end; it++) {
        if(*it == val) return it;
    }
    return end;
}
// * Complex Data Structure: A vector of (forward, singly linked) list.
//   Primary: std::vector, Secondary: std::forward_list.
template<typename T>
class Set{
  public:
    using ListIterator = typename std::forward_list<T>::iterator;
    Set(): data_(6) {}
    std::size_t Hash(const T& val) {
        std::hash<T> hasher;
        return (hasher(val) + data_.size()) % data_.size();
    }
    bool Contains(const T& val) {
        std::size_t bucketid = Hash(val);
        std::forward_list<T>& bucket = data_[bucketid];
        ListIterator it = Find(std::begin(bucket), std::end(bucket), val);
        return it != std::end(bucket);
    }
    void Insert(const T& val) {
        if(!Contains(val)) {
            std::size_t bucketid = Hash(val);
            std::forward_list<T>& bucket = data_[bucketid];
            bucket.push_front(val);
        }
    }
    void Insert(T&& val) {
        if(!Contains(val)) {
            std::size_t bucketid = Hash(val);
            std::forward_list<T>& bucket = data_[bucketid];
            bucket.push_front(std::move(val));
        }
    }
    void Erase(const T& val) {
        if(Contains(val)) {
            std::size_t bucketid = Hash(val);
            std::forward_list<T>& bucket = data_[bucketid];
            bucket.remove(val);
        }
    }
    class Iterator{
        friend class Set;
      public:
        bool operator==(const Iterator& other) const {
            return iterator_ == other.iterator_;
        }
        bool operator!=(const Iterator& other) const {
            return iterator_ != other.iterator_;
        }
        Iterator operator++(int) {
            Iterator tmp{index_, data_, iterator_};
            iterator_++;
            while(iterator_==std::end(data_[index_]) && index_+1 < data_.size()) {
                index_++;
                iterator_ = std::begin(data_[index_]);
            }
            return tmp;
        }
        Iterator operator++() {
            iterator_++;
            while(iterator_==std::end(data_[index_]) && index_+1 < data_.size()) {
                index_++;
                iterator_ = std::begin(data_[index_]);
            }
            return *this;
        }
        const T& operator*() {
            return *iterator_;
        }
      private:
        Iterator(
            const int& index,
            const std::vector<std::forward_list<T>>& data,
            const typename std::forward_list<T>::const_iterator& iterator
        ): index_(index), data_(data), iterator_(iterator) {}
        int index_;
        const std::vector<std::forward_list<T>> data_;
        typename std::forward_list<T>::const_iterator iterator_;
    };
    Iterator Begin() const {
        int id = 0;
        while(std::begin(data_[id]) == std::end(data_[id]) && id+1 < data_.size()) {
            id++;
        }
        return {id, data_, std::begin(data_[id])};
    }
    Iterator End() const {
        int id = data_.size() - 1;
        return {id, data_, std::end(data_[id])};
    }
    void Clear() {
        Iterator it = Begin();
        int index = it.index_;
        if(it == End()) return;
        while(std::begin(data_[index]) != std::end(data_[index]) || index+1 < data_.size()) {
            if(std::begin(data_[index]) != std::end(data_[index])) data_[index].clear();
            index++;
        }
        // for(Iterator it = Begin(); it != End(); it++) {
        //     int index = it.index_;
        //     data_[index].clear();
        // }
    }
  private:
    std::vector<std::forward_list<T>> data_;
};
// * Template Specialization (for int)
template<>
class Set<int>{
  public:
    using ListIterator = typename std::forward_list<int>::iterator;
    Set(): data_(6) {}
    std::size_t Hash(const int& val) {
        int n = data_.size();
        return (val % n + n) % n;
    }
    bool Contains(const int& val) {
        std::size_t bucketid = Hash(val);
        std::forward_list<int>& bucket = data_[bucketid];
        ListIterator it = Find(std::begin(bucket), std::end(bucket), val);
        return it != std::end(bucket);
    }
    void Insert(const int& val) {
        if(!Contains(val)) {
            std::size_t bucketid = Hash(val);
            std::forward_list<int>& bucket = data_[bucketid];
            bucket.push_front(val);
        }
    }
    void Insert(int&& val) {
        if(!Contains(val)) {
            std::size_t bucketid = Hash(val);
            std::forward_list<int>& bucket = data_[bucketid];
            bucket.push_front(std::move(val));
        }
    }
    void Erase(const int& val) {
        if(Contains(val)) {
            std::size_t bucketid = Hash(val);
            std::forward_list<int>& bucket = data_[bucketid];
            bucket.remove(val);
        }
    }
    class Iterator{
        friend class Set<int>;
      public:
        bool operator==(const Iterator& other) const {
            return iterator_ == other.iterator_;
        }
        bool operator!=(const Iterator& other) {
            return iterator_ != other.iterator_;
        }
        Iterator operator++(int) {
            Iterator tmp{index_, data_, iterator_};
            iterator_++;
            while(iterator_==std::end(data_[index_]) && index_+1 < data_.size()) {
                index_++;
                iterator_ = std::begin(data_[index_]);
            }
            return tmp;
        }
        Iterator operator++() {
            iterator_++;
            while(iterator_==std::end(data_[index_]) && index_+1 < data_.size()) {
                index_++;
                iterator_ = std::begin(data_[index_]);
            }
            return *this;
        }
        const int& operator*() {
            return *iterator_;
        }
      private:
        Iterator(
            const int& index,
            const std::vector<std::forward_list<int>>& data,
            const typename std::forward_list<int>::const_iterator& iterator
        ): index_(index), data_(data), iterator_(iterator) {}
        int index_;
        const std::vector<std::forward_list<int>>& data_;
        typename std::forward_list<int>::const_iterator iterator_;
    };
    Iterator Begin() const {
        int id = 0;
        while(std::begin(data_[id]) == std::end(data_[id]) && id+1 < data_.size()) {
            id++;
        }
        return {id, data_, std::begin(data_[id])};
    }
    Iterator End() const {
        int id = data_.size() - 1;
        return {id, data_, std::end(data_[id])};
    }
    void Clear() {
        Iterator it = Begin();
        int index = it.index_;
        if(it == End()) return;
        while(std::begin(data_[index]) != std::end(data_[index]) || index+1 < data_.size()) {
            if(std::begin(data_[index]) != std::end(data_[index])) data_[index].clear();
            index++;
        }
        // for(Iterator it = Begin(); it != End(); it++) {
        //     int index = it.index_;
        //     data_[index].clear();
        // }
    }
  private:
    std::vector<std::forward_list<int>> data_;
};
template<typename Container>
typename Container::Iterator Begin(const Container& con) {
    return con.Begin();
}
template<typename Container>
typename Container::Iterator End(const Container& con) {
    return con.End();
}
template<typename Container>
void Print(const Container& con) {
    std::cout << "{";
    for(typename Container::Iterator it = Begin(con); it != End(con); it++) {
        if(it!=Begin(con)) std::cout << ", ";
        std::cout << *it;
    }
    std::cout << "}";
}
}

int main() {

    STD::Set<int> soi;
    soi.Insert(7);
    soi.Insert(-2);
    soi.Insert(2);
    soi.Insert(8);
    soi.Insert(20);
    soi.Insert(14);
    soi.Insert(10);
    soi.Insert(16);

    std::cout << std::boolalpha;
    std::cout << soi.Contains(8) << std::endl;
    std::cout << soi.Contains(7) << std::endl;

    soi.Erase(7);

    std::cout << soi.Contains(8) << std::endl;
    std::cout << soi.Contains(7) << std::endl;
    std::cout << "{";
    for(auto p = STD::Begin(soi); p != STD::End(soi); ++p) {
        if(p != STD::Begin(soi)) std::cout << ", ";
        std::cout << *p;
    }
    std::cout << "}" << std::endl;
    STD::Print(soi);
    std::cout << std::endl;
    soi.Clear();
    STD::Print(soi);
    std::cout << std::endl;

    std::cout << "============================" << std::endl;
    STD::Set<std::string> sos;
    std::string member1 = "Mary";
    std::string member2 = "John";
    sos.Insert(member1);
    sos.Insert(std::move(member2));
    sos.Insert("Jeffery");
    std::cout << sos.Contains(member1) << std::endl;
    std::cout << sos.Contains(member2) << std::endl;
    std::cout << member2 << std::endl;
    STD::Print(sos);
    std::cout << std::endl;
    sos.Clear();
    STD::Print(sos);

    return 0;
}