cmake_minimum_required (VERSION 3.10)

project (ORB)


add_library(${PROJECT_NAME}  
		    src/ORB.cpp 
		    include/ORB.h 
		    ${CMAKE_ROOT_DIR}/ImgProcessorBase/include/ImgProcessorBase.h
		    ${CMAKE_ROOT_DIR}/ImgProcessorBase/src/ImgProcessorBase.cpp)

target_include_directories(${PROJECT_NAME}  PUBLIC include/)
target_link_libraries(${PROJECT_NAME}  
					  ${OpenCV_LIBS} 
					  Qt5::Widgets 
					  Qt5::Core Qt5::Gui
					  ImgProcessorBase)