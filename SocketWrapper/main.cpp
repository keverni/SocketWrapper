#include <iostream>

import SocketWrapper;

int main() noexcept
{
	SocketWrapper server_socket;

	server_socket.bind(27015);

	server_socket.listen();

	auto client_socket{ server_socket.accept() };

	std::string msg{ "Hello world" };

	server_socket.sendSize(client_socket, msg.size());
	server_socket.sendMsg(client_socket, msg);

	return 0;
}