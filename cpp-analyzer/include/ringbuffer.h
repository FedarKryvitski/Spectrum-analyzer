#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdexcept>
#include <vector>

template <typename T> class RingBuffer final
{
  public:
    RingBuffer(size_t capacity) noexcept;
    ~RingBuffer() = default;

    void push(const T &item);
    void pop();
    void clear();

    T front() const;
    size_t size() const;
    size_t capacity() const;
    bool isEmpty() const;
    bool isFull() const;

  private:
    std::vector<T> buffer_;
    size_t capacity_{}, size_{};
    size_t head_{}, tail_{};
};

template <typename T> RingBuffer<T>::RingBuffer(size_t capacity) noexcept : capacity_(capacity)
{
    buffer_.resize(capacity);
}

template <typename T> void RingBuffer<T>::clear()
{
    size_ = 0;
    head_ = 0;
    tail_ = 0;
}

template <typename T> bool RingBuffer<T>::isEmpty() const
{
    return size_ == 0;
}

template <typename T> bool RingBuffer<T>::isFull() const
{
    return size_ == capacity_;
}

template <typename T> void RingBuffer<T>::push(const T &item)
{
    if (isFull())
    {
        tail_ = (tail_ + 1) % capacity_;
    }
    else
    {
        size_++;
    }
    buffer_[head_] = item;
    head_ = (head_ + 1) % capacity_;
}

template <typename T> T RingBuffer<T>::front() const
{
    if (isEmpty())
    {
        throw std::runtime_error("Buffer is empty");
    }
    T item = buffer_[tail_];
    return item;
}

template <typename T> void RingBuffer<T>::pop()
{
    if (isEmpty())
    {
        throw std::runtime_error("Buffer is empty");
    }
    tail_ = (tail_ + 1) % capacity_;
    size_--;
}

template <typename T> size_t RingBuffer<T>::size() const
{
    return size_;
}

template <typename T> size_t RingBuffer<T>::capacity() const
{
    return capacity_;
}

#endif // RINGBUFFER_H
