#ifndef USRINFOONLINE_H
#define USRINFOONLINE_H

#include <QObject>
#include <QCryptographicHash>
#include "usrinfo.h"
#include "iloadsave.h"
#include "iloadsaveprocessor.h"

//2017.05重写用户权限管理
//可以在QML、QWebChannel中使用
class UsrInfoOnline :  public QObject
{
    Q_OBJECT

    Q_PROPERTY(int       usrID          READ usrID)
    Q_PROPERTY(QString   onlineUsrInfo  READ onlineUsrInfo  WRITE setOnlineUsrInfo)
    Q_PROPERTY(QDateTime expireTime     READ expireTime     WRITE setExpireTime)
    Q_PROPERTY(QDateTime loginTime      READ loginTime)
    Q_PROPERTY(QDateTime activeTime     READ activeTime)

public:
    explicit UsrInfoOnline(UsrInfo* usrInfo , QObject *parent = 0);
    ~UsrInfoOnline();

public slots:
    //get offline configuration usrInfo.
    QObject* usrInfo(void);
    //get or set usr ID
    int usrID(void);
    bool setUsrID(int id);
    //get or set online usr infomation
    QString onlineUsrInfo(void)const;
    bool setOnlineUsrInfo(const QString& info);
    //get or set log in expireTime
    QDateTime expireTime(void)const;
    bool setExpireTime(const QDateTime& time);
    //get last log in time
    QDateTime loginTime(void)const;
    bool setLoginTime(void);
    //get last active time
    QDateTime activeTime(void)const;
    bool setActiveTime(long sec );

private:
    int         _usrID;
    UsrInfo*    _usrInfo;
    QString     _onlineUsrInfo;
    QDateTime   _expireTime,_loginTime,_activeTime;
};

#endif // USRINFOONLINE_H
