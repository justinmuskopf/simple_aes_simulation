/*
*   Author: Justin Muskopf
*   Instructor: Mark Hoffman
*   Course: CSCE 4550, Fall 2018
*   Assignment: Project 2
*/
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include "file.h"

//
// brief: Default constructor
//
File::File()
{

}

//
// brief: Initializing constructor
//
File::File(std::string fileType)
{
    type = fileType;
}

//
// brief: Open the file of filename with openMode readWrite
//
// param: filename - The filename of the file
// param: readWrite - The open mode of the file
//
int File::open(std::string filename, FileOpenMode readWrite)
{
    // The return code
    int ret;

    // Open file in proper mode
    switch(readWrite)
    {
        case READ:
            file = fopen(filename.c_str(), "r"); 
            break;
        case WRITE:
            file = fopen(filename.c_str(), "w+"); 
            break;
        default:
            return FILE_FAILED_TO_OPEN;
    }

    // File was opened
    if (file)
    {
        ret = FILE_SUCCESS;
    }
    // Error
    else
    {
        ret = FILE_FAILED_TO_OPEN;
    }

    return ret;
}

//
// brief: Opens a file from stdin
//
// param: readWrite - The mode to open the file in
//
int File::openFromInput(FileOpenMode readWrite)
{
    // String to store filename
    std::string filename;

    // Prompt for filename
    std::cout << "Please enter the name of the " << type << " file: ";
    std::cin >> filename;
    open(filename, readWrite);

    // Loop until a file is opened
    while (!file)
    {
        std::cout << "Unable to open '" << filename << "'. Please try again: ";
        std::cin >> filename;

        open(filename, readWrite);
    }
}

//
// brief: Sets the file's type
//
// param: fileType - the file's type
//
void File::setFileType(std::string fileType)
{
    type = fileType;
}


//
// brief: Close the file
//
int File::close()
{
    int ret = FILE_FAILED_TO_CLOSE;

    if (!file)
    {
        return FILE_SUCCESS;
    }

    fclose(file);

    if (!file)
    {
        ret = FILE_SUCCESS;
    }

    return ret;
}

//
// brief: Return a string of the file's contents
//
std::string File::getContents()
{
    
    std::string contents;        // The string to hold contents
    char line[MAX_LINE_LEN + 1]; // The c-string temp to hold interim

    // While the file has more contents, continue to read
    while (fgets(line, MAX_LINE_LEN, file))
    {
        // Terminate line if it came through with a newline
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        contents.append(line);
    }

    

    return contents;
}

//
// brief: Returns whether or not a file is open
//
bool File::isOpen()
{
    return file == NULL;
}

//
// brief: Writes buffer to the file
//
// param: buffer - the buffer to write to the file
//
int File::write(std::string buffer)
{
    return fputs(buffer.c_str(), file);
}
