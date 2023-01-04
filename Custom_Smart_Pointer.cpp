#include <iostream>
#include <vector>
#include <memory>
#include <string>
class LargeObject{
  public:
    LargeObject() {
        std::cout << "Default Initialization." << std::endl;
    }
    LargeObject(const std::string& name) : name_(name) {
        std::cout << "Direct Initialization." << std::endl;
    }
    LargeObject(const LargeObject& other) {
        std::cout << "Copy Constructor" << std::endl;
        name_ = other.name_;
    }
    LargeObject(LargeObject&& other) {
        std::cout << "Move Constructor" << std::endl;
        name_ = other.name_;
        other.name_ = "";
    }
    LargeObject& operator=(const LargeObject& other) {
        std::cout << "Copy Assignment Operator" << std::endl;
        if(&other == this) return *this;
        name_ = other.name_;
        return *this;
    }
    LargeObject& operator=(LargeObject&& other) {
        std::cout << "Move Assignment Operator" << std::endl;
        if(&other == this) return *this;
        name_ = other.name_;
        other.name_ = "";
        return *this;
    }
    ~LargeObject() {}
    void Process() {
        std::cout << name_ << std::endl;
    }
  private:
    std::string name_ {};
    int arr_[1000];
};

template<typename T>
class UniquePtr{
  public:
    UniquePtr(T* ptr): ptr_(ptr) {}
    UniquePtr(UniquePtr&& other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    };
    UniquePtr& operator=(UniquePtr&& other) {
        if(&other == this) return *this;
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    };
    ~UniquePtr() {
        if(ptr_) {
            std::cout << "Destructing the UniquePtr..." << std::endl;
            delete ptr_;
        }
        // delete ptr_;  // * delete nullptr directly is OK and safe.
    }
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    T* operator->() {
        return ptr_;
    }
    T* get() {
        return ptr_;
    }
  private:
    T* ptr_ {};
};

template<typename T, typename ArgT>
UniquePtr<T> MakeUnique(ArgT arg) {
    UniquePtr<T> up {new T{arg}};  // NRVO
    return up;
}

template<typename T, typename ArgT>
UniquePtr<T> CreateLargeObjectUP(ArgT arg) {
    return MakeUnique<T>(arg);  // URVO
}

template<typename T>
class SharedPtr{
  public:
    SharedPtr(T* ptr): ptr_(ptr) {
        count_ = new int{1};
    }
    SharedPtr(const SharedPtr& other) {
        ptr_ = other.ptr_;
        count_ = other.count_;
        (*count_)++;
    }
    SharedPtr(SharedPtr&& other) {
        ptr_ = other.ptr_;
        count_ = other.count_;
        other.ptr_ = nullptr;
        other.count_ = 0;
    }
    SharedPtr& operator=(const SharedPtr& other) {
        if(&other == this) return *this;
        (*count_)--;
        if(*count_ == 0) {
            delete ptr_;
            delete count_;
        }
        ptr_ = other.ptr_;
        count_ = other.count_;
        count_++;
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) {
        if(&other == this) return *this;
        (*count_)--;
        if(*count_ == 0) {
            delete ptr_;
            delete count_;
        }
        ptr_ = other.ptr_;
        count_ = other.count_;
        other.ptr_ = nullptr;
        other.count_ = 0;
        return *this;
    }
    ~SharedPtr() {
        (*count_)--;
        if(*count_ == 0) {
            std::cout << "Destructing the SharedPtr..." << std::endl;
            delete ptr_;
            delete count_;
        }
    }
    bool operator==(const SharedPtr& other) const {
        return ptr_==other.ptr_;
    }
    T* operator->() {
        return ptr_;
    }
    T* get() {
        return ptr_;
    }
  private:
    T* ptr_ {};
    int* count_ {};
};

template<typename T, typename ArgT>
SharedPtr<T> MakeShared(ArgT arg) {
    SharedPtr<T> sp {new T{arg}};  // NRVO
    return sp;
}

template<typename T, typename ArgT>
SharedPtr<T> CreateLargeObjectSP(ArgT arg) {
    return MakeShared<T>(arg);  // URVO
}

int main() {
    std::unique_ptr<LargeObject> up1 = std::make_unique<LargeObject>("A");
    std::unique_ptr<LargeObject> up2 = std::make_unique<LargeObject>("B");
    std::unique_ptr<LargeObject> up3 = std::make_unique<LargeObject>("C");
    std::vector<std::unique_ptr<LargeObject>> upv;  // 使用std::unique_ptr的vector來自動管理內部大型資源
    upv.push_back(std::move(up1));
    upv.push_back(std::move(up2));
    upv.push_back(std::move(up3));
    for(auto& up: upv) {  // & (reference) is used only, since up (unique_ptr) 
                          // cannot be copied.
        up->Process();
    }
    // up1->Process(); // up1 is a nullptr after the std::move(up1).

    std::shared_ptr<LargeObject> sp1 = std::make_shared<LargeObject>("A");
    std::shared_ptr<LargeObject> sp2 = std::make_shared<LargeObject>("A");
    std::shared_ptr<LargeObject> sp3 = sp2;
    // sp1 and sp2 point to different memory locations, so sp1 != sp2.
    std::cout << std::boolalpha << (sp1 == sp2) << std::endl;
    std::cout << std::boolalpha << (sp2 == sp3) << std::endl;
    sp3->Process();

    std::cout << "=====================================" << std::endl;

    /* Custom Version */
    UniquePtr<LargeObject> UP1 = CreateLargeObjectUP<LargeObject>("A");
    UniquePtr<LargeObject> UP2 = CreateLargeObjectUP<LargeObject>("B");
    UniquePtr<LargeObject> UP3 = CreateLargeObjectUP<LargeObject>("C");
    std::vector<UniquePtr<LargeObject>> UPv;
    UPv.push_back(std::move(UP1));
    UPv.push_back(std::move(UP2));
    UPv.push_back(std::move(UP3));
    for(auto& UP: UPv) {  // & (reference) is used only, since UP (UniquePtr)
                          // cannot be copied.
        UP->Process();
    }
    // UP1->Process(); // UP1 is a nullptr after the std::move(UP1).

    SharedPtr<LargeObject> SP1 = CreateLargeObjectSP<LargeObject>("A");
    SharedPtr<LargeObject> SP2 = CreateLargeObjectSP<LargeObject>("A");
    SharedPtr<LargeObject> SP3 = SP2;
    // SP1 and SP2 point to different memory locations, so SP1 != SP2.
    std::cout << std::boolalpha << (SP1 == SP2) << std::endl;
    std::cout << std::boolalpha << (SP2 == SP3) << std::endl;
    SP3->Process();
    std::cout << "=====================================" << std::endl;

    return 0;
}