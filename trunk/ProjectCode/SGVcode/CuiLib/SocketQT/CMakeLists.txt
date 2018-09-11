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
				QtTcpServer.cpp
				QtTcpClient.cpp
				QtThreadBase.cpp
				QtThreadClientCtrl.cpp
				QtThreadClientCtrlTest.cpp
				QtThreadServer.cpp
				QtThreadPLC.cpp
				QtThreadStepMotor.cpp
				DataFrame.cpp
				CMD_CTRL_DATA.cpp
				CMD_CTRL.cpp
				CMD_CTRL_Q.cpp				
				QtThread8Video.cpp
				QtThreadSocketClient.cpp
				exCircleData.cpp
				ChannelsData.cpp
				QtThread8VideoProcess.cpp
				QtThread8VideoRaw.cpp
				QtTcpServerTest.cpp
				QtLink.cpp
				Saveframe.cpp)		
	
	endif (CBB_BUILDING_CRACK_DETECTION)
 
ADD_LIBRARY(SOCKET_SERVER   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(SOCKET_SERVER
						pthreads
						MY_SDK_LIB
						QT_SDK_LIB
	) 
############################################################ 
############################################################
		

################################################################################