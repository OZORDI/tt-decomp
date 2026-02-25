// ── pongDrawBucketManager  [vtable @ 0x8204E280] ──────────────────────────
//
// Manages a set of named pongDrawBucket render buckets loaded from an XML
// config file ("$/tune/settings.drawbucket").  Up to 32 buckets are
// supported.  Each bucket stores a flat array of renderable pointers that
// have been registered via AddEntry().
//
// Struct layout verified from pongDrawBucketManager_Load @ 0x82227430:
//   +0x00  vtable
//   +0x04  (padding / reserved — single read in cluster data)
//   +0x08  pongDrawBucket** m_pBuckets     heap array, xe_EC88 alloc, numBuckets*4
//   +0x0C  uint16_t m_numBuckets           count read from BucketCount: XML attr (1..32)
//   +0x0E  uint16_t m_defaultBucket        index from DefaultBucket: XML attr
//   +0x10  char m_configBasename[32]       strncpy from .rdata ptr → e.g. "settings"
//   +0x30  char m_configExt[32]            strncpy from .rdata ptr → "drawbucket"
//
// Global: g_numDrawBuckets @ 0x82606358 (SDA) — mirrors m_numBuckets
// Destructor pattern: calls atSingleton cleanup, then conditionally rage_free(this)
struct pongDrawBucketManager {
    void**      vtable;                     // +0x00
    uint32_t    m_field_04;                 // +0x04  (purpose unknown — read-only)
    pongDrawBucket** m_pBuckets;            // +0x08  xe_EC88(numBuckets * 4)
    uint16_t    m_numBuckets;               // +0x0C  from BucketCount: (1..32)
    uint16_t    m_defaultBucket;            // +0x0E  from DefaultBucket:
    char        m_configBasename[32];       // +0x10  e.g. "settings"
    char        m_configExt[32];            // +0x30  e.g. "drawbucket"
                                            //  total: >= 0x50 bytes

    virtual ~pongDrawBucketManager();       // [0] @ 0x822272B0

    void Load();                            // @ 0x82227430 — parse XML, alloc buckets
    void AddEntry();                        // @ 0x822278D8 — add renderable to bucket
};