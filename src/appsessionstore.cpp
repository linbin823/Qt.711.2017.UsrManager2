#include "appsessionstore.h"
#include "usrinfo.h"
#include "sessioninfo.h"
#include <QUuid>

AppSessionStore::AppSessionStore(const Tufao::SessionSettings &settings,
                                       QObject *parent) :
    Tufao::SessionStore(settings, parent)
{
    _mgr = UsrManager::Instance();

    //parent's settings' timeout always following the UsrManager.
    onTimeOutAftLogInChanged( _mgr->timeOutAftLogIn() );
    connect(_mgr, &UsrManager::msgTimeOutAftLogInChanged,
            this, &AppSessionStore::onTimeOutAftLogInChanged);
}

AppSessionStore::~AppSessionStore()
{

}
Tufao::SessionSettings& AppSessionStore::settings(void){
    return Tufao::SessionStore::settings;
}
bool AppSessionStore::logIn(const QString& usrName, const QByteArray &usrPwd, Tufao::HttpServerResponse &response){
    QByteArray& sID = _mgr->logIn(usrName, usrPwd );
    if(sID != QByteArray() ){
    //successed
        //必须先要设置一个SessionID，后面的setProperty才能知道
        setSession(response, sID);
        return true;
    }
    return false;
}

bool AppSessionStore::hasSession(const Tufao::HttpServerRequest &request) const
{
    QByteArray session(Tufao::SessionStore::session(request));

    if (session.isEmpty() || !_mgr->isLogIn(session) )
        return false;

    return true;
}

void AppSessionStore::removeSession(const Tufao::HttpServerRequest &request,
                                       Tufao::HttpServerResponse &response)
{
    QByteArray session(Tufao::SessionStore::session(request, response));

    if (session.isEmpty())
        return;

    unsetSession(response);

    if ( !_mgr->isLogIn(session ) )
        return;

    _mgr->logOut(session );
}

void AppSessionStore::setSession(Tufao::HttpServerResponse &response,
                                 const QByteArray &session) const{
    SessionStore::setSession( response, session);
}

QList<QByteArray>
AppSessionStore::properties(const Tufao::HttpServerRequest &request,
                               const Tufao::HttpServerResponse &response) const
{
    QByteArray session(Tufao::SessionStore::session(request, response));

    if (session.isEmpty() || !_mgr->isLogIn(session) )
        return QList<QByteArray>();

    QList<QString> keys( (*_mgr)[session].properties().keys());
    QList<QByteArray> ret;

    ret.reserve(keys.size());

    for (int i = 0;i != keys.size();++i)
        ret += keys[i].toUtf8();

    return ret;
}

bool AppSessionStore::hasProperty(const Tufao::HttpServerRequest &request,
                                     const Tufao::HttpServerResponse &response,
                                     const QByteArray &key) const
{
    QByteArray session(Tufao::SessionStore::session(request, response));

    if (session.isEmpty() || !_mgr->isLogIn(session) )
        return false;

    return (*_mgr)[session].properties().contains(key);
}

QVariant AppSessionStore::property(const Tufao::HttpServerRequest &request,
                                      Tufao::HttpServerResponse &response,
                                      const QByteArray &key) const
{
    QByteArray session(Tufao::SessionStore::session(request, response));

    if (session.isEmpty())
        return QVariant();

    if (!_mgr->isLogIn(session)) {
        // possibly avoid useless future queries
        unsetSession(response);

        return QVariant();
    }

    // change session expire time
    (*_mgr)[session].setExpireTime( QDateTime::currentDateTime().addSecs(_mgr->timeOutAftLogIn() ));

    // update cookie (expire time)
    setSession(response, session);

    const QVariantMap &sessionData( (*_mgr)[session].properties() );

    if (!sessionData.contains(key))
        return QVariant();

    return sessionData[key];
}

void AppSessionStore::setProperty(const Tufao::HttpServerRequest &request,
                                     Tufao::HttpServerResponse &response,
                                     const QByteArray &key,
                                     const QVariant &value)
{
    // init session variable
    QByteArray session(Tufao::SessionStore::session(request, response));

    if ( session.isEmpty() || !_mgr->isLogIn(session) )
        return;

    // set property
    (*_mgr)[session][key] = value;

    // change session expire time
    (*_mgr)[session].setExpireTime( QDateTime::currentDateTime().addSecs(_mgr->timeOutAftLogIn() ));

    // create, if not set yet, and update cookie (expire time)
    setSession(response, session);
}

void AppSessionStore::removeProperty(const Tufao::HttpServerRequest &request,
                                        Tufao::HttpServerResponse &response,
                                        const QByteArray &key)
{
    // init session variable
    QByteArray session(Tufao::SessionStore::session(request, response));

    if (session.isEmpty() || !_mgr->isLogIn(session) )
        return;

    // remove property
    (*_mgr)[session].properties().remove(key);

    // change session expire time
    (*_mgr)[session].setExpireTime( QDateTime::currentDateTime().addSecs(_mgr->timeOutAftLogIn() ));

    // update cookie (expire time)
    setSession(response, session);
}

AppSessionStore &AppSessionStore::defaultInstance()
{
    static AppSessionStore instance;
    return instance;
}

void AppSessionStore::save(iLoadSaveProcessor* processor){
    processor->moveToInstance("UsrManager","");
    _mgr->save(processor);
    processor->moveBackToParent();
}

void AppSessionStore::load(iLoadSaveProcessor* processor){
    processor->moveToInstance("UsrManager","");
    _mgr->load(processor);
    processor->moveBackToParent();
}

void AppSessionStore::showUI(bool show, QWidget* parent){
    _mgr->showUI(show,parent);
}

void AppSessionStore::onTimeOutAftLogInChanged(long newValue){
    Tufao::SessionStore::settings.timeout = newValue / 60;
}
