module;

#include <WinSock2.h>
#include <stdexcept>
#include <string>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)

export module SocketWrapper;

export class SocketWrapper {

public:
    SocketWrapper() : 
        m_Sock{ INVALID_SOCKET }
    {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        m_Sock = socket(AF_INET, SOCK_STREAM, 0);
        if (m_Sock == INVALID_SOCKET) 
        {
            throw std::runtime_error("Socket creation failed");
        }
    }
    explicit SocketWrapper(SOCKET other) :
        m_Sock(other)
    {
    }
    ~SocketWrapper() 
    {
        close();
        WSACleanup();
    }

    void bind(int port)
    {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (::bind(m_Sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) 
        {
            throw std::runtime_error("Bind failed");
        }
    }

    void listen(int backlog = SOMAXCONN) 
    {
        if (::listen(m_Sock, backlog) == SOCKET_ERROR) 
        {
            throw std::runtime_error("Listen failed");
        }
    }

    SocketWrapper accept() 
    {
        sockaddr_in clientAddress{};
        int clientAddressSize{ sizeof(clientAddress) };
        SocketWrapper clientSock{ ::accept(m_Sock, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize) };
        if (clientSock.m_Sock == INVALID_SOCKET) 
        {
            throw std::runtime_error("Accept failed");
        }
        return clientSock;
    }

    void connect(int port) 
    {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (::connect(m_Sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) 
        {
            throw std::runtime_error("Connect failed");
        }
    }

    void sendSize(const SocketWrapper& socket, int size) 
    {
        ::send(socket.m_Sock, reinterpret_cast<char*>(&size), sizeof(int), 0);
    }

    void sendMsg(const SocketWrapper& socket, std::string_view data) 
    {
        ::send(socket.m_Sock, data.data(), static_cast<int>(data.size()), 0);
    }

    int receiveSize() 
    {
        auto buf_size{ 0 };

        if (recv(m_Sock, reinterpret_cast<char*>(&buf_size), sizeof(buf_size), 0) < 0)
        {
            throw std::runtime_error("Error to receive size");
        }

        return buf_size;
    }

    std::string receiveMsg(int buf_size = 4096) 
    {
        std::string buf(buf_size, '\0');

        if (recv(m_Sock, buf.data(), buf_size, 0) < 0)
        {
            throw std::runtime_error("Error to receive message");
        }

        return buf;
    }

    void close() 
    {
        if (m_Sock != INVALID_SOCKET) {

            closesocket(m_Sock);
            m_Sock = INVALID_SOCKET;
        }
    }

private:
    SOCKET m_Sock;
};