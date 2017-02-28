#ifndef WORKQUEUE_H
#define WORKQUEUE_H

#include "common.h"

class WorkQueue : public QThread {
    Q_OBJECT

public:
    WorkQueue(QObject* parent = 0) : QThread(parent) { }
    virtual ~WorkQueue() { }

    void run_task (QRunnable*);
    void finishTasks ();

protected:
    virtual void lockQueue ();
    virtual void unlockQueue ();
    virtual void startTasks ();
    virtual void waitForTasks ();
    virtual void run ();

private:
    QVector<QRunnable*> tasks;
    QMutex mutex;

signals:
    void finishedTask (QRunnable*);
};

#endif // WORKQUEUE_H
