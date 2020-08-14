//
// Created by nbdy on 09.08.20.
//

#ifndef RSETTINGSQT_RSETTINGSQT_H
#define RSETTINGSQT_RSETTINGSQT_H

#include <thread>

#include <QDebug>
#include <QObject>
#include <QtQml>

#include <disqt/disqt.h>

/**
 * RSettings QT Class
 * Q_PROPERTY(QString group READ getGroup WRITE setGroup NOTIFY groupChanged)
 */
class RSettingsQT : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString group READ getGroup WRITE setGroup NOTIFY groupChanged)

public:
    /**
     * creates the RSettingsQT instance with a default RedisQT instance
     * @param parent QObject
     */
    explicit RSettingsQT(QObject *parent = nullptr): QObject(parent), redis(new RedisQT(parent)){
        _init();
    };

    /**
     * creates the RSettingsQT instance with a provided RedisQT instance
     * we connect to the RedisQT instance if it was not already connected
     * @param redisQt RedisQT
     * @param parent QObject
     */
    explicit RSettingsQT(RedisQT *redisQt, QObject *parent = nullptr): QObject(parent), redis(redisQt){
        _init();
    };

    /**
     * destructor; unsubscribes and disconnects; then deletes the pointer to the redis instance
     */
    ~RSettingsQT() {
        redis->unsubscribe(this->group);
        redis->disconnect();
        delete redis;
    };

    /**
     * register the qml type RSettings with uri 'io.eberlein.rsettingsqt' version 1.0
     */
    static void registerType(){
        qmlRegisterType<RSettingsQT>("io.eberlein.rsettingsqt", 1, 0, "RSettings");
    }

    /**
     * creates a key based on the current group
     * @param key QString
     * @return QString
     */
    Q_INVOKABLE QString makeKey(const QString& key) const {
        return group + key;
    }

    /**
     * gets a QJsonValue from the RedisQT instance based on the key
     * @param key QString
     * @return QJsonValue
     */
    Q_INVOKABLE QJsonValue get(const QString& key) const {
        return redis->get(makeKey(key));
    }

    /**
     * set value for a key
     * @tparam T Anything that fits into a QJsonValue
     * @param key QString
     * @param value T
     */
    Q_INVOKABLE template<typename T> void set(const QString& key, T value){
        redis->setAsync(key, value);
    }

    /**
     * check if a key exists
     * @param key QString
     * @return bool
     */
    Q_INVOKABLE bool exists(const QString& key) {
        return redis->exists(key);
    }

    /**
     * get the current Group
     * @return QString
     */
    Q_INVOKABLE QString getGroup() const {return group;}

    /**
     * set the current group
     * @param grp QString
     */
    Q_INVOKABLE void setGroup(QString& grp){
        if(!grp.endsWith("/")) grp += "/";
        redis->unsubscribe(this->group);
        this->group = makeKey(grp);
        redis->subscribe(this->group);
        emit groupChanged();
    }

signals:
    void groupChanged();
    void settingChanged(const QString& key, const QJsonValue& value);

private:
    void _init(){
        if(redis->getClientIsConnected()) redis->connect_client();
        if(redis->getSubscriberIsConnected()) redis->connect_subscriber();
    }

    QString group;
    RedisQT *redis;
};

#endif //RSETTINGSQT_RSETTINGSQT_H
