#ifndef USRINFO_H
#define USRINFO_H

#include <QObject>
#include "iloadsave.h"
#include "iloadsaveprocessor.h"

//2017.05重写用户权限管理
//可以在QML、QWebChannel中使用
class UsrInfo :  public QObject,  public iLoadSave
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(QString usrDescript READ usrDescript WRITE setUsrDescript NOTIFY usrDescriptChanged)

public:
    explicit UsrInfo(QObject *parent = 0, const QString salt = QString("8y*P%") );
    ~UsrInfo();

    virtual void save(iLoadSaveProcessor* processor);
    virtual void load(iLoadSaveProcessor* processor);


public slots:
    //查询名称
    QString name(void) const;
    //修改用户名，成功返回ture，否则返回false
    bool setName(const QString& newName, const QString& pwd);
    //查询等级
    int level(void) const;
    //修改等级，成功返回ture，否则返回false
    bool setLevel(int newLevel, const QString& pwd);
    //查询用户数据
    QString usrDescript(void)const;
    //修改用户数据
    bool setUsrDescript(const QString& data);

    //检查密码，正确返回ture，否则返回false
    bool passWordCheck(const QString& testPswd);
    //修改密码，成功返回ture，否则返回false
    bool setPassWord(const QString& oldPswd, const QString& newPswd);

private:
    QString     _salt;
    QByteArray  _pswd;
    int         _level;
    QString     _name;
    QString     _usrDescript;

signals:
    void nameChanged();
    void levelChanged();
    void pswdChanged();
    void usrDescriptChanged();
};

#endif // USRINFO_H
