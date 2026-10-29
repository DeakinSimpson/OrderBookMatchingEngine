/**
 * Created by deakin on 12/10/26.
 *
 * Resources used (a great help):
 * - https://www.youtube.com/watch?v=K3P_Lmq6pw0&t=1220s
 * -
 */

#pragma once
#include <memory>
#include <atomic>



namespace TSC
{
    template <typename T, typename Alloc = std::allocator<T>>
    class Queue : private Alloc
    {
        /**
         * we use std::allocator_traits<Alloc>::size_type instead of std::size_t
         * this because it allows for generic code to be generated for custom
         * allocators.
         *
         * if an allocator decided to use a different type other then std::size_t
         * then this would lead to undefined behavour, thus we use the size type
         * that the allocator specifies.
         */
        using allocator_traits = std::allocator_traits<Alloc>;
        using size_type = typename allocator_traits::size_type;
        using cursor_type = std::atomic<size_type>;

        size_type mask_;
        T* ring_;

        // checks whether std::size_t is always lock free
        static_assert(cursor_type::is_always_lock_free);

        /**
         * To prevent the cursors being on the same cache line (which they most
         * likely will be as they are declared right next to eachother) we must
         * specify that we want each cursor to have its own cache line.
         *
         * This is because on modern processors, to increase performance, when
         * an object in memory is retrieved, to save time, instead of just
         * reading that single object to the cpu cache, we instead grab the
         * entire cache line.
         *
         * However because both cursors reside on the cache line, it means that
         * there is a very high likelihood of false sharing, thus, when both
         * cpu cores attempt to place the same cache line in memory, the cache
         * line will be sent back to the cpu to rectify the contention between
         * the cores (greatly reducing performance)
        */

        /**
         * std::hardware_destructive_interference_size can vary between compilers
         * so instead I verified that my CPU cache lines are in fact 64 bytes
         * thus i am using 64 bytes as the width
         */
        static constexpr unsigned int CPU_CACHE_WIDTH { 64 };
        static constexpr auto hardware_destructive_interference_size
            { size_type{CPU_CACHE_WIDTH} };

        // used by push thread
        alignas(hardware_destructive_interference_size)
            cursor_type pushCursor_ {};

        /**
         * Adding caching means that the atomic pushCursor that is accessed by
         * each core only needs to be accessed when the current thread believes
         * the queue to be full.
         *
         * This means that instead of both threads trying to share a single
         * cache line, subsequently causing the other thread to wait, the
         * threads are free to push and pop until they believe that the queue
         * is full, then and only then will they get the cache line from memory
         * to confirm.
         */
        alignas(hardware_destructive_interference_size)
            size_type cachedPushCursor_ {};

        // used by pop thread
        alignas(hardware_destructive_interference_size)
            cursor_type popCursor_ {};

        alignas (hardware_destructive_interference_size)
            size_type cachedPopCursor_ {};

        /**
         * This padding is added after the popCursor_ so that nothing else
         * appears on its cache line, this is to allow the cursor to be accessed
         * without the chance of yet again causing false sharing
         */
        char padding_
            [hardware_destructive_interference_size - sizeof(size_type)] {};

        auto isFull(size_type pushCursor, size_type popCursor) const
        {
            return (pushCursor - popCursor) == capacity();
        }

        static auto isEmpty(size_type pushCursor, size_type popCursor)
        {
            return pushCursor == popCursor;
        }

        auto element(size_type cursor)
        {
            return &ring_[cursor & mask_];
        }

    public:
        /**
         *
         * @param capacity The max size of the queue
         * @param alloc Set the default allocator to Alloc
         */
        explicit Queue(size_type capacity, Alloc const& alloc = Alloc {})
            : Alloc { alloc }
            , mask_ { capacity - 1 }
            , ring_ { allocator_traits::allocate(*this, capacity) }
        {  }

        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;

        Queue(Queue&&) = delete;
        Queue& operator=(Queue&&) = delete;

        /**
         * Loop through all values and deallocate them
         *
         * Then deallocate the container
         */
        ~Queue()
        {
            while (!empty())
            {
                ring_[popCursor_ & mask_].~T();
                ++popCursor_;
            }

            allocator_traits::deallocate(*this, ring_, capacity());
        }

        auto capacity() const { return mask_ + 1; }
        auto size() const { return pushCursor_ - popCursor_; }
        auto empty() const { return size() == 0; }
        auto full() const { return size() == capacity(); }

        auto push(T const& value)
        {
            const auto pushCursor
                { pushCursor_.load(std::memory_order_relaxed) };

            if (isFull(pushCursor, cachedPopCursor_))
            {
                cachedPopCursor_ = popCursor_.load(std::memory_order_acquire);

                if (isFull(pushCursor, cachedPopCursor_))
                {
                    return false;
                }
            }

            // create new element at pushCursor and store it
            new (element(pushCursor)) T(value);
            pushCursor_.store(pushCursor + 1, std::memory_order_release);

            return true;
        }

        auto pop(T& value)
        {
            const auto popCursor
                { popCursor_.load(std::memory_order_relaxed) };

            if (isEmpty(cachedPushCursor_, popCursor))
            {
                cachedPushCursor_ = pushCursor_.load(std::memory_order_acquire);
                if (isEmpty(cachedPushCursor_, popCursor))
                {
                    return false;
                }
            }


            // copy the value to value found at popCursors location
            value = *element(popCursor);
            element(popCursor)->~T();

            popCursor_.store(popCursor + 1, std::memory_order_release);

            return true;
        }
    };
}
