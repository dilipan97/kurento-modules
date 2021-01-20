#ifndef _GST_RTSP_TO_RTP_ENDPOINT_H_
#define _GST_RTSP_TO_RTP_ENDPOINT_H_

#include <gst/video/gstvideofilter.h>

G_BEGIN_DECLS

#define GST_TYPE_RTSP_TO_RTP_ENDPOINT   (gst_rtsp_to_rtp_endpoint_get_type())
#define GST_RTSP_TO_RTP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_RTSP_TO_RTP_ENDPOINT,GstRtspToRtpEndpoint))
#define GST_RTSP_TO_RTP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_RTSP_TO_RTP_ENDPOINT,GstRtspToRtpEndpointClass))
#define GST_IS_RTSP_TO_RTP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_RTSP_TO_RTP_ENDPOINT))
#define GST_IS_RTSP_TO_RTP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_RTSP_TO_RTP_ENDPOINT))
typedef struct _GstRtspToRtpEndpoint GstRtspToRtpEndpoint;
typedef struct _GstRtspToRtpEndpointClass GstRtspToRtpEndpointClass;
typedef struct _GstRtspToRtpEndpointPrivate GstRtspToRtpEndpointPrivate;

struct _GstRtspToRtpEndpoint
{
  GstVideoFilter base;
  GstRtspToRtpEndpointPrivate *priv;
};

struct _GstRtspToRtpEndpointClass
{
  GstVideoFilterClass base_rtsp_to_rtp_endpoint_class;

  /*Actions*/
  void (*create_elements) (GstRtspToRtpEndpoint * self);

};

GType gst_rtsp_to_rtp_endpoint_get_type (void);

gboolean gst_rtsp_to_rtp_endpoint_plugin_init (GstPlugin *plugin);

G_END_DECLS

#endif /* _GST_RTSP_TO_RTP_ENDPOINT_H_ */
