# EMP
### Embedded Message Pack PURPOSE

This is a C++ library to perform the  send/receive data between any platform (arduino, unix, ecc...) with any type of data pack using the UART/USART, the user need to chose very small configuration and pass very small data.

The key feature are the transmission between 2 different machine using UART/USART, indipendently form the machine on the other usedSpace, for example:

- Arduino <==> Arduino
- Arduino <==> Linux (Raspberry Pi for example)
- Linux <==> Linux
- end so on...

 To perform this request, this library have the source for different type of the HW:

- [ ] Arduino
- [ ] Linux

### Clone and build

To clone this repository you need to use:

```bash
git clone https://github.com/Automatione-Tor-Vergata/EMP.git --recurse-submodules
```

or, if you just download it, use inside the directory:

```bash
git submodule update --init --recursive
```

to clone also the submodule

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
- Custom solution

In any case is responsibility of the user write the right code to understand what data was revived and how use him, this suggestion are user-full to standardize the code and to simplify the work load.

### Include in your Project

The source can be included in your project and use as you wish, selecting the right code based on your architecture.

---
Tor-Vergata Automation Department