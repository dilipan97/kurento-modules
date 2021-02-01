include_directories(
  ${GSTREAMER_INCLUDE_DIRS}
  ${GSTREAMER_VIDEO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(RTSP_TO_RTP_ENDPOINT_SOURCES
  rtsptortpendpoint.c
  gstrtsptortpendpoint.h
  gstrtsptortpendpoint.cpp
)

add_library(rtsptortpendpoint MODULE ${RTSP_TO_RTP_ENDPOINT_SOURCES})
if(SANITIZERS_ENABLED)
  add_sanitizers(rtsptortpendpoint)
endif()

target_link_libraries(rtsptortpendpoint
  ${GSTREAMER_LIBRARIES}
  ${GSTREAMER_VIDEO_LIBRARIES}
)

install(
  TARGETS rtsptortpendpoint
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_GST_PLUGINS_DIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
