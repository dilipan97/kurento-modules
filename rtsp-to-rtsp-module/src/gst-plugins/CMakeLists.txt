include_directories(
  ${GSTREAMER_INCLUDE_DIRS}
  ${GSTREAMER_VIDEO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GSTREAMER_RTSP_INCLUDE_DIRS}
)

set(RTSP_TO_RTSP_ENDPOINT_SOURCES
  rtsptortspendpoint.c
  gstrtsptortspendpoint.h
  gstrtsptortspendpoint.cpp
)

add_library(rtsptortspendpoint MODULE ${RTSP_TO_RTSP_ENDPOINT_SOURCES})
if(SANITIZERS_ENABLED)
  add_sanitizers(rtsptortspendpoint)
endif()

target_link_libraries(rtsptortspendpoint
  ${GSTREAMER_LIBRARIES}
  ${GSTREAMER_VIDEO_LIBRARIES}
  ${GSTREAMER_RTSP_LIBRARIES}
)

install(
  TARGETS rtsptortspendpoint
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_GST_PLUGINS_DIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
