#include <lipstickcompositor.h>
#include <homeapplication.h>
#include <QQmlApplicationEngine>

int main(int argc, char **argv)
{
    HomeApplication app(argc, argv, QString());
    app.setCompositorPath("simplecompositor.qml");
    return app.exec();
}