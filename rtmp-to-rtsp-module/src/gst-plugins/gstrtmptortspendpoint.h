#ifndef _GST_RTMP_TO_RTSP_ENDPOINT_H_
#define _GST_RTMP_TO_RTSP_ENDPOINT_H_

#include <gst/video/gstvideofilter.h>

G_BEGIN_DECLS

#define GST_TYPE_RTMP_TO_RTSP_ENDPOINT   (gst_rtmp_to_rtsp_endpoint_get_type())
#define GST_RTMP_TO_RTSP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_RTMP_TO_RTSP_ENDPOINT,GstRtmpToRtspEndpoint))
#define GST_RTMP_TO_RTSP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_RTMP_TO_RTSP_ENDPOINT,GstRtmpToRtspEndpointClass))
#define GST_IS_RTMP_TO_RTSP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_RTMP_TO_RTSP_ENDPOINT))
#define GST_IS_RTMP_TO_RTSP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_RTMP_TO_RTSP_ENDPOINT))
typedef struct _GstRtmpToRtspEndpoint GstRtmpToRtspEndpoint;
typedef struct _GstRtmpToRtspEndpointClass GstRtmpToRtspEndpointClass;
typedef struct _GstRtmpToRtspEndpointPrivate GstRtmpToRtspEndpointPrivate;

struct _GstRtmpToRtspEndpoint
{
  GstVideoFilter base;
  GstRtmpToRtspEndpointPrivate *priv;
};

struct _GstRtmpToRtspEndpointClass
{
  GstVideoFilterClass base_rtmp_to_rtsp_endpoint_class;

  /*Actions*/
  void (*create_elements) (GstRtmpToRtspEndpoint * self);
};

GType gst_rtmp_to_rtsp_endpoint_get_type (void);

gboolean gst_rtmp_to_rtsp_endpoint_plugin_init (GstPlugin *plugin);

G_END_DECLS

#endif /* _GST_RTMP_TO_RTSP_ENDPOINT_H_ */
