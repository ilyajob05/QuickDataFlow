#pragma once
#include <stdalign.h>
#include <stdint.h>
#include <vector>
#include <atomic>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <functional>

namespace fshm {

class MessageBuff
{
public:
    std::mutex input_message_waiter;
    std::mutex output_message_waiter;

    std::atomic_bool input_message_complete; /// for async read data
    std::atomic_bool output_message_complete; /// for async write data

    std::vector<uint8_t> buff; /// message buffer
    // set
    // size_t & len()       { return len_; }
    // get
    const size_t & len_get() const { return len; }
    const size_t & q_size_in_get() const {return q_size_in;}
    const char* shm_in_name_get() const {return shm_in_name;}
    const char* shm_out_name_get() const {return shm_out_name;}

    virtual ~MessageBuff();
    MessageBuff(const char *shm_src_name, const char *shm_dst_name,
                void *mem_src_element, void *mem_dst_element,
                const size_t q_size_in_, const size_t q_size_out_,
                const size_t element_size_in_, const size_t element_size_out_);

private:
    size_t len; /// size message
    void *buffPtr = nullptr; /// pointer on buff

    void *mem_src_element = nullptr; ///pointer for external src element
    void *mem_dst_element = nullptr; ///pointer for external dst element
    const char *shm_in_name; /// name of shared memory input
    const char *shm_out_name; /// name of shared memory output
    const size_t q_size_in; /// num elements for input
    const size_t q_size_out; /// num elements for output
    const size_t element_size_in; /// size for one input element
    const size_t element_size_out; /// size for one output element

    /// struct for lock shm buffer
    struct shmemq_info {
        pthread_mutex_t lock; /// mutex for memory lock
        unsigned long read_index;
        unsigned long write_index;
        char data[1];
    };

    /// atributes for shm shared memory
    typedef struct _shmemq {
        unsigned long max_count;
        unsigned int element_size;
        unsigned long max_size;
        char* name;
        int shmem_fd;
        unsigned long mmap_size;
        struct shmemq_info* mem;
    } shmemq_t;

/// todo:
//    struct cb_storage_t {
//        std::function<void(uint32_t)> cb;
//        uint32_t handle;
//    };

    std::atomic_bool thr_in_event_exit{false}; /// signal for exit
    std::atomic_bool thr_out_event_exit{false}; /// signal for exit
    std::atomic_bool thr_idle_event_exit{false}; /// signal for exit


//    std::unique_ptr<std::thread> th_reader_input{nullptr};
//    std::unique_ptr<std::thread> th_writer_out{nullptr};
    std::thread th_reader_input;
    std::thread th_writer_out;
//    std::thread th_client(client_shm, shm_dst_name, shm_src_name, &thr2_event_exit);
//    std::thread th_idle(idle, &thr_idle_event_exit);


    shmemq_t *queue_input = nullptr; /// shared memory queue input
    shmemq_t *queue_output = nullptr; /// shared memory queue output

    std::function<void(uint32_t)> cb; /// callback function for read input data

    /// generate shared memory attributes
    /// \param name name shared memory descriptor
    /// \param max_count max size of memory buffer
    /// \param element_size size for one element
    shmemq_t* shmemq_new(char const* name, size_t q_size, size_t element_size);
    /// copy data src to shred memory
    /// \param self shared memory descriptor
    /// \param src data source
    /// \param len is element size of buffer for write to memory
    bool shmemq_try_enqueue(shmemq_t* self, void* src, unsigned int len);
    /// copy shared memory to dst
    /// \param self shared memory descriptor
    /// \param dst data destination
    /// \param len is element size of buffer for write to memory
    bool shmemq_try_dequeue(shmemq_t* self, void* dst, unsigned int len);
    /// thread for write to out buffer
    static void write_to_out(MessageBuff *self, void *mem_src_element);
    /// thread for read from input buffer
    static void read_from_input(MessageBuff *self, void *mem_dst_element);
    /// clear shared memory
    void clear_shmem_attr(shmemq_t *shmem);
    /// destroy shared memory
    void shmemq_destroy(shmemq_t* self, int unlink);

};
}
