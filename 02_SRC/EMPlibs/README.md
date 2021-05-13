# EMPLibs Structure

The library are written to be compiled directly from the linux companion and the arduino compiler.

The linux side have his CmakeFile.txt, and inside him are defined `CMAKE_COMPILING`, if you using something else, be carrefour to define this define, are used to avoi error form arduino builder.

## Message Pack system
This UML diagram explain HOW the class are related:

<p align="center">
  <i>uartDrive</i>
  <img src="https://github.com/Automatione-Tor-Vergata/EMP/blob/main/01_DOC/img/EMP-Hierarchy.png?raw=true"> 
</p>
Two concrete class are ALWAYS able to comunicate (with his O/I system).
This task are perform equals thanks MPcore packege, all the logic work are do there, and the concrete class have the responsability only to implement the I/O fot that metod.
Obiviusly The pIn and pOut need to be equal and swappet form the 2 side.

Attention!!
Linux side need to be compiled using C++20, to enable the compiling time optimization, unfortunaly, for now, arduino framework work with C++14, and for this reason in MPcore, are used some define to mantain the compatibility