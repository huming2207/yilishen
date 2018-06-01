//
// Created by Ming Hu on 31/5/18.
//

#ifndef YILISHEN_COMMAND_H
#define YILISHEN_COMMAND_H

#include <string>

#define YEELIGHT_CMD_PORT 55443
#define YEELIGHT_CMD_ID   9527

class Command
{
    public:
        explicit Command(std::string& addr);
        void sendCommand(std::string& method, std::string& value);

    private:
        void establishConnection(std::string& addr);
        std::string& generateCommand(std::string& method, std::string& values);
        int socket_fd = 0;
};

#endif //YILISHEN_COMMAND_H
