#ifndef QFACEBOOKLOGIN_H
#define QFACEBOOKLOGIN_H

#include <QObject>
#include <QtWebKit/QWebView>

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
  QString client_id;
  QString state;
  QString email;
  QString password;
  QString scope;
  QWebView webView;
  bool accessTokenReceived;

public Q_SLOTS:
  void loadFinished(bool ok);
  void onRedirected(const QUrl &url);
};

#endif // QFACEBOOKLOGIN_H
