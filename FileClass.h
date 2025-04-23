//
//  FileClass.hpp
//  Learning Winter 2025
//
//  Created by Любомир Витвицький on 16.04.2025.
//

#ifndef FileClass_hpp
#define FileClass_hpp

#include <iostream>
#include <fcntl.h>
#include <unistd.h>


//unix = 0 win = 1
typedef int opratingSystem;


enum class Mode{
    READ = 0x1,
    WRITE = 0x2,
    REWR = 0x4,
    APPEND =  0x8,
    CREATE = 0x10,
    TRUNC = 0x20

};

constexpr Mode operator|(Mode m1, Mode m2){
    return static_cast<Mode>(int(m1) | int(m2));
}

constexpr Mode operator&(Mode m1, Mode m2){
    return static_cast<Mode>(int(m1) & int(m2));
}

constexpr Mode validMode[] = {
    Mode::READ,
    Mode::WRITE,
    Mode::REWR,
    Mode::APPEND,
    Mode::CREATE,
    Mode::TRUNC,
    Mode::CREATE | Mode::READ,
    Mode::CREATE | Mode::WRITE,
    Mode::CREATE | Mode::REWR,
    Mode::APPEND | Mode::WRITE,
    Mode::APPEND | Mode::REWR,
    Mode::TRUNC | Mode::WRITE,
    Mode::TRUNC | Mode::READ,
    Mode::TRUNC | Mode::REWR,

};

bool isValidMode(Mode value){
    for (Mode mode : validMode) {
        if(value == mode)
            return true;
    }
    return false;
}

int convertMode(Mode m, opratingSystem op){
    int flags = 0;
    
    struct ModeFlag {
            Mode mode;
            int flag;
    };
    
    ModeFlag unixModes[] = {
        {Mode::READ, O_RDONLY},
        {Mode::WRITE, O_WRONLY},
        {Mode::APPEND, O_APPEND},
        {Mode::REWR, O_RDWR},
        {Mode::CREATE, O_CREAT},
        {Mode::TRUNC, O_TRUNC},
    };
    
    if (op == 0) {
        for (auto mode : unixModes) {
            if (int(mode.mode & m) != 0) {
                flags |= mode.flag;
            }
        }
    }
    else if(op == 1){
        return flags;
    }
    
    
    return flags;
}



class File{
public:
    int fileDescriptor;
    size_t writePos;
    size_t readPos;
 
    
private:
    File & operator=(File & f);
    File(File & f);
    
    
    
public:
    File(const char * filename, Mode flags = Mode::READ);

    bool isOpen();
    size_t seek(size_t pos);
    size_t tell();
    void clear();
    
    bool operator>>(char * buffer);
    File &  operator<<(char * str);
    
    bool readline(char * buffer);
};



#endif 
