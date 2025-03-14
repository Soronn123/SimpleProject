#pragma once

#include <stdint.h>


namespace quer
{
    struct multiPacket
    {
        long header; /* Always equal to -2 (0xFFFFFFFE). Means the packet is split. */
        long ID; /* Unique number assigned by server per answer. 
                    Same as the Goldsource server meaning. 
                    However, if the most significant bit is 1, 
                    then the response was compressed with bzip2 before being cut and sent. 
                    Refer to compression procedure below.*/
        int total; /* The total number of packets in the response. */
        int number; /* The number of the packet. Starts at 0. */
        short size; /* (Orange Box Engine and above only.) 
                    Maximum size of packet before packet switching occurs. 
                    The default value is 1248 bytes (0x04E0), 
                    but the server administrator can decrease this. 
                    For older engine versions: the maximum and minimum size 
                    of the packet was unchangeable. 
                    AppIDs which are known not to contain this field: 
                    215, 17550, 17700, and 240 when protocol = 7. */
        char* playload; /* just playload */
    };


    struct bufPacket
    {
        char* ptr; /* message from server */
        int size; /* size messagew */
    };


    struct infoServer
    {
        /* data */
        uint8_t header; /* Always equal to 'I' (0x49) */
        uint8_t protocol; /* Protocol version used by the server. */
        char* name; /* Name of the server. */
        char* map; /* Map the server has currently loaded. */
        char* folder; /* Name of the folder containing the game files. */
        char* game; /* Full name of the game. */
        short id; /* Steam Application ID of game. */
        int players; /* Number of players on the server. */
        int maxPlayers; /* Maximum number of players the server reports it can hold. */
        int bots; /* Number of bots on the server. */
        uint8_t serverType; /* Indicates the type of server:
                                        'd' for a dedicated server
                                        'l' for a non-dedicated server
                                        'p' for a SourceTV relay (proxy) */
        uint8_t environment; /* Indicates the operating system of the server:
                                        'l' for Linux
                                        'w' for Windows
                                        'm' or 'o' for Mac (the code changed after L4D1) */
        bool visibility; /* Indicates whether the server requires a password
                                        0 for public
                                        1 for private */
        bool vac; /* Specifies whether the server uses VAC: */
        char* version; /* Version of the game installed on the server. */
        uint8_t edf; /* If present, this specifies which additional data fields will be included. */
        short port; /* The server's game port number. */
        long long steamID; /* Server's SteamID. */
        short portSpect; /* Spectator port number for SourceTV. */
        char* nameSpect; /* Name of the spectator server for SourceTV. */
        char* keyWords; /* Tags that describe the game according to the server (for future use.) */
        long long gameID; /* The server's 64-bit GameID. If this is present,
                            a more accurate AppID is present in the low 24 bits.
                            The earlier AppID could have been truncated as it was forced into 16-bit storage. */
    };


    struct playerChunk
    {
        char* name; /* Name of the player. */
        long score; /* Player's score (usually "frags" or "kills".) */
        float duration; /* Time (in seconds) player has been connected to the server. */
    };


    struct playerServer
    {
        uint8_t header; /* Always equal to 'D' (0x44) */
        int players; /* Number of players whose information was gathered. */
        playerChunk* chunk; /* For every player in "Players" there is this chunk in the response: */
    };


    struct rulesChunk
    {
        char* name; /* Name of the rule. */
        char* value; /* Value of the rule. */
    };


    struct rulesServer
    {
        uint8_t header; /* Always equal to 'E' (0x45) */
        short rules; /* Number of rules in the response. */
        rulesChunk* chunk; /* For every rule in "Rules" there is this chunk in the response:  */
    };
}
