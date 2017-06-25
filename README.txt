用户管理器2
v2.02 2017.6

以后修改记录在git commit中

修改：
1、把UsrID改成QByteArray， 使用UDID生成
2、1的其他适配的修改。
3、UsrInfoOnline类更名为 SessionInfo类。表示它是代表一个在线会话
4、SessionInfo类内部结构大修改，增加一个QVariantMap负责所有参数的存储功能。在线识别信息改为：<Identifier,“data”>，是必备的初始化数据。它用来识别会话客户端。
5、SessionInfo增加一个友元类UsrManager。manager访问SessionInfo更简单。
6、在界面上，SessionInfo增加一列：是否有效。
7、API说明分散到各个h文件中。
8、修改说明放到tag描述中。
9、readme.txt存放功能说明。

v2.01 2017.6
修改：
1、配合网页的通用加密方式，修改密码存储加密方式为：algorith(algorith(pswd) + name) , 其中algorith = MD5
2、前台到后台传输时加密
3、密码使用QByteArray传输
4、其他适配的修改。
新增：
1、内置的基于widget的管理UI

v2.0 2017.5
功能：
1、多用户登录、登出
2、后台用户管理：用户的添加、删除和修改
3、在线Session的状态查询、登录、自动登出等控制。
4、加密存储
5、已经实现基于Widget的后台用户管理UI
6、事件记录

流程描述：
1、用户管理器提供登录、登出、查询登录Session等函数。用户页面发送登录请求，如果登录成功则可以获得一个sessionID。
2、用户页面可以使用该sessionID用于webserver的其他功能。
3、用户管理器提供注册用户、删除用户、用户页面用户页面
4、用户管理器提供一个后台UI，可以在线管理用户，注册、删除、修改用户。
4、用户等级必须大于等于1，小于等于200。本模块不负责定义每个等级所对应的权限，仅提供一个等级数字。

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
