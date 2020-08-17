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
class RSettingsQT : public RedisQT {
    Q_OBJECT
    Q_PROPERTY(QString group READ getGroup WRITE setGroup NOTIFY groupChanged)

public:
    /**
     * creates the RSettingsQT instance
     * @param parent QObject
     */
    explicit RSettingsQT(QObject *parent = nullptr): RedisQT(parent){
        _init();
    };

    explicit RSettingsQT(const QString& group, QObject *parent = nullptr): RedisQT(parent), group(group){
        _init();
    }

    /**
     * destructor; unsubscribes and disconnects
     */
    ~RSettingsQT() {
        unsubscribe(group);
        disconnect();
    };

    /**
     * creates a key based on the current group
     * @param key QString
     * @return QString
     */
    Q_INVOKABLE QString makeKey(const QString& key) const {
        return group + key;
    }

    Q_INVOKABLE template<typename T> void set(const QString& key, T value) {
        QJsonDocument d = RedisQT::set(makeKey(key).toStdString(), mjo(key, value));
        RedisQT::publish(group, d.toJson());
    }

    Q_INVOKABLE QJsonValue get(const QString& key) const {
        return RedisQT::get(makeKey(key).toStdString());
    }

    /**
     * get the current Group
     * @return QString
     */
    Q_INVOKABLE QString getGroup() const {
        return group;
    }

    /**
     * set the current group
     * @param grp QString
     */
    Q_INVOKABLE void setGroup(QString& grp){
        if(!grp.endsWith("/")) grp += "/";
        unsubscribe(this->group);
        this->group = grp;
        subscribe(this->group);
        emit groupChanged();
    }

    /**
     * should be overridden
     * @return bool
     */
    Q_INVOKABLE virtual bool valuesSet(){
        return false;
    };

    /**
     * should be overridden
     */
    Q_INVOKABLE virtual void setDefaultValues(){

    };

signals:
    void groupChanged();

protected:
    virtual void pre_init(){};

    QString group;

private:
    void _init(){
        pre_init();
        connect();
        setGroup(group);
        if(getIsConnected() && !valuesSet()) setDefaultValues();
    }
};

#endif //RSETTINGSQT_RSETTINGSQT_H
