#include <config.h>
#include "gnc-module.h"

// #include <gtk/gtk.h>
// #include <glib/gi18n.h>
#include <errno.h>
#include <string.h>

// #include "dialog-utils.h"
// #include "assistant-xml-encoding.h"
#include "gnc-commodity.h"
// #include "gnc-component-manager.h"
#include "gnc-engine.h"
#include "Account.h"
// #include "gnc-file.h"
#include "gnc-features.h"
#include "gnc-filepath-utils.h"
// #include "gnc-gui-query.h"
#include "gnc-hooks.h"
// #include "gnc-keyring.h"
// #include "gnc-splash.h"
// #include "gnc-ui.h"
// #include "gnc-ui-util.h"
#include "gnc-uri-utils.h"
// #include "gnc-window.h"
// #include "gnc-plugin-file-history.h"
#include "qof.h"
#include "Scrub.h"
#include "TransLog.h"
#include "gnc-session.h"
// #include "gnc-state.h"
// #include "gnc-autosave.h"
// #include <gnc-sx-instance-model.h>
#include <SX-book.h>

#include "files.hpp"
#include "gnc-backend-xml.h"
#include "gnc-engine.h"

QofSession* post_file_open (char *filename)
{
  gboolean is_readonly = TRUE;
  char *newfile = filename;
  // gnc_module_system_init();
  // libgncmod_engine_gnc_module_init();
  gnc_module_init_backend_xml();
  gnc_engine_init(0, NULL);
  // libgncmod_app_utils_gnc_module_init();

  QofSession *current_session, *new_session;
  QofBook *new_book;
  GList *invalid_account_names;
  gboolean uh_oh = FALSE;
  QofBackendError io_err = ERR_BACKEND_NO_ERR;

  /* load the accounts from the users datafile */
  /* but first, check to make sure we've got a session going. */
  new_session = qof_session_new ();

  // Begin the new session. If we are in read-only mode, ignore the locks.
  qof_session_begin (new_session, newfile, is_readonly, FALSE, FALSE);
  io_err = qof_session_get_error (new_session);

  if (ERR_BACKEND_BAD_URL == io_err)
  {
    printf("ERR_BACKEND_BAD_URL");
  }

  /* if file appears to be locked, ask the user ... */
  else if (ERR_BACKEND_LOCKED == io_err || ERR_BACKEND_READONLY == io_err)
  {
    printf("ERR");
      // GtkWidget *dialog;
      // gchar *displayname = NULL;

      // char *fmt1 = _("GnuCash could not obtain the lock for %s.");
      // char *fmt2 = ((ERR_BACKEND_LOCKED == io_err) ?
      //               _("That database may be in use by another user, "
      //                 "in which case you should not open the database. "
      //                 "What would you like to do?") :
      //               _("That database may be on a read-only file system, "
      //                 "you may not have write permission for the directory, "
      //                 "or your anti-virus software is preventing this action. "
      //                 "If you proceed you may not be able to save any changes. "
      //                 "What would you like to do?")
      //               );
      // int rc;

      // dialog = gtk_message_dialog_new(parent,
      //                                 0,
      //                                 GTK_MESSAGE_WARNING,
      //                                 GTK_BUTTONS_NONE,
      //                                 fmt1, displayname);
      // gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
      //         "%s", fmt2);
      // gtk_window_set_skip_taskbar_hint(GTK_WINDOW(dialog), FALSE);

      // gnc_gtk_dialog_add_button(dialog, _("_Open Read-Only"),
      //                           "document-revert", RESPONSE_READONLY);
      // gnc_gtk_dialog_add_button(dialog, _("_Create New File"),
      //                           "document-new", RESPONSE_NEW);
      // gnc_gtk_dialog_add_button(dialog, _("Open _Anyway"),
      //                           "document-open", RESPONSE_OPEN);
      // if (shutdown_cb)
      //     gtk_dialog_add_button(GTK_DIALOG(dialog),
      //                           _("_Quit"), RESPONSE_QUIT);
      // rc = gtk_dialog_run(GTK_DIALOG(dialog));
      // gtk_widget_destroy(dialog);
      // g_free (displayname);

      // if (rc == GTK_RESPONSE_DELETE_EVENT)
      // {
      //     rc = shutdown_cb ? RESPONSE_QUIT : RESPONSE_NEW;
      // }
      // switch (rc)
      // {
      // case RESPONSE_QUIT:
      //     if (shutdown_cb)
      //         shutdown_cb(0);
      //     g_assert(1);
      //     break;
      // case RESPONSE_READONLY:
      //     is_readonly = TRUE;
      //     /* user told us to open readonly. We do ignore locks (just as before), but now also force the opening. */
      //     qof_session_begin (new_session, newfile, is_readonly, FALSE, TRUE);
      //     break;
      // case RESPONSE_OPEN:
      //     /* user told us to ignore locks. So ignore them. */
      //     qof_session_begin (new_session, newfile, TRUE, FALSE, FALSE);
      //     break;
      // default:
      //     /* Can't use the given file, so just create a new
      //       * database so that the user will get a window that
      //       * they can click "Exit" on.
      //       */
      //     gnc_file_new (parent);
      //     break;
      // }
  }
  /* if the database doesn't exist, ask the user ... */
  else if ((ERR_BACKEND_NO_SUCH_DB == io_err))
  {
      // if (!show_session_error (parent, io_err, newfile, GNC_FILE_DIALOG_OPEN))
      // {
      //     /* user told us to create a new database. Do it. We
      //               * shouldn't have to worry about locking or clobbering,
      //               * it's supposed to be new. */
      //     qof_session_begin (new_session, newfile, FALSE, TRUE, FALSE);
      // }
  }

  /* Check for errors again, since above may have cleared the lock.
    * If its still locked, still, doesn't exist, still too old, then
    * don't bother with the message, just die. */
  io_err = qof_session_get_error (new_session);
  if ((ERR_BACKEND_LOCKED == io_err) ||
          (ERR_BACKEND_READONLY == io_err) ||
          (ERR_BACKEND_NO_SUCH_DB == io_err))
  {
      uh_oh = TRUE;
  }

  else
  {
      // uh_oh = show_session_error (parent, io_err, newfile, GNC_FILE_DIALOG_OPEN);
  }

  if (!uh_oh)
  {
      Account *new_root;

      /* If the new "file" is a database, attempt to store the password
        * in a keyring. GnuCash itself will not save it.
        */
      // if ( !gnc_uri_is_file_scheme (scheme))
      //     gnc_keyring_set_password ( scheme, hostname, port,
      //                                 path, username, password );

      // xaccLogDisable();
      // gnc_window_show_progress(_("Loading user data..."), 0.0);
      qof_session_load (new_session, gnc_window_show_progress);
      // gnc_window_show_progress(NULL, -1.0);
      // xaccLogEnable();

      if (is_readonly)
      {
          // If the user chose "open read-only" above, make sure to have this
          // read-only here.
          qof_book_mark_readonly(qof_session_get_book(new_session));
      }

      /* check for i/o error, put up appropriate error dialog */
      io_err = qof_session_pop_error (new_session);

      if (io_err == ERR_FILEIO_NO_ENCODING)
      {
        printf("ERR");
          // if (gnc_xml_convert_single_file (newfile))
          // {
          //     /* try to load once again */
          //     // gnc_window_show_progress(("Loading user data..."), 0.0);
          //     qof_session_load (new_session, gnc_window_show_progress);
          //     // gnc_window_show_progress(NULL, -1.0);
          //     // xaccLogEnable();
          //     io_err = qof_session_get_error (new_session);
          // }
          // else
          // {
          //     io_err = ERR_FILEIO_PARSE_ERROR;
          // }
      }

      // uh_oh = show_session_error (parent, io_err, newfile, GNC_FILE_DIALOG_OPEN);
      /* Attempt to update the database if it's too old */
      if ( !uh_oh && io_err == ERR_SQL_DB_TOO_OLD )
      {
          // gnc_window_show_progress(_("Re-saving user data..."), 0.0);
          qof_session_safe_save(new_session, gnc_window_show_progress);
          io_err = qof_session_get_error(new_session);
          // uh_oh = show_session_error(parent, io_err, newfile, GNC_FILE_DIALOG_SAVE);
      }
      /* Database is either too old and couldn't (or user didn't
        * want it to) be updated or it's too new. Mark it as
        * read-only
        */
      if (uh_oh && (io_err == ERR_SQL_DB_TOO_OLD ||
                    io_err == ERR_SQL_DB_TOO_NEW))
      {
          qof_book_mark_readonly(qof_session_get_book(new_session));
          uh_oh = FALSE;
      }
      new_root = gnc_book_get_root_account (qof_session_get_book (new_session));
      if (uh_oh) new_root = NULL;

      /* Umm, came up empty-handed, but no error:
        * The backend forgot to set an error. So make one up. */
      if (!uh_oh && !new_root)
      {
          // uh_oh = show_session_error (parent, ERR_BACKEND_MISC, newfile,
          //                             GNC_FILE_DIALOG_OPEN);
      }

      /* test for unknown features. */
      if (!uh_oh)
      {
          QofBook *book = qof_session_get_book (new_session);
          gchar *msg = gnc_features_test_unknown (book);
          // Account *template_root = gnc_book_get_template_root (book);
          Account *template_root = NULL;

          if (msg)
          {
              uh_oh = TRUE;

              // XXX: should pull out the file name here */
              // gnc_error_dialog (parent, msg, "");
              g_free (msg);
          }
          if (template_root != NULL)
          {
              GList *child = NULL;
              GList *children = gnc_account_get_descendants (template_root);

              for (child = children; child; child = g_list_next (child))
              {
                  Account *acc = GNC_ACCOUNT (child->data);
                  GList *splits = xaccAccountGetSplitList (acc);
                  // g_list_foreach (splits,
                  //                 (GFunc)gnc_sx_scrub_split_numerics, NULL);
              }
              g_list_free (children);
          }
      }
  }

  // g_free (scheme);
  // g_free (hostname);
  // g_free (username);
  // g_free (password);
  // g_free (path);

  // gnc_unset_busy_cursor (NULL);

  /* going down -- abandon ship */
  if (uh_oh)
  {
      // xaccLogDisable();
      qof_session_destroy (new_session);
      // xaccLogEnable();

      // gnc_get_current_session ();

      g_free (newfile);

      qof_event_resume ();
      // gnc_gui_refresh_all ();

      return FALSE;
  }

  // gnc_set_current_session(new_session);

  /* --------------- END CORE SESSION CODE -------------- */

  /* clean up old stuff, and then we're outta here. */
  // gnc_add_history (new_session);

  // g_free (newfile);

  qof_event_resume ();
  // gnc_gui_refresh_all ();

  /* Call this after re-enabling events. */
  // gnc_book_opened ();

  /* Check for account names that may contain the current separator character
    * and inform the user if there are any */
  // new_book = gnc_get_current_book();
  // invalid_account_names = gnc_account_list_name_violations ( new_book,
  //                         gnc_get_account_separator_string() );
  // if ( invalid_account_names )
  // {
  //     gchar *message = gnc_account_name_violations_errmsg ( gnc_get_account_separator_string(),
  //                       invalid_account_names );
  //     gnc_warning_dialog(parent, "%s", message);
  //     g_free ( message );
  // }

  // // Fix account color slots being set to 'Not Set', should run once on a book
  // qof_event_suspend();
  // xaccAccountScrubColorNotSet (gnc_get_current_book());
  // qof_event_resume();

  return new_session;
}

void gnc_window_show_progress (const char *message, double percentage) {
  printf("Loading the file (%d): %s: \n", (int)percentage, message);
}
