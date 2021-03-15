include_directories(
  ${GSTREAMER_INCLUDE_DIRS}
  ${GSTREAMER_VIDEO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GSTREAMER_RTSP_INCLUDE_DIRS}
)

set(RTMP_TO_RTSP_ENDPOINT_SOURCES
  rtmptortspendpoint.c
  gstrtmptortspendpoint.h
  gstrtmptortspendpoint.cpp
)

add_library(rtmptortspendpoint MODULE ${RTMP_TO_RTSP_ENDPOINT_SOURCES})
if(SANITIZERS_ENABLED)
  add_sanitizers(rtmptortspendpoint)
endif()

target_link_libraries(rtmptortspendpoint
  ${GSTREAMER_LIBRARIES}
  ${GSTREAMER_VIDEO_LIBRARIES}
  ${GSTREAMER_RTSP_LIBRARIES}
)

install(
  TARGETS rtmptortspendpoint
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_GST_PLUGINS_DIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
