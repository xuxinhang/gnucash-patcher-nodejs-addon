#include <iterator>

#include <config.h>
#include "book-patcher.hpp"
#include "sess.hpp"

#include "common.h"
#include <glib.h>
#include "qof.h"
#include "gnc-engine.h"
#include "gnc-date.h"
#include "Account.h"
#include "Transaction.h"
#include "Split.h"
#include "gnc-commodity.h"
#include "gnc-backend-xml.h"


void init_gnc_modules () {
  gnc_module_init_backend_xml();
  gnc_engine_init(0, NULL);
}

BookPatcher::BookPatcher(QofBook *init_book) {
  book = init_book;
  root_account = gnc_book_get_root_account(book);
}

bool BookPatcher::addTransfer(TransferRecord *record) {
  Account *sourceAct, *targetAct;
  sourceAct = gnc_account_lookup_by_code(root_account, record->sourceAccount);
  targetAct = gnc_account_lookup_by_code(root_account, record->targetAccount);

  gnc_commodity *currency;
  currency = xaccAccountGetCommodity(targetAct);

  time64 timestamp = record->timestamp;
  gnc_numeric targetAmount = gnc_numeric_create(record->amount, record->denom);
  gnc_numeric sourceAmount = gnc_numeric_neg(targetAmount);

  Transaction *trans = xaccMallocTransaction(book);
  xaccTransBeginEdit(trans);

  xaccTransSetDatePostedSecsNormalized(trans, timestamp);
  xaccTransSetDateEnteredSecs(trans, timestamp);
  // xaccTransSetNum(trans, "Bla...");
  // xaccTransSetNotes(trans, record->description);
  xaccTransSetDescription(trans, record->description);
  xaccTransSetCurrency(trans, currency);

  // the split for the source account
  Split *sourceSplit = xaccMallocSplit(book);
  xaccTransAppendSplit(trans, sourceSplit);
  xaccAccountInsertSplit(sourceAct, sourceSplit);
  xaccSplitSetValue(sourceSplit, sourceAmount);
  xaccSplitSetAmount(sourceSplit, sourceAmount);
  xaccSplitSetReconcile(sourceSplit, record->reconcileState);

  // the split for the target account
  Split *targetSplit = xaccMallocSplit(book);
  xaccTransAppendSplit(trans, targetSplit);
  xaccAccountInsertSplit(targetAct, targetSplit);
  xaccSplitSetValue(targetSplit, targetAmount);
  xaccSplitSetAmount(targetSplit, targetAmount);
  xaccSplitSetReconcile(targetSplit, record->reconcileState);

  xaccTransCommitEdit(trans);

  return true;
}


