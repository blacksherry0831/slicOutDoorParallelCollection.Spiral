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
qt5_wrap_ui(ui_FILES dialog.ui)
######################################
IF (CBB_BUILDING_CRACK_DETECTION)  
   
# Tell CMake to create the helloworld executable
add_executable(TEST_QT  WIN32
                            main.cpp
							dialog.cpp
							MouseLabel.cpp
                            ClickableLabel.cpp
							mainwindow.cpp
                            ${ui_FILES})



TARGET_LINK_LIBRARIES(TEST_QT 
							QT_SOCKET_BASE
							QT_THREAD_MODULE
							QT_THREAD_FLOWCTRL
							SerialPortModule) 

ENDIF (CBB_BUILDING_CRACK_DETECTION)
######################################
