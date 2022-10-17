# Install script for directory: /work2/08571/yhkan/frontera/mercury/src/util

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
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libmercury_util.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/lib" TYPE STATIC_LIBRARY FILES "/work2/08571/yhkan/frontera/mercury/Testing/bin/libmercury_util.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/mercury_util_config.h;/usr/local/include/mercury_atomic.h;/usr/local/include/mercury_atomic_queue.h;/usr/local/include/mercury_byteswap.h;/usr/local/include/mercury_compiler_attributes.h;/usr/local/include/mercury_dlog.h;/usr/local/include/mercury_event.h;/usr/local/include/mercury_hash_string.h;/usr/local/include/mercury_hash_table.h;/usr/local/include/mercury_inet.h;/usr/local/include/mercury_list.h;/usr/local/include/mercury_log.h;/usr/local/include/mercury_mem.h;/usr/local/include/mercury_mem_pool.h;/usr/local/include/mercury_param.h;/usr/local/include/mercury_poll.h;/usr/local/include/mercury_queue.h;/usr/local/include/mercury_request.h;/usr/local/include/mercury_thread.h;/usr/local/include/mercury_thread_annotation.h;/usr/local/include/mercury_thread_condition.h;/usr/local/include/mercury_thread_mutex.h;/usr/local/include/mercury_thread_pool.h;/usr/local/include/mercury_thread_rwlock.h;/usr/local/include/mercury_thread_spin.h;/usr/local/include/mercury_time.h;/usr/local/include/mercury_util.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/include" TYPE FILE FILES
    "/work2/08571/yhkan/frontera/mercury/Testing/src/util/mercury_util_config.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_atomic.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_atomic_queue.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_byteswap.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_compiler_attributes.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_dlog.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_event.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_hash_string.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_hash_table.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_inet.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_list.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_log.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_mem.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_mem_pool.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_param.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_poll.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_queue.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_request.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_thread.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_thread_annotation.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_thread_condition.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_thread_mutex.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_thread_pool.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_thread_rwlock.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_thread_spin.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_time.h"
    "/work2/08571/yhkan/frontera/mercury/src/util/mercury_util.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/pkgconfig/mercury_util.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/lib/pkgconfig" TYPE FILE FILES "/work2/08571/yhkan/frontera/mercury/Testing/src/util/CMakeFiles/mercury_util.pc")
endif()

