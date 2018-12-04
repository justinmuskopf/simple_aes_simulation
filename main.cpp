/*
*   Author: Justin Muskopf
*   Instructor: Mark Hoffman
*   Course: CSCE 4550, Fall 2018
*   Assignment: Project 2
*/
#include <iostream>
#include "file.h"
#include "cipherengine.h"

int main()
{
    std::string plaintext; // The plaintext
    std::string key;       // The key

    File plainTextFile("plaintext");// The plaintext file
    File keyFile("key");            // The key file
    File outputFile("output");      // The output file

    CipherEngine cipherEngine;      // The cipher engine for encryption

    // Initialize the plaintext and key files
    plainTextFile.openFromInput();
    keyFile.openFromInput();
    outputFile.openFromInput(WRITE);

    // Get the plaintext and key from files 
    plaintext = plainTextFile.getContents();
    key = keyFile.getContents();

    // Close plaintext and key files
    plainTextFile.close();
    keyFile.close();

    // Set the output file
    cipherEngine.setOutputFile(outputFile);

    // Encrypt plaintext using key
    cipherEngine.encrypt(plaintext, key);

    // Close output file
    outputFile.close();

    return 0;
}

