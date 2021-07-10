cmake_minimum_required(VERSION 3.10)
project(CLParser)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)



# Message coloring convenience variables
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(ColourBold  "${Esc}[1m")
    set(Red         "${Esc}[31m")
    set(Green       "${Esc}[32m")
    set(Yellow      "${Esc}[33m")
    set(Blue        "${Esc}[34m")
    set(Magenta     "${Esc}[35m")
    set(Cyan        "${Esc}[36m")
    set(White       "${Esc}[37m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
    set(BoldYellow  "${Esc}[1;33m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldMagenta "${Esc}[1;35m")
    set(BoldCyan    "${Esc}[1;36m")
    set(BoldWhite   "${Esc}[1;37m")
endif()

# Convenience function to color output
function(DisplayPackage pkg)
    message("[${Yellow}${pkg}${ColourReset}]")
endfunction()

# Set subdirectory cmake options
if(CMAKE_BUILD_TYPE MATCHES Debug)
    DisplayPackage("CLParser")
    message(STATUS "Building Debug Version")
    add_definitions(-DDEBUG_MESSAGES)
else()
    DisplayPackage("CLParser")
    message(STATUS "Building Release Version")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

# Get all of the files in src/ that match C/C++ style ext.
file(GLOB SOURCE_FILES "src/*.c" "src/*.h" "src/*.cpp" "src/*.hpp")

add_library(CLParser STATIC
        ${SOURCE_FILES}
        )

add_executable(CLParserTest
        main.cpp
        )
target_link_libraries(CLParserTest CLParser)

target_include_directories(CLParser PRIVATE src)
target_include_directories(CLParserTest PRIVATE src)

# Set G++ build info for this project (depending on debug vs release)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(CLParser PRIVATE -g -pthread -Wno-deprecated-declarations)
else()
    target_compile_options(CLParser PRIVATE -O3 -pthread -Wno-deprecated-declarations)
endif(CMAKE_BUILD_TYPE MATCHES Debug)