#include <glib.h>
// #include <config.h>
#include "qof.h"

#ifndef FILES_H
#define FILES_H

QofSession* post_file_open (char *filename);
void gnc_window_show_progress (const char *message, double percentage);

#endif
