#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cassert>

template<size_t _SleepWhenAcquireFailedInMicroSeconds = size_t(-1)>
class SpinLockByTAS
{
    std::atomic_flag locked_flag_ = ATOMIC_FLAG_INIT;
public:

    void lock()
    {
        while (locked_flag_.test_and_set()) {
            if (_SleepWhenAcquireFailedInMicroSeconds == size_t(-1))
            {
                std::this_thread::yield();
            }
            else if (_SleepWhenAcquireFailedInMicroSeconds != 0)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(_SleepWhenAcquireFailedInMicroSeconds));
            }
        }
    }
    void unlock()
    {
        locked_flag_.clear();
    }
};

template<size_t _SleepWhenAcquireFailedInMicroSeconds = size_t(-1)>
class SpinLockByCAS
{
    std::atomic<bool> locked_flag_ = ATOMIC_VAR_INIT(false);
public:
    void lock()
    {
        bool exp = false;
        while (!locked_flag_.compare_exchange_strong(exp, true))
        {
            exp = false;
            if (_SleepWhenAcquireFailedInMicroSeconds == size_t(-1)) {
                std::this_thread::yield();
            }
            else if (_SleepWhenAcquireFailedInMicroSeconds != 0)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(_SleepWhenAcquireFailedInMicroSeconds));
            }
        }
    }
    void unlock()
    {
        locked_flag_.store(false);
    }
};

namespace zce::lockfree
{
//https://www.codeproject.com/Articles/153898/Yet-another-implementation-of-a-lock-free-circul
//��������Ҷ���֪����ʲôʱ�����ˡ�

/*!
* @tparam    _value_type ��������
* @brief     ħ��.ѭ������LILO�������Զ���չ���������ĸ��ǵ�һ����
*/
template<class _value_type >
class rings
{
public:
    ///���캯��������������,initialize
    rings() :
        read_start_(0),
        read_end_(0),
        write_start_(0),
        write_end_(0),
        rings_capacity_(0),
        value_ptr_(NULL)
    {
    }

    ///���캯����ͬʱ��ɳ�ʼ��,������ȫ û�б�Ҫ����,initialize
    rings(size_t max_len) :
        read_start_(0),
        read_end_(0),
        write_start_(0),
        write_end_(0),
        rings_capacity_(0),
        value_ptr_(NULL)
    {
        assert(max_len > 0);
        initialize(max_len);
    }

    ///�����������ͷſռ�
    ~rings()
    {
        finalize();
    }

    ///��ʼ�����������͹��캯���ɵ��������һ����ֻ�Ƕ���һ��ԭ�������ݾ������
    ///initialize ����lock-free������
    void initialize(size_t max_len)
    {
        assert(max_len > 0);

        rings_capacity_ = max_len + 1;

        read_start_.store(0, std::memory_order_release);
        read_end_.store(0, std::memory_order_release);
        write_start_.store(0, std::memory_order_release);
        write_end_.store(rings_capacity_ - 1, std::memory_order_release);

        //�����ֳ�
        if (value_ptr_)
        {
            free(value_ptr_);
            value_ptr_ = NULL;
        }
        //����new����ʹ�õĹ��캯��
        value_ptr_ = (_value_type*)::malloc(sizeof(_value_type) * rings_capacity_);
    }

    ///��������ɣ����٣�finalize��lock-free������
    void finalize()
    {
        //�����ֳ�
        if (value_ptr_)
        {
            size_t rings_size = size();
            for (size_t i = 0; i < rings_size; ++i)
            {
                value_ptr_[(read_start_ + i) % rings_capacity_].~_value_type();
            }

            ::free(value_ptr_);
            value_ptr_ = NULL;
        }
        read_start_.store(0, std::memory_order_release);
        read_end_.store(0, std::memory_order_release);
        write_start_.store(0, std::memory_order_release);
        write_end_.store(0, std::memory_order_release);
        rings_capacity_ = 0;
    }

    ///���·���һ���ռ�,resize��lock-free������
    bool resize(size_t new_max_size)
    {
        assert(new_max_size > 0);
        size_t rings_size = size();

        //���ԭ���ĳߴ�����µĳߴ磬�޷���չ
        if (rings_size > new_max_size)
        {
            return false;
        }
        new_max_size += 1;
        _value_type* new_value_ptr = (_value_type*)::malloc(sizeof(_value_type) * new_max_size);
        //���ԭ��������,�������µ�������
        if (value_ptr_ != NULL)
        {
            for (size_t i = 0; i < rings_size && i < new_max_size; ++i)
            {
                new_value_ptr[i] = value_ptr_[(read_start_ + i) % rings_capacity_];
            }

            ::free(value_ptr_);
            value_ptr_ = NULL;
        }

        //���������ڲ�����
        rings_capacity_ = new_max_size;
        read_start_.store(0, std::memory_order_release);
        read_end_.store(rings_size, std::memory_order_release);
        write_start_.store(rings_size, std::memory_order_release);
        write_end_.store(rings_capacity_ - 1, std::memory_order_release);

        value_ptr_ = new_value_ptr;

        return true;
    }

    ///����ܵ���
    void clear()
    {
        read_start_.store(0, std::memory_order_release);
        read_end_.store(0, std::memory_order_release);
        write_start_.store(0, std::memory_order_release);
        write_end_.store(rings_capacity_ - 1, std::memory_order_release);
    }

    ///�ߴ�ռ�
    inline size_t size() const
    {
        size_t end = read_end_.load(std::memory_order_acquire);
        size_t start = read_start_.load(std::memory_order_acquire);
        size_t rings_size = (rings_capacity_ - start + end) % rings_capacity_;
        return rings_size;
    }
    ///���ؿ��пռ�Ĵ�С
    inline size_t freesize() const
    {
        size_t end = write_end_.load(std::memory_order_acquire);
        size_t start = write_start_.load(std::memory_order_acquire);
        size_t free_size = (rings_capacity_ - start + end) % rings_capacity_;
        return free_size;
    }

    ///���ض��е�����
    inline size_t capacity() const
    {
        return rings_capacity_ - 1;
    }

    ////����Ƿ��Ѿ�����
    inline bool full() const
    {
        size_t end = write_end_.load(std::memory_order_acquire);
        size_t start = write_start_.load(std::memory_order_acquire);

        //�Ѿ�ֻ��һ���յ�
        if ((start + 1) % rings_capacity_ == end)
        {
            return true;
        }
        return false;
    }

    ///�ж϶����Ƿ�Ϊ��
    inline bool empty() const
    {
        //�ռ����0
        size_t end = read_end_.load(std::memory_order_acquire);
        size_t start = read_start_.load(std::memory_order_acquire);
        if (end == start)
        {
            return true;
        }
        return false;
    }

    ///��һ�����ݷ�����е�β��,��������Ѿ�����,����Խ�lay_over������λtrue,����ԭ�е�����
    bool push_back(const _value_type& value_data)
    {
        //д��ʼ����1
        size_t w_start = 0, w_end = 0;
        do
        {
            w_end = write_end_.load(std::memory_order_acquire);
            w_start = write_start_.load(std::memory_order_acquire);

            //����Ѿ�ֻ��һ���յ㣬�Ѿ�����
            if ((w_start + 1) % rings_capacity_ == w_end)
            {
                return true;
            }
        } while (!write_start_.compare_exchange_weak(w_start,
                 (w_start + 1) % rings_capacity_,
                 std::memory_order_acq_rel));

        //д�����ݣ�ֱ�ӷ��ڶ�β
        new (value_ptr_ + w_start) _value_type(value_data);

        //����������

        bool ret = false;
        do
        {
            size_t expected = w_start;
            ret = read_end_.compare_exchange_weak(expected,
                                                  (w_start + 1) % rings_capacity_,
                                                  std::memory_order_acq_rel);
        } while (!ret);
        return true;
    }

    ///�Ӷ��е�ǰ��pop���ҵõ�һ������
    bool pop_front(_value_type& value_data)
    {
        size_t r_start = 0, r_end = 0;
        do
        {
            r_start = read_start_.load(std::memory_order_acquire);
            r_end = read_end_.load(std::memory_order_acquire);
            if (r_end == r_start)
            {
                return false;
            }
        } while (!read_start_.compare_exchange_weak(r_start,
                 (r_start + 1) % rings_capacity_,
                 std::memory_order_acq_rel));
        //��ȡ���ݣ�
        value_data = value_ptr_[r_start];
        value_ptr_[r_start].~_value_type();

        bool ret = false;
        do
        {
            size_t expected = (r_start - 1 + rings_capacity_) % rings_capacity_;
            ret = write_end_.compare_exchange_weak(expected,
                                                   r_start,
                                                   std::memory_order_acq_rel);
        } while (!ret);
        return true;
    }

protected:

    ///ѭ�������ܶ�ȡ����ʼλ��[read_start_,read_end_)Ϊ�ɶ�ȡ�����䷶Χ
    ///read_end_ = read_start_ Ϊ��
    ///rings size(�Ѿ�ʹ�õĿռ�) Ϊ  rings_capacity_ - read_start_ + read_end_
    std::atomic<size_t> read_start_;
    ///ѭ�������ܶ�ȡ�Ľ���λ�ã�
    std::atomic<size_t> read_end_;

    ///ѭ�������ܿ�д����ʼλ�� [write_start_,write_end_)Ϊ����д��Ŀռ�
    ///(write_start_ +1 % rings_capacity_)= write_end_ Ϊ��(ע��ѭ��)
    std::atomic<size_t> write_start_;
    ///ѭ�����п�д�Ľ���λ�ã�
    std::atomic<size_t> write_end_;

    ///���еĳ��ȣ�
    size_t rings_capacity_;
    ///������ݵ�ָ��
    _value_type* value_ptr_;
};
};

int hello_atomic_rings(int argc, char* argv[])
{
    zce::lockfree::rings<int> abc;
    abc.initialize(100);
    for (int i = 0; i < 120; ++i)
    {
        abc.push_back(i);
    }

    for (int i = 0; i < 120; ++i)
    {
        int t;
        bool ret = abc.pop_front(t);
        if (ret)
        {
            std::cout << i << " " << std::boolalpha << ret << " " << t << std::endl;
        }
        else
        {
            std::cout << i << " " << std::boolalpha << ret << std::endl;
        }
    }

    return 0;
}

struct A
{
    int i;
    int *ptr;
};

struct B
{
    int i = 0;
    int j = 0;
};

struct C
{
    int *ptr1;
    int *ptr2;
};

struct D
{
    int64_t i = 0;
    int64_t j = 0;
};

int hello_atomic_islockfree(int argc, char* argv[])
{
    std::cout << std::boolalpha << "A " << std::atomic<A>{}.is_lock_free() << std::endl;
    std::cout << std::boolalpha << "A " << std::atomic<A>::is_always_lock_free << std::endl;

    std::cout << std::boolalpha << "B " << std::atomic<B>{}.is_lock_free() << std::endl;
    std::cout << std::boolalpha << "B " << std::atomic<B>::is_always_lock_free << std::endl;

    std::cout << std::boolalpha << "C " << std::atomic<C>{}.is_lock_free() << std::endl;
    std::cout << std::boolalpha << "C " << std::atomic<C>::is_always_lock_free << std::endl;

    std::cout << std::boolalpha << "D " << std::atomic<D>{}.is_lock_free() << std::endl;
    std::cout << std::boolalpha << "D " << std::atomic<D>::is_always_lock_free << std::endl;

    //
    std::cout << std::boolalpha << "int *" << std::atomic<int *>{}.is_lock_free() << std::endl;
    std::cout << std::boolalpha << "int *" << std::atomic<int *>::is_always_lock_free << std::endl;

    return 0;
}

int hello_atomic_size(int argc, char* argv[])
{
    std::cout << "std::atomic_bool size:" << sizeof(std::atomic_bool) << std::endl;
    std::cout << "std::atomic_char size:" << sizeof(std::atomic_char) << std::endl;
    std::cout << "std::atomic_short size:" << sizeof(std::atomic_short) << std::endl;
    std::cout << "std::atomic_int size:" << sizeof(std::atomic_int) << std::endl;
    std::cout << "std::atomic_long size:" << sizeof(std::atomic_long) << std::endl;
    std::cout << "std::atomic_llong size:" << sizeof(std::atomic_llong) << std::endl;

    return 0;
}