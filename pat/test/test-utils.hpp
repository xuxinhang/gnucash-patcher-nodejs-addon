#ifndef PAT_TEST_UTILS_H
#define PAT_TEST_UTILS_H

#include "qof.h"

bool test_utils_load_sample_file(QofSession **sess_p);

bool test_utils_unload_sample_file(QofSession *sess);

#endif