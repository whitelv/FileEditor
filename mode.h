//
//  mode.h
//  Learning Winter 2025
//
//  Created by Любомир Витвицький on 29.04.2025.
//

#include <fcntl.h>
#include <unistd.h>
#include <iostream>


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

int convertMode(Mode m){
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
    
    
    for (auto mode : unixModes) {
        if (int(mode.mode & m) != 0) {
            flags |= mode.flag;
        }
    }
    
    
    return flags;
}
