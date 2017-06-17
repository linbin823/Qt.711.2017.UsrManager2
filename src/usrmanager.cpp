#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QUuid>
#include <QQmlEngine>
#include "usrmanager.h"
#include "usrmanagerui.h"
#include "loadsaveprocessorjson.h"

/*
 * 单例模式
 * 输入参数：无
 * 返回数值：无
 * 功能描述：
 * 1、获取单例的指针
 */
UsrManager* UsrManager::_singleton = nullptr;
UsrManager* UsrManager::Instance(){
    if(_singleton == nullptr){
        _singleton = new UsrManager();
    }
    return _singleton;
}

/*
 * 构造函数
 * 输入参数：父QObject
 * 返回数值：无
 * 功能描述：
 * 1、新建保存器
 * 2、设置密码
 * 3、初始化自动登出定时器
 * 4、读取数据
 */
UsrManager::UsrManager(QObject *parent) : QObject(parent){
    _pUI = nullptr;
    _secsTimeOutAftLogIn = 12*60*60;
    _usrInfoInit();
}
/*
 * 析构函数
 * 输入参数：无
 * 返回数值：无
 * 功能描述：
 * 1、保存所有数值
 * 2、删除保存器
 * 3、停止定时器
 */
UsrManager::~UsrManager(){
    if(_pUI != nullptr){
        _pUI->deleteLater();
    }
}

//查询登录状态，输入usrID，返回权限等级
int UsrManager::checkLogInLevel(QByteArray usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID )
            if( t->expireTime() >= QDateTime::currentDateTime() ){
                t->setActiveTime( _secsTimeOutAftLogIn );
                int level = static_cast<UsrInfo*>(t->usrInfo())->level();
                emit msgEventString( tr("在线查询结果：用户“%1”已登录，等级为%2。").arg( QString(usrID) ).arg(level) );
                return level;
            }
    }
    emit msgEventString( tr("在线查询结果：用户“%1”未登录。").arg( QString(usrID) ) );
    return 0;
}

//usrID已登录，返回true，否则返回false
bool UsrManager::isLogIn(QByteArray usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID )
            if( t->expireTime() >= QDateTime::currentDateTime() ){
                t->setActiveTime( _secsTimeOutAftLogIn );
                emit msgEventString( tr("在线查询结果：用户“%1”已登录。").arg( QString(usrID) ) );
                return true;
            }
    }
    emit msgEventString( tr("在线查询结果：用户“%1”未登录。").arg( QString(usrID) ) );
    return false;
}

//查询在线信息
QObject* UsrManager::usrInfoOnline(QByteArray usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID ){
            t->setActiveTime( _secsTimeOutAftLogIn );
            QQmlEngine::setObjectOwnership(t,QQmlEngine::CppOwnership);
            return t;
        }
    }
    return nullptr;
}

//获取用户信息
QObject* UsrManager::usrInfo( const QString& name ){
    foreach( UsrInfo* t, _usrInfoList){
        if( t->name() == name ){
            QQmlEngine::setObjectOwnership(t,QQmlEngine::CppOwnership);
            return t;
        }
    }
    return nullptr;
}

QObject* UsrManager::usrInfo(QByteArray usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID ){
            t->setActiveTime( _secsTimeOutAftLogIn );
            QQmlEngine::setObjectOwnership(t->usrInfo(),QQmlEngine::CppOwnership);
            return t->usrInfo();
        }
    }
    return nullptr;
}

void UsrManager::setTimeOutAftLogIn(long newValue){
    if(newValue != _secsTimeOutAftLogIn){
        _secsTimeOutAftLogIn = newValue;
        emit msgEventString( tr("登录超时时间设置为%1秒").arg(newValue) );
        emit msgTimeOutAftLogInChanged(newValue);
    }
}

/*
 * 登入
 * 输入参数：用户名、明文密码
 * 返回数值：
 * 1、返回usrID，失败返回0
 * 功能描述：
 * 1、把用户名登录信息到UsrInfoOnline
 */
QByteArray UsrManager::logIn( const QString& usrName, const QString& usrPwd, const QString& onlineUsrInfo ){
    QByteArray usrID = QByteArray();
    UsrInfo* info;
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        info = static_cast<UsrInfo*>(t->usrInfo() );
        if( info->name() == usrName && info->passWordCheck( info->genCryptoString( usrPwd ) )){
            if( t->onlineUsrInfo() == onlineUsrInfo ){
                //有登录信息
                t->setLoginTime();
                t->setActiveTime( _secsTimeOutAftLogIn );
                t->setExpireTime( QDateTime::currentDateTime().addSecs( _secsTimeOutAftLogIn ) );
                emit msgUsrInfoOnlineListChanged();
                emit msgEventString( tr("在线登录：重复登录\n\t时间：%1。\n\t用户名：%2。\n\t用户ID：%3。\n\t在线用户信息：%4。")
                                     .arg( QDateTime::currentDateTime().toString() )
                                     .arg( usrName)
                                     .arg( QString(usrID) )
                                     .arg( onlineUsrInfo) );
                return t->usrID();
            }
        }
    }
    foreach( UsrInfo* t, _usrInfoList){
        if( t->name() == usrName && t->passWordCheck( t->genCryptoString( usrPwd ) )){
            //新登录
            usrID = _usrIDGen();
            UsrInfoOnline* newOne = new UsrInfoOnline(t, this);
            newOne->setLoginTime();
            newOne->setActiveTime( _secsTimeOutAftLogIn );
            newOne->setExpireTime( QDateTime::currentDateTime().addSecs( _secsTimeOutAftLogIn ) );
            newOne->setOnlineUsrInfo( onlineUsrInfo );
            newOne->setUsrID( usrID );
            _usrInfoOnlineList.append( newOne );
            emit msgUsrInfoOnlineListChanged();
            emit msgEventString( tr("在线登录：新登录\n\t时间：%1。\n\t用户名：%2。\n\t用户ID：%3。\n\t在线用户信息：%4。")
                                 .arg( QDateTime::currentDateTime().toString() )
                                 .arg( usrName)
                                 .arg( QString(usrID))
                                 .arg( onlineUsrInfo) );
            return usrID;
        }
    }
    //用户名找不到， 密码不对, return 0
    emit msgEventString( tr("在线登录：失败，找不到用户名或密码不对。") );
    return usrID;
}

//登出
void UsrManager::logOut( QByteArray usrID ){
    foreach( UsrInfoOnline* t , _usrInfoOnlineList){
        if( t->usrID() == usrID){
            t->setExpireTime( QDateTime::currentDateTime());
            emit msgEventString( tr("在线登出：成功，登出'%1'。").arg( QString(usrID) ) );
            emit msgUsrInfoOnlineListChanged();
            return;
        }
    }
    emit msgEventString( tr("在线登出：失败，找不到'%1'。").arg( QString(usrID) ) );
}

//全部登出
void UsrManager::logOutAll(void){
    foreach( UsrInfoOnline* t , _usrInfoOnlineList){
            t->setExpireTime( QDateTime::currentDateTime());
    }
    emit msgUsrInfoOnlineListChanged();
    emit msgEventString( tr("在线登出：全部手动登出。") );
}

//添加用户
QObject* UsrManager::addUsr(const QString& name, int level, const QString& pswd,
                const QString& usrDescript){
    bool ok = checkName( name );
    if( ok ){
        //用户名不冲突
        UsrInfo* newOne = new UsrInfo(this);
        newOne->setName( name , "");
        newOne->setLevel( level , newOne->genCryptoString( "" ));
        newOne->setPassWord( newOne->genCryptoString( "" ), newOne->genCryptoString( pswd ));
        newOne->setUsrDescript( usrDescript );
        connect(newOne,&UsrInfo::nameChanged,
                this,&UsrManager::_usrInfoChanged);
        connect(newOne,&UsrInfo::levelChanged,
                this,&UsrManager::_usrInfoChanged);
        connect(newOne,&UsrInfo::usrDescriptChanged,
                this,&UsrManager::_usrInfoChanged);
        _usrInfoList.append(newOne);
        emit msgUsrInfoListChanged();
        emit msgEventString( tr("用户添加：成功，新增用户'%1'。").arg(name) );
        return newOne;
    }
    emit msgEventString( tr("用户添加：失败，用户名'%1'重复！").arg(name) );
    //用户名冲突
    return nullptr;
}

//删除用户
bool UsrManager::deleteUsr(const QString& name){
    foreach( UsrInfo* t , _usrInfoList){
        if( t->name() == name){
            foreach( UsrInfoOnline* o, _usrInfoOnlineList){
                if( static_cast<UsrInfo*>(o->usrInfo())->name() == name ){
                    logOut( o->usrID() );
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

//获得用户列表
QList<QObject*> UsrManager::allUsrInfo(void){
    QList<QObject*> usrInfoList;
    foreach (UsrInfo* it, _usrInfoList) {
        QQmlEngine::setObjectOwnership(it,QQmlEngine::CppOwnership);
        usrInfoList << it;
    }
    return usrInfoList;
}

QList<QObject*> UsrManager::allUsrInfoOnline(void){
    QList<QObject*> usrInfoOnlineList;
    foreach (UsrInfoOnline* it, _usrInfoOnlineList) {
        QQmlEngine::setObjectOwnership(it,QQmlEngine::CppOwnership);
        usrInfoOnlineList << it;
    }
    return usrInfoOnlineList;
}

//用户数量统计
int UsrManager::activeOnlineUsrNumber(void){
    int count = 0;
    foreach (UsrInfoOnline* t, _usrInfoOnlineList) {
        if(isLogIn( t->usrID() )){
            count++;
        }
    }
    return count;
}

int UsrManager::totalOnlineUsrNumber(void){
    return _usrInfoOnlineList.size();
}

int UsrManager::offlineUsrNumber(void){
    return _usrInfoList.size();
}
/*
 * 检查用户名是否重名
 * 输入参数：
 * 1、新的用户名称
 * 返回数值：
 * 1、重名=false，无重名=true
 * 功能描述：
 * 1、遍历所有用户，检查新的用户名是否重名
 */
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

//生成UsrID
inline QByteArray UsrManager::_usrIDGen(void)const{
//    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//    int usrID = 0;
//    bool duplicatedID = false;
//    while( usrID ==0 || duplicatedID ){
//        usrID = qrand();
//        foreach(UsrInfoOnline* t, this->_usrInfoOnlineList){
//            if(t->usrID() == usrID ){
//                duplicatedID = true;
//                continue;
//            }
//        }
//        duplicatedID = false;
//    }
//    return usrID;
    return QUuid::createUuid().toByteArray();
}

void UsrManager::load(iLoadSaveProcessor* processor){

    processor->readValue("secsTimeOutAftLogIn", _secsTimeOutAftLogIn );

    UsrInfo* usr;
    _usrInfoInit();
    int count;
    int ret = processor->readValue("usrCounts",count);
    if(ret != 0){
        for(int i=0; i<count; i++){
            //load的时候要检查用户名是否重复，重复的话要丢弃
            processor->moveToInstance("usrInfo", QString::number(i));
            usr = new UsrInfo(this);
            usr->load(processor);
            processor->moveBackToParent();

            if( !checkName( usr->name() ) ){
                usr->deleteLater();
                continue;
            }
            _usrInfoList.append(usr);
        }
        emit msgUsrInfoListChanged();
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
            _usrInfoList.at(i)->save(processor);
            processor->moveBackToParent();
        }
    }
    emit msgEventString( tr("用户管理模块保存。。成功") );
}

void UsrManager::_usrInfoInit(){
    foreach(UsrInfo* t, _usrInfoList){
        t->deleteLater();
        _usrInfoList.removeOne(t);
    }
    foreach(UsrInfoOnline* t, _usrInfoOnlineList){
        t->deleteLater();
        _usrInfoOnlineList.removeOne(t);
    }
    addUsr("superior",199,"supp1","");
    emit msgUsrInfoListChanged();
    emit msgUsrInfoOnlineListChanged();
}

//处理UsrInfo参数的改变
void UsrManager::_usrInfoChanged(void){
    emit msgUsrInfoListChanged();
}

//显示管理页面
void UsrManager::showUI(QWidget *parent){
    if(_pUI == nullptr){
        _pUI = new UsrManagerUI(this, parent);
        _pUI->show();
    }
    if(_pUI->isHidden()){
        _pUI->show();
    }
}
