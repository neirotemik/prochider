cmake_minimum_required(VERSION 3.10)
project(prochider C)

set(CMAKE_C_STANDARD 11)

add_library(prochider SHARED
    src/prochider.c
    src/prochider.h
)
# write the correct path
find_library(MINHOOK_LIB
    NAMES MinHook libMinHook minhook
    HINTS 
        ${MINHOOK_PATH}
        "C:/msys64/ucrt64/lib"
        "$ENV{MINHOOK_DIR}/lib"
)

if(NOT MINHOOK_LIB)
    message(FATAL_ERROR "MinHook not found! Set MINHOOK_PATH variable")
endif()

find_path(MINHOOK_INCLUDE MinHook.h
    HINTS 
        ${MINHOOK_PATH}/../include
        "C:/msys64/ucrt64/include"
        "$ENV{MINHOOK_DIR}/include"
)

target_include_directories(prochider PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${MINHOOK_INCLUDE}
)

target_link_libraries(prochider PRIVATE ${MINHOOK_LIB} ntdll)