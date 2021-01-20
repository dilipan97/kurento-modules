#include <config.h>
#include <gst/gst.h>

#include "gstrtsptortpendpoint.h"

static gboolean
init (GstPlugin *plugin)
{
  if (!gst_rtsp_to_rtp_endpoint_plugin_init (plugin))
    return FALSE;

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR,
    rtsptortpendpoint,
    "Convert RTSP to RTP",
    init, VERSION, GST_LICENSE_UNKNOWN, "PACKAGE_NAME", "origin")
