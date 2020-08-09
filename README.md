### RSettingsQT
Redis based Settings for QT
#### features
- notify when setting changed
- async
#### install / build
```shell script
./build.sh
```
#### dependencies
- [QT](https://www.qt.io)
- [disqt](https://github.com/smthnspcl/disqt)
    - [cpp_redis](https://github.com/cpp-redis/cpp_redis)
        - [tacopie](https://github.com/cylix/tacopie)
#### usage
##### cpp
```c++
#include <rsettingsqt/rsettingsqt.h>

RSettingsQT::registerType(); // register qml type
```
##### qml
```
import io.eberlein.disqt 1.0
import io.eberlein.rsettingsqt 1.0

Redis {
    id: redis

    onClientConnected: {
        settings.set("volume", 42)
        var v = settings.get("volume")
        settings.getAsync("volume")
    }
}

RSettings {
    id: settings
    redis: redis
    group: "audio" // subscribes to this 'key'

    onSettingChanged: {
        console.log(key, value)
    }

    onSetSuccessful: {
        console.log("set value for", key)  // will log that value for volume was set
    }

    onGetSuccessful: {
        console.log("got value:", value, "for key:", key)  // signal emitted from getAsync, will log key volume and its value
    }
}
```