/**
   @file facebookcommon.h

   Copyright (C) 2012 Ivaylo Dimitrov <freemangordon@abv.bg>

   This file is part of feedservice-plugin-fb-common.

   feedservice-plugin-fb-common is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Libbme is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with libbme. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _FACEBOOKCOMMON_H_
#define _FACEBOOKCOMMON_H_

#define FACEBOOK_CREDENTIAL_SECRET_KEY          "/apps/maemo/facebook/secret_key"
#define FACEBOOK_CREDENTIAL_SESSION_KEY         "/apps/maemo/facebook/session_key"
#define FACEBOOK_CREDENTIAL_UID                 "/apps/maemo/facebook/uid"
#define FACEBOOK_CREDENTIAL_EMAIL               "/apps/maemo/facebook/email"
#define FACEBOOK_GRAPH_CREDENTIAL_ACCESS_TOKEN  "/apps/maemo/facebook/access_token"

#define FACEBOOK_API_KEY                "765c02438771b10df50fe933bfaa7734"
#define FACEBOOK_SECRET_KEY             "f3630da1d45d2b283e3237aa344f4d5d"
#define FACEBOOK_GRAPH_API_CLIENT_ID    "779989358743970"

#define FACEBOOK_AUTH_LOGIN_RESPONSE ((xmlChar*)"auth_login_response")
#define FACEBOOK_ERROR_RESPONSE ((xmlChar*)"error_response")
#define FACEBOOK_ERROR_CODE ((xmlChar*)"error_code")

#define FACEBOOK_ERROR_CANNOT_LOGIN "Could not login to facebook"
#define FACEBOOK_ERROR_CANNOT_VERIFY_CERT "Could not verify certificate!"
#define FACEBOOK_ERROR_NETWORK_ERROR "Network error while logging to facebook!"
#define FACEBOOK_ERROR_QUARK "FACEBOOK_ERROR"

typedef struct {
  gchar *secret;
  gchar *session_key;
  gchar *uid;
  gchar *email;
}facebook_credentials;

typedef struct {
  guint type;
  gchar *secret;
  gchar *session_key;
  gchar *uid;
  glong tv_sec;
  gchar *email;
  gchar *password;
  GHashTable *query_params;
  query_loader *db_loader;
  db_interface *database;
  file_watcher *watcher;
  void *data;
  gint validation_status;
}facebook_request;

extern void
facebook_store_credentials_to_gconf(facebook_credentials *credentials) __attribute__ ((deprecated));

extern facebook_credentials *
facebook_credentials_from_gconf(void) __attribute__ ((deprecated));

extern void
facebook_credentials_free(facebook_credentials* credentials) __attribute__ ((deprecated));

extern void
facebook_delete_credentials(void) __attribute__ ((deprecated));

extern gboolean
facebook_is_credentials_exist(void) __attribute__ ((deprecated));

extern void
facebook_set_email(gchar* email);

extern gchar*
facebook_get_email(void);

extern facebook_credentials*
facebook_login(facebook_request* request,
               ConIcConnection* con,
               HttpProgress* progress,
               GError** error) __attribute__ ((deprecated));

extern gboolean
generate_signature(facebook_request* request) __attribute__ ((deprecated));

extern void
facebook_request_reset(facebook_request* request) __attribute__ ((deprecated));

extern void
facebook_request_null(facebook_request* request) __attribute__ ((deprecated));

extern facebook_request*
facebook_request_new() __attribute__ ((deprecated));

extern void
facebook_request_free(facebook_request* request) __attribute__ ((deprecated));

typedef struct {
  gchar *access_token;
  gchar *email;
}facebook_graph_credentials;

typedef struct {
  guint type;
  gchar *access_token;
  glong tv_sec;
  gchar *email;
  gchar *password;
  gchar *scope;
  GHashTable *query_params;
  query_loader *db_loader;
  db_interface *database;
  file_watcher *watcher;
  gpointer data;
  gint validation_status;
}facebook_graph_request;

extern facebook_graph_credentials*
facebook_graph_login(facebook_graph_request *request,
                     ConIcConnection *con,
                     HttpProgress *progress,
                     GError **error);
extern void
facebook_store_graph_credentials_to_gconf(
    facebook_graph_credentials *credentials);

extern void
facebook_graph_credentials_free(facebook_graph_credentials *credentials);

extern facebook_graph_request *
facebook_graph_request_new();

void
facebook_graph_request_null(facebook_graph_request *request);

extern void
facebook_graph_request_reset(facebook_graph_request *request);

extern void
facebook_graph_request_free(facebook_graph_request *request);

#endif
