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

