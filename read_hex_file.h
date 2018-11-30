#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

//check if c is a valid hex char
int valid_digit(char c);

//convert single char to the corresponding int value
int char_to_int(char c);

//convert two chars to the corresponding 2-digit value
int convert_to_int(char c1, char c2);

//read hex string from file
int read_hex_file(uint8_t *buffer, char *fileName);

