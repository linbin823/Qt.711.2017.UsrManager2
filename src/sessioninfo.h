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
    Q_PROPERTY(QDateTime    expireTime     READ expireTime     WRITE  setExpireTime      NOTIFY expireTimeChanged)

    friend class UsrManager;

public:
    explicit SessionInfo(UsrInfo* usrInfo , QObject *parent = 0);
    ~SessionInfo();

    QVariant& operator [](const QString& key){
        if( !_properties.contains(key)){
            _properties.insert(key, QVariant());
        }
        return _properties[key];
    }

public slots:

    /*!
     *  本SessionInfo是否有效。
     */
    inline bool isActive(void)const{
        return (_expireTime >= QDateTime::currentDateTime());
    }

    /*!
     *  会话信息的所有内容
     */
    inline QVariantMap& properties(void){
        return _properties;
    }

    /*!
     * 获取一个属性的值
     * SessionInfo初始的时候必有一个属性"identifier"。
     * 同时UsrInfo的主要属性也可以使用该函数查询。
     * "usrName","usrLevel","usrDescription"是保留的关键字。
     * 找不到返回QVariant()
     */
    QVariant& property(const QString& key);

    /*!
     *  删除一个属性
     */
    void unsetProperty(const QString& key);

    /*!
     *  新增/更改一个属性
     */
    void setProperty(const QString& key, const QVariant& value);

    /*!
     *  获取失效时间。当系统时间大于失效时间后，登录失效。
     */
    QDateTime& expireTime(void);

    /*!
     *  设置失效时间。
     */
    void setExpireTime(const QDateTime& time);

    /*!
     *  保存 实现iLoadSave
     */
    void save(iLoadSaveProcessor* processor);

    /*!
     *  读取 实现iLoadSave
     */
    void load(iLoadSaveProcessor* processor);

private:
    QVariantMap _properties;
    UsrInfo*    _usrInfo;
    QDateTime   _expireTime;

private slots:
    //process the usrInfo properties changed event
    void propertyChanged();

signals:
    void propertiesChanged();
    void expireTimeChanged();
};

#endif // SESSIONINFO_H
