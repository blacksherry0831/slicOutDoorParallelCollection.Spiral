#####################################
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
)
#####################################

#####################################
ADD_SUBDIRECTORY(tinyxml2)
#ADD_SUBDIRECTORY(tinyxml)
ADD_SUBDIRECTORY(FilePathName)

ADD_SUBDIRECTORY(QT_SDK_LIB)
ADD_SUBDIRECTORY(JQLibrary)

if(NEED_OPENCV)

	ADD_SUBDIRECTORY(opencv_calibration)
	

endif()

if (NEED_CUDA)

	ADD_SUBDIRECTORY(cudaLib)	
	

endif()

ADD_SUBDIRECTORY(SGVcode)

ADD_SUBDIRECTORY(ML)
ADD_SUBDIRECTORY(SVMcode)

ADD_SUBDIRECTORY(HW_SDK_LIB)
ADD_SUBDIRECTORY(MY_SDK_LIB)

ADD_SUBDIRECTORY(SerialPort)
ADD_SUBDIRECTORY(SerialPortBase)

ADD_SUBDIRECTORY(SocketQT)

ADD_SUBDIRECTORY(Harr)

if (NEED_HTTP)

	ADD_SUBDIRECTORY(libcurl)

endif()

if (NEED_JSON)

	ADD_SUBDIRECTORY(jsoncpp-master)

endif()


ADD_SUBDIRECTORY(pthreads-w32-2-9-1-release)

ADD_SUBDIRECTORY(uCOS)

if(NEED_WEBSOCKETPP)

	ADD_SUBDIRECTORY(websocketpp-experimental)
	ADD_SUBDIRECTORY(stomp)

endif(NEED_WEBSOCKETPP)

if (CBB_BUILDING_AGV_CAR AND NEED_WEBSOCKETPP AND NEED_HTTP)

	ADD_SUBDIRECTORY(communication_2_server)

endif()
