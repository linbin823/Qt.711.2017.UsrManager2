#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include <QObject>
#include "usrinfo.h"
#include "iloadsave.h"

QT_BEGIN_NAMESPACE
class UsrManager;
QT_END_NAMESPACE

/*!
 *  SessionInfo
 *  用来记录在线会话信息。
 *  不同的客户端登录后，都会生成一个SessionInfo，记录了所有的登录信息。
 */
class SessionInfo :  public QObject , public iLoadSave
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap  properties     READ properties     NOTIFY propertiesChanged)

public:
    explicit SessionInfo(UsrInfo* usrInfo , QObject *parent = 0);
    ~SessionInfo();

    QVariant& operator [](const QString& key){
        return _properties[key];
    }

public slots:

    /*!
     * 本SessionInfo是否有效。
     */
    inline bool isActive(void)const{
        return (_properties["expireTime"] >= QDateTime::currentDateTime());
    }

    /*!
     * 会话信息的所有内容
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
     * SessionInfo初始的时候必有属性"identifier","expireTime"。
     * 不允许删除"identifier","expireTime"
     */
    void unsetProperty(const QString& key);

    /*!
     * 新增/更改一个属性
     * SessionInfo初始的时候必有属性"identifier","expireTime"。
     */
    void setProperty(const QString& key, const QVariant& value);

    /*!
     * 获取失效时间。当系统时间大于失效时间后，登录失效。
     */
    QDateTime expireTime(void)const;

    /*!
     * 设置失效时间。
     */
    void setExpireTime(const QDateTime& time);

    /*!
     * 获取标识字符串
     */
    QString identifier(void)const;

    /*!
     * 设置标识字符串
     */
    bool setIdentifier(const QString& id);
    /*!
     * 保存 实现iLoadSave
     */
    void save(iLoadSaveProcessor* processor);

    /*!
     * 读取 实现iLoadSave
     */
    void load(iLoadSaveProcessor* processor);

    /*!
     * 获取UsrInfo
     */
    inline UsrInfo& usrInfo(void){
        return *_usrInfo;
    }

private:
    QVariantMap _properties;
    UsrInfo*    _usrInfo;

signals:
    void propertiesChanged();
};

#endif // SESSIONINFO_H
