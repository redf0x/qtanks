#ifndef UTILITY_H
#define UTILITY_H

#include "common.h"

QQuickItem* qml_find_item_by_name (QList<QObject*> nodes, const QString& name);
void qml_dump_objects (QList<QObject*> nodes);

#endif // UTILITY_H
