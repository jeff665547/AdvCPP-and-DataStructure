// * 變數是個有名稱的物件，佔據記憶體空間。
// * 對變數名稱做取址運算(&)可以取得該物件的記憶體"起始"位址，而該回傳的記憶體位址為一
//   種暫時物件，是右值表示式。
// * 記憶體位址的特性：
//   1. 每個物件從出生到死亡，它的記憶體皆不能被改變。
//   2. 記憶體位址複製成本很低(佔據的儲存空間小)，因此可以利用儲存記憶體的位址尋找到複雜
//      資料結構的物件，進而取代直接複製複雜資料結構的物件(通常所佔據的儲存空間也較大)。
// * 陣列型態物件可以隱性轉型成第一個元素的記憶體位址(指標)，並透過此種方式在函式之間傳遞，
//   與進行複製。
#include <iostream>
int main() {

    // C-style array initialization
    int a[3] = {1, 2, 3};  // * 使用初始化串列(initializer_list)來初始C風格陣列
    int b[3] {1, 2, 3};    // * 另一種初始化方式(將等號拿掉) => 較為現代的寫法
    for(int i = 0; i < 3; i++){
        std::cout << a[i] << std::endl;
        std::cout << b[i] << std::endl;
    }
    for(int v: b){
        std::cout << v << std::endl;
    }

    int* c = a;  // a -> &a[0] => int *c = &a[0]
    for(int i = 0; i < 3; i++){
        std::cout << c[i] << std::endl;
    }

    // 往後一個陣列元素型態後的位址。(e.g. 底下的 +1 代表 +1 個整數寬)
    std::cout << std::boolalpha << (&a[0] + 1 == &a[1]) << std::endl;
    std::cout << std::boolalpha << (c + 1 == &a[1]) << std::endl;
    std::cout << std::boolalpha << (*(c + 2) == a[2]) << std::endl;
    // * a[b] 運算等同於*(a+b)，反之亦然。
    // * c 雖然是一個指標，儲存a[0]的位址，但是用起來和原本的陣列a一樣。
    //   意即陣列元素的指標用起來像陣列。
    std::cout << std::boolalpha << (*(c + 2) == c[2]) << std::endl;
    std::cout << std::boolalpha << (c[0] == *c) << std::endl;
    std::cout << std::boolalpha << (c[0] == *a) << std::endl;
    std::cout << std::boolalpha << (c[2] == *(a + 2)) << std::endl;
    std::cout << std::boolalpha << (c[1] == a[1]) << std::endl;

    // * 將陣列元素歸零
    int v[10] = {0};  // * C語言規定，照理來說，要給10個元素，但這裡只給一個元素，
                      // 此一個元素會自動指派給陣列中的第一個元素，其他元素則是
                      // 自動補成0。相同道理，如果是使用{1, 3}來初始化陣列，則這
                      // 這兩個元素會自動分別指派到陣列中的第一個以及第二個元素，
                      // 其他元素則是自動補成0。
    int w[10] = {};   // * C++ 的歸零寫法。新版C++省略等號: int w[10] {};

    int y[10];
    for(int i = 0; i < 10; i++) {
        y[i] = 0;
    }
    int z[10];
    for(int *p = z; p < z+10; p++){
        *p = 0;
    }

    return 0;
}