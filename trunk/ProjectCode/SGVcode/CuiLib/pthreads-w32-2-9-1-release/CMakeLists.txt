#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
#####################################################
C11_Config()  
ConfigOpenCv()
#####################################################
macro (ConfigPthreadsWindowsLib)

#CMAKE的内建变量，如果是true，就说明编译器的64位的，自然可以编译64bit的程序
INCLUDE_DIRECTORIES( 
					./include)
						
	if(CMAKE_CL_64)    
	
			SET(LIB_PTHREADS
				pthreadVC2.lib)

		    LINK_DIRECTORIES(		
				./lib/x64)

	else()
			SET(LIB_PTHREADS
				pthreadVC2.lib
				pthreadVCE2.lib
				pthreadVSE2.lib)

			LINK_DIRECTORIES(		
				./lib/x86)

	endif()

	link_libraries(${LIB_PTHREADS})
  
endmacro(ConfigPthreadsWindowsLib) 
#####################################################

###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
###################################
ConfigMFC_2() 
ConfigPthreadsWindowsLib()
##################################
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
   
ELSE ()  
   
ENDIF ()  

###############################################################################
SET(pthreads_SRCS
		./include/pt_mutex_switch.cpp
		./include/pt_mutex.cpp)

ADD_LIBRARY(pthreads   STATIC    ${pthreads_SRCS})
SET_TARGET_PROPERTIES(pthreads PROPERTIES LINKER_LANGUAGE CXX)