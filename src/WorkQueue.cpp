#include "WorkQueue.h"

void WorkQueue::run_task (QRunnable *task)
{
    lockQueue ();
    tasks.push_back (task);
    unlockQueue ();
    startTasks ();
}

void WorkQueue::finishTasks ()
{
    waitForTasks ();
}

void WorkQueue::lockQueue ()
{
    mutex.lock ();
}

void WorkQueue::unlockQueue ()
{
    mutex.unlock ();
}

void WorkQueue::startTasks ()
{
    if (!isRunning ())
        start (LowPriority); // calls run() in a new thread
}

void WorkQueue::waitForTasks ()
{
    wait ();
}

void WorkQueue::run ()
{
    lockQueue ();

    while (!tasks.empty ()) {
        QRunnable* f = tasks [0];

        unlockQueue ();
        f->run ();
        emit finishedTask (f);
        lockQueue ();
        tasks.erase (tasks.begin ());
    }

    unlockQueue ();
}
