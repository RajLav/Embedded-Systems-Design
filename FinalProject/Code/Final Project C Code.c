/* Name :- Raj Lavingia
Final Project Code
Date :- 05/05/2018
*/

//Standard Libraries inclusion
#include <stdio.h>
#include <stdint.h>
#include <mcs51reg.h>
#include <at89c51ed2.h>
#include <string.h>
#include <math.h>

//Defining pins of 8051
#define SCL P2_0
#define SDA P2_1
#define next P2_2                   //increment digit
#define inc  P2_3                  //increment value
#define set  P2_4                   //set time

//Values taken from memory mapping , SPLD logic
volatile xdata unsigned char *lcd_instruction_write = 0x8000;
volatile xdata unsigned char *lcd_instruction_read = 0xB000;
volatile xdata unsigned char *lcd_data_write = 0xA000;
volatile xdata unsigned char *lcd_data_read = 0x9000;


unsigned char f2,g2,h2,l,l2,m1,n1,o1,p,q,r,t,u,v,x,y,z,j2,k2;
char command;
uint8_t r4,r5,s1,s2,s3,s4;
int count1,count2,count3,count10;
int r9,r10,r11;
unsigned char result[1];
unsigned char result1[1];
unsigned char command1;
unsigned char card_id[12];
unsigned char card_id1[12];
char ack;
unsigned char day1=1;
unsigned char k,x;
unsigned int date=1, mon=1, hour=0, min=0, sec=0;
int year=0;

//Delay Function
void delay(int itime)
{
    int i,j;
    for(i=0;i<itime;i++)
    for(j=0;j<1275;j++);
}

//get a character from user
char getchar ()
{
    while (RI == 0); // wait for character to be received, spin on RI
	RI = 0;			// clear RI flag
	return SBUF;	// return character from SBUF
}

// print a character
void putchar (char c)
{
    while (TI == 0); // wait for TX ready, spin on TI
    SBUF = c;  	// load serial port with transmit value
   	TI = 0;  	// clear TI flag
}
                         //shift back to DDRAM location 0
//print a string
void putstr (char *p)
{
    int i = 0;
	while (*(p+i)!='\0') //output characters until NULL found
    {
		putchar(*(p+i));
		i++;
	}
}

//Delay Function
void delay1()
{
       unsigned char i,j;
        for(i=0;i<255;i++)
        {
            for(j=0;j<255;j++)
            {
            }
        }
}

//Busy wait function for LCD
void lcdbusywait()
{
    while(*lcd_instruction_read & 0x80);
}
void lcdwrite(int dat)
{
    lcdbusywait();
   *lcd_data_write = dat;

}

//Function for printing string on lCD
void stringprint(char *str)
{
	int q;
	for(q=0;str[q]!=0;q++)  // print every character of screen
	{
        lcdwrite(str[q]);
	}
}

//Delay Function
void delay_ms(unsigned int ms_count)
 {
     unsigned int i = 0;

        while(ms_count != 0)
         {
            for(i = 0; i < 112; i++);
             ms_count--;
            }}

unsigned char m,n;
char *size,*size1;
char data_get[10]; //get data from user


char *getstr()
{
	char *s=0;
	char c;
	int i=0;
	c = getchar();
	while( c !='\r' && c !='\0') //get characters till null found
	{
		data_get[i]=c;
		i++;
		c = getchar();
	}
	 data_get[i]='\0';
	s=data_get;
	*(s+i)='\0';
	return s;
}

//Initialisation of serial port 9600bps
void init_serial( void )
{
  SCON = 0x50;
  PCON &= 0x7F;
TMOD &= 0xCF;
TMOD |= 0x20;TH1 = 0xFD;
TR1 = 1;
TI = 1;
}

//Initialization of LCD
void lcdinit()
{

     delay_ms(10);
     *lcd_instruction_write = 0x30; //unlock command


     delay_ms(10);
     *lcd_instruction_write = 0x30; //unlock command

     delay_ms(10);
    *lcd_instruction_write = 0x30; //unlock command


     lcdbusywait();
    *lcd_instruction_write = 0x38; //Function set command


     lcdbusywait();
     *lcd_instruction_write = 0x08; // Turn the display off


     lcdbusywait();
     *lcd_instruction_write = 0x0C; // Turn the display On

     lcdbusywait();
     *lcd_instruction_write = 0x06; // Entry Mode Set

     lcdbusywait();
    *lcd_instruction_write = 0x01; // Clear the screen and send cursor home

    lcdbusywait();
    *lcd_instruction_write = 0x0E; // Cursor On

    lcdbusywait();
    *lcd_instruction_write = 0x80; // DDram address 0,0 position

}


//Function to receive data serially from RS232

void recieve()
{
    unsigned char k;
     for(k=0;k<12;k++)
     {
         while(RI==0)
         {

         }
          card_id[k]=SBUF;
          RI=0;
    }
}

//Function to receive data serially from RS232
void recieve1()
{
    unsigned char k;
     for(k=0;k<12;k++)
     {
         while(RI==0)
         {

         }
          card_id1[k]=SBUF;
          RI=0;
    }
}

//function to print string on LCD
void lcdstring1(char *str)
{
    while(*str)
    {
      lcdwrite(*str);
        str++;
    }
}

//I2C communication

void I2CStart(){SDA=1;SCL=1,SDA=0,SCL=0;}             //"start" function for communicate with ds1307 RTC
void I2CStop(){SDA=0,SCL=1,SDA=1;}                     //"stop" function for communicate wit ds1307 RTC

unsigned char I2CSend(unsigned char Data)             //send data to ds1307
{
char i;
char ack_bit;
for(i=0;i<8;i++)
{
if(Data & 0x80) SDA=1;
else SDA=0;
SCL=1;
Data<<=1;
SCL=0;
}
SDA=1,SCL=1;
ack_bit=SDA;
SCL=0;
return ack_bit;
}

unsigned char I2CRead(char ack)                      //receive data from ds1307
{
unsigned char i, Data=0;
SDA=1;
for(i=0;i<8;i++)
{
 Data<<=1;
 do{SCL=1;}
 while(SCL==0);
 if(SDA) Data|=1;
 SCL=0;
}
if(ack)SDA=0;
else SDA=1;
SCL=1;
SCL=0;
SDA=1;
return Data;
}

int BCDToDecimal(char bcdByte)
{
       char a,b,dec;
    a=(((bcdByte & 0xF0) >> 4) * 10);
    b=(bcdByte & 0x0F);
    dec=a+b;
    return dec;
}

char DecimalToBCD (int decimalByte)
{
  char a,b,bcd;
  a=((decimalByte / 10) << 4);
  b= (decimalByte % 10);
  bcd=a|b;
  return bcd;
}
/*
void show_time1()                            //function to display time/date/day on LCD
{
  char var10[5];
  *lcd_instruction_write =  0x80;
  stringprint("Date:");
  sprintf(var10,"%d",date);
  stringprint(var10);
  sprintf(var10,"/%d",mon);
  stringprint(var10);
  sprintf(var10,"/%d",year+2000);
  stringprint(var10);
  stringprint("   ");
  *lcd_instruction_write =  (0xc0);
  stringprint("Time:");
  sprintf(var10,"%d",hour);
  stringprint(var10);
  sprintf(var10,":%d",min);
  stringprint(var10);
  sprintf(var10,":%d",sec);
  stringprint(var10);
  stringprint(" ");
//  day(day1);
   stringprint("   ");
 }*/

 //Function to display time on LCD
 void show_time()                            //function to display time/date/day on LCD
{
  char var[5];char var1[5];char var2[5];

  *lcd_instruction_write = 0xD8;
    sprintf(var,"%d",hour);
    stringprint(var);
    sprintf(var1,":%d",min);
    stringprint(var1);
    sprintf(var2,":%d",sec);
    stringprint(var2);



 }

void set_time()                                            //time set function
{
 *lcd_instruction_write = 0x0C;
 while(k<7)
 {
  while(k==3)                                            //set date
  {
   x=year%4;
   if(inc==0)
   {
       date++;while(inc==0);
   if(x==1 && mon==2 && date==28){date=1;}                //check for 28 day febuary
   if(x==0 && mon==2 && date==29){date=1;}                //check for 29 day febuary
   if((date==31) && (mon==4) || (mon==6) || (mon==9) || (mon==17)){date=1;}        // check for 30 day month
   if(date==32){date=1;}                                                        // check for 31 day month
   show_time();
   }
   if(next==0)
   {
     k=5;

     while(next==0);
     }                                                //check for next digit
 *lcd_instruction_write =(0x85);
  }

  while(k==2)                                            //set month
  {
   if(inc==0)
   {mon++;while(inc==0);
   if(mon==13){mon=1;}                                  //check for end of year
   show_time(); }
   if(next==0){k=3;
   while(next==0);

   }
   *lcd_instruction_write =(0x88);
  }

  while(k==1)                                         //set year
  {
   if(inc==0)
   {year++;while(inc==0);
   if(year==30){year=0;}
   show_time();     }
   if(next==0){k=2;

   while(next==0);}
 *lcd_instruction_write = (0x8d);
  }

  while(k==5)                                      //set hour
  {
   if(inc==0)
   {hour++;while(inc==0);
   if(hour==24){hour=0;}
   show_time();}
   if(next==0){k=6;
   while(next==0);}
   *lcd_instruction_write = (0xc5);
  }

  while(k==6)                                       //set min
  {
   if(inc==0)
   {min++;while(inc==0);
   if(min==60){min=0;}
   show_time();}
   if(next==0){k=10;
   while(next==0);}
   *lcd_instruction_write = (0xc8);
  }
 }
}

//Attendance function for Raj
void attendance()
{
            *lcd_instruction_write = 0x90;
            sprintf(result, "%d", count1);
            lcdstring1(result);
            delay(500);
            *lcd_instruction_write = 0x01;
}

//Attendance Function for Meet
void attendance2()
{
            *lcd_instruction_write = 0x90;
            sprintf(result, "%d", count2);
            lcdstring1(result);
            delay(500);
            *lcd_instruction_write = 0x01;
}

//Attendance function for custom name
void attendance3()
{
            *lcd_instruction_write = 0x90;
            sprintf(result, "%d", count10);
            lcdstring1(result);
            delay(500);
            *lcd_instruction_write = 0x01;
}

//Function for registration process
void registration()
{
            printf_tiny("Enter the pass code for registration \n");
            printf_tiny("\r");
            size1 = getstr();
            if(strncmp(size1,"8438",4)==0)
                {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Password is correct \n");
                printf_tiny("\r");
                *lcd_instruction_write = 0x01;
                lcdbusywait();
                *lcd_instruction_write = 0x80;
                stringprint("Place Your Card");
                recieve1();
                *lcd_instruction_write = 0xC1;      //Place cursor to second position of second line
             for(m=0;m<12;m++)
             {
                  lcdbusywait();
                  lcdwrite(card_id1[m]);
             }
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("This is your unique id : \n");
                printf_tiny("\r");
                putstr(card_id1);
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Write your name \n");
                printf_tiny("\r");
                size= getstr();
                printf_tiny("user entered name is \n");
                printf_tiny("\r");
                putstr(size);
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("You are now registered. \n");
                printf_tiny("\r");
               *lcd_instruction_write = 0x01;
                lcdbusywait();
                }

else
                {
                    printf_tiny("\n");
                    printf_tiny("\r");
                    printf_tiny("Entered Password is wrong. Nice try though !! \n");
                    printf_tiny("\r");
                }

}

//Function for deletion of attendance
void deleteattendance()
{
       *lcd_instruction_write = 0x01;
        lcdbusywait();
        *lcd_instruction_write = 0x80;
        lcdbusywait();
        stringprint("Place Your Card");
        recieve();
        *lcd_instruction_write = 0xC1;      //Place cursor to second position of second line
         for(l=0;l<12;l++)
         {
              lcdbusywait();
              lcdwrite(card_id[l]);
        }
if(strncmp(card_id,"04001C9B0487",12)==0)
        {
            printf_tiny("Enter the pass code for deletion \n");
            printf_tiny("\r");
            size1 = getstr();
            if(strncmp(size1,"8538",4)==0)
            {
            printf_tiny("\n");
            printf_tiny("\r");
            printf_tiny("Welcome Raj, You can delete the attendance now. \n ");
            printf_tiny("\r");
            printf_tiny("Whose attendance you want to delete? \n");
            printf_tiny("\r");
            size1 =getstr();
            if(strncmp(size1,"RAJ",3)==0)
            {
                if(count1<1)
                {
                    printf_tiny("There is already 0 attendance \n");
                    printf_tiny("\r");

                }
                else
                {
                count1--;
                r9--;
                attendance();
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Attendance is deleted one time. \n");
                printf_tiny("\r");
                }

            }
            else if(strncmp(size1,"MEET",4)==0)
            {
                if(count2<1)
                {
                    printf_tiny("There is already 0 attendance \n");
                    printf_tiny("\r");
                }
                else
                {
                count2--;
                r10--;
                attendance2();
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Attendance is deleted one time. \n");
                printf_tiny("\r");
                }
            }
            else if(strncmp(size1,size,100)==0)
            {
                if(count10<1)
                {
                    printf_tiny("There is already 0 attendance \n");
                    printf_tiny("\r");
                }
                else
                {
                count10--;
                r11--;
                attendance3();
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Attendance is deleted one time. \n");
                printf_tiny("\r");
                }
            }
            else
            {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("The name you enetered is not correct");
                printf_tiny("\n");
                printf_tiny("\r");
            }

            }
            else
            {

                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Please enter the correct password. \n");
                printf_tiny("\r");
            }
}

else
        {
            printf_tiny("\n");
            printf_tiny("\r");
            printf_tiny(" You are not admin. \n");
            printf_tiny("\r");
            lcdbusywait();
            *lcd_instruction_write = 0x01;
         }


}
char a3,b3,c3,a4,b4,c4;
char x4,y4,z4,x5,y5,z5;
char x7,y7,z7,x8,y8,z8;
char X,Y,Z,A,C,D;

//Function for check in of raj
void logbookpresence()
{
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Presence Book \n");
    printf_tiny("\r");
    if(count1 == 1)
    {
      printf_tiny("Attendance done at 1st time %d:%d:%d \n",a3,b3,c3);
      printf_tiny("\r");
    }
    else if(count1 == 2)
    {
    printf_tiny("Attendance done at 1st time %d:%d:%d \n",a3,b3,c3);
    printf_tiny("\r");
    printf_tiny("Attendance done at 2nd time %d:%d:%d \n",a4,b4,c4);
    printf_tiny("\r");
    }

    else
    {
        printf_tiny("No capacity to store more \n");
        printf_tiny("\r");
    }
}

//Function for check in of Meet
void logbookpresence1()
{
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Presence Book \n");
    printf_tiny("\r");
    if(count2 == 1)
    {
      printf_tiny("Attendance done at 1st time %d:%d:%d \n",x,y,z);
      printf_tiny("\r");
    }
    else if(count2 == 2)
    {
    printf_tiny("Attendance done at 1st time %d:%d:%d \n",x,y,z);
    printf_tiny("\r");
    printf_tiny("Attendance done at 2nd time %d:%d:%d \n",p,q,r);
    printf_tiny("\r");
    }
    else
    {
        printf_tiny("\n");
        printf_tiny("\r");
        printf_tiny("No capacity to store more \n");
        printf_tiny("\r");
    }
}

//Function for check in of custom name
void logbookpresence2()
{
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Presence Book \n");
    printf_tiny("\r");

    if(count10 == 1)
    {
      printf_tiny("Attendance done at 1st time %d:%d:%d \n",m1,n1,o1);
      printf_tiny("\r");
    }
    else if(count10 == 2)
    {
    printf_tiny("Attendance done at 1st time %d:%d:%d \n",m1,n1,o1);
    printf_tiny("\r");
    printf_tiny("Attendance done at 2nd time %d:%d:%d \n",j2,k2,l2);
    printf_tiny("\r");
    }
    else
    {   printf_tiny("\n");
    printf_tiny("\r");
        printf_tiny("No capacity to store more \n \r \n \r");
    }
}

//Function for check out of Raj
void logbookleave()
{
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Leave Book \n");
    printf_tiny("\r");

    if(r9 == 1)
    {
      printf_tiny("leaving done at 1st time %d:%d:%d \n",x4,y4,z4);
      printf_tiny("\r");
    }
    else if(r9 == 2)
    {
    printf_tiny("Leaving done at 1st time %d:%d:%d \n",x4,y4,z4);
    printf_tiny("\r");
    printf_tiny("Leaving done at 2nd time %d:%d:%d \n",x5,y5,z5);
    printf_tiny("\r");
    }
    else
    {
        printf_tiny("\n");
        printf_tiny("\r");
        printf_tiny("No capacity to store more \n");
        printf_tiny("\r");
    }
}

//Function for check out of Meet
void logbookleave1()
{
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Leave Book \n");
    printf_tiny("\r");

    if(r10 == 1)
    {
      printf_tiny("leaving done at 1st time %d:%d:%d \n",x7,y7,z7);
      printf_tiny("\r");
    }
    else if(r10 == 2)
    {
    printf_tiny("Leaving done at 1st time %d:%d:%d \n",x7,y7,z7);
    printf_tiny("\r");
    printf_tiny("Leaving done at 2nd time %d:%d:%d \n",x8,y8,z8);
    printf_tiny("\r");
    }
    else
    {
        printf_tiny("\n");
        printf_tiny("\r");
        printf_tiny("No capacity to store more \n");
        printf_tiny("\r");

    }
}

//Function for check out of Custom name
void logbookleave2()
{
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Leave Book \n ");
    printf_tiny("\r");

    if(r11 == 1)
    {
      printf_tiny("leaving done at 1st time %d:%d:%d \n",X,Y,Z);
      printf_tiny("\r");
    }
    else if(r11 == 2)
    {
    printf_tiny("Leaving done at 1st time %d:%d:%d \n",X,Y,Z);
    printf_tiny("\r");
    printf_tiny("Leaving done at 2nd time %d:%d:%d \n",A,C,D);
    printf_tiny("\r");
    }
    else
    {
        printf_tiny("\n");
        printf_tiny("\r");
        printf_tiny("No capacity to store more \n");
        printf_tiny("\r");
    }
}

uint8_t r6, r7;
uint8_t s5,s6,s7,s8;

//Function for Hours worked of Raj
void resulttime()
{
    if(count1 == 1 && r9 == 1)
    {
    r4 = ((int)a3 * 3600) ;
    r5 = ((int)b3 * 60);
    r4 = (r4 + r5 + (int)c3) ;
    printf_tiny("\n");
    printf_tiny("\r");
    r6 = ((int)x4 * 3600);
    r7 = ((int)y4 * 60);
    r6 = (r6+r7+(int)z4);
    r6 = r6 - r4;
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Seconds worked in 1st day %d \n",r6);
    printf_tiny("\r");
    r6 = (r6 / 3600 );
    printf_tiny("Hours worked on 1st day %d \n",r6);
    printf_tiny("\r");
    }
    else if(count1 == 2 & r9 ==2)
    {
        r4 = ((int)a3 * 3600) ;
    r5 = ((int)b3 * 60);
    r4 = (r4 + r5 + (int)c3) ;
    printf_tiny("\n");
    printf_tiny("\r");
    r6 = ((int)x4 * 3600);
    r7 = ((int)y4 * 60);
    r6 = (r6+r7+(int)z4);
    r6 = r6 - r4;
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Seconds worked in 1st day %d \n",r6);
    printf_tiny("\r");
    r6 = (r6 / 3600 );
    printf_tiny("Hours worked on 1st day %d \n",r6);
    printf_tiny("\r");

    s1 = ((int)a4 * 3600) ;
    s2 = ((int)b4 * 60);
    s1 = (s1 + s2 + (int)c4) ;
    printf_tiny("\n");
    printf_tiny("\r");
    s3 = ((int)x5 * 3600);
    s4 = ((int)y5 * 60);
    s3 = (s3+s4+(int)z5);
    s3 = s3 - s1 ;
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Seconds worked in 2nd day %d \n",s3);
    printf_tiny("\r");
    s3 = (s3 / 3600 );
    printf_tiny("Hours worked on 2nd day %d \n",s3);
    printf_tiny("\r");
    }

}

////Function for Hours worked of Meet
void resulttime1()
{
    if(count2 == 1 && r10 == 1)

 {

    s5 = ((int)x * 3600) ;
    s6 = ((int)y * 60);
    s5 = (s5 + s6 + (int)z) ;
    printf_tiny("\n");
    printf_tiny("\r");
    s7 = ((int)x7 * 3600);
    s8 = ((int)y7 * 60);
    s7 = (s7+s8+(int)z7);
    s7 = s7 - s5;
    printf_tiny("\n");
    printf_tiny("\r");
    printf_tiny("Seconds worked in 1st day %d \n",s7);
    printf_tiny("\r");
    s7 = (s7 / 3600 );
    printf_tiny("Hours worked on 1st day %d \n",s7);
    printf_tiny("\r");
 }

}


void main()
{
    lcdinit();
    lcdbusywait();
    init_serial();
    delay_ms(100);
    printf("How are you today?\r");
    delay_ms(3000);
    printf(" Welcome to Class \r");
    *lcd_instruction_write = 0x80;
    delay_ms(100);
    stringprint("RFID");
    *lcd_instruction_write = 0xC0;
    delay_ms(100);
     stringprint("Based");
    *lcd_instruction_write = 0x90;
    delay_ms(100);
    stringprint("Attendance");
    *lcd_instruction_write = 0xD0;
    delay_ms(100);
    stringprint("System");
    delay(200);
    *lcd_instruction_write = 0x01;

//RTC
while(1)
{

    if(set==0)
 {

  I2CStart();
 I2CSend(0xD0);
 I2CSend(0x00);
 I2CSend(0x00);
 I2CSend(0x00);
 I2CSend(0x00);
 I2CSend(0x01);
 I2CSend(0x01);
 I2CSend(0x01);
 I2CSend(0x00);
 I2CSend(0x80);
 I2CStop();
  k=1;
  set_time();                     // call time set function
     min=DecimalToBCD(min);
   sec=DecimalToBCD(0);
      hour=DecimalToBCD(hour);
      year=DecimalToBCD(year);
      mon=DecimalToBCD(mon);
   date=DecimalToBCD(date);
  I2CStart();
  I2CSend(0xD0);
  I2CSend(0x00);
  I2CSend(0x00);
  I2CSend(min);
  I2CSend(hour);
  I2CSend(day1);
  I2CSend(date);
  I2CSend(mon);
  I2CSend(year);
  I2CSend(0x80);
  I2CStop();
  *lcd_instruction_write = (1);
  *lcd_instruction_write = (0x0c);
  }

  I2CStart();
  I2CSend(0xD0);
  I2CSend(0x00);
  I2CStart();
  I2CSend(0xD1);
  sec=BCDToDecimal(I2CRead(1));
  min=BCDToDecimal(I2CRead(1));
  hour=BCDToDecimal(I2CRead(1));
  //day1=BCDToDecimal(I2CRead(1));
  date=BCDToDecimal(I2CRead(1));
  mon=BCDToDecimal(I2CRead(1));
  year=BCDToDecimal(I2CRead(1));
  I2CStop();
  delay(1);

    //Menu
       printf_tiny("\n \r \n \rEnter 'a' for attendance register \n \rEnter 'b' Attendance Check \n \rEnter 'c' for registration \n \rEnter 'd' for delete attendance \n \rEnter 'e' for Log out \n \r");
        command = getchar();
        printf_tiny("You Entered \n ");
        putchar(command);
        printf_tiny("\r");
        delay_ms(200);
        if(command == 'a')
        {

            *lcd_instruction_write = 0x01;
            lcdbusywait();
            printf_tiny("Place Your Card ");
            printf_tiny(" \n \r");
            recieve();
            delay_ms(1600);
            *lcd_instruction_write = 0x80;
            lcdbusywait();
            stringprint("Your Unique ID");
            printf_tiny(" \n \r");
            delay_ms(100);
            *lcd_instruction_write = 0xC0;

         for(l=0;l<12;l++)
         {
              lcdbusywait();
              lcdwrite(card_id[l]);
         }

        delay_ms(1500);
        printf_tiny("\n \r");
if(strncmp(card_id,"04001C9B0487",12)==0)
{
            show_time();


            if(count1 == 0)
            {
            count1++;
            *lcd_instruction_write =0x90;
            lcdbusywait();
            stringprint("raj is present");
            delay_ms(1500);
            printf_tiny("\n \r");
            // printf_tiny("%d:%d:%d",hour,min,sec);
             a3 = hour;
             b3 = min;
             c3 = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;
            }
            else if(count1 == 1 && r9 ==1)
            {
                count1++;
            *lcd_instruction_write =0x90;
            lcdbusywait();
            stringprint("raj is present");
            delay_ms(1500);
            printf_tiny("\n \r");
           // printf_tiny("%d:%d:%d",hour,min,sec);
             a4 = hour;
             b4 = min;
             c4 = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;

            }

             else
            {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("put leave time \n ");
                printf_tiny("\r");
            }
}

else if(strncmp(card_id,"04001BAAE85D",12)==0)
        {
            show_time();


            if(count2 == 0)
            {
            count2++;
                *lcd_instruction_write =0x90;
            stringprint("meet is present");
            delay_ms(1500);
            printf_tiny("\n \r");
           //  printf_tiny("%d:%d:%d",hour,min,sec);
             x = hour;
             y = min;
             z = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;
            }
            else if(count2 == 1 && r10 ==1)
            {
                count2++;
                *lcd_instruction_write =0x90;
            stringprint("meet is present");
            delay_ms(1500);
            printf_tiny("\n \r");
           //  printf_tiny("%d:%d:%d",hour,min,sec);
             p = hour;
             q = min;
             r = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;
            }
            else
            {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Put leave time \n");
                printf_tiny("\r");
            }

        }
else if((strncmp(card_id,card_id1,12)==0))
        {
            show_time();


           if(count10 == 0)
            {
                count10++;
                *lcd_instruction_write =0x90;
            lcdbusywait();
            stringprint(size);
            lcdbusywait();
            stringprint(" is present");
            delay_ms(1500);
            printf_tiny("\n \r");
          //  printf_tiny("%d:%d:%d",hour,min,sec);
            m1 = hour;
            n1 = min;
            o1 = sec;
            delay_ms(1000);
            *lcd_instruction_write = 0x01;
            }
            else if(count10 == 1 && r11 == 1)
            {
                count10++;
                *lcd_instruction_write =0x90;
            lcdbusywait();
            stringprint(size);
            lcdbusywait();
            stringprint(" is present");
            delay_ms(1500);
            printf_tiny("\n \r");
          //   printf_tiny("%d:%d:%d",hour,min,sec);
             j2 = hour;
             k2 = min;
             l2 = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;
            }
             else
            {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("put leave time \n");
                printf_tiny("\r");
            }
}

else
{
    printf_tiny("Your Card is not registered. Please register it. \n \r \n \r");
    lcdbusywait();
    *lcd_instruction_write = 0x01;

}
        }


else if(command == 'b')
{
        *lcd_instruction_write = 0x01;
       printf_tiny("\n");
       printf_tiny("\r");
     printf_tiny("Place Your Card");
     printf_tiny("\n");
     printf_tiny("\r");
        recieve();
        delay_ms(200);
        *lcd_instruction_write = 0x80;
        lcdbusywait();
        stringprint("Your Unique ID");
        delay_ms(200);
        *lcd_instruction_write = 0xC1;
         for(l=0;l<12;l++)
         {
              lcdbusywait();
              lcdwrite(card_id[l]);

          }

          delay_ms(1500);
        *lcd_instruction_write = 0x01;

if(strncmp(card_id,"04001C9B0487",12)==0)
{

            *lcd_instruction_write = 0x01;
            lcdbusywait();
            *lcd_instruction_write = 0x80;
            stringprint("Attendance Check");
            delay_ms(200);
            attendance();
            logbookpresence();
            logbookleave();
            resulttime();
            delay_ms(200);

}


else if(strncmp(card_id,"04001BAAE85D",12)==0)
{
            *lcd_instruction_write = 0x01;
            lcdbusywait();
            *lcd_instruction_write = 0x80;
            stringprint("Attendance Check");
            attendance2();
            logbookpresence1();
            logbookleave1();
            resulttime1();
            delay_ms(200);
}

else if(strncmp(card_id,card_id1,12)==0)
{
            *lcd_instruction_write = 0x01;
            lcdbusywait();
            *lcd_instruction_write = 0x80;
            stringprint("Attendance Check");
            attendance3();
            logbookpresence2();
            logbookleave2();
            delay_ms(200);
}
else
{
    printf_tiny(" Your Card is not having any attendance registration. Please register your card. \n \r\ \n \r");
    lcdbusywait();
    *lcd_instruction_write = 0x01;
    lcdbusywait();
}


}
else if(command == 'c')
{
    printf_tiny("Do you want to register yourself ? Y or N \n");
    printf_tiny("\r");
    command = getchar();
    printf_tiny("You Entered : \n");
    printf_tiny("\r");
    putchar(command);
    if(command == 'Y')
    {
        *lcd_instruction_write = 0x01;
        lcdbusywait();
        *lcd_instruction_write = 0x80;
        lcdbusywait();
        stringprint("Place Your Card");
        recieve();
        *lcd_instruction_write = 0xC1;      //Place cursor to second position of second line
        for(l=0;l<12;l++)
         {
              lcdbusywait();
              lcdwrite(card_id[l]);
        }
        lcdbusywait();
        *lcd_instruction_write = 0x01;
        delay_ms(200);
        if((strncmp(card_id,"04001BAAE85D",12)==0)|| (strncmp(card_id,"04001C9B0487",12)==0) ||(strncmp(card_id,card_id1,12)==0))
            {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Your are already registered. Please select from option a or b \n");
                printf_tiny("\r");
                lcdbusywait();
                *lcd_instruction_write = 0x01;
                lcdbusywait();
            }
        else
        {
        printf_tiny("\n");
        printf_tiny("\r");
        printf_tiny("We have checked your card. It is not registered. \n");
        printf_tiny("\r");
        registration();
        delay_ms(1600);
        }
    }
    else if(command == 'N')
    {
        delay_ms(200);
        *lcd_instruction_write = 0x01;
        printf_tiny("\n");
        printf_tiny("\r");
        printf_tiny("Not Registered \n");
        printf_tiny("\r");
        delay_ms(200);
    }
    else
    {
        printf_tiny("Please input from the choice \n");
        printf_tiny("\r");
    }
}
else if(command == 'd')
{
    delay_ms(100);
    deleteattendance();
    delay_ms(109);
}
else if(command == 'e')
{

            *lcd_instruction_write = 0x01;
            lcdbusywait();
            *lcd_instruction_write = 0x80;
            printf_tiny("\n");
            printf_tiny("\r");
            printf_tiny("Place Your Card");
            recieve();
            delay_ms(500);
            *lcd_instruction_write = 0x01;
            lcdbusywait();
            *lcd_instruction_write = 0x80;
            stringprint("Your Unique ID");
            lcdbusywait();
            *lcd_instruction_write = 0xC0;

         for(l=0;l<12;l++)
         {
              lcdbusywait();
              lcdwrite(card_id[l]);
         }

        delay_ms(1500);
        *lcd_instruction_write = 0x01;
        if(strncmp(card_id,"04001C9B0487",12)==0)
        {
            printf_tiny("\n");
            printf_tiny("\r");
           // printf_tiny("You are leaving \n");
          //  printf_tiny("\r");
            delay_ms(100);
            if(count1 > 0)

        {
             r9++;
            *lcd_instruction_write =0x90;
            lcdbusywait();
            stringprint("raj is leaving");
            delay_ms(500);
            show_time();
             delay_ms(1500);
            printf_tiny("\n");
            printf_tiny("\r");
            if(r9 == 1)
            {
           // printf_tiny("%d:%d:%d",hour,min,sec);
             x4 = hour;
             y4 = min;
             z4 = sec;
            delay_ms(1000);
            *lcd_instruction_write = 0x01;

            }
            else if(r9 == 2)
            {

            // printf_tiny("%d:%d:%d",hour,min,sec);
             x5 = hour;
             y5 = min;
             z5 = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;

            }
            else
                {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("No previous record present");
                printf_tiny("\n");
                printf_tiny("\r");
            }
        }
        }
        else if(strncmp(card_id,"04001BAAE85D",12)==0)

        {
            printf_tiny("\n");
            printf_tiny("\r");
            delay_ms(100);
            if(count2 > 0)
            {
            r10++;
            *lcd_instruction_write =0x90;
            stringprint("meet is leaving");
            delay_ms(500);
            show_time();
             delay_ms(1500);
            printf_tiny("\n");
            printf_tiny("\r");
            if(r10 == 1)
            {
             //printf_tiny("\n");
           //  printf_tiny("\r");
           //  printf_tiny("%d:%d:%d",hour,min,sec);
             x7 = hour;
             y7 = min;
             z7 = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;

            }
            else if(r10 == 2)
            {

           //  printf_tiny("\n");
           //  printf_tiny("\r");
           //  printf_tiny("%d:%d:%d",hour,min,sec);
             x8 = hour;
             y8 = min;
             z8 = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;

            }
            else
            {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Max Number of count achieved. No More attendance can be taken into log.");
                printf_tiny("\n");
                printf_tiny("\r");
            }
    }
        }

        else if((strncmp(card_id,card_id1,12)==0))
{
            printf_tiny("\n");
            printf_tiny("\r");
            putstr(size);printf_tiny(" is leaving");
            printf_tiny("\n");
            printf_tiny("\r");
            delay_ms(100);
            if(count10 > 0)
            {
            r11++;
            *lcd_instruction_write =0x90;
            lcdbusywait();
            stringprint(size);
            lcdbusywait();
            stringprint("is leaving");
            lcdbusywait();
            delay_ms(500);
            show_time();
            delay_ms(1500);
             printf_tiny("\n");
             printf_tiny("\r");
            if(r11== 1)
            {
           //  printf_tiny("\n");
            // printf_tiny("\r");
            // printf_tiny("%d:%d:%d",hour,min,sec);
             X = hour;
             Y = min;
             Z = sec;
            delay_ms(1000);
            *lcd_instruction_write = 0x01;

            }
            else if(r11 == 2)
            {
            // printf_tiny("\n");
            // printf_tiny("%d:%d:%d",hour,min,sec);
             A = hour;
             C = min;
             D = sec;
             delay_ms(1000);
            *lcd_instruction_write = 0x01;

            }
            else
            {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("Max Number of count achieved. No More attendance can be taken into log. \n");
                printf_tiny("\n");
            }
            }
}
                else
                {
                printf_tiny("\n");
                printf_tiny("\r");
                printf_tiny("No previous record present \n");
                printf_tiny("\r");
                printf_tiny("\n");
                printf_tiny("\r");
                }

}
else
{
    printf_tiny(" PLease enter from the above choice \n");
    printf_tiny("\r");
}


        }

}


