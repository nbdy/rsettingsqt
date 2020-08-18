//
// Created by nbdy on 09.08.20.
//

#ifndef RSETTINGSQT_RSETTINGSQT_H
#define RSETTINGSQT_RSETTINGSQT_H

#include <thread>

#include <QObject>
#include <QtQml>
#include <utility>

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
    explicit RSettingsQT(QObject *parent = nullptr): RedisQT(parent){};

    explicit RSettingsQT(QString group, QObject *parent = nullptr): RedisQT(parent), group(std::move(group)){}

    /**
     * destructor; unsubscribes and disconnects
     */
    ~RSettingsQT() override {
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
        RedisQT::publish(group, d.toJson(QJsonDocument::Compact));
    }

    Q_INVOKABLE QJsonValue get(const QString& key) const {
        return RedisQT::get(makeKey(key).toStdString())[key];
    }

    Q_INVOKABLE bool exists(const QString& key) const {
        return RedisQT::exists(makeKey(key));
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
     * checks if the database keys exist and have a value
     * should be overridden
     * @return bool
     */
    virtual bool valuesSet() = 0;

    /**
     * sets the default database keys and values
     * should be overridden
     */
    virtual void setDefaultValues() = 0;

    /**
     * sets the class variables by getting the data from the database
     * should be overridden
     */
    virtual void setValues() = 0;

public slots:
    /**
     * executes the above functions
     */
    void readyReceived(){
        if(!valuesSet()) setDefaultValues();
        else setValues();
    };

signals:
    void groupChanged();

protected:
    void init(){
        connect();
        setGroup(group);
    }

    QString group;
};

#endif //RSETTINGSQT_RSETTINGSQT_H
