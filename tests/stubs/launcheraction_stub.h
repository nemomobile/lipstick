#include "launcheraction.h"
#include <stubbase.h>

// 1. DECLARE STUB
class LauncherActionStub : public StubBase {
    public:
        virtual void LauncherActionConstructor();
        virtual void LauncherActionConstructor(const QString& desktopEntry);
};

// 2. IMPLEMENT STUB
void LauncherActionStub::LauncherActionConstructor() {
}

void LauncherActionStub::LauncherActionConstructor(const QString& desktopEntry) {
    Q_UNUSED(desktopEntry);
}

// 3. CREATE A STUB INTERFACE
LauncherActionStub gDefaultLauncherActionStub;
LauncherActionStub* gLauncherActionStub = &gDefaultLauncherActionStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
LauncherAction::LauncherAction() {
    gLauncherActionStub->LauncherActionConstructor();
}

LauncherAction::LauncherAction(const QString& desktopEntry) {
    gLauncherActionStub->LauncherActionConstructor(desktopEntry);
}

bool operator!=(const LauncherAction &a, const LauncherAction &b)
{
    Q_UNUSED(a);
    Q_UNUSED(b);
    return true;
}
