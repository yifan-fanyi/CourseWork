ADD_LIBRARY(basic SHARED 
  	Basic.cpp
    	BinaryTree/BinaryTree.cpp
    	Evaluate/Evaluate.cpp
    	KMeans/KMeans.cpp
)

target_link_libraries(	basic
	thirdparty
    	${THIRD_PARTY_LIBS}
)