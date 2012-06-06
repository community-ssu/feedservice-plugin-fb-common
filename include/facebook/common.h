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

#define FACEBOOK_CREDENTIAL_SECRET_KEY  "/apps/maemo/facebook/secret_key"
#define FACEBOOK_CREDENTIAL_SESSION_KEY "/apps/maemo/facebook/session_key"
#define FACEBOOK_CREDENTIAL_UID         "/apps/maemo/facebook/uid"
#define FACEBOOK_CREDENTIAL_EMAIL       "/apps/maemo/facebook/email"

#define FACEBOOK_API_KEY                "765c02438771b10df50fe933bfaa7734"

#define FACEBOOK_AUTH_LOGIN_RESPONSE ((xmlChar*)"auth_login_response")
#define FACEBOOK_ERROR_RESPONSE ((xmlChar*)"error_response")
#define FACEBOOK_ERROR_CODE ((xmlChar*)"error_code")

#define FACEBOOK_ERROR_CANNOT_LOGIN "Could not login to facebook"
#define FACEBOOK_ERROR_CANNOT_VERIFY_CERT "Could not verify certificate!"
#define FACEBOOK_ERROR_NETWORK_ERROR "Network error while logging to facebook!"
#define FACEBOOK_ERROR_QUARK "FACEBOOK_ERROR"

typedef struct {
  gchar * secret_key;
  gchar * session_key;
  gchar * uid;
  gchar * email;
}facebook_credentials;

typedef struct {
  guint type;
  gchar* secret_key;
  gchar* session_key;
  gchar* uid;
  glong tv_sec;
  gchar* email;
  gchar* password;
  GHashTable* query_params;
  query_loader* db_loader;
  db_interface* database;
  file_watcher* watcher;
  void* data;
  gint validation_status;
}facebook_request;

extern void
facebook_store_credentials_to_gconf(facebook_credentials* credentials);

extern facebook_credentials*
facebook_credentials_from_gconf(void);

extern void
facebook_credentials_free(facebook_credentials* credentials);

extern void
facebook_delete_credentials(void);

extern gboolean
facebook_is_credentials_exist(void);

extern void
facebook_set_email(gchar* email);

extern gchar*
facebook_get_email(void);

extern facebook_credentials*
facebook_login(facebook_request* request,
                                            ConIcConnection* con,
                                            HttpProgress* progress,
                                            GError** error);

extern gboolean
generate_signature(facebook_request* request);

extern void
facebook_request_reset(facebook_request* request);

extern void
facebook_request_null(facebook_request* request);

extern facebook_request*
facebook_request_new();

extern void
facebook_request_free(facebook_request* request);

#endif
