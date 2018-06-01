//
// Created by Ming Hu on 31/5/18.
//

#include <iostream>
#include <sstream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "Command.h"

using namespace rapidjson;

Command::Command(std::string addr)
{
    this->establishConnection(addr);
}

void Command::sendCommand(std::string& method, std::vector<std::string>& values)
{
    std::string commandPayload = this->generateCommand(method, values);
    if(send(this->socket_fd, commandPayload.c_str(), commandPayload.length(), 0) < 0) {
        std::cerr << "Oops, failed to send off payload, reason: "  << strerror(errno) <<  std::endl;
        exit(1);
    }
}

void Command::receiveResult()
{
    char socketRecvBuffer[YEELIGHT_RESULT_LENGTH] = {"\0"};
    if(read(this->socket_fd, socketRecvBuffer, YEELIGHT_RESULT_LENGTH) < 0) {
        std::cerr << "Oops, failed to read content, reason: "  << strerror(errno) <<  std::endl;
        exit(1);
    }

    std::cout << "Got result: "
              << socketRecvBuffer
              << "\n" << std::endl;
}

void Command::establishConnection(std::string& addr)
{
    struct sockaddr_in sockAddr{};

    // Create socket
    if((this->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Oops, failed to create socket, reason: "  << strerror(errno) <<  std::endl;
        exit(1);
    }

    // Prepare connect()
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    sockAddr.sin_port = htons(YEELIGHT_CMD_PORT);

    if(connect(this->socket_fd, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) {
        std::cerr << "Oops, failed to perform connect(), reason: "  << strerror(errno) << std::endl;
        exit(1);
    }
}

std::string Command::generateCommand(std::string& method, std::vector<std::string>& values)
{
    Document document;
    document.SetObject();

    // Get allocator
    Document::AllocatorType& allocator = document.GetAllocator();

    // Add ID
    document.AddMember("id", YEELIGHT_CMD_ID, allocator);

    // Add method
    document.AddMember("method", StringRef(method.c_str()), allocator);

    // Add command value array
    Value array(rapidjson::kArrayType);

    if(values.size() > 2) {
        for(auto const& value : values) {

            // Try parse to int first. If no integer found, then just add as string
            try {
                int possibleDigit = std::stoi(value);
                array.PushBack(possibleDigit, allocator);
            } catch (std::invalid_argument& notNumeric) {
                array.PushBack(StringRef(value.c_str()), allocator);
            }
        }
    }

    // Append array to JSON payload
    document.AddMember("params", array, allocator);

    // Manipulate JSON payload
    StringBuffer stringBuffer;
    Writer<StringBuffer> writer(stringBuffer);
    document.Accept(writer);

    std::string jsonCmdPayload = stringBuffer.GetString();
    jsonCmdPayload += "\r\n"; // Yeelight protocol requires every command add a "\r\n" in the end to terminate
    return jsonCmdPayload;
}


