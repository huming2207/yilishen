/*
 * YiLiShen Yeelight SSDP discovery tool, originally from: https://github.com/kallisti5/libmicrossdp
 * Modified for C++, but the style is still way too C or Java, need to be refactored soon...
 *
 * libmicrossdp
 * Copyright 2014-2015 Alexander von Gluck IV <kallisti5@unixzen.com>
 * Released under the terms of the MIT license
 *
 * Authors:
 *  Alexander von Gluck IV <kallisti5@unixzen.com>
 *  "Jackson" Ming Hu <huming2207@gmail.com>
 */

#ifndef YILISHEN_PROBER_H
#define YILISHEN_PROBER_H

#include <map>
#include <string>

#define SSDP_PACKET_BUFFER			512

#define BULB_SEARCH_TARGET          "wifi_bulb"

typedef struct _yl_devices
{
    std::string location;
    std::string model;
    std::string fw_ver;
    bool power;
    int brightness;
    int color_mode;
    int color_temp;
    unsigned long color_rgb;
    int color_hue;
    int color_sat;
    std::string name;
    unsigned long long id;
} Device;

class Prober
{
    public:
        Prober();
        std::map<unsigned long long, Device> discoverDevices();

    private:
        void parseSsdpResponse(char *buffer);
        std::map<unsigned long long, Device> deviceList;
};

#endif //YILISHEN_PROBER_H
