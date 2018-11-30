#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keccak.h"
#include "oaes_lib.h"
#include "read_hex_file.h"


char txFile[8]= "tx.txt\0";	//this file should contain the tx binary.
char crFile[7]= "cr.txt\0";	//this file should contain 64 bytes of c and r (that contain the hidden msg).




//This function extracts a hidden msg (if any) given a key, c, and r "random" numbers.
void extract(uint8_t *key, uint8_t *c, uint8_t *r)
{
	int k_len = 16;
	int block_len = 16;
	uint8_t extracted_IV[16];

	OAES_RET result;
	OAES_CTX * ctx;

	// create ctx and set key
	ctx = oaes_alloc();
	oaes_set_option((oaes_ctx *)ctx, OAES_OPTION_ECB, NULL );
	result = oaes_key_import_data(ctx, key, k_len);
	if(result != 0){
		printf("Result of oaes_key_import_data: %d - Error, return \n",(int) result);
		return;
	}

	//1- extract IV
	memcpy(extracted_IV, c, block_len);

	//2- decrypt IV
	result = oaes_decrypt_block_n(ctx, extracted_IV, block_len);
	if(result != 0){
		printf("Result of 1st half of r oaes_decrypt_block: %d - Error, return \n",(int) result);
		return;
	}

	
	printf("Decrypted IV: ");
    	for(int i=0; i<block_len; i++) printf("%02x", extracted_IV[i]);
    	printf("\n");

	//3- check if last 8 bytes are equal to start_pattern (0's)
	int start_pattern = 1;
	for(int d=0; d<8; d++)
	{
		if(extracted_IV[8+d] != 0) start_pattern = 0;
	}

	
	if(start_pattern)
	{
		printf("-------------------------------------------------------------------------\n");
		printf("Start pattern was successfully found.\n");
		
		//4- extract IV (again)
		memcpy(extracted_IV, c, block_len);
		printf("IV: ");
	    	for(int i=0; i<block_len; i++) printf("%02x", extracted_IV[i]);
	    	printf("\n");

		//5- decrypt 1st half of r
		result = oaes_decrypt_block_n(ctx, r, block_len);
		if(result != 0){
			printf("Result of 1st half of r oaes_decrypt_block: %d - Error, return \n",(int) result);
			return;
		}
	
		//6- xor decrypted 1st half of r with extracted IV
		for(int i=0; i<block_len; i++) r[i] ^= extracted_IV[i];

		printf("Hidden message is (16 bytes): ");
	    	for(int i=0; i<block_len; i++) printf("%c ", r[i]);
	    	printf("\n");
		printf("-------------------------------------------------------------------------\n");
	}
	else
	{
		printf("## Unable to extract any hidden message ##\n");
	
	}

	return;
}

//this function is copied from the Bytecoin project
void hash_process(union hash_state *state, const uint8_t *buf, size_t count) {
	keccak1600(buf, (int)count, (uint8_t*)state);
}


//This function reads the file contains the two random numbers: c, and r.
//Sets the key, and calls extract(..) to extract the hidden msg.
void extract_hidden_msg()
{
	uint8_t c[32];
	uint8_t r[32];

	uint8_t buffer[2000];
    	int counter = 0;
    	char fileName[7]= "cr.txt\0";
	
	counter = read_hex_file(buffer,crFile);

	if(!counter)
	{	
		printf("Failed to read \"%s\" - make sure c and r in cr.txt\n", crFile);
		return;
	}

	if(counter != 64)
	{
		printf("Invalid length of cr.txt\n");
		return;
	}

	memcpy(c, buffer, 32);
	memcpy(r, buffer+32, 32);
	

	printf("-------------------------------------------------------------------------\n");
	printf("c and r (check if they are found within tx binary): \n");
	printf("c: ");
	for(int i=0; i<32; i++) printf("%02x", c[i]);
	printf("\n");
	printf("r: ");
	for(int i=0; i<32; i++) printf("%02x", r[i]);
	printf("\n");
	printf("-------------------------------------------------------------------------\n");

	uint8_t key[16];
	int k_len = 16;
	//use a 16-byte key: {14,15,...,29}
	for(int i=0; i<k_len; i++) key[i] = 14 + i;

	extract(key,c,r);
}


//This function reads in the file containing the tx binary, and hashes it using hash_process()
void hash_tx()
{
	
	uint8_t buffer[2000];
	int counter = 0;
	counter = read_hex_file(buffer,txFile);

	if(!counter) printf("Failed to read \"%s\" - make sure tx binary in \"%s\"\n", txFile, txFile);
	/*
	printf("------------------------------------------------\n");
	printf("tx size: %d and tx binary is: \n", counter);
	for(int i=0; i<counter; i++) printf("%02x", buffer[i]);
	printf("\n------------------------------------------------\n");
	*/
	unsigned char hash[HASH_SIZE];
	union hash_state state;
	hash_process(&state, buffer, counter);
	memcpy(hash, &state, HASH_SIZE);

	printf("-------------------------------------------------------------------------\n");
	printf("hash of the given tx (check if it corresponds to paper and chain explorer): \n");
	for(int i=0; i<HASH_SIZE; i++) printf("%02x", hash[i]);
	printf("\n");
	printf("-------------------------------------------------------------------------\n");

	return;
}

// main
int main(int argc, char **argv)
{
	printf("This a demo tool to hash a given tx binary, and extracta hidden msg\n");
	printf("************************************************************************ \n"); 
	printf("Before proceeding make sure to provide two files: %s and %s \n", txFile, crFile);
	printf("************************************************************************ \n");
	
	printf("\n");
	printf("\n");
	
	hash_tx();
	extract_hidden_msg();
    

    	return 0;
}
