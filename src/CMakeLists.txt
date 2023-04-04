set(ALL_SOURCE_TARGETS)

macro(CREATE_DGM_LIBRARY ROBOT_NAME)
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
endmacro(CREATE_DGM_LIBRARY LIB_NAME)

# macro for building the executables
macro(CREATE_DGM_EXECUTABLE ROBOT_NAME)
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
endmacro(CREATE_DGM_EXECUTABLE ROBOT_NAME)

# For installing the shared memory config files
set(SM_CONFIG_SOURCE_DIR "${PROJECT_SOURCE_DIR}/shared_memory_configs")
set(SM_CONFIG_INSTALL_DESTINATION "share/${PROJECT_NAME}")

# Dynamic Interface
CREATE_DGM_LIBRARY(fr3_dyn)
CREATE_DGM_EXECUTABLE(fr3_dyn)
install(FILES ${SM_CONFIG_SOURCE_DIR}/franka_dynamic_interface.yaml 
        DESTINATION ${SM_CONFIG_INSTALL_DESTINATION})

# Kinematic Interface
CREATE_DGM_LIBRARY(fr3_kin)
CREATE_DGM_EXECUTABLE(fr3_kin)
install(FILES ${SM_CONFIG_SOURCE_DIR}/franka_kinematic_interface.yaml 
        DESTINATION ${SM_CONFIG_INSTALL_DESTINATION})

# install both the libraries and the executables
install(
  TARGETS ${ALL_SOURCE_TARGETS}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  INCLUDES
  DESTINATION include)