#include "Utility.h"

QQuickItem* qml_find_item_by_name (QList<QObject*> nodes, const QString& name)
{
    for (int i = 0; i < nodes.size (); i++) {
        if (nodes.at(i) && nodes.at (i)->objectName () == name){
            return dynamic_cast<QQuickItem*>(nodes.at (i));
        } else
            if (nodes.at (i) && nodes.at(i)->children ().size () > 0) {
                QQuickItem* item = qml_find_item_by_name (nodes.at (i)->children (), name);

                if (item)
                    return item;
            }
    }

    return NULL;
}

void qml_dump_objects (QList<QObject*> nodes)
{
    for (int i = 0; i < nodes.size (); i++) {
        if (nodes.at (i)) {
            qDebug () << nodes.at (i);
        } else
            if (nodes.at (i) && nodes.at(i)->children ().size () > 0) {
                qml_dump_objects (nodes.at (i)->children ());
            }
    }
}

void msleep (int ms)
{
#ifdef WIN32
    Sleep (ms);
#else
    usleep (ms * 1000);
#endif
}
