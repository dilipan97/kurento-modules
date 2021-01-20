include_directories(
  ${GSTREAMER_INCLUDE_DIRS}
  ${GSTREAMER_VIDEO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${KmsGstCommons_INCLUDE_DIRS}
  ${GSTREAMER_RTSP_INCLUDE_DIRS}
)

set(FILE_TO_RTSP_ENDPOINT_SOURCES
  filetortspendpoint.c
  gstfiletortspendpoint.h
  gstfiletortspendpoint.cpp
)

add_library(filetortspendpoint MODULE ${FILE_TO_RTSP_ENDPOINT_SOURCES})
if(SANITIZERS_ENABLED)
  add_sanitizers(filetortspendpoint)
endif()

target_link_libraries(filetortspendpoint
  ${GSTREAMER_LIBRARIES}
  ${GSTREAMER_VIDEO_LIBRARIES}
  ${KmsGstCommons_LIBRARIES}
  ${GSTREAMER_RTSP_LIBRARIES}
)

install(
  TARGETS filetortspendpoint
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_GST_PLUGINS_DIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
