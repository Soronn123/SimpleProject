#include "Request.h"

namespace quer
{
    char* RequestValve::request(char* message, uint16_t lenMess, uint16_t len2)
    {
        SOCKET sock = createSock(2000);
        sockaddr_in address = createAddress();
        int lenaddr = sizeof(address);

        char* result = checkArgv(sock, message, lenMess, len2, address, lenaddr);

        closeSock(sock);

        return result;
    }


    int RequestValve::closeSock(SOCKET sock)
    {
        int clsSock = closesocket(sock);
        if (clsSock == SOCKET_ERROR) 
            throw CustomException((char*)"Class RequestValve fucntion closeSock Close socket: ", WSAGetLastError());

        int wsas = WSACleanup();
        if (wsas == SOCKET_ERROR)
            throw CustomException((char*)"Class RequestValve fucntion closeSock Close Winsock.dll: ", WSAGetLastError());
        return 0;
    }


    char* RequestValve::checkArgv(SOCKET sock, char* message, int lenMess, int newLen, sockaddr_in address, int lenaddr)
    {
        std::vector<bufPacket> packeti;
        char* ptrPack = nullptr;

        sendMessage(sock, message, lenMess, address, lenaddr);

        packeti.push_back(getMessage(sock, address, lenaddr, PACKET_SIZE));

        long header;
        std::memcpy(&header, packeti[0].ptr, sizeof(header));
        if (header == -1)
        {
            if (packeti[0].ptr[4] == '\x41')
            {
                message[newLen - 1] = packeti[0].ptr[8];
                message[newLen - 2] = packeti[0].ptr[7];
                message[newLen - 3] = packeti[0].ptr[6];
                message[newLen - 4] = packeti[0].ptr[5];

                ptrPack = checkArgv(sock, message, newLen, newLen, address, lenaddr);
            }
            else
            {
                //cast header (type long - 4 uint8_t)
                ptrPack = packeti[0].ptr + 4;
            }

        }
        else if (header == -2)
        {
            // long + long
            int total = (int)packeti[0].ptr[4 + 4];
            int sizePack = 0;
            long origid;
            memcpy(&origid, packeti[0].ptr + 4, 4);

            for (size_t i = 0; i < total - 1; i++)
                packeti.push_back(getMessage(sock, address, lenaddr, PACKET_SIZE));

            multiPacket* arrayPack = new multiPacket[total];
            for (size_t k = 0; k < total; k++)
            {
                int i = 0;
                memset(&arrayPack[k], 0, sizeof(arrayPack[k]));

                memcpy(&arrayPack[k].header, packeti[k].ptr + i, sizeof(arrayPack[k].header));
                if (arrayPack[k].header != -2)
                    throw CustomException((char*)"Class RequestValve fucntion checkArgv Unknow header");
                i += sizeof(arrayPack[k].header);

                memcpy(&arrayPack[k].ID, packeti[k].ptr + i, sizeof(arrayPack[k].ID));
                if (origid != arrayPack[k].ID)
                    throw CustomException((char*)"Class RequestValve fucntion checkArgv Unknow ID");
                i += sizeof(arrayPack[k].ID);

                arrayPack[k].total = (int)packeti[k].ptr[i++];

                arrayPack[k].number = (int)packeti[k].ptr[i++];

                memcpy(&arrayPack[k].size, packeti[k].ptr + i, sizeof(arrayPack[k].size));
                i += sizeof(arrayPack[k].size);
                sizePack = sizePack + arrayPack[k].size;

                arrayPack[k].playload = packeti[k].ptr + i;
            }

            for (int i = 0; i < total - 1; i++) {
                for (int j = 0; j < total - i - 1; j++) {
                    // Swap if the element found is
                    // greater than the next element
                    if (arrayPack[j].number > arrayPack[j + 1].number)
                    {
                        std::swap(arrayPack[j], arrayPack[j + 1]);
                    }
                }
            }
            // sizePack have header as -1 and we delete 4 byte (long)
            ptrPack = new char[sizePack - 4];
            int pos = 0;
            for (size_t j = 4; j < arrayPack[0].size; j++)
                ptrPack[pos++] = arrayPack[0].playload[j];


            for (size_t i = 1; i < total; i++)
            {
                for (size_t j = 0; j < arrayPack[i].size; j++)
                    ptrPack[pos++] = arrayPack[i].playload[j];
            }

        }
        else
            throw CustomException((char*)"Class RequestValve fucntion checkArgv Unknow type");


        return ptrPack;
    }


    int RequestValve::sendMessage(SOCKET sock, const char* message, int lenMessage, sockaddr_in address, int lenaddr)
    {
        int sendResult = sendto(sock, message, lenMessage, 0, (SOCKADDR*)&address, lenaddr);
        if (sendResult == SOCKET_ERROR)
            throw CustomException((char*)"Class RequestValve fucntion sendMessage: ", WSAGetLastError());
        return 0;
    }


    bufPacket RequestValve::getMessage(SOCKET sock, sockaddr_in address, int lenaddr, int sizePacket)
    {
        bufPacket buf{};

        buf.ptr = new char[sizePacket];
        memset(buf.ptr, 0, sizePacket);

        buf.size = recvfrom(sock, buf.ptr, sizePacket, 0, (struct sockaddr*)&address, &lenaddr);
        if (buf.size == SOCKET_ERROR)
            throw CustomException((char*)"Class RequestValve fucntion getMessage: ", WSAGetLastError());
        return buf;
    }


    SOCKET RequestValve::createSock(const int timeout)
    {
        WSADATA wsaData;

        /* Initialization winsock DLL (First function for using winsocket!!!) */
        int wsas = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsas != 0)
            throw CustomException((char*)"Class RequestValve fucntion createSock Initialization winsock DLL: ", WSAGetLastError());

        SOCKET socketApp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (socketApp == INVALID_SOCKET)
            throw CustomException((char*)"Class RequestValve fucntion createSock Initialization socket", WSAGetLastError());

        /* Set socket options */
        int setSock = setsockopt(socketApp, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));
        if (setSock == SOCKET_ERROR)
            throw CustomException((char*)"Class RequestValve fucntion createSock Set socket", WSAGetLastError());

        return socketApp;
    }


    sockaddr_in RequestValve::createAddress() const
    {
        /* Create an address that responce the data */
        sockaddr_in address;
        ZeroMemory(&address, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = port;
        std::wstring stemp = std::wstring(ip.begin(), ip.end());
        int ipe = InetPton(AF_INET, stemp.c_str(), &address.sin_addr.s_addr);
        if (ipe == -1)  
            throw CustomException((char*)"Class RequestValve fucntion createAddress InetPton");

        return address;
    }


    int RequestValve::changeIP(u_short sPort, std::string sIp)
    {
        port = htons(sPort);
        ip = sIp;

        return 0;
    }
}