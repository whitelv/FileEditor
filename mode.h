//
//  mode.h
//  Learning Winter 2025
//
//  Created by Любомир Витвицький on 29.04.2025.
//

#ifndef mode_h
#define mode_h

#ifdef __APPLE__
    #include <fcntl.h>
    #include <unistd.h>
#endif

#ifdef _WIN32
    #include <fileapi.h>
    #include <windows.h>
#endif

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
    Mode::APPEND | Mode::READ,
    Mode::TRUNC | Mode::WRITE,
    Mode::TRUNC | Mode::READ,
    Mode::TRUNC | Mode::REWR,

};

inline bool isValidMode(Mode value){
    for (Mode mode : validMode) {
        if(value == mode)
            return true;
    }
    return false;
}

struct ModeFlag {
    Mode mode;
    unsigned long flag;
};



extern ModeFlag Modes[];
unsigned long convertMode(Mode m);
unsigned long convertCreationDisposition(Mode m);


#endif