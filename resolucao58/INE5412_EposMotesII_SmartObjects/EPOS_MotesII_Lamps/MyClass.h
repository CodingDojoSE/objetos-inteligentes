









class MyClass
{
public:

    int hi;
    
    MyClass();
    
    int get_hi();

};



MyClass::MyClass()
{
    this->hi = 5;
}

int MyClass::get_hi()
{
    return this->hi;
}




