#include <iostream>
#include "src/Prober.h"

int main(int argc, char** argv)
{
    Prober prober;

    std::map<unsigned long long, Device> deviceList = prober.discoverDevices();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}