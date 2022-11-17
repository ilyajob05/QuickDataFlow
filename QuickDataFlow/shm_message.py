import os
import sys
import ctypes as ct


class _MessageBuff(ct.c_void_p):
    pass


PMessageBuff = ct.POINTER(_MessageBuff)


class MessageBuff:
    self_path = os.path.dirname(__file__)
    lib_path = os.path.join(self_path, "build/libQuickDataFlow.so")
    if not os.path.exists(lib_path):
        lib_path = "/usr/lib/QuickDataFlow/libQuickDataFlow.so"
        if not os.path.exists(lib_path):
            sys.stderr(f'lib path is not exist: {lib_path}')
            sys.exit()
    _dll = ct.CDLL(lib_path)

    _dll.MessageBuff_new.argtypes = ct.POINTER(ct.c_char), ct.POINTER(ct.c_char),
    ct.c_size_t, ct.c_size_t, ct.c_size_t, ct.c_size_t, ct.c_uint32, ct.c_bool, ct.c_bool
    _dll.MessageBuff_new.restype = PMessageBuff

    _dll.MessageBuff_delete.argtypes = PMessageBuff,
    _dll.MessageBuff_delete.restype = None

    _dll.MessageBuff_push_msg_sync.argtypes = PMessageBuff, ct.POINTER(ct.c_char)
    _dll.MessageBuff_push_msg_sync.restype = None

    _dll.MessageBuff_get_msg_sync.argtypes = PMessageBuff, ct.POINTER(ct.c_char)
    _dll.MessageBuff_get_msg_sync.restype = None

    _dll.MessageBuff_len_get.argtypes = PMessageBuff,
    _dll.MessageBuff_len_get.restype = ct.c_size_t

    _dll.MessageBuff_q_size_in_get.argtypes = PMessageBuff,
    _dll.MessageBuff_q_size_in_get.restype = ct.c_size_t

    _dll.MessageBuff_in_name_get.argtypes = PMessageBuff,
    _dll.MessageBuff_in_name_get.restype = ct.POINTER(ct.c_char)

    _dll.MessageBuff_out_name_get.argtypes = PMessageBuff,
    _dll.MessageBuff_out_name_get.restype = ct.POINTER(ct.c_char)

    def __init__(self, shm_src_name, shm_dst_name,
                       q_size_in_, q_size_out_,
                       element_size_in_=10, element_size_out_=10,
                       read_cycle_delay_us_=1000, clear_buffer_in=True, clear_buffer_out=True):
        self.obj = self._dll.MessageBuff_new(shm_src_name.encode('utf-8'), shm_dst_name.encode('utf-8'),
        q_size_in_, q_size_out_, element_size_in_, element_size_out_, read_cycle_delay_us_,
        clear_buffer_in, clear_buffer_out)

    def __del__(self):
        self._dll.MessageBuff_delete(self.obj)

    def push_msg(self, buff):
        self._dll.MessageBuff_push_msg_sync(self.obj, ct.cast(buff, ct.POINTER(ct.c_char)))

    def get_msg(self, buff):
        self._dll.MessageBuff_get_msg_sync(self.obj, ct.cast(buff, ct.POINTER(ct.c_char)))


if __name__ == "__main__":
    mb_first = MessageBuff("img_sender_point_o_", "img_sender_point_i_", 10, 10, 1000, 1000)
    mb_second = MessageBuff("img_sender_point_i_", "img_sender_point_o_", 10, 10, 1000, 1000)

    buff1 = bytes([1] * 1000)
    buff2 = bytes([2] * 1000)

    mb_first.push_msg(buff1)
    mb_second.push_msg(buff2)

    mb_first.get_msg(buff2)
    mb_second.get_msg(buff1)

    print(buff1)
    print(buff2)
