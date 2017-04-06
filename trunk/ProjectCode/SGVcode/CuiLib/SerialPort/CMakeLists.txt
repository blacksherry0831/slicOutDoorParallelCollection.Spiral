############≈‰÷√OPENCV#########################################################
C11_Config()
ConfigOpenCv()
###############################################################################
ConfigMFC_2()
###############################################################################
SET(DIR_SRCS   
		SerialPort.cpp
		Gps_WG_8020.cpp
		Compass_HCM365.cpp
		AGV_Dirver.cpp			 
		 )

ADD_LIBRARY(SerialPort   STATIC    ${DIR_SRCS}  )



	