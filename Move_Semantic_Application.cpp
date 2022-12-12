#include <iostream>
#include <string>
#include <vector>
#include <memory>
class LargeObject{
  public:
    LargeObject(std::string name) : name_(name) { std::cout << "Direct Construction" << std::endl; }
    LargeObject( const LargeObject& ) { std::cout << "Copy Construction" << std::endl; }
    ~LargeObject() { std::cout << "Destruction" << std::endl; }
    void Process() { std::cout << "Do Something" << std::endl; }
  private:
    std::string name_;
    int garbage[1000];
};

int main() {

    /* Move Semantics */
    std::string s1 = "ABC";
    std::string s2 = s1;             // * 根據C++規定去呼叫 std::string s2(s1); 因為s1是一個變數(lvalue 
                                     //   expression)，因此呼叫複製建構子來複製建構s2，也就是說s2為s1的複製品。
    std::string s3(s1);              // * 同上。
    std::string s4{s1};              // * 同上。
    s1[0] = 'a';                     // * 因為std::string是const char的陣列，所以裡面的元素都是char型態。
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl;

    std::string s5 = std::move(s1);  // * 因為std::move(s1)的結果是一種rvalue expression，因此會呼叫移動建構子
                                     //   來移動建構s1，s1底下的C風格字元陣列資源將會被移動到s5底下，
                                     //   s1接著就會變成nullptr，而s5就會擁有s1內部的資源。
    std::cout << s1 << std::endl;
    std::cout << s5 << std::endl;


    /* Fundamental Types */
    int i1 = 3;
    int i2 = i1;                     // * 根據C++規定去呼叫 int i2(i1); 呼叫複製建構子。
    int i3 = std::move(i1);          // * 雖然std::move(i1)的結果是一種rvalue expression，但是因為int為一種
                                     //   基礎型態(fundamental types)，因此其並沒有移動建構子以及移動賦值運算子，
                                     //   會自動退化成複製建構子以及複製賦值運算子，在這裡是使用複製建構子來建構
                                     //   i3。
    std::cout << i1 << std::endl;    // * i1不會受到影響。
    std::cout << i3 << std::endl;


    /* std::vector, std::unique_ptr, and std::move() */
    LargeObject a{"A"};
    LargeObject b{"B"};
    LargeObject c{"C"};
    std::vector<LargeObject> v;
    v.push_back(a);                  // * push_back函式因為傳入參數為一個變數(lavlue expression)，因此會使用
    v.push_back(b);                  //   copy constructor建構std::vector裡面的元素。
    v.push_back(c);                  // * std::vector因為其內部為C風格陣列，而且是會根據陣列大小來動態產生該陣列，
                                     //   因此其在push_back的過程中會發生複製(複製建構)以及刪除(解構)的情況。
                                     //   也因為有多次大型物件複製 => 效率低。
                                     // * 這裡即使是使用std::move()(e.g. std::move(a))也是一樣會使用複製的方式，
                                     //   因為LargeObject沒有實作移動建構子，會自動使用複製建構的方式。
    for(auto& p: v) {
        p.Process();
    }

    // auto pa = new LargeObject{"A"};  // * auto <=> LargeObject*
    // auto pb = new LargeObject{"B"};
    // auto pc = new LargeObject{"C"};
    // std::vector<LargeObject*> vLp;
    // vLp.push_back(pa);               // * push_back函式因為傳入參數為一個變數(lavlue expression)，因此會使用
    // vLp.push_back(pb);               //   copy constructor建構std::vector裡面的元素。但是在這裡是對指標進行
    // vLp.push_back(pc);               //   複製建構，也就是說是對位址進行複製而不是直接複製LargeObject，這樣做
    //                                  //   可以大幅度地增進效率，節省空間以及時間。
    //                                  // * 此種作法只有new但卻沒有delete，會造成memory leak。
    // for(auto& p: vLp) {
    //     p->Process();
    // }

    auto pa = std::make_unique<LargeObject>("A");  // * auto <=> std::unique_ptr<LargeObject>
    auto pb = std::make_unique<LargeObject>("B");
    auto pc = std::make_unique<LargeObject>("C");
    std::vector<std::unique_ptr<LargeObject>> upv;
    upv.push_back(std::move(pa));       // * 這邊一定要加上std::move()，因為std::unique_ptr不允許複製，也不允許
    upv.push_back(std::move(pb));       //   共享，只能移動，並且在移動後放棄內部資源的管理權。
    upv.push_back(std::move(pc));       // * 透過std::unique_ptr來進行內部資源的自動管理，其內部資源會在
                                        //   std::vector的元素解構時(也就是std::vector解構時)一起釋放。
                                        // * 除了使用std::move以外，std::vector內的元素型態(std::unique_ptr)也
                                        //   要支援移動建構才能讓std::unique_ptr內部資源順利轉移(由pa轉移至
                                        //   std::vector內的元素)。
    for(auto& p: upv) {
        p->Process();
    }

    std::string sa = "A";
    std::string sb = "B";
    std::string sc = "C";
    const std::string& sd = sc;
    std::vector<std::string> sv;
    sv.push_back(std::move(sa));         // * sa在轉移內部資源(C風格字元陣列)後就變成了空字串。
    sv.push_back(std::move(sb));         // * std::vector的元素建構時會優先呼叫該元素的移動建構子，因為
                                         //   此時的引數(std::move(sa))是個rvalue expression。
    sv.push_back(std::move(sd));         // * sd 是個const lvalue expression，因此std::move(sd)是個
                                         //   const rvalue expression，其型態為const std::string&&
                                         //   但因為push_back()只有支援rvalue expression的版本，也就是
                                         //   (push_back(T&&))，而T&&無法使用const rvalue expression
                                         //   進行初始化(要const T&&才可以)，因此會改採用
                                         //   push_back(const T&)的版本(const T&可以使用const rvalue 
                                         //   expression進行初始化)。也就是說在此處雖然使用了
                                         //   std::move()，但是實際上其行為還是去複製一份，而非去轉移內部
                                         //   資源的擁有權。也因此sc和sd在經過這行的操作後，其內部所儲存的
                                         //   資料仍然維持不變，想轉移內部資料擁有權只能使用std::move(sc)。
    for(const auto& p: sv) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    std::cout << "(a, b, c) = (" << sa << ", " << sb << ", " << sc << ")" << std::endl;

    return 0;
}