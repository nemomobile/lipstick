#ifndef NOTIFICATIONPREVIEWPRESENTER_STUB
#define NOTIFICATIONPREVIEWPRESENTER_STUB

#include "notificationpreviewpresenter.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationPreviewPresenterStub : public StubBase {
  public:
  virtual void NotificationPreviewPresenterConstructor(QObject *parent);
  virtual void NotificationPreviewPresenterDestructor();
  virtual LipstickNotification * notification() const;
  virtual void showNextNotification();
  virtual void updateNotification(uint id);
  virtual void removeNotification(uint id, bool onlyFromQueue);
}; 

// 2. IMPLEMENT STUB
void NotificationPreviewPresenterStub::NotificationPreviewPresenterConstructor(QObject *parent) {
  Q_UNUSED(parent);

}
void NotificationPreviewPresenterStub::NotificationPreviewPresenterDestructor() {

}
LipstickNotification * NotificationPreviewPresenterStub::notification() const {
  stubMethodEntered("notification");
  return stubReturnValue<LipstickNotification *>("notification");
}

void NotificationPreviewPresenterStub::showNextNotification() {
  stubMethodEntered("showNextNotification");
}

void NotificationPreviewPresenterStub::updateNotification(uint id) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  stubMethodEntered("updateNotification",params);
}

void NotificationPreviewPresenterStub::removeNotification(uint id, bool onlyFromQueue) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(id));
  params.append( new Parameter<bool >(onlyFromQueue));
  stubMethodEntered("removeNotification",params);
}



// 3. CREATE A STUB INSTANCE
NotificationPreviewPresenterStub gDefaultNotificationPreviewPresenterStub;
NotificationPreviewPresenterStub* gNotificationPreviewPresenterStub = &gDefaultNotificationPreviewPresenterStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationPreviewPresenter::NotificationPreviewPresenter(QObject *parent) {
  gNotificationPreviewPresenterStub->NotificationPreviewPresenterConstructor(parent);
}

NotificationPreviewPresenter::~NotificationPreviewPresenter() {
  gNotificationPreviewPresenterStub->NotificationPreviewPresenterDestructor();
}

LipstickNotification * NotificationPreviewPresenter::notification() const {
  return gNotificationPreviewPresenterStub->notification();
}

void NotificationPreviewPresenter::showNextNotification() {
  gNotificationPreviewPresenterStub->showNextNotification();
}

void NotificationPreviewPresenter::updateNotification(uint id) {
  gNotificationPreviewPresenterStub->updateNotification(id);
}

void NotificationPreviewPresenter::removeNotification(uint id, bool onlyFromQueue) {
  gNotificationPreviewPresenterStub->removeNotification(id, onlyFromQueue);
}


#endif
