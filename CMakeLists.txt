cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME "chess")
project(${PROJECT_NAME})
set(SFML_DIR "libs/SFML-2.5.1/lib/cmake/SFML")

file(GLOB ALL_REQUIRED_DLL "libs/required_dlls/*.dll")
file(COPY ${ALL_REQUIRED_DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

link_libraries(stdc++fs)
file(GLOB_RECURSE SCRF "src/.cpp" "src/.hpp")

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} ${SCRF} main.cpp src/Piece.cpp src/Piece.h src/Board.cpp src/Board.h src/Move.cpp src/Move.h src/Button.cpp src/Button.h src/Menu.cpp src/Menu.h src/Connector.h src/Connector.cpp)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-network sfml-system)