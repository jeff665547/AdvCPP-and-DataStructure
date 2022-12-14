#include <iostream>
#include <string>
void f1(std::string& s) {};
void f2(const std::string& s) {};
void f3(std::string&& s) {};
void f4(const std::string&& s) {};

void fT(std::string& s) { std::cout << "std::string& s" << std::endl; };
void fT(const std::string& s) { std::cout << "const std::string& s" << std::endl; };
void fT(std::string&& s) { std::cout << "std::string&& s" << std::endl; };
void fT(const std::string&& s) { std::cout << "const std::string&& s" << std::endl; };

int main() {
    std::string s("Hi");        // s  is a  lvalue       expression
    const std::string cs("Hi"); // cs is a  const lvalue expression
    std::move(s);               // <- is an rvalue       expression
    std::move(cs);              // <- is a  const rvalue expression

    f1(s);              // OK
    f1(cs);             // ERROR
    f1(std::move(s));   // ERROR 
    f1(std::move(cs));  // ERROR

    f2(s);              // OK
    f2(cs);             // OK
    f2(std::move(s));   // OK
    f2(std::move(cs));  // OK


    f3(s);              // ERROR 
    f3(cs);             // ERROR 
    f3(std::move(s));   // OK
    f3(std::move(cs));  // ERROR

    f4(s);              // ERROR 
    f4(cs);             // ERROR 
    f4(std::move(s));   // OK
    f4(std::move(cs));  // OK


    fT(s);              // call std::string& s
    fT(cs);             // call const std::string& s
    fT(std::move(s));   // call std::string&& s
    fT(std::move(cs));  // call const std::string&& s

    return 0;
}