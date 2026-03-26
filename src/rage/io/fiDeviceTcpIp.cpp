/**
 * rage::fiDeviceTcpIp — TCP/IP File Device Implementation
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * Network socket wrappers for RAGE's file I/O over TCP.
 * All wrappers prepend the XNet startup handle (=1) before
 * calling the Xbox NetDll kernel functions.
 */

#include <cstdint>

// Xbox NetDll kernel socket functions
extern "C" {
    int NetDll_getsockname(int handle, int socket, void* name, int* namelen);
    int NetDll_connect(int handle, int socket, const void* name, int namelen);
    int NetDll_listen(int handle, int socket, int backlog);
    int NetDll_accept(int handle, int socket, void* addr, int* addrlen);
    int NetDll_recv(int handle, int socket, void* buf, int len, int flags);
    int NetDll_send(int handle, int socket, const void* buf, int len, int flags);
    int NetDll_inet_addr(int handle, const char* cp);
    int NetDll_socket(int handle, int af, int type, int protocol);
    int NetDll_bind(int handle, int socket, const void* name, int namelen);
    int NetDll_closesocket(int handle, int socket);
}

namespace rage {

// ─────────────────────────────────────────────────────────────────────────────
// XNet Socket Wrappers (16-24B each)
// All prepend handle=1 and shift args right by one register slot.
// ─────────────────────────────────────────────────────────────────────────────

/**
 * fiDeviceTcpIp::GetSockName @ 0x82481B70 | size: 0x14
 * Wrapper for NetDll_getsockname with XNet handle=1.
 */
int fiDeviceTcpIp_GetSockName(int socket, void* name, int* namelen) {
    return NetDll_getsockname(1, socket, name, namelen);
}

/**
 * fiDeviceTcpIp::Connect @ 0x82481BA0 | size: 0x14
 * Wrapper for NetDll_connect with XNet handle=1.
 */
int fiDeviceTcpIp_Connect(int socket, const void* name, int namelen) {
    return NetDll_connect(1, socket, name, namelen);
}

/**
 * fiDeviceTcpIp::Listen @ 0x82481BB8 | size: 0x10
 * Wrapper for NetDll_listen with XNet handle=1.
 */
int fiDeviceTcpIp_Listen(int socket, int backlog) {
    return NetDll_listen(1, socket, backlog);
}

/**
 * fiDeviceTcpIp::Accept @ 0x82481BC8 | size: 0x14
 * Wrapper for NetDll_accept with XNet handle=1.
 */
int fiDeviceTcpIp_Accept(int socket, void* addr, int* addrlen) {
    return NetDll_accept(1, socket, addr, addrlen);
}

/**
 * fiDeviceTcpIp::Recv @ 0x82481BE0 | size: 0x18
 * Wrapper for NetDll_recv with XNet handle=1.
 */
int fiDeviceTcpIp_Recv(int socket, void* buf, int len, int flags) {
    return NetDll_recv(1, socket, buf, len, flags);
}

/**
 * fiDeviceTcpIp::Send @ 0x82481C18 | size: 0x18
 * Wrapper for NetDll_send with XNet handle=1.
 */
int fiDeviceTcpIp_Send(int socket, const void* buf, int len, int flags) {
    return NetDll_send(1, socket, buf, len, flags);
}

// ─────────────────────────────────────────────────────────────────────────────
// fiDeviceTcpIp::ConnectToHost @ 0x822E8CA8 | size: 0xD8
//
// Creates a TCP socket, binds to a local ephemeral port (1024),
// and connects to the specified host:port. Returns the socket fd
// on success, -1 on failure.
//
// If hostAddress is NULL, uses a default address from a .rdata constant
// (likely "127.0.0.1" or a dev server IP).
// ─────────────────────────────────────────────────────────────────────────────

extern "C" {
    int fiDeviceTcpIp_CreateSocket(int af, int type, int protocol);  // util_1B10 @ 0x82481B10
    void fiDeviceTcpIp_CloseSocket(int socket);                       // rage_1B28 @ 0x82481B28
}

// Default host address from .rdata (used when hostAddress is NULL)
// Original binary: lis r11,-32252; ld r11,-31440(r11) → loads 8-byte value
static const char* g_defaultTcpHost = "127.0.0.1";

/**
 * fiDeviceTcpIp::ConnectToHost @ 0x822E8CA8 | size: 0xD8
 *
 * Resolves host address via inet_addr, creates an AF_INET/SOCK_STREAM socket,
 * binds to port 1024 on INADDR_ANY, then connects to the resolved address.
 *
 * @param hostAddress  IP address string (NULL = use default)
 * @param port         TCP port number (network byte order uint16)
 * @return             Socket file descriptor, or -1 on failure
 */
int fiDeviceTcpIp_ConnectToHost(const char* hostAddress, uint16_t port) {
    // Use default address if none provided
    const char* addr = hostAddress ? hostAddress : g_defaultTcpHost;

    // Resolve address
    // sockaddr_in structure (16 bytes, zeroed)
    struct {
        uint16_t sin_family;
        uint16_t sin_port;
        uint32_t sin_addr;
        uint8_t  sin_zero[8];
    } remoteAddr = {0}, localAddr = {0};

    uint32_t resolvedAddr = (uint32_t)NetDll_inet_addr(1, addr);
    if (resolvedAddr == 0xFFFFFFFF) {
        return -1;  // Address resolution failed
    }

    // Fill remote address
    remoteAddr.sin_family = 2;  // AF_INET
    remoteAddr.sin_port = port;
    remoteAddr.sin_addr = resolvedAddr;

    // Create TCP socket (AF_INET=2, SOCK_STREAM=1, IPPROTO_TCP=0)
    int sockfd = fiDeviceTcpIp_CreateSocket(2, 1, 0);
    if (sockfd == -1) {
        return -1;
    }

    // Bind to local ephemeral port 1024 on INADDR_ANY
    localAddr.sin_family = 2;   // AF_INET
    localAddr.sin_port = 1024;  // Ephemeral port
    fiDeviceTcpIp_GetSockName(sockfd, &localAddr, nullptr);

    // Connect to remote host
    int result = fiDeviceTcpIp_Connect(sockfd, &remoteAddr, 16);
    if (result >= 0) {
        return sockfd;  // Connected successfully
    }

    // Connection failed — close socket and return error
    fiDeviceTcpIp_CloseSocket(sockfd);
    return -1;
}

} // namespace rage
