#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QQmlEngine>
#include "usrmanager.h"
#include "loadsaveprocessorjson.h"
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

}

//查询登录状态，输入usrID，返回权限等级
int UsrManager::checkLogInLevel( int usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID )
            if( t->expireTime() >= QDateTime::currentDateTime() ){
                t->setActiveTime();
                return static_cast<UsrInfo*>(t->usrInfo())->level();
            }
    }
    return 0;
}

//usrID已登录，返回true，否则返回false
bool UsrManager::isLogIn( int usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID )
            if( t->expireTime() >= QDateTime::currentDateTime() ){
                t->setActiveTime();
                return true;
            }
    }
    return false;
}

//查询在线信息
QObject* UsrManager::getUsrInfoOnline( int usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID ){
            t->setActiveTime();
            QQmlEngine::setObjectOwnership(t,QQmlEngine::CppOwnership);
            return t;
        }
    }
    return nullptr;
}

//获取用户信息
QObject* UsrManager::getUsrInfo( const QString& name ){
    foreach( UsrInfo* t, _usrInfoList){
        if( t->name() == name ){
            QQmlEngine::setObjectOwnership(t,QQmlEngine::CppOwnership);
            return t;
        }
    }
    return nullptr;
}

QObject* UsrManager::getUsrInfo( int usrID ){
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        if(t->usrID() == usrID ){
            t->setActiveTime();
            QQmlEngine::setObjectOwnership(t->usrInfo(),QQmlEngine::CppOwnership);
            return t->usrInfo();
        }
    }
    return nullptr;
}

/*
 * 登入
 * 输入参数：用户名、明文密码
 * 返回数值：
 * 1、返回usrID，失败返回0
 * 功能描述：
 * 1、把用户名登录信息到UsrInfoOnline
 */
int UsrManager::logIn( const QString& usrName, const QString& usrPwd, const QString& onlineUsrInfo ){
    int usrID = 0;
    UsrInfo* info;
    foreach( UsrInfoOnline* t, _usrInfoOnlineList){
        info = static_cast<UsrInfo*>(t->usrInfo() );
        if( info->name() == usrName && info->passWordCheck( usrPwd )){
            if( t->onlineUsrInfo() == onlineUsrInfo ){
                //有登录信息
                t->setLoginTime();
                t->setActiveTime();
                t->setExpireTime( QDateTime::currentDateTime().addSecs( 12*60*60 ) );
                return t->usrID();
            }
        }
    }
    foreach( UsrInfo* t, _usrInfoList){
        if( t->name() == usrName && t->passWordCheck( usrPwd )){
            //新登录
            usrID = usrIDGen();
            UsrInfoOnline* newOne = new UsrInfoOnline(t, this);
            newOne->setLoginTime();
            newOne->setActiveTime();
            newOne->setExpireTime( QDateTime::currentDateTime().addSecs( 12*60*60 ) );
            newOne->setOnlineUsrInfo( onlineUsrInfo );
            newOne->setUsrID( usrID );
            _usrInfoOnlineList.append( newOne );
            emit msgUsrInfoOnlineListChanged();
            return usrID;
        }
    }
    //用户名找不到， 密码不对, return 0
    return usrID;
}

//登出
void UsrManager::logOut( int usrID ){
    foreach( UsrInfoOnline* t , _usrInfoOnlineList){
        if( t->usrID() == usrID){
            t->setExpireTime( QDateTime::currentDateTime());
            return;
        }
    }
}

//全部登出
void UsrManager::logOutAll(void){
    foreach( UsrInfoOnline* t , _usrInfoOnlineList){
            t->setExpireTime( QDateTime::currentDateTime());
    }
}

//添加用户
QObject* UsrManager::addUsr(const QString& name, int level, const QString& pswd,
                const QString& usrDescript){
    bool ok = checkName( name );
    if( ok ){
        //用户名不冲突
        UsrInfo* newOne = new UsrInfo(this);
        newOne->setName( name , "");
        newOne->setLevel( level , "");
        newOne->setPassWord( "", pswd);
        newOne->setUsrDescript( usrDescript );
        _usrInfoList.append(newOne);
        emit msgUsrInfoListChanged();
        return newOne;
    }
    //用户名冲突
    return nullptr;
}

//删除用户
bool UsrManager::deleteUsr(const QString& name){
    foreach( UsrInfo* t , _usrInfoList){
        if( t->name() == name){
            foreach( UsrInfoOnline* o, _usrInfoOnlineList){
                if( static_cast<UsrInfo*>(o->usrInfo())->name() == name ){
                    _usrInfoOnlineList.removeOne(o);
                    o->deleteLater();
                    emit msgUsrInfoOnlineListChanged();
                }
            }
            _usrInfoList.removeOne( t );
            t->deleteLater();
            emit msgUsrInfoListChanged();
            return true;
        }
    }
    return false;
}

//获得用户列表
QList<QObject*> UsrManager::getAllUsrInfo(void){
    QList<QObject*> usrInfoList;
    foreach (UsrInfo* it, _usrInfoList) {
        QQmlEngine::setObjectOwnership(it,QQmlEngine::CppOwnership);
        usrInfoList << it;
    }
    return usrInfoList;
}

QList<QObject*> UsrManager::getAllUsrInfoOnline(void){
    QList<QObject*> usrInfoOnlineList;
    foreach (UsrInfoOnline* it, _usrInfoOnlineList) {
        QQmlEngine::setObjectOwnership(it,QQmlEngine::CppOwnership);
        usrInfoOnlineList << it;
    }
    return usrInfoOnlineList;
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
int UsrManager::usrIDGen(void)const{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int usrID = 0;
    bool duplicatedID = false;
    while( usrID ==0 || duplicatedID ){
        usrID = qrand();
        foreach(UsrInfoOnline* t, this->_usrInfoOnlineList){
            if(t->usrID() == usrID ){
                duplicatedID = true;
                continue;
            }
        }
        duplicatedID = false;
    }
    return usrID;
}

void UsrManager::load(iLoadSaveProcessor* processor){
    UsrInfo* usr;
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
    }
}

void UsrManager::save(iLoadSaveProcessor* processor){

    int count = _usrInfoList.size();
    int ret = processor->writeValue("usrInfoCounts", count );
    if(ret != 0){
        for(int i=0; i<count; i++){
            processor->moveToInstance("usrInfo", QString::number(i));
            _usrInfoList.at(i)->save(processor);
            processor->moveBackToParent();
        }
    }
}

void UsrManager::UsrInfoInit(){
    UsrInfo* superior;
    superior = new UsrInfo();
    superior->setLevel(levelSuperior, "");
    superior->setName("superior","");
    superior->setPassWord("","supp1");
    _usrInfoList.append(superior);
}
