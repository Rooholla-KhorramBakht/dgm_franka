set(ALL_SOURCE_TARGETS)

macro(create_dgm_library ROBOT_NAME)
  set(LIB_NAME dgm_${ROBOT_NAME})

  # create library
  add_library(${LIB_NAME} SHARED dgm_${ROBOT_NAME}/${LIB_NAME}.cpp)

  # Add the include dependencies
  target_include_directories(${LIB_NAME} PUBLIC  
                             $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${LIB_NAME}>
                             $<INSTALL_INTERFACE:include/${LIB_NAME}>)

  # Link the dependencies.
  target_link_libraries(${LIB_NAME} ament_index_cpp::ament_index_cpp)
  target_link_libraries(${LIB_NAME} Franka::Franka Threads::Threads)
  target_link_libraries(${LIB_NAME} lcm)
  target_link_libraries(${LIB_NAME} dynamic_graph_manager::dynamic_graph_manager)
  target_link_libraries(${LIB_NAME} yaml_utils::yaml_utils)
  
  list(APPEND ALL_SOURCE_TARGETS ${LIB_NAME})
endmacro(create_dgm_library LIB_NAME)

# macro for building the executables
macro(create_dgm_executable ROBOT_NAME)
  set(EXECUTABLE_NAME dgm_${ROBOT_NAME}_main)

  # Robot executables.
  add_executable(${EXECUTABLE_NAME} dgm_${ROBOT_NAME}/dgm_${ROBOT_NAME}_main.cpp franka_trajectory_utils.cpp)
  # Add the include dependencies.
  target_include_directories(${EXECUTABLE_NAME} PUBLIC 
                             $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                             ${dynamic_graph_manager_INCLUDE_DIRS} 
                             $<INSTALL_INTERFACE:include>)
  
  # Link the dependencies.
  target_link_libraries(${EXECUTABLE_NAME} dgm_${ROBOT_NAME})

  list(APPEND ALL_SOURCE_TARGETS ${EXECUTABLE_NAME})
endmacro(create_dgm_executable ROBOT_NAME)

# For installing the shared memory config files
set(IF_CONFIG_SOURCE_DIR "${PROJECT_SOURCE_DIR}/interface_configs")
set(SM_CONFIG_INSTALL_DESTINATION "share/${PROJECT_NAME}")

# Dynamic Interface
create_dgm_library(fr3_joint_dyn)
create_dgm_executable(fr3_joint_dyn)
install(FILES ${IF_CONFIG_SOURCE_DIR}/fr3_joint_dyn_interface.yaml 
        DESTINATION ${SM_CONFIG_INSTALL_DESTINATION})

# Kinematic Interface
create_dgm_library(fr3_joint_kin)
create_dgm_executable(fr3_joint_kin)
install(FILES ${IF_CONFIG_SOURCE_DIR}/fr3_joint_kin_interface.yaml 
        DESTINATION ${SM_CONFIG_INSTALL_DESTINATION})

create_dgm_library(fr3_task_pose)
create_dgm_executable(fr3_task_pose)
install(FILES ${IF_CONFIG_SOURCE_DIR}/fr3_task_pose_interface.yaml 
        DESTINATION ${SM_CONFIG_INSTALL_DESTINATION})

create_dgm_library(fr3_task_vel)
create_dgm_executable(fr3_task_vel)
install(FILES ${IF_CONFIG_SOURCE_DIR}/fr3_task_vel_interface.yaml 
        DESTINATION ${SM_CONFIG_INSTALL_DESTINATION})

# install both the libraries and the executables
install(
  TARGETS ${ALL_SOURCE_TARGETS}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)
