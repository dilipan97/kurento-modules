cmake_minimum_required(VERSION 2.8)

project("rtmp-to-rtsp-endpoint")
message(STATUS "CMake project: ${PROJECT_NAME}")

message("If KurentoHelpers is not found, you need to install 'kms-cmake-utils' from the Kurento repository")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
find_package(KurentoHelpers REQUIRED)

message("If CodeGenerator is not found, you need to install 'kms-core' from the Kurento repository")
include(CodeGenerator)
get_values_from_model(PREFIX VALUE MODELS ${CMAKE_CURRENT_SOURCE_DIR}/src/server/interface KEYS version)

include(KurentoGitHelpers)
get_git_version(PROJECT_VERSION ${VALUE_VERSION})
message(STATUS "Project version: ${PROJECT_NAME}-${PROJECT_VERSION}")

# Compiler flags
include(CommonBuildFlags)
common_buildflags_set()
#common_buildflags_print()

# Development: Add here exceptions to the "Warnings are Errors" rule.
# Also, DOCUMENT WHY and always remove them as soon as the problem is fixed.
# For example:
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=unused-function")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=unused-variable")

string(REPLACE "." ";" VERSION_LIST ${PROJECT_VERSION})
list(GET VERSION_LIST 0 PROJECT_VERSION_MAJOR)
list(LENGTH VERSION_LIST _len)
if(${_len} GREATER 1)
  list(GET VERSION_LIST 1 PROJECT_VERSION_MINOR)
endif()
if(${_len} GREATER 2)
  list(GET VERSION_LIST 2 PROJECT_VERSION_PATCH)
endif()

set(GST_REQUIRED ^1.5.0)
set(GLIB_REQUIRED ^2.38)

find_package(PkgConfig)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.5>=${GST_REQUIRED})
pkg_check_modules(GSTREAMER_BASE REQUIRED gstreamer-base-1.5>=${GST_REQUIRED})
pkg_check_modules(GSTREAMER_VIDEO REQUIRED gstreamer-video-1.5>=${GST_REQUIRED})
pkg_check_modules(GSTREAMER_CHECK REQUIRED gstreamer-check-1.5>=${GST_REQUIRED})
pkg_check_modules(GSTREAMER_RTSP REQUIRED gstreamer-rtsp-server-1.0>=${GST_REQUIRED})
pkg_check_modules(KMSCORE REQUIRED kmscore)

# Generate file "config.h"
set(VERSION ${PROJECT_VERSION})
set(PACKAGE ${PROJECT_NAME})
set(GETTEXT_PACKAGE "rtmp-to-rtsp-endpoint")
include(GNUInstallDirs)
set(CMAKE_INSTALL_GST_PLUGINS_DIR ${CMAKE_INSTALL_LIBDIR}/gstreamer-1.5)
set(KURENTO_MODULES_SO_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${KURENTO_MODULES_DIR_INSTALL_PREFIX})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -DHAVE_CONFIG_H")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H")

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)
