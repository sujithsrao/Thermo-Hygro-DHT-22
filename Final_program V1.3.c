/*
 *  Final_Prog V1.3
 *
 *  Created: 29/06/2015 02:22:05 p. m.
 *  Author: Team B
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
 *
 * Final Program with all the functionalities
 * Display current Temperature and Humidity all the time 
 * Display Peak Temperatures and Humidities on pressing BUtton 1 and 2 respectively
 * Warning Buzzer on temperature sound 
 */ 


//Libraries
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#define warning_temp 35

//Variables
uint8_t bits[5] = {0,0,0,0,0};             //Variable bits[5]
uint8_t i,j = 0;                           //Variables i,j=0
int8_t t = 0;                              //Variable t
int8_t h = 0;                              //Variable t
char temp[10];                             //Variable temp[10]
char hum[10];                              //Variable hum[10]
int8_t max_hum=0;                          //Variable max_hum initialize to 0
char max_humidity[10];                     //Variable max_humidity
int8_t min_hum=100;                        //Variable min_hum initialize to 100
char min_humidity[10];                     //Variable min_humidity
int8_t max_tem=0;                          //Variable max_tem initialize to 0
char max_temperature[10];                  //Variable max_temperature
int8_t min_tem=100;                        //Variable min_tem initialize to 100
char min_temperature[10];                  //Variable min_temperature

//Function to get Temperature and Humidity
void get_temp_and_hum()
{
	//reset port
	DDRC |= (1<<PC0);                      //PC0 as Output
	PORTC |= (1<<PC0);                     //PC0 High
	_delay_ms(100);                        //Delay 100 ms
	//send request
	PORTC &= ~(1<<PC0);                    //PC0 Low
	_delay_ms(18);                         //Delay 18 ms
	PORTC |= (1<<PC0);                     //PC0 High
	DDRC &= ~(1<<PC0);                     //PC0 as Input
	_delay_us(40);                         //Delay 40 us
	_delay_us(80);                         //Delay 80 us
	_delay_us(80);                         //Delay 80 us
	//read the data
	uint16_t timeoutcounter = 0;           //Variable timeoutcounter = 0;
	for (j=0; j<5; j++)                    //Read 5 byte
	{
		uint8_t result=0;                  //Variable result = 0
		for(i=0; i<8; i++)                 //Read every bit
		{
			timeoutcounter = 0;            //Variable timeoutcounter = 0;
			while(!(PINC & (1<<PC0)))      //wait for an high input (non blocking)
			{
				timeoutcounter++;          //Increasing variable timeoutcounter
			}
			_delay_us(30);                 //Delay 30 us
			if(PINC & (1<<PC0))            //If input is High after 30 us, get result
			result |= (1<<(7-i));          //Saving the results
			timeoutcounter = 0;            //Variable timeoutcounter
			while(PINC & (1<<PC0))         //wait until input get low (non blocking)
			{
				timeoutcounter++;          //Increasing the timeoutcounter
			}
		}
		bits[j] = result;                  //Saving the results in the array bits[j]
	}
	//Reseting port
	DDRC |= (1<<PC0);                      //PC0 as Output
	PORTC |= (1<<PC0);                     //PC0 low
	_delay_ms(100);                        //Delay 100 ms
	//Checking checksum
	//Checking if the addition of bits[0]+bits[1]+bits[2]+bits[3] is the same that bits[4]
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) 
	
	{
		t = bits[2]; //Variable t is the same that bits[2]
		h = bits[0]; //Variable h is the same that bits[0]
	}
}

void assignation (void)
{
	if(h>max_hum) //Checking the value of h
	{
		max_hum=h; //New value of max_hum
	}
	if(h<min_hum) //Checking the value of h
	{
		min_hum=h; //New value of min_hum
	}
	if(t>max_tem) //Checking the value of t
	{
		max_tem=t; //New value of max_tem
	}
	if(t<min_tem) //Checking the value of t
	{
		min_tem=t; //New value of min_tem
	}
}


//Function to show temperature High and Low
void humidity (void)
{
	lcd_clrscr();                  //Clear screen
	lcd_gotoxy(0,0);               //Position (X,Y) in LCD
	lcd_puts("HUM(HIGH):");        //Write 'HUM(HIGH):' in LCD
	itoa(max_hum,max_humidity,10); //Puts the value of max_hum in max_humidity to see the value in the LCD
	lcd_gotoxy(12,0);              //Position (X,Y) in LCD
	lcd_puts(max_humidity);        //Writes variable max_humidity in the LCD
	lcd_gotoxy(0,1);               //Position (X,Y) in LCD
	lcd_puts("HUM(LOW):");         //Write 'HUM(LOW):' in LCD
	itoa(min_hum,min_humidity,10); //Puts the value of min_hum in min_humidity to see the value in the LCD
	lcd_gotoxy(12,1);              //Position (X,Y) in LCD
	lcd_puts(min_humidity);        //Writes variable min_humidity in the LCD
	_delay_ms(3000);               //Delay 3 seconds
	lcd_clrscr();                  //Clear screen
}

//Function to show humidity High and Low
void temperature (void)
{
	lcd_clrscr();                     //Clear screen
	lcd_gotoxy(0,0);                  //Position (X,Y) in LCD
	lcd_puts("TEMP(HIGH):");          //Write 'TEMP(HIGH):' in LCD
	itoa(max_tem,max_temperature,10); //Puts the value of max_tem in max_temperature to see the value in the LCD
	lcd_gotoxy(12,0);                 //Position (X,Y) in LCD
	lcd_puts(max_temperature);        //Writes variable max_temperature in the LCD
	lcd_gotoxy(0,1);                  //Position (X,Y) in LCD
	lcd_puts("TEMP(LOW):");           //Write 'TEMP(LOW):' in LCD
	itoa(min_tem,min_temperature,10); //Puts the value of min_tem in min_temperature to see the value in the LCD
	lcd_gotoxy(12,1);                 //Position (X,Y) in LCD
	lcd_puts(min_temperature);        //Writes variable min_temperature in the LCD
	_delay_ms(3000);                  //Delay 3 seconds
	lcd_clrscr();                     //Clear screen
}

//Function to reset the values
void reset (void)
{
	lcd_clrscr();                     //Clear screen
	lcd_gotoxy(0,0);                  //Position (X,Y) in LCD
	lcd_puts("RESETING  VALUES");     //Write 'RESETING  VALUES' in LCD
	bits[5] = 0;                      //Variable bits[5]
	i = 0;                            //Variables i,j=0
	j= 0;
	t = 0;                            //Variable t
	h = 0;                            //Variable t
	temp[10] = 0;                     //Variable temp[10]
	hum[10] = 0;                      //Variable hum[10]
	max_hum = 0;                      //Variable max_hum initialize to 0
	max_humidity[10] = 0;             //Variable max_humidity
	min_hum=100;                      //Variable min_hum initialize to 100
	min_humidity[10] = 0;             //Variable min_humidity
	max_tem=0;                        //Variable max_tem initialize to 0
	max_temperature[10] = 0;          //Variable max_temperature
	min_tem=100;                      //Variable min_tem initialize to 100
	min_temperature[10] = 0;          //Variable min_temperature
	_delay_ms(3000);                  //Delay 3 seconds
	lcd_clrscr();                     //Clear screen
}

//Main part of the program
int main(void)
{
	DDRC = 0b00100000;               //PORTC Buzzer output
	PORTC= 0b00000000;               //Active low
	DDRA = 0b00000000;               //PORTA as Input
	PORTA = 0b11111111;              //Activate Pull up resistor
	lcd_init(LCD_DISP_ON);           //Initiate LCD
	lcd_clrscr();                    //Clear screen of the LCD
	while(1)
	{
		lcd_gotoxy(0,0);             //Position (X,Y) in LCD
		lcd_puts("CUR TEMP:");       //Write 'CUR TEMP:' in LCD
		lcd_gotoxy(0,1);             //Position (X,Y) in LCD
		lcd_puts("CUR HUM:");        //Write 'CUR HUM:' in LCD
		get_temp_and_hum();	         //Calling function get_temp_and_hum()
		assignation();
		itoa(t,temp,10);             //Puts the value of t in temp to see the value in the LCD
		lcd_gotoxy(10,0);            //Position (X,Y) in LCD
		lcd_puts(temp);	             //Writes variable temp in the LCD
		itoa(h,hum,10);              //Puts the value of h in hum to see the value in the LCD
		lcd_gotoxy(10,1);            //Position (X,Y) in LCD
		lcd_puts(hum);               //Writes variable hum in the LCD
		if((!(PINA & 1)))            //Check if Button1 is pressed
		{
			while(!(PINA & 1));      //Avoid anti-repeat
			temperature();           //Calling function temp()
		}
		if((!(PINA & 2)))            //Check if Button2 is pressed
		{
			while(!(PINA & 2));      //Avoid anti-repeat
			humidity();              //Calling function hum()
		}
		if((!(PINA & 4)))            //Check if Button3 is pressed
		{
			while(!(PINA & 4));      //Avoid anti-repeat
			reset();                 //Calling function reset()
		}
		if(t>warning_temp)           //Check if temperature reading exceeds set allowable temperature
		{
			PORTC = 0b00100000;      //Run Buzzer
		}
		else
		{
			PORTC = 0b00000000;      //Stop when temperature is under warning temperature
		}
		
	}
}

