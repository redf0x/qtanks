#include <QGuiApplication>

#include "common.h"
#include "Entity.h"
#include "Block.h"
#include "ActiveItem.h"
#include "Loader.h"
#include "Exceptions.h"

Loader* setup_main_window (QQmlApplicationEngine* e)
{
    Loader* l = new Loader(e);
    QObject* top;
    QQuickWindow* window;

    e->load (QUrl("qrc:/qml/ui/main.qml"));
    top = e->rootObjects ().at (0);
    window = qobject_cast<QQuickWindow *>(top);
    e->rootContext()->setContextProperty ("gameLdr", l);
    QObject::connect(l, SIGNAL(contentRequest(QVariant)), window, SLOT(contentRequest(QVariant)));

    return l;
}

QObject* globals_provider (QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Globals* g = new Globals();
    return g;
}

int main (int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Loader* loader = setup_main_window (&engine);

    qmlRegisterSingletonType<Globals>("Tanks.Globals", 0, 1, "Globals", globals_provider);
    qmlRegisterType<Entity>();
    qmlRegisterType<Block>();
    qmlRegisterType<ActiveItem>("Tanks.ActiveItem", 0, 1, "ActiveItem");

    loader->titleScreen ();

    app.exec ();

    return 0;
}
