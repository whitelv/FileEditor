//
//  FileClass.cpp
//  Learning Winter 2025
//
//  Created by Любомир Витвицький on 16.04.2025.
//

#include "FileClass.h"
#include <iostream>
#include <cstring>


const int BUFFER_SIZE = 2;




File::File(const char * filename, Mode mode) : readPos(0), writePos(0), eof(0){
        
    if (filename == nullptr) {
        std::cerr << "Error: You must enter a filename.\n";
        throw std::invalid_argument("Error: You must enter a filename.\n");
        
    }
    
    if (!isValidMode(mode)) {
        std::cerr << "Error: You must enter a valid mode.\n";
        throw std::invalid_argument("Error: You must enter a valid mode.\n");
    }
    
    fileDescriptor = createFileDescriptor(filename, mode);

    if (!isOpen()) {
        std::cerr << "Error: Problem with opening file\n";
        throw std::invalid_argument("Error: Problem with opening file.\n");
    }
    
}
void File::seek(size_t pos){
    seekRead(pos);
    seekWrite(pos);
}






bool File::operator>>(char * buffer){
    if (buffer == nullptr) {
        std::cerr << "Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Buffer cannot be nullptr\n");
    }
    seekRead(readPos);
    
    char BUFFER[BUFFER_SIZE];
    std::memset(BUFFER, 0, BUFFER_SIZE);
    std::strcpy(buffer, "");
    
    bytesType bytesRead{};
    int i = 0;
    char ch;
    
    if (eof) {
        return false;
    }
    
    while (true) {

        bytesRead = readF(1, &ch);

        
        if (bytesRead == 0) {
            eof = 1;
            break;
        }
        
        if (ch == ' ' || ch == '\n' ) {
            break;
        }
        
        BUFFER[i++] = ch;
    
        if (i >= BUFFER_SIZE - 1) {
            std::strcat(buffer, BUFFER);
            std::memset(BUFFER, 0, BUFFER_SIZE);
            i = 0;
        }
    }
    
    if (i > 0) {
            BUFFER[i] = '\0';
            std::strcat(buffer, BUFFER);
    }
    
    
    readPos = tell();
    
   
    return true;
}




bool File::operator>>(std::string & str){
   
    seekRead(readPos);
    
    char BUFFER[BUFFER_SIZE];
    std::memset(BUFFER, 0, BUFFER_SIZE);
    str = "";
    
    bytesType bytesRead{};
    int i = 0;
    char ch;
    
    if (eof) {
        return false;
    }
    
    while (true) {

        bytesRead = readF(1, &ch);

        
        if (bytesRead == 0) {
            eof = 1;
            break;
        }
        
        if (ch == ' ' || ch == '\n' ) {
            break;
        }
        
        BUFFER[i++] = ch;
    
        if (i >= BUFFER_SIZE - 1) {
            str += BUFFER;
            std::memset(BUFFER, 0, BUFFER_SIZE);
            i = 0;
        }
    }
    
    if (i > 0) {
            BUFFER[i] = '\0';
            str += BUFFER;
    }
    
    
    readPos = tell();
    
    return true;
}


File & File::operator<<(const char * str){
    if (str == nullptr) {
        std::cerr << "Error: Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Error: Buffer cannot be nullptr\n");
    }
    seekWrite(writePos);


    bytesType bytesWrite{};


    bytesWrite = writeF(std::strlen(str), str);


    writePos = tell();
    return *this;
}



bool File::readline(char * buffer, size_t size){
    if (buffer == nullptr) {
        std::cerr << "Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Buffer cannot be nullptr\n");
    }
    seekRead(readPos);
    
    char BUFFER[BUFFER_SIZE];
    std::memset(BUFFER, 0, BUFFER_SIZE);
    std::memset(buffer, 0, size);

    bytesType bytesRead{};
    int i = 0;
    char ch;
    
    if (eof) {
        return false;
    }
    
    while (true) {

        bytesRead = readF(1, &ch);

        if (bytesRead == 0) {
            eof = 1;
            break;
        }
        
        if (ch == '\n') {
            break;
        }
        
        BUFFER[i++] = ch;
    
        if (i >= BUFFER_SIZE - 1) {
            std::strcat(buffer, BUFFER);
            std::memset(BUFFER, 0, BUFFER_SIZE);
            i = 0;
        }
        
        if (std::strlen(buffer) + std::strlen(BUFFER) >= size - 1) {
            break;
        }
    }
    
    if (i > 0) {
            BUFFER[i] = '\0';
            std::strcat(buffer, BUFFER);
    }
    
    
    readPos = tell();
    
   
    return true;
}


void File::writeline(const char * buffer){
    *this << buffer << "\n";
}




size_t File::readBytes(char *buffer, size_t size){
    seekRead(readPos);

    if (buffer == nullptr) {
        std::cerr << "Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Buffer cannot be nullptr\n");
    }
    
    std::memset(buffer, 0, size);

    bytesType bytesRead{};
    
    if (eof) {
        return false;
    }


    bytesRead = readF(size, buffer);


    if (bytesRead == 0) {
        eof = 1;
    }
    
    
    readPos = tell();
    return bytesRead;
}



void File::writeBytes(const char *buffer ,size_t size){
    seekWrite(writePos);
    bytesType bytesWrite{};
    

    if (buffer == nullptr) {
        std::cerr << "Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Buffer cannot be nullptr\n");
    }
    
    if (size > std::strlen(buffer)) {
        std::cerr << "Error: Buffer cannot be smaller than size\n";
        throw std::invalid_argument("Error: Buffer cannot be smaller than size\n");
    }

    bytesWrite = writeF(size, buffer);
    writePos = tell();
}



void File::resetEOF(){
    eof = 0;
    seekRead(0);
}




