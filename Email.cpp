/*************************************************

FileName:       Email.cpp
Author:         ��Ѳ�     
Description:    ʹ��SmtpClient-for-Qt-v1.1�ⷢ�Ͱ�����֤����ʼ�

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
Description: �����ʼ�����
      Input: 
     Return: �Ƿ����ӳɹ�
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
Description: �����ʼ�����
      Input: toMailAddress=�û����� 
             code=��֤��
     Return: �Ƿ��ͳɹ�
*************************************************/
bool Email::sendMail(QString toMailAddress,QString code)
{
    MimeMessage message;
    EmailAddress sender(mailAddress);
    EmailAddress to(toMailAddress);
    message.setSender(&sender);
    message.addRecipient(&to);
    message.setSubject(toUTF8("SPDS���˼��ϵͳ�û�ע����֤��")); //�ʼ�����

    QString textStr= toUTF8("\
���ã�\n\
�����ڽ���SPDS���˼��ϵͳע�ᣬ\n\
����������֤���ǣ�[code]��5��������Ч����\
�뼰ʱ������֤�������֤��\n\n\n\
�㶫����ѧԺSPDS��Ŀ�Ŷ�\n\
[DateTime]\n");
    textStr.replace("[code]", code);
    textStr.replace("[DateTime]", QDateTime::currentDateTime().toString());

    MimeText text;
    text.setText(textStr);
    message.addPart(&text);
    return  smtp->sendMail(message);
}