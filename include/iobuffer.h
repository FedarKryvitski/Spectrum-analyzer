#ifndef IOBUFFER_H
#define IOBUFFER_H

#include <QIODevice>

class IOBuffer : public QIODevice
{
    Q_OBJECT

public:
    explicit IOBuffer(QObject* parent = nullptr);

    QByteArray getData();

signals:
    void dataReceived();

protected:
    qint64 readData(char* data, qint64 maxSize);
    qint64 writeData(const char* data, qint64 maxSize);

private:
    QByteArray m_buffer;
};

#endif // IOBUFFER_H
