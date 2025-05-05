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

#ifdef __APPLE__
typedef long long bytesType;
#elif _WIN32
typedef unsigned long bytesType;
#endif

#ifdef __APPLE__
bool File::isOpen(){
    if (fileDescriptor == -1) {
        return 0;
    }
    return 1;
}
#elif _WIN32
bool File::isOpen(){
    if (fileDescriptor == INVALID_HANDLE_VALUE) {
        return 0;
    }
    return 1;
}

#endif



File::File(const char * filename, Mode mode) : readPos(0), writePos(0), eof(0){
    
// SEPARATE FUNCTION VALIDATION?
    
    if (filename == nullptr) {
        std::cerr << "Error: You must enter a filename.\n";
        throw std::invalid_argument("Error: You must enter a filename.\n");
        
    }
    
    if (!isValidMode(mode)) {
        std::cerr << "Error: You must enter a valid mode.\n";
        throw std::invalid_argument("Error: You must enter a valid mode.\n");
    }
    
    
    unsigned long flags = convertMode(mode);
    
    #ifdef __APPLE
        fileDescriptor = open(filename, flags, S_IRWXU);
    #elif _WIN32
        unsigned long dispos = convertCreationDisposition(mode);
        fileDescriptor = CreateFileA(filename, flags, 0, NULL, dispos, FILE_ATTRIBUTE_NORMAL, NULL);
    #endif
    
    if (!isOpen()) {
        std::cerr << "Error: Problem with opening file\n";
        throw std::invalid_argument("Error: Problem with opening file.\n");
    }
    
}
void File::seek(size_t pos){
    seekRead(pos);
    seekWrite(pos);
}

#ifdef __APPLE__
void File::seekRead(size_t pos){
    readPos = pos;
    if(lseek(fileDescriptor, readPos, SEEK_SET) == -1){
        std::cerr << "Error: Problem with reading file\n";
        throw std::invalid_argument("Error: Problem with reading file\n");
    }
}


void File::seekWrite(size_t pos){
    writePos = pos;
    if(lseek(fileDescriptor, writePos, SEEK_SET) == -1){
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file\n");
    }
}

size_t File::tell(){
    size_t current_pos = lseek(fileDescriptor, 0, SEEK_CUR);

    if(current_pos == -1){
        std::cerr << "Error: Problem with cursor position\n";
        throw std::invalid_argument("Error: Problem with cursor postion\n");
    }

    return current_pos;

}
void File::clear(){
    if (ftruncate(fileDescriptor, 0) == -1){
        std::cerr << "Error: Problem with clearing file\n";
        throw std::invalid_argument("Error: Problem with clearing file\n");
    }
    seek(0);
}


#elif _WIN32
void File::seekRead(size_t pos){
    readPos = pos;
    LARGE_INTEGER li;
    li.QuadPart = readPos;
    if(!SetFilePointerEx(fileDescriptor, li, NULL, FILE_BEGIN)){
        std::cerr << "Error: Problem with reading file\n";
        throw std::invalid_argument("Error: Problem with reading file\n");
    }
}


void File::seekWrite(size_t pos){
    writePos = pos;
    LARGE_INTEGER li;
    li.QuadPart = writePos;
    if(!SetFilePointerEx(fileDescriptor, li, NULL, FILE_BEGIN)){
        std::cerr << "Error: Problem with reading file\n";
        throw std::invalid_argument("Error: Problem with reading file\n");
    }
}

size_t File::tell(){
    LARGE_INTEGER li;
    li.QuadPart = 0;
    LARGE_INTEGER current_pos;
    if(!SetFilePointerEx(fileDescriptor, li, &current_pos, FILE_CURRENT)){
        std::cerr << "Error: Problem with cursor position\n";
        throw std::invalid_argument("Error: Problem with cursor postion\n");
    }
    return size_t (current_pos.QuadPart);
    
}

void File::clear(){
    seek(0);
    if(!SetEndOfFile(fileDescriptor)){
        std::cerr << "Error: Problem with clearing file\n";
        throw std::invalid_argument("Error: Problem with clearing file\n");
    }
}



#endif





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
        return 0;
    }
    
    while (true) {

        #ifdef __APPLE__
        bytesRead = read(fileDescriptor, &ch, 1);
        
        if (bytesRead == -1){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }
        #elif _WIN32

        if(!ReadFile(fileDescriptor, &ch, 1, &bytesRead, NULL)){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }

        #endif
        
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
    
   
    return 1;
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
        return 0;
    }
    
    while (true) {
        #ifdef __APPLE__
        bytesRead = read(fileDescriptor, &ch, 1);
        
        if (bytesRead == -1){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }
        #elif _WIN32

        if(!ReadFile(fileDescriptor, &ch, 1, &bytesRead, NULL)){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }

        #endif
        
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
    
    return 1;
}


File & File::operator<<(const char * str){
    if (str == nullptr) {
        std::cerr << "Error: Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Error: Buffer cannot be nullptr\n");
    }
    seekWrite(writePos);
    bytesType bytesWrite{};


    #ifdef __APPLE__

    bytesWrite = write(fileDescriptor, str, std::strlen(str));
    if (bytesWrite == -1) {
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file.\n");
    }

    #elif _WIN32
    if (!WriteFile(fileDescriptor, str, std::strlen(str), &bytesWrite, NULL)){
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file.\n");
    }

    #endif


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
        return 0;
    }
    
    while (true) {


        #ifdef __APPLE__
        bytesRead = read(fileDescriptor, &ch, 1);
        
        if (bytesRead == -1){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }
        #elif _WIN32

        if(!ReadFile(fileDescriptor, &ch, 1, &bytesRead, NULL)){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }

        #endif


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
    
   
    return 1;
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
        return 0;
    }
    
    #ifdef __APPLE__
    
    bytesRead = read(fileDescriptor, buffer, size);
    if (bytesRead == -1) {
        std::cerr << "Error: Problem with reading file\n";
        throw std::invalid_argument("Error: Problem with reading file.\n");
    }

    #elif _WIN32
    if(!ReadFile(fileDescriptor, buffer, size, &bytesRead, NULL)){
        std::cerr << "Error: Problem with reading file\n";
        throw std::invalid_argument("Error: Problem with reading file.\n");
        return 0;
    }

    #endif


    if (bytesRead == 0) {
        eof = 1;
    }
    
    
    readPos = tell();
    return bytesRead;
}





void File::writeBytes(const char *buffer,size_t size){
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
    #ifdef __APPLE__
    bytesWrite = write(fileDescriptor, buffer, size);
    
    if (bytesWrite == -1) {
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file.\n");
    }
    #elif _WIN32
    if (!WriteFile(fileDescriptor, buffer, size, &bytesWrite, NULL)){
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file.\n");
    }

    #endif
    writePos = tell();
}



void File::resetEOF(){
    eof = 0;
    seekRead(0);
}
#ifdef __APPLE__


bool File::closeFile(){
    return !close(fileDescriptor);
}

#elif _WIN32
bool File::closeFile(){
    return CloseHandle(fileDescriptor);
}

#endif



