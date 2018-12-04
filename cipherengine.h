/*
*   Author: Justin Muskopf
*   Instructor: Mark Hoffman
*   Course: CSCE 4550, Fall 2018
*   Assignment: Project 2
*/
#ifndef CIPHERENGINE_H
#define CIPHERENGINE_H

#include <stdint.h>
#include <string>
#include <vector>
#include "file.h"

class CipherEngine
{
    public:
        void setOutputFile(File &file);
        void encrypt(std::string _plaintext, std::string _key);
    private:
        static const int UPPER_MOD = 65;
        static const int NUM_ROWS = 4;
        static const int NUM_COLS = 4;

        std::string plaintext;   // The plaintext
        std::string key;         // The key
        std::string ciphered;    // Ciphered text
        std::string padded;      // Padded text
        std::string shifted;     // Shifted text
        std::string parityChars; // The parity string
        std::string mixedColumns;// Mixed columns string
        File outFile;            // Output file

        int numPaddedLines; // Number of padded lines

        // Define CipherBlock types
        typedef unsigned char CipherBlock[NUM_COLS][NUM_ROWS];
        typedef unsigned char CipherRow[NUM_COLS];
        typedef unsigned char CipherColumn[NUM_ROWS];

        // The vector of parity numbers
        std::vector<int> parityNumbers;

        void printAndWriteLine(std::string line);
        void removeWhiteSpaceFromPlainText();
        void applyCipher();
        void padCipheredText();
        void shiftPaddedText();
        void calculateParity();
        void mixColumns(CipherColumn &col);
        void mixColumnsOfCipherBlock(CipherBlock block);
        
        int rgfMul(int mul);
};

#endif
