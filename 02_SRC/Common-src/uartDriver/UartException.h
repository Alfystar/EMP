//
// Created by alfyhack on 10/11/19.
//

#ifndef PCLISTENUART_UARTEXCEPTION_H
#define PCLISTENUART_UARTEXCEPTION_H

#include <exception>
#include <string>
#include <cstring>

#define excType "(UartException) "
//Exeption tree
namespace Uart {
    class UartException : public std::exception{
        const std::string _msg;
    public:
        UartException (const std::string &msg) : _msg(std::string(excType).append(msg)){}

        UartException (const std::string &msg, int errCode) : _msg(std::string(excType).append(msg).append(
                std::string("\n\terrno: ").append(std::string(strerror(errCode))))){
        }

        virtual const char *what () const noexcept override{
            return _msg.c_str();
        }
    };
}
#undef excType
#undef excType
#endif //PCLISTENUART_UARTEXCEPTION_H
