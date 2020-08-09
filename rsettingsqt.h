//
// Created by nbdy on 09.08.20.
//

#ifndef RSETTINGSQT_RSETTINGSQT_H
#define RSETTINGSQT_RSETTINGSQT_H

#include <thread>

#include <QDebug>
#include <QObject>

#include <disqt/disqt.h>

enum DataTypeEnum {
    String,
    Double,
    Integer,
    Bool,
    Array
};

class RSettingsQT : public QObject {
    Q_OBJECT
    Q_PROPERTY(RedisQT redis READ getRedis WRITE setRedis NOTIFY redisChanged)
    Q_PROPERTY(QString group READ getGroup WRITE setGroup NOTIFY groupChanged)

public:
    explicit RSettingsQT(QObject *parent = nullptr){};

    ~RSettingsQT() override {
        delete redis;
    };

    Q_INVOKABLE QString makeKey(const QString& key) const {
        return group + key;
    }

    Q_INVOKABLE void setAsync(const QString& key, const QString& value, int dataType){
        std::thread t([&](){set(makeKey(key), value, dataType);});
        t.detach();
    };

    Q_INVOKABLE void set(const QString& key, const QString& value, int dataType){
        QJsonObject t;
        t.insert(key, value);
        t.insert("dT", dataType);
        redis->set(makeKey(key), QJsonDocument(t).toJson(QJsonDocument::Compact));
        emit setSuccessful(key);
    };

    Q_INVOKABLE QJsonValue get(const QString& key){
        QString data = redis->get(makeKey(key));
        QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
        return d[key];
        /*
        switch (d["dT"].toInt()) {
            case DataTypeEnum::String:
                return v.toString();
            case DataTypeEnum::Double:
                return v.toDouble();
            case DataTypeEnum::Integer:
                return v.toInt();
            case DataTypeEnum::Bool:
                return v.toBool();
            case DataTypeEnum::Array:
                return v.toArray();
            default:
                return v.toString();
        }
        */
    }

    Q_INVOKABLE RedisQT *getRedis() const {return redis;}
    Q_INVOKABLE void setRedis(RedisQT *redis) {
        delete redis;
        this->redis = redis;
        connect(this->redis, &RedisQT::message, this, &RSettingsQT::onSubscriberReceived);
        emit redisChanged();
    }

    Q_INVOKABLE QString getGroup() const {return group;}
    Q_INVOKABLE void setGroup(QString& group){
        if(!group.endsWith("/")) group += "/";
        redis->unsubscribe(this->group);
        this->group = makeKey(group);
        redis->subscribe(this->group);
        emit groupChanged();
    }

signals:
    void setSuccessful(const QString& key);
    void getSuccessful(const QString& key, const QString& value);

    void redisChanged();
    void groupChanged();

    void onSettingChanged(const QString& key, const QString& value);

private:
    void onSubscriberReceived(const QString& channel, const QString& message){
        qDebug() << "Channel: " << channel << " | received: " << message;
    }

    QString group;
    RedisQT *redis{};
};

#endif //RSETTINGSQT_RSETTINGSQT_H
