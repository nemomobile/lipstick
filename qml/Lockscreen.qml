import QtQuick 1.1

Item {
    property alias clockRunning: clock.running

    Connections {
        target: LipstickSettings

        // TODO: turn clock off when screen turns off to save power
        onLockscreenVisibleChanged: {
            if (!LipstickSettings.lockscreenVisible) {
                if (dashboard.currentPage == 0) {
                    // lockscreen visible, but we don't want to see it.
                    // reset view to switcher/launcher
                    dashboard.currentPage = 1
                    dashboard.snapToCurrentPage();
                    stopClock()
                }
                return
            } else {
                // lockscreen enabled. make sure we're already there.
                startClock()
                dashboard.currentPage = 0
                dashboard.snapToCurrentPage();
            }
        }
    }

    Clock {
        id: clock
        y: LipstickSettings.lockscreenVisible ? (parent.height / 2 - height / 2) : parent.height - height

        Behavior on y {
            SpringAnimation {
                spring: 3
                damping: 0.2
                mass: 0.3
            }
        }
    }

    function startClock() {
        console.log("Starting clock")
        clock.start()
    }

    function stopClock() {
        console.log("Stopping clock")
        clock.stop()
    }
}

