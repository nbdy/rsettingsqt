### RSettingsQT
Redis based Settings for QT
#### features
- have the same values over all RSettingsQT instance
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
import io.eberlein.rsettingsqt 1.0

RSettings {
    id: settings
    group: "audio" // subscribes to this 'key'

    onGroupChanged: {
        console.log("changed the current group to", group)
    }

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