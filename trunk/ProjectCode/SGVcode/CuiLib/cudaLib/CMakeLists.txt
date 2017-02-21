#check_for_cxx11_compiler########
check_for_cxx11_compiler(CXX11_COMPILER_T)
IF(CXX11_COMPILER_T)
 message(STATUS "Checking for C++11 compiler - available")
ENDIF()
#C11########
C11_Config()
#check_for_cxx11_compiler(C11Support)
############≈‰÷√OPENCV#########################################################
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
#Cuda########
ConfigCUDA()

SET(DIR_SRCS   
	     fuzzy.cu
	     HarrWaveletKernel.cu
	     SlicKernel.cu
		 CUDA_Code.cpp		 
		 ) 


CUDA_ADD_LIBRARY(cudaLib   STATIC    ${DIR_SRCS}  )



	