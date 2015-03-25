#include "qfacebooklogin.h"

#include <QWebElementCollection>
#include <QWebFrame>
#include <QCoreApplication>
#include <QTextStream>
#include <QSslConfiguration>

QFacebookLogin::QFacebookLogin(QObject *parent) :
  QObject(parent),
  accessTokenReceived(false)
{
  QSslConfiguration sslCfg = QSslConfiguration::defaultConfiguration();
  sslCfg.setProtocol(QSsl::TlsV1);
  QSslConfiguration::setDefaultConfiguration(sslCfg);

  connect(&webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
  connect(&webView, SIGNAL(urlChanged (const QUrl&)), this,
          SLOT(onRedirected(const QUrl&)));
}

void QFacebookLogin::login()
{
  QString url = "https://www.facebook.com/dialog/oauth?"
      "client_id="+ client_id +
      "&display=popup"
      "&redirect_uri=https://www.facebook.com/connect/login_success.html"
      "&state="+ state +
      "&response_type=token" +
      (!scope.isEmpty() ? "&scope=" + scope : "");

  webView.load(QUrl::fromUserInput(url));
}

void QFacebookLogin::loadFinished(bool ok)
{
  if (ok)
  {
    QWebFrame *frame = webView.page()->currentFrame();

    if (frame)
    {
      QWebElementCollection elem_email =
          frame->findAllElements("input[name=email]");
      QWebElementCollection elem_pass =
          frame->findAllElements("input[name=pass]");
      QWebElementCollection elem_login =
          frame->findAllElements("input[name=login]");
      QWebElementCollection elem_confirm =
          frame->findAllElements("button[name=__CONFIRM__]");

      if (elem_email.count() && elem_pass.count() && elem_login.count())
      {
        elem_email.first().setAttribute("value", email);
        elem_pass.first().setAttribute("value", password);
        elem_login.first().evaluateJavaScript("this.click()");
      }
      else if (elem_confirm.count())
        elem_confirm.first().evaluateJavaScript("this.click()");
    }
  }
  else if (!accessTokenReceived)
  {
    QTextStream(stdout) << "LOGIN_FAILED";
    QCoreApplication::exit();
  }
}

void QFacebookLogin::onRedirected(const QUrl &url)
{
  if (url.host() == "www.facebook.com" &&
      url.path() == "/connect/login_success.html" &&
      url.hasFragment())
  {
    QStringList params = url.fragment().split("&");
    QMap <QString, QString> map;

    for (int i = 0; i < params.count(); i ++)
    {
      QStringList pairs = params.at(i).split("=");

      if (pairs.count() == 2)
        map.insert(pairs.at(0), pairs.at(1));
    }

    if (map.contains("access_token"))
    {
      QTextStream(stdout) << map["access_token"];
    }
    else
    {
      QTextStream(stdout) << "LOGIN_FAILED";
    }

    accessTokenReceived = true;
    QCoreApplication::exit();
  }
  else if(url.host() == "www.facebook.com" &&
          url.path() == "/login.php")
  {
    if (url.hasQueryItem("login_attempt"))
    {
      QTextStream(stdout) << "LOGIN_FAILED";
      QCoreApplication::exit();
    }
  }
}
