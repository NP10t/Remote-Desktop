#include <iostream>
#include <thread>
#include <cstdlib>

int x = 1;

void MyFunction(int i)
{
    if(i == x)
    std::cout << "Hi bang x" << std::endl;
    else
    std::cout << "i khac x";
}

int main()
{
    std::thread myThread;
    int i = 0;
    while (true)
    {
        if (!myThread.joinable())
        {
            myThread = std::thread(MyFunction, i);
            i += 1;
            myThread.detach();
        }
    }

    return 0;
}