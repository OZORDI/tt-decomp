/**
 * Xbox 360 Network API Implementation - BSD Sockets Backend
 * Rockstar Presents Table Tennis
 * 
 * Cross-platform implementation of Xbox 360 XNet using standard BSD sockets.
 * Most XNet functions are stubbed since the game likely doesn't use online features.
 */

#include "xam/network.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <errno.h>
    #include <unistd.h>
#endif

// Initialization flag
static uint8_t g_xnet_initialized = 0;

//=============================================================================
// XNet Initialization Functions
//=============================================================================

/**
 * XNetStartup @ 0x8258603C
 */
uint32_t XNetStartup(void* pDummy) {
    (void)pDummy;
    
    if (g_xnet_initialized) {
        return ERROR_SUCCESS;
    }
    
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        fprintf(stderr, "XNet: WSAStartup failed: %d\n", result);
        return result;
    }
#endif
    
    g_xnet_initialized = 1;
    printf("XNet: Network stack initialized\n");
    return ERROR_SUCCESS;
}

/**
 * XNetCleanup @ 0x8258604C
 */
uint32_t XNetCleanup(void) {
    if (!g_xnet_initialized) {
        return ERROR_SUCCESS;
    }
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    g_xnet_initialized = 0;
    printf("XNet: Network stack cleaned up\n");
    return ERROR_SUCCESS;
}

//=============================================================================
// XNet Address Functions (Stubbed)
//=============================================================================

/**
 * XNetGetTitleXnAddr @ 0x825860AC
 * 
 * Returns a fake XNet address for single-player/local play.
 */
uint32_t XNetGetTitleXnAddr(XNADDR* pAddr) {
    if (!pAddr) {
        return 0;
    }
    
    // Return fake address (not connected to Xbox Live)
    memset(pAddr, 0, sizeof(XNADDR));
    
    // Fake MAC address
    pAddr->abEnet[0] = 0x00;
    pAddr->abEnet[1] = 0x50;
    pAddr->abEnet[2] = 0xF2;
    pAddr->abEnet[3] = 0x00;
    pAddr->abEnet[4] = 0x00;
    pAddr->abEnet[5] = 0x01;
    
    // Return 0 = not connected to Xbox Live (single-player mode)
    return 0;
}

/**
 * XNetXnAddrToInAddr @ 0x8258605C
 */
uint32_t XNetXnAddrToInAddr(const XNADDR* pxna, const XNKID* pxnkid, uint32_t* pina) {
    (void)pxna;
    (void)pxnkid;
    
    if (!pina) {
        return ERROR_INVALID_PARAMETER;
    }
    
    // Return localhost for local play
    *pina = htonl(0x7F000001);  // 127.0.0.1
    return ERROR_SUCCESS;
}

/**
 * XNetInAddrToXnAddr @ 0x8258606C
 */
uint32_t XNetInAddrToXnAddr(uint32_t ina, XNADDR* pxna, XNKID* pxnkid) {
    (void)ina;
    
    if (!pxna || !pxnkid) {
        return ERROR_INVALID_PARAMETER;
    }
    
    // Return fake addresses
    memset(pxna, 0, sizeof(XNADDR));
    memset(pxnkid, 0, sizeof(XNKID));
    
    return ERROR_SUCCESS;
}

//=============================================================================
// XNet QoS Functions (Stubbed)
//=============================================================================

/**
 * XNetQosListen @ 0x8258607C
 */
uint32_t XNetQosListen(const XNKID* pxnkid, const uint8_t* pb, uint32_t cb,
                       uint32_t dwBitsPerSec, uint32_t dwFlags) {
    (void)pxnkid;
    (void)pb;
    (void)cb;
    (void)dwBitsPerSec;
    (void)dwFlags;
    
    printf("XNet: XNetQosListen called (stubbed)\n");
    return ERROR_SUCCESS;
}

/**
 * XNetQosLookup @ 0x8258608C
 */
uint32_t XNetQosLookup(uint32_t cxna, const XNADDR* axna, const XNKID* axnkid,
                       const XNKEY* axnkey, uint32_t cina, const uint32_t* aina,
                       const uint32_t* ainaService, uint32_t cProbes,
                       uint32_t dwBitsPerSec, uint32_t dwFlags, void** phEvent,
                       XNQOS** pxnqos) {
    (void)cxna;
    (void)axna;
    (void)axnkid;
    (void)axnkey;
    (void)cina;
    (void)aina;
    (void)ainaService;
    (void)cProbes;
    (void)dwBitsPerSec;
    (void)dwFlags;
    
    if (phEvent) {
        *phEvent = NULL;
    }
    if (pxnqos) {
        *pxnqos = NULL;
    }
    
    printf("XNet: XNetQosLookup called (stubbed)\n");
    return ERROR_NOT_SUPPORTED;
}

/**
 * XNetQosRelease @ 0x8258609C
 */
uint32_t XNetQosRelease(XNQOS* pxnqos) {
    (void)pxnqos;
    return ERROR_SUCCESS;
}

//=============================================================================
// BSD Socket Functions
//=============================================================================

/**
 * sendto @ 0x8258600C
 * 
 * Direct passthrough to system sendto().
 */
int32_t sendto(int32_t s, const void* buf, int32_t len, int32_t flags,
               const void* to, int32_t tolen) {
#ifdef _WIN32
    return (int32_t)::sendto((SOCKET)s, (const char*)buf, len, flags,
                             (const struct sockaddr*)to, tolen);
#else
    return (int32_t)::sendto(s, buf, (size_t)len, flags,
                             (const struct sockaddr*)to, (socklen_t)tolen);
#endif
}

/**
 * recvfrom @ 0x82585FEC
 * 
 * Direct passthrough to system recvfrom().
 */
int32_t recvfrom(int32_t s, void* buf, int32_t len, int32_t flags,
                 void* from, int32_t* fromlen) {
#ifdef _WIN32
    int fromlen_int = fromlen ? *fromlen : 0;
    int result = (int32_t)::recvfrom((SOCKET)s, (char*)buf, len, flags,
                                     (struct sockaddr*)from, &fromlen_int);
    if (fromlen) {
        *fromlen = fromlen_int;
    }
    return result;
#else
    socklen_t fromlen_socklen = fromlen ? (socklen_t)*fromlen : 0;
    int result = (int32_t)::recvfrom(s, buf, (size_t)len, flags,
                                     (struct sockaddr*)from, &fromlen_socklen);
    if (fromlen) {
        *fromlen = (int32_t)fromlen_socklen;
    }
    return result;
#endif
}

/**
 * inet_addr @ 0x8258601C
 * 
 * Direct passthrough to system inet_addr().
 */
uint32_t inet_addr(const char* cp) {
#ifdef _WIN32
    return ::inet_addr(cp);
#else
    return ::inet_addr(cp);
#endif
}

/**
 * WSAGetLastError @ 0x8258602C
 * 
 * Returns last socket error.
 */
int32_t WSAGetLastError(void) {
#ifdef _WIN32
    return ::WSAGetLastError();
#else
    // On POSIX, map errno to WSA error codes
    switch (errno) {
        case EINVAL:
            return WSAEINVAL;
        case EWOULDBLOCK:
            return WSAEWOULDBLOCK;
        default:
            return errno;
    }
#endif
}
