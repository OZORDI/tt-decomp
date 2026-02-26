#pragma once

/**
 * Xbox 360 Network API - XNet Functions
 * Rockstar Presents Table Tennis
 * 
 * These are kernel import thunks for Xbox 360 networking (XNet).
 * XNet is the Xbox 360 network stack that sits above BSD sockets.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
#define ERROR_SUCCESS                0
#define ERROR_NOT_SUPPORTED          50
#define ERROR_INVALID_PARAMETER      87
#define WSAEINVAL                    10022
#define WSAEWOULDBLOCK               10035

// XNet address structure (Xbox 360 secure address)
typedef struct _XNADDR {
    uint8_t  abEnet[6];          // Ethernet MAC address
    uint8_t  abOnline[20];       // Online identification
    uint16_t wPortOnline;        // Online port
} XNADDR;

// XNet key exchange key
typedef struct _XNKID {
    uint8_t ab[8];               // Session ID
} XNKID;

// XNet key
typedef struct _XNKEY {
    uint8_t ab[16];              // Session key
} XNKEY;

// QoS lookup structure
typedef struct _XNQOS {
    uint32_t cxnqos;             // Number of QoS entries
    uint32_t cxnqosPending;      // Number pending
    // ... additional fields
} XNQOS;

//=============================================================================
// XNet Initialization Functions
//=============================================================================

/**
 * XNetStartup @ 0x8258603C | ordinal: varies
 * 
 * Initializes the Xbox 360 network stack.
 * 
 * @param pDummy Reserved, must be NULL
 * @return ERROR_SUCCESS if successful
 */
uint32_t XNetStartup(void* pDummy);

/**
 * XNetCleanup @ 0x8258604C | ordinal: varies
 * 
 * Shuts down the Xbox 360 network stack.
 * 
 * @return ERROR_SUCCESS if successful
 */
uint32_t XNetCleanup(void);

//=============================================================================
// XNet Address Functions
//=============================================================================

/**
 * XNetGetTitleXnAddr @ 0x825860AC | ordinal: varies
 * 
 * Retrieves the title's XNet address.
 * 
 * @param pAddr Pointer to XNADDR structure to receive address
 * @return State flags (0 = not ready, non-zero = ready)
 */
uint32_t XNetGetTitleXnAddr(XNADDR* pAddr);

/**
 * XNetXnAddrToInAddr @ 0x8258605C | ordinal: varies
 * 
 * Converts an XNet address to an IP address.
 * 
 * @param pxna    Pointer to XNADDR
 * @param pxnkid  Pointer to XNKID (session ID)
 * @param pina    Pointer to receive IP address (in_addr)
 * @return ERROR_SUCCESS if successful
 */
uint32_t XNetXnAddrToInAddr(const XNADDR* pxna, const XNKID* pxnkid, uint32_t* pina);

/**
 * XNetInAddrToXnAddr @ 0x8258606C | ordinal: varies
 * 
 * Converts an IP address to an XNet address.
 * 
 * @param ina     IP address (in_addr)
 * @param pxna    Pointer to receive XNADDR
 * @param pxnkid  Pointer to receive XNKID (session ID)
 * @return ERROR_SUCCESS if successful
 */
uint32_t XNetInAddrToXnAddr(uint32_t ina, XNADDR* pxna, XNKID* pxnkid);

//=============================================================================
// XNet QoS (Quality of Service) Functions
//=============================================================================

/**
 * XNetQosListen @ 0x8258607C | ordinal: varies
 * 
 * Starts listening for QoS probes.
 * 
 * @param pxnkid      Pointer to session ID
 * @param pb          Pointer to data buffer
 * @param cb          Size of data buffer
 * @param dwBitsPerSec Bandwidth in bits per second
 * @param dwFlags     Flags
 * @return ERROR_SUCCESS if successful
 */
uint32_t XNetQosListen(const XNKID* pxnkid, const uint8_t* pb, uint32_t cb,
                       uint32_t dwBitsPerSec, uint32_t dwFlags);

/**
 * XNetQosLookup @ 0x8258608C | ordinal: varies
 * 
 * Performs QoS lookup to measure network quality.
 * 
 * @param cxna        Number of addresses
 * @param axna        Array of XNADDR structures
 * @param axnkid      Array of XNKID structures
 * @param axnkey      Array of XNKEY structures
 * @param cina        Number of IP addresses
 * @param aina        Array of IP addresses
 * @param ainaService Array of service IP addresses
 * @param cProbes     Number of probes
 * @param dwBitsPerSec Bandwidth in bits per second
 * @param dwFlags     Flags
 * @param phEvent     Pointer to receive event handle
 * @param pxnqos      Pointer to receive QoS results
 * @return ERROR_SUCCESS if successful
 */
uint32_t XNetQosLookup(uint32_t cxna, const XNADDR* axna, const XNKID* axnkid,
                       const XNKEY* axnkey, uint32_t cina, const uint32_t* aina,
                       const uint32_t* ainaService, uint32_t cProbes,
                       uint32_t dwBitsPerSec, uint32_t dwFlags, void** phEvent,
                       XNQOS** pxnqos);

/**
 * XNetQosRelease @ 0x8258609C | ordinal: varies
 * 
 * Releases QoS lookup resources.
 * 
 * @param pxnqos Pointer to XNQOS structure
 * @return ERROR_SUCCESS if successful
 */
uint32_t XNetQosRelease(XNQOS* pxnqos);

//=============================================================================
// BSD Socket Functions (NetDll wrappers)
//=============================================================================

/**
 * sendto @ 0x8258600C | ordinal: varies
 * 
 * Sends data to a specific destination (UDP).
 * 
 * @param s       Socket descriptor
 * @param buf     Data buffer
 * @param len     Length of data
 * @param flags   Flags
 * @param to      Destination address
 * @param tolen   Length of address structure
 * @return Number of bytes sent, or -1 on error
 */
int32_t sendto(int32_t s, const void* buf, int32_t len, int32_t flags,
               const void* to, int32_t tolen);

/**
 * recvfrom @ 0x82585FEC | ordinal: varies
 * 
 * Receives data from a socket (UDP).
 * 
 * @param s       Socket descriptor
 * @param buf     Buffer to receive data
 * @param len     Length of buffer
 * @param flags   Flags
 * @param from    Pointer to receive source address
 * @param fromlen Pointer to address structure length
 * @return Number of bytes received, or -1 on error
 */
int32_t recvfrom(int32_t s, void* buf, int32_t len, int32_t flags,
                 void* from, int32_t* fromlen);

/**
 * inet_addr @ 0x8258601C | ordinal: varies
 * 
 * Converts a string IP address to binary form.
 * 
 * @param cp String IP address (e.g., "192.168.1.1")
 * @return IP address in network byte order, or INADDR_NONE on error
 */
uint32_t inet_addr(const char* cp);

/**
 * WSAGetLastError @ 0x8258602C | ordinal: varies
 * 
 * Returns the last socket error code.
 * 
 * @return Error code (WSA error codes)
 */
int32_t WSAGetLastError(void);

#ifdef __cplusplus
}
#endif
