#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
###############################################################################
#MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
C11_Config()  
ConfigOpenCv()
###############################################################################
function (ConfigPT)

#CMAKE的内建变量，如果是true，就说明编译器的64位的，自然可以编译64bit的程序

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
  
endfunction (ConfigPT) 
###############################################################################
SET(BASE_LINK_LIB 
	         tinyxml2
			 SGVcode
			 SerialPort
			 pthreads
	)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	
	ConfigMFC_2() 
    ConfigPT()


ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
###############################################################################
SET(HW_SDK_SRCS
	live_video.cpp
	live_set.cpp
	ipc.cpp
	dc_save.cpp)

ADD_LIBRARY(HW_SDK_LIB   STATIC    ${HW_SDK_SRCS})

TARGET_LINK_LIBRARIES(HW_SDK_LIB ${BASE_LINK_LIB})
###############################################################################