#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "usrinfo.h"

UsrInfo::UsrInfo(QObject *parent, QCryptographicHash::Algorithm alg):
    QObject(parent),_alg(alg)
{
    _properties.insert("name",      QVariant("undefined"));
    _properties.insert("password",  QVariant(genCryptoString( name(), QString(""), _alg)));
    _properties.insert("level",     QVariant(1));
    _properties.insert("exclusive", QVariant(true));
}

UsrInfo::~UsrInfo(){

}

QString UsrInfo::genCryptoString(const QString &name,
                                 const QString &pwdWithoutCrypto, QCryptographicHash::Algorithm alg){
    QCryptographicHash hash(alg);

    QByteArray temp = pwdWithoutCrypto.toUtf8();
    hash.addData(temp);
    temp = hash.result();

    temp = temp.toHex() + name.toUtf8();

    hash.reset();
    hash.addData(temp);
    temp = hash.result();
    return temp.toHex();
//QCryptographicHash有连续编码设计
//前面的结果是自动添加迭代的
//    QByteArray ba = QCryptographicHash::hash( pwdWithoutCrypto.toUtf8(), alg);
//    ba = _cryptoHexToString(ba) + name.toUtf8();
//    ba = QCryptographicHash::hash( ba, alg);
//    ba = _cryptoHexToString(ba);
//    return QString(ba);
}

QString UsrInfo::name(){
    return _properties["name"].toString();
}

int UsrInfo::level() const{
    return _properties["level"].toInt();
}

bool UsrInfo::passWordCheck(const QString &testPswd){
    if(testPswd == _properties["password"] ){
        return true;
    }
    return false;
}

bool UsrInfo::setName(const QString& newName, const QString &pwdWithoutCrypto){
    QString temp =  genCryptoString(name(), pwdWithoutCrypto, _alg);
    if(passWordCheck(temp)){
        if( _properties["name"] != newName ){
            _properties["name"] = newName;
            _properties["password"] = genCryptoString(newName, pwdWithoutCrypto, _alg);
            emit propertiesChanged();
            return true;
        }
    }
    return false;
}

bool UsrInfo::setLevel(int newLevel, const QString &cryptoPwd){
    if(passWordCheck(cryptoPwd)){
        if( _properties["level"]!=newLevel && newLevel >=1 && newLevel <=200 ){
            _properties["level"] = newLevel;
            emit propertiesChanged();
            return true;
        }
    }
    return false;
}

bool UsrInfo::exclusive(void)const{
    return _properties["exclusive"].toBool();
}

bool UsrInfo::setExclusive(bool exclusive, const QString& pwd){
    if(passWordCheck(pwd)){
        if(exclusive != _properties["exclusive"]){
            _properties["exclusive"] = exclusive;
            emit propertiesChanged();
            return true;
        }
    }
    return false;
}

bool UsrInfo::setPassWord(const QString &oldCryptoPswd, const QString &newCryptoPswd){
    if(passWordCheck(oldCryptoPswd)){
        if(newCryptoPswd != _properties["password"]){
            _properties["password"] = newCryptoPswd;
            emit propertiesChanged();
            return true;
        }
    }
    return false;
}

QVariant& UsrInfo::property(const QString& key){
    return _properties[key];
}

//不允许删除"name","password","level","exclusive"
bool UsrInfo::unsetProperty(const QString& key, const QString &pwd){
    if(key=="name" || key=="password" || key=="level" || key=="exclusive") return false;
    if(passWordCheck(pwd)){
        int ret = _properties.remove(key);
        if(ret>0){
            emit propertiesChanged();
            return true;
        }
    }
    return false;
}

//"name","password"需要特殊修改方法。使用专用API修改
bool UsrInfo::setProperty(const QString& key, const QVariant& value, const QString& pwd){
    if(key=="name" || key=="password") return false;
    if(passWordCheck(pwd)){
        _properties.insert(key,value);
        emit propertiesChanged();
        return true;
    }
    return false;
}

void UsrInfo::save(iLoadSaveProcessor* processor){
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

void UsrInfo::load(iLoadSaveProcessor* processor){
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
