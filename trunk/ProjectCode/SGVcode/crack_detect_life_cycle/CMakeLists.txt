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
add_executable(EXE_CRACK_DETCCT_LIFE_CYCLE  WIN32
                            main.cpp
							)



TARGET_LINK_LIBRARIES(EXE_CRACK_DETCCT_LIFE_CYCLE 
							SOCKET_SERVER
							SerialPortModule
							) 

ENDIF (CBB_BUILDING_CRACK_DETECTION)
######################################
