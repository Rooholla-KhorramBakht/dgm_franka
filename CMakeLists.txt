cmake_minimum_required(VERSION 3.4)

project(libfranka-examples CXX)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT FRANKA_IS_FOUND)
    find_package(Franka REQUIRED)
endif()
find_package(Eigen3 REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


add_library(examples_common STATIC
  src/examples_common.cpp
)
# target_include_directories(examples_common PUBLIC /include)
include_directories(/include)
target_link_libraries(examples_common PUBLIC Franka::Franka Eigen3::Eigen3)

set(EXAMPLES
  joint_impedance_control
)

foreach(example ${EXAMPLES})
  add_executable(${example} src/${example}.cpp)
  # target_include_directories(${example} PUBLIC /include)
  target_link_libraries(${example} Franka::Franka examples_common Eigen3::Eigen3)
endforeach()

target_link_libraries(joint_impedance_control Threads::Threads)

include(GNUInstallDirs)
install(TARGETS ${EXAMPLES}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
