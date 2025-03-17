# SocketWrapper
SocketWrapper - Educational and just-for-fun project on С++

This is project for comfortable working with Windows sockets. 
To use it in your project add the module "SocketWrapper" and import it.

# Server example
```cpp
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
```

# Client example
```cpp
#include <iostream>

import SocketWrapper;

int main() noexcept
{
	SocketWrapper client_socket;

	client_socket.create();
	client_socket.connect();
	std::cout << client_socket.receive() << std::endl;

	return 0;
}
```
