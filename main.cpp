#include <iostream>
#include <cstring>
#include "FileClass.h"



int main(){
    File f = File("/Users/lubomirvitvickij/Desktop/Learning Winter 2025/Learning Winter 2025/FiliEditor1/file1.txt", Mode::CREATE | Mode::REWR);
    std::cout << "Project works" << std::endl;
    
    

     const char * str = "LOREM ipsum i go to school every morning";
      f.writeBytes(str, 10);

     f << str << "\nhere" << "\ntoto";

     f.seek(30);

     f << "\nhere" << "\ntoto";

     f.writeline("here we are");
     f << "today";

     std::cout << f.tell() << std::endl;


    char buffer[100];
    std::memset(buffer, 0, 100);

    
     f.seek(0);

    f.readBytes(buffer, 17);
    std::cout << buffer << std::endl;


    while(f.readline(buffer, 99)){
        std::cout << buffer << std::endl;
    }



    std::cout << f.tell() << std::endl;




    // f << "Lorem\n ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque a eleifend quam.\n Fusce dictum nisi sit amet odio egestas dignissim.\nAenean dignissim id diam sed sagittis. Integer eu hendrerit urna, et rhoncus ligula. Fusce venenatis ligula mi, quis fermentum risus consequat at.\n Donec pretium orci urna, at fringilla lacus rutrum vitae. Proin quis leo mi. Etiam id faucibus metus, nec luctus massa. Etiam ornare magna nunc, nec porta ligula porttitor a. Suspendisse potenti.\n" << "here\n";
    
    // f.seek(30);
    
    // f.writeline("HELLO");
    // f.writeline("HELLO");
    
    // f.writeBytes("HELOOO", 6);
    
    // char buffer[100];
    
    
    // std::string str;
    

//
//    while (f >> str) {
//        std::cout << str << " ";
//    }
    
    
    // while (f.readline(buffer, 100)) {
    //     std::cout << buffer << std::endl;
    // }

    // f.resetEOF();
    
    // f.readBytes(buffer, 1);
    // std::cout << buffer << std::endl;
    
    // f.readBytes(buffer, 1);
    // std::cout << buffer << std::endl;
    
    // f.readBytes(buffer, 1);
    // std::cout << buffer << std::endl;



    // f.readline(buffer, 20);
    // std::cout << buffer << std::endl;

    // f.readline(buffer, 20);

    // std::cout << buffer << std::endl;

    // f.readline(buffer, 20);

    // std::cout << buffer << std::endl;
    
    // std::cout << std::strlen("") << std::endl;
    // std::cout << f.closeFile() << std::endl;


    
//
//    std::cout << std::bitset<16>(O_WRONLY | O_APPEND | O_CREAT) << std::endl;
//    std::cout << std::bitset<16>(convertMode(Mode::WRITE | Mode::APPEND | Mode::CREATE, 0)) << std::endl;
    
    
   


}
