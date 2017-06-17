#ifndef USRMANAGER_H
#define USRMANAGER_H

#include <QObject>
#include <QList>
#include <QTimer>

#include "iloadsave.h"
#include "usrinfo.h"
#include "usrinfoonline.h"

QT_BEGIN_NAMESPACE
class UsrManagerUI;
QT_END_NAMESPACE

//2017.05重写用户权限管理
//可以在QML、QWebChannel中使用
class UsrManager  : public QObject, public iLoadSave
{
    Q_OBJECT

    Q_PROPERTY(long timeOutAftLogIn             READ timeOutAftLogIn    WRITE setTimeOutAftLogIn    NOTIFY msgTimeOutAftLogInChanged)
    Q_PROPERTY(QList<QObject*> allUsrInfo       READ allUsrInfo         NOTIFY msgUsrInfoListChanged)
    Q_PROPERTY(QList<QObject*> allUsrInfoOnline READ allUsrInfoOnline   NOTIFY msgUsrInfoOnlineListChanged)

private:
    explicit UsrManager(QObject *parent = 0);
    //单例设计
    static UsrManager* _singleton;

public:
    static UsrManager* Instance();

    ~UsrManager();
public slots:
    //查询登录状态，输入usrID，返回权限等级
    int checkLogInLevel( QByteArray usrID );
    //usrID已登录，返回true，否则返回false
    bool isLogIn( QByteArray usrID );
    //查询在线信息
    QObject* usrInfoOnline( QByteArray usrID );
    //获取用户信息
    QObject* usrInfo( const QString& name );
    QObject* usrInfo( QByteArray usrID );
    //查询单次登录的有效期（秒）
    long timeOutAftLogIn(){
        return _secsTimeOutAftLogIn;
    }
    //设置单次登录的有效期（秒）
    void setTimeOutAftLogIn(long newValue);

    //登录，返回usrID
    QByteArray logIn( const QString& usrName, const QString& usrPwd, const QString& onlineUsrInfo );
    //登出
    void logOut( QByteArray usrID );
    //全部登出
    void logOutAll(void);

    //添加用户
    QObject* addUsr(const QString& name, int level = 1, const QString& pswd = QString() ,
                    const QString& usrDescript = QString() );
    //删除用户
    bool deleteUsr(const QString& name);

    //获得用户列表
    QList<QObject*> allUsrInfo(void);
    QList<QObject*> allUsrInfoOnline(void);

    //用户数量统计
    int activeOnlineUsrNumber(void);
    int totalOnlineUsrNumber(void);
    int offlineUsrNumber(void);

    virtual void save(iLoadSaveProcessor* processor);
    virtual void load(iLoadSaveProcessor* processor);

    //检查用户名是否重名
    bool checkName(const QString &newName) const;

    //显示管理页面
    void showUI(QWidget* parent = 0);
signals:
    void msgUsrInfoListChanged();
    void msgUsrInfoOnlineListChanged();
    void msgTimeOutAftLogInChanged(long newValue);
    void msgEventString(const QString& msg);

private:
    QList<UsrInfoOnline*> _usrInfoOnlineList;
    QList<UsrInfo*> _usrInfoList;
    long _secsTimeOutAftLogIn;
    UsrManagerUI* _pUI;

    //生成UsrID
    QByteArray  _usrIDGen(void)const;

    void _usrInfoInit(void);

private slots:
    void _usrInfoChanged(void);
};

#endif // USRMANAGER_H
