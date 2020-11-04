### RSettingsQT
Redis based Settings for QT
#### features
- have the same values over all RSettingsQT instances
#### [example](https://github.com/nbdy/carpi/blob/qml/libs/Settings/Music/MusicSettings.cpp)
#### install / build
```shell script
./build.sh
```
#### dependencies
- [QT](https://www.qt.io)
- [disqt](https://github.com/nbdy/disqt)
    - [cpp_redis](https://github.com/cpp-redis/cpp_redis)
        - [tacopie](https://github.com/cylix/tacopie)
#### usage
##### cpp
```c++
#include <rsettingsqt/rsettingsqt.h>

RSettingsQT::registerType(); // register qml type
```
##### qml
``` qml
import io.eberlein.rsettingsqt 1.0

RSettings {
    id: settings
    group: "audio"

    onReady: {
        console.log("we are connected and ready")
    }

    onGroupChanged: {
        console.log("changed the current group to", group)
    }

    onGetReturned: {
        console.log("get returned:", key, value)
    }

    onMessage: {
        console.log("we have a message from:", channel, ">", msg)
    }
}
```
