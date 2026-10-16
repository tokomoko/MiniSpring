#include "HttpClient.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

namespace {
    std::string receiveAll(SOCKET sock) {
        std::string response;
        char buffer[4096];
        int bytesReceived = 0;

        while ((bytesReceived = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
            response.append(buffer, bytesReceived);
        }

        return response;
    }
}

HttpResponse HttpClient::get(const std::string& host, int port, const std::string& path) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        HttpResponse res;
        res.status(500);
        res.body("WSAStartup failed");
        return res;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        HttpResponse res;
        res.status(500);
        res.body("Socket creation failed");
        return res;
    }

    DWORD timeout = 3000; // 3 second timeout
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&timeout), sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&timeout), sizeof(timeout));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        HttpResponse res;
        res.status(503);
        res.body("Connection refused");
        return res;
    }

    std::ostringstream request;
    request << "GET " << path << " HTTP/1.1\r\n"
        << "Host: " << host << ":" << port << "\r\n"
        << "User-Agent: MiniSpringClient/1.0\r\n"
        << "Connection: close\r\n\r\n";

    std::string reqStr = request.str();
    send(sock, reqStr.c_str(), static_cast<int>(reqStr.size()), 0);

    // Reads full buffer until socket closes or times out
    std::string rawResponse = receiveAll(sock);

    closesocket(sock);
    WSACleanup();

    return HttpResponse::parse(rawResponse);
}