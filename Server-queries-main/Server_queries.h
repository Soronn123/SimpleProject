#pragma once


#include "Request.h"
#include "ParserMessage.h"
#include "CustomException.h"

#include <string>

//Server queries namespace
namespace quer
{

    static void ShowInfo(infoServer infServ)
    {
        if (infServ.header != '\x49')
            throw CustomException((char*)"fucntion ShowInfo()");

        printf("\nUnpuck UDP-datagam \n\tA2S_INFO\nBasic information about the server. \n");
        //print hex as xx
        printf("Header: %2.2x \n", infServ.header);

        printf("Protocol: %2.2x \n", infServ.protocol);
        //print char array
        printf("Name: %s\n", infServ.name);

        printf("Map: %s\n", infServ.map);

        printf("Folder: %s\n", infServ.folder);

        printf("Game: %s\n", infServ.game);

        printf("ID: %hd \n", infServ.id);

        printf("Player: %2.2x \n", infServ.players);

        printf("Max players: %2.2x \n", infServ.maxPlayers);

        printf("Bots: %2.2x \n", infServ.bots);

        printf("Server type: %2.2x \n", infServ.serverType);

        printf("Environment: %2.2x \n", infServ.environment);

        printf("Visibility: %2.2x \n", infServ.visibility);

        printf("Vac: %2.2x \n", infServ.vac);

        printf("Version: %s\n", infServ.version);

        printf("Edf: %2.2x \n", infServ.edf);

        printf("Port: %hd \n", infServ.port);

        printf("SteamID: %lld \n", infServ.steamID);

        printf("PortSpect: %hd \n", infServ.portSpect);
        printf("NameSpect: %s\n", infServ.nameSpect);

        printf("KeyWords: %s\n", infServ.keyWords);

        printf("Edf: %lld \n \n", infServ.gameID);
    }


    static void ShowPlayer(playerServer plrServ)
    {
        if (plrServ.header != '\x44')
        {
            printf("plrServ is empty \n");
            return;
        }

        printf("\nUnpuck UDP-datagam \n\tA2S_PLAYER\nDetails about each player on the server. \n");

        printf("Header: %2.2x \n", plrServ.header);

        printf("Player: %d \n", plrServ.players);

        for (size_t i = 0; i < plrServ.players; i++)
        {
            printf("Index: %zu \n", i);

            printf("Name: %s\n", plrServ.chunk[i].name);
            // print long int
            printf("Score: %ld \n", plrServ.chunk[i].score);
            //using .2 after dot
            printf("Duration: %.2f\n", plrServ.chunk[i].duration);

        }
        printf("\n");
    }


    static void ShowRules(rulesServer rlsServ)
    {
        if (rlsServ.header != '\x45')
        {
            printf("rlsServ is empty \n");
            return;
        }

        printf("\nUnpuck UDP-datagam \n\tA2S_RULES\nThe rules the server is using \n");

        printf("Header: %2.2x \n", rlsServ.header);

        printf("Rules: %hu \n", rlsServ.rules);

        for (int i = 0; i < rlsServ.rules; ++i)
        {
            printf("%s : %s\n", rlsServ.chunk[i].name, rlsServ.chunk[i].value);
        }
        printf("\n");
    }


    //3 qirues to server and return;
    int getAll(int port, std::string ipserver)
    {
        infoServer infServ{ 0 };
        playerServer plrServ{ 0 };
        rulesServer	rlsServ{ 0 };

        RequestValve reqval(port, ipserver);
        ParserMessage parser;

        infServ = parser.parseInfoServer(reqval.A2S_INFO());
        plrServ = parser.parsePlayerServer(reqval.A2S_PLAYER());
        rlsServ = parser.parseRulesServer(reqval.A2S_RULES());

        ShowInfo(infServ);
        ShowPlayer(plrServ);
        ShowRules(rlsServ);

        return 0;
    }
}