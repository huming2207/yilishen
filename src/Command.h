//
// Created by Ming Hu on 31/5/18.
//

#ifndef YILISHEN_COMMAND_H
#define YILISHEN_COMMAND_H

#include <string>

class Command
{
 public:
    explicit Command(std::string addr);
    void sendCommand(std::string method, std::string value);
};

#endif //YILISHEN_COMMAND_H
