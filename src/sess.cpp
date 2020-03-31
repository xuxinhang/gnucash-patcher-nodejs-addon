// #include "gnc-backend-xml.h"
// #include "Split.h"
#include <config.h>
#include "common.h"
#include <glib.h>
#include "gnc-uri-utils.h"
#include "qof.h"
#include "qofbook.h"
#include "gnc-backend-xml.h"
#include "gnc-engine.h"
#include "Account.h"
#include "sess.hpp"
#include <iostream>
#include <list>
#include <map>
#include <string>

using namespace std;

void start_sess(const char *fname) {
  QofSession *sess;
  QofBackendError backend_err;
  char *newfile;

  gnc_module_init_backend_xml();
  gnc_engine_init(0, NULL);

  qof_event_suspend ();

  sess = qof_session_new();
  newfile = (char *)fname;
  // newfile = gnc_uri_normalize_uri(fname, true);
  qof_session_begin(sess, newfile, TRUE, FALSE, FALSE);

  if (backend_err = qof_session_get_error(sess)) {
    printf("Some error occured when beginning session. (%d)\n", backend_err);
    // return;
  }

  QofBook *book = nullptr;
  book = qof_session_get_book(sess);
  printf("File path: %s", qof_session_get_file_path(sess));

  // xaccLogDisable();
  qof_session_load(sess, sess_load_cb);
  // xaccLogEnable();

  if (backend_err = qof_session_get_error(sess)) {
    printf("Some error occured when loading session. (%d)\n", backend_err);
    // return;
  }

  // return;

  book = qof_session_get_book(sess);
  // be = qof_book_get_backend(book);

  Account *root_account = nullptr;
  root_account = gnc_book_get_root_account(book);

  cout
  << "Root account GUID: "
  << guid_to_string(xaccAccountGetGUID(root_account))
  << endl;

  // char **target_account_codes = "MEAL";

  // list<string> tacc_codes;
  // tacc_codes.push_back("")

  map <string, Account*> tacc_code_map;
  tacc_code_map["MEAL"] = gnc_account_lookup_by_code(root_account, "MEAL");
  // tacc_code_map["MEAL"] = gnc_account_lookup_by_code(root_account, "MEAL");
  // tacc_code_map["MEAL"] = gnc_account_lookup_by_code(root_account, "MEAL");

  cout
  << "MEAL's GUID: "
  << guid_to_string(xaccAccountGetGUID(tacc_code_map.at("MEAL")))
  << endl;

  qof_session_save(sess, [](const char *msg, double prec ) -> void {
    std::cout << "Saving (" << (int)prec << "): " << msg << std::endl;
  });

  qof_session_end(sess);
  qof_session_destroy(sess);
}

void sess_load_cb (const char *message, double percentage) {
  printf("Loading the file (%d): %s: \n", (int)percentage, message);
}
