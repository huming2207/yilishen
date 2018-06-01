//
// Created by Ming Hu on 31/5/18.
//

#ifndef YILISHEN_COMMAND_H
#define YILISHEN_COMMAND_H

#include <string>

#define YEELIGHT_CMD_PORT           55443
#define YEELIGHT_CMD_ID             9527
#define YEELIGHT_RESULT_LENGTH      256

class Command
{
    public:
        explicit Command(std::string addr);
        void sendCommand(std::string& method, std::vector<std::string>& values);
        void receiveResult();

    private:
        void establishConnection(std::string& addr);
        std::string generateCommand(std::string& method, std::vector<std::string>& values);
        int socket_fd = 0;
};

#endif //YILISHEN_COMMAND_H
