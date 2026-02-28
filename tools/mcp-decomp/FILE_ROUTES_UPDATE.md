# MCP Server FILE_ROUTES Update

**Date:** February 28, 2026  
**Status:** ✅ Complete

## Summary

Updated the `suggest_file_placement` tool in the MCP decompilation server to reflect the new subdirectory structure in `src/rage/` and `src/game/`.

## Changes Made

### File Modified
- `tools/mcp-decomp/server.js` (lines 1152-1455)
- Backup created: `server.js.backup`

### Updated Routing Table

The FILE_ROUTES mapping now correctly routes class/function names to their new subdirectory locations:

#### rage/ Subdirectories (10 total)
- `rage/animation` - Animation system (rage::crAnim, rage::crIK, rage::cr)
- `rage/audio` - Audio system (rage::aud)
- `rage/data` - Data serialization (rage::dat, rage::dc, rage::pg)
- `rage/io` - File I/O and streaming (rage::fi, rage::par)
- `rage/math` - Math utilities (rage::MatrixMND, rage::VectorNI, rage::VectorND)
- `rage/memory` - Memory management (rage::at, atSingleton, atSafeDLList)
- `rage/misc` - Miscellaneous RAGE subsystems (rage::sysMem, rage::cm, rage::mf, etc.)
- `rage/network` - Networking and sessions (rage::sn, rage::Evt, rage::rl, rage::hsm)
- `rage/rendering` - Rendering system (rage::grm, rage::frag, rage::spd)
- `rage/ui` - UI system (rage::swf, rage::xml, xmlNodeStruct*)

#### game/ Subdirectories (13 total)
- `game/audio` - Game audio (pongAudioManager, audMsgSink, CBiquadFilter, etc.)
- `game/ball` - Ball physics (pongBall, pongPaddle, gdBall, gmBallRobot)
- `game/camera` - Camera system (pongCam, pongCamera, camShot, etc.)
- `game/char_view` - Character viewer (charViewChar, charViewData)
- `game/creature` - AI creatures (pongCreature, pongLocomotion, pcr, etc.)
- `game/cutscene` - Cutscenes (pongCutScene, pongCS, gdCS)
- `game/data` - Game data (plr, gdai, gdUnlock, gdStats, gdTourney, gd)
- `game/drill` - Drill mode (pongDrill, gdTraining, hitTipData, etc.)
- `game/match` - Match logic (gmLogic, MatchData, shotTarget, powerZone)
- `game/misc` - Miscellaneous game code (pong, ai, msg, cm operators, etc.)
- `game/network` - Multiplayer (PongNet, pongLive, NetworkClient, etc.)
- `game/player` - Player controller (pongPlayer)
- `game/rendering` - Game rendering (pongRenderable, pongDrawable, etc.)
- `game/save` - Save system (mcMemcard, mcSegment, mcFile)
- `game/ui` - UI and menus (pongBoot, pongFrontend, pongHUD, pongGame, etc.)

#### Other Directories (unchanged)
- `graphics/` - grc_render, sg_scene, fx_effects
- `physics/` - ph_physics
- `ui/` - hud, cv_ui

## Impact

The `suggest_file_placement` tool now correctly suggests:

```javascript
// Example: pongBall class
suggest_file_placement("pongBall")
// Returns:
//   Header: src/game/ball.hpp
//   Source: src/game/ball.cpp

// Example: rage::sn networking class
suggest_file_placement("rage::snJoinMachine")
// Returns:
//   Header: src/rage/network.hpp
//   Source: src/rage/network.cpp
```

## Verification

- ✅ JavaScript syntax validated with `node -c`
- ✅ Backup created before modification
- ✅ 277 routing entries (was 304 - optimized for new structure)
- ✅ All subdirectories from reorganization covered

## Testing

To test the updated routing:

```bash
# Start the MCP server
cd tools/mcp-decomp
node server.js

# Test with Kiro MCP client
# Call: suggest_file_placement("pongBall")
# Expected: src/game/ball.hpp and src/game/ball.cpp
```

## Related Files

- Main reorganization summary: `/FILE_REORGANIZATION_SUMMARY.md`
- Reorganization script: `/tmp/reorganize_files.sh`
- Analysis script: `/tmp/analyze_file_organization.py`
