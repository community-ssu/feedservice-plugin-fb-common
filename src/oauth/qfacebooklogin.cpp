#include "qfacebooklogin.h"

#include <QWebElementCollection>
#include <QWebFrame>
#include <QCoreApplication>
#include <QTextStream>
#include <QSslConfiguration>

#define TIMEOUT 30000

QFacebookLogin::QFacebookLogin(QObject *parent) :
  QObject(parent),
  accessTokenReceived(false),
  credentialsEntered(false)
{

  webView.setPage(new QWebPageN900());

  QSslConfiguration sslCfg = QSslConfiguration::defaultConfiguration();
  sslCfg.setProtocol(QSsl::TlsV1);
  QSslConfiguration::setDefaultConfiguration(sslCfg);

  connect(&webView, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
  connect(&webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
  connect(&webView, SIGNAL(urlChanged (const QUrl&)), this,
          SLOT(onRedirected(const QUrl&)));
  connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void QFacebookLogin::login()
{
  QString url = "https://m.facebook.com/dialog/oauth?"
      "client_id="+ client_id +
      "&display=popup"
      "&redirect_uri=https://www.facebook.com/connect/login_success.html"
      "&state="+ state +
      "&response_type=token" +
      (!scope.isEmpty() ? "&scope=" + scope : "");

  webView.load(QUrl::fromUserInput(url));
  timer.start(TIMEOUT);
}

void QFacebookLogin::outLine(const QString &s)
{
  QTextStream stream(stdout);
  stream << s << endl;
  stream.flush();
  if (!webView.isFullScreen())
    timer.start(TIMEOUT);
}

void QFacebookLogin::exit(const QString &s)
{
  accessTokenReceived = true;
  outLine(s);
  timer.stop();
  QCoreApplication::exit();
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
      /*QWebElementCollection elem_confirm =
          frame->findAllElements("input[name=__CONFIRM__]");*/

      QWebElementCollection tmp = frame->findAllElements("*");

      foreach (QWebElement paraElement, tmp)
      {
        qDebug() << paraElement.attribute("type") << paraElement.attribute("name");
       }

      if (elem_email.count() && elem_pass.count() && elem_login.count())
      {
        if (!credentialsEntered)
        {
          credentialsEntered = true;
          elem_email.first().setAttribute("value", email);
          elem_pass.first().setAttribute("value", password);
          elem_login.first().evaluateJavaScript("this.click()");
        }
        else
          exit("-1023,Could not login to facebook");
      }
      else
      {
      /*else if (elem_confirm.count())
      {
        if (!webView.isVisible())
          elem_confirm.first().evaluateJavaScript("this.click()");
        if (!webView.isVisible())
          webView.showFullScreen();
      }
      else if(credentialsEntered)
      {*/
        timer.stop();

        if (!webView.isVisible())
        {
          webView.setZoomFactor(1.8);
          webView.showFullScreen();
        }
      }
    }
  }
  else if (!accessTokenReceived)
    exit("-1022,Network error while logging to facebook!");
}

void QFacebookLogin::loadProgress(int progress)
{
  quint64 bytes = webView.page()->totalBytes();

  if (!accessTokenReceived && bytes)
    outLine("%" + QString::number(bytes));
}

void QFacebookLogin::onRedirected(const QUrl &url)
{
  timer.start(TIMEOUT);

  if (url.host().endsWith(".facebook.com") &&
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
      exit("0," + map["access_token"]);
    else
      exit("-1024,Could not login to facebook");
  }
}

void QFacebookLogin::timeout()
{
  exit("-1024,Could not login to facebook");
}
