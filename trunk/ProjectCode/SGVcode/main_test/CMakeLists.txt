#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#项目名称
###############################################################################

C11_Config()  
ConfigOpenCv()
  

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	ConfigMFC_2() 
   
	#SET(HW_WIN32	
	#play_sdk.lib
	#hwclient.lib
	#ijl15l.lib)

   #LINK_DIRECTORIES(
   #CuiLib/HW_SDK_LIB/lib/
   #)

  #link_libraries(${HW_WIN32})

ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  
