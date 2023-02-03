#pragma once
#include <qobject.h>
#include<SMTPEmail/SmtpMime>
#include<qstring.h>

class Email :
    public QObject
{
    Q_OBJECT
public:
    Email();
    ~Email();
    bool connect();
    bool sendMail(QString toMailAddress, QString code);

private:
  //  const static QString mailAddress;
  //  const static QString mailpassword;
    QString mailAddress  = "1955474265@qq.com";
    QString mailpassword = "xjsvouqvrvblebic";

private:
    SmtpClient* smtp=NULL;
}; 


/*
//管理员邮箱账号和密码
*/