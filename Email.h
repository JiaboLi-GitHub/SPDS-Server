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
    QString mailAddress  = "1955474265@qq.com";
    QString mailpassword = "joskdngricgccfbe";

private:
    SmtpClient* smtp=NULL;
}; 
