/**
 * rage::swf* — Scaleform Flash UI System Classes
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 * 
 * The SWF subsystem handles Flash/Scaleform UI rendering and ActionScript
 * execution. These classes form the core of the game's menu and HUD system.
 * 
 * All classes are in the rage:: namespace and follow RAGE engine conventions.
 */

#pragma once

#include <cstdint>

// Forward declarations
namespace rage {
    class swfBASE;
    class swfSCRIPTOBJECT;
    class swfCONTEXT;
    class swfFILE;
    class swfINSTANCE;
    class swfACTIONFUNC;
    class swfSCRIPTARRAY;
    class swfOBJECT;
    class swfSPRITE;
    class swfFONT;
    class swfTEXT;
    class swfEDITTEXT;
    class swfSHAPE;
    class swfBUTTON;
    class swfBITMAP;
    class swfCMD;
    class swfCMD_DoAction;
    class swfCMD_DoInitAction;
    class swfCMD_PlaceObject2;
    class swfCMD_PlaceObject2ClipEvent;
    class swfCMD_RemoveObject2;
}

namespace rage {

/**
 * swfBASE @ vtable 0x82074CCC
 * 
 * Base class for all SWF objects. Provides fundamental vtable structure
 * and memory management for the Scaleform integration.
 */
class swfBASE {
public:
    virtual ~swfBASE();  // @ 0x823F8A98 | size: 0x48
    
protected:
    void* m_vtable;  // +0x00
};

/**
 * swfSCRIPTOBJECT @ vtable 0x82074D0C
 * 
 * ActionScript object representation. Handles script-side object instances
 * and their properties/methods.
 */
class swfSCRIPTOBJECT : public swfBASE {
public:
    swfSCRIPTOBJECT();  // @ 0x823F8AE0 | size: 0x64
    virtual ~swfSCRIPTOBJECT();  // @ 0x823F8B70 | size: 0x70
    
    virtual void vfn_6();  // @ 0x823FED10 | size: 0x8
    virtual void vfn_8();  // @ 0x823F8B48 | size: 0x24
    virtual void vfn_9();  // @ 0x823FC1A0 | size: 0x8
    virtual void vfn_10();  // @ 0x823FBFC0 | size: 0x1DC
    virtual void vfn_11();  // @ 0x823FC8A8 | size: 0x5C
    virtual void vfn_12();  // @ 0x823FC848 | size: 0x60
    virtual void vfn_13();  // @ 0x823FEA28 | size: 0x120
    
protected:
    void* m_pNext;      // +0x04 - linked list next pointer
    void* m_pPrev;      // +0x08 - linked list prev pointer
};

/**
 * swfCONTEXT @ vtable 0x82074D4C
 * 
 * Flash execution context. Manages the runtime environment for ActionScript
 * execution including variable scope and function calls.
 */
class swfCONTEXT : public swfBASE {
public:
    virtual ~swfCONTEXT();  // @ 0x823F8FF0 | size: 0x50
    virtual void ScalarDestructor(int flags);  // @ 0x823F90C8 | size: 0x16C
    virtual void vfn_2();  // @ 0x823F9238 | size: 0x44
};

/**
 * swfFILE @ vtable 0x82074D8C
 * 
 * Represents a loaded SWF file. Manages the file's resources including
 * sprites, fonts, shapes, and ActionScript bytecode.
 */
class swfFILE : public swfBASE {
public:
    virtual ~swfFILE();  // @ 0x823F8BE0 | size: 0xDC
    virtual void vfn_10();  // @ 0x823F9DC8 | size: 0xD4
    
protected:
    void** m_pResourceArray;  // +0x14 (20) - array of child resources
    uint16_t m_resourceCount;  // +0x2E (46) - number of resources
};

/**
 * swfINSTANCE @ vtable 0x82074DCC
 * 
 * Instance of a Flash display object on stage. Represents a placed sprite,
 * shape, or other visual element with transform and properties.
 */
class swfINSTANCE : public swfSCRIPTOBJECT {
public:
    virtual ~swfINSTANCE();  // @ 0x823FB600 | size: 0x70
    virtual void vfn_1();  // @ 0x823FB7A8 | size: 0xF8
    virtual void vfn_2();  // @ 0x823FB8A0 | size: 0x7C
    virtual void vfn_3();  // @ 0x823FB920 | size: 0x50
    virtual void vfn_4();  // @ 0x823FB760 | size: 0x44
    virtual void vfn_5();  // @ 0x823FB150 | size: 0xC
    virtual void vfn_6();  // @ 0x823FED18 | size: 0xD0
    virtual void vfn_7();  // @ 0x823FC908 | size: 0x144
    virtual void vfn_8();  // @ 0x823FB160 | size: 0x28
    virtual void vfn_9();  // @ 0x823FBF98 | size: 0x28
    virtual void vfn_10();  // @ 0x823FB970 | size: 0x624
    virtual void vfn_11();  // @ 0x823FC1A8 | size: 0x5F0
    virtual void vfn_12();  // @ 0x823FABC0 | size: 0xE8
    virtual void vfn_13();  // @ 0x823FE500 | size: 0x3D0
};

/**
 * swfACTIONFUNC @ vtable 0x82074E0C
 * 
 * ActionScript function object. Represents a callable function in the
 * ActionScript VM with bytecode and execution context.
 */
class swfACTIONFUNC : public swfSCRIPTOBJECT {
public:
    virtual void vfn_8();  // @ 0x823FF2A8 | size: 0x44
    virtual void vfn_9();  // @ 0x823FF3C0 | size: 0xDC
    virtual void vfn_10();  // @ 0x823FF2F0 | size: 0xCC
    virtual void vfn_11();  // @ 0x823FF4A0 | size: 0x14
    virtual void vfn_12();  // @ 0x823FF4B8 | size: 0x14
};

/**
 * swfSCRIPTARRAY @ vtable 0x82074E4C
 * 
 * ActionScript array object. Implements dynamic arrays accessible from
 * ActionScript code.
 */
class swfSCRIPTARRAY : public swfSCRIPTOBJECT {
public:
    virtual ~swfSCRIPTARRAY();  // @ 0x823FF218 | size: 0x8C
    virtual void vfn_10();  // @ 0x823FEB48 | size: 0xF0
    virtual void vfn_11();  // @ 0x823FEC38 | size: 0x70
};

/**
 * swfOBJECT @ vtable 0x82074E8C
 * 
 * Generic Flash object. Base for display list objects that can be
 * instantiated and placed on the stage.
 */
class swfOBJECT : public swfBASE {
public:
    virtual ~swfOBJECT();
};

/**
 * swfSPRITE @ vtable 0x82074ECC
 * 
 * Flash sprite/movie clip. Contains timeline, frames, and child objects.
 * The primary container for animated Flash content.
 */
class swfSPRITE : public swfOBJECT {
public:
    virtual ~swfSPRITE();
};

/**
 * swfFONT @ vtable 0x82074F0C
 * 
 * Flash font definition. Contains glyph shapes and metrics for text
 * rendering in the Flash UI.
 */
class swfFONT : public swfOBJECT {
public:
    virtual ~swfFONT();
};

/**
 * swfTEXT @ vtable 0x82074F4C
 * 
 * Static text field. Represents non-editable text placed in the Flash
 * authoring tool.
 */
class swfTEXT : public swfOBJECT {
public:
    virtual ~swfTEXT();
};

/**
 * swfEDITTEXT @ vtable 0x82074F8C
 * 
 * Dynamic/editable text field. Supports user input and dynamic text
 * updates from ActionScript.
 */
class swfEDITTEXT : public swfOBJECT {
public:
    virtual ~swfEDITTEXT();
};

/**
 * swfSHAPE @ vtable 0x82074FCC
 * 
 * Vector shape definition. Contains fill styles, line styles, and
 * edge records for rendering vector graphics.
 */
class swfSHAPE : public swfOBJECT {
public:
    virtual ~swfSHAPE();
};

/**
 * swfBUTTON @ vtable 0x8207500C
 * 
 * Interactive button. Handles mouse events and button state transitions
 * (up, over, down).
 */
class swfBUTTON : public swfOBJECT {
public:
    virtual ~swfBUTTON();
};

/**
 * swfBITMAP @ vtable 0x8207504C
 * 
 * Bitmap image resource. Represents raster images embedded in or loaded
 * by the SWF file.
 */
class swfBITMAP : public swfOBJECT {
public:
    virtual ~swfBITMAP();
};

/**
 * swfCMD @ vtable 0x8207508C
 * 
 * Base class for SWF tag commands. Represents operations in the SWF
 * bytecode stream.
 */
class swfCMD : public swfBASE {
public:
    virtual ~swfCMD();
};

/**
 * swfCMD_DoAction @ vtable 0x820750CC
 * 
 * DoAction tag. Contains ActionScript bytecode to execute when the
 * playhead reaches this frame.
 */
class swfCMD_DoAction : public swfCMD {
public:
    virtual ~swfCMD_DoAction();
};

/**
 * swfCMD_DoInitAction @ vtable 0x8207510C
 * 
 * DoInitAction tag. Executes ActionScript initialization code for a
 * sprite, runs once when the sprite is first instantiated.
 */
class swfCMD_DoInitAction : public swfCMD {
public:
    virtual ~swfCMD_DoInitAction();
    virtual void vfn_2();  // @ 0x821EFC00 | size: 0x10
};

/**
 * swfCMD_PlaceObject2 @ vtable 0x8207514C
 * 
 * PlaceObject2 tag. Places, moves, or modifies an object on the display
 * list with transform, color, and clip actions.
 */
class swfCMD_PlaceObject2 : public swfCMD {
public:
    virtual ~swfCMD_PlaceObject2();
};

/**
 * swfCMD_PlaceObject2ClipEvent @ vtable 0x8207518C
 * 
 * Clip event handler for PlaceObject2. Defines ActionScript code to
 * execute in response to movie clip events (load, enterFrame, etc).
 */
class swfCMD_PlaceObject2ClipEvent : public swfBASE {
public:
    virtual ~swfCMD_PlaceObject2ClipEvent();
};

/**
 * swfCMD_RemoveObject2 @ vtable 0x820751CC
 * 
 * RemoveObject2 tag. Removes an object from the display list at the
 * specified depth.
 */
class swfCMD_RemoveObject2 : public swfCMD {
public:
    virtual ~swfCMD_RemoveObject2();
};

} // namespace rage
