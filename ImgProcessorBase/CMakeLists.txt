cmake_minimum_required (VERSION 3.10)

project (ImgProcessorBase)


add_library(${PROJECT_NAME}  
		    include/ImgProcessorBase.h
		    src/ImgProcessorBase.cpp)

target_include_directories(${PROJECT_NAME}  PUBLIC include/)
target_link_libraries(${PROJECT_NAME}  
					  ${OpenCV_LIBS} 
					  Qt5::Widgets 
					  Qt5::Core Qt5::Gui)