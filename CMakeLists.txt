cmake_minimum_required (VERSION 2.8)

project(ModelFit)

set(ASSIMP_INCLUDE_DIR "/usr/include")
set(ASSIMP_LIBRARY_DIRS "/usr/lib")
set(ASSIMP_LIBRARIES "assimp")

set(FREEIMAGE_INCLUDE_DIR "/usr/include")
set(FREEIMAGE_LIBRARY_DIRS "/usr/lib")
set(FREEIMAGE_LIBRARIES "freeimage")

set(GLFW_INCLUDE_DIR "/usr/include/GLFW/")
set(GLFW_LIBRARY_DIRS "/usr/lib")
set(GLFW_LIBRARIES "glfw")

set(ZMQ_INCLUDE_DIR "/usr/include")
set(ZMQ_LIBRARY_DIRS "/usr/lib")
set(ZMQ_LIBRARIES "zmq")

find_package(Boost COMPONENTS system filesystem regex)

# c++11 enabled by default
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DHAND_FIT -DGCC -DFREEIMAGE_LIB")

include_directories(${ASSIMP_INCLUDE_DIR} ${FREEIMAGE_INCLUDE_DIR}
                    ${GLFW_INCLUDE_DIR} ${ZMQ_INCLUDE_DIR}
		    ${Boost_INCLUDE_DIRS}
		    "${PROJECT_SOURCE_DIR}/kinect_interface/include"
                    "${PROJECT_SOURCE_DIR}/ModelFit/src")
                    #"${PROJECT_SOURCE_DIR}/ModelFit/src/model_fit"
		    #"${PROJECT_SOURCE_DIR}/ModelFit/src/renderer"
		    #"${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/camera"
		    #"${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/geometry"
		    #"${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/lights"
		    #"${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/objects"
		    #"${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/shader"
		    #"${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/texture")
link_directories(${ASSIMP_LIBRARY_DIRS} ${FREEIMAGE_LIBRARY_DIRS}
                 ${GLFW_LIBRARY_DIRS} ${ZMQ_LIBRARY_DIRS}
		 ${Boost_LIBRARY_DIRS}
		 ${PROJECT_BINARY_DIR})
file(GLOB modelfit_src "${PROJECT_SOURCE_DIR}/ModelFit/src/model_fit/*.cpp"
                       "${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/*.cpp"
	               "${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/camera/*.cpp"
	               "${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/geometry/*.cpp"
	               "${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/lights/*.cpp"
	               "${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/objects/*.cpp"
	               "${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/shader/*.cpp"
	               "${PROJECT_SOURCE_DIR}/ModelFit/src/renderer/texture/*.cpp")

file(GLOB kinect_i_src "${PROJECT_SOURCE_DIR}/kinect_interface/src/data_str/*.cpp"
                       "${PROJECT_SOURCE_DIR}/kinect_interface/src/fastlz/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/file_io/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/glew/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/image_util/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/kinect_interface_primesense/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/math/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/string_util/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/threading/*.cpp"
		       "${PROJECT_SOURCE_DIR}/kinect_interface/src/windowing/*.cpp")

add_library(kinect_interface SHARED ${kinect_i_src})
target_link_libraries(kinect_interface ${ASSIMP_LIBRARIES} ${FREEIMAGE_LIBRARIES}
                      ${GLFW_LIBRARIES} ${ZMQ_LIBRARIES}
		      ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})

add_executable(modelfit_main ${modelfit_src} "${PROJECT_SOURCE_DIR}/ModelFit/src/main/main.cpp")
target_link_libraries(modelfit_main ${ASSIMP_LIBRARIES} ${FREEIMAGE_LIBRARIES}
                      ${GLFW_LIBRARIES} ${ZMQ_LIBRARIES} GLEW GLU GL kinect_interface )

add_executable(modelfit_calibrate ${modelfit_src} "${PROJECT_SOURCE_DIR}/ModelFit/src/main/main_calibrate.cpp")
target_link_libraries(modelfit_calibrate ${ASSIMP_LIBRARIES} ${FREEIMAGE_LIBRARIES}
                      ${GLFW_LIBRARIES} ${ZMQ_LIBRARIES} GLEW GLU GL kinect_interface )