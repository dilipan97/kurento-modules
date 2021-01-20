/* Autogenerated with kurento-module-creator */

#ifndef __RTSP_TO_RTSP_ENDPOINT_IMPL_HPP__
#define __RTSP_TO_RTSP_ENDPOINT_IMPL_HPP__

#include "FilterImpl.hpp"
#include "RtspToRtspEndpoint.hpp"
#include <EventHandler.hpp>
#include <boost/property_tree/ptree.hpp>
#include <functional>

namespace kurento
{
namespace module
{
namespace rtsptortspendpoint
{
class RtspToRtspEndpointImpl;
} /* rtsptortspendpoint */
} /* module */
} /* kurento */

namespace kurento
{
void Serialize (std::shared_ptr<kurento::module::rtsptortspendpoint::RtspToRtspEndpointImpl> &object, JsonSerializer &serializer);
} /* kurento */

namespace kurento
{
class MediaPipelineImpl;
} /* kurento */

namespace kurento
{
namespace module
{
namespace rtsptortspendpoint
{

class RtspToRtspEndpointImpl : public FilterImpl, public virtual RtspToRtspEndpoint
{

public:

  RtspToRtspEndpointImpl (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline,
                          const std::string &ipcamUri, int port);

  virtual ~RtspToRtspEndpointImpl() = default;

  /* Next methods are automatically implemented by code generator */
  virtual bool connect (const std::string &eventType, std::shared_ptr<EventHandler> handler);
  virtual void invoke (std::shared_ptr<MediaObjectImpl> obj,
                       const std::string &methodName, const Json::Value &params,
                       Json::Value &response);

  virtual void Serialize (JsonSerializer &serializer);

private:

  class StaticConstructor
  {
  public:
    StaticConstructor();
  };

  static StaticConstructor staticConstructor;

};

} /* rtsptortspendpoint */
} /* module */
} /* kurento */

#endif /*  __RTSP_TO_RTSP_ENDPOINT_IMPL_HPP__ */
