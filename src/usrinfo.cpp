#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "usrinfo.h"

UsrInfo::UsrInfo(QObject *parent, QCryptographicHash::Algorithm alg):
    QObject(parent),_alg(alg)
{
    _name = "undefined";
    _pswd = genCryptoString( _name, QString(""), _alg);
    _level = 1;
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

QString& UsrInfo::name(){
    return _name;
}

int UsrInfo::level() const{
    return _level;
}

bool UsrInfo::passWordCheck(const QString &testPswd){
    if(testPswd == _pswd ){
        return true;
    }
    return false;
}

bool UsrInfo::setName(const QString& newName, const QString &pwdWithoutCrypto){
    QString temp =  genCryptoString(_name, pwdWithoutCrypto, _alg);
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

bool UsrInfo::setLevel(int newLevel, const QString &cryptoPwd){
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

bool UsrInfo::setUsrDescript(const QString& data , const QString &cryptoPwd){
    if(passWordCheck(cryptoPwd)){
        if(data != _usrDescript){
            _usrDescript = data;
            emit usrDescriptChanged();
            return true;
        }
    }
    return false;
}

bool UsrInfo::setPassWord(const QString &oldCryptoPswd, const QString &newCryptoPswd){
    if(passWordCheck(oldCryptoPswd)){
        if(newCryptoPswd != _pswd){
            _pswd = newCryptoPswd;
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
