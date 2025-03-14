#pragma once


#include "Structures.h"
#include "CustomException.h"

#include <cstring>


namespace quer
{
	class ParserMessage
	{
	private:
		playerServer plrServ;
		infoServer infServ;
		rulesServer rlsServ;

		char* getString(char* data, int& i);

	public:
		infoServer parseInfoServer(char* dataResult);
		playerServer parsePlayerServer(char* dataResult);
		rulesServer parseRulesServer(char* dataResult);
		ParserMessage();
		~ParserMessage();
	};
}


