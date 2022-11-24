cmake_minimum_required(VERSION 3.10)
project(Tetris VERSION  1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS system chrono thread)

if(WIN32)
    find_package (sqlite3 REQUIRED)
elseif(UNIX)
    find_package (SQLite3 REQUIRED)
endif()


if(UNIX)
    find_package (Curses REQUIRED)
endif()

add_executable(Tetris
    main.cpp
    Tetris.cpp
    Tetromino.cpp
    Tetris_Statistics.cpp
    Tetris_Draw_Linux_Console.cpp
    Tetris_Draw_Windows_Console.cpp
    Tetris.h
    Tetris_Definitions.h
    Tetris_Draw.h
    Tetris_Draw_Console_Factory.h
    Tetris_Draw_Linux_Console.h
    Tetris_Draw_Windows_Console.h
    Tetris_Statistics.h
    Tetromino.h
)

# *** setup includes ***
list(APPEND list_of_includes ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS} )

if(WIN32)
    list(APPEND list_of_includes ${SQLITE3_INCLUDE_DIRS})
elseif(UNIX)
    list(APPEND list_of_includes ${SQLite3_INCLUDE_DIRS})
    list(APPEND list_of_includes ${CURSES_INCLUDE_DIR})
endif()

target_include_directories(Tetris PUBLIC ${list_of_includes})

# *** setup libs ***
list(APPEND list_of_libs ${Boost_LIBRARIES} )

if(WIN32)
    list(APPEND list_of_libs ${SQLITE3_LIBRARIES} )
elseif(UNIX)
    list(APPEND list_of_libs ${SQLite3_LIBRARIES} )
    list(APPEND list_of_libs ${CURSES_LIBRARIES} )
endif()

target_link_libraries(Tetris PUBLIC ${list_of_libs})
