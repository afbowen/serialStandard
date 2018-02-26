//Adam Bowen
//Serial Standard for interface to PC
//Created: 21 November 2017

#include "Arduino.h"
#include "serialStandard.h"

serialStandard::serialStandard(uint8_t UART, uint32_t baud)
{
    _baud = baud;
    _uart = UART;
}

void serialStandard::begin()
{
    switch (_uart)
    {
        case 0:
            Serial.begin(_baud);
            break;
        case 1:
            Serial1.begin(_baud);
            break;
        case 2:
            Serial2.begin(_baud);
            break;
        case 3:
            Serial3.begin(_baud);
            break;
    }

    //Serial.begin(_baud);    //Will always use Serial 0 (USB port on Arduino Uno and Mega)
}


//Call when there is serial data available
packetContents_t serialStandard::serialCommandHandler()
{
    uint8_t _opCodeRead = 0;
    uint16_t _opValRead = 0;
    uint8_t _checksumRead = 0;

    //serialEvent has occured, first byte should be header
    while(Serial.available())
    {
        if(Serial.find('!'))
        {
            delayMicroseconds(100);         //Wait for buffer to populate
            _opCodeRead = Serial.read();    //Read the operation code character (one ASCII byte)
            _opValRead = Serial.parseInt(); //Read ASCII characters following the opCode and parse out the integer
            /*
            _opValRead = Serial.read();     //Big-endian (MSB to the left)
            _opValRead <<= 8;               //Shift MSB to the left
            _opValRead |= Serial.read();    //Populate LSB
            */



            if(Serial.read() == '~')    //If trailer is found, look for checksum (1 byte)
            {

                _checksumRead = Serial.parseInt();
                //_checksumRead = Serial.read();  //Read in checksum
                if(_verifyChecksum(_opCodeRead, _opValRead, _checksumRead)) //If received checksum matches calculated checksum, return the package contents to MAIN
                {
                    packetContents_t output = {_opCodeRead, _opValRead};
                    return output;
                }
                else
                {
                    packetContents_t output = {'E', CHECKSUM_FAILED};
                    return output;
                }
            }
            else
            {
                packetContents_t output = {'E', NO_TRAILER};
                return output;
            }
        }
        else
        {
            packetContents_t output = {'E', NO_HEADER};
            return output;
        }
    }
}


/*//For now, this will simply return a struct for MAIN to do with what it pleases
void serialStandard::_processCommand(uint8_t _currentOpCode, uint8_t _currentOpVal)
{

}*/

bool serialStandard::_verifyChecksum(uint8_t _opCode, uint16_t _opVal, uint8_t _checksumReceived)
{
    uint8_t _opValLSB = _opVal;
    uint8_t _opValMSB = _opVal >> 8;
    uint8_t _checksumCalculated = _headerByte ^ _opCode ^ _opValMSB ^ _opValLSB ^ _trailerByte ^ 0xFF;
    if(_checksumReceived == _checksumCalculated)
    {
        return true;
    }
    else
    {
        return false;
    }
}
