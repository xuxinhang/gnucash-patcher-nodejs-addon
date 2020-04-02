
#include <config.h>
#include "book-patcher.hpp"
#include "common.h"
#include <glib.h>
#include "qof.h"
#include "gnc-engine.h"
#include "gnc-date.h"
#include "Account.h"
#include "Transaction.h"
#include "Split.h"
#include "gnc-commodity.h"


class BookPatcher {
  public:
  QofBook *book;
  Account *root_account;

  BookPatcher(QofBook *init_book) {
    book = init_book;
    root_account = gnc_book_get_root_account(book);
  }

  bool addTransfer(TransferRecord *record);
};

bool BookPatcher::addTransfer(TransferRecord *record) {
  Account *sourceAct, *targetAct;
  sourceAct = gnc_account_lookup_by_code(root_account, record->sourceAccount);
  targetAct = gnc_account_lookup_by_code(root_account, record->targetAccount);

  gnc_commodity *currency;
  currency = xaccAccountGetCommodity(targetAct);

  time64 timestamp = 0;
  gnc_numeric targetAmount = gnc_numeric_create(record->amount, record->denom);
  gnc_numeric sourceAmount = gnc_numeric_neg(targetAmount);

  Transaction *trans = xaccMallocTransaction(book);
  xaccTransBeginEdit(trans);

  // xaccTransSetDatePostedSecs(trans, );
  xaccTransSetDatePostedSecsNormalized(trans, timestamp);
  xaccTransSetDateEnteredSecs(trans, timestamp);
  // xaccTransSetNum(trans, "Bla...");
  // xaccTransSetNotes(trans, "Bla...");
  xaccTransSetCurrency(trans, currency);

  // the split for the source account
  Split *sourceSplit = xaccMallocSplit(book);
  xaccTransAppendSplit(trans, sourceSplit);
  xaccAccountInsertSplit(sourceAct, sourceSplit);
  xaccSplitSetValue(sourceSplit, sourceAmount);
  xaccSplitSetAmount(sourceSplit, sourceAmount);

  // the split for the target account
  Split *targetSplit = xaccMallocSplit(book);
  xaccTransAppendSplit(trans, targetSplit);
  xaccAccountInsertSplit(targetAct, targetSplit);
  xaccSplitSetValue(targetSplit, targetAmount);
  xaccSplitSetAmount(targetSplit, targetAmount);

  xaccTransCommitEdit(trans);

  return true;
}


