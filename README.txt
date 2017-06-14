用户管理器2
2017.5
功能：
1、多用户登录、登出
2、用户管理，用户的添加、删除和修改
3、用户登录的状态查询
4、自动登出
5、加密存储

机制描述：
用户程序调用登录函数后，如果登录成功则可以获得一个usrID，否则返回0。
用户可以使用该usrID来验证是否通过登录。
用户管理器维护一个已登录的用户列表。

特殊要求：
能与QML交互、能与QWebChannel交互。

使用：
在Pro文件中添加：
include(../lib/usrManager/src/usrmanager.pri)

#include "usrmanager.h"
UsrManager* _manager = UsrManager::Instance();


例子：
example/

TODO:
1、事件记录接口
2、signal完善

API说明
---------------------------------------------------------------------------------------
一、UsrManager calss

    int checkLogInLevel( int usrID );
    功能：检查登录的等级。
    描述：输入usrID，返回登录的等级。登录的等级是一个0或正整数。数字越大等级越高。本模块不负责定义每个等级所对应的权限，仅提供一个等级数字。返回值为0说明该usrID没有登录，或登录的用户设置的权限最低，如Guest用户。

    bool isLogIn( int usrID );
    功能：检查是否登录。
    描述：输入usrID，若usrID已登录，返回true，否则返回false

    QObject* getUsrInfoOnline( int usrID );
    功能：查询在线用户信息
    描述：返回一个在线用户信息的实例指针。在线用户信息包含动态的登录信息，该对象由UsrManager管理，不允私自许删除！
    备注：为了方便在QML中使用，返回的指针降级为QObject*。在C++中使用时，可以用static_cast<UsrInfoOnline*> 静态转换。

    QObject* getUsrInfo( const QString& name );
    功能：获取离线用户信息
    描述：输入用户名，返回一个离线用户信息的实例指针。离线用户信息包含离线的用户名、密码、描述等。该对象由UsrManager管理，不允私自许删除！若找不到离线用户信息，则返回nullptr！
    备注：为了方便在QML中使用，返回的指针降级为QObject*。在C++中使用时，可以用static_cast<UsrInfo*> 静态转换。

    QObject* getUsrInfo( int usrID );
    功能：获取离线用户信息（重载）
    描述：某个登录的usrID。若找不到离线用户信息，或usrID未登录，则返回nullptr！

    int logIn( const QString& usrName, const QString& usrPwd, const QString& onlineUsrInfo );
    功能：登录
    描述：输入登录的用户名、密码和在线识别信息，返回登录的usrID。产生几种结果：
    1、找不到用户名和密码，返回的usrID等于0。
    2、用户名或密码错误，返回的usrID等于0。等同于情况1。
    3、用户名密码正确，与之前的在线登录的在线识别信息完全一致。属于重复登录。返回已存在的登录的usrID。更新登录时间、活动时间和失效时间。
    4、用户名密码正确，识别信息独一无二。属于新登录。记录新的登录信息，并返回一个新生成的usrID。更新登录时间、活动时间和失效时间。

    void logOut( int usrID );
    功能：登出
    描述：登出某个登录的usrID。注意：仅修改失效时间，并不会删除记录。

    void logOutAll(void);
    功能：全部登出
    描述：全部登出。注意：仅修改失效时间，并不会删除记录。

    QObject* addUsr(const QString& name, int level = 0, const QString& pswd = QString::null ,const QString& usrDescript = QString::null );
    功能：添加用户
    描述：新增用户。必填：新增的用户名称。选填：等级、密码、用户描述信息。返回线增加的用户的实例。该对象由UsrManager管理，不允私自许删除！注意用户名不能重复，否则返回nullptr！
    备注：为了方便在QML中使用，返回的指针降级为QObject*。在C++中使用时，可以用static_cast<UsrInfo*> 静态转换。

    bool deleteUsr(const QString& name);
    功能：删除用户
    描述：删除用户。输入要删除的用户名称。删除成功返回true。否则返回false。

    QList<QObject*> getAllUsrInfo(void);
    功能：获得用户列表
    描述：获得用户列表。列表中的对象由UsrManager管理，不允私自许删除！
    备注：为了方便在QML中使用，返回的指针降级为QObject*。在C++中使用时，可以用static_cast<UsrInfo*> 静态转换。

    QList<QObject*> getAllUsrInfoOnline(void);
    功能：获得在线用户列表
    描述：获得在线用户列表。列表中的对象由UsrManager管理，不允私自许删除！
    备注：为了方便在QML中使用，返回的指针降级为QObject*。在C++中使用时，可以用static_cast<UsrInfoOnline*> 静态转换。

    int activeUsrNumber(void);
    功能：用户数量统计
    描述：返回活动的用户数量

    virtual void save(iLoadSaveProcessor* processor);
    功能：保存接口
    描述：保存所有信息到iLoadSaveProcessor

    virtual void load(iLoadSaveProcessor* processor);
    功能：读取接口
    描述：从iLoadSaveProcessor读取所有信息

二、UsrInfo class

    QString name(void) const;
    功能：查询名称

    QByteArray genCryptoString(const QString &pswd);
    (2017.06.14new)功能：方便UsrManager使用，使用Algorithm( Algorithm(pswd)+name )算法，从明文密码生成加密密码。

    bool setName(const QString& newName, const QString &pwdWithoutCrypto);
    (2017.06.14modify)功能：修改用户名，成功返回ture，否则返回false
    修改用户名时，必须传入明文密码。需要它来生成新的密码。


    int level(void) const;
    功能：查询等级

    bool setLevel(const quint8& newLevel, const QByteArray& pwd);
    (2017.06.14modify)功能：修改等级，成功返回ture，否则返回false

    QString usrDescript(void)const;
    功能：查询用户描述

    bool setUsrDescript(const QString& data);
    功能：修改用户描述，成功返回ture，否则返回false

    bool passWordCheck(const QByteArray& testPswd);
    (2017.06.14modify)功能：检查密码，正确返回ture，否则返回false

    bool setPassWord(const QByteArray& oldPswd, const QByteArray& newPswd);
    (2017.06.14modify)功能：修改密码，成功返回ture，否则返回false

三、UsrInfoOnline class

    QObject* usrInfo(void);
    功能：查询离线用户信息。
    描述：每个在线用户信息都包含一个指针，指向与之对应的离线用户信息。该函数可以返回一个离线用户信息的实例指针。该对象由UsrManager管理，不允私自许删除！
    备注：为了方便在QML中使用，返回的指针降级为QObject*。在C++中使用时，可以用static_cast<UsrInfo*> 静态转换。

    int usrID(void);
    功能：查询usrID。usrID由UsrManager动态生成和分配。

    bool setUsrID(int id);
    功能：设置usrID。该功能只能由UsrManager使用。

    QString onlineUsrInfo(void)const;
    功能：查询在线识别信息
    描述：字符串。可以使IP地址+端口地址、浏览器型号、mac地址、手机串号等等独一无二的标志组成。

    bool setOnlineUsrInfo(const QString& info);
    功能：设置在线识别信息
    描述：由登录的时候同时传入。

    QDateTime expireTime(void)const;
    功能：获取失效时间。当系统时间大于失效时间后，登录失效。

    bool setExpireTime(const QDateTime& time);
    功能：设置失效时间。

    QDateTime loginTime(void)const;
    功能：获取登录时间。

    bool setLoginTime(void);
    功能：设备当前时间为登录时间。

    QDateTime activeTime(void)const;
    功能：获取最后活动时间。

    bool setActiveTime(void);
    功能：设置当前时间为最后活动时间。


