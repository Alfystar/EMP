# EMP
### Embedded Message Pack PURPOSE

This is a C++ library to perform the  send/receive data between any platform (arduino, unix, ecc...) with any type of data pack using the UART/USART, the user need to chose very small configuration and pass very small data.

The key feature are the transmission between 2 different machine using UART/USART, indipendently form the machine on the other usedSpace, for example:

- Arduino <==> Arduino
- Arduino <==> Linux (Raspberry Pi for example, or Any linux PC)
- Linux <==> Linux (Using Socket abstarction)
- STM32 <==> Linux
- end so on...

 To perform this request, this library have the source for different type of the HW:

- [x] Linux
- [x] Arduino AvR (boot for Arduino-Ide and platformIo)
- [x] STM32 (Tested on the discovery board)

#### Transmission idea
The technique used to send the packet without knowing the usedSpace, use:

- [CRC8](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) to and minimal security check of the pack transmission

- [COBS codification](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) to transform any pack-data in **self ended pack data**

The system receive a buffer with a usedSpace, internally now are calculated the CRC8 and converted all with the  COBS format. Now, **based of the used platform**, the library serialize the pack in to the UART/USART stream and on the other usedSpace the library read every byte and when found **2 zero** *(see COBS)* provide a de-serialization and CRC8 check.

If the pack transmission is success, then the pack are stored until the reading.

Other the pack will be reject with the security of the impossibility to save the corrupted data.   

 The purpose is create the smallest foot-print and the most faster running code possible.

---

### Msg standardize

The system not need any specific format for the message, but our teems suggest:

-  [Nanopb - Protocol Buffers for Embedded Systems](https://github.com/nanopb/nanopb)
- ROS/ROS2 Msg pack compiled
- Custom solution (used on the Test directory)

In any case is responsibility of the user write the right code to understand what data was revived and how use him, this suggestion are user-full to standardize the code and to simplify the work load.

# Include in your Project

To include the source in your code there are 2 different way:

1.  Download this repository and copy  `EMP/02_SRC/EMPlibs` directory in the lib-directory of your project. (static download, no update)

2.  Clone this repository as submodule of your repository, any where you want, and add a *symbolic link* in in the lib-directory of your project of the `EMP/02_SRC/EMPlibs` directory.

##### Clone and link

   To clone this repository you need to use:

   ```bash
   cd <path_to_clone> #Path where the project will clone
   git clone https://github.com/Automatione-Tor-Vergata/EMP.git --recurse-submodules
   cd <lib-direcotry>
   ln -s <EMP/02_SRC/EMPlibs path> -f # Create symbolink link and override precedent if present
   ```
To update the code in the future, now you need to type:
   ```bash
   git pull --recurse-submodules
   ```
### #Include <> source
All the file had specific `#define` to isolate the source code and avoid error during the compiling.

After this phase, you need only to include the final class you need (MP_FD, MP_Uart, MP_Serial, MP_ST_usb)

The source can be included in your project and use as you wish, the only thinks you need are copy `EMP/02_SRC/EMPlibs` directory in your project lib directory.

For more example see the `03_Test` directory, all the example are like the (2) point, obviously without the git submodule.

#### Linux Include

After linked, or copied the library in the correct position to correctly include the library you need:

CmakeLists.txt:

```cmake
add_subdirectory(EMPlibs)
target_link_libraries(<Executable> PUBLIC EMPlibs)
```

C++ main include:

```c++
#include <MP_Fd.h>
//or
#include <MP_Uart.h>
```

#### PlatformIo Include

After linked, or copied the library in the correct position to correctly include the library you need:

CmakeListsProvate.txt:

```cmake
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib/EMPlibs/src")
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib/EMPlibs/src/MPcore")
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib/EMPlibs/src/MPcore/circularBuffer")
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib/EMPlibs/src/MPcore/cobs-c")
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib/EMPlibs/src/MPcore/crc8")
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib/EMPlibs/src/timeOp")
```

C++ main include:

```c++
// Have to be the FIRST
#include <Arduino.h>
// Message Pack 
#include <MP_Serial.h>
```



#### STM32_Cube Include

**The project have to be C++ !!!** Recreate your project, in case of the old was c, and change *main.c* to *main.cpp*, more in general, every source file (*.c or *.cpp) that uses EMP objs or objs in general **MUST BE** *.cpp

First configure your **USB device** in the *.ioc file. 

After linked, or copied the library in the correct position to correctly include the library you need to do:

1. click right on the project in the `Project Explorer`
2. Go to: `Propeties --> Paths and Sybmols`
3. In `Includes tab`, select `GNU C++` as languages and `Add` the `EMPlibs/src` directory
4. In `Includes tab`, select `GNU C` as languages and `Add` the `EMPlibs/src` directory
5. In the `Source Location tab`, add ` EMPlibs/src` directory

C++ main include:

```c++
#include "usb_device.h" // with the correct parameter in the *.ioc file
// Message Pack 
#include <MP_ST_usb.h>

```





---
Tor-Vergata Automation Department

