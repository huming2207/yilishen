/*
 * YiLiShen Yeelight SSDP discovery tool, originally from: https://github.com/kallisti5/libmicrossdp
 * Modified for C++ and Yeelight discovery protocol
 * The style is still way too C or Java, need to be refactored soon...
 *
 * libmicrossdp
 * Copyright 2014-2015 Alexander von Gluck IV <kallisti5@unixzen.com>
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 *  "Jackson" Ming Hu <huming2207@gmail.com>
 */

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <ctime>
#include <cerrno>

#include "Prober.h"

std::map<unsigned long long, Device> Prober::discoverDevices()
{
    struct sockaddr_in groupSock{};
    struct sockaddr_in localSock{};
    struct ip_mreq multicast_group{};

    // Initialize UDP socket
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // If socket failed to create, then just exit
    if(udpSocket == -1) {
        fprintf(stderr, "Oops! Socket cannot be created, reason: %s\n", strerror(errno));
        exit(1);
    }

    // Reuse socket
    int reuse = 1;
    setsockopt(udpSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

    // 100ms second timeout on recvfrom
    struct timeval tv{};
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    char loopch = 0;
    setsockopt(udpSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loopch, sizeof(loopch));

    groupSock.sin_family = AF_INET;
    groupSock.sin_port = htons(1982);
    groupSock.sin_addr.s_addr = inet_addr("239.255.255.250");

    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(1982);
    localSock.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if(bind(udpSocket, (struct sockaddr*)&localSock, sizeof(localSock)) == -1) {
        fprintf(stderr, "Oops! Failed to bind socket, reason: %s", strerror(errno));
        exit(1);
    }

    // Prepare group
    multicast_group.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    setsockopt(udpSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&multicast_group, sizeof(multicast_group));

    // Form M-SEARCH message
    char search[SSDP_PACKET_BUFFER] = "\0";
    snprintf(search, SSDP_PACKET_BUFFER,
             "M-SEARCH * HTTP/1.1\r\n"
             "HOST: %s:%d\r\n"
             "ST: %s\r\n"
             "MAN: \"ssdp:discover\"\r\n",
             "239.255.255.250", 1982, BULB_SEARCH_TARGET);
    printf("Query:\n%s", search);

    // Send multicast package
    if(sendto(udpSocket, search, SSDP_PACKET_BUFFER, 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) == -1) {
        fprintf(stderr, "Oops! Cannot run send broadcast request, reason: %s", strerror(errno));
        exit(1);
    }

    // Wait and parse devices for 3 seconds
    time_t start = time(nullptr);
    int device_count = 0;
    while ((time(nullptr) - start) <= 3) {
        struct sockaddr_in si_other{};
        socklen_t slen = sizeof(si_other);
        char buffer[SSDP_PACKET_BUFFER] = "";
        recvfrom(udpSocket, buffer, SSDP_PACKET_BUFFER, 0, (struct sockaddr*)&si_other, &slen);

        // HTTP/1.1's are responses. NOTIFY are random broadcasts
        if (strstr(buffer, "HTTP/1.1 200 OK") != nullptr) {
            printf("------------------\n");
            printf("Got response:\n");
            printf("%s\n", buffer);
            parseSsdpResponse(buffer);
            device_count++;

            // Reset timer when response
            start = time(nullptr);
        }
    }

    shutdown(udpSocket, 0);

    return this->deviceList;
}

void Prober::parseSsdpResponse(char *buffer)
{
    std::istringstream bufferString(buffer);
    std::string key, value;

    Device device{};

    // Read buffer line by line
    while(std::getline(std::getline(bufferString, key, ':') >> std::ws, value)) {

        // Remove '\r' and the very first empty space if possible (length longer than 2)
        value.erase(value.length() - 1);

        // Do a long if-else...(probably there's a better way to do this?)
        if(key == "Location") device.location = value;
        else if(key == "model") device.model = value;
        else if(key == "fw_ver") device.fw_ver = value;
        else if(key == "power") device.power = (value == "on");
        else if(key == "bright") device.brightness = std::stoi(value);
        else if(key == "color_mode") device.color_mode = std::stoi(value);
        else if(key == "ct") device.color_temp = std::stoi(value);
        else if(key == "hue") device.color_hue = std::stoi(value);
        else if(key == "sat") device.color_sat = std::stoi(value);
        else if(key == "name") device.name = (value.length() <= 2 ? "" : value); // Just in case if name is empty
        else if(key == "id") device.id = std::stoull(value.erase(0, 2));
    }

    // Only adds the device that not exist
    if(this->deviceList.count(device.id) <= 0) {
        this->deviceList[device.id] = device;
    }

}

Prober::Prober()
{
    this->deviceList = std::map<unsigned long long, Device>();
}
