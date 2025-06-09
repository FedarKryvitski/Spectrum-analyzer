#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <stdexcept>

template <typename T>
class RingBuffer final{
public:
    RingBuffer(int capacity) noexcept;

    void push(const T& item);
    void pop();
    void clear();

    T front() const;
    int size() const;
    int capacity() const;
    bool isEmpty() const;
    bool isFull() const;

private:
    std::vector<T> m_buffer;
    int m_capacity{}, m_size{};
    int m_head{}, m_tail{};
};

template<typename T>
RingBuffer<T>::RingBuffer(int capacity) noexcept
    : m_capacity(capacity)
{
    m_buffer.resize(capacity);
}

template<typename T>
void RingBuffer<T>::clear() {
    m_size = 0;
    m_head = 0;
    m_tail = 0;
}

template<typename T>
bool RingBuffer<T>::isEmpty() const {
    return m_size == 0;
}

template<typename T>
bool RingBuffer<T>::isFull() const {
    return m_size == m_capacity;
}

template<typename T>
void RingBuffer<T>::push(const T& item) {
    if (isFull()) {
        m_tail = (m_tail + 1) % m_capacity;
    } else {
        m_size++;
    }
    m_buffer[m_head] = item;
    m_head = (m_head + 1) % m_capacity;
}

template<typename T>
T RingBuffer<T>::front() const {
    if (isEmpty()) {
        throw std::runtime_error("Buffer is empty");
    }
    T item = m_buffer[m_tail];
    return item;
}

template<typename T>
void RingBuffer<T>::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Buffer is empty");
    }
    m_tail = (m_tail + 1) % m_capacity;
    m_size--;
}

template<typename T>
int RingBuffer<T>::size() const {
    return m_size;
}

template<typename T>
int RingBuffer<T>::capacity() const {
    return m_capacity;
}

#endif // RINGBUFFER_H
