import socket
import time
import serial

import threading


UDP_ADDRESS = ('localhost', 12346)
SERIAL_PORT = 'COM4'
BAUD = 115200

SQUARE_COORDINATES = (
    (20, 0, 90, 0),
    (20, 20, 90, 0),
    (0, 20, 90, 0),
    (0, 0, 90, 0),
)
NAME_ROBOT = 'palletizer'


class Coordinates:
    def __init__(self, coordinates):
        self._coordinates = coordinates
        self._index = 0

    def next(self):
        if self._index == len(self._coordinates) - 1:
            self._index = 0
        else:
            self._index += 1

    @property
    def coordinate(self):
        return self._coordinates[self._index]

    @property
    def is_start(self):
        return self._index == 0


my_coordinates = Coordinates(SQUARE_COORDINATES)
is_moving = False


def moving(kmr, knu):
    global is_moving
    is_moving = True

    x, y, z, v = my_coordinates.coordinate

    print(f'new pose x: {x} y: {y} z: {z} v: {v}')
    kmr.sendto(f'{NAME_ROBOT}:{x}:{y}:{z}:{v}#'.encode(), UDP_ADDRESS)
    my_coordinates.next()

    m_data, _ = kmr.recvfrom(1024)
    if m_data.decode().startswith('done'):
        print('got message from manipulate:')
        print(m_data.decode().split('.')[1])

        # send_data = f'{"11" if my_coordinates.is_start else "1"}\n'.encode('ascii')
        send_data = f'S:{"Wait" if my_coordinates.is_start else "Complete"}\n'.encode('ascii')
        knu.write(send_data)


def start_moving(*args):
    global is_moving

    is_moving = True
    moving(*args)
    is_moving = False


def main():
    global is_moving

    kmr = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    knu = serial.Serial(SERIAL_PORT, BAUD, timeout=0.5)

    try:
        while True:
            if knu.in_waiting > 0:
                data = knu.readline()
                try:
                    data = data.decode('ascii').strip()
                except UnicodeDecodeError:
                    print(data)
                    continue

                if data.startswith('B:') and len(data) == 4:
                    button_state = int(data.split(':')[1][:-1])
                    if not is_moving and button_state == 1:
                        print('button pressed!')
                        threading.Thread(target=start_moving, args=(kmr, knu)).start()
                elif data.startswith('E:'):
                    print(data.split(':')[1])
    except KeyboardInterrupt:
        print('Program stopped!')
        knu.close()
        kmr.close()
        exit()


if __name__ == '__main__':
    main()
