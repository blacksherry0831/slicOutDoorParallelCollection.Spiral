############配置OPENCV#########################################################
C11_Config()
ConfigOpenCv()
###############################################################################
function(ConfigPT)
	#CMAKE的内建变量，如果是true，就说明编译器的64位的，自然可以编译64bit的程序

	IF (CMAKE_SYSTEM_NAME MATCHES "Windows")

		INCLUDE_DIRECTORIES( #相关头文件的目录
				../pthreads-w32-2-9-1-release/include
			)

		if(CMAKE_CL_64)    
			
			SET(LIB_PTHREADS
				pthreadVC2.lib)

			LINK_DIRECTORIES(		
				../pthreads-w32-2-9-1-release/lib/x64)

		else()
			SET(LIB_PTHREADS
				pthreadVC2.lib
				pthreadVCE2.lib
				pthreadVSE2.lib)

			LINK_DIRECTORIES(		
				../pthreads-w32-2-9-1-release/lib/x86)

		endif()

			link_libraries(${LIB_PTHREADS})  

	ENDIF()  
endfunction (ConfigPT) 
############################################################ 
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	ConfigMFC_2()
	ConfigPT()


	SET(DIR_SRCS   
			SerialPortBase.cpp
			SerialPort.cpp)

	if (CBB_BUILDING_AGV_CAR)
		SET(DIR_SRCS   
			${DIR_SRCS}
			Compass_HCM365.cpp
			Gps_WG_8020.cpp
			AGV_Dirver.cpp)
	endif (CBB_BUILDING_AGV_CAR)		
	
	if(CBB_BUILDING_CRACK_DETECTION)
		SET(DIR_SRCS   
			${DIR_SRCS}
			BE_1105_Dirver.cpp)		
	
	endif (CBB_BUILDING_AGV_CAR)
 
ENDIF () 
############################################################ 
############################################################
		
ADD_LIBRARY(SerialPort   STATIC    ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(SerialPort
	MY_SDK_LIB
	pthreads) 
################################################################################