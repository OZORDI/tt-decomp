# Complete Auto-Naming Research Report
## Goal: Achieve 100% Function Coverage (Currently at 68.2%)

---

## Current State
- **Total functions**: 16,998
- **Named**: 11,594 (68.2%)
- **Unnamed (fn_8)**: 5,404 (31.8%)
- **Isolated (no call edges)**: 1,090 (20.2% of unnamed)
- **Connected**: 4,314 (79.8% of unnamed)

---

## Key Findings

### 1. Missing Class Names (42 classes not in KNOWN_PREFIXES)

These classes appear in Class::Method debug strings but aren't in our current prefix list:

1. **LocomotionStateAnim** - 6 methods
2. **LocomotionStateMf** - 13 methods  
3. **Timer** - general timer class
4. **audBank** - audio bank management
5. **audVoiceSfx** - audio voice effects
6. **audVoiceStream** - audio streaming
7. **ballPredict** - ball prediction system
8. **crIKBodyBase** - IK body base class
9. **crIKHead** - IK head controller
10. **crIKLimb** - IK limb controller (5 methods)
11. **crIKSpine** - IK spine controller (4 methods)
12. **crSkeketonData** - skeleton data (typo in original)
13. **fxCloth** - cloth physics effects
14. **fxHair** - hair physics effects
15. **gdCSCharCamShotNameSeq** - cutscene camera sequences
16. **gdCrAttachmentData** - creature attachment data
17. **gdCrData** - creature data
18. **gdDrillChargingData** - training drill: charging (7 methods)
19. **gdDrillCounterSpinData** - training drill: counter spin (10 methods)
20. **gdDrillFocusShotData** - training drill: focus shot
21. **gdDrillMovementData** - training drill: movement (10 methods)
22. **gdDrillServingData** - training drill: serving
23. **gdDrillSoftShotData** - training drill: soft shot (10 methods)
24. **gdDrillSpinData** - training drill: spin
25. **gdLevelData** - level/environment data
26. **gdReplayMgr** - replay manager
27. **gdRivalryData** - rivalry system data
28. **gdTrainingDrillData** - training drill base (19 methods)
29. **geomBoxes** - geometry/collision boxes
30. **gmLogicSwingPractice** - game mode: swing practice
31. **grmModelFactory** - model factory
32. **pongDrillCounterSpin** - drill implementation: counter spin
33. **pongDrillSmash** - drill implementation: smash
34. **pongDrillSoftShot** - drill implementation: soft shot
35. **pongDrillSpin** - drill implementation: spin
36. **pongLadder** - ladder/tournament system
37. **pongLerpQueue** - interpolation queue (18 functions already named)
38. **pongPlayerShaderParams** - player shader parameters
39. **pongRemotePlayer** - remote/network player
40. **pongShadowMap** - shadow mapping
41. **pongTrainingDrill** - training drill base class
42. **sysMemSimpleAllocator** - memory allocator

**Impact**: Adding these 42 classes will immediately improve class extraction and enable better propagation.

---

### 2. Function Name Patterns Found in Strings

#### Create/Init/Setup Patterns
- **CreateFragType**
- **CreateMachine**
- **CreateNewMatch**
- **InitWinSock**
- **UpdateFinalize**
- **UpdateScoreCard**
- **RenderMapLinear**
- **HandleMatchScoreMessage**
- **HandleNextMatchMessage**
- **HandleSwingStartMessage**

These suggest subsystems that could be used for naming.

---

### 3. Isolated Functions Analysis (1,090 functions)

**Size Distribution**:
- Tiny (≤8 bytes): 46 functions
- Small (9-16 bytes): 235 functions
- Medium (17-32 bytes): 429 functions
- Large (33-64 bytes): 242 functions
- Huge (>64 bytes): 138 functions

**Characteristics**:
- 37 contain BLR instructions (valid functions)
- 0 contain TRAP instructions
- Likely called indirectly (callbacks, vtables, function pointers)
- May be dead code or data misinterpreted as code

**Strategy**: Use indirect call analysis (mtctr/bctrl patterns) to find callers.

---

### 4. Connected Functions Analysis (4,314 functions)

**Current Propagation Potential**:
- Forward propagation candidates: 15 functions (meet v5b thresholds)
- Reverse propagation candidates: 198 functions (meet v5b thresholds)
- Total: 213 functions ready for naming with current thresholds

**Why v5b stopped**: Name collisions or threshold too strict.

---

## Strategies to Reach 100% Coverage

### Strategy 1: Add Missing Class Names (Immediate Impact)
**Effort**: Low  
**Expected Gain**: 5-10% coverage  
**Implementation**: Add 42 missing classes to KNOWN_PREFIXES list

### Strategy 2: Relax Propagation Thresholds
**Effort**: Low  
**Expected Gain**: 3-5% coverage  
**Current thresholds**:
- Forward: 25% (2+ calls, ratio ≥0.25)
- Reverse: 35% (2+ calls, ratio ≥0.35)
- Helper size: ≤80 bytes

**Proposed thresholds**:
- Forward: 20% (2+ calls, ratio ≥0.20)
- Reverse: 30% (2+ calls, ratio ≥0.30)
- Helper size: ≤120 bytes
- Single call wrapper: ≤100 bytes (up from 64-80)

### Strategy 3: Multi-Hop Propagation
**Effort**: Medium  
**Expected Gain**: 5-8% coverage  
**Approach**: If A→B→C and C has class X, propagate to B then A

### Strategy 4: Cross-Class Utility Detection
**Effort**: Low  
**Expected Gain**: 2-3% coverage  
**Approach**: Functions called by 3+ different classes → name as "util_XXXX"

### Strategy 5: Indirect Call Analysis
**Effort**: High  
**Expected Gain**: 3-5% coverage  
**Approach**: Track mtctr/bctrl patterns to find indirect callees

### Strategy 6: Pattern-Based Naming
**Effort**: Medium  
**Expected Gain**: 2-4% coverage  
**Patterns**:
- Tiny functions (≤16 bytes): likely getters/setters → "get_XXXX" / "set_XXXX"
- Load-only functions: "load_XXXX"
- Store-only functions: "store_XXXX"
- Math-heavy functions: "calc_XXXX" / "compute_XXXX"

### Strategy 7: Jump Table Analysis
**Effort**: Medium  
**Expected Gain**: 1-2% coverage  
**Approach**: Find switch statement jump tables, name functions in table based on context

### Strategy 8: Prologue/Epilogue Analysis
**Effort**: Medium  
**Expected Gain**: 1-2% coverage  
**Patterns**:
- No stack frame (leaf functions): "leaf_XXXX"
- Large stack frame: "complex_XXXX"
- Specific register save patterns: indicate function type

### Strategy 9: Aggressive RTTI Usage
**Effort**: Low  
**Expected Gain**: 2-3% coverage  
**Approach**: Use RTTI to classify ALL vtable functions (not just unnamed), then propagate more aggressively

### Strategy 10: Data-Driven Naming
**Effort**: High  
**Expected Gain**: 2-4% coverage  
**Approach**: 
- Find string references near function addresses
- Analyze nearby data structures
- Use context from surrounding code

---

## Implementation Priority

### Phase 1: Quick Wins (Expected: +10-15% coverage)
1. ✅ Add 42 missing class names
2. ✅ Relax propagation thresholds
3. ✅ Add cross-class utility detection
4. ✅ Improve name collision handling

### Phase 2: Medium Effort (Expected: +8-12% coverage)
5. Multi-hop propagation (2-3 hops)
6. Pattern-based naming (getters/setters/loaders)
7. Jump table analysis
8. Prologue/epilogue analysis

### Phase 3: Advanced Techniques (Expected: +5-8% coverage)
9. Indirect call analysis (mtctr/bctrl)
10. Data-driven naming
11. Machine learning classification (if needed)

---

## Expected Final Coverage

- **Phase 1**: 68.2% → 78-83%
- **Phase 2**: 78-83% → 86-95%
- **Phase 3**: 86-95% → 93-100%

**Realistic target**: 90-95% coverage with automated techniques  
**Remaining 5-10%**: May require manual analysis or are dead code/data

---

## Next Steps

1. **Implement v6** with Phase 1 improvements
2. **Run v6** and measure actual gains
3. **Analyze v6 results** to validate assumptions
4. **Implement Phase 2** based on v6 findings
5. **Iterate** until target coverage reached

---

## Additional Observations

### Top Function Prefixes (Current State)
1. **game**: 6,550 functions (generic, needs better classification)
2. **rage**: 720 functions (RAGE engine core)
3. **jumptable**: 358 functions (switch statement targets)
4. **grc**: 120 functions (graphics)
5. **ref**: 98 functions (reference counting?)
6. **xam**: 83 functions (Xbox API)
7. **aud**: 56 functions (audio)

The large number of "game" prefixed functions suggests many could be reclassified with better class detection.

### Enum/State Patterns Found
- **flagList** - flag management
- Various state machine patterns (need deeper analysis)

### Function Patterns in Strings
- Match/game state management
- Network message handling
- Training drill systems
- Cutscene/camera systems

---

## Conclusion

Reaching 90-95% coverage is achievable with the strategies outlined above. The key is:
1. **Expand class knowledge** (42 missing classes)
2. **Improve propagation** (lower thresholds, multi-hop)
3. **Use multiple naming strategies** (patterns, utilities, indirect calls)
4. **Iterate and refine** based on results

The remaining 5-10% will likely be:
- Dead code
- Data misinterpreted as code
- Extremely isolated functions
- Functions requiring manual analysis
