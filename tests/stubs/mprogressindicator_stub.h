#ifndef MPROGRESSINDICATOR_STUB
#define MPROGRESSINDICATOR_STUB

#include "mprogressindicator.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MProgressIndicatorStub : public StubBase {
  public:
   const MTheme::ViewType barType ;
  virtual void MProgressIndicatorDestructor();
  virtual int maximum() const;
  virtual int minimum() const;
  virtual int value() const;
  virtual bool unknownDuration() const;
  virtual void setMaximum(int maximum);
  virtual void setMinimum(int minimum);
  virtual void setRange(int minimum, int maximum);
  virtual void setValue(int value);
  virtual void setUnknownDuration(bool unknownDuration);
  virtual void MProgressIndicatorConstructor(MProgressIndicatorPrivate *dd, MProgressIndicatorModel *model, QGraphicsItem *parent);
}; 

// 2. IMPLEMENT STUB
void MProgressIndicatorStub::MProgressIndicatorDestructor() {

}
int MProgressIndicatorStub::maximum() const {
  stubMethodEntered("maximum");
  return stubReturnValue<int>("maximum");
}

int MProgressIndicatorStub::minimum() const {
  stubMethodEntered("minimum");
  return stubReturnValue<int>("minimum");
}

int MProgressIndicatorStub::value() const {
  stubMethodEntered("value");
  return stubReturnValue<int>("value");
}

bool MProgressIndicatorStub::unknownDuration() const {
  stubMethodEntered("unknownDuration");
  return stubReturnValue<bool>("unknownDuration");
}

void MProgressIndicatorStub::setMaximum(int maximum) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(maximum));
  stubMethodEntered("setMaximum",params);
}

void MProgressIndicatorStub::setMinimum(int minimum) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(minimum));
  stubMethodEntered("setMinimum",params);
}

void MProgressIndicatorStub::setRange(int minimum, int maximum) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(minimum));
  params.append( new Parameter<int >(maximum));
  stubMethodEntered("setRange",params);
}

void MProgressIndicatorStub::setValue(int value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(value));
  stubMethodEntered("setValue",params);
}

void MProgressIndicatorStub::setUnknownDuration(bool unknownDuration) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(unknownDuration));
  stubMethodEntered("setUnknownDuration",params);
}

void MProgressIndicatorStub::MProgressIndicatorConstructor(MProgressIndicatorPrivate *dd, MProgressIndicatorModel *model, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
MProgressIndicatorStub gDefaultMProgressIndicatorStub;
MProgressIndicatorStub* gMProgressIndicatorStub = &gDefaultMProgressIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MProgressIndicator::~MProgressIndicator() {
  gMProgressIndicatorStub->MProgressIndicatorDestructor();
}

int MProgressIndicator::maximum() const {
  return gMProgressIndicatorStub->maximum();
}

int MProgressIndicator::minimum() const {
  return gMProgressIndicatorStub->minimum();
}

int MProgressIndicator::value() const {
  return gMProgressIndicatorStub->value();
}

bool MProgressIndicator::unknownDuration() const {
  return gMProgressIndicatorStub->unknownDuration();
}

void MProgressIndicator::setMaximum(int maximum) {
  gMProgressIndicatorStub->setMaximum(maximum);
}

void MProgressIndicator::setMinimum(int minimum) {
  gMProgressIndicatorStub->setMinimum(minimum);
}

void MProgressIndicator::setRange(int minimum, int maximum) {
  gMProgressIndicatorStub->setRange(minimum, maximum);
}

void MProgressIndicator::setValue(int value) {
  gMProgressIndicatorStub->setValue(value);
}

void MProgressIndicator::setUnknownDuration(bool unknownDuration) {
  gMProgressIndicatorStub->setUnknownDuration(unknownDuration);
}

MProgressIndicator::MProgressIndicator(MProgressIndicatorPrivate *dd, MProgressIndicatorModel *model, QGraphicsItem *parent) {
  gMProgressIndicatorStub->MProgressIndicatorConstructor(dd, model, parent);
}


#endif
