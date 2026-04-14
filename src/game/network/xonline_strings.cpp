// XONLINE_E_*/XONLINE_S_* error name strings for Xbox Live HRESULT lookup.
// Source: TT decomp debug string excavation, lines 101-125.
//
// Table maps HRESULT codes (as reported by XOnline APIs) to their symbolic
// names. Used by diagnostic / error-dialog paths to log human-readable
// identifiers. Facility 0x2DB = XONLINE; severity bit distinguishes E vs S.

#include <cstdint>
#include <cstddef>

namespace {

struct XOnlineErrorName {
    int         code;
    const char* name;
};

// Canonical XDK values for Xbox 360 XOnline HRESULTs (facility 0x2DB).
// The debug string table embeds the *names*; the codes below are the
// standard platform values the original binary compared against.
const XOnlineErrorName kXOnlineErrorNames[] = {
    // Generic user state
    { (int)0x82DB0000, "XONLINE_E_USER_NOT_LOGGED_ON"                  }, // 0x820686B8

    // Logon failures (E_)
    { (int)0x82DB0006, "XONLINE_E_LOGON_NO_NETWORK_CONNECTION"         }, // 0x820688BC
    { (int)0x82DB0008, "XONLINE_E_LOGON_CANNOT_ACCESS_SERVICE"         }, // 0x820688E4
    { (int)0x82DB0009, "XONLINE_E_LOGON_UPDATE_REQUIRED"               }, // 0x8206890C
    { (int)0x82DB000A, "XONLINE_E_LOGON_SERVERS_TOO_BUSY"              }, // 0x8206892C
    { (int)0x82DB000B, "XONLINE_E_LOGON_CONNECTION_LOST"               }, // 0x82068950
    { (int)0x82DB000C, "XONLINE_E_LOGON_KICKED_BY_DUPLICATE_LOGON"     }, // 0x82068970
    { (int)0x82DB000D, "XONLINE_E_LOGON_INVALID_USER"                  }, // 0x8206899C
    { (int)0x82DB0015, "XONLINE_E_LOGON_FLASH_UPDATE_REQUIRED"         }, // 0x820689BC

    // Logon status (S_)
    { (int)0x02DB0001, "XONLINE_S_LOGON_CONNECTION_ESTABLISHED"        }, // 0x820689E4
    { (int)0x02DB0002, "XONLINE_S_LOGON_DISCONNECTED"                  }, // 0x82068A0C
    { (int)0x02DB0003, "XONLINE_S_LOGON_CHANGING"                      }, // 0x82068A2C
    { (int)0x02DB0004, "XONLINE_S_LOGON_TICKETS_ONLY"                  }, // 0x82068A48

    // Per-service logon failures
    { (int)0x82DB0010, "XONLINE_E_LOGON_SERVICE_NOT_REQUESTED"         }, // 0x82068A68
    { (int)0x82DB0011, "XONLINE_E_LOGON_SERVICE_NOT_AUTHORIZED"        }, // 0x82068A90
    { (int)0x82DB0012, "XONLINE_E_LOGON_SERVICE_TEMPORARILY_UNAVAILABLE"}, // 0x82068AB8
    { (int)0x02DB0005, "XONLINE_S_LOGON_USER_HAS_MESSAGE"              }, // 0x82068AE8
    { (int)0x82DB0016, "XONLINE_E_LOGON_USER_ACCOUNT_REQUIRES_MANAGEMENT" }, // 0x82068B0C
    { (int)0x82DB0017, "XONLINE_E_LOGON_MU_NOT_MOUNTED"                }, // 0x82068B40
    { (int)0x82DB0018, "XONLINE_E_LOGON_MU_IO_ERROR"                   }, // 0x82068B60
    { (int)0x82DB0019, "XONLINE_E_LOGON_NOT_LOGGED_ON"                 }, // 0x82068B7C

    // Session / matchmaking
    { (int)0x82DB0100, "XONLINE_E_SESSION_NOT_LOGGED_ON"               }, // 0x82069070

    // Competition / topology
    { (int)0x82DB0201, "XONLINE_E_COMP_TOPOLOGY_ERROR"                 }, // 0x820693B4
    { (int)0x82DB0202, "XONLINE_E_COMP_TOPOLOGY_PENDING"               }, // 0x820693D4

    // Feedback
    { (int)0x82DB0501, "XONLINE_E_FEEDBACK_CANNOT_LOG"                 }, // 0x820695BC
};

const size_t kXOnlineErrorNameCount =
    sizeof(kXOnlineErrorNames) / sizeof(kXOnlineErrorNames[0]);

// Related diagnostic strings referenced from error-dialog and XNet init paths.
const char* const kOnlineNotAuthorizedFormat =
    "online STILL NOT authorized, showing error dialog! (errorCode = %d)"; // 0x8205D0D8

const char* const kXNAddrNotInitializedMsg =
    "  XNADDR: Either the network stack is not initialized or no debugger was found"; // 0x82068568

} // namespace

// Public lookup: returns symbolic name for an XOnline HRESULT, or nullptr
// if the code is not in the table.
extern "C" const char* XOnlineGetErrorName(int hr) {
    for (size_t i = 0; i < kXOnlineErrorNameCount; ++i) {
        if (kXOnlineErrorNames[i].code == hr) {
            return kXOnlineErrorNames[i].name;
        }
    }
    return nullptr;
}
