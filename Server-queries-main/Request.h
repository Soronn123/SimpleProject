#pragma once


#include <vector>
#include <string>

#include <winsock2.h>
#include <Ws2tcpip.h>

#include <algorithm>

#include <cstdio>
#include <cstring>

#pragma comment (lib, "Ws2_32.lib")

#define PACKET_SIZE 1400

#include "Structures.h"
#include "CustomException.h"


namespace quer
{
    class RequestValve
    {
    private:
        u_short port = 0;
        std::string ip;

        sockaddr_in createAddress() const;
        SOCKET createSock(const int timeout);
        int closeSock(SOCKET sock);

        int sendMessage(SOCKET sock, const char* message, int lenMessage, sockaddr_in address, int lenaddr);
        bufPacket getMessage(SOCKET sock, sockaddr_in address, int lenaddr, int sizePacket);
        
        char* checkArgv(SOCKET sock, char* message, int lenMess, int newLen, sockaddr_in address, int lenaddr);
        char* request(char* message, uint16_t lenMess, uint16_t len2);


    public:
        char* A2S_INFO();
        char* A2S_PLAYER();
        char* A2S_RULES();
        int changeIP(u_short sPort, std::string sIp);
        RequestValve(u_short sPort, std::string sIp);
        ~RequestValve();
    };


    inline char* RequestValve::A2S_INFO()
    {
        // create char array what we need send to server
        // And get to function request message with len and len again

        // two len need only A2S_INFO because this function add new char if server requset challenge
        // it so bad idea ( I want change this but I think is more optimation than write code every protocol ) 
        char infoRequest[29] = { '\xFF', '\xFF', '\xFF', '\xFF', '\x54', '\x53', '\x6F', '\x75', '\x72',
                                '\x63', '\x65', '\x20', '\x45', '\x6E', '\x67', '\x69', '\x6E', '\x65', '\x20',
                                '\x51', '\x75', '\x65', '\x72', '\x79', '\x00', '\xAD', '\xAD', '\xAD', '\xAD' };
        char* info = request(infoRequest, 25, 29);
        return info;
    }


    inline char* RequestValve::A2S_PLAYER()
    {
        // len message stay as we change already char
        char playerRequest[9] = { '\xFF', '\xFF', '\xFF', '\xFF', '\x55', '\xFF', '\xFF', '\xFF', '\xFF' };
        char* info = request(playerRequest, 9, 9);
        return info;
    }


    inline char* RequestValve::A2S_RULES()
    {
        //also how A2S_PLAYER
        char rulesRequest[9] = { '\xFF', '\xFF', '\xFF', '\xFF', '\x56', '\xFF', '\xFF', '\xFF', '\xFF' };
        char* info = request(rulesRequest, 9, 9);
        return info;
    }


    inline RequestValve::RequestValve(u_short sPort, std::string sIp)
    {
        port = htons(sPort);
        ip = sIp;
    }


    inline RequestValve::~RequestValve()
    {
    }

}