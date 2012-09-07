import QtQuick 1.1

Text {
    id: time

    height: paintedHeight + time.anchors.topMargin
    text: Qt.formatDateTime(new Date(), "hh:mm");
    font.pixelSize: 60
    font.family: headerFont.name
    color: "#ffffff"
    horizontalAlignment: Text.AlignHCenter
    property alias running: timer.running

    function start() {
        if (!timer.running)
            timer.update();
    }

    function stop() {
        if (timer.running)
            timer.stop();
    }

    Timer {
        id: timer
        running: false
        repeat: true

        function update() {
            var date = new Date();
            var millis = 60000 - date.getSeconds() * 1000;
            timer.interval = millis;
            if (!timer.running)
                timer.start();
            time.text = Qt.formatDateTime(new Date(), "hh:mm");
        }

        onTriggered: {
            if (timer.interval < 60000)
                timer.interval = 60000;
            time.text = Qt.formatDateTime(new Date(), "hh:mm");
        }
    }
}


