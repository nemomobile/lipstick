#include <QtPlugin>
#include <QDebug>
#include <DuiMashupCanvas>

#include "fixture_mashupcanvas.h"

Q_EXPORT_PLUGIN2(fixture_mashupcanvas, FixtureMashupCanvas)

FixtureMashupCanvas::FixtureMashupCanvas(QObject* parent)
    : QObject(parent)
{
}

FixtureMashupCanvas::~FixtureMashupCanvas()
{
}

bool FixtureMashupCanvas::execute(void * objectInstance, QString actionName, QHash<QString, QString> parameters, QString & stdOut)
{
    Q_UNUSED(parameters);

    DuiMashupCanvas *mashupCanvas = NULL;
    bool result = true;

    if (objectInstance != 0) {
        QGraphicsItem *item = qgraphicsitem_cast<QGraphicsItem*>((QGraphicsItem*)objectInstance);
        mashupCanvas = qobject_cast<DuiMashupCanvas*>((QGraphicsWidget*)item);
    }

    if (mashupCanvas == 0) {
        stdOut = "This fixture can be called for DuiMashupCanvas only!";
        return false;
    }

    if (actionName == "categories") {
        QStringList categories = mashupCanvas->categories();
        stdOut = categories.join(QString(","));

    } else if (actionName == "setCategories") {
        if(!parameters.contains("categories")) {
            stdOut = "This action expects an argument: categories";
            return false;
        }

        QStringList categories =
            parameters.value("categories").split(",", QString::SkipEmptyParts);
        mashupCanvas->setCategories(categories);

    } else {
        stdOut = "Unknown action: " + actionName;
        result = false;
    }
    return result;
}

