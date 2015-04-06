#include <QtGui/QApplication>

#include "qfacebooklogin.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  if (a.argc() < 5)
    return 1;

  QFacebookLogin *login = new QFacebookLogin;

  login->setClientId(a.arguments()[1]);
  login->setState(a.arguments()[2]);
  login->setEmail(a.arguments()[3]);
  login->setPassword(a.arguments()[4]);

  if (a.argc() > 5)
    login->setScope(a.arguments()[5]);

  login->login();

  int rv = a.exec();

  delete login;

  return rv;
}
