# -*- coding: utf8 -*-

import socket
import cv2
import numpy as np
from parse_cur_meta import parse_meta
from time import sleep, time
import binascii




host = '0.0.0.0'
port = 5555

address = (host, port)

socket01 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

socket01.bind(address)

i = 0
human = 0
pd_result = None

socket01.listen(5)
print('Socket Startup')

conn, addr = socket01.accept()
print('Connected by', addr)

start_time = time()

while True:
    header = bytearray(b'')
    while len(header) < 7:
        rcv_buf = conn.recv(7-len(header))
        header += rcv_buf

    type = header[2]
    #print('type = ', type)
    pkt_len = int.from_bytes(header[3:7], byteorder='little')
    #print('pkt_len =', pkt_len)

    fname = 'recv_' + str(i) + '.jpg'
    
    if pkt_len > 200000:
        print('try to get sync byte again...')
        while True:
            sync_byte_1 = conn.recv(1)
            if sync_byte_1 == 0xC0:
                print('sync 1 matched.')
                sync_byte_2 = conn.recv(1)
                if sync_byte_2 == 0x5A:
                    print('sync 2 matched.')
                    buf = conn.recv(5)
                    type = buf[0]
                    pkt_len = int.from_bytes(buf[1:5], byteorder='little')
                    print('recovery the pkt, the new pkt_len is', pkt_len)
                    break

    content = bytearray(b'')
    while len(content) < pkt_len:
        rcv_buf = conn.recv(pkt_len-len(content))
        content += rcv_buf
    #print('content length =', len(content))

    if type == 1:
        print('got jpg, size =', len(content))
        #print('get jpg at', time() - start_time)
        start_time = time()
        nparr = np.frombuffer(content, np.uint8)
        frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
        # draw box
        if 1 == human:
            cv2.putText(frame, 'human presence', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 1, cv2.LINE_AA)
        else:
            cv2.putText(frame, 'no human', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 1, cv2.LINE_AA)
        cv2.imshow('frame', frame)
    elif type == 19:
        #print('pkt_len = ', pkt_len)
        pd_result = parse_meta(content)
        human = pd_result['human_presence']
        if human == 1:
            print('human presenece')
        else:
            print('no human')
    if cv2.waitKey(1) == ord('q'):
        break
    i += 1

conn.close()
socket01.close()
print('server close')