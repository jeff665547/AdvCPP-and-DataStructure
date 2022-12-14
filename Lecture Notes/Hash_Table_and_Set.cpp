/* Hash Table and Set 雜湊表與集合 */
// * 程式撰寫概念：
//   1. 讓程式可編譯執行。
//   2. 想辦法用現成的工具將它給組合出來。 (類似實作ADT(Abstract Data Type))
//      a. 看甚麼樣的工具最適合，使用上最方便且直覺
//   3. 優化程式碼，改進效率。
//      a. (成員)函式參數型態優化，避免不必要的複製：T -> const T& or T&&
//      b. 資料結構以及演算法複雜度評估簡化替換(依據使用情境場景來決定)。
//         e.g. std::vector(快速查找、保留元素新增順序) <=> std::forward_list or std::list(快速新增、刪除)
// * 集合 (set)
// * 數學上的集合：裡面的元素值都是唯一(不重覆)的且沒有排序(order)的概念。
// * 標準庫中的set：std::unordered_set <- 使用雜湊表實作
// * 雜湊 (hash)： 將一個大範圍的東西(e.g. 數值、字串)對應到一個小範圍的物件(e.g. 一個數字陣列)
#include <iostream>
#include <vector>
#include <forward_list>
#include <string>
#include <algorithm>
#include <unordered_map>
template<typename T>
class Set{
  public:
    using Iterator = typename std::vector<T>::iterator; // * iterator為內建定義在std::vector內部的成員型態(也就是說
                                                        //   std::vector<T>類別"模板"內部有using iterator = .....
                                                        //   的格式)，屬於類別成員的一部分(因此前面需要加上類別的
                                                        //   namespace)，也因此在使用此種型態時最前方要加上typename
                                                        //   的關鍵字，告訴編譯器，iterator這個名字是此種類別模板中的
                                                        //   一種成員型態，而非代表成員變數、成員函式的名字，或是其他種
                                                        //   成員。
                                                        // * Iterator是Set<T>類別模板的成員型態，屬於類別成員的一部分，
                                                        //   因為其有可能會給其他類別模板使用，因此這邊將using....放在
                                                        //   public的範圍。
    Iterator Begin() {
        return std::begin(data_);
    }
    Iterator End() {
        return std::end(data_);
    }
    bool Contains(const T& value) { // * Check此資料結構內部是否有value這個元素，
                                    //   有就回傳true, 沒有就回傳false。
        return std::count(std::begin(data_), std::end(data_), value) > 0;
    }
    void Insert(const T& value) {  // * 在集合內加入新的元素。
        if(!Contains(value)) data_.push_back(value);  // * value會複製一份進data_
    }
    void Insert(T&& value) {  // * 在集合內加入新的元素(不複製版本)。
        if(!Contains(value)) data_.push_back(std::move(value));  // * value直接移進data_ (擁有權直接移轉)
    }
    void Erase(const T& value) {  // * 從集合內拿出某個元素。
        auto erase_it = std::find(std::begin(data_), std::end(data_), value);
        // * 使用std::vector<T>::erase來移除指定元素(因為algorithm中的remove()較不好
        //   使用)。
        if(erase_it != std::end(data_)) data_.erase(erase_it);
    }
    std::size_t Size() {
        return data_.size();
    }
  private:
    std::vector<T> data_;   // * 在沒有特別明確要使用甚麼資料結構的情況下，
                            //   可以先使用std::vector當成是基礎的資料結構
                            //   儲存型態，因為其在大多數時候的效率是最好的。
                            //   若有特殊情境要使用別種資料結構時再去優化即可。
                            // * 若只使用vector實作此類別會使得Contains, Insert,
                            //   Erase這些方法的演算法複雜度和陣列長度成正比，
                            //   即O(n)，若要優化就必須引入其他資料結構及演算法。
};

template<typename Container>  // * 這裡也可以使用template<typename T>
                              //   typename Set<T>::Iterator Begin(Set<T>& c)
                              //   意思一樣，只不過左邊寫法更為泛用，可用在Set<T>
                              //   以外的資料結構(只要該類別內部有定義Iterator這個
                              //   類別即可)。
typename Container::Iterator Begin(Container& c) {
    return c.Begin();
}

template<typename Container>
typename Container::Iterator End(Container& c) {
    return c.End();
}

template<typename Container>
void Print(Container con) {
    std::cout << "{";
    for(auto p = Begin(con); p != End(con); ++p) {
        if(p != Begin(con)) std::cout << ", ";
        std::cout << *p;
    }
    std::cout << "}" << std::endl;
}

/* 導入雜湊表的概念(i.e. 串列的陣列)讓方法的演算法複雜度和元素個數之間的關係脫鉤。 */
// * 上面Set的功能其實可以把它轉換成是一個儲存bool的陣列，當今天有相對應的元素要加入，
//   就把它的bool值改成true，要移除某個相對應的值，就把它的bool值改成false。
//   舉例來說：在下方例子中，如果 n = 6(固定大小)，則當value = 3，Hash(value) = 3。
//            當value = -5，Hash(value) = 1。當value = 2，Hash(value) = 2。
// * 若同時有兩個value經由Hash()算出來的結果是一致的，例如：當value為1或是-5時，Hash()
//   算出來的結果是一致的(都是1)，此種狀況被稱為碰撞(collision)。在此種情況下，一般會
//   希望能夠在Hash()算出來的結果附近快速新增(or刪除)元素，也因此會使用串列來解決此類問
//   題(而不是去新增一個陣列元素)，也就是說將陣列中的每個元素設計成串列，並且將串列中的
//   節點設計成可以同時儲存value以及該value是否存在的指示器的資料結構(e.g. std::pair)，
//   並將經過Hash()計算後具有同樣索引值的資料(節點)串接起來。因此，只要一開始陣列的
//   size()夠大，比較不會發生碰撞，效率也會比較好。
// * 雜湊表橫向是固定大小的，縱向會因節點新增而延伸。
// * 雜湊表用(橫向)陣列的原因：希望能快速跳至Hash()算出來的串列(快速查找)。
// * 雜湊表用(縱向)串列的原因：因為需要一直新增(or刪除)元素，因此希望能快速新增(or刪除)資料節點。
// * 標準庫中使用雜湊表實作的容器：std::unordered_xxx系列，
//                              如：std::unordered_map(不保證(留)插入順序，這個也就是python的dict)
//                              (map是使用二元搜尋樹實作的 => 較省空間，但效率未必較高)。
// * 在容器的使用中，若目標是要效率就要使用std::vector (python中的list、保留元素新增順序)，
//   但若目標是要方便使用就使用std::unordered_map (python中的dict、不保留元素新增順序、
//                                              預設同樣的鍵值(key)只能有一個、用空間換取時間)。
template<typename T>
class SetOptimize{
  public:
    SetOptimize(): data_(6) {}
    std::size_t Hash(const T& value) {   // * 這裡使用STL中提供的雜湊函數，他能夠將任何型態的參數
                                           //   對應到[0, data_.size()-1]範圍內的一個整數值，其原
                                           //   理是針對傳入物件的位元表示法做計算得到映射值。
        std::hash<T> hasher;
        return hasher(value) % data_.size();
    }
    bool Contains(const T& value) {
        auto& bucket = data_[Hash(value)];
        auto  result = std::find(std::begin(bucket), std::end(bucket), value);
        return result != end(bucket);
    }
    void Insert(const T& value) {
        auto& bucket = data_[Hash(value)];
        bucket.push_front(value);
    }
    void Insert(T&& value) {
        auto& bucket = data_[Hash(value)];
        bucket.push_front(std::move(value));
    }
    void Erase(const T& value) {
        auto& bucket = data_[Hash(value)];
        bucket.remove(value);
    }
    class Iterator {
        friend class SetOptimize;
      public:
        bool operator!=(const Iterator& other) {
            return iterator_ != other.iterator_;
        }
        Iterator operator++() {
            ++iterator_;
            while(iterator_ == std::end(data_[index_]) &&
                  index_+1 < data_.size()) {
                ++index_;
                iterator_ = std::begin(data_[index_]);
            }
            return *this;
        }
        T operator*() {
            return *iterator_;
        }
      private:
        Iterator(
            std::size_t index,
            const std::vector<std::forward_list<T>>& data,
            typename std::forward_list<T>::const_iterator iterator
        ) : index_(index), data_(data), iterator_(iterator) {}
        std::size_t index_;
        const std::vector<std::forward_list<T>>& data_;
        typename std::forward_list<T>::const_iterator iterator_;
    };
    Iterator Begin() {
        std::size_t index = 0;
        auto iterator = std::begin(data_[index]);
        while(index+1 < data_.size() && iterator == std::end(data_[index])) {
            index++;
            iterator = std::begin(data_[index]);
        }
        return {index, data_, std::begin(data_[index])};
    }
    Iterator End() {
        std::size_t index = data_.size() - 1;
        return {index, data_, std::end(data_[index])};
    }
  private:
    std::vector<std::forward_list<T>> data_;
};

// * 替int型態特化的SetOptimize類別模板，這個是int的專用版本，此處是去改寫Hash函式，
//   有此專用版本的前提是要有通用的SetOptimize類別模板。
// * 特化的概念就是定義模板參數是特定引數(如int)時要產生特定版本，
//   可以做到替某些型態優化的功效。例如：std::vector<bool>就是
//   std::vector針對bool型態去做特別優化的例子，和一般普通型別
//   的std::vector<T>不一樣。
template<>
class SetOptimize<int> {
  public:
    SetOptimize(): data_(6) {}
    std::size_t Hash(const int& value) {   // * 自定義雜湊函數，針對int的版本做特化，
                                           //   讓所有的value都會對應到一個
                                           //   [0, data_.size()-1]範圍內的一個整數值。
        int n = data_.size();
        return (value % n + n) % n;        // * 這裡 + n 的目的是為了要處理負數，確保它是正的。
    }
    bool Contains(const int& value) {
        auto& bucket = data_[Hash(value)];
        auto  result = std::find(std::begin(bucket), std::end(bucket), value);
        return result != end(bucket);
    }
    void Insert(const int& value) {
        auto& bucket = data_[Hash(value)];
        bucket.push_front(value);
    }
    void Insert(int&& value) {
        auto& bucket = data_[Hash(value)];
        bucket.push_front(std::move(value));
    }
    void Erase(const int& value) {
        auto& bucket = data_[Hash(value)];
        bucket.remove(value);
    }
    class Iterator {
        friend class SetOptimize<int>;
      public:
        bool operator!=(const Iterator& other) {
            return iterator_ != other.iterator_;
        }
        Iterator operator++() {
            ++iterator_;
            while(iterator_ == std::end(data_[index_]) &&
                  index_+1 < data_.size()) {
                ++index_;
                iterator_ = std::begin(data_[index_]);
            }
            return *this;
        }
        int operator*() {
            return *iterator_;
        }
      private:
        Iterator(
            std::size_t index,
            const std::vector<std::forward_list<int>>& data,
            typename std::forward_list<int>::const_iterator iterator
        ) : index_(index), data_(data), iterator_(iterator) {}
        std::size_t index_;
        const std::vector<std::forward_list<int>>& data_;
        typename std::forward_list<int>::const_iterator iterator_;
    };
    Iterator Begin() {
        std::size_t index = 0;
        auto iterator = std::begin(data_[index]);
        while(index+1 < data_.size() && iterator == std::end(data_[index])) {
            index++;
            iterator = std::begin(data_[index]);
        }
        return {index, data_, std::begin(data_[index])};
    }
    Iterator End() {
        std::size_t index = data_.size() - 1;
        return {index, data_, std::end(data_[index])};
    }
  private:
    std::vector<std::forward_list<int>> data_;
};

int main() {

    Set<int> s;
    s.Insert(3);
    s.Insert(5);
    s.Insert(2);

    std::cout << s.Contains(3) << std::endl;

    s.Erase(3);

    std::cout << s.Contains(3) << std::endl;

    Set<std::string> ss;
    std::string k1 = "Mary", k2 = "John", k3 = "Bob", k4 = "Any";
    ss.Insert(k1);
    ss.Insert(k4);
    ss.Insert(k2);
    std::cout << ss.Contains(k1) << std::endl;
    std::cout << ss.Contains(k4) << std::endl;
    std::cout << ss.Contains(k2) << std::endl;
    ss.Erase(k1);
    std::cout << ss.Contains(k1) << std::endl;
    std::cout << ss.Contains(k4) << std::endl;
    std::cout << ss.Contains(k2) << std::endl;
    ss.Erase(k4);
    ss.Erase(k2);

    ss.Insert(std::move(k1));
    ss.Insert(std::move(k4));
    ss.Insert(std::move(k2));
    ss.Insert(std::move(k3));
    std::cout << ss.Contains(k1) << std::endl;  // * k1變數內部資料被轉移至ss，因此
    std::cout << ss.Contains(k4) << std::endl;  //   此時搜索的k1是去搜索""。
    std::cout << ss.Contains(k2) << std::endl;
    std::cout << ss.Contains(k3) << std::endl;

    std::cout << "{";
    for(auto p = Begin(ss); p != End(ss); p++) {
        if(p != Begin(ss)) std::cout << ", ";
        std::cout << *p;
    }
    std::cout << "}" << std::endl;


    SetOptimize<int> soi;
    soi.Insert(9);
    soi.Insert(3);
    soi.Insert(7);
    soi.Insert(-2);

    std::cout << soi.Contains(9) << std::endl;
    std::cout << soi.Contains(3) << std::endl;

    soi.Erase(3);

    std::cout << soi.Contains(9) << std::endl;
    std::cout << soi.Contains(3) << std::endl;
    std::cout << "{";
    for(auto p = Begin(soi); p != End(soi); ++p) {
        if(p != Begin(soi)) std::cout << ", ";
        std::cout << *p;
    }
    std::cout << "}" << std::endl;
    Print(soi);

    SetOptimize<std::string> sos;
    std::string member1 = "Mary";
    std::string member2 = "John";
    sos.Insert(member1);
    sos.Insert(std::move(member2));
    sos.Insert("Jeffery");
    std::cout << sos.Contains(member1) << std::endl;
    std::cout << sos.Contains(member2) << std::endl;
    Print(sos);

    /* unordered_map */
    std::unordered_map<std::string, int> ma;
    ma["Mary"] = 7;
    ma["John"] = 3;
    ma["Alan"] = 4;
    std::cout << ma["Mary"] << ", " << ma["Alan"] << std::endl;

    for(auto p = std::begin(ma); p != std::end(ma); p++) {
        std::cout << p->first << ", " << p->second << std::endl;
    }
    for(const auto& p: ma) {
        // p => std::pair, p.first  => the first element (key), 
        //                 p.second => the second element (content)
        std::cout << p.first << ", " << p.second << std::endl;
    }
    for(auto& [key, val]: ma) {
        std::cout << key << ", " << val << std::endl;
    }

    return 0;
}