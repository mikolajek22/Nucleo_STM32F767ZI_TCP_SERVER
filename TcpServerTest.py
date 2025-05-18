import socket
import time

while (1):
    clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientsocket.connect(('192.168.1.10', 5000))
    clientsocket.send(b'hello_wrld')
    data = clientsocket.recv(128)  # Odbierz dane (STM32 powinien je odesłać)
    print("Odebrane:", data.decode())
    time.sleep(1)

