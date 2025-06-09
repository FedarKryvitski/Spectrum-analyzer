#include "iobuffer.h"

IOBuffer::IOBuffer(QObject* parent)
    : QIODevice(parent)
{}

QByteArray IOBuffer::getData()
{
    QByteArray result(std::move(m_buffer));
    m_buffer.clear();

    return result;
}

qint64 IOBuffer::readData(char* data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);

    return maxSize;
}

qint64 IOBuffer::writeData(const char* data, qint64 maxSize)
{
    std::copy(data, data + maxSize,
              std::back_inserter(m_buffer));

    emit dataReceived();
    return maxSize;
}
