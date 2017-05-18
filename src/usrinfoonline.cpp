#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QQmlEngine>
#include <QDateTime>
#include "usrinfoonline.h"

/*
 * 构造函数
 * 输入参数：无
 * 返回数值：无
 * 功能描述：
 * 1、初始化密码为空
 * 2、设置密码
 * 3、初始化名称
 * 4、初始化等级
 */
UsrInfoOnline::UsrInfoOnline(UsrInfo* usrInfo, QObject *parent): QObject(parent)
{
    _usrInfo = usrInfo;
    _usrID   = 0;
    _onlineUsrInfo = QString::null;
    _activeTime = QDateTime::currentDateTime();
    _expireTime = QDateTime::currentDateTime();
    _loginTime  = QDateTime::currentDateTime();
}


/*
 * 析构函数
 * 输入参数：无
 * 返回数值：无
 * 功能描述：
 */
UsrInfoOnline::~UsrInfoOnline(){

}

//get offline configuration usrInfo.
QObject* UsrInfoOnline::usrInfo(void){
    QQmlEngine::setObjectOwnership(_usrInfo,QQmlEngine::CppOwnership);
    return _usrInfo;
}

//get or set usr ID
int UsrInfoOnline::usrID(void){
    return _usrID;
}

bool UsrInfoOnline::setUsrID(int id){
    if(_usrID != id){
        _usrID = id;
        return true;
    }
    return false;
}

//get or set online usr infomation
QString UsrInfoOnline::onlineUsrInfo(void)const{
    return _onlineUsrInfo;
}
bool UsrInfoOnline::setOnlineUsrInfo(const QString& info){
    if(_onlineUsrInfo != info){
        _onlineUsrInfo = info;
        return true;
    }
    return false;
}

//get or set log in expireTime
QDateTime UsrInfoOnline::expireTime(void)const{
    return _expireTime;
}
bool UsrInfoOnline::setExpireTime(const QDateTime& time){
    if(_expireTime != time){
        _expireTime = time;
        return true;
    }
    return false;
}

//get last log in time
QDateTime UsrInfoOnline::loginTime(void)const{
    return _loginTime;
}
bool UsrInfoOnline::setLoginTime(void){
    _loginTime = QDateTime::currentDateTime();
    return true;
}
//get last active time
QDateTime UsrInfoOnline::activeTime(void)const{
    return _activeTime;
}
bool UsrInfoOnline::setActiveTime(void){
    _activeTime = QDateTime::currentDateTime();
    return true;
}
