#include "config.h"
#include "qof.h"

#include "test-utils.hpp"
#include "../book-patcher.hpp"
#include "../sess.hpp"

#define SAMPLE_FILE "file:///mnt/e/example.gnucash"

bool test_utils_load_sample_file(QofSession **sess_p) {
  // patcher_init_gnc_modules();
  *sess_p = qof_session_new();
  qof_session_begin(*sess_p, SAMPLE_FILE, TRUE, FALSE, FALSE);
  qof_session_load(*sess_p, [](const char *msg, double perc) -> void { });

  return true;
}

bool test_utils_unload_sample_file(QofSession *sess) {
  qof_session_save(sess, [](const char *message, double percent) -> void { });
  qof_session_end(sess);
  qof_session_destroy(sess);

  return true;
}
