#include "gstrtsptortpendpoint.h"

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>
#include <glib/gstdio.h>
#include <memory>
#include <string.h>

GST_DEBUG_CATEGORY_STATIC (gst_rtsp_to_rtp_endpoint_debug_category);
#define GST_CAT_DEFAULT gst_rtsp_to_rtp_endpoint_debug_category

#define PLUGIN_NAME "rtsptortpendpoint"

#define GST_RTSP_TO_RTP_ENDPOINT_GET_PRIVATE(obj) (                          \
    G_TYPE_INSTANCE_GET_PRIVATE (                                            \
        (obj),                                                               \
        GST_TYPE_RTSP_TO_RTP_ENDPOINT,                                       \
        GstRtspToRtpEndpointPrivate                                          \
    )                                                                        \
)

#define BASE_TIME_LOCK(obj) (                                                \
  g_mutex_lock (&GST_RTSP_TO_RTP_ENDPOINT(obj)->priv->base_time_mutex)       \
)

#define BASE_TIME_UNLOCK(obj) (                                              \
  g_mutex_unlock (&GST_RTSP_TO_RTP_ENDPOINT(obj)->priv->base_time_mutex)     \
)
enum
{
    PROP_0,
    PROP_IPCAMERA_URI,
    PROP_PORT,
    N_PROPERTIES
};

struct _GstRtspToRtpEndpointPrivate {
    GstElement *pipeline;
    GMutex base_time_mutex;

    gchar *camera_uri;
    gint port;

};

/* pad templates */

#define VIDEO_SRC_CAPS \
  GST_VIDEO_CAPS_MAKE ("{ BGRA }")

#define VIDEO_SINK_CAPS \
  GST_VIDEO_CAPS_MAKE ("{ BGRA }")

/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstRtspToRtpEndpoint, gst_rtsp_to_rtp_endpoint,
        GST_TYPE_VIDEO_FILTER,
        GST_DEBUG_CATEGORY_INIT (gst_rtsp_to_rtp_endpoint_debug_category,
                                 PLUGIN_NAME, 0,
                                 "debug category for rtsp_to_rtp_endpoint element"));

static void
gst_rtsp_to_rtp_endpoint_dispose (GObject *object)
{
    GstRtspToRtpEndpoint *self = GST_RTSP_TO_RTP_ENDPOINT(object);

    if(self->priv->pipeline != NULL)
    {
        GstBus *bus;

        bus = gst_pipeline_get_bus (GST_PIPELINE (self->priv->pipeline));
        g_object_unref (bus);

        gst_element_set_state (self->priv->pipeline, GST_STATE_NULL);
        gst_object_unref (GST_OBJECT (self->priv->pipeline));
        self->priv->pipeline = NULL;
    }

    /* clean up as possible. May be called multiple times */

    G_OBJECT_CLASS (gst_rtsp_to_rtp_endpoint_parent_class)->dispose (object);
}

static void
gst_rtsp_to_rtp_endpoint_finalize (GObject *object)
{

    GstRtspToRtpEndpoint *self = GST_RTSP_TO_RTP_ENDPOINT(object);

    GST_DEBUG_OBJECT (self, "finalize");

    g_mutex_clear (&self->priv->base_time_mutex);

    G_OBJECT_CLASS (gst_rtsp_to_rtp_endpoint_parent_class)->dispose (object);
}

static void
gst_rtsp_to_rtp_create_elements (GstRtspToRtpEndpoint *self)
{
    GstBus *bus;
    // GstMessage *msg;

    g_mutex_init (&self->priv->base_time_mutex);

    self->priv->pipeline = gst_parse_launch(
        "rtspsrc location=rtsp://195.46.114.132/live/ch00_0 ! rtph264depay ! rtph264pay config-interval=1 pt=96 ! udpsink host=127.0.0.1 port=6060 sync=false",
        nullptr);

    gst_element_set_state(self->priv->pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(self->priv->pipeline);

    // msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
    //                                  static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    g_print("rtp streaming started!!");
    // if (msg != nullptr)
    //     gst_message_unref(msg);

    gst_object_unref(bus);
}

static void
gst_rtsp_to_rtp_endpoint_set_property (GObject * object, guint property_id,
                                       const GValue * value, GParamSpec * pspec)
{
    GstRtspToRtpEndpoint *self = GST_RTSP_TO_RTP_ENDPOINT(object);

    switch (property_id) {
        case PROP_IPCAMERA_URI:
            self->priv->camera_uri = g_value_dup_string (value);
            break;
        case PROP_PORT:
            self->priv->port = g_value_get_int (value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }

    if(property_id == PROP_PORT) {
        gst_rtsp_to_rtp_create_elements(self);
    }
}

static void
gst_rtsp_to_rtp_endpoint_get_property (GObject * object, guint property_id,
                                       GValue * value, GParamSpec * pspec)
{
    GstRtspToRtpEndpoint *self = GST_RTSP_TO_RTP_ENDPOINT(object);

    switch (property_id) {
        case PROP_IPCAMERA_URI:
            g_value_set_string (value, self->priv->camera_uri);
            break;
        case PROP_PORT:
            g_value_set_int (value, self->priv->port);
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

static void
gst_rtsp_to_rtp_endpoint_init (GstRtspToRtpEndpoint *self)
{
    self->priv = GST_RTSP_TO_RTP_ENDPOINT_GET_PRIVATE (self);

    self->priv->camera_uri = NULL;
    self->priv->port = 0;
}

static void
gst_rtsp_to_rtp_endpoint_class_init (GstRtspToRtpEndpointClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, PLUGIN_NAME, 0, PLUGIN_NAME);

  gst_element_class_add_pad_template (GST_ELEMENT_CLASS (klass),
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gst_caps_from_string (VIDEO_SRC_CAPS)));
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS (klass),
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
          gst_caps_from_string (VIDEO_SINK_CAPS)));

    gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
                                           "rtsp to rtp element",
                                           "Video/Filter",
                                           "Converts rtsp uri to rtp stream",
                                           "Developer");

    gobject_class->set_property = gst_rtsp_to_rtp_endpoint_set_property;
    gobject_class->get_property = gst_rtsp_to_rtp_endpoint_get_property;
    gobject_class->finalize = gst_rtsp_to_rtp_endpoint_finalize;
    gobject_class->dispose = gst_rtsp_to_rtp_endpoint_dispose;

    klass->create_elements = gst_rtsp_to_rtp_create_elements;

    /* Properties initialization */
    g_object_class_install_property (gobject_class, PROP_IPCAMERA_URI,
                                     g_param_spec_string ("ipcamera-uri", "IP Camera URI",
                                     "set the uri of the ip camera", NULL, G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class, PROP_PORT,
                                     g_param_spec_int ("rtp-port", "rtp stream port",
                                     "rtp packets are received in this port", 0, G_MAXINT, 0,
                                     G_PARAM_READWRITE));

    /* Registers a private structure for the instantiatable type */
    g_type_class_add_private (klass, sizeof (GstRtspToRtpEndpointPrivate));
}

gboolean
gst_rtsp_to_rtp_endpoint_plugin_init (GstPlugin *plugin)
{
    return gst_element_register (plugin, PLUGIN_NAME, GST_RANK_NONE,
                                 GST_TYPE_RTSP_TO_RTP_ENDPOINT);
}
