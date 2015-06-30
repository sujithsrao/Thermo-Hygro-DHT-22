/*
  *  DHT_Sensor_Test.c
  *  Version  1.2
  *  Created: 26/06/2015 01:19:36 p. m.
  *  Author : Group B
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
 * 
  * Returns: Sensor Reading (Reading not displayed yet)
  * Read the datasheet for more information about the times.
 */

//Libraries
#include <avr/io.h>
#include <util/delay.h>

//Main part of the program
int main(void)
{
	DDRA = 0b11111111; //PORTA as Input
	int8_t temperature = 0; //Variable temperature
	int8_t humidity = 0; //Variable temperature	
	//PORTA |= (1<<PA0);
	//_delay_ms(2000);
	while(1)
	{
		uint8_t bits[5] = {0,0,0,0,0}; //Variable bits[5]
		uint8_t i,j = 0; //Variables i,j
											//memset(bits, 0, sizeof(bits));
		//reset port
		DDRC |= (1<<PC0); //PC0 as Output
		PORTC |= (1<<PC0); //PC0 High
		_delay_ms(100); //Delay 100 ms
		//send request
		PORTC &= ~(1<<PC0); //PC0 Low
		//#if DHT_TYPE == DHT_DHT11
		//_delay_ms(18); //Delay 18 ms
		//#elif DHT_TYPE == DHT_DHT22
		_delay_us(500);
		//#endif
		PORTC |= (1<<PC0); //PC0 High
		DDRC &= ~(1<<PC0); //PC0 as Input
		_delay_us(40); //Delay 40 us
		//check start condition 1
						//if((PINC & (1<<PC0)))
						//{
			///return -1;
			//return;
						//}
		_delay_us(80); //Delay 80 us
		//check start condition 2
						//if(!(PINC & (1<<PC0)))
						//{
			//return -1;
			//return;
						//}
		_delay_us(80); //Delay 80 us
		//read the data
		uint16_t timeoutcounter = 0; //Variable timeoutcounter = 0;
		for (j=0; j<5; j++) //Read 5 byte
		{
			uint8_t result=0; //Variable result = 0
			for(i=0; i<8; i++) //Read every bit
			{
				timeoutcounter = 0; //Variable timeoutcounter = 0;
				while(!(PINC & (1<<PC0))) //wait for an high input (non blocking)
				{ 
					timeoutcounter++; //Increasing variable timeoutcounter
								//if(timeoutcounter > 200) //DHT_TIMEOUT
								//{
						//return -1; //timeout
						//return;
								//}
				}
				_delay_us(30); //Delay 30 us
				if(PINC & (1<<PC0)) //If input is High after 30 us, get result
				result |= (1<<(7-i)); //Saving the results
				timeoutcounter = 0; //Variable timeoutcounter
				while(PINC & (1<<PC0)) //wait until input get low (non blocking)
				{ 
					timeoutcounter++;
								//if(timeoutcounter > 200)
								//{
						//return -1; //timeout
						//return;
								//}
				}
			}
			bits[j] = result; //Saving the results in the array bits[j]
		}
		//Reseting port
		DDRC |= (1<<PC0); //PC0 as Output
		PORTC |= (1<<PC0); //PC0 low
		_delay_ms(100); //Delay 100 ms
		//Checking checksum
		if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) //Checking if the addition of bits[0]+bits[1]+bits[2]+bits[3] is the same that bits[4]
		{
			//return temperature and humidity
			//#if DHT_TYPE == DHT_DHT11 // para el 11
			//temperature = bits[2]; //Variable temperature is the same that bits[2]
			//humidity = bits[0]; //Variable humidity is the same that bits[0]			
			
			//#elif DHT_TYPE == DHT_DHT22
			uint16_t rawhumidity = bits[0]<<8 | bits[1];
			uint16_t rawtemperature = bits[2]<<8 | bits[3];
			if(rawtemperature & 0x8000) {
			temperature = (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
			} else {
			temperature = (float)(rawtemperature)/10.0;
			}
			PORTA = temperature; //PORTA gets the value of the variable temperature			
			humidity = (float)(rawhumidity)/10.0;
			//#endif
			//return;
			//return;
		}
		//return -1;
		//return;				
	}
}
