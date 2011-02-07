#ifndef MDESKTOPENTRY_STUB
#define MDESKTOPENTRY_STUB

#include "mdesktopentry.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MDesktopEntryStub : public StubBase {
  public:
  virtual void MDesktopEntryConstructor(const QString &fileName);
  virtual void MDesktopEntryDestructor();
  virtual QString fileName() const;
  virtual bool isValid() const;
  virtual uint hash() const;
  virtual QString type() const;
  virtual QString version() const;
  virtual QString name() const;
  virtual QString nameUnlocalized() const;
  virtual QString genericName() const;
  virtual bool noDisplay() const;
  virtual QString comment() const;
  virtual QString icon() const;
  virtual bool hidden() const;
  virtual QStringList onlyShowIn() const;
  virtual QStringList notShowIn() const;
  virtual QString tryExec() const;
  virtual QString exec() const;
  virtual QString path() const;
  virtual bool terminal() const;
  virtual QStringList mimeType() const;
  virtual QStringList categories() const;
  virtual bool startupNotify() const;
  virtual QString startupWMClass() const;
  virtual QString url() const;
  virtual QString xMaemoService() const;
  virtual QString value(const QString &key) const;
  virtual QString value(const QString &group, const QString &key) const;
  virtual bool contains(const QString &key) const;
  virtual bool contains(const QString &group, const QString &key) const;
  virtual void MDesktopEntryConstructor(MDesktopEntryPrivate &dd);
}; 

// 2. IMPLEMENT STUB
void MDesktopEntryStub::MDesktopEntryConstructor(const QString &fileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(fileName));
  stubMethodEntered("MDesktopEntry",params);
}
void MDesktopEntryStub::MDesktopEntryDestructor() {
  stubMethodEntered("~MDesktopEntry");
}
QString MDesktopEntryStub::fileName() const {
  stubMethodEntered("fileName");
  return stubReturnValue<QString>("fileName");
}

bool MDesktopEntryStub::isValid() const {
  stubMethodEntered("isValid");
  return stubReturnValue<bool>("isValid");
}

uint MDesktopEntryStub::hash() const {
  stubMethodEntered("hash");
  return stubReturnValue<uint>("hash");
}

QString MDesktopEntryStub::type() const {
  stubMethodEntered("type");
  return stubReturnValue<QString>("type");
}

QString MDesktopEntryStub::version() const {
  stubMethodEntered("version");
  return stubReturnValue<QString>("version");
}

QString MDesktopEntryStub::name() const {
  stubMethodEntered("name");
  return stubReturnValue<QString>("name");
}

QString MDesktopEntryStub::nameUnlocalized() const {
  stubMethodEntered("nameUnlocalized");
  return stubReturnValue<QString>("nameUnlocalized");
}

QString MDesktopEntryStub::genericName() const {
  stubMethodEntered("genericName");
  return stubReturnValue<QString>("genericName");
}

bool MDesktopEntryStub::noDisplay() const {
  stubMethodEntered("noDisplay");
  return stubReturnValue<bool>("noDisplay");
}

QString MDesktopEntryStub::comment() const {
  stubMethodEntered("comment");
  return stubReturnValue<QString>("comment");
}

QString MDesktopEntryStub::icon() const {
  stubMethodEntered("icon");
  return stubReturnValue<QString>("icon");
}

bool MDesktopEntryStub::hidden() const {
  stubMethodEntered("hidden");
  return stubReturnValue<bool>("hidden");
}

QStringList MDesktopEntryStub::onlyShowIn() const {
  stubMethodEntered("onlyShowIn");
  return stubReturnValue<QStringList>("onlyShowIn");
}

QStringList MDesktopEntryStub::notShowIn() const {
  stubMethodEntered("notShowIn");
  return stubReturnValue<QStringList>("notShowIn");
}

QString MDesktopEntryStub::tryExec() const {
  stubMethodEntered("tryExec");
  return stubReturnValue<QString>("tryExec");
}

QString MDesktopEntryStub::exec() const {
  stubMethodEntered("exec");
  return stubReturnValue<QString>("exec");
}

QString MDesktopEntryStub::path() const {
  stubMethodEntered("path");
  return stubReturnValue<QString>("path");
}

bool MDesktopEntryStub::terminal() const {
  stubMethodEntered("terminal");
  return stubReturnValue<bool>("terminal");
}

QStringList MDesktopEntryStub::mimeType() const {
  stubMethodEntered("mimeType");
  return stubReturnValue<QStringList>("mimeType");
}

QStringList MDesktopEntryStub::categories() const {
  stubMethodEntered("categories");
  return stubReturnValue<QStringList>("categories");
}

bool MDesktopEntryStub::startupNotify() const {
  stubMethodEntered("startupNotify");
  return stubReturnValue<bool>("startupNotify");
}

QString MDesktopEntryStub::startupWMClass() const {
  stubMethodEntered("startupWMClass");
  return stubReturnValue<QString>("startupWMClass");
}

QString MDesktopEntryStub::url() const {
  stubMethodEntered("url");
  return stubReturnValue<QString>("url");
}

QString MDesktopEntryStub::xMaemoService() const {
  stubMethodEntered("xMaemoService");
  return stubReturnValue<QString>("xMaemoService");
}

QString MDesktopEntryStub::value(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(key));
  stubMethodEntered("value",params);
  return stubReturnValue<QString>("value");
}

QString MDesktopEntryStub::value(const QString &group, const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(group));
  params.append( new Parameter<QString>(key));
  stubMethodEntered("value",params);
  return stubReturnValue<QString>("value");
}

bool MDesktopEntryStub::contains(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(key));
  stubMethodEntered("contains",params);
  return stubReturnValue<bool>("contains");
}

bool MDesktopEntryStub::contains(const QString &group, const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(group));
  params.append( new Parameter<QString>(key));
  stubMethodEntered("contains",params);
  return stubReturnValue<bool>("contains");
}

void MDesktopEntryStub::MDesktopEntryConstructor(MDesktopEntryPrivate &dd) {
  Q_UNUSED(dd);

}


// 3. CREATE A STUB INSTANCE
MDesktopEntryStub gDefaultMDesktopEntryStub;
MDesktopEntryStub* gMDesktopEntryStub = &gDefaultMDesktopEntryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MDesktopEntry::MDesktopEntry(const QString &fileName) : d_ptr(NULL) {
  gMDesktopEntryStub->MDesktopEntryConstructor(fileName);
}

MDesktopEntry::~MDesktopEntry() {
  gMDesktopEntryStub->MDesktopEntryDestructor();
}

QString MDesktopEntry::fileName() const {
  return gMDesktopEntryStub->fileName();
}

bool MDesktopEntry::isValid() const {
  return gMDesktopEntryStub->isValid();
}

uint MDesktopEntry::hash() const {
  return gMDesktopEntryStub->hash();
}

QString MDesktopEntry::type() const {
  return gMDesktopEntryStub->type();
}

QString MDesktopEntry::version() const {
  return gMDesktopEntryStub->version();
}

QString MDesktopEntry::name() const {
  return gMDesktopEntryStub->name();
}

QString MDesktopEntry::nameUnlocalized() const {
  return gMDesktopEntryStub->nameUnlocalized();
}

QString MDesktopEntry::genericName() const {
  return gMDesktopEntryStub->genericName();
}

bool MDesktopEntry::noDisplay() const {
  return gMDesktopEntryStub->noDisplay();
}

QString MDesktopEntry::comment() const {
  return gMDesktopEntryStub->comment();
}

QString MDesktopEntry::icon() const {
  return gMDesktopEntryStub->icon();
}

bool MDesktopEntry::hidden() const {
  return gMDesktopEntryStub->hidden();
}

QStringList MDesktopEntry::onlyShowIn() const {
  return gMDesktopEntryStub->onlyShowIn();
}

QStringList MDesktopEntry::notShowIn() const {
  return gMDesktopEntryStub->notShowIn();
}

QString MDesktopEntry::tryExec() const {
  return gMDesktopEntryStub->tryExec();
}

QString MDesktopEntry::exec() const {
  return gMDesktopEntryStub->exec();
}

QString MDesktopEntry::path() const {
  return gMDesktopEntryStub->path();
}

bool MDesktopEntry::terminal() const {
  return gMDesktopEntryStub->terminal();
}

QStringList MDesktopEntry::mimeType() const {
  return gMDesktopEntryStub->mimeType();
}

QStringList MDesktopEntry::categories() const {
  return gMDesktopEntryStub->categories();
}

bool MDesktopEntry::startupNotify() const {
  return gMDesktopEntryStub->startupNotify();
}

QString MDesktopEntry::startupWMClass() const {
  return gMDesktopEntryStub->startupWMClass();
}

QString MDesktopEntry::url() const {
  return gMDesktopEntryStub->url();
}

QString MDesktopEntry::xMaemoService() const {
  return gMDesktopEntryStub->xMaemoService();
}

QString MDesktopEntry::value(const QString &key) const {
  return gMDesktopEntryStub->value(key);
}

QString MDesktopEntry::value(const QString &group, const QString &key) const {
  return gMDesktopEntryStub->value(group, key);
}

bool MDesktopEntry::contains(const QString &key) const {
  return gMDesktopEntryStub->contains(key);
}

bool MDesktopEntry::contains(const QString &group, const QString &key) const {
  return gMDesktopEntryStub->contains(group, key);
}

MDesktopEntry::MDesktopEntry(MDesktopEntryPrivate &dd) : d_ptr(NULL) {
  gMDesktopEntryStub->MDesktopEntryConstructor(dd);
}


#endif
