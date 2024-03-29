cmake_minimum_required(VERSION 3.10.2)
project(dgm_franka)

# Using C++17
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

# In order to quiet the find_package(dynamic-graph[-python] REQUIRED)
set(CHECK_MINIMAL_CXX_STANDARD ON)
set(Python_ADDITIONAL_VERSIONS 3.6)

# build dep
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(mpi_cmake_modules REQUIRED)
find_package(mim_msgs REQUIRED)

# dependencies 
find_package(lcm REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(dynamic_graph_manager REQUIRED)
find_package(yaml_utils REQUIRED)
find_package(Franka REQUIRED)

find_package(Eigen3 REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(real_time_tools REQUIRED)

# LAAS dependencies
find_package(dynamic-graph REQUIRED)
find_package(dynamic-graph-python REQUIRED)

# Export the dependencies.
ament_export_dependencies(
  ament_cmake
  ament_cmake_python
  mpi_cmake_modules
  mim_msgs
  ament_index_cpp
  dynamic_graph_manager
  yaml_utils
  real_time_tools
  lcm
  Eigen3
  dynamic-graph
  dynamic-graph-python) 

# prepare the final export
ament_export_interfaces(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

# Manage the creation of the libraries and executables.
add_subdirectory(src)

# Install the package includes.
install(DIRECTORY include/ DESTINATION include)

# Export as an ament_package
ament_package()