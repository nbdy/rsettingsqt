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

class RSettingsQT : public QObject {
    Q_OBJECT
    Q_PROPERTY(RedisQT* redis READ getRedis WRITE setRedis NOTIFY redisChanged)
    Q_PROPERTY(QString group READ getGroup WRITE setGroup NOTIFY groupChanged)

public:
    explicit RSettingsQT(QObject *parent = nullptr): QObject(parent){};

    ~RSettingsQT() {
        delete redis;
    };

    static void registerType(){
        qmlRegisterType<RSettingsQT>("io.eberlein.rsettingsqt", 1, 0, "RSettings");
    }

    Q_INVOKABLE QString makeKey(const QString& key) const {
        return group + key;
    }

    Q_INVOKABLE QJsonValue get(const QString& key) const {
        return redis->get(key);
    }

    Q_INVOKABLE void getAsync(const QString& key) const {
        redis->getAsync(key);
    }

    Q_INVOKABLE template<typename T> void set(const QString& key, T value){
        redis->setAsync(key, value);
    }

    Q_INVOKABLE RedisQT *getRedis() const {return redis;}
    Q_INVOKABLE void setRedis(RedisQT *redisQt) {
        delete this->redis;
        this->redis = redisQt;
        this->redis->connect();
        emit ready();
        emit redisChanged();
    }

    Q_INVOKABLE QString getGroup() const {return group;}
    Q_INVOKABLE void setGroup(QString& grp){
        if(!grp.endsWith("/")) grp += "/";
        redis->unsubscribe(this->group);
        this->group = makeKey(grp);
        redis->subscribe(this->group);
        emit groupChanged();
    }

signals:
    void redisChanged();
    void groupChanged();
    void ready();
    void getSuccessful(const QString& key, const QJsonValue& value);

    void onSettingChanged(const QString& key, const QString& value);

    void subscriberConnectedChanged(bool value);
    void clientConnectedChanged(bool value);

private:
    void onSubscriberReceived(const QString& channel, const QString& message){
        qDebug() << "Channel: " << channel << " | received: " << message;
    }

    QString group;
    RedisQT *redis{};
};

#endif //RSETTINGSQT_RSETTINGSQT_H
