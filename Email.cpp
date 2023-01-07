/*************************************************

FileName:       Email.cpp
Author:         李佳波     
Description:    使用SmtpClient-for-Qt-v1.1库发送包含验证码的邮件

*************************************************/

#include "Email.h"
#include<qhostinfo.h>
#include<qdatetime.h>
#define toUTF8(str)  QString::fromLocal8Bit(str)

Email::Email()
{
    smtp=new SmtpClient("smtp.qq.com", 587, SmtpClient::TcpConnection);
    smtp->setUser(mailAddress);
    smtp->setPassword(mailpassword);
}

Email::~Email()
{
    smtp->quit();
    smtp->deleteLater();
}

/*************************************************
Description: 连接邮件服务
      Input: 
     Return: 是否连接成功
*************************************************/
bool Email::connect()
{
    if (smtp->connectToHost())
    {
        if (smtp->login())
            return true;
        else
            false;
    }
    else
    {
        return true;
    }

}

/*************************************************
Description: 连接邮件服务
      Input: toMailAddress=用户邮箱 
             code=验证码
     Return: 是否发送成功
*************************************************/
bool Email::sendMail(QString toMailAddress,QString code)
{
    MimeMessage message;
    EmailAddress sender(mailAddress);
    EmailAddress to(toMailAddress);
    message.setSender(&sender);
    message.addRecipient(&to);
    message.setSubject(toUTF8("SPDS坐姿监测系统用户注册验证码")); //邮件主题

    QString textStr= toUTF8("\
您好！\n\
您正在进行SPDS坐姿监测系统注册，\n\
您的邮箱验证码是：[code]（5分钟内有效），\
请及时输入验证码进行验证。\n\n\n\
广东东软学院SPDS项目团队\n\
[DateTime]\n");
    textStr.replace("[code]", code);
    textStr.replace("[DateTime]", QDateTime::currentDateTime().toString());

    MimeText text;
    text.setText(textStr);
    message.addPart(&text);
    return  smtp->sendMail(message);
}