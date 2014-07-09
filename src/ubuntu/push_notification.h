/*
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <QtCore>

#include <cplugin.h>
#include <cordova.h>

class PushNotification: public CPlugin {
    Q_OBJECT
public:
    explicit PushNotification(Cordova *cordova);

    virtual const QString fullName() override {
        return PushNotification::fullID();
    }

    virtual const QString shortName() override {
        return PushNotification::fullID();
    }

    static const QString fullID() {
        return "UbuntuPushNotification";
    }
public slots:
    void notified(QString);
    void getNotifications(int scId, int ecId);
    void getToken(int scId, int ecId);
    void startWatch(int scId, int ecId);
private:
    int _scId;
    QString _token;
};
