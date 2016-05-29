#!/usr/bin/env python

import socketserver

MAX_MSG_SIZE = 1024*1024

def connect(addr, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((addr,port)) #TODO try block and handle failure
    thread = threading.Thread(target=lambda: handle_connection(sock, addr, port))
    thread.start()


def handle_connection(sock, addr, port):
    while True:
        data = sock.recv(MAX_MSG_SIZE)
        if data == b"":
            break

       # TODO

    sock.close()


class RequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        print("new connection ;)")
        handle_connection(self.request, self.client_address[0], 0)

class Server(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass
