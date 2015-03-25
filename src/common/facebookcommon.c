/**
   @file facebookcommon.c

   @brief facebook plugin common functions

   Copyright (C) 2012 Ivaylo Dimitrov <freemangordon@abv.bg>

   This file is part of feedservice-plugin-fb-common.

   feedservice-plugin-fb-common is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   feedservice-plugin-fb-common is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with libbme. If not, see <http://www.gnu.org/licenses/>.
*/

#include <glib.h>
#include <gconf/gconf-client.h>
#include <libxml/tree.h>
#include <conicconnection.h>

#include "facebook/feedserviceutils2.h"
#include "facebook/common.h"

//#define DEBUG_LOG(msg) g_debug(msg)
#define DEBUG_LOG(msg)

#define RETURN_ERROR(_err_code_,_err_msg_) \
  do{ \
    error_code = _err_code_; \
    error_message = _err_msg_; \
    goto out; \
  }while(0);

void facebook_store_credentials_to_gconf(facebook_credentials* credentials)
{
  GConfClient* client = gconf_client_get_default();

  if(client)
  {
    gconf_client_set_string(client, FACEBOOK_CREDENTIAL_EMAIL,
                            credentials->email,
                            NULL);
    gconf_client_set_string(client, FACEBOOK_CREDENTIAL_SESSION_KEY,
                            credentials->session_key,
                            NULL);
    gconf_client_set_string(client, FACEBOOK_CREDENTIAL_SECRET_KEY,
                            credentials->secret,
                            NULL);
    gconf_client_set_string(client, FACEBOOK_CREDENTIAL_UID,
                            credentials->uid,
                            NULL);

    g_object_unref(client);
  }
}

facebook_credentials* facebook_credentials_from_gconf(void)
{
  gchar *secret_key;
  gchar *session_key;
  gchar *uid;
  gchar *email;
  facebook_credentials *credentials = NULL;
  GConfClient *client = gconf_client_get_default();

  if(client)
  {
    email = gconf_client_get_string(client,
                                    FACEBOOK_CREDENTIAL_EMAIL,
                                    NULL);
    session_key = gconf_client_get_string(client,
                                          FACEBOOK_CREDENTIAL_SESSION_KEY,
                                          NULL);
    secret_key = gconf_client_get_string(client,
                                         FACEBOOK_CREDENTIAL_SECRET_KEY,
                                         NULL);
    uid = gconf_client_get_string(client,
                                  FACEBOOK_CREDENTIAL_UID,
                                  NULL);

    if (secret_key && session_key && uid && email)
    {
      credentials = g_malloc(sizeof(facebook_credentials));

      credentials->secret = g_strdup(secret_key);
      credentials->session_key = g_strdup(session_key);
      credentials->uid = g_strdup(uid);
      credentials->email = g_strdup(email);
    }

    g_object_unref(client);
  }

  return credentials;
}

void facebook_credentials_free(facebook_credentials *credentials)
{
  if(credentials)
  {
    g_free(credentials->email);
    g_free(credentials->session_key);
    g_free(credentials->secret);
    g_free(credentials->uid);
    g_free(credentials);
  }
}

extern void facebook_delete_credentials(void)
{
  GConfClient *client = gconf_client_get_default();

  if (client)
  {
    gconf_client_unset(client,
                       FACEBOOK_CREDENTIAL_EMAIL,
                       NULL);
    gconf_client_unset(client,
                       FACEBOOK_CREDENTIAL_SESSION_KEY,
                       NULL);
    gconf_client_unset(client,
                       FACEBOOK_CREDENTIAL_SECRET_KEY,
                       NULL);
    gconf_client_unset(client,
                       FACEBOOK_CREDENTIAL_UID,
                       NULL);

    g_object_unref(client);
  }
}

gboolean facebook_is_credentials_exist(void)
{
  gchar *session_key = NULL;
  GConfClient *client = gconf_client_get_default();

  if (client)
  {
    session_key = gconf_client_get_string(client,
                                          FACEBOOK_CREDENTIAL_SESSION_KEY,
                                          NULL);
    g_object_unref(client);
  }

  return (session_key != NULL);
}

void facebook_set_email(gchar *email)
{
  GConfClient *client = gconf_client_get_default();
  if (client)
  {
    gconf_client_set_string(client,
                            FACEBOOK_CREDENTIAL_EMAIL,
                            email,
                            NULL);
    g_object_unref(client);
  }
}

gchar *facebook_get_email(void)
{
  gchar* email = NULL;
  GConfClient *client = gconf_client_get_default();

  if(client)
  {
    email = gconf_client_get_string(client,
                                    FACEBOOK_CREDENTIAL_EMAIL,
                                    NULL);
    g_object_unref(client);
  }

  return email;
}

static void facebook_request_common(facebook_request *request)
{
  GTimeVal time;
  GString* string;

  string = g_string_new(FACEBOOK_SECRET_KEY);
  request->secret = string->str;
  g_string_free(string,FALSE);

  g_get_current_time(&time);
  request->tv_sec = time.tv_sec;

  request->query_params = g_hash_table_new_full(g_str_hash,
                                                g_str_equal,
                                                NULL,
                                                g_free);

  g_hash_table_insert(request->query_params,
                      "api_key",
                      g_strdup(FACEBOOK_API_KEY));

  string = g_string_new(NULL);
  request->tv_sec ++;
  g_string_printf(string, "%ld", request->tv_sec);
  g_hash_table_insert(request->query_params, "call_id",  string->str);
  g_hash_table_insert(request->query_params, "v", g_strdup("1.0"));

  g_string_free(string,FALSE);

  request->data = NULL;
  request->validation_status = 0;
}

facebook_request *facebook_request_new()
{
  facebook_request *request = NULL;

  DEBUG_LOG(__func__);

  request = g_malloc0(sizeof(facebook_request));

  if(request)
    facebook_request_common(request);

  return request;
}

void facebook_request_null(facebook_request *request)
{
  DEBUG_LOG(__func__);

  g_free(request->secret);
  g_free(request->session_key);
  g_free(request->uid);
  g_free(request->email);
  g_free(request->password);

  request->secret = NULL;
  request->session_key = NULL;
  request->uid = NULL;
  request->email = NULL;
  request->password = NULL;

  g_hash_table_destroy(request->query_params);

  facebook_request_common(request);
}

void facebook_request_reset(facebook_request *request)
{
  DEBUG_LOG(__func__);

  g_hash_table_destroy(request->query_params);

  facebook_request_common(request);
}

gboolean generate_signature(facebook_request *request)
{
  GList *list;
  GList *sorted;
  GString *string;
  gchar *sig;

  DEBUG_LOG(__func__);

  list = g_hash_table_get_keys(request->query_params);
  sorted = g_list_sort(list,(GCompareFunc)g_strcmp0);
  string = g_string_new(NULL);

  list = sorted;

  while (list)
  {
    gchar *value = g_hash_table_lookup(request->query_params, list->data);
    g_string_append(string, list->data);
    g_string_append_c(string, '=');

    if(value)
      g_string_append(string, value);

    list = list->next;
  }

  g_list_free(sorted);

  g_string_append(string, request->secret);
  sig = g_compute_checksum_for_string(G_CHECKSUM_MD5, string->str, -1);

  g_string_free(string,TRUE);

  if(sig)
  {
    g_hash_table_insert(request->query_params, "sig", sig);

    return TRUE;
  }

  return FALSE;
}

facebook_credentials *facebook_login(facebook_request *request,
                                     ConIcConnection *con,
                                     HttpProgress* progress, GError **error)
{
  GArray *array;
  GString *string;
  xmlDocPtr xmldoc = NULL;
  xmlNodePtr xmlnode = NULL;
  facebook_credentials *credentials = NULL;
  GError* network_error = NULL;

  int error_code = 0;
  char* error_message = NULL;

  DEBUG_LOG(__func__);

  g_hash_table_insert(request->query_params,
                      "method",
                      g_strdup("facebook.auth.login"));
  g_hash_table_insert(request->query_params,
                      "email",
                      g_strdup(request->email));
  g_hash_table_insert(request->query_params,
                      "password",
                      g_strdup(request->password));

  generate_signature(request);

  array = g_array_new(FALSE, FALSE, 1);
  string = g_string_new("https://api.facebook.com/restserver.php");

  error_code =
      network_utils_get_with_progress_and_validate_certificate(
        string,
        array,
        NULL,
        request->query_params,
        con,
        progress,
        &network_error);

  g_string_free(string, FALSE);

  if (error_code != 200) /* HTTP OK */
  {
    g_warning(network_error->message);
    g_clear_error(&network_error);
    if (error_code == -1018)
      RETURN_ERROR(error_code, FACEBOOK_ERROR_CANNOT_VERIFY_CERT)
    else
      RETURN_ERROR(-1022, FACEBOOK_ERROR_NETWORK_ERROR)
  }

  error_code = 0;

  if (!array)
    RETURN_ERROR(-1024, FACEBOOK_ERROR_CANNOT_LOGIN);

  xmldoc = xmlParseMemory(array->data, array->len);
  xmlnode = xmlDocGetRootElement(xmldoc);

  if (!xmlnode || !xmlnode->name)
    RETURN_ERROR(-1024, FACEBOOK_ERROR_CANNOT_LOGIN);

  if (!xmlStrcmp(xmlnode->name, FACEBOOK_AUTH_LOGIN_RESPONSE))
  {
    if (!(xmlnode = xmlnode->children))
      RETURN_ERROR(-1024, FACEBOOK_ERROR_CANNOT_LOGIN)
    else
    {
      while ((xmlnode = xmlnode->next))
      {
        if (xmlnode->type != XML_ELEMENT_NODE)
           continue;

        if (!xmlStrcmp(xmlnode->name, (xmlChar *)"session_key"))
          request->session_key = (gchar *)xmlNodeGetContent(xmlnode);
        else if (!xmlStrcmp(xmlnode->name, (xmlChar *)"secret_key"))
        {
          g_free(request->secret);
          request->secret = (gchar *)xmlNodeGetContent(xmlnode);
        }
        else if (!xmlStrcmp(xmlnode->name, (xmlChar*)"uid"))
          request->uid = (gchar *)xmlNodeGetContent(xmlnode);
      }

      if (!request->uid ||
          !request->secret ||
          !request->session_key)
      {
        if (request->session_key)
          g_free(request->session_key);

        if (request->uid)
          g_free(request->uid);

        request->uid = request->session_key = NULL;

        RETURN_ERROR(-1024, FACEBOOK_ERROR_CANNOT_LOGIN);
      }
    }
  }
  else if (!xmlStrcmp(xmlnode->name, FACEBOOK_ERROR_RESPONSE))
  {
    if (!(xmlnode = xmlnode->children))
      RETURN_ERROR(-1024, FACEBOOK_ERROR_CANNOT_LOGIN)
    else
    {
      while ((xmlnode = xmlnode->next))
      {
        xmlChar *content;

        if ((xmlnode->type != XML_ELEMENT_NODE) ||
           (xmlStrcmp(xmlnode->name, FACEBOOK_ERROR_CODE)))
          continue;

        content = xmlNodeGetContent(xmlnode);
        error_code = strtol((const char*)content, NULL, 10);
        g_free(content);

        if (error_code == 401) /* Authentication failure */
          RETURN_ERROR(-1023, FACEBOOK_ERROR_CANNOT_LOGIN)
        else
          RETURN_ERROR(-1024, FACEBOOK_ERROR_CANNOT_LOGIN)
      }
    }
  }

out:

  if (xmldoc)
    xmlFreeDoc(xmldoc);
  if (array)
    g_array_free(array,TRUE);

  if (!error_code)
  {
    credentials = g_malloc(sizeof(facebook_credentials));

    if (credentials)
    {
      credentials->secret = g_strdup(request->secret);
      credentials->session_key = g_strdup(request->session_key);
      credentials->uid = g_strdup(request->uid);
      credentials->email = g_strdup(request->email);
    }
  }
  else
    g_set_error(error, g_quark_from_static_string(FACEBOOK_ERROR_QUARK),
                error_code, error_message);

  return credentials;
}

void facebook_request_free(facebook_request *request)
{
  DEBUG_LOG(__func__);

  if (request)
  {
    g_free(request->secret);
    g_free(request->uid);
    g_free(request->session_key);
    g_free(request->email);
    g_free(request->password);

    if (request->database)
      db_interface_free(request->database);

    if (request->db_loader)
      query_loader_free(request->db_loader);

    if (request->watcher)
      file_watcher_free(request->watcher);

    g_hash_table_destroy(request->query_params);

    g_free(request);
  }
}
