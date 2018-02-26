//This library is for simplifying communication between Arduino and PC GUI

//Adam Bowen, 21 November 2017

#ifndef SERIALSTANDARD_H
#define SERIALSTANDARD_H

#include "Arduino.h"
#include "String.h"

typedef struct
{
    char opCode;
    uint16_t opVal;
}   packetContents_t;


class serialStandard
{
public:
    serialStandard(uint8_t UART, uint32_t baud);  //Contructor, requires baud (32-bit) argument
    packetContents_t serialCommandHandler();    //Called by serialEvent() in the Arduino sketch, looks for header, opcode, opval, data, trailer
    //void updateGUI(String);         //Called when it's time to send data to the GUI
    void begin();

    typedef enum
    {
        NO_HEADER           = 0,
        NO_TRAILER          = 1,
        CHECKSUM_FAILED     = 2
    }   errorCode_t;


private:
    //Variables
    uint32_t _baud;               //Baud rate for serial communication
    uint8_t _uart;                //Serial port
    //char _currentOpCode;          //Current operation code (after confirming complete packet, checksumming)
    //uint16_t _currentOpVal;       //Current operation value (after confirming complete packet checksumming)
    const char _headerByte = '!';       //This will remain fixed as "!" (CHANGE DATA TYPE)
    const char _trailerByte = '~';      //This will remain fixed as "~" (CHANGE DATA TYPE)

    //Functions
    //void processCommand();  //Private function that interprets command and provides instruction to main program
    bool _verifyChecksum(uint8_t _opCode, uint16_t _opVal, uint8_t _checksumReceived);
};



#endif // SERIALSTANDARD_H

/*
Packet Structure
Header (1 byte) + OpCode (1 byte) + OpVal (2 bytes) + Trailer (1 byte) + Checksum (1 byte)

Multi-tasking is okay. If we want to get 100 LiDAR samples at 10Hz, there is plenty of time in between samples to perform other tasks. What
we do need is a way to prevent the program from trying to use the same resource (LiDAR, peripheral, etc.) from being used for multiple tasks
at the same time.


When opening a port in Qt, the PC serial port and Arduino serial0 should verify some standard command set. Initially, this will be basic character
assignments, such as opCode = 'L' for "lidar". This structure will leave it open to becoming more sophisticated later on, potentially with header
"modules" with different command assignments
*/
