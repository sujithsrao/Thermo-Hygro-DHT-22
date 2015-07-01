/*
  *  DHT_Sensor_Test.c
  *  Version  1.1
  *  Created: 26/06/2015 01:19:36 p. m.
  *  Author : Group
   *	DHT11 (Temperature and Humidity sensor)
 *  _________
 * |  -+-+-  |
 * | +-+-+-+ |
 * | +-+-+-+ |
 * |  -+-+-  |
 * | +-+-+-+ |
 * |_________|
 *   |  |  |
 *   1  2  3
 *
 * 1. Ground
 * 2. VCC (3 to 5V power)
 * 3. Data Out



  * Returns: Sensor Reading (Reading not displayed yet)


  * Read the datasheet for more information about the times.
 */

//Libraries
#include <avr/io.h>
#include <util/delay.h>

//Main part of the program
int main(void)
{
	DDRA = 0b11111111;                         //PORTA as Input
	int8_t temperature = 0;                   //Variable temperature
	int8_t humidity = 0;                      //Variable temperature
	while(1)
	{
		uint8_t bits[5] = {0,0,0,0,0};      //Variable bits[5]- Stores incoming data from sensor
		uint8_t i,j = 0;                    //Variables i,j
		                                   //reset port
		DDRC |= (1<<PC0);                  //PC0 as Output
		PORTC |= (1<<PC0);                 //PC0 High

		_delay_ms(100); //Delay 100 ms
		//send request (Based on the communication protocol)

		PORTC &= ~(1<<PC0);                //PC0 Low

		_delay_ms(18); //Delay 18 ms

		PORTC |= (1<<PC0);                 //PC0 High
		DDRC &= ~(1<<PC0);                //PC0 as Input

		_delay_us(40); //Delay 40 us
		_delay_us(80); //Delay 80 us
		_delay_us(80); //Delay 80 us
		//read the data

		uint16_t timeoutcounter = 0;      //Variable timeoutcounter = 0;
		for (j=0; j<5; j++)               //Read 5 byte
		{
			uint8_t result=0;             //Variable result = 0
			for(i=0; i<8; i++)            //Read every bit
			{
				timeoutcounter = 0;       //Variable timeoutcounter = 0;
				while(!(PINC & (1<<PC0))) //wait for an high input (non blocking)
				{
					timeoutcounter++;     //Increasing variable timeoutcounter
				}
				_delay_us(30); //Delay 30 us

				if(PINC & (1<<PC0))       //If input is High after 30 us, get result
				result |= (1<<(7-i));     //Saving the results
				timeoutcounter = 0;       //Variable timeoutcounter
				while(PINC & (1<<PC0))    //wait until input get low (non blocking)
				{
					timeoutcounter++;
				}
			}
			bits[j] = result;             //Saving the results in the array bits[j]
		}
		//Reseting port
		DDRC |= (1<<PC0);                 //PC0 as Output
		PORTC |= (1<<PC0);                //PC0 low
		_delay_ms(100);                   //Delay 100 ms
		//Checking checksum

		//Checking if the addition of bits[0]+bits[1]+bits[2]+bits[3] is the same that bits[4]
		if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4])
		{
			temperature = bits[2];       //Variable temperature is the same that bits[2]
			humidity = bits[0];          //Variable humidity is the same that bits[0]
			PORTA = humidity;            //PORTA gets the value of the variable humidity
		}
	}
}
