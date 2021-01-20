#ifndef _GST_FILE_TO_RTSP_ENDPOINT_H_
#define _GST_FILE_TO_RTSP_ENDPOINT_H_

#include <gst/video/gstvideofilter.h>

G_BEGIN_DECLS

#define GST_TYPE_FILE_TO_RTSP_ENDPOINT   (gst_file_to_rtsp_endpoint_get_type())
#define GST_FILE_TO_RTSP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_FILE_TO_RTSP_ENDPOINT,GstFileToRtspEndpoint))
#define GST_FILE_TO_RTSP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_FILE_TO_RTSP_ENDPOINT,GstFileToRtspEndpointClass))
#define GST_IS_FILE_TO_RTSP_ENDPOINT(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_FILE_TO_RTSP_ENDPOINT))
#define GST_IS_FILE_TO_RTSP_ENDPOINT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_FILE_TO_RTSP_ENDPOINT))
typedef struct _GstFileToRtspEndpoint GstFileToRtspEndpoint;
typedef struct _GstFileToRtspEndpointClass GstFileToRtspEndpointClass;
typedef struct _GstFileToRtspEndpointPrivate GstFileToRtspEndpointPrivate;

struct _GstFileToRtspEndpoint
{
  GstVideoFilter base;
  GstFileToRtspEndpointPrivate *priv;
};

struct _GstFileToRtspEndpointClass
{
  GstVideoFilterClass base_file_to_rtsp_endpoint_class;

    /*Actions*/
    void (*create_elements) (GstFileToRtspEndpoint * self);
};

GType gst_file_to_rtsp_endpoint_get_type (void);

gboolean gst_file_to_rtsp_endpoint_plugin_init (GstPlugin *plugin);

G_END_DECLS

#endif /* _GST_FILE_TO_RTSP_ENDPOINT_H_ */
