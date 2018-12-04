/*
*   Author: Justin Muskopf
*   Instructor: Mark Hoffman
*   Course: CSCE 4550, Fall 2018
*   Assignment: Project 2
*/
#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "cipherengine.h"

//
// brief: Sets the output file
//
// param: file - Reference to the output file
//
void CipherEngine::setOutputFile(File &file)
{
    outFile = file;
}

//
// brief: Calls helper functions to ecrypt plaintext using key
//
// param: _plaintext - The plaintext to encrypt
// param: _key - The key to use
//
void CipherEngine::encrypt(std::string _plaintext, std::string _key)
{
    // Set plaintext and key
    plaintext = _plaintext;
    key = _key;

    removeWhiteSpaceFromPlainText();

    applyCipher();

    padCipheredText();

    shiftPaddedText();

    calculateParity();

    printAndWriteLine("MixColumns:\n" + mixedColumns);

}

//
// brief: Removes whitespace from the plaintext provided previously
//
void CipherEngine::removeWhiteSpaceFromPlainText()
{
    // The whitespace characters to remove
    const char ToRemove[] = {' ', '\t', '\n'};
    const size_t RemoveArrSize = (sizeof(ToRemove) / sizeof(char));

    // The range of ascii values to ELIMINATE
    int punctMin = 32, punctMax = 64;

    // Erase whitetext
    for (int i = 0; i < RemoveArrSize; i++)
    {
        plaintext.erase(std::remove(plaintext.begin(), 
                                    plaintext.end(), 
                                    ToRemove[i]), 
                                    plaintext.end());
    }

    // Erase punctuation
    for (int i = punctMin; i < punctMax; i++)
    {
        plaintext.erase(std::remove(plaintext.begin(), 
                                    plaintext.end(), 
                                    (char)i), 
                                    plaintext.end());
    }

    printAndWriteLine("Preprocessing:\n" + plaintext);
}

//
// brief: Applies the vigenere cipher to the plaintext using key
//
void CipherEngine::applyCipher()
{
    // The lengths of the plaintext and key
    int ptLen = plaintext.length();
    int keyLen = key.length();

    int  keyVal;        // The numerical value of keyChar in the alphabet
    int  incCharValue;  // The incremented character value
    int  toIncreaseBy;  // The amount to increase the OOB char by
    char ch;            // The current character
    char keyChar;       // The current key character
    char finalChar;     // The character value of the encrypted char

    // Clear ciphered string
    ciphered = "";

    // Loop through plaintext
    for (int i = 0; i < ptLen; i++)
    {
        ch = plaintext[i];
        keyChar = key[i % keyLen];
        keyVal = keyChar % UPPER_MOD;
        incCharValue = ch + keyVal;

        // Out of bounds
        if (incCharValue > 'Z')
        {
            toIncreaseBy = incCharValue - 'Z' - 1;
            incCharValue = UPPER_MOD + toIncreaseBy;
        }
    
        finalChar = (char)incCharValue;

        ciphered += finalChar;
    }

    printAndWriteLine("Substitution:\n" + ciphered);
}

//
// brief: Pads the ciphered text
// 
void CipherEngine::padCipheredText()
{
    // Clear padded string
    padded = "";

    // Pad with A characters until len % 16 = 0
    while (ciphered.length() % 16)
    {
        ciphered += 'A';
    }

    int cipherLength = ciphered.length(); // Current length of cipher
    int charIdx      = 0;                 // Current character index
    int numLines     = 1;                 // Number of lines
    

    // Loop through the cipher to format padded string
    while (charIdx < cipherLength)
    {
        padded += ciphered[charIdx++];
        
        // New row
        if (charIdx % NUM_COLS == 0)
        {
            padded += "\n";

            // New block
            if (numLines++ % NUM_ROWS == 0)
            {
                padded += "\n";
            }
        }
    }

    // The number of lines after pad
    numPaddedLines = numLines - 1;

    printAndWriteLine("Padding:\n" + padded);
}

//
// brief: Prints a line to stdout and writes to outFile
//
void CipherEngine::printAndWriteLine(std::string line)
{
    line += "\n\n";

    std::cout << line;
    outFile.write(line);
}

//
// brief: Shifts text after padding
//
void CipherEngine::shiftPaddedText()
{

    int lineMod;    // The modulus value of the current line
    int blockCount; // The number of cipher blocks
    int charLow;    // The first index of this row
    int shiftIdx;   // The index to begin shift at
    int charIdx;    // The current character's index
    int charVal;    // Character representation of padded int

    // Init shifted to the first row and put in vector
    shifted = padded.substr(0, 5);
    for (int i = 0; i < NUM_COLS; i++)
    {
        charVal = (int)padded[i];
        parityNumbers.push_back(charVal);
    }

    // Loop through number of padded lines and shift
    blockCount = 0; 
    for (int lineNum = 1; lineNum < numPaddedLines; lineNum++)
    {
        
        charLow = (lineNum * NUM_COLS) + lineNum + blockCount;

        // New block
        lineMod = lineNum % NUM_ROWS;
        if (lineMod == 0)
        {
            shifted += "\n";
            charLow++;
            blockCount++;
        }
        
        // Get the current row's chars
        std::string row = padded.substr(charLow, NUM_COLS);
        parityChars += row;

        shiftIdx = lineMod;
    
        // Loop through row and shift columns
        for (int i = 0; i < NUM_COLS; i++)
        {
            
            charIdx = (shiftIdx + i) % NUM_COLS;

            // Char and its ASCII value
            char ch = row[charIdx];
            charVal = (int)ch;

            // Append to shifted
            shifted += ch;
            
            parityNumbers.push_back(charVal);
        }

        shifted += "\n";
    }

    printAndWriteLine("ShiftRows:\n" + shifted);
}

//
// brief: Performs Rijndael's Galois Field multiplication
//
// param: mul - Number to multiply
//
int CipherEngine::rgfMul(int mul)
{
    int by2 = mul << 1; // Multiply by 2
    int msb = mul >> 7; // Extract MSB

    // Flip MSB if necessary
    int msbFlip = (msb & 1) * 0x1b;

    int ret = by2 ^ msbFlip;

    return ret;

}

//
// brief: Performs column mixing on col
//
// param: col - The column to mix
//
void CipherEngine::mixColumns(CipherColumn &col)
{

    int mul; // rgfMultiplied value

    // All values XORd
    int allXOR = col[0] ^ col[1] ^ col[2] ^ col[3];

    int temp = col[0]; // Placeholder for first value

    // Loop through columns
    for (int i = 0; i < NUM_COLS; i++)
    {
        // Calculate rgfMultiplied
        mul = (i != (NUM_COLS - 1)) ? rgfMul(col[i] ^ col[i + 1]) : rgfMul(col[i] ^ temp);
        
        // XOR
        col[i] ^= mul ^ allXOR;
    }
}

//
// brief: Call helper functions to mix columns of block
//
// param: block - The cipher block whose columns are to be mixed
//
void CipherEngine::mixColumnsOfCipherBlock(CipherBlock block)
{                 
    // String to print
    std::stringstream toPrint;
    
    // Mix all columns of this block
    for (int col = 0; col < NUM_COLS; col++)
    {
        mixColumns(block[col]);
    }

    // Reverse rows and columns, store string
    for (int rowNum = 0; rowNum < NUM_ROWS; rowNum++)
    {
        for (int colNum = 0; colNum < NUM_COLS; colNum++)
        {
            int num = block[colNum][rowNum];

            // Add string representation to toPrint
            toPrint << std::hex << std::setw(2) << num << " ";

        }
        toPrint << "\n";
    }

    mixedColumns.append(toPrint.str());
}


//
// brief: Calculates the parity of an 8bit integer,
//        and flips its MSB when parity is odd
//
void CipherEngine::calculateParity()
{
    // Stores the string representation of a hex number
    std::stringstream hexNumbers;

    int thisNum;     // The current number in parityNumbers
    int pushNum;     // The number to push to hexNumbers
    int colNum  = 0; // Current column number
    int rowNum  = 0; // Current row number
    int numOnes = 0; // The number of ones in current binary rep

    CipherBlock block; // Current cipher block

    // Loop through the numbers found in previous step
    for (int i = 0; i < parityNumbers.size(); i++)
    {
        // The number of ones in this binary
        numOnes = 0;

        // Calc current column number
        colNum = i % NUM_COLS;

        // Count number of ones using bitwise arithmetic
        thisNum = parityNumbers[i];
        pushNum = thisNum;
        while (thisNum)
        {
            numOnes += thisNum & 1;
            thisNum >>= 1;
        }

        // The number of ones is odd, flip MSB
        if (numOnes % 2)
        {
            pushNum |= 0b10000000;
        }

        // Overwrite parityNumber value, and set block value
        parityNumbers[i] = pushNum;
        block[colNum][rowNum] = pushNum;

        // Add string representation to hexNumbers
        hexNumbers << std::hex << std::setw(2) << pushNum << " ";

        // Row needs to be incremented to stay in bounds
        if ((i + 1) % NUM_COLS == 0)
        {
            hexNumbers << "\n";

            // Increment row and error check
            rowNum++;
            if (rowNum >= NUM_ROWS)
            {
                mixColumnsOfCipherBlock(block);
                rowNum = 0;
            }
        }
    }

    parityChars = hexNumbers.str();

    printAndWriteLine("Parity Bit:\n" + parityChars);
}

