cmake_minimum_required(VERSION 3.19)
project ( CSCI576 )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )


include_directories( 
	${PROJECT_SOURCE_DIR}
)
add_library( core SHARED
    	Image.cpp
)

add_executable( main main.cpp )
target_link_libraries(main 
	core
)




