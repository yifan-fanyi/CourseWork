add_library( HW3 SHARED
    	HW3Basic.cpp
    	GeometricModification.cpp
    	HW3Fancy.cpp
    	MorphologicalProcessing.cpp
)

target_link_libraries( HW3
	basic 
	HW1
	HW2
)
