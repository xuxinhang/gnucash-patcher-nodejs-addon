#include <stdio.h>
#include <config.h>
#include <glib.h>
#include "qof.h"
// #include "gnc-uri-utils.h"

#include "sess.hpp"
// #include "common.h"

int main () {
  const char * str = "Hello, CMake!\n";
  printf(str);
  const char *fname = "file:///mnt/e/example.gnucash";
  // printf("QOF=%d\n", qof_session_new);
  // #ifdef GNCURIUTILS_H_
  // printf("URI=%d\n", gnc_uri_create_uri);
  // printf("GNCURIUTILS_H_ defined.\n");
  // #endif

  start_sess(fname);

  // post_file_open((char *)fname);
  // printf("SESS_P = %x\n", my_session);

  return 0;
}


