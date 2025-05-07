#include "mode.h"
#include "FileClass.h"

#ifdef OS_UNIX


ModeFlag Modes[] = {
    {Mode::READ, O_RDONLY},
    {Mode::WRITE, O_WRONLY},
    {Mode::APPEND, O_APPEND | O_WRONLY},
    {Mode::REWR, O_RDWR},
    {Mode::CREATE, O_CREAT},
    {Mode::TRUNC, O_TRUNC},
};

bool File::isOpen(){
    if (fileDescriptor == -1) {
        return 0;
    }
    return 1;
}


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

bool File::closeFile(){
    return !close(fileDescriptor);
}


Filetype File::createFileDescriptor(const char * filename, Mode mode){
    unsigned long flags = convertMode(mode);
    return open(filename, flags, S_IRWXU);

}

bytesType File::readF(size_t size, char * buffer){
    bytesType bytesRead = read(fileDescriptor, buffer, size);
        
        if (bytesRead == -1){
            std::cerr << "Error: Problem with reading file\n";
            throw std::invalid_argument("Error: Problem with reading file.\n");
            return 0;
        }
    return bytesRead;
}


bytesType File::writeF(size_t size,const char * buffer){
    bytesType bytesWrite{};
    bytesWrite = write(fileDescriptor, buffer, size);
    if (bytesWrite == -1) {
        std::cerr << "Error: Problem with writing to file\n";
        throw std::invalid_argument("Error: Problem with writing to file.\n");
    }
    return bytesWrite;
}

#endif
