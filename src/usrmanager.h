#ifndef USRMANAGER_H
#define USRMANAGER_H

#include "usrinfo.h"
#include "sessioninfo.h"
#include "iloadsave.h"
#include <QObject>
#include <QList>
#include <QTimer>

QT_BEGIN_NAMESPACE
class UsrManagerUI;
QT_END_NAMESPACE

//2017.05重写用户权限管理
//可以在QML、QWebChannel中使用
class UsrManager  : public QObject, public iLoadSave
{
    Q_OBJECT

    Q_PROPERTY(long timeOutAftLogIn             READ timeOutAftLogIn    WRITE setTimeOutAftLogIn    NOTIFY msgTimeOutAftLogInChanged)
    Q_PROPERTY(QList<UsrInfo*> allUsrInfo       READ allUsrInfo         NOTIFY msgUsrInfoListChanged)
    Q_PROPERTY(QHash<QByteArray, SessionInfo*>  allSessionInfo   READ allSessionInfo     NOTIFY msgSessionInfoListChanged)

private:
    explicit UsrManager(QObject *parent = 0);
    //单例设计
    static UsrManager* _singleton;

public:
    static UsrManager* Instance();
    ~UsrManager();

    SessionInfo& operator [](const QByteArray& id){
        if( _sessionInfoList.contains(id)){
            return *(_sessionInfoList[id]);
        }else{
            static UsrInfo* uErr = new UsrInfo(this);
            static SessionInfo* sErr = new SessionInfo( uErr,this);
            return *sErr;
        }
    }

public slots:

    /*!
     * 检查登录的等级。
     * 输入sessionID，返回登录的等级。登录的等级是一个0或正整数。数字越大等级越高。
     * 本模块不负责定义每个等级所对应的权限，仅提供一个等级数字。
     * 返回值为0说明该sessionID没有登录。
     * 不更新失效时间，不更新最后活动时间
     */
    int checkLogInLevel( const QByteArray& sessionID );

    /*!
     * 检查是否登录。
     * 输入sessionID，若sessionID已登录，返回true，否则返回false
     */
    bool isLogIn( const QByteArray& sessionID );

    /*!
     * 查询会话信息
     * 返回一个session信息的实例指针。session信息包含动态的登录信息，
     * 该对象由UsrManager管理，不允私自许删除！
     * 若找不到session时，则返回nullptr！
     * 另一方面，失效的session不会被删除，此方法可以查询到运行后所有的session。
     * 不更新失效时间，不更新最后活动时间
     */
    QObject* sessionInfo(const QByteArray &sessionID )const;

    /*!
     * 获取用户信息
     * 输入用户名，返回一个用户信息的实例指针。用户信息包含用户名、密码、描述等。
     * 该对象由UsrManager管理，不允私自许删除！若找不到用户信息，则返回nullptr！
     */
    QObject* usrInfo( const QString& name )const;

    /*!
     * 获取用户信息（重载）
     * 某个登录的sessionID。若找不到用户信息，或sessionID未登录，则返回nullptr！
     */
    QObject* usrInfo(const QByteArray &sessionID )const;

    /*!
     * 获取单次登录的有效期（秒）
     */
    long timeOutAftLogIn(void)const;

    /*!
     * 设置单次登录的有效期（秒）
     */
    void setTimeOutAftLogIn(long newValue);

    /*!
     * 登录，返回usrID
     * 输入登录的用户名、密码和在线识别信息，返回登录的sessionID。产生几种结果：
     * 1、找不到用户名和密码，返回的sessionID等于QByteArray()。
     * 2、用户名或密码错误，返回的sessionID等于QByteArray()。等同于情况1。
     * 3、用户名密码正确。新登录。记录新的登录信息，并返回一个新生成的sessionID。更新登录时间、活动时间和失效时间。
     */
    QByteArray& logIn(const QString& usrName, const QString &usrPwd);

    /*!
     * 登出
     * 登出某个登录的sessionID。注意：仅修改会话的失效时间和状态，并不会删除记录。
     */
    void logOut( QByteArray sessionID );

    /*!
     * 全部登出
     * 全部登出。注意：仅修改会话的失效时间和状态，并不会删除记录。
     */
    void logOutAll(void);

    /*!
     * 添加用户
     * 新增用户。必填：新增的用户名称。选填：等级、密码、是否独占。
     * 注意：修改用户名或新增用户时，必须传递一个未加密的密码
     * 返回线增加的用户的实例。该对象由UsrManager管理，不允私自许删除！
     * 注意用户名不能重复，否则返回nullptr！
     * 为了方便在QML中使用，返回的指针降级为QObject*。在C++中使用时，可以用static_cast<UsrInfo*> 静态转换。
     */
    QObject* addUsr(const QString& name, int level = 1, const QString &pwdWithoutCrypto = "" ,
                    bool exclusive = true );

    /*!
     * 删除用户
     * 删除用户。输入要删除的用户名称。删除成功返回true。否则返回false。
     */
    bool deleteUsr(const QString& name);

    /*!
     * 获得用户列表
     * 获得用户列表。列表中的对象由UsrManager管理，不允私自许删除！
     */
    QList<UsrInfo*>& allUsrInfo(void);

    /*!
     * 获得会话列表
     * 获得会话列表。列表中的对象由UsrManager管理，不允私自许删除！
     */
    QHash<QByteArray, SessionInfo*>& allSessionInfo(void);

    /*!
     * 有效的会话数量统计
     */
    int activeSessionNumber(void);

    /*!
     * 总的会话数量统计
     */
    int totalSessionNumber(void);

    /*!
     * 无效的会话数量统计
     */
    int disactiveSessionNumber(void);

    /*!
     * 保存 实现iLoadSave
     */
    void save(iLoadSaveProcessor* processor);

    /*!
     * 读取 实现iLoadSave
     */
    void load(iLoadSaveProcessor* processor);

    /*!
     * 检查用户名是否重名
     */
    bool checkName(const QString &newName) const;

    /*!
     * 显示管理页面
     */
    void showUI(bool show=true, QWidget* parent=0);

signals:
    void msgUsrInfoListChanged();
    void msgSessionInfoListChanged();
    void msgTimeOutAftLogInChanged(long newValue);
    void msgEventString(const QString& msg);

private:
    QHash<QByteArray,SessionInfo*> _sessionInfoList;
    QList<UsrInfo*> _usrInfoList;
    long _secsTimeOutAftLogIn;
    UsrManagerUI* _pUI;

    //生成UsrID
    inline QByteArray _sessionIDGen(void)const;
    //初始化用户列表
    void _usrInfoInit(void);

private slots:
    //事件转发
    void _usrInfoChanged(void);
};

#endif // USRMANAGER_H
