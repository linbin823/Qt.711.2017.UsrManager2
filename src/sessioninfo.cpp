#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QQmlEngine>
#include <QDateTime>
#include "usrinfo.h"
#include "sessioninfo.h"

SessionInfo::SessionInfo(UsrInfo* usrInfo, QObject *parent): QObject(parent)
{
    _usrInfo = usrInfo;
    _activeTime = QDateTime::currentDateTime();
    _expireTime = QDateTime::currentDateTime();
    _loginTime  = QDateTime::currentDateTime();
    connect(_usrInfo, &UsrInfo::nameChanged,
            this,&SessionInfo::propertyChanged);
    connect(_usrInfo, &UsrInfo::levelChanged,
            this,&SessionInfo::propertyChanged);
    connect(_usrInfo, &UsrInfo::usrDescriptChanged,
            this,&SessionInfo::propertyChanged);
    propertyChanged();
}

SessionInfo::~SessionInfo(){

}

QVariant& SessionInfo::property(const QString& key){
    static QVariant emptyValue = QVariant();
    if(_properties.contains(key))
        return _properties[key];
    return emptyValue;
}

// "identifier","usrName","usrLevel","usrDescription"是保留的关键字。
void SessionInfo::unsetProperty(const QString& key){
    if(key=="idemtifier" || key=="usrName" || key=="usrLevel" || key=="usrDescription") return;
    int ret = _properties.remove(key);
    if(ret>0) emit propertiesChanged();
}

// "identifier","usrName","usrLevel","usrDescription"是保留的关键字。
void SessionInfo::setProperty(const QString& key, const QVariant &value){
    if(key=="idemtifier" || key=="usrName" || key=="usrLevel" || key=="usrDescription") return;
    _properties.insert(key,value);
    emit propertiesChanged();
}

QDateTime& SessionInfo::expireTime(void){
    return _expireTime;
}
void SessionInfo::setExpireTime(const QDateTime& time){
    if(_expireTime != time){
        _expireTime = time;
        emit expireTimeChanged();
    }
}

QDateTime& SessionInfo::loginTime(void){
    return _loginTime;
}
void SessionInfo::setLoginTime(void){
    _loginTime = QDateTime::currentDateTime();
    emit loginTimeChanged();
}

QDateTime& SessionInfo::activeTime(void){
    return _activeTime;
}

void SessionInfo::setActiveTime(long sec){
    _activeTime = QDateTime::currentDateTime();
    emit activeTimeChanged();
    if(sec!=0){
        _expireTime = _activeTime.addSecs(sec);
        emit expireTimeChanged();
    }
}

void SessionInfo::save(iLoadSaveProcessor* processor){
    processor->writeValue("expireTime", _expireTime);
    processor->writeValue("loginTime", _loginTime);
    processor->writeValue("activeTime", _activeTime);
    int num =_properties.count();
    QString key;
    QString value;
    QVariantMap::const_iterator it;
    processor->writeValue("propertiesNumber", num );
    int i=0;
    for(it = _properties.constBegin(); it != _properties.constEnd(); ++it,i++){
        processor->moveToInstance("properties",QString::number(i));
        QString temp = it.key();
        QVariant temp1=it.value();
        processor->writeValue("key",temp );
        processor->writeValue("value",temp1 );
        processor->moveBackToParent();
    }
}

void SessionInfo::load(iLoadSaveProcessor* processor){
    processor->readValue("expireTime", _expireTime);
    processor->readValue("loginTime", _loginTime);
    processor->readValue("activeTime", _activeTime);
    _properties.clear();
    int ret=0,num =0;
    QString key;
    QString value;
    ret = processor->readValue("propertiesNumber", num);
    if(ret !=0 ) return;//not found num
    for(int i=0; i<num; i++){
        ret = processor->moveToInstance("properties",QString::number(i));
        if(ret !=0 ) continue;//not found Instance
        ret = processor->readValue("key",key);
        if(ret !=0 ) continue;//not found num
        ret = processor->readValue("value",value);
        if(ret !=0 ) continue;//not found value
        _properties.insert(key, QVariant(value));
        processor->moveBackToParent();
    }
    emit propertiesChanged();
}

void SessionInfo::propertyChanged(){
    _properties.insert("usrName", QVariant::fromValue(_usrInfo->name()) );
    _properties.insert("usrLevel", QVariant::fromValue(_usrInfo->level()) );
    _properties.insert("usrDescription", QVariant::fromValue(_usrInfo->usrDescript()) );
    emit propertiesChanged();
}
