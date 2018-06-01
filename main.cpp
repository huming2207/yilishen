#include <iostream>
#include <vector>
#include "src/Prober.h"
#include "src/Command.h"

int main(int argc, char** argv)
{
    Prober prober;

    std::vector<std::string> cmdList;

    for(int argCounter = 0; argCounter < argc; argCounter += 1) {
        cmdList.emplace_back(std::string(argv[argCounter]));
    }

    for(auto const& deviceItem : prober.discoverDevices()) {
        auto const& device = deviceItem.second;
        std::cout << "Found a device at " << device.location << ", ID: " << device.id << std::endl;

        if(argc > 1) {
            Command command(device.location);
            std::string method(argv[1]);
            std::cout << "Sending command " << method << " to this device..." << std::endl;
            command.sendCommand(method, cmdList);
            command.receiveResult();

        } else {
            std::cout << "No action required on this YeeLight lamp" << std::endl;
        }
    }

    return 0;
}