#ifndef _GST_RTSP_TO_RTSP_ENDPOINT_H_
#define _GST_RTSP_TO_RTSP_ENDPOINT_H_

#include <gst/video/gstvideofilter.h>

G_BEGIN_DECLS

#define GST_TYPE_RTSP_TO_RTSP_ENDPOINT   (gst_rtsp_to_rtsp_endpoint_get_type())
#define GST_RTSP_TO_RTSP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_RTSP_TO_RTSP_ENDPOINT,GstRtspToRtspEndpoint))
#define GST_RTSP_TO_RTSP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_RTSP_TO_RTSP_ENDPOINT,GstRtspToRtspEndpointClass))
#define GST_IS_RTSP_TO_RTSP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_RTSP_TO_RTSP_ENDPOINT))
#define GST_IS_RTSP_TO_RTSP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_RTSP_TO_RTSP_ENDPOINT))
typedef struct _GstRtspToRtspEndpoint GstRtspToRtspEndpoint;
typedef struct _GstRtspToRtspEndpointClass GstRtspToRtspEndpointClass;
typedef struct _GstRtspToRtspEndpointPrivate GstRtspToRtspEndpointPrivate;

struct _GstRtspToRtspEndpoint
{
  GstVideoFilter base;
  GstRtspToRtspEndpointPrivate *priv;
};

struct _GstRtspToRtspEndpointClass
{
  GstVideoFilterClass base_rtsp_to_rtsp_endpoint_class;

  /*Actions*/
  void (*create_elements) (GstRtspToRtspEndpoint * self);
};

GType gst_rtsp_to_rtsp_endpoint_get_type (void);

gboolean gst_rtsp_to_rtsp_endpoint_plugin_init (GstPlugin *plugin);

G_END_DECLS

#endif /* _GST_RTSP_TO_RTSP_ENDPOINT_H_ */
