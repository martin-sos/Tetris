cmake_minimum_required(VERSION 3.10)

project(Tetris VERSION  1.0)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BOOST_ROOT C:/boost_1_80_0) # either set it here or from the command line  
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
find_package(Boost REQUIRED COMPONENTS system chrono thread)

find_package (sqlite3 REQUIRED)

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
	Tetris_Draw_Linux_Console.h 
	Tetris_Draw_Windows_Console.h
	Tetris_Statistics.h
	Tetromino.h
)
target_include_directories(Tetris PUBLIC 
	${PROJECT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
)

target_link_libraries(Tetris PUBLIC ${Boost_LIBRARIES} ${SQLITE3_LIBRARIES})
