/* Autogenerated with kurento-module-creator */

#include <gst/gst.h>
#include "MediaPipeline.hpp"
#include "MediaPipelineImpl.hpp"
#include <FileToRtspEndpointImplFactory.hpp>
#include "FileToRtspEndpointImpl.hpp"
#include <jsonrpc/JsonSerializer.hpp>
#include <KurentoException.hpp>
#include "SignalHandler.hpp"

#define GST_CAT_DEFAULT kurento_file_to_rtsp_endpoint_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoFileToRtspEndpointImpl"
#define FACTORY_NAME "filetortspendpoint"

#define SET_FILE_URI "file-uri"
#define SET_RTSP_PORT "rtsp-port"

namespace kurento
{
namespace module
{
namespace filetortspendpoint
{

FileToRtspEndpointImpl::FileToRtspEndpointImpl (const boost::property_tree::ptree &config,
                                                std::shared_ptr<MediaPipeline> mediaPipeline,
                                                const std::string &fileUri, int port)
    : FilterImpl ( config, std::dynamic_pointer_cast<MediaPipelineImpl> (mediaPipeline) )
{
  GstElement* filePlugin;

  g_object_set (element, "filter-factory", FACTORY_NAME, NULL);

  g_object_get (G_OBJECT (element), "filter", &filePlugin, NULL);

  if (filePlugin == NULL) {
    throw KurentoException (MEDIA_OBJECT_NOT_AVAILABLE,
                            "Media Object not available");
  }

  if(fileUri != "") {
      g_object_set(G_OBJECT (filePlugin), SET_FILE_URI, fileUri.c_str(), NULL);
  }

  if(port != 0) {
      g_object_set(G_OBJECT (filePlugin), SET_RTSP_PORT, port, NULL);
  }

  g_object_unref (filePlugin);
}

MediaObjectImpl *
FileToRtspEndpointImplFactory::createObject (const boost::property_tree::ptree &config,
                                             std::shared_ptr<MediaPipeline> mediaPipeline,
                                             const std::string &fileUri, int port) const
{
  return new FileToRtspEndpointImpl (config, mediaPipeline, fileUri, port);
}

FileToRtspEndpointImpl::StaticConstructor FileToRtspEndpointImpl::staticConstructor;

FileToRtspEndpointImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* filetortspendpoint */
} /* module */
} /* kurento */
