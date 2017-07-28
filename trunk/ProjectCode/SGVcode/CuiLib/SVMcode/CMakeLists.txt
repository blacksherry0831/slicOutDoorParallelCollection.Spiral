C11_Config()
#####################################
SET(DIR_SRCS
		TrainImgUseSvm.cpp
		TrainTreeBuilding.cpp
	)  

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

TARGET_LINK_LIBRARIES(SVMcode ${LINK_LIB})



