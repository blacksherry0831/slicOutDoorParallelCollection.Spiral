#####################################
ADD_SUBDIRECTORY(tinyxml2)
#ADD_SUBDIRECTORY(tinyxml)
ADD_SUBDIRECTORY(FilePathName)



if (NEED_CUDA)

	ADD_SUBDIRECTORY(cudaLib)	
	ADD_SUBDIRECTORY(SGVcode)

endif()



ADD_SUBDIRECTORY(ML)
ADD_SUBDIRECTORY(SVMcode)

ADD_SUBDIRECTORY(HW_SDK_LIB)
ADD_SUBDIRECTORY(MY_SDK_LIB)

ADD_SUBDIRECTORY(SerialPort)

if (NEED_HTTP)

	ADD_SUBDIRECTORY(libcurl)

endif()

if (NEED_JSON)

	ADD_SUBDIRECTORY(jsoncpp-master)

endif()


ADD_SUBDIRECTORY(pthreads-w32-2-9-1-release)



if(NEED_WEBSOCKETPP)

	ADD_SUBDIRECTORY(websocketpp-experimental)
	ADD_SUBDIRECTORY(stomp)

endif(NEED_WEBSOCKETPP)

if (CBB_BUILDING_AGV_CAR AND NEED_WEBSOCKETPP AND NEED_HTTP)

	ADD_SUBDIRECTORY(communication_2_server)

endif()
