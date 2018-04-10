############≈‰÷√OPENCV#########################################################
C11_Config()
ConfigOpenCv()
CONFIGQT()
###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

	ConfigMFC_2()
	CONFIGPTHREADS()
	
ENDIF () 
############################################################
if (CBB_BUILDING_AGV_CAR)
		SET(DIR_SRCS   
			Compass_HCM365.cpp
			Gps_WG_8020.cpp
			AGV_Dirver.cpp)
endif (CBB_BUILDING_AGV_CAR)
############################################################	
if (CBB_BUILDING_CRACK_DETECTION)
		SET(DIR_SRCS   
				BE_1105_Dirver.cpp)		
	
endif (CBB_BUILDING_CRACK_DETECTION)
 ############################################################
ADD_LIBRARY(SerialPortModule   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(SerialPortModule
	SerialPortBase
	pthreads) 	
############################################################
		

################################################################################