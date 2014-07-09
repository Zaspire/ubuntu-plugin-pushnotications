var exec = require('cordova/exec');

exports.getNotifications = function(callback) {
    exec(callback, null, 'UbuntuPushNotification', 'getNotifications', []);
}

exports.getToken = function(callback) {
    exec(callback, null, 'UbuntuPushNotification', 'getToken', []);
}

exports.watchNotifications = function(callback) {
    exec(callback, null, 'UbuntuPushNotification', 'startWatch', []);
}
