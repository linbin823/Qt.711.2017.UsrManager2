#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QCryptographicHash>
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
UsrInfo::UsrInfo(QObject *parent, const QString salt): QObject(parent), _salt(salt)
{
    QByteArray initPswd = _salt.toLocal8Bit();
    _pswd = QCryptographicHash::hash(initPswd, QCryptographicHash::Sha256);
    _name = "undefined";
    _level = 0;
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
 * 查询名称
 * 输入参数：无
 * 返回数值：用户名称
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
bool UsrInfo::passWordCheck(const QString& testPswd){
    QString temp = testPswd+_salt;
    QByteArray res = QCryptographicHash::hash(temp.toLatin1(), QCryptographicHash::Sha256);
    if(res == _pswd ){
        return true;
    }
    return false;
}

/*
 * 修改用户名
 * 输入参数：
 * 1、newName 新用户名
 * 2、testPswd 测试密码
 * 返回数值：
 * 1、成功返回ture，否则返回false
 * 功能描述：
 */
bool UsrInfo::setName(const QString& newName, const QString& pwd){
    if(passWordCheck(pwd)){
        if(_name != newName ){
            _name = newName;
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
 * 功能描述：
 */
bool UsrInfo::setLevel(int newLevel, const QString& pwd){
    if(passWordCheck(pwd)){
        if(_level!=newLevel){
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
bool UsrInfo::setPassWord(const QString& oldPswd, const QString& newPswd){
    if(passWordCheck(oldPswd)){
        QString temp = newPswd + _salt;
        QByteArray temp1 = QCryptographicHash::hash(temp.toLatin1(), QCryptographicHash::Sha256);
        if(temp1 != _pswd){
            _pswd = temp1;
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
