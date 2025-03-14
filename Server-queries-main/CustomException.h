#pragma once

#include <exception>
#include <string>

namespace quer
{
    class CustomException : public std::exception {
    private:
        std::string message;

    public:
        CustomException(char* msg) : message(msg) {}

        CustomException(char* msg, int k) 
        {
            message = msg;
            message += std::to_string(k);;
        }

        std::string what() {
            return message;
        }
    };
}