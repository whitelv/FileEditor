#include "mode.h"


#ifdef OS_UNIX
ModeFlag Modes[] = {
    {Mode::READ, O_RDONLY},
    {Mode::WRITE, O_WRONLY},
    {Mode::APPEND, O_APPEND | O_WRONLY},
    {Mode::REWR, O_RDWR},
    {Mode::CREATE, O_CREAT},
    {Mode::TRUNC, O_TRUNC},
};
#elif OS_WINDOWS
ModeFlag Modes[] = {
    {Mode::READ, GENERIC_READ},
    {Mode::WRITE, GENERIC_WRITE},
    {Mode::APPEND, FILE_APPEND_DATA},
    {Mode::REWR, GENERIC_READ | GENERIC_WRITE},
};
#endif



unsigned long convertMode(Mode m){
    unsigned long flags = 0;
    
    for (auto mode : Modes) {
        if ((unsigned long)(mode.mode & m) != 0) {
            flags |= mode.flag;
        }
    }
    
    
    return flags;
}



#ifdef OS_WINDOWS

unsigned long convertCreationDisposition(Mode m){
    unsigned long dispos = 0;

    if(int(Mode::CREATE & m) != 0){
        dispos |= CREATE_ALWAYS;
    }else if(int(Mode::TRUNC & m) != 0){
        dispos |= TRUNCATE_EXISTING;
    }else{
        dispos |= OPEN_EXISTING;
    }

    
    return dispos;

}


#endif