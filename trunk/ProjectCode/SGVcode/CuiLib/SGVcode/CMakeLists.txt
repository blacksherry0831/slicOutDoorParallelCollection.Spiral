#####################################
C11_Config()
############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
SET(DIR_SRCS    ManhattanHistogram.cpp
				SeparateSpBlock.cpp
				
				SpAnalyze.cpp
				HsHg.cpp				
			
				SLIC.cpp
				
				Cui_DrawDepth.cpp
				
				ComputeSVG.cpp
				ComputeSVG2.cpp
				
				SplitTreeandBuilding.cpp
				LabColorAnalyzeCluster.cpp	
				
				ColorAnalyzeOnLab.cpp
				
				ColorBarCluster.cpp
				##################
				SceneDetermine.cpp
				####
				CuiHough/CuiHough3.cpp
				CuiHough/HoughAnalyze.cpp
				CuiHough/HoughAnalyze.cpp
				###
				Harr/CuiHarrTransformLet.cpp
				Harr/HarrWaveletTransImg.cpp)  
 #ImageType.h
SET(LINK_LIB 
	         FilePathName
	         cudaLib
	         Kmean
	         SVMcode
	         SGVBaseCode
	)
###############################################################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
			
			ADD_LIBRARY(SGVcode  STATIC     ${DIR_SRCS}  )
			
			find_package(Threads REQUIRED)
			
			if(THREADS_HAVE_PTHREAD_ARG)
			  #target_compile_options(PUBLIC SGVcode "-pthread")
			endif()
	
			
			if(CMAKE_THREAD_LIBS_INIT)
	 
				TARGET_LINK_LIBRARIES(SGVcode 
										"${CMAKE_THREAD_LIBS_INIT}"
										 ${LINK_LIB})
			ELSE()
			
				TARGET_LINK_LIBRARIES(SGVcode ${LINK_LIB})
				
			endif()	


ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")   

		ADD_LIBRARY(SGVcode  STATIC     ${DIR_SRCS}  )
		TARGET_LINK_LIBRARIES(SGVcode ${LINK_LIB})


ELSE()

		ADD_LIBRARY(SGVcode  STATIC     ${DIR_SRCS}  )
		TARGET_LINK_LIBRARIES(SGVcode ${LINK_LIB})


	
ENDIF () 
###############################################################################


###############################################################################
SET(BASE_SRCS   
		MallocCui.cpp	
		SpSetProperty.cpp
		SpectralClusterInDoor.cpp
		SpectralClustering.cpp
		ImageMemData.cpp
		ImageData.cpp
		cui_GeneralImgProcess.cpp
	)  
SET(BASE_LINK_LIB 
	         FilePathName
	         cudaLib
	         Kmean
	         SVMcode
	         SGVcode
	)
ADD_LIBRARY(SGVBaseCode  STATIC     ${BASE_SRCS}  )
TARGET_LINK_LIBRARIES(SGVBaseCode ${BASE_LINK_LIB})
###############################################################################
#CuiLib/SGVcode/ImageType.h
#CuiLib/SGVcode/ManhattanHistogram.cpp
#CuiLib/SGVcode/SeparateSpBlock.cpp
#CuiLib/SGVcode/SpectralClustering.cpp
#CuiLib/SGVcode/SpAnalyze.cpp
#CuiLib/SGVcode/HsHg.cpp
#CuiLib/SGVcode/SpSetProperty.cpp
#CuiLib/SGVcode/ImageMemData.cpp
#CuiLib/SGVcode/ImageData.cpp
#CuiLib/SGVcode/SLIC.cpp
#CuiLib/SGVcode/cui_GeneralImgProcess.h
#CuiLib/SGVcode/cui_GeneralImgProcess.cpp
#CuiLib/SGVcode/Cui_DrawDepth.cpp
#CuiLib/SGVcode/ComputeSVG.cpp
#CuiLib/SGVcode/ComputeSVG2.cpp
#CuiLib/SGVcode/SpectralClusterInDoor.cpp
#CuiLib/SGVcode/SplitTreeandBuilding.cpp
#CuiLib/SGVcode/LabColorAnalyzeCluster.cpp
#CuiLib/SGVcode/CuiHough/CuiHough3.cpp
#CuiLib/SGVcode/CuiHough/HoughAnalyze.cpp
#CuiLib/SGVcode/CuiHough/HoughAnalyze.cpp
#CuiLib/SGVcode/Harr/CuiHarrTransformLet.cpp
#CuiLib/SGVcode/Harr/HarrWaveletTransImg.cpp				
#CuiLib/SGVcode/ColorAnalyzeOnLab.cpp
#CuiLib/SGVcode/rtthread.cpp
#CuiLib/SGVcode/MallocCui.cpp
#CuiLib/SGVcode/LabColorAnalyzeCluster_veins.cpp
#CuiLib/SGVcode/ColorBarCluster.cpp