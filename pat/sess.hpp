#include <config.h>
#include "qof.h"

#ifndef PAT_SESS_H
#define PAT_SESS_H

void start_sess(const char *fname);
void sess_load_cb(const char *message, double percentage);
void pat_init_gnc_modules();

#endif
