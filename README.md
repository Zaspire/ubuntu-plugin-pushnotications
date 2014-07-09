# Cordova Push Notifications Plugin for Ubuntu

### Cordova

The plugin can be installed via the Cordova command line interface:

1) Navigate to the root folder for your phonegap project. 2) Run the command.

```sh
cordova plugin add https://github.com/Zaspire/ubuntu-plugin-pushnotications.git
```

### Example
'''js
document.addEventListener('deviceready', function() {
    ubuntu.pushNotification.watchNotifications(function(){
        ubuntu.pushNotification.getNotifications(function(o){
            document.write(o)
        })
    })
}, false);
'''
