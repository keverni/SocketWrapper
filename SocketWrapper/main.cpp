#include <iostream>

import SocketWrapper;

int main() noexcept
{
	SocketWrapper server_sock;
	server_sock.create();
	server_sock.bind();
	server_sock.listen();

	auto client_socket{ server_sock.accept() };
	server_sock.send(client_socket, "Hello world!");

	return 0;
}