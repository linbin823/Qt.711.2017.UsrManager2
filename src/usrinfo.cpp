#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "usrinfo.h"

UsrInfo::UsrInfo(QObject *parent, QCryptographicHash::Algorithm alg):
    QObject(parent),_alg(alg)
{
    _name = "undefined";
    _pswd = QCryptographicHash::hash( QString("").toUtf8() , _alg);
    _pswd = QCryptographicHash::hash( (_pswd + _name.toUtf8() ), _alg);
    _level = 1;
}

UsrInfo::~UsrInfo(){

}

QByteArray UsrInfo::genCryptoString(const QString &name, const QString &pswd, QCryptographicHash::Algorithm alg){
    QByteArray ret = QCryptographicHash::hash( pswd.toUtf8(), alg);
    ret = QCryptographicHash::hash( (ret + name.toUtf8() ), alg);
    return ret;
}

QString& UsrInfo::name(){
    return _name;
}

int UsrInfo::level() const{
    return _level;
}

bool UsrInfo::passWordCheck(const QByteArray& testPswd){
    if(testPswd == _pswd ){
        return true;
    }
    return false;
}

bool UsrInfo::setName(const QString& newName, const QString &pwdWithoutCrypto){
    QByteArray temp =  genCryptoString(_name, pwdWithoutCrypto, _alg);
    if(passWordCheck(temp)){
        if(_name != newName ){
            _name = newName;
            _pswd = genCryptoString(_name, pwdWithoutCrypto, _alg);
            emit nameChanged();
            return true;
        }
    }
    return false;
}

bool UsrInfo::setLevel(int newLevel, const QByteArray &cryptoPwd){
    if(passWordCheck(cryptoPwd)){
        if(_level!=newLevel && newLevel >=1 && newLevel <=200 ){
            _level = newLevel;
            emit levelChanged();
            return true;
        }
    }
    return false;
}

QString& UsrInfo::usrDescript(void){
    return _usrDescript;
}

bool UsrInfo::setUsrDescript(const QString& data , const QByteArray &cryptoPwd){
    if(passWordCheck(cryptoPwd)){
        if(data != _usrDescript){
            _usrDescript = data;
            emit usrDescriptChanged();
            return true;
        }
    }
    return false;
}

bool UsrInfo::setPassWord(const QByteArray &oldPswd, const QByteArray &newPswd){
    if(passWordCheck(oldPswd)){
        if(newPswd != _pswd){
            _pswd = newPswd;
            emit pswdChanged();
            return true;
        }
    }
    return false;
}

void UsrInfo::save(iLoadSaveProcessor* processor){
    processor->writeValue( QString("usrName"), _name );
    processor->writeValue( QString("usrPassword"), _pswd );
    processor->writeValue( QString("usrLevel"), _level );
    processor->writeValue( QString("usrDescript"), _usrDescript);
}

void UsrInfo::load(iLoadSaveProcessor* processor){
    processor->readValue( QString("usrName"), _name );
    processor->readValue( QString("usrPassword"), _pswd );
    processor->readValue( QString("usrLevel"), _level );
    processor->readValue( QString("usrDescript"), _usrDescript);
}
