#include "push_notification.h"

#include <QtDBus>
#include <QApplication>

#include <sstream>
#include <iomanip>
#include <string>

#define PUSH_SERVICE "com.ubuntu.PushNotifications"
#define PUSH_PATH "/com/ubuntu/PushNotifications"
#define PUSH_IFACE "com.ubuntu.PushNotifications"

#define POSTAL_SERVICE "com.ubuntu.Postal"
#define POSTAL_PATH "/com/ubuntu/Postal"
#define POSTAL_IFACE "com.ubuntu.Postal"

inline const QString appId() {
    return QCoreApplication::applicationName();
}

static QString EscapedAppId() {
    const std::string id = appId().toStdString();
    std::stringstream ss;
    for (char c: id) {
        if (isalnum(c)) {
            ss << c;
            continue;
        }
        ss << '_' <<std::hex << std::setfill('0') << std::setw(2) << int(c);
    }

    return QString::fromStdString(ss.str());
}

PushNotification::PushNotification(Cordova *cordova): CPlugin(cordova) {
    QDBusConnection bus = QDBusConnection::sessionBus();

    //FIXME: make async
    QDBusMessage message = QDBusMessage::createMethodCall(PUSH_SERVICE, QString(PUSH_PATH) + "/" + EscapedAppId(), PUSH_IFACE, "Register");
    message << appId() + "_" + QCoreApplication::applicationVersion();

    QDBusMessage reply = bus.call(message);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qCritical() << "Error registering: " << reply.errorMessage();
    } else {
        QString s = reply.arguments()[0].toString();
        qCritical() << s;
        _token = s;
    }

    bool b = bus.connect(POSTAL_SERVICE, QString(POSTAL_PATH) + "/" + EscapedAppId(), POSTAL_IFACE, "Post", "s", this, SLOT(notified(QString)));

    if (!b)
        qCritical() << "Error connecting";
}

void PushNotification::notified(QString) {
    callbackWithoutRemove(_scId, "");
}

void PushNotification::startWatch(int scId, int) {
    _scId = scId;
}

void PushNotification::getToken(int scId, int) {
    cb(scId, _token);
}

void PushNotification::getNotifications(int scId, int) {
    QDBusConnection bus = QDBusConnection::sessionBus();

    QDBusMessage message = QDBusMessage::createMethodCall(POSTAL_SERVICE, QString(POSTAL_PATH) + "/" + EscapedAppId(), POSTAL_IFACE, "Messages");
    message << appId() + "_" + QCoreApplication::applicationVersion();

    QDBusMessage reply = bus.call(message);
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qCritical() << "Error fetching notifications:" << reply.errorMessage();
        return;
    }

    QStringList list = reply.arguments()[0].toStringList();

    QString res;
    for (const QString s: list) {
        if (res.size())
            res += ", ";
        res += CordovaInternal::format(s);
    }

    callback(scId, QString("[ %1 ]").arg(res));
}
