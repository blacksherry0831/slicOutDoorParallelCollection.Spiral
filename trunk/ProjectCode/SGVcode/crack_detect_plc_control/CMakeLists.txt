CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
######################################
CONFIGQT()
CONFIGPTHREADS()
ConfigOpenCv()
######################################
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


######################################
IF (CBB_BUILDING_CRACK_DETECTION)  
   
# Tell CMake to create the helloworld executable
add_executable(EXE_CRACK_DETECT_PLC_CONTROL  WIN32
                            main.cpp)



TARGET_LINK_LIBRARIES(EXE_CRACK_DETECT_PLC_CONTROL
													QT_SDK_LIB
													QT_SOCKET_BASE
													QT_THREAD_MODULE
													QT_THREAD_FLOWCTRL

													SerialPortModule) 

ENDIF (CBB_BUILDING_CRACK_DETECTION)
######################################
