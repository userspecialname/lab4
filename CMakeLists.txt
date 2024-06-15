cmake_minimum_required(VERSION 3.28)
project(laboratory4)

set(CMAKE_CXX_STANDARD 17)

add_executable(laboratory4 main.cpp
        tree.h
        generate.h
        generate.cpp
        functions.h
        ArraySequence.h
        Sequence.h
        main.cpp
        UI.h
        UI.cpp
        DynamicArray.h
        UI.cpp
        tests.cpp
tests.h
        tests.cpp)
set(GRAPHVIZ_DIR "usr/opt/homebrew/opt/graphviz")
file(GLOB GVC_LIBS "${GRAPHVIZ_DIR}/lib/*.dylib")
target_link_libraries(${CMAKE_PROJECT_NAME} ${GVC_LIBS})
include_directories("${GRAPHVIZ_DIR}/include/graphviz")