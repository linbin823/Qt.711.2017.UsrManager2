#ifndef USRMANAGER_H
#define USRMANAGER_H

#include <QObject>
#include <QList>
#include <QTimer>

#include "iloadsaveprocessor.h"
#include "usrinfo.h"
#include "usrinfoonline.h"

//2017.05重写用户权限管理
//可以在QML、QWebChannel中使用
class UsrManager  : public QObject, public iLoadSave
{
    Q_OBJECT
private:
    explicit UsrManager(QObject *parent = 0);
    //单例设计
    static UsrManager* _singleton;



public:
    static UsrManager* Instance();

    ~UsrManager();
    //等级设计
    enum level{
        levelSuperior                    = 0xFF,      //工程权限
        levelMantainance                 = 0x03,      //最高权限，维护工程师
        levelAdministartor               = 0x02,      //管理员权限
        levelOperator                    = 0x01,      //操作员权限
        levelAnonymous                   = 0x00,      //未登录
    };


public slots:
    //查询登录状态，输入usrID，返回权限等级
    int checkLogInLevel( int usrID );
    //usrID已登录，返回true，否则返回false
    bool isLogIn( int usrID );
    //查询在线信息
    QObject* getUsrInfoOnline( int usrID );
    //获取用户信息
    QObject* getUsrInfo( const QString& name );
    QObject* getUsrInfo( int usrID );


    //登录，返回usrID
    int logIn( const QString& usrName, const QString& usrPwd, const QString& onlineUsrInfo );
    //登出
    void logOut( int usrID );
    //全部登出
    void logOutAll(void);

    //添加用户
    QObject* addUsr(const QString& name, int level = 0, const QString& pswd = QString::null ,
                    const QString& usrDescript = QString::null );
    //删除用户
    bool deleteUsr(const QString& name);

    //获得用户列表
    QList<QObject*> getAllUsrInfo(void);
    QList<QObject*> getAllUsrInfoOnline(void);

    virtual void save(iLoadSaveProcessor* processor);
    virtual void load(iLoadSaveProcessor* processor);

signals:
    void msgUsrInfoListChanged();
    void msgUsrInfoOnlineListChanged();

private:
    QList<UsrInfoOnline*> _usrInfoOnlineList;
    QList<UsrInfo*> _usrInfoList;

    //检查用户名是否重名
    bool checkName(const QString &newName) const;
    //生成UsrID
    int  usrIDGen(void)const;

    void UsrInfoInit(void);
};

#endif // USRMANAGER_H
