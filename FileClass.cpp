//
//  FileClass.cpp
//  Learning Winter 2025
//
//  Created by Любомир Витвицький on 16.04.2025.
//

#include "FileClass.h"
#include <iostream>
#include <cstring>
#include <errno.h>

const int BUFFER_SIZE = 100;

bool File::isOpen(){
    if (fileDescriptor == -1) {
        return 0;
    }
    return 1;
}



File::File(const char * filename, Mode mode) : readPos(0), writePos(0){
    
// SEPARATE FUNCTION VALIDATION?
    
    if (filename == nullptr) {
        std::cerr << "Error: You must enter a filename.\n";
        throw std::invalid_argument("Error: You must enter a filename.\n");
        
    }
    
    if (!isValidMode(mode)) {
        std::cerr << "Error: You must enter a valid mode.\n";
        throw std::invalid_argument("Error: You must enter a valid mode.\n");
    }
    
    
    int flags = convertMode(mode, 0);
    
    fileDescriptor = open(filename, flags);
    
    if (!isOpen()) {
        std::cerr << "Error: Problem with opening file\n";
        throw std::invalid_argument("Error: Problem with opening file.\n");
    }
    
}

size_t File::seek(size_t pos){
    return lseek(fileDescriptor, pos, SEEK_SET);
}

size_t File::tell(){
    return lseek(fileDescriptor, 0, SEEK_CUR);
}

void File::clear(){
    ftruncate(fileDescriptor, 0);
    seek(0);
}

bool File::operator>>(char * buffer){
    seek(readPos);
    
    char ch;
    int i = 0;
    int bytesRead = 0;
    
    bytesRead = (int) read(fileDescriptor, &ch, 1);
    
    if (bytesRead == 0) {
        readPos = 0;
        return 0;
    }
    
    while (bytesRead != 0 && ch != ' ' && ch != '\n' && i < BUFFER_SIZE - 1) {
        buffer[i++] = ch;
        bytesRead = (int) read(fileDescriptor, &ch, 1);
    }
    buffer[i] = '\0';
    readPos = tell();
    
    return 1;
}



File & File::operator<<(char * str){
    seek(writePos);
    write(fileDescriptor, str, std::strlen(str));
    writePos = tell();
    return *this;
}

bool File::readline(char * buffer){
    seek(readPos);
    
    char ch;
    int i = 0;
    int bytesRead = 0;
    
    bytesRead = (int) read(fileDescriptor, &ch, 1);
    
    if (bytesRead == 0) {
        readPos = 0;
        std::cout << "EOF" << std::endl;
        return 0;
    }
    
    while (bytesRead != 0 && ch != '\n' && i < BUFFER_SIZE - 1) {
        buffer[i++] = ch;
        bytesRead = (int) read(fileDescriptor, &ch, 1);
    }
    buffer[i] = '\0';
    readPos = tell();
    
    return 1;
}


int main(){
    File f = File("/Users/lubomirvitvickij/Desktop/Learning Winter 2025/Learning Winter 2025/fileEditor/file.txt", Mode::REWR | Mode::TRUNC);
    
    
    
    
    f << "a" << " b" << " hello\n" << "yohan";
    
    char buffer[BUFFER_SIZE];
//    
//    while (f >> buffer) {
//        std::cout << buffer << " ";
//    }
    
    f.readline(buffer);
    std::cout << buffer << std::endl;

    f.readline(buffer);
    std::cout << buffer << std::endl;

    f.readline(buffer);

    std::cout << buffer << std::endl;

    
    
   

    
//
//    std::cout << std::bitset<16>(O_WRONLY | O_APPEND | O_CREAT) << std::endl;
//    std::cout << std::bitset<16>(convertMode(Mode::WRITE | Mode::APPEND | Mode::CREATE, 0)) << std::endl;
    
    
   


}

