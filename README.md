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
	server_sock.bind(27015);
	server_sock.listen();

	auto client_socket{ server_sock.accept() };

        std::string msg{ "Hello world!" };
        server_sock.sendSize(client_socket, msg.size());
        server_sock.sendMsg(client_socket, msg);

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

	client_socket.connect();

        auto size{ client_socket.receiveSize() };
	std::cout << client_socket.receiveMsg(size) << std::endl;

	return 0;
}
```
