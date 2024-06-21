class Thing
{
private:
    int m_number{};

public:
    Thing(int number)
        : m_number(number)
    {
    }
    void printNumber();
};