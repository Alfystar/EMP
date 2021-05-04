![](https://img.shields.io/github/stars/Alfystar/Scorbot-CA) ![](https://img.shields.io/github/forks/Alfystar/Scorbot-CA) ![](https://img.shields.io/github/tag/Alfystar/Scorbot-CA) ![](https://img.shields.io/github/release/Alfystar/Scorbot-CA) ![](https://img.shields.io/github/issues/Alfystar/Scorbot-CA) ![](https://img.shields.io/bower/v/editor.md.svg)

------------

# Common_src Arduino uart <--> Linux uart

Sono qui presenti 

    .
    ├── circularBuffer              // Libreria template per il buffer circolare dei pacchetti
    ├── DataTransfertPackge         // typeDef dei pacchetti scambiabili tra i sistemi
    ├── timeOp                      // Raccolta di macro per gestire comodamente il tempo in Linux
    ├── uarDriver                  // Classi sia per linux che per arduino che realizzano la cominicazione tra le schede
    ├── CMakeLists.txt
    └── README.md

## uartDriver
Vale la pena approfondire la cartella "uartDriver":
Essa al suo interno contiene l'implementazioni delle classi che realizzano il "Transport Layer" di questa applicazione:
il diagramma UML di questa classe spiega meglio come si compongono:

<p align="center">
  <i>uartDrive</i>
  <img src="https://github.com/Alfystar/Scorbot-CA/blob/master/1_Doc/ScorBoard%20UML%20Diagrams/diagrams/SerialDrives%20Class%20Diagram.png?raw=true"> 
</p>
Le classi template necessitano di sapere il tipo di pIn e pOut, così da poter allocare opportunamente lo spazio di bufferizzazione, nella send però, continua a pretendere di sapere la size da inviare, questo per poter compattare la trasmissione, e permettere al livello superiore di inviare in realtà pacchetti diversi e di dimensione diversa.

Alla classe deve però arrivare una union in questo caso, che avrà la dimensione massima del pacchetto, permettendo di memorizzare opportunamente e in sicurezza nel buffer circolare il pacchetto. 