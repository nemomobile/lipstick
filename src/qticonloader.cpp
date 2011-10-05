#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QIcon>
#include <QFile>
#include <QSettings>

#include "qticonloader.h"

static QString findIconHelper(const QString &pathName, const QString &icon)
{
    QStringList extensions;
    extensions << "";
    extensions << ".png";
    extensions << ".svg";
//    qDebug() << "Trying " << pathName << " for " << icon;

    foreach (const QString &extension, extensions) {
        if (QFile::exists(pathName + QDir::separator() + icon + extension))
            return pathName + QDir::separator() + icon + extension;
    }

    QStringList entryList = QDir(pathName).entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (const QString &dir, entryList) {
        QString retval = findIconHelper(QDir(pathName).absoluteFilePath(dir), icon);
        if (!retval.isNull())
            return retval;
    }

    // all else failed
    return QString();
}

/*!
 * Returns a path to a given icon ID.
 */
QString QtIconLoader::icon(const QString &name)
{
    if (QFile::exists(name)) {
        // fast path: file given
        return name;
    }

    QSettings settings("MeeGo", "IconCache");
    QString cachedPath = settings.value(name).toString();

    if (!QFile::exists(cachedPath)) {
        qDebug() << Q_FUNC_INFO << "Negative cache hit for " << name << " to " << cachedPath;
    } else {
        return cachedPath;
    }

    QStringList themes = QDir("/usr/share/themes").entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot);
    if (themes.isEmpty())
        return QString();

    // TODO: look up active theme in gconf and set it to the first search path, don't hardcode it.
    // TODO: would be nice to investigate if our fallback behaviour is actually correct, too, but meh.
    if (themes.contains("n900de") &&
        themes.at(0) != "n900de") {
        themes.removeAll("n900de");
        themes.insert(0, "n900de");
    }

    foreach (const QString &theme, themes) {
        QString retval = findIconHelper("/usr/share/themes/" + theme, name);
        if (!retval.isNull()) {
            settings.setValue(name, retval);
            return retval;
        }
    }

    // they also seem to get plonked here
    QString retval = findIconHelper("/usr/share/pixmaps/", name);
    if (!retval.isNull()) {
        settings.setValue(name, retval);
        return retval;
    }

    // I hate all application developers
    retval = findIconHelper("/usr/share/icons/hicolor/64x64/", name);
    if (!retval.isNull()) {
        settings.setValue(name, retval);
        return retval;
    }

    return QString();
}

