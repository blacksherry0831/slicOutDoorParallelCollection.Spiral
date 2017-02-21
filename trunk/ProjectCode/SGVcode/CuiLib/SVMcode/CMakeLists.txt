C11_Config()
#####################################
#SET(SVM_SRCS  SVMcode/TrainImgUseSvm.cpp
#	          SVMcode/TrainTreeBuilding.cpp
#	)
#####################################
SET(DIR_SRCS
		TrainImgUseSvm.cpp
		TrainTreeBuilding.cpp
	)  
 #ImageType.h
SET(LINK_LIB
	      FilePathName
	      tinyxml2
	)

############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
ADD_LIBRARY(SVMcode  STATIC     ${DIR_SRCS}  )
#TARGET_LINK_LIBRARIES(SVMcode
#	                      tinyxml2)
#FilePathName
TARGET_LINK_LIBRARIES(SVMcode ${LINK_LIB})



