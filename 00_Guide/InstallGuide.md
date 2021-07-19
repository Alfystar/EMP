## Include in your project Guide

To include the source in your code there are 2 different way:

1.  Download this repository and copy  `EMP/02_SRC/EMPlibs` directory in the lib-directory of your project. (static download, no update)

2.  Clone this repository in your PC or add as submodule of your repository, and add a *symbolic link* in in the lib-directory of your project of the `EMP/02_SRC/EMPlibs` directory.

##### Clone in File System

   To clone this repository you need to use:

   ```bash
cd <path_to_clone> #Path where the project will clone in File-System
git clone https://github.com/Automatione-Tor-Vergata/EMP.git --recurse-submodules
   ```

##### Add-submodule inside *`your Repository`*

   To add as submodule this repository you need to use:

   ```bash
cd <path_to_clone> #Path where the project will clone INSIDE your Repository
git submodule add https://github.com/Automatione-Tor-Vergata/EMP.git
   ```

To make effective this submodule, you also need to commit and push the local repository.

##### Link the directory

  After the download/clone/submodule

   ```bash
cd <lib-direcotry>
ln -s <EMP/02_SRC/EMPlibs path> -f # Create symbolink link and override precedent if present
   ```

`<EMP/02_SRC/EMPlibs path>` is **strongly** recommended  use **RELATIVE PATH** for better compatibility in different computer with different file system organization 

##### Future Update

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

CmakeLists.txt (inside just outside the `EMPlibs` directory):

```cmake
add_subdirectory(EMPlibs)
target_link_libraries(<Executable> PUBLIC EMPlibs) # in ROS2, PUBLIC could create problem
```

C++ main include:

```c++
#include <MP_Fd.h>
//or
#include <MP_Uart.h>
```

#### Platform-Io Include

After linked, or copied the library in the correct position to correctly include the library you need:

CmakeListsPrivate.txt:

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

**The project have to be C++ !!!** Recreate your project, in case of the old was c, and change *`main.c`* to *`main.cpp`*, more in general, every source file (`*.c` or `*.cpp`) that uses EMP objects or objects in general **MUST BE** `*.cpp`

First configure your **USB device** in the `*.ioc` file. 

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

Tor-Vergata Automation Department [HOME](/README.md)

