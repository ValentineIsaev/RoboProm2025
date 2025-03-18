import socket
import time


def move(x, y, z, v):
    print('Moving...')
    time.sleep(5)
    print('Got on position')


def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server.settimeout(0.5)
    server.bind(('localhost', 12346))

    try:
        while True:
            try:
                data, client = server.recvfrom(1024)
            except socket.timeout:
                continue

            name, x, y, z, v = data.decode().replace('#', '').split(':')
            print(f'got pose x:{x} y:{y} z:{z} v:{v}')

            move(x, y, z, v)

            server.sendto(f'done.{name} manipulate got in pose x:{x} y:{y} z:{z} v:{v}'.encode(), client)

    except KeyboardInterrupt:
        print('Program stopped!')
        server.close()
        exit()


if __name__ == '__main__':
    main()
