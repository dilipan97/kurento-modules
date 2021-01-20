#include <config.h>
#include <gst/gst.h>

#include "gstfiletortspendpoint.h"

static gboolean
init (GstPlugin *plugin)
{
  if (!gst_file_to_rtsp_endpoint_plugin_init (plugin))
    return FALSE;

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR,
    filetortspendpoint,
    "Convert File to RTSP",
    init, VERSION, GST_LICENSE_UNKNOWN, "PACKAGE_NAME", "origin")
