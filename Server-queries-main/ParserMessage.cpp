#include "ParserMessage.h"


namespace quer
{
    char* ParserMessage::getString(char* data, int& i)
    {
        char* ptr;
        int sizeStr = 0;

        while (data[sizeStr++] != '\0') {}

        ptr = new char[sizeStr];
        memcpy(ptr, data, sizeStr);

        i = i + sizeStr;

        return ptr;
    }


    ParserMessage::ParserMessage()
    {
    }


    ParserMessage::~ParserMessage()
    {
    }


    infoServer ParserMessage::parseInfoServer(char* dataResult)
    {
        // use i as pos in char array
        // add i = i++
        int i = 0;

        //check correct message3
        if (dataResult[i] != '\x49')
            throw CustomException((char*)"Class ParserMessage fucntion parseInfoServer inccorrect header");

        infoServer infoServer;
        memset(&infoServer, 0, sizeof(infoServer));

        memcpy(&infoServer.header, dataResult + i++, sizeof(infoServer.header));

        memcpy(&infoServer.protocol, dataResult + i++, sizeof(infoServer.protocol));

        infoServer.name = getString(dataResult + i, i);

        infoServer.map = getString(dataResult + i, i);

        infoServer.folder = getString(dataResult + i, i);

        infoServer.game = getString(dataResult + i, i);

        memcpy(&infoServer.id, dataResult + i, sizeof(infoServer.id));
        i += sizeof(infoServer.id);

        infoServer.players = (int)dataResult[i++];

        infoServer.maxPlayers = (int)dataResult[i++];

        infoServer.bots = (int)dataResult[i++];

        memcpy(&infoServer.serverType, dataResult + i++, sizeof(infoServer.serverType));

        memcpy(&infoServer.environment, dataResult + i++, sizeof(infoServer.environment));

        memcpy(&infoServer.visibility, dataResult + i++, sizeof(infoServer.visibility));

        memcpy(&infoServer.vac, dataResult + i++, sizeof(infoServer.vac));

        infoServer.version = getString(dataResult + i, i);

        memcpy(&infoServer.edf, dataResult + i++, sizeof(infoServer.edf));

        if (infoServer.edf != '\x00')
        {
            if (infoServer.edf == '\x80')
            {
                memcpy(&infoServer.port, dataResult + i, sizeof(infoServer.port));
                i += sizeof(infoServer.port);
            }

            if (infoServer.edf == '\x10')
            {
                memcpy(&infoServer.steamID, dataResult + i, sizeof(infoServer.steamID));
                i += sizeof(infoServer.steamID);
            }

            if (infoServer.edf == '\x40')
            {
                memcpy(&infoServer.portSpect, dataResult + i, sizeof(infoServer.portSpect));
                i += sizeof(infoServer.port);

                infoServer.nameSpect = getString(dataResult + i, i);
            }

            if (infoServer.edf == '\x20')
            {
                infoServer.keyWords = getString(dataResult + i, i);
            }

            if (infoServer.edf == '\x01')
            {
                memcpy(&infoServer.gameID, dataResult + i, sizeof(infoServer.gameID));
                i += sizeof(infoServer.gameID);
            }
        }
        return infoServer;
    }


    playerServer ParserMessage::parsePlayerServer(char* dataResult)
    {
        int i = 0;

        if (dataResult[i] != '\x44')
            throw CustomException((char*)"Class ParserMessage fucntion parsePlayerServer inccorrect header");

        playerServer playerServer;
        memset(&playerServer, 0, sizeof(playerServer));

        memcpy(&playerServer.header, dataResult + i++, sizeof(playerServer.header));

        playerServer.players = (int)dataResult[i++];

        playerChunk* chunkPtr = new playerChunk[playerServer.players];

        for (int j = 0; j < playerServer.players; ++j)
        {
            playerChunk chunk;
            memset(&chunk, 0, sizeof(chunk));
            /* Index of player chunk starting from 0. */
            if (dataResult[i++] != '\x00')
                throw CustomException((char*)"Class ParserMessage fucntion parsePlayerServer start index");

            chunk.name = getString(dataResult + i, i);

            memcpy(&chunk.score, dataResult + i, sizeof(chunk.score));
            i += sizeof(chunk.score);

            memcpy(&chunk.duration, dataResult + i, sizeof(chunk.duration));
            i += sizeof(chunk.duration);

            chunkPtr[j] = chunk;
        }

        playerServer.chunk = chunkPtr;

        return playerServer;
    }


    rulesServer ParserMessage::parseRulesServer(char* dataResult)
    {
        int i = 0;

        if (dataResult[i] != '\x45')
            throw CustomException((char*)"Class ParserMessage fucntion parseRulesServer inccorrect header");

        rulesServer rulesServer;
        memset(&rulesServer, 0, sizeof(rulesServer));

        memcpy(&rulesServer.header, dataResult + i++, sizeof(rulesServer.header));

        memcpy(&rulesServer.rules, dataResult + i, sizeof(rulesServer.rules));
        i += sizeof(rulesServer.rules);

        rulesChunk* chunkPtr = new rulesChunk[rulesServer.rules];

        for (int j = 0; j < rulesServer.rules; ++j)
        {
            rulesChunk chunk;
            memset(&chunk, 0, sizeof(chunk));

            chunk.name = getString(dataResult + i, i);

            chunk.value = getString(dataResult + i, i);

            chunkPtr[j] = chunk;
        }

        rulesServer.chunk = chunkPtr;

        return rulesServer;
    }
}
