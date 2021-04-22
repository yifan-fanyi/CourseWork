cmake_minimum_required( VERSION 2.8 )
project ( EE569HW )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR} )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set( THIRD_PARTY_LIBS 
    ${OpenCV_LIBS}
)

if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

############### dependencies ######################

include_directories( 
	${PROJECT_SOURCE_DIR}/basic
	${PROJECT_SOURCE_DIR}/Thirdparty
	${PROJECT_SOURCE_DIR}/Layout

	${PROJECT_SOURCE_DIR}/Homework1
	${PROJECT_SOURCE_DIR}/Homework2
	${PROJECT_SOURCE_DIR}/Homework3
	${PROJECT_SOURCE_DIR}/Homework4
)

add_subdirectory( 	basic		)
add_subdirectory(	Thirdparty	)
add_subdirectory(	Layout 		)
add_subdirectory(	Homework1	)
add_subdirectory(	Homework2	)
add_subdirectory(	Homework3	)
add_subdirectory(	Homework4	)








