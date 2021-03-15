#include <config.h>
#include <gst/gst.h>

#include "gstrtmptortspendpoint.h"

static gboolean
init (GstPlugin *plugin)
{
  if (!gst_rtmp_to_rtsp_endpoint_plugin_init (plugin))
    return FALSE;

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR,
    rtmptortspendpoint,
    "Convert RTMP to RTSP",
    init, VERSION, GST_LICENSE_UNKNOWN, "PACKAGE_NAME", "origin")
