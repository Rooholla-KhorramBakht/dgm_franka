#
# Copyright (c) 2019, New York University and Max Planck Gesellschaft.
#
# License BSD-3 clause
#

set(all_src_targets)

macro(create_dgm_library robot_name)
  set(lib_name dgm_${robot_name})

  # create library
  add_library(${lib_name} SHARED dgm_${robot_name}/${lib_name}.cpp)
  # Add the include dependencies

#     target_include_directories(${lib_name} PUBLIC  
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
#     $<INSTALL_INTERFACE:include>  # <prefix>/include/mylib
# )

target_include_directories(${lib_name} PUBLIC  
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${lib_name}>
      $<INSTALL_INTERFACE:include/${lib_name}>
)

  # Link the dependencies.
  target_link_libraries(${lib_name} ament_index_cpp::ament_index_cpp)
  target_link_libraries(${lib_name} Franka::Franka Threads::Threads)
  target_link_libraries(${lib_name} lcm)
  target_link_libraries(${lib_name} dynamic_graph_manager::dynamic_graph_manager)
  target_link_libraries(${lib_name} yaml_utils::yaml_utils)
  
  list(APPEND all_src_targets ${lib_name})
endmacro(create_dgm_library lib_name)


#
# Declare a simple macro to build the demos.
#
macro(create_dgm_executable robot_name)
  set(executable_name dgm_${robot_name}_main)
  # Robot executables.
  add_executable(${executable_name} dgm_${robot_name}/dgm_${robot_name}_main.cpp 
                                    franka_trajectory_utils.cpp)
  # Add the include dependencies.
  target_include_directories(
    ${executable_name}
    PUBLIC 
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      ${dynamic_graph_manager_INCLUDE_DIRS} $<INSTALL_INTERFACE:include>)
  # Link the dependencies.
  target_link_libraries(${executable_name} dgm_${robot_name})
  # Some compile definitions
#   string(
#     CONCAT dgm_yaml_path 
#     "${PythonModules_robot_properties_kuka_PATH}/robot_properties_kuka/"
#         "robot_properties_kuka/dynamic_graph_manager/dgm_parameters_iiwa.yaml"
#     )
#   target_compile_definitions(
#     ${executable_name} 
#     PUBLIC 
#       YAML_PARAMS="${dgm_yaml_path}")
  list(APPEND all_src_targets ${executable_name})
endmacro(create_dgm_executable robot_name)

#
# build the DGMs and the executable
#

#Dynamic Interface
create_dgm_library(fr3_dyn)
create_dgm_executable(fr3_dyn)

#Kinematic Interface
create_dgm_library(fr3_kin)
create_dgm_executable(fr3_kin)

install(
  TARGETS ${all_src_targets}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  INCLUDES
  DESTINATION include)