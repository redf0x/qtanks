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

    inline QObject* getoffender () const { return _offender; }

private:
    QObject* _offender;
};

class InvalidLevelData : public Exception {
public:
    enum Reason { JUNK, MISALIGNED_DATA, UNEXPECTED_EOF };

    InvalidLevelData(QString o, Reason r = JUNK) : Exception("invalid data in level map file"), _dataFile(o),
        _specificReason(r) { }

    inline QString getfilename () const { return _dataFile; }
    inline Reason getreasoncode () const { return _specificReason; }
    inline QString getreasondesc () const {
        switch (_specificReason) {
            case JUNK:
                return QString("Non-alpha characters");

            case MISALIGNED_DATA:
                return QString("Invalid number of rows or columns in data file");

            case UNEXPECTED_EOF:
                return QString("Unexpected end of data file. Map may be incomplete");

            default:
                return QString("Unknown");
        }
    }

private:
    QString _dataFile;
    Reason _specificReason;
};

#endif // EXCEPTION_H
