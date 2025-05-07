cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

# Встановлюємо стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(COMMON_SOURCES
    main.cpp
    mode.cpp
    FileClass.cpp
)

if (WIN32)
    set(OS_DEFINE OS_WINDOWS)
    set(PLATFORM_SOURCE platformWindows.cpp)
elseif(APPLE OR UNIX)
    set(OS_DEFINE OS_UNIX)
    set(PLATFORM_SOURCE platformUnix.cpp)
endif()


# Додаємо виконуваний файл з усіх cpp-файлів
add_executable(MyProject
    ${COMMON_SOURCES}
    ${PLATFORM_SOURCE}
    
)


target_compile_definitions(MyProject PRIVATE ${OS_DEFINE})


