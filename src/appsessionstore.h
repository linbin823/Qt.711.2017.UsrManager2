#ifndef APPSESSIONSTORE_H
#define APPSESSIONSTORE_H

#include <Tufao/SessionStore>
#include "usrmanager.h"
#include "iloadsave.h"

class AppSessionStore : public Tufao::SessionStore, public iLoadSave
{
    Q_OBJECT
public:
    /*!
     * 构造函数
     * 传入一个SessionSettings，它规定了session cookie的基本特征。
     */
    explicit AppSessionStore(const Tufao::SessionSettings &settings = defaultSettings(),
                                QObject *parent = 0);

    /*!
     * 析构函数
     */
    ~AppSessionStore();

    /*!
     * 用于修改Session Cookie的设置
     */
    Tufao::SessionSettings& settings(void);

    /*!
     * 登录，返回bool
     * 输入登录的用户名、密码和在线识别信息，返回登录的sessionID。产生几种结果：
     * 1、找不到用户名和密码，返回false。
     * 2、用户名或密码错误，返回的false。等同于情况1。
     * 3、用户名密码正确。新登录。记录新的登录信息，并返回true。更新登录时间、活动时间和失效时间。
     * 4、登录成功后，在response中写入sessionID
     */
    bool logIn(const QString& usrName, const QByteArray &usrPwd, Tufao::HttpServerResponse &response);

    ////implements abstract class Tufao::SessionStore
    /*!
     * 是否有Session
     * 在request中找session cookie的sessionID（按照cookie name来找）
     * 找到了且该Session已经登录，返回true
     */
    bool hasSession(const Tufao::HttpServerRequest &request) const override;

    /*!
     * 删除Session
     * 在response和request中找session cookie的sessionID
     * 找不到就算了，找到了先在respone中加入unsetcookie
     * 然后登出sessionID
     */
    void removeSession(const Tufao::HttpServerRequest &request,
                       Tufao::HttpServerResponse &response) override;

    /*!
     * 增加Session
     * 在response中增加cookie
     */
    void setSession(Tufao::HttpServerResponse &response, const QByteArray &session)
        const;

    /*!
     * 读取Session所有属性keys
     * 在response和request中找session cookie的sessionID
     * 找不到返回QList<QByteArray>()
     * 找到了返回store中sessionID的对应的session的属性keys
     */
    QList<QByteArray> properties(const Tufao::HttpServerRequest &request,
                                 const Tufao::HttpServerResponse &response) const override;

    /*!
     * 判断是否有某一个属性
     * 在response和request中找session cookie的sessionID
     * 找不到返回false
     * sessionID未登录返回false
     * session有名为key的属性返回true，没有则返回fasle
     */
    bool hasProperty(const Tufao::HttpServerRequest &request,
                     const Tufao::HttpServerResponse &response,
                     const QByteArray &key) const override;

    /*!
     * 读取某一个属性
     * 在response和request中找session cookie的sessionID
     * 找不到返回QVariant()
     * sessionID未登录返回QVariant()
     * 返回session[key]的值
     * 算一次活动，延长session失效时间，发出cookie
     */
    QVariant property(const Tufao::HttpServerRequest &request,
                      Tufao::HttpServerResponse &response,
                      const QByteArray &key) const override;

    /*!
     * 写入某一个属性
     * 在response和request中找session cookie的sessionID
     * 找不到返回
     * sessionID未登录返回
     * 写入session[key]的值
     * 算一次活动，延长session失效时间，发出cookie
     */
    void setProperty(const Tufao::HttpServerRequest &request,
                     Tufao::HttpServerResponse &response, const QByteArray &key,
                     const QVariant &value) override;

    /*!
     * 删除某一个属性
     * 在response和request中找session cookie的sessionID
     * 找不到返回
     * sessionID未登录返回
     * 删除session[key]的值
     * 算一次活动，延长session失效时间，发出cookie
     */
    void removeProperty(const Tufao::HttpServerRequest &request,
                        Tufao::HttpServerResponse &response,
                        const QByteArray &key) override;

    /*!
     * 返回一个默认的AppSessionStore的实例
     * 方便没有要求的情况
     */
    static AppSessionStore &defaultInstance();

    /*!
     * 保存功能
     */
    void save(iLoadSaveProcessor* processor) override;

    /*!
     * 读取功能
     */
    void load(iLoadSaveProcessor* processor) override;

    /*!
     * 显示管理页面
     */
    void showUI(bool show=true, QWidget* parent=0);


private:
    UsrManager* _mgr;

private slots:
    void onTimeOutAftLogInChanged(long newValue);
};

#endif // APPSESSIONSTORE_H
