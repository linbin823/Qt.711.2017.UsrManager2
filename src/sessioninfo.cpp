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

    _properties.insert("expireTime",QVariant(QDateTime::currentDateTime() ));
    _properties.insert("identifier",QVariant("" ));
    emit propertiesChanged();
}

SessionInfo::~SessionInfo(){

}

QVariant& SessionInfo::property(const QString& key){
    return _properties[key];
}

//不允许删除"identifier","expireTime"
void SessionInfo::unsetProperty(const QString& key){
    if(key=="identifier" || key=="expireTime") return;
    int ret = _properties.remove(key);
    if(ret>0) emit propertiesChanged();
}

void SessionInfo::setProperty(const QString& key, const QVariant &value){
    _properties.insert(key,value);
    emit propertiesChanged();
}

QDateTime SessionInfo::expireTime(void)const{
    return _properties["expireTime"].toDateTime();
}
void SessionInfo::setExpireTime(const QDateTime& time){
    if(_properties["expireTime"] != time){
        _properties["expireTime"] = time;
        emit propertiesChanged();
    }
}
QString SessionInfo::identifier(void)const{
    return _properties["identifier"].toString();
}

bool SessionInfo::setIdentifier(const QString& id){
    _properties.insert("identifier", id);
    return true;
}

void SessionInfo::save(iLoadSaveProcessor* processor){
    int num =_properties.count();
    QString key;
    QVariant value;
    QVariantMap::const_iterator it;
    processor->writeValue("propertiesNumber", num );
    int i=0;
    for(it = _properties.constBegin(); it != _properties.constEnd(); ++it,i++){
        processor->moveToInstance("properties",QString::number(i));
        key = it.key();
        value=it.value();
        processor->writeValue("key",key );
        processor->writeValue("value",value );
        processor->moveBackToParent();
    }
}

void SessionInfo::load(iLoadSaveProcessor* processor){
    _properties.clear();
    int ret=0,num =0;
    QString key;
    QVariant value;
    ret = processor->readValue("propertiesNumber", num);
    if(ret !=0 ) return;//not found num
    for(int i=0; i<num; i++){
        ret = processor->moveToInstance("properties",QString::number(i));
        if(ret !=0 ) continue;//not found Instance
        ret = processor->readValue("key",key);
        if(ret !=0 ) continue;//not found num
        ret = processor->readValue("value",value);
        if(ret !=0 ) continue;//not found value
        _properties.insert(key, value);
        processor->moveBackToParent();
    }
    emit propertiesChanged();
}
