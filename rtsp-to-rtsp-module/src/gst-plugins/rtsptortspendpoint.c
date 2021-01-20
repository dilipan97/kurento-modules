#include <config.h>
#include <gst/gst.h>

#include "gstrtsptortspendpoint.h"

static gboolean
init (GstPlugin *plugin)
{
  if (!gst_rtsp_to_rtsp_endpoint_plugin_init (plugin))
    return FALSE;

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR,
    rtsptortspendpoint,
    "Convert RTSP to RTSP",
    init, VERSION, GST_LICENSE_UNKNOWN, "PACKAGE_NAME", "origin")
