#include "gstrtsptortspendpoint.h"

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/rtsp-server/rtsp-media-factory-uri.h>
#include <glib/gstdio.h>
#include <memory>
#include <commons/kmsloop.h>

GST_DEBUG_CATEGORY_STATIC (gst_rtsp_to_rtsp_endpoint_debug_category);
#define GST_CAT_DEFAULT gst_rtsp_to_rtsp_endpoint_debug_category

#define PLUGIN_NAME "rtsptortspendpoint"

#define GST_RTSP_TO_RTSP_ENDPOINT_GET_PRIVATE(obj) (                          \
    G_TYPE_INSTANCE_GET_PRIVATE (                                              \
        (obj),                                                                 \
        GST_TYPE_RTSP_TO_RTSP_ENDPOINT,                                       \
        GstRtspToRtspEndpointPrivate                                                 \
    )                                                                          \
)

#define BASE_TIME_LOCK(obj) (                                                \
  g_mutex_lock (&GST_RTSP_TO_RTSP_ENDPOINT(obj)->priv->base_time_mutex)       \
)

#define BASE_TIME_UNLOCK(obj) (                                              \
  g_mutex_unlock (&GST_RTSP_TO_RTSP_ENDPOINT(obj)->priv->base_time_mutex)     \
)
enum
{
    PROP_0,
    PROP_IPCAMERA_URI,
    PROP_PORT,
    N_PROPERTIES
};

struct _GstRtspToRtspEndpointPrivate {
    KmsLoop *loop;
    GstRTSPServer *server;
    GstRTSPMediaFactory *factory;
    gint gst_server_id;
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

G_DEFINE_TYPE_WITH_CODE (GstRtspToRtspEndpoint, gst_rtsp_to_rtsp_endpoint,
    GST_TYPE_VIDEO_FILTER,
    GST_DEBUG_CATEGORY_INIT (gst_rtsp_to_rtsp_endpoint_debug_category,
        PLUGIN_NAME, 0,
        "debug category for rtsp_to_rtsp_endpoint element"));

static GstRTSPFilterResult
client_filter (GstRTSPServer * server, GstRTSPClient * client,
               gpointer user_data)
{
    /* Simple filter that shuts down all clients. */
    return GST_RTSP_FILTER_REMOVE;
}

static void
pool_cleanup (GstRTSPServer * server)
{
    GstRTSPSessionPool *pool;
    pool = gst_rtsp_server_get_session_pool (server);
    gst_rtsp_session_pool_cleanup (pool);
    g_object_unref (pool);
}

static void
remove_map (GstRTSPServer * server)
{
    GstRTSPMountPoints *mounts;
    mounts = gst_rtsp_server_get_mount_points (server);
    gst_rtsp_mount_points_remove_factory (mounts, "/stream");
    g_object_unref (mounts);
}

static void
gst_rtsp_to_rtsp_endpoint_dispose (GObject *object)
{
    GstRtspToRtspEndpoint *self = GST_RTSP_TO_RTSP_ENDPOINT(object);

    g_source_remove(self->priv->gst_server_id);

    pool_cleanup(self->priv->server);
    remove_map(self->priv->server);

    g_clear_object(&self->priv->loop);
    g_clear_object(&self->priv->factory);

    gst_rtsp_server_client_filter (self->priv->server, client_filter, NULL);

    if (G_IS_OBJECT(self->priv->server))
    {
        g_object_unref(self->priv->server);
        self->priv->server = nullptr;
    }

    g_free(self->priv->camera_uri);

    /* clean up as possible. May be called multiple times */

    G_OBJECT_CLASS (gst_rtsp_to_rtsp_endpoint_parent_class)->dispose (object);
}

static void
gst_rtsp_to_rtsp_endpoint_finalize (GObject *object)
{
    GstRtspToRtspEndpoint *self = GST_RTSP_TO_RTSP_ENDPOINT(object);

    GST_DEBUG_OBJECT (self, "finalize");

    g_mutex_clear (&self->priv->base_time_mutex);

    G_OBJECT_CLASS (gst_rtsp_to_rtsp_endpoint_parent_class)->dispose (object);
}


static void
gst_rtsp_tp_rtsp_create_elements (GstRtspToRtspEndpoint *self)
{

    GstRTSPMountPoints *mounts;
    gchar *port;
    gchar *element;

    g_mutex_init (&self->priv->base_time_mutex);

    self->priv->loop = kms_loop_new();

    self->priv->server = gst_rtsp_server_new();

    mounts = gst_rtsp_server_get_mount_points(self->priv->server);

    port = (gchar *) g_malloc (10);
    g_sprintf(port, "%d", self->priv->port);

    element = g_strdup_printf ("('rtspsrc location=%s ! rtph264depay ! rtph264pay " 
                               "config-interval=1 pt=96 name=pay0 ')", self->priv->camera_uri);

    gst_rtsp_server_set_address(self->priv->server, "0.0.0.0");
    gst_rtsp_server_set_service(self->priv->server, port);

    // self->priv->factory = gst_rtsp_media_factory_uri_new();

    // gst_rtsp_media_factory_uri_set_uri(self->priv->factory, self->priv->camera_uri);

    self->priv->factory = gst_rtsp_media_factory_new();

    gst_rtsp_media_factory_set_launch(self->priv->factory, element);

    gst_rtsp_media_factory_set_shared(GST_RTSP_MEDIA_FACTORY(self->priv->factory), TRUE);

    gst_rtsp_mount_points_add_factory(mounts, "/stream",
                                      GST_RTSP_MEDIA_FACTORY(self->priv->factory));

    g_object_unref(mounts);

    if ((self->priv->gst_server_id=gst_rtsp_server_attach(self->priv->server, NULL)) == 0) {
        g_print("failed to attach the server\n");
        return;
    }

    g_print("stream ready at rtsp://127.0.0.1:%s/stream\n", port);
    g_free(port);
    g_free(element);
}

static void
gst_rtsp_to_rtsp_endpoint_set_property (GObject * object, guint property_id,
                                       const GValue * value, GParamSpec * pspec)
{
    GstRtspToRtspEndpoint *self = GST_RTSP_TO_RTSP_ENDPOINT(object);

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
        gst_rtsp_tp_rtsp_create_elements(self);
    }
}

static void
gst_rtsp_to_rtsp_endpoint_get_property (GObject * object, guint property_id,
                                       GValue * value, GParamSpec * pspec)
{
    GstRtspToRtspEndpoint *self = GST_RTSP_TO_RTSP_ENDPOINT(object);

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
gst_rtsp_to_rtsp_endpoint_init (GstRtspToRtspEndpoint *self)
{
    self->priv = GST_RTSP_TO_RTSP_ENDPOINT_GET_PRIVATE (self);

    self->priv->camera_uri = NULL;
    self->priv->port = 0;

}

static void
gst_rtsp_to_rtsp_endpoint_class_init (GstRtspToRtspEndpointClass *klass)
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
      "rtsp to rtsp server",
      "Video/Filter",
      "Filter doc",
      "Developer");

  gobject_class->set_property = gst_rtsp_to_rtsp_endpoint_set_property;
  gobject_class->get_property = gst_rtsp_to_rtsp_endpoint_get_property;
  gobject_class->finalize = gst_rtsp_to_rtsp_endpoint_finalize;
  gobject_class->dispose = gst_rtsp_to_rtsp_endpoint_dispose;

  klass->create_elements = gst_rtsp_tp_rtsp_create_elements;

  /* Properties initialization */
  g_object_class_install_property (gobject_class, PROP_IPCAMERA_URI,
                                   g_param_spec_string ("ipcamera-uri", "IP Camera URI",
                                                        "set the uri of the ip camera", NULL, G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_PORT,
                                   g_param_spec_int ("rtsp-port", "rtp stream port",
                                                    "rtp packets are received in this port", 0, G_MAXINT, 0,
                                                     G_PARAM_READWRITE));

  /* Registers a private structure for the instantiatable type */
   g_type_class_add_private (klass, sizeof (GstRtspToRtspEndpointPrivate));
}

gboolean
gst_rtsp_to_rtsp_endpoint_plugin_init (GstPlugin *plugin)
{
  return gst_element_register (plugin, PLUGIN_NAME, GST_RANK_NONE,
      GST_TYPE_RTSP_TO_RTSP_ENDPOINT);
}
