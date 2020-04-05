/* book patcher */

#ifndef PAT_BOOK_PATCHER_H
#define PAT_BOOK_PATCHER_H

#include <glib.h>

#include "gnc-date.h"
#include "qof.h"
#include "Account.h"

void init_gnc_modules();

typedef struct TransferRecord {
  time64 timestamp;
  char *description;
  char *sourceAccount; // account code
  char *targetAccount; // account code
  int amount;
  int denom;
  char reconcileState;
} TransferRecord;

class BookPatcher {
  public:
  QofBook *book;
  Account *root_account;

  BookPatcher(QofBook *init_book);
  bool addTransfer(TransferRecord *record);
};

#endif
