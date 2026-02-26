#pragma once

#include <cstdint>

/**
 * sgNode - Base scene graph node
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Foundation class for all scene graph objects in the RAGE engine.
 * Provides basic node functionality and virtual interface for hierarchy management.
 * 
 * Vtable @ 0x8203338C (32 slots, 128 bytes)
 */
class sgNode {
public:
    virtual ~sgNode();                              // @ 0x8212B350 | slot 0
    
    // Virtual interface (slots 1-31)
    virtual void vfn_1() = 0;
    virtual void vfn_2() = 0;
    virtual void vfn_3() = 0;
    virtual void vfn_4() = 0;
    virtual void vfn_5() = 0;
    virtual void vfn_6() = 0;
    virtual void vfn_7() = 0;
    virtual void vfn_8() = 0;
    virtual void vfn_9() = 0;
    virtual void vfn_10() = 0;
    virtual void vfn_11() = 0;
    virtual void vfn_12() = 0;
    virtual void vfn_13() = 0;
    virtual void vfn_14() = 0;
    virtual void vfn_15() = 0;
    virtual void vfn_16() = 0;
    virtual void vfn_17() = 0;
    virtual void vfn_18() = 0;
    virtual void vfn_19() = 0;
    virtual void Clone();                           // @ 0x8212B330 | slot 20
    virtual void vfn_21() = 0;
    virtual void Render();                          // @ 0x8212B2A0 | slot 22
    virtual void vfn_23() = 0;
    virtual void vfn_24() = 0;
    virtual void vfn_25() = 0;
    virtual void vfn_26() = 0;
    virtual void vfn_27() = 0;
    virtual void vfn_28() = 0;
    virtual void vfn_29() = 0;
    virtual void vfn_30() = 0;
    virtual void vfn_31() = 0;
};
