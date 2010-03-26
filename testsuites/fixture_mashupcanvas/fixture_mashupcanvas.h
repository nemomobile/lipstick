#ifndef FIXTURE_MASHUPCANVAS_H
#define FIXTURE_MASHUPCANVAS_H

#include <QObject>
#include <tasfixtureplugininterface.h>

class FixtureMashupCanvas : public QObject, public TasFixturePluginInterface
{
    Q_OBJECT
    Q_INTERFACES(TasFixturePluginInterface)

public:
    FixtureMashupCanvas(QObject* parent = 0);
    ~FixtureMashupCanvas();

    bool execute(void* objectInstance, QString actionName, QHash<QString, QString> parameters, QString & stdOut);
};

#endif
