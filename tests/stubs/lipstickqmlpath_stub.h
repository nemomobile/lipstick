#ifndef LIPSTICKQMLPATH_STUB
#define LIPSTICKQMLPATH_STUB

#include "lipstickqmlpath.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QmlPathStub : public StubBase {
  public:
  virtual void append(const QString &);
  virtual void prepend(const QString &);
  virtual QUrl to(const QString &);
};

// 2. IMPLEMENT STUB
void QmlPathStub::append(const QString &path) {
  QList<ParameterBase*> params;
  params.append(new Parameter<QString>(path));
  stubMethodEntered("append", params);
}

void QmlPathStub::prepend(const QString &path) {
  QList<ParameterBase*> params;
  params.append(new Parameter<QString>(path));
  stubMethodEntered("prepend", params);
}

QUrl QmlPathStub::to(const QString &filename) {
  QList<ParameterBase*> params;
  params.append(new Parameter<QString>(filename));
  stubMethodEntered("to", params);
  return QUrl("qrc:/qml/" + filename);
}


// 3. CREATE A STUB INSTANCE
QmlPathStub gDefaultQmlPathStub;
QmlPathStub *gQmlPathStub = &gDefaultQmlPathStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
void QmlPath::append(const QString &path) {
    gQmlPathStub->append(path);
}

void QmlPath::prepend(const QString &path) {
    gQmlPathStub->prepend(path);
}

QUrl QmlPath::to(const QString &filename) {
    return gQmlPathStub->to(filename);
}

#endif
