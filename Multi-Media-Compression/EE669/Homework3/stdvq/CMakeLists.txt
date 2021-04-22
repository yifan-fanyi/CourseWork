cmake_minimum_required( VERSION 2.8 )
project ( EE669HW )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c99 -march=native -O3" )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )


############### dependencies ######################

include_directories( 
	${PROJECT_SOURCE_DIR}
)
add_library( HW3 SHARED
    	lloyd0.c
    	lloyd1.c
    	lloyd2.c
    	stdvq_util.c
)



add_executable( stdvq stdvq.c )
target_link_libraries(stdvq 
	HW3 
)

add_executable( stdvqe stdvqe.c )
target_link_libraries(stdvqe 
	HW3 
)


