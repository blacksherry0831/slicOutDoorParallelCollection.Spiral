C11_Config()
#add_library(FilePathName FileNameSplit.cpp PointWithDistance.cpp£©
SET(DIR_SRCS   
				FileNameSplit.cpp 
				PointWithDistance.cpp )  	

ADD_LIBRARY(FilePathName  STATIC     ${DIR_SRCS}  )
TARGET_LINK_LIBRARIES(FilePathName tinyxml2)

if(MSVC_VERSION)
  set_target_properties(tinyxml2 PROPERTIES
    LINK_FLAGS " /INCREMENTAL:NO /MANIFEST /NOLOGO"
 #COMPILE_FLAGS "-DHELLO_EXPORTS"  #ajout®¶ in last a verif demin
 )
endif(MSVC_VERSION)





############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################



	