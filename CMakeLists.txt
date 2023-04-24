cmake_minimum_required(VERSION 3.15)
project(LES)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    "src/Main.cpp"
    "src/Interpreter.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
