############≈‰÷√OPENCV#########################################################
C11_Config()
ConfigOpenCv()
CONFIGQT()
	CONFIGPTHREADS()
###############################################################################

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

	

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

		ConfigMFC_2()	
	
ENDIF () 
	
	if (CBB_BUILDING_CRACK_DETECTION)
		SET(DIR_SRCS				
				QtTcpServer.cpp
				QtTcpClient.cpp
				QtThreadBase.cpp
				QtThreadClient.cpp
				QtThreadServer.cpp
				QtThreadPLC.cpp
				DataFrame.cpp
				CMD_CTRL.cpp
				QtThread8Video.cpp
				QtThreadSocketClient.cpp)		
	
	endif (CBB_BUILDING_CRACK_DETECTION)
 
ADD_LIBRARY(SOCKET_SERVER   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(SOCKET_SERVER
						pthreads
	) 
############################################################ 
############################################################
		

################################################################################