############����OPENCV#########################################################
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
				QtThreadBaseCmdQ.cpp			
				QtTcpServerClientSession.cpp	
				QtTcpServer.cpp
				
				QtTcpClientBase.cpp
				QtTcpClient.cpp
				
				QtThreadServer.cpp
								
				QtThreadSession.cpp
				QtThreadSessionWorkFlow.cpp	

				QtThreadClientLifeCycle.cpp	
				QtThreadClientCtrlTest.cpp

				DataFrame.cpp
				
				CMD_CTRL_DATA_LOCAL.cpp
				CMD_CTRL_DATA_ARM_FPGA.cpp
				CMD_CTRL_DATA_PLC.cpp				
				CMD_CTRL_DATA.cpp
				
				
				CMD_CTRL.cpp
				CMD_CTRL_Q.cpp
	
				QtMsgCmdQ.cpp				

				QtThreadSocketClient.cpp
				QtThreadSocketClientPlc.cpp

				exCircleData.cpp
				
				ChannelsDataBase.cpp
				ChannelsData.cpp
				ChannelsData4Show.cpp
				
				QtLink.cpp
				Saveframe.cpp)		
	
	endif (CBB_BUILDING_CRACK_DETECTION)
 
ADD_LIBRARY(QT_SOCKET_BASE   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(QT_SOCKET_BASE
						pthreads
						MY_SDK_LIB
						QT_SDK_LIB
	) 
############################################################ 
############################################################
		

################################################################################