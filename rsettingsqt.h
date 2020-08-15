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
        connect();
        emit ready();
    };

    /**
     * destructor; unsubscribes and disconnects
     */
    ~RSettingsQT() {
        unsubscribe(group);
        disconnect();
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

    Q_INVOKABLE template<typename T> void gset(const QString& key, T value) {
        set<T>(makeKey(key).toStdString(), mjo(key, value));
    }

    Q_INVOKABLE QJsonValue gget(const QString& key) const {
        return const_cast<RSettingsQT*>(this)->get(key.toStdString());
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
        group = makeKey(grp);
        subscribe(this->group);
        emit groupChanged();
    }

signals:
    void groupChanged();
    void getReturned(const QString& key, const QJsonValue& value);
    void message(const QString& key, const QString& value);
    void ready();

private:
    QString group;
};

#endif //RSETTINGSQT_RSETTINGSQT_H
