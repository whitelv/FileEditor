cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

# Встановлюємо стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Додаємо виконуваний файл з усіх cpp-файлів
add_executable(MyProject
    main.cpp mode.h FileClass.cpp FileClass.h mode.cpp
)

# Вказуємо заголовочні файли (не обов'язково, але для IDE корисно)
target_include_directories(MyProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


