/* test book patcher */
#include <stdio.h>

#include "config.h"
#include "qof.h"
#include "gnc-backend-xml.h"
#include "gnc-engine.h"
#include "Account.h"

#include "../book-patcher.hpp"
#include "../sess.hpp"
#include "test-utils.hpp"

// void sess_load_cb_2 (const char *message, double percentage);
// void patcher_init_gnc_modules_2();

int main() {
  printf("this good function loaded: %x\n", test_utils_unload_sample_file);

  pat_init_gnc_modules();

  QofSession *sess;
  test_utils_load_sample_file(&sess);

  QofBook* book = qof_session_get_book(sess);
  Account* root_acc = gnc_book_get_root_account(book);
  printf("The root account's GUID = %s\n", guid_to_string(xaccAccountGetGUID(root_acc)));

  // The begin of testing

  BookPatcher patcher(book);

  // initialize some values of a defined struct
  TransferRecord record;
  record.dateStr = (char*)"2020-02-24 08:30";
  record.amount = 460;
  record.denom = 100;
  record.description = (char*)"Enjoy a splendent dinner.";
  record.sourceAccount = (char*)"CAMPUS_CARD";
  record.targetAccount = (char*)"MEAL";
  record.timestamp = 1581002235002 / 1000; // in sec.

  patcher.addTransfer(&record);

  record.amount = 1320;
  record.description = (char*)"一次极好的校车乘坐体验";
  record.targetAccount = (char*)"TRAFFIC";
  record.timestamp = 1591003449;

  patcher.addTransfer(&record);

  // The end of testing

  test_utils_unload_sample_file(sess);
  printf("The test program has been execuated successfully.\n");
  return 0;
}

// int main2() {
//   QofSession *sess;
//   QofBackendError backend_err;
//   char *newfile;

//   patcher_init_gnc_modules_2();
//   // qof_event_suspend ();

//   sess = qof_session_new();
//   newfile = (char *)"file:///mnt/e/example.gnucash";
//   // newfile = gnc_uri_normalize_uri(fname, true);
//   qof_session_begin(sess, newfile, TRUE, FALSE, FALSE);

//   if (backend_err = qof_session_get_error(sess)) {
//     printf("Some error occured when beginning session. (%d)\n", backend_err);
//     // return;
//   }

//   QofBook *book = nullptr;
//   // book = qof_session_get_book(sess);
//   printf("File path: %s", qof_session_get_file_path(sess));

//   qof_session_load(sess, sess_load_cb_2);

//   if (backend_err = qof_session_get_error(sess)) {
//     printf("Some error occured when loading session. (%d)\n", backend_err);
//     // return;
//   }

//   book = qof_session_get_book(sess);
//   // be = qof_book_get_backend(book);

//   Account *root_account = nullptr;
//   root_account = gnc_book_get_root_account(book);

//   qof_session_end(sess);
//   qof_session_destroy(sess);
// }

// void sess_load_cb_2 (const char *message, double percentage) {
//   printf("Loading the file (%d): %s: \n", (int)percentage, message);
// }

// void patcher_init_gnc_modules_2 () {
//   gnc_module_init_backend_xml();
//   gnc_engine_init(0, NULL);
// }




