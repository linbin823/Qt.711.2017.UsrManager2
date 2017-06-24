#ifndef USRINFO_H
#define USRINFO_H

#include <QObject>
#include <QCryptographicHash>
#include "iloadsave.h"
#include "iloadsaveprocessor.h"

//2017.05重写用户权限管理
//可以在QML、QWebChannel中使用
class UsrInfo :  public QObject,  public iLoadSave
{
    Q_OBJECT

    Q_PROPERTY(QString name         READ name           NOTIFY nameChanged)
    Q_PROPERTY(int level            READ level          NOTIFY levelChanged)
    Q_PROPERTY(QString usrDescript  READ usrDescript    NOTIFY usrDescriptChanged)

public:
    explicit UsrInfo(QObject *parent = 0, QCryptographicHash::Algorithm alg = QCryptographicHash::Md5 );
    ~UsrInfo();

public slots:

    /*!
     * 查询名称
     */
    QString &name(void);

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
     * 查询用户数据
     */
    QString& usrDescript(void);

    /*!
     * 修改用户描述，成功返回ture，否则返回false
     */
    bool setUsrDescript(const QString& data, const QString& pwd);

    /*!
     * 检查密码，正确返回ture，否则返回false
     */
    bool passWordCheck(const QString& testPswd);

    /*!
     * 修改密码，成功返回ture，否则返回false
     */
    bool setPassWord(const QString& oldCryptoPswd, const QString& newCryptoPswd);

    /*!
     * 保存 实现iLoadSave
     */
    void save(iLoadSaveProcessor* processor);

    /*!
     * 读取 实现iLoadSave
     */
    void load(iLoadSaveProcessor* processor);

private:
    QString  _pswd;
    int         _level;
    QString     _name;
    QString     _usrDescript;
    QCryptographicHash::Algorithm _alg;

signals:
    void nameChanged();
    void levelChanged();
    void pswdChanged();
    void usrDescriptChanged();
};

#endif // USRINFO_H
