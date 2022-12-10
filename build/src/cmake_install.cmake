# Install script for directory: /home/robocaster/myData/Robotics/libs_and_tools/DGM/dgm_franka/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdgm_fr3_dyn.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdgm_fr3_dyn.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdgm_fr3_dyn.so"
         RPATH "/usr/local/lib:/usr/local/lib/dynamic_graph_plugins:/opt/ros/foxy/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/mim_msgs/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/shared_memory/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/real_time_tools/lib:/usr/local/lib/dynamic-graph-plugins:/opt/openrobots/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/robocaster/myData/Robotics/libs_and_tools/DGM/dgm_franka/build/src/libdgm_fr3_dyn.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdgm_fr3_dyn.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdgm_fr3_dyn.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdgm_fr3_dyn.so"
         OLD_RPATH "/opt/ros/foxy/lib:/usr/local/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/mim_msgs/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/shared_memory/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/real_time_tools/lib:/usr/local/lib/dynamic-graph-plugins:/opt/openrobots/lib:::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/usr/local/lib:/usr/local/lib/dynamic_graph_plugins:/opt/ros/foxy/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/mim_msgs/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/shared_memory/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/real_time_tools/lib:/usr/local/lib/dynamic-graph-plugins:/opt/openrobots/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdgm_fr3_dyn.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main"
         RPATH "/usr/local/lib:/usr/local/lib/dynamic_graph_plugins:/opt/ros/foxy/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/mim_msgs/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/shared_memory/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/real_time_tools/lib:/usr/local/lib/dynamic-graph-plugins:/opt/openrobots/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn" TYPE EXECUTABLE FILES "/home/robocaster/myData/Robotics/libs_and_tools/DGM/dgm_franka/build/src/dgm_fr3_dyn_main")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main"
         OLD_RPATH "/home/robocaster/myData/Robotics/libs_and_tools/DGM/dgm_franka/build/src:/opt/ros/foxy/lib:/usr/local/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/mim_msgs/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/shared_memory/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/real_time_tools/lib:/usr/local/lib/dynamic-graph-plugins:/opt/openrobots/lib:"
         NEW_RPATH "/usr/local/lib:/usr/local/lib/dynamic_graph_plugins:/opt/ros/foxy/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/mim_msgs/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/shared_memory/lib:/home/robocaster/myData/Robotics/projects/manipulation/dg_head/workspace/install/real_time_tools/lib:/usr/local/lib/dynamic-graph-plugins:/opt/openrobots/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/dgm_fr3_dyn/dgm_fr3_dyn_main")
    endif()
  endif()
endif()

