﻿#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QUuid>
#include <QQmlEngine>
#include "usrinfo.h"
#include "sessioninfo.h"
#include "usrmanager.h"
#include "usrmanagerui.h"
#include "loadsaveprocessorjson.h"

UsrManager* UsrManager::_singleton = nullptr;
UsrManager* UsrManager::Instance(){
    if(_singleton == nullptr){
        _singleton = new UsrManager();
    }
    return _singleton;
}

UsrManager::UsrManager(QObject *parent) : QObject(parent){
    _pUI = nullptr;
    _secsTimeOutAftLogIn = 12*60*60;//12hrs
    _usrInfoInit();
}

UsrManager::~UsrManager(){
    if(_pUI != nullptr){
        _pUI->deleteLater();
    }
}

int UsrManager::checkLogInLevel(const QByteArray &usrID ){
    if( _sessionInfoList.contains(usrID) ){
        SessionInfo* t = _sessionInfoList[usrID];
        if(t->isActive()){
            t->setActiveTime( _secsTimeOutAftLogIn );
            int level = t->_usrInfo->level();
            emit msgEventString( tr("在线查询结果：用户“%1”已登录，等级为%2。").arg( QString(usrID) ).arg( QString::number(level) ));
            return level;
        }
    }
    emit msgEventString( tr("在线查询结果：用户“%1”未登录。").arg( QString(usrID) ) );
    return 0;
}

bool UsrManager::isLogIn(const QByteArray &usrID ){
    if( _sessionInfoList.contains(usrID) ){
        SessionInfo* t = _sessionInfoList[usrID];
        if(t->isActive()){
            t->setActiveTime( _secsTimeOutAftLogIn );
            emit msgEventString( tr("在线查询结果：用户“%1”已登录。").arg( QString(usrID) ) );
            return true;
        }
    }
    emit msgEventString( tr("在线查询结果：用户“%1”未登录。").arg( QString(usrID) ) );
    return false;
}

QObject* UsrManager::sessionInfo(const QByteArray &usrID )const{
    if( _sessionInfoList.contains(usrID) ){
        SessionInfo* t = _sessionInfoList[usrID];
        QQmlEngine::setObjectOwnership(t,QQmlEngine::CppOwnership);
        return t;
    }
    return nullptr;
}

QObject* UsrManager::usrInfo( const QString& name )const{
    for( int i=0; i<_usrInfoList.size(); i++){
        UsrInfo* t = _usrInfoList[i];
        if( t->name() == name ){
            QQmlEngine::setObjectOwnership(t,QQmlEngine::CppOwnership);
            return t;
        }
    }
    return nullptr;
}

QObject* UsrManager::usrInfo(const QByteArray &usrID )const{
    if( _sessionInfoList.contains(usrID) ){
        SessionInfo* t = _sessionInfoList[usrID];
        UsrInfo* u = t->_usrInfo;
        QQmlEngine::setObjectOwnership(u,QQmlEngine::CppOwnership);
        return u;
    }
    return nullptr;
}

long UsrManager::timeOutAftLogIn(void)const{
    return _secsTimeOutAftLogIn;
}

void UsrManager::setTimeOutAftLogIn(long newValue){
    if(newValue != _secsTimeOutAftLogIn){
        _secsTimeOutAftLogIn = newValue;
        emit msgEventString( tr("登录超时时间设置为%1秒").arg(newValue) );
        emit msgTimeOutAftLogInChanged(newValue);
    }
}

QByteArray& UsrManager::logIn( const QString& usrName, const QByteArray& usrPwd, const QString& usrIdentifier ){
    static QByteArray sessionID = QByteArray();
    sessionID = QByteArray();
    UsrInfo* uInfo;
    SessionInfo* sInfo;
    QHash<QByteArray,SessionInfo*>::iterator it;
    QString onlineUsrIdentifier;
    for(it=_sessionInfoList.begin() ; it!=_sessionInfoList.end() ; ++it){
        sInfo = it.value();
        uInfo = sInfo->_usrInfo;
        if( uInfo->name() == usrName && uInfo->passWordCheck( usrPwd )){
            onlineUsrIdentifier = sInfo->property("identifier").toString();
            if( usrIdentifier == onlineUsrIdentifier ){
                //有登录信息,重复登录
                sInfo->setLoginTime();
                sInfo->setActiveTime( _secsTimeOutAftLogIn );
                sessionID = it.key();
                emit msgSessionInfoListChanged();
                emit msgEventString( tr("在线登录：重复登录\n\t用户名：%1。\n\t用户ID：%2。\n\t会话识别信息：%3。")
                                     .arg( usrName)
                                     .arg( QString(sessionID) )
                                     .arg( usrIdentifier) );
                return sessionID;
            }
        }
    }
    foreach (UsrInfo* t, _usrInfoList) {
        if( t->name() == usrName && t->passWordCheck( usrPwd )){
            //新登录
            sessionID = _sessionIDGen();
            SessionInfo* newOne= new SessionInfo(t, this);
            QQmlEngine::setObjectOwnership(newOne,QQmlEngine::CppOwnership);
            newOne->setLoginTime();
            newOne->setActiveTime( _secsTimeOutAftLogIn );
            newOne->setProperty( "identifier", QVariant::fromValue(usrIdentifier) );
            _sessionInfoList.insert( sessionID, newOne);
            emit msgSessionInfoListChanged();
            emit msgEventString( tr("在线登录：新登录\n\t用户名：%1。\n\tSessionID：%2。\n\t会话识别信息：%3。")
                                 .arg( usrName)
                                 .arg( QString(sessionID))
                                 .arg( usrIdentifier) );
            return sessionID;
        }
    }
    //用户名找不到， 密码不对, return 0
    emit msgEventString( tr("在线登录：失败，找不到用户名或密码不对。") );
    return sessionID;
}

void UsrManager::logOut( QByteArray usrID ){
    if( _sessionInfoList.contains(usrID) ){
        SessionInfo* t = _sessionInfoList[usrID];
        if(t->isActive()){
            t->setExpireTime( QDateTime::currentDateTime());
            emit msgEventString( tr("在线登出：成功，登出'%1'。").arg( QString(usrID) ) );
            emit msgSessionInfoListChanged();
        }
        return;
    }
    emit msgEventString( tr("在线登出：失败，找不到'%1'。").arg( QString(usrID) ) );
}

void UsrManager::logOutAll(void){
    QList<QByteArray> keys = _sessionInfoList.keys();
    foreach( QByteArray key, keys){
        logOut(key);
    }
}

QObject* UsrManager::addUsr(const QString& name, int level, const QString &pwdWithoutCrypto,
                const QString& usrDescript){
    bool ok = checkName( name );
    if( ok ){
        //用户名不冲突
        UsrInfo* newOne = new UsrInfo(this);
        newOne->setName( name , "");
        newOne->setPassWord( UsrInfo::genCryptoString( name, "" ), UsrInfo::genCryptoString( name, pwdWithoutCrypto ));
        newOne->setLevel( level , UsrInfo::genCryptoString( name, pwdWithoutCrypto ));
        newOne->setUsrDescript( usrDescript, UsrInfo::genCryptoString( name, pwdWithoutCrypto ) );
        connect(newOne,&UsrInfo::nameChanged,
                this,&UsrManager::_usrInfoChanged);
        connect(newOne,&UsrInfo::levelChanged,
                this,&UsrManager::_usrInfoChanged);
        connect(newOne,&UsrInfo::usrDescriptChanged,
                this,&UsrManager::_usrInfoChanged);
        _usrInfoList.append(newOne);
        emit msgUsrInfoListChanged();
        emit msgEventString( tr("用户添加：成功，新增用户'%1'。").arg(name) );
        QQmlEngine::setObjectOwnership(newOne,QQmlEngine::CppOwnership);
        return newOne;
    }
    emit msgEventString( tr("用户添加：失败，用户名'%1'重复！").arg(name) );
    //用户名冲突
    return nullptr;
}

bool UsrManager::deleteUsr(const QString& name){

    foreach (UsrInfo* t, _usrInfoList) {
        if( t->name() == name){
            QHash<QByteArray,SessionInfo*>::iterator it;
            for(it = _sessionInfoList.begin(); it!= _sessionInfoList.end(); ++it){
                if(it.value()->_usrInfo->name() == name){
                    logOut( it.key() );
                }
            }
            _usrInfoList.removeOne( t );
            t->deleteLater();
            emit msgUsrInfoListChanged();
            emit msgEventString( tr("用户删除：成功，删除用户'%1'。").arg(name) );
            return true;
        }
    }
    emit msgEventString( tr("用户删除：失败，找不到用户'%1'！").arg(name) );
    return false;
}

QList<UsrInfo*> &UsrManager::allUsrInfo(void){
    return _usrInfoList;
}

QHash<QByteArray,SessionInfo*> &UsrManager::allSessionInfo(void){
    return _sessionInfoList;
}

int UsrManager::activeSessionNumber(void){
    int count = 0;
    QList<SessionInfo*> list (_sessionInfoList.values());
    foreach (SessionInfo* t, list) {
        if(t->isActive() ){
            count++;
        }
    }
    return count;
}

int UsrManager::totalSessionNumber(void){
    return _sessionInfoList.size();
}

int UsrManager::disactiveSessionNumber(void){
    return ( totalSessionNumber() - activeSessionNumber() );
}

bool UsrManager::checkName(const QString& newName) const{
    QListIterator<UsrInfo*> it(_usrInfoList);
    UsrInfo* temp;
    it.toFront();
    while(it.hasNext()){
        temp = it.next();
        if(temp->name() == newName ){
                return false;
        }
    }
    return true;
}

QByteArray UsrManager::_sessionIDGen(void)const{
    return QUuid::createUuid().toByteArray();
}

void UsrManager::load(iLoadSaveProcessor* processor){

    processor->readValue("secsTimeOutAftLogIn", _secsTimeOutAftLogIn );

    _usrInfoInit();
    int count;
    int ret = processor->readValue("usrCounts",count);
    UsrInfo* uInfo;
    SessionInfo* sInfo;
    QByteArray sessionID;
    if(ret != 0){
        for(int i=0; i<count; i++){
            //load的时候要检查用户名是否重复，重复的话要丢弃
            processor->moveToInstance("usrInfo", QString::number(i));
            uInfo = new UsrInfo(this);
            uInfo->load(processor);

            if( checkName( uInfo->name() ) ){
                connect(uInfo,&UsrInfo::nameChanged,
                        this,&UsrManager::_usrInfoChanged);
                connect(uInfo,&UsrInfo::levelChanged,
                        this,&UsrManager::_usrInfoChanged);
                connect(uInfo,&UsrInfo::usrDescriptChanged,
                        this,&UsrManager::_usrInfoChanged);
                _usrInfoList.append(uInfo);
                continue;
            }
            uInfo->deleteLater();
            processor->moveBackToParent();
        }
        emit msgUsrInfoListChanged();
        emit msgEventString( tr("用户管理模块读取。。成功") );
    }

    ret = processor->readValue("sessionCounts",count);
        if(ret != 0){
            for(int i=0; i<count; i++){
                QString usrName;
                processor->moveToInstance("sessionInfo", QString::number(i));
                processor->readValue("usrName", usrName );
                processor->readValue("sessionID", sessionID );

                foreach (UsrInfo* t, _usrInfoList) {
                    if(t->name() == usrName){
                        sInfo = new SessionInfo(t,this);
                        sInfo->load(processor);
                        _sessionInfoList.insert( sessionID, sInfo);
                        break;
                    }
                }
                processor->moveBackToParent();
            }
            emit msgUsrInfoListChanged();
            emit msgSessionInfoListChanged();
            emit msgEventString( tr("用户管理模块读取。。成功") );
        }
}

void UsrManager::save(iLoadSaveProcessor* processor){

    processor->writeValue("secsTimeOutAftLogIn", _secsTimeOutAftLogIn );

    int count = _usrInfoList.size();
    int ret = processor->writeValue("usrInfoCounts", count );
    if(ret != 0){
        for(int i=0; i<count; i++){
            processor->moveToInstance("usrInfo", QString::number(i));
            _usrInfoList[i]->save(processor);
            processor->moveBackToParent();
        }
    }
    count = _sessionInfoList.count();
    ret = processor->writeValue("sessionCounts", count );
    if(ret != 0){
        int i=0;
        QHash<QByteArray,SessionInfo*>::iterator it;
        for(it=_sessionInfoList.begin();it!=_sessionInfoList.end();++it,++i){
            processor->moveToInstance("sessionInfo", QString::number(i));
            processor->writeValue("usrName", it.value()->_usrInfo->name() );
            QByteArray temp = it.key();
            processor->writeValue("sessionID", temp );
            it.value()->save(processor);
            processor->moveBackToParent();
        }
    }
    emit msgEventString( tr("用户管理模块保存。。成功") );
}

void UsrManager::_usrInfoInit(){
    for( int i=0;i<_usrInfoList.size();i++){
        UsrInfo* t = _usrInfoList[i];
        t->deleteLater();
        _usrInfoList.removeOne(t);
    }
    addUsr("superior",199,"supp1","");
}

void UsrManager::_usrInfoChanged(void){
    emit msgUsrInfoListChanged();
}

void UsrManager::showUI(QWidget *parent){
    if(_pUI == nullptr){
        _pUI = new UsrManagerUI(parent);
        _pUI->show();
    }
    if(_pUI->isHidden()){
        _pUI->show();
    }
}
