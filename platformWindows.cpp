#include "mode.h"
#include "FileClass.h"

#ifdef OS_WINDOWS

bool File::isOpen(){
    if (fileDescriptor == INVALID_HANDLE_VALUE) {
        return 0;
    }
    return 1;
}

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


bool File::closeFile(){
    return CloseHandle(fileDescriptor);
}

Filetype File::createFileDescriptor(const char * filename, Mode mode){
    unsigned long flags = convertMode(mode);
    unsigned long dispos = convertCreationDisposition(mode);
    return CreateFileA(filename, flags, 0, NULL, dispos, FILE_ATTRIBUTE_NORMAL, NULL);
}


bytesType File::readF(size_t size, char * buffer){
    bytesType bytesRead{};
    if(!ReadFile(fileDescriptor, &buffer, size, &bytesRead, NULL)){
        std::cerr << "Error: Problem with reading file\n";
        throw std::invalid_argument("Error: Problem with reading file.\n");
        return 0;
    }
}

bytesType File::writeF(size_t size,const char * buffer){
    bytesType bytesWrite{};
    if (!WriteFile(fileDescriptor, buffer, size, &bytesWrite, NULL)){
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file.\n");
    }
    return bytesWrite;

}

#endif
