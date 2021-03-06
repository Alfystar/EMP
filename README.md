# EMP
<img src="00_Guide/EMP-Logo-Background.png" alt="EMP-Logo-Background" style="zoom:33%;" />

### Embedded Message Pack

This is a C++ library to perform the send/receive data between any platform (Arduino, Unix, etc...) with any type of data pack using asynchronous byte streams.

The key feature is the possibility to transmit between 2 different machines, without any necessity to have the same type of device on the other hand.

If you want learn more about the protocol used, [see here](/00_Guide/packProtocoll.md)

#### Supported Device form the library NOW

 This library have the source for different type of the HW:

- [x] Linux
- [x] Arduino AvR (boot for Arduino-Ide and platformIo)
- [x] STM32 (Tested on the discovery board)

#### Tested communication for NOW 

For now the library was tested with success on: 

- [x] Arduino <==> Linux (Raspberry Pi for example, or Any Linux PC)

- [x] Linux <==> Linux (Using Socket abstraction)

- [x] STM32 <==> Linux

- [ ] Arduino <==> Arduino

- [ ] STM32 <==> Arduino

- [ ] STM32 <==> STM32 

- [ ] We are open to new ideas :stuck_out_tongue_winking_eye:

But we are confident for the correctly of the library, because the source file is always the same



# Include in your Project

To include the source in your code please follow [ours guide](/00_Guide/InstallGuide.md) for your IDE.

![EMP-Hierarchy](/01_DOC/img/EMP-Hierarchy.png)

It is easy to see that all concrete classes descend form the MP classes in MP core Package, this design simplify the developing process because the concrete, hard and critical work is coded all in the same manner for all, and after the debugging of the core, all the rest of the problem must be in the son-class and limit his damage only to that zone.

---
Tor-Vergata Automation Department

