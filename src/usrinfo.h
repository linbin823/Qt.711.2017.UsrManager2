#ifndef USRINFO_H
#define USRINFO_H

#include <QObject>
#include <QCryptographicHash>
#include "iloadsave.h"
#include "iloadsaveprocessor.h"
/*!
 * UsrInfo
 * 用来记录在用户信息。
 * 保存大量的用户相关信息在QVarintMap中
 * 可以在QML、QWebChannel中使用
 */
class UsrInfo :  public QObject,  public iLoadSave
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap  properties  READ properties     NOTIFY propertiesChanged)

public:
    explicit UsrInfo(QObject *parent = 0, QCryptographicHash::Algorithm alg = QCryptographicHash::Md5 );
    ~UsrInfo();

    QVariant& operator [](const QString& key){
        return _properties[key];
    }

public slots:

    /*!
     * 查询名称
     */
    QString name(void);

    /*!
     * 静态类函数
     * 方便管理UI使用，使用Algorithm( Algorithm(pswd)+name )算法，从明文密码生成加密密码。
     */
    static QString genCryptoString(const QString &name, const QString &pwdWithoutCrypto ,
                                      QCryptographicHash::Algorithm alg = QCryptographicHash::Md5);

    /*!
     * 修改用户名，成功返回ture，否则返回false
     * 修改用户名时，必须传入明文密码。需要它来生成新的密码。
     */
    bool setName(const QString& newName, const QString& pwdWithoutCrypto);

    /*!
     * 查询等级。等级必须大于等于1，小于等于200。
     */
    int level(void) const;

    /*!
     * 修改等级，成功返回ture，否则返回false
     * 等级必须大于等于1，小于等于200。
     */
    bool setLevel(int newLevel, const QString &pwd);

    /*!
     * 登录唯一性
     */
    bool exclusive(void)const;

    /*!
     * 设置登录唯一性
     */
    bool setExclusive(bool exclusive, const QString& pwd);

    /*!
     * 检查密码，正确返回ture，否则返回false
     */
    bool passWordCheck(const QString& testPswd);

    /*!
     * 修改密码，成功返回ture，否则返回false
     */
    bool setPassWord(const QString& oldCryptoPswd, const QString& newCryptoPswd);

    /*!
     * UsrInfo的所有内容。
     */
    inline QVariantMap& properties(void){
        return _properties;
    }

    /*!
     * 获取一个属性的值
     * 找不到新建记录
     */
    QVariant& property(const QString& key);

    /*!
     * 删除一个属性
     * UsrInfo的所有内容,初始的时候必有属性"name","password","level","exclusive"。
     * "name","password","level","exclusive"是保留的关键字。不允许删除。
     * 正确返回ture，否则返回false。
     */
    bool unsetProperty(const QString& key, const QString &pwd);

    /*!
     * 新增/更改一个属性
     * "name","password"需要特殊修改方法。使用专用API修改。
     * 正确返回ture，否则返回false。
     */
    bool setProperty(const QString& key, const QVariant& value, const QString &pwd);

    /*!
     * 保存 实现iLoadSave
     */
    void save(iLoadSaveProcessor* processor);

    /*!
     * 读取 实现iLoadSave
     */
    void load(iLoadSaveProcessor* processor);

private:
    QCryptographicHash::Algorithm _alg;

    QVariantMap  _properties;
signals:
    void propertiesChanged();
};

#endif // USRINFO_H
