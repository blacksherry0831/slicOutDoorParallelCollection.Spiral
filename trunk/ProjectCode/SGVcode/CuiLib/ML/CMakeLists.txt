C11_Config()
#####################################
#SET(SVM_SRCS  SVMcode/TrainImgUseSvm.cpp
#	          SVMcode/TrainTreeBuilding.cpp
#	)
#####################################
SET(DIR_SRCS     Kmean/Kmean.cpp)  
 #ImageType.h

############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################

ADD_LIBRARY(Kmean  STATIC     ${DIR_SRCS}  )
#TARGET_LINK_LIBRARIES(SGVcode FilePathName cudaLib)