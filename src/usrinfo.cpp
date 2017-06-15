#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "usrinfo.h"
/*
 * 构造函数
 * 输入参数：无
 * 返回数值：无
 * 功能描述：
 * 1、初始化密码为空
 * 2、设置密码
 * 3、初始化名称
 * 4、初始化等级
 */
UsrInfo::UsrInfo(QObject *parent, QCryptographicHash::Algorithm alg):
    QObject(parent),_alg(alg)
{
    _name = "undefined";
    _pswd = QCryptographicHash::hash( QString("").toUtf8() , _alg);
    _pswd = QCryptographicHash::hash( (_pswd + _name.toUtf8() ), _alg);
    _level = 1;
}

/*
 * 析构函数
 * 输入参数：无
 * 返回数值：无
 * 功能描述：
 */
UsrInfo::~UsrInfo(){

}

/*
 * 辅助函数
 * 输入参数：
 * 1、const QString& name
 * 2、const QString& pswd
 * 返回数值：
 * QString
 * 功能描述：
 * 界面用,用于生成加密的密码字符串
 * 加密方式：Algorithm( Algorithm(pswd)+name )
 */
QByteArray UsrInfo::genCryptoString(const QString &pswd){
    QByteArray ret = QCryptographicHash::hash( pswd.toUtf8(), _alg);
    ret = QCryptographicHash::hash( (ret + _name.toUtf8() ), _alg);
    return ret;
}

/*
 * 查询名称
 * 输入参数：无
 * 返回数值：用户名称SSS
 * 功能描述：
 */
QString UsrInfo::name() const{
    return _name;
}

/*
 * 查询等级
 * 输入参数：无
 * 返回数值：用户等级
 * 功能描述：
 */
int UsrInfo::level() const{
    return _level;
}

/*
 * 检查密码
 * 输入参数：
 * 1、testPswd 测试密码
 * 返回数值：
 * 1、正确返回ture，否则返回false
 * 功能描述：
 */
bool UsrInfo::passWordCheck(const QByteArray& testPswd){
    if(testPswd == _pswd ){
        return true;
    }
    return false;
}

/*
 * 修改用户名
 * 输入参数：
 * 1、newName 新用户名
 * pwdWithoutCrypto 密码
 * 返回数值：
 * 1、成功返回ture，否则返回false
 * 2、修改用户名后，密码也要更新
 * 功能描述：
 */
bool UsrInfo::setName(const QString& newName, const QString &pwdWithoutCrypto){
    QByteArray temp =   QCryptographicHash::hash( pwdWithoutCrypto.toUtf8() , _alg);
    temp = QCryptographicHash::hash( (temp + _name.toUtf8() ), _alg);
    if(passWordCheck(temp)){
        if(_name != newName ){
            _name = newName;
            _pswd = QCryptographicHash::hash( pwdWithoutCrypto.toUtf8() , _alg);
            _pswd = QCryptographicHash::hash( (_pswd + _name.toUtf8() ), _alg);
            emit nameChanged();
            return true;
        }
    }
    return false;
}

/*
 * 修改等级
 * 输入参数：
 * 1、newLevel 新用户等级
 * 2、testPswd 测试密码
 * 返回数值：
 * 1、成功返回ture，否则返回false
 * 2、Level必须大于等于1，小于等于200
 * 功能描述：
 */
bool UsrInfo::setLevel(int newLevel, const QByteArray &pwd){
    if(passWordCheck(pwd)){
        if(_level!=newLevel && newLevel >=1 && newLevel <=200 ){
            _level = newLevel;
            emit levelChanged();
            return true;
        }
    }
    return false;
}

/*
 * 查询用户数据
 * 输入参数：
 * 返回数值：
 * 1、用户数据
 * 功能描述：
 */
QString UsrInfo::usrDescript(void)const{
    return _usrDescript;
}

/*
 * 修改用户数据
 * 输入参数：
 * 1、const QString& data
 * 返回数值：
 * 1、成功返回ture，否则返回false
 * 功能描述：
 */
bool UsrInfo::setUsrDescript(const QString& data){
    if(data != _usrDescript){
        _usrDescript = data;
        emit usrDescriptChanged();
        return true;
    }
    return false;
}

/*
 * 修改密码
 * 输入参数：
 * 1、newPswd 新用户密码
 * 2、oldPswd 旧密码
 * 返回数值：
 * 1、成功返回ture，否则返回false
 * 功能描述：
 * 1、检查密码，正确就换新密码
 */
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

/*
 * 保存参数
 * 输入参数：
 * 1、newPswd 新用户密码
 * 2、oldPswd 旧密码
 * 返回数值：
 * 1、成功返回ture，否则返回false
 * 功能描述：
 * 1、检查密码，正确就换新密码
 */
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
