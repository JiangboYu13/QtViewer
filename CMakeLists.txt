cmake_minimum_required (VERSION 3.10)

project (QtViewer)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
  	set (QT5_CMAKE_DIR "C:/Qt/5.13.0/msvc2015_64/lib/cmake")
	set (Qt5Widgets_DIR ${QT5_CMAKE_DIR}/Qt5Widgets)
	set (Qt5Gui_DIR ${QT5_CMAKE_DIR}/Qt5Gui)
	set (Qt5Core_DIR ${QT5_CMAKE_DIR}/Qt5Core)
	set (OpenCV_DIR "c:/opencv/build/x64/vc14/lib")
endif(WIN32)


find_package(Qt5 REQUIRED
			     Core
			     Gui
			     Widgets	
)
find_package(OpenCV  REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(GaussianBlur 
		    src/GaussianBlur.cpp 
		    include/GaussianBlur.h 
		    include/ImgProcessorBase.h)
target_link_libraries(GaussianBlur 
					  ${OpenCV_LIBS} 
					  Qt5::Widgets 
					  Qt5::Core Qt5::Gui)

add_library(Canny 
		    src/Canny.cpp 
		    include/Canny.h 
		    include/ImgProcessorBase.h)
target_link_libraries(Canny 
					  ${OpenCV_LIBS} 
					  Qt5::Widgets 
					  Qt5::Core Qt5::Gui)



add_executable(${PROJECT_NAME} 
			   src/QtViewer.cpp 
			   include/QtViewer.h 
			   include/GaussianBlur.h 
			   include/ImgProcessorBase.h 
			   include/Canny.h 
			   src/main.cpp)
target_link_libraries(${PROJECT_NAME}  
					  ${OpenCV_LIBS} 
					  Qt5::Widgets 
					  Qt5::Core 
					  Qt5::Gui 
					  GaussianBlur
					  Canny)
if (WIN32)
	set_target_properties(${PROJECT_NAME}  PROPERTIES WIN32_EXECUTABLE true)
endif(WIN32)
