//
// Created by alfyhack on 10/11/19.
//

#ifndef PCLISTENUART_UARTEXCEPTION_H
#define PCLISTENUART_UARTEXCEPTION_H
#ifdef CMAKE_COMPILING
#include <cstring>
#include <exception>
#include <string>

/*
 * The exeption are used to signal a terminal error, not catch him, the system will crash in any case, but so are
 * possible watch on the terminal the error, if you need to send back an error, the error code in the return value
 * might be better solution for this library
 */

using namespace std;
namespace EMP {
class MP_FDexept : public exception {
  const string _msg;
  const int _errCode = 0;
  string ret;

public:
  MP_FDexept(const string &msg, int errCode) : _msg(msg), _errCode(errCode) {
    ret = "|" + _msg + "|";
    ret += "\terror code: ";
    ret += to_string(_errCode);
  };

  MP_FDexept(const string &msg) : MP_FDexept(msg, 0){};

  virtual const char *what() const noexcept override { return ret.c_str(); }
  virtual const int errNum() const noexcept { return _errCode; }
}; // class MP_exept

class MP_UartExept : public MP_FDexept {
public:
  MP_UartExept(const string &msg, int errCode) : MP_FDexept(msg, errCode){};
  MP_UartExept(const string &msg) : MP_UartExept(msg, 0){};

}; // class MP_UartExept

} // namespace EMP

#endif //#ifdef CMAKE_COMPILING
#endif // PCLISTENUART_UARTEXCEPTION_H
