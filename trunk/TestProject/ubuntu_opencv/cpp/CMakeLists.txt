PROJECT(OpenCV_Example)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
FIND_PACKAGE( OpenCV REQUIRED )
#这是cmake用来查找opencv包用的，不用改
# Declare the target (an executable)
ADD_EXECUTABLE(OpenCV_Example  houghlines.cpp)      
#这里括号里面的两个参数分别是工程项目名和我们要编译文件名的意思，记住中间一空格键隔开
TARGET_LINK_LIBRARIES(OpenCV_Example ${OpenCV_LIBS})
#这是我们链接到OpenCV库的环节，我们只要更改前面第一个参数位我们的工程项目名即可
#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
#好了，就修改这么点东西，保存，关闭。
