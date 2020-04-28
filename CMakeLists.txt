cmake_minimum_required (VERSION 3.10)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_ROOT_DIR ${CMAKE_SOURCE_DIR})
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

 
add_subdirectory(ImgProcessorBase)
add_subdirectory(GaussianBlur)
add_subdirectory(Canny)
add_subdirectory(QtViewer)
add_subdirectory(CheckableLabel)
add_subdirectory(ORB)
add_subdirectory(SIFT)
 