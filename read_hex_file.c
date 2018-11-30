#include "read_hex_file.h"

//check if c is a valid hex char
int valid_digit(char c)
{
	int num = (int)c;
	//printf("c: %c, and num:%d \n", c, num);

	if((num >= 0x30)&&(num <= 0x39)) return 1;   //0<=c<=9
	if((num >= 0x41)&&(num <= 0x46)) return 1;   //A<=c<=F
	if((num >= 0x61)&&(num <= 0x66)) return 1;   //a<=c<=f

	return 0;
	
}

//convert single char to the corresponding int value
//Because this function should only be called after valid_digit
//for simplicity, validity of char c is not checked in
//this function.
int char_to_int(char c)
{
	if((c >= 0x30)&&(c <= 0x39)) return ((int)(c-0x30));
	if((c >= 0x41)&&(c <= 0x46)) return ((int)(c-0x41+10));
	if((c >= 0x61)&&(c <= 0x66)) return ((int)(c-0x61+10));
	
}

//convert two chars to the corresponding 2-digit value
//Because this function should only be called after valid_digit
//for simplicity, validity of char c1 and char c2 is 
//not checked in this function.
int convert_to_int(char c1, char c2)
{

	//printf("c1: %c and its integer: %d \n", c1, char_to_int(c1));
	//printf("c2: %c and its integer: %d \n", c2, char_to_int(c2));
	return ((char_to_int(c1)*16) + char_to_int(c2));
}

//read hex string from file
//return 0 if process fails, and non-0 otherwise.
int read_hex_file(uint8_t *buffer, char *fileName)
{
	FILE *fp;
	int ch1,ch2;
	int counter = 0;

	if(access(fileName, F_OK ) == -1 ) 
	{
	    printf("File \"%s\" does not exist\n", fileName);
	    return 0;
	} 
 	//printf("File \"%s\" exists \n", fileName);

	fp = fopen(fileName, "r");

	

	while(!feof(fp))
	{
		ch1 = getc(fp); 
		//printf("%c",ch1);
		if((ch1 == EOF)||(!valid_digit(ch1)))
		{
			//printf("\ncondition one is not met.is valid: %d\n", valid_digit(ch1));
			//printf("is ch1 == EOF: %d\n", (ch1==EOF));
			break;
		}

		ch2 = getc(fp);
		//printf("%c",ch2); 
		if((ch2 == EOF)||(!valid_digit(ch2)))
		{
			buffer[counter++] = convert_to_int('0', ch1);
			break;
		}

		
		buffer[counter++] = 0xff&(convert_to_int(ch1,ch2));
		
		
		
	}//; 
	/*
	printf("------------------------------------------------\n");
    	printf("counter: %d and buffer is: \n", counter);
    	for(int i=0; i<counter; i++) printf("%02x", buffer[i]);
    	printf("\n------------------------------------------------\n");
	*/

	
  	return counter;

	
}

