# hash_and_extract
This repository is solely for academic demo purposes

The transaction binary is included in tx.txt
The pair of relevant (c,r) is found in cr.txt
The generated hash should be as in the paper and the blockchain explorer, unless tx.txt is not valid.
(c,r) can also be seen in tx binary.
When compiled and executed, the tool should be able to display the 'decrypted' IV and extract a 16-byte message.

To compile and run the tool:
user$ gcc *.c -o he
user$ ./he
