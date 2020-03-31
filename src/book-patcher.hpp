/* book patcher */

#ifndef PAT_BOOK_PATCHER_H
#define PAT_BOOK_PATCGER_H

#include <glib.h>
#include "gnc-date.h"

typedef struct TransferRecord {
  char *dateStr;
  time64 timestamp;
  char *description;
  char *sourceAccount; // account code
  char *targetAccount; // account code
  int amount;
  int denom;
} TransferRecord;

class BookPatcher;


#endif
