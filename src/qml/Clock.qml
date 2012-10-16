import QtQuick 1.1

Text {
    id: time
    property alias running: timer.running

    color: "white"
    text: Qt.formatDateTime(new Date(), "hh:mm");
    font { pixelSize: 120; family: headerFont.name }
    anchors.horizontalCenter: parent.horizontalCenter

    function start() {
        if (!timer.running)
            timer.update();
    }

    function stop() {
        timer.stop();
    }

    Timer {
        id: timer
        repeat: true

        function update() {
            var date = new Date();
            var millis = 60000 - date.getSeconds() * 1000;
            timer.interval = millis;
            if (!running)
                start();
            time.text = Qt.formatDateTime(new Date(), "hh:mm");
        }

        onTriggered: {
            timer.interval = Math.max(60000, timer.interval)
            time.text = Qt.formatDateTime(new Date(), "hh:mm");
        }
    }
}


