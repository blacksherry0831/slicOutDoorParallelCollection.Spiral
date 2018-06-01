#####################################
C11_Config()
############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
SET(DIR_SRCS    CuiHarrTransformLet.cpp
				HarrWaveletTransImg.cpp)  
###############################################################################
SET(LINK_LIB 

	)
###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
									
			find_package(Threads REQUIRED)
			
			if(THREADS_HAVE_PTHREAD_ARG)
			  
			endif()
				
			if(CMAKE_THREAD_LIBS_INIT)
	 
				SET(LINK_LIB	${CMAKE_THREAD_LIBS_INIT}
								${LINK_LIB})
			ELSE()
							
			endif()	


ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")   

ELSE()
	
ENDIF () 
###############################################################################
ADD_LIBRARY(Harr  STATIC     ${DIR_SRCS}  )

TARGET_LINK_LIBRARIES(Harr   ${LINK_LIB})

###############################################################################

###############################################################################