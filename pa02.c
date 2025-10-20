/*============================================================================
| Assignment: pa02 - Encrypting a plaintext file using the Hill cipher
|
| Author: Aidan Pinkney
| Language: C
| To Compile: gcc -o pa02 pa02.c
| To Execute: ./pa02 kX.txt pX.txt
|     where kX.txt is the key file
|     and pX.txt is the plaintext
|
| Class: CIS3360 - Security in Computing - Fall 2025
| Instructor: McAlpin
| Due Date: 10/19/2025
+===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TEXT 10000
#define MAX_KEY_SIZE 9
#define LINE_LENGTH 80

// function prototypes (just to keep main shorter)
void readKeyMatrix(FILE *keyFile, int key[MAX_KEY_SIZE][MAX_KEY_SIZE], int *n);
void readPlaintext(FILE *plainFile, char plaintext[MAX_TEXT], int *len);
void hillEncrypt(int key[MAX_KEY_SIZE][MAX_KEY_SIZE], int n, char plaintext[MAX_TEXT], char ciphertext[MAX_TEXT], int len);
void printMatrix(int key[MAX_KEY_SIZE][MAX_KEY_SIZE], int n);
void printText(char text[MAX_TEXT], int len);

// converts a char (a-z) to number 0–25
int charToIndex(char c) {
    return c - 'a';
}

// converts number 0–25 back to lowercase letter
char indexToChar(int idx) {
    return (char)(idx + 'a');
}

int main(int argc, char *argv[]) {

    // check command line args
    if (argc != 3) {
        fprintf(stderr, "Usage: %s keyFile plaintextFile\n", argv[0]);
        return EXIT_FAILURE;
    }
    // open key file
    FILE *keyFile = fopen(argv[1], "r");
    if (!keyFile) {
        fprintf(stderr, "Error opening key file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // open plaintext file
    FILE *plainFile = fopen(argv[2], "r");
    if (!plainFile) {

        fprintf(stderr, "Error opening plaintext file %s\n", argv[2]);
        fclose(keyFile);
        return EXIT_FAILURE;
    }

    int key[MAX_KEY_SIZE][MAX_KEY_SIZE];
    int n = 0;

    // read matrix from file
    readKeyMatrix(keyFile, key, &n);
    fclose(keyFile);

    // read plaintext (only alphabetic)
    char plaintext[MAX_TEXT];
    int len = 0;
    readPlaintext(plainFile, plaintext, &len);
    fclose(plainFile);

    // pad with x's if needed (so length is multiple of n)
    while (len % n != 0) {
        plaintext[len++] = 'x';
    }
    plaintext[len] = '\0';   // make sure it's terminated

    char ciphertext[MAX_TEXT];

    // encrypt
    hillEncrypt(key, n, plaintext, ciphertext, len);

    // output results
    printf("\nKey matrix:\n");
    printMatrix(key, n);

    printf("\nPlaintext:\n");
    printText(plaintext, len);

    printf("\nCiphertext:\n");
    printText(ciphertext, len);

    return EXIT_SUCCESS;
}

// reads the key matrix from file
void readKeyMatrix(FILE *keyFile, int key[MAX_KEY_SIZE][MAX_KEY_SIZE], int *n) {
    fscanf(keyFile, "%d", n);    // first number = size of matrix

    // read all values row by row
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            fscanf(keyFile, "%d", &key[i][j]);
        }
    }
}

// reads plaintext and keeps only letters
void readPlaintext(FILE *plainFile, char plaintext[MAX_TEXT], int *len) {
    char ch;
    *len = 0;

    while ((ch = fgetc(plainFile)) != EOF) {
        if (isalpha(ch)) {
            plaintext[(*len)++] = tolower(ch);   // convert everything to lowercase
        }
    }

    // terminate later in main
}

// does the Hill cipher encryption
// multiplies key matrix with plaintext blocks (mod 26)
void hillEncrypt(int key[MAX_KEY_SIZE][MAX_KEY_SIZE], int n, char plaintext[MAX_TEXT], char ciphertext[MAX_TEXT], int len) {

    // go through the text n chars at a time
    for (int i = 0; i < len; i += n) {
        for (int row = 0; row < n; row++) {
            int sum = 0;
            // multiply row * column values
            for (int col = 0; col < n; col++) {
                sum += key[row][col] * charToIndex(plaintext[i + col]);
            }

            // wrap around alphabet
            ciphertext[i + row] = indexToChar(sum % 26);
            
        }
    }
}

// prints the key matrix
void printMatrix(int key[MAX_KEY_SIZE][MAX_KEY_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", key[i][j]);
        }
        printf("\n");
    }
}

// prints text 80 chars per line
void printText(char text[MAX_TEXT], int len) {
    for (int i = 0; i < len; i++) {
        putchar(text[i]);

        // new line every 80 chars
        if ((i + 1) % LINE_LENGTH == 0)
            putchar('\n');
    }

    if (len % LINE_LENGTH != 0)
        putchar('\n');
}

/*=============================================================================
| I Aidan Pinkney (5557994) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/