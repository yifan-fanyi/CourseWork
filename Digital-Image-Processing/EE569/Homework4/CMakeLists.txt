add_library( HW4 SHARED
    	HW4Basic.cpp
	TextureAnalysis.cpp
	FeatureExtractor.cpp
	HW4Fancy.cpp


)
target_link_libraries(	HW4
  	basic HW1
)
