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
add_executable(EXE_CRACK_DETECT_IMG_PROCESS  WIN32
                            main.cpp
							dialog.cpp
							MouseLabel.cpp
                            ClickableLabel.cpp
							mainwindow.cpp
                            ${ui_FILES})



TARGET_LINK_LIBRARIES(EXE_CRACK_DETECT_IMG_PROCESS
							QT_THREAD_FLOWCTRL
							QT_THREAD_MODULE
							SerialPortModule) 

ENDIF (CBB_BUILDING_CRACK_DETECTION)
######################################
