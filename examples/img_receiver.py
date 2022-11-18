from PIL import Image
from QuickDataFlow import shm_message as sm

mb_buff = sm.MessageBuff("img_sender_point_o_", "img_sender_point_i_",
                         10, 10, 640 * 480 * 3, 640 * 480 * 3)
buff = bytes([0] * (640 * 480 * 3))
mb_buff.get_msg(buff)
img = Image.frombytes("RGB", (640,480), buff)
img.show()
