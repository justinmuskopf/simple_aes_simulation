/*
*   Author: Justin Muskopf
*   Instructor: Mark Hoffman
*   Course: CSCE 4550, Fall 2018
*   Assignment: Project 2
*/
#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <string>

#define FILE_SUCCESS 0
#define FILE_FAILED_TO_CLOSE 1
#define FILE_FAILED_TO_OPEN  2

enum FileOpenMode {READ, WRITE};

class File
{
    public:
        File();
        File(std::string fileType);
        std::string getContents();
        int open(std::string filename, FileOpenMode readWrite = READ);
        int openFromInput(FileOpenMode readWrite = READ);
        int close();
        void setFileType(std::string fileType);
        bool isOpen();
        int write(std::string buffer);
    private:
        // Max line length of file
        static const int MAX_LINE_LEN = 80;

        FILE *file;       // The file pointer
        std::string type; // The type of the file
};

#endif
