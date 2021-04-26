//
// Created by alfyhack on 15/10/19.
//

#ifndef SPIRASP_TERMINAL_TYPEPACKWRONGEXCEPT_H
#define SPIRASP_TERMINAL_TYPEPACKWRONGEXCEPT_H
#ifndef ScorboarFirmware

#include <exception>
#include <string>

//Exeption tree
class typePackWrongExcept : public std::exception{
    std::string _msg;
public:
    typePackWrongExcept (const std::string &msg) : _msg(msg){}

    virtual const char *what () const noexcept override{
        std::string tipo = "typePackWrongExcept";
        return tipo.append(_msg).c_str();
    }
};

#endif
#endif //SPIRASP_TERMINAL_TYPEPACKWRONGEXCEPT_H
