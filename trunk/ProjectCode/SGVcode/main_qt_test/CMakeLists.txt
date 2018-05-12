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

qt5_wrap_ui(ui_FILES mainwindow.ui)
######################################
IF (CBB_BUILDING_CRACK_DETECTION)  
   
# Tell CMake to create the helloworld executable
add_executable(TEST_QT  WIN32
                            main.cpp
                            mainwindow.cpp
                            ${ui_FILES})



TARGET_LINK_LIBRARIES(TEST_QT 
							SOCKET_SERVER
							SerialPortModule) 

ENDIF (CBB_BUILDING_CRACK_DETECTION)
######################################
