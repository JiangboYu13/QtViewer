project (CheckableLabel)


add_library(${PROJECT_NAME}  
		    include/CheckableLabel.h
		    src/CheckableLabel.cpp)

target_include_directories(${PROJECT_NAME}  PUBLIC include/)
target_link_libraries(${PROJECT_NAME}  
					  Qt5::Widgets 
					  Qt5::Core Qt5::Gui)