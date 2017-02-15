#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "common.h"

class Exception {
public:
    Exception(QString m) : _msg(m) { }
    ~Exception() { }

    virtual QString getmsg () const { return _msg; }

private:
    QString _msg;
};

class InvalidOwnership : public Exception {
public:
    InvalidOwnership(QObject* o) : Exception("invalid ownership assignment to class"), _offender(o) { }

    QObject* getoffender () const { return _offender; }

private:
    QObject* _offender;
};

#endif // EXCEPTION_H
