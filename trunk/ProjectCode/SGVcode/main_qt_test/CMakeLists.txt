CMAKE_MINIMUM_REQUIRED(VERSION 3.8.1)
######################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
   
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	
ELSE ()  
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
######################################
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")  
   
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")  
	
	if(MSVC)
		set (CMAKE_PREFIX_PATH "C:\\Qt\\5.9.1\\msvc2015")
	endif(MSVC)
 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")  
   
ELSE ()  
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")  
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
######################################

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5SerialPort REQUIRED) 
find_package(Qt5Core  REQUIRED)

include_directories(
					${Qt5Widgets_INCLUDES}
					${Qt5SerialPort_INCLUDES})

qt5_wrap_ui(ui_FILES mainwindow.ui)
######################################
IF (CBB_BUILDING_CRACK_DETECTION)  
   
# Tell CMake to create the helloworld executable
add_executable(TEST_QT  WIN32
                            main.cpp
                            mainwindow.cpp
                            ${ui_FILES})



TARGET_LINK_LIBRARIES(TEST_QT 
							${Qt5Widgets_LIBRARIES}
							${Qt5SerialPort_LIBRARIES}) 

ENDIF (CBB_BUILDING_CRACK_DETECTION)
######################################
