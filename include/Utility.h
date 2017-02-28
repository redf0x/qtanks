#ifndef UTILITY_H
#define UTILITY_H

#ifdef WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include "common.h"

QQuickItem* qml_find_item_by_name (QList<QObject*> nodes, const QString& name);
void qml_dump_objects (QList<QObject*> nodes);
void msleep (int ms);

#endif // UTILITY_H
