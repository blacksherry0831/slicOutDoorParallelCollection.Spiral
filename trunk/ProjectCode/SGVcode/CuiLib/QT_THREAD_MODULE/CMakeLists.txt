############≈‰÷√OPENCV#########################################################
C11_Config()
ConfigOpenCv()
CONFIGQT()
CONFIGPTHREADS()
###############################################################################
set(CMAKE_AUTOMOC ON)
###############################################################################

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

	

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

		ConfigMFC_2()	
	
ENDIF () 
	
	if (CBB_BUILDING_CRACK_DETECTION)
		SET(DIR_SRCS
				
				QtThreadPLC.cpp
				QtThreadStepMotor.cpp
				
				)		
	
	endif (CBB_BUILDING_CRACK_DETECTION)
 
ADD_LIBRARY(QT_THREAD_MODULE   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(QT_THREAD_MODULE
						MY_SDK_LIB
						QT_SDK_LIB
						QT_SOCKET_BASE
	) 
############################################################ 
############################################################
		

################################################################################