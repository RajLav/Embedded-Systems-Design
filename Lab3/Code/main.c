*/Name :- Raj Lavingia
Date :-03/16/2018
*/

#include <malloc.h> // Memory allocation
#include <at89c51ed2.h>  // includes features of 8051 and 8052 controller
#include <string.h> // characters
#include <stdio.h> // i/p and o/p functions
#include <stdlib.h> // standard libraries
#include <mcs51reg.h> //supports more than 20 microcontrollers

#define DEBUG            //Enables the debug port

#ifdef DEBUG

#define DEBUGPORT(x) dataout(x);  // generates a MOVX 0FFFFh,x where x is an 8-bit value
#else

#endif

//heap size declaratio

#define HEAP_SIZE 4000  // if size is more than defualt value sdcc -c _heap.c -D HEAP_SIZE= 1024

_sdcc_external_startup()  //for 1kb ram by setting up bits xsr0 and xsr1 to 1.
{
    AUXR |= 0x0C;
    return 0;
}

unsigned char  heap1[HEAP_SIZE];

//external ram data declaration

xdata int *a; //declare variables upto 64k

void dataout(int x)
{
   a= (xdata *) 0xFFFF;
   *a = x;
   printf_tiny(" Debug Port \n \r ");
}

xdata char *buffer0; // buffer 0 declared
xdata char *buffer1; // buffer 1 declared
xdata unsigned int *p; //pointer p declared

//get character from rs232
char getchar ()
{
    while (RI == 0); // wait for character to be received, spin on RI
	RI = 0;			// clear RI flag
	return SBUF;  	// return character from SBUF
}

// for rs232 printing
void putchar (char c)
{
    while (TI == 0); // wait for TX ready, spin on TI
    SBUF = c;  	// load serial port with transmit value
   	TI = 0;  	// clear TI flag
}


// print a string
void putstr (char *p)
{
	int i = 0;
	while (*(p+i)!='\0') //output characters until NULL found
    {
		putchar(*(p+i));
		i++;
	}

}

//characters

char *buffer_array[200];
char *size,*size1,command ;
char *buffer_num; // to get buffer number from user

char data_get[200]; //get data from user

//unsigned values

unsigned int store=0;
unsigned command_count1=0;
unsigned char_received;
unsigned int buffer_index, buffer_size[100] ;
unsigned result,n;
unsigned temp,result=0;

unsigned int i,j;
unsigned command_count = 0,clear;
unsigned new,elements,temp;

//New buffer creation


char *creationofbuffer(unsigned int size)
{
   char *temp= malloc((size)); //allocation of new buffer
   printf_tiny("created \n \r");
   n++;
   return temp;
}

// Buffer shift
void shiftofbuffer(int buffer_result)
{
    unsigned int start= buffer_result;
    while(start < (n-1)) //Keep deleting a buffer till maximum number of buffers present
    {
        buffer_array[start]=buffer_array[start+1];
        buffer_size[start]=buffer_size[start+1];
        start++;
    }
    printf_tiny("Buffer shifted \n \r");
    n--;
}

// get a string from user
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


void init_hardware()
{
    TMOD=0x20; //use Timer 1, mode 2
    TH1= 0xFD ; // 9600 baud rate
    SCON=0x50; //SCON initialisation
    TR1=1;   //Start the Timer
    TI=1; //Set TI flag to 1
}

void heap_report()
{
    printf_tiny("Storage Characters %d \n \r",store);
    printf_tiny("command characters %d \n \r",command_count);
    printf_tiny("no of %d buffers \n \r",n);
    printf_tiny("Buffer in Heap \n \r");
    for(i=0;i<n;i++)
    {
      printf_tiny("[%d] buffer%d \r \n",(i+1),i);
    }
    printf_tiny("Report of each buffer in the heap: \n \r");

    for(i=0;i<n;i++)
    {
      elements=0;
      printf_tiny("Buffer %d report: \r \n",i);
      printf_tiny("Size of Buffer%d is %d \r \n",i,buffer_size[i]);
      printf_tiny("Starting address of Buffer%d is %d \r \n",i,(unsigned int)buffer_array[i]);
      printf_tiny("Ending address of Buffer%d is %d \r \n",i,(((unsigned int)buffer_array[i]) + buffer_size[i]));

      while(*(buffer_array[i]+elements)!='\0') //Calculate number of elements in each buffer
      {
         elements++;
      }
      printf_tiny("Free space in Buffer%d is %d \r \n",i,(unsigned int)(buffer_size[i] - elements));
      if(i==0)
      {
        clear=0;
        printf_tiny("Number of storage characters in buffer%d are %d \r \n",i,store);
        if(store!=0)
        {
          printf_tiny("The storage characters are: \r \n");
        }
        while(clear<store) //Empty buffer0
        {
          temp=0;
          do
          {
          putchar(*(buffer0+clear));
        *(buffer0+clear)='\0';
          clear++;
          temp++;
          }while(temp<32);
          printf_tiny(" \r \n");
        }

        if(store!=0)
        printf_tiny("Buffer0 is emptied\r\n");
        else
        printf_tiny("Buffer0 is already empty\r\n");
        store=0;
        command_count=0;
      }

      else
         printf_tiny("There are no storage characters in buffer%d\r\n",i);

    }
    printf_tiny("Number of characters received since last '?' command is %d\r\n",char_received);
    char_received=0;
}

/*This function displays values in buffer0 in hex format*/
void buffer0_hex()
{
    printf_small("Hex values in Buffer 0 are: \r \n");
    i=0;
    if(store==0)//check if buffer0 has storage characters
    printf_tiny("There are no storage characters in Buffer0 \r \n");
    while(i<store) //keep printing till the last storage character
    {
        if((i%16)==0)
        {
            printf_small("\r\n");
            printf("%04X:",(unsigned int)(buffer0+i)); //print the address of the 1st byte of 16 bytes in each line

        }
            printf("%02X\t",(*(buffer0+i))); //print the character in buffer0
            i++;
    }

    printf_tiny("\r\n");
}

/*This function is used for freeing the buffers in the heap*/
void buffer_free()
{
    for(i=0;i<n;i++) //free all the buffers created
    {
        free(buffer_array[i]);
        buffer_array[i]='\0';
    }
    free(buffer0);
    free(buffer1);
    printf_tiny("The space allocated to buffers in the heap is freed\n\r");


}


void add()
{

     do
            {
                do
                {
       back:    printf_tiny("enter between 20 & 400 \n \r");
                size1= getstr(); //obtain the new buffer size
                printf_tiny("entered buffer size ");
	            putstr(size1);

	            i=0;
	            new=0;
	            while(*(size1+i)!='\0') //Check if the size is a number
                {
                   if(*(size1+i)>= '0' && *(size1+i)<='9')
                   {
                       i++;

                   }
                   else

                    {
                    printf_tiny("Please enter a number \r \n");
                    goto back;
                    }
                }
                i=0;
                new=atoi(size1); //Convert ascii size to an integer
                printf_tiny("The buffer size integer value is %d \n",new);
                }while(new<20 || new>400); //Check if the size is between 20 and 400

                buffer_array[buffer_index]=creationofbuffer(new);//Create a new buffer

                if ((buffer_array[buffer_index]) == 0)  //allocate new buffer
                {
                    printf_tiny("malloc buffer%d failed, Please enter a smaller size \n \r",buffer_index);
                    free(buffer_array[buffer_index]);
                }

             } while (buffer_array[buffer_index] == 0); //if fails then ask the user to give the size again
             printf_tiny("malloc passed for buffer%d \n \r",buffer_index);
             buffer_size[buffer_index]=new;
             buffer_index++;
}


void delete()
{
            printf_tiny("Enter a valid buffer number \n \r");
   back:    buffer_num=getstr();//get the buffer number from the user
            printf_tiny("The buffer number you entered is: ");
            putstr(buffer_num);
            printf_tiny("\n\r");
            result=0;
            i=0;
            while(*(buffer_num+i)!='\0') //Convert ascii to integer
                {
                    result= result*10+(*(buffer_num+i) - '0');
                    i++;
                }
            if(result >(n-1)) //check if the buffer with that number does not exist
            {
                printf_tiny("Enter a valid buffer number \n \r");
                goto back;
            }
            else if(result==0) //check if the user is trying to delete buffer0
            {
                printf_tiny("Enter a valid buffer number other than 0 \n \r");
                goto back;
            }
            else
            {
                printf_tiny("Buffer%d deleted \n \r",result);
                free(buffer_array[result]);
                buffer_array[result]= NULL;
                shiftofbuffer(result);
                buffer_index--;
            }
}

void main()
{
        init_hardware();  // initialise hardware
        buffer_index = 2;
	    n = 2; //buffer0 and buffer 1 initialised

        init_dynamic_memory((MEMHEADER xdata *)heap1, HEAP_SIZE); //allocates heap of size HEAP_SIZE

loop:do
      {
        do
        {
          printf_tiny("buffer size between 32 and 3200 bytes divisible by 32 \n \r"); //print command
          size= getstr(); //ask for a size from user
          printf_tiny("user entered buffer size \n \r ");
	      putstr(size);
	      store = 0;
	      command_count1 = 0;
	      i=0;
//check if the size entered by user is correct or not

	      while(*(size+i)!='\0') // size entered is correct or not
          {
            if(*(size+i)>= '0' && *(size+i)<='9')
            {
                i++;

            }
            else
                {
                    printf_tiny("enter a number \n \r "); //user must give no again
                    goto loop;
                }
          }

	      result=atoi(size); //ascii to integer conversion
       printf_tiny("The buffer size integer value is %d \n \r",result); //get the answer in the form of value

       }while(((result%32)!=0)||(result<32)||(result>3200));

        if ((buffer0 = malloc((result))) == 0)
        {
          printf_tiny("buffer0 failed \n \r");
          free(buffer0); //free buffer0 if failed
        }

	    if ((buffer1 = malloc((result))) == 0)         //allocate buffer1
	    {
		  printf_tiny("buffer1 failed \n \r ");
		  free (buffer1); //free buffer1 if failed
		free (buffer0);
	    }

	} while ((buffer0 == 0) || (buffer1 == 0));

	printf_tiny("malloc passed for both buffer0 and buffer1 \n \r ");
	buffer_array[0]=buffer0; //alloacte buffers in buffer_array and their sizes in buffer_size
	buffer_array[1]=buffer1;
	buffer_size[0]=result;
	buffer_size[1]=result;


	while(1)
    {
        printf_tiny("The command characters with functions are: \n \r");
        printf_tiny(" + , - , ? , = , @ character \n \r"); // all characters which are present , choose any 1 of them
        printf_tiny("\n Enter a character \n \r ");    //prompt the user to enter a character
        command= getchar(); //obtain the character from user
		printf_tiny("The character you entered is :  \n \r ");
	    	putchar(command);

        if((command >='a' && command <='z') || (command >='0' && command <='9'))
        {
            printf_tiny("can be stored \n \r ");
            if(command_count>(buffer_size[0]-1)) //if buffer gets filled
            {
                 printf_tiny("Buffer0 is full \n \r");
                 printf_tiny("The character you entered is: \n \r");
                 putchar(command);

            }
            else
            {
                *(buffer0+command_count)=command;
                 command_count++;
                 store++; //counting storage characters
            }
            char_received++; //counting char
        }
        else
        {

            printf_tiny("a command character \n \r"); //now its command character since conditions of storage char were not fulfilled
            command_count1++ ;
            char_received++;
            switch(command)       //switch case for valid command characters defined with functions
        {
        case '+':
                DEBUGPORT(1)
     		    add();         //add biffer
                break;

        case '-':
            DEBUGPORT(2)
            delete();      //delete buffer
            break;

        case '?':
            DEBUGPORT(3)
            heap_report();        //heap report
            break;

        case '=':
            DEBUGPORT(4)
            buffer0_hex();     //hex values
            break;

        case '@':
            DEBUGPORT(5)
            buffer_free();      //free space from buffers
            goto loop;
            break;

       default:
             printf_tiny("invalid command \n \r ");
        }
        }
    }

}

int atoi(char *a)
{
    i=0;
    temp=0;
    while(*(a+i)!='\0')
    {
        temp= temp* 10 + ( *(a+i)- '0' );
        i++;
    }
    return temp;
}





