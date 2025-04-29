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

const int BUFFER_SIZE = 2;

bool File::isOpen(){
    if (fileDescriptor == -1) {
        return 0;
    }
    return 1;
}



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
    
    
    int flags = convertMode(mode, 0);
    
    fileDescriptor = open(filename, flags, S_IRWXU);
    
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
    if (buffer == nullptr) {
        std::cerr << "Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Buffer cannot be nullptr\n");
    }
    seek(readPos);
    
    char BUFFER[BUFFER_SIZE];
    std::memset(BUFFER, 0, BUFFER_SIZE);
    std::strcpy(buffer, "");
    
    long bytesRead{};
    int i = 0;
    char ch;
    
    if (eof) {
        return 0;
    }
    
    while (true) {
        bytesRead = read(fileDescriptor, &ch, 1);
        
        if (bytesRead == -1){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }
        
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
   
    seek(readPos);
    
    char BUFFER[BUFFER_SIZE];
    std::memset(BUFFER, 0, BUFFER_SIZE);
    str = "";
    
    long bytesRead{};
    int i = 0;
    char ch;
    
    if (eof) {
        return 0;
    }
    
    while (true) {
        bytesRead = read(fileDescriptor, &ch, 1);
        
        if (bytesRead == -1){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }
        
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
    seek(writePos);
    long bytesWrite = write(fileDescriptor, str, std::strlen(str));
    if (bytesWrite == -1) {
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file.\n");
    }
    writePos = tell();
    return *this;
}

bool File::readline(char * buffer, size_t size){
    if (buffer == nullptr) {
        std::cerr << "Error: Buffer cannot be nullptr\n";
        throw std::invalid_argument("Error: Buffer cannot be nullptr\n");
    }
    seek(readPos);
    
    char BUFFER[BUFFER_SIZE];
    std::memset(BUFFER, 0, BUFFER_SIZE);
    std::strcpy(buffer, "");
    
    long bytesRead{};
    int i = 0;
    char ch;
    
    if (eof) {
        return 0;
    }
    
    while (true) {
        bytesRead = read(fileDescriptor, &ch, 1);
        
        if (bytesRead == -1){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }
        
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



int main(){
    File f = File("/Users/lubomirvitvickij/Desktop/Learning Winter 2025/Learning Winter 2025/FiliEditor1/file.txt", Mode::REWR | Mode::TRUNC);
    
    
    
    f << "Lorem\n ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque a eleifend quam.\n Fusce dictum nisi sit amet odio egestas dignissim.\nAenean dignissim id diam sed sagittis. Integer eu hendrerit urna, et rhoncus ligula. Fusce venenatis ligula mi, quis fermentum risus consequat at.\n Donec pretium orci urna, at fringilla lacus rutrum vitae. Proin quis leo mi. Etiam id faucibus metus, nec luctus massa. Etiam ornare magna nunc, nec porta ligula porttitor a. Suspendisse potenti.\n";
    
    f.writeline("HELLO");
    f.writeline("HELLO");
    
    char buffer[100];
    
    std::string str;
    

//
//    while (f >> str) {
//        std::cout << str << " ";
//    }

    while (f.readline(buffer, 100)) {
        std::cout << buffer << std::endl;
    }

    
//
//    f.readline(buffer);
//    std::cout << buffer << std::endl;
//
//    f.readline(buffer);
//
//    std::cout << buffer << std::endl;

    
    std::cout << std::strlen("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque a eleifend quam. Fusce dictum nisi sit amet odio egestas dignissim.") << std::endl;
   

    
//
//    std::cout << std::bitset<16>(O_WRONLY | O_APPEND | O_CREAT) << std::endl;
//    std::cout << std::bitset<16>(convertMode(Mode::WRITE | Mode::APPEND | Mode::CREATE, 0)) << std::endl;
    
    
   


}

