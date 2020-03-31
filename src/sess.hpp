#include <config.h>
#include "qof.h"

#ifndef SESS_H
#define SESS_H

void start_sess(const char *fname);
void sess_load_cb (const char *message, double percentage);
void patcher_init_gnc_modules();

#endif
