module;

#include <WinSock2.h>
#include <stdexcept>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)



export module SocketWrapper;

export class SocketWrapper {

public:
    SocketWrapper() : 
        m_Sock{ INVALID_SOCKET }
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
        {
            throw std::runtime_error("WSAStartup failed");
        }
    }
    ~SocketWrapper() 
    {
        close();
        WSACleanup();
    }

    void create(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0) 
    {
        m_Sock = socket(af, type, protocol);
        if (m_Sock == INVALID_SOCKET) 
        {
            throw std::runtime_error("Socket creation failed");
        }
    }

    void bind(std::string_view ip = "127.0.0.1", int port = 27015)
    {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.data()); //inet_addr("192.168.213.128");

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

    SOCKET accept() 
    {
        sockaddr_in clientAddress{};
        int clientAddressSize{ sizeof(clientAddress) };
        SOCKET clientSock{ ::accept(m_Sock, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize) };
        if (clientSock == INVALID_SOCKET) 
        {
            throw std::runtime_error("Accept failed");
        }
        return clientSock;
    }

    void connect(std::string_view ip = "127.0.0.1", int port = 27015) 
    {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.data());

        if (::connect(m_Sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) 
        {
            throw std::runtime_error("Connect failed");
        }
    }

    void send(SOCKET socket, std::string_view data) 
    {
        auto size{ data.size() };
        
        ::send(socket, reinterpret_cast<char*>(&size), sizeof(int), 0);

        ::send(socket, data.data(), static_cast<int>(data.size()), 0);
    }

    std::string receive() 
    {
        auto buf_size{ 0 };
        std::string buf;

        if (recv(m_Sock, reinterpret_cast<char*>(&buf_size), sizeof(int), 0) < 0)
        {
            throw std::runtime_error("Error to receive size");
        }
  
        buf.reserve(buf_size);

        if (recv(m_Sock, buf.data(), buf_size, 0) < 0)
        {
            throw std::runtime_error("Error to receive message");
        }
        return buf.data();
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