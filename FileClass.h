//
//  FileClass.hpp
//  Learning Winter 2025
//
//  Created by Любомир Витвицький on 16.04.2025.
//

#ifndef FileClass_hpp
#define FileClass_hpp

#include "mode.h"










class File{
public:
    bool isOpen();
    void seek(size_t pos);
    size_t tell();

    void clear();
    void resetEOF();
    
    bool operator>>(char * buffer);
    bool operator>>(std::string & str);
    
    File &  operator<<(const char * str);
    
    bool readline(char * buffer, size_t size);
    void writeline(const char * buffer);
    
    size_t readBytes(char *buffer,size_t size);
    void writeBytes(char *buffer,size_t size);
    
    bool closeFile();
    
    
public:
    File(const char * filename, Mode flags = Mode::READ);
    

    
    
    
private:
    File & operator=(File & f);
    File(File & f);
    size_t seekRead(size_t pos);
    size_t seekWrite(size_t pos);

    
private:
    int fileDescriptor;
    size_t writePos;
    size_t readPos;
    bool eof;
    


    
    
  
};





#endif 
