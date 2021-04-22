add_library( HW1 SHARED
    	HW1Basic.cpp
   	ImageDenoising.cpp
    	ImgDemHistMupl.cpp
    	HW1Fancy.cpp
)

target_link_libraries(	HW1
	basic
)
