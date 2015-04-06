#ifndef QFACEBOOKLOGIN_H
#define QFACEBOOKLOGIN_H

#include <QObject>
#include <QtWebKit/QWebView>
#include <QTimer>
#include <QDebug>

class QWebPageN900 : public QWebPage
{
  Q_OBJECT

protected:
  QString userAgentForUrl(const QUrl & url) const
  {
    Q_UNUSED(url);

    QString languageName = QLocale().name();
    languageName.replace(QLatin1Char('_'), QLatin1Char('-'));

    /* For some reason facebook sends garbage when UA is Qt mobile. So, pretend
     * we are microb, but add "mobile" in the user agent, otherwise Facebook
     * presents to us desktop versions of the login pages
     */
    return
        "Mozilla/5.0 (X11; U; Linux armv7l; "+ languageName +
        "; rv:1.9.2b6pre) Gecko/20100318 Firefox/3.5 Maemo Browser 1.7.4.8 RX-51 N900 Mobile";
  }
};

class QFacebookLogin : public QObject
{
  Q_OBJECT
public:
  explicit QFacebookLogin(QObject *parent = 0);
  void setClientId(const QString &client_id)
  {
    this->client_id = client_id;
  }

  void setState(const QString &state)
  {
    this->state = state;
  }

  void setEmail(const QString &email)
  {
    this->email = email;
  }

  void setPassword(const QString &password)
  {
    this->password = password;
  }

  void setScope(const QString &scope)
  {
    this->scope = scope;
  }
  void login();

private:
  void outLine(const QString &s);
  void exit(const QString &s);
  QString client_id;
  QString state;
  QString email;
  QString password;
  QString scope;
  QWebView webView;
  bool accessTokenReceived;
  bool credentialsEntered;
  QTimer timer;

public Q_SLOTS:
  void loadFinished(bool ok);
  void loadProgress(int progress);
  void onRedirected(const QUrl &url);
  void timeout();
};

#endif /* QFACEBOOKLOGIN_H */
