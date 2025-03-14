#include <iostream>
#include "Server_queries.h"


int main()
{
    int port;
    std::string ipserver;

    try
    {
        quer::getAll(27015, "74.91.115.12");
    }
    catch (quer::CustomException e)
    {
        printf("%s \n", e.what().c_str());
    }
    catch (...)
    {
        printf("Error in code");
    }

    return 0;
}
