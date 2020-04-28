cmake_minimum_required (VERSION 3.10)

project (QtViewer)

add_executable(${PROJECT_NAME} 
			   src/QtViewer.cpp 
			   include/QtViewer.h 
			   ${CMAKE_ROOT_DIR}/GaussianBlur/include/GaussianBlur.h 
			   ${CMAKE_ROOT_DIR}/ImgProcessorBase/include/ImgProcessorBase.h 
			   ${CMAKE_ROOT_DIR}/Canny/include/Canny.h 
			   ${CMAKE_ROOT_DIR}/CheckableLabel/include/CheckableLabel.h
			    ${CMAKE_ROOT_DIR}/ORB/include/ORB.h
			   src/main.cpp)

target_include_directories(${PROJECT_NAME}  PUBLIC include/)
target_link_libraries(${PROJECT_NAME}  
					  ${OpenCV_LIBS} 
					  Qt5::Widgets 
					  Qt5::Core 
					  Qt5::Gui 
					  GaussianBlur
					  Canny
					  ImgProcessorBase
					  CheckableLabel
					  ORB)
#if (WIN32)
#	set_target_properties(${PROJECT_NAME}  PROPERTIES WIN32_EXECUTABLE true)
#endif(WIN32)