#ifndef QMLOCKS_STUB
#define QMLOCKS_STUB

#include <stubbase.h>
#include <qmlocks.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QmLocksStub : public StubBase {
public:
    virtual void QmLocksConstructor(QObject *parent = 0);
    virtual void QmLocksDestructor();
    virtual MeeGo::QmLocks::State getState(MeeGo::QmLocks::Lock what) const;
    virtual bool setState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
    virtual bool setDeviceAutolockTime(int seconds);
    virtual int getDeviceAutolockTime();
    virtual void connectNotify(const char *signal);
    virtual void disconnectNotify(const char *signal);
};

// 2. IMPLEMENT STUB
void QmLocksStub::QmLocksConstructor(QObject *parent) {
    Q_UNUSED(parent)
}

void QmLocksStub::QmLocksDestructor() {

}

MeeGo::QmLocks::State QmLocksStub::getState(MeeGo::QmLocks::Lock what) const {
    QList<ParameterBase*> params;
    params.append(new Parameter<MeeGo::QmLocks::Lock>(what));
    stubMethodEntered("getState", params);
    return stubReturnValue<MeeGo::QmLocks::State>("getState");
}

bool QmLocksStub::setState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how) {
    QList<ParameterBase*> params;
    params.append(new Parameter<MeeGo::QmLocks::Lock>(what));
    params.append(new Parameter<MeeGo::QmLocks::State>(how));
    stubMethodEntered("setState", params);
    return stubReturnValue<bool>("setState");
}

bool QmLocksStub::setDeviceAutolockTime(int seconds) {
    QList<ParameterBase*> params;
    params.append(new Parameter<int>(seconds));
    stubMethodEntered("setDeviceAutolockTime", params);
    return stubReturnValue<bool>("setDeviceAutolockTime");
}

int QmLocksStub::getDeviceAutolockTime() {
    stubMethodEntered("getDeviceAutolockTime");
    return stubReturnValue<int>("getDeviceAutolockTime");
}

void QmLocksStub::connectNotify(const char *signal)
{
  QList<ParameterBase*> params;
  params.append(new Parameter<const char *>(signal));
  stubMethodEntered("connectNotify",params);
}

void QmLocksStub::disconnectNotify(const char *signal)
{
  QList<ParameterBase*> params;
  params.append(new Parameter<const char *>(signal));
  stubMethodEntered("disconnectNotify",params);
}

// 3. CREATE A STUB INSTANCE
QmLocksStub gDefaultQmLocksStub;
QmLocksStub* gQmLocksStub = &gDefaultQmLocksStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
namespace MeeGo
{

QmLocks::QmLocks(QObject *parent) {
    gQmLocksStub->QmLocksConstructor(parent);
}

QmLocks::~QmLocks() {
    gQmLocksStub->QmLocksDestructor();
}

QmLocks::State QmLocks::getState(QmLocks::Lock what) const {
    return gQmLocksStub->getState(what);
}

bool QmLocks::setState(QmLocks::Lock what, QmLocks::State how) {
    return gQmLocksStub->setState(what, how);
}

bool QmLocks::setDeviceAutolockTime(int seconds) {
    return gQmLocksStub->setDeviceAutolockTime(seconds);
}

int QmLocks::getDeviceAutolockTime() {
    return gQmLocksStub->getDeviceAutolockTime();
}

void QmLocks::connectNotify(const char *signal)
{
    gQmLocksStub->connectNotify(signal);
}

void QmLocks::disconnectNotify(const char *signal)
{
    gQmLocksStub->disconnectNotify(signal);
}

}
#endif // QMLOCKS_STUB
