# Table Tennis Decompilation Plan

This plan outlines the systematic reconstruction of Rockstar Presents Table Tennis from Hex-Rays pseudocode to cross-platform C code. The project contains 16,331 functions that need to be converted from Xbox 360-specific code to portable C.

## Phase 1: Infrastructure Setup
1. **Create proper project structure** - Organize source files by module (crt, rage, game, graphics, etc.)
2. **Set up cross-platform compatibility layer** - Abstract Xbox 360 XDK calls to SDL2/OpenGL equivalents
3. **Establish type system** - Define proper data types replacing Xbox 360-specific types
4. **Create build system** - Configure CMake for cross-platform compilation

## Phase 2: Core Runtime (CRT) Reconstruction
1. **Convert CRT functions** - Reimplement C runtime functions (malloc, free, strlen, etc.)
2. **Memory management** - Replace Xbox 360 memory allocation with standard C equivalents
3. **Thread/Process abstraction** - Convert Xbox 360 threading to pthread/SDL2 threads
4. **File I/O abstraction** - Replace Xbox 360 file system calls with standard C FILE*

## Phase 3: Graphics Pipeline Reconstruction
1. **Xenos GPU abstraction** - Replace Xbox 360 GPU calls with OpenGL equivalents
2. **Render pipeline** - Convert grcDevice calls to OpenGL/SDL2
3. **Present/swap chain** - Implement VdSwap equivalent using SDL_GL_SwapWindow
4. **Command buffer removal** - Eliminate GPU ring buffer, use OpenGL command queue

## Phase 4: Game Logic Conversion
1. **Main game loop** - Convert main() and update/render cycles
2. **Game object systems** - Reimplement gameplay classes and vtables
3. **Physics and animation** - Convert game-specific systems
4. **Input handling** - Replace Xbox 360 input with SDL2 input

## Phase 5: Audio and Systems
1. **XAudio2 abstraction** - Replace with SDL2_mixer or OpenAL
2. **Network code** - Convert Xbox Live calls to standard sockets
3. **Save system** - Replace Xbox 360 storage with standard file I/O

## Conversion Strategy
- **Function-by-function approach** - Process functions in dependency order
- **Type normalization** - Convert __int64, _DWORD, etc. to standard C types
- **Register calling convention** - Remove __fastcall, use standard C calling
- **Memory layout preservation** - Maintain struct layouts for compatibility
- **Hardware abstraction** - Replace MMIO, DMA, interrupts with software equivalents

## Cross-Platform Considerations
- **Endianness** - Handle PowerPC little-endian to host conversion
- **Alignment** - Maintain 16-byte alignment where required
- **Vector operations** - Convert AltiVec to standard SIMD intrinsics
- **System calls** - Replace Xbox 360 kernel calls with POSIX equivalents

## Testing Strategy
1. **Unit tests** - Test individual functions against expected behavior
2. **Integration tests** - Verify module interactions
3. **Cross-platform builds** - Test on Windows, Linux, macOS
4. **Behavioral verification** - Compare game behavior with original

## Estimated Timeline
- **Phase 1**: 1-2 days (infrastructure)
- **Phase 2**: 3-4 days (CRT/core systems)  
- **Phase 3**: 5-7 days (graphics pipeline)
- **Phase 4**: 8-12 days (game logic)
- **Phase 5**: 2-3 days (audio/systems)
- **Total**: ~3 weeks for full reconstruction

## Success Criteria
- Clean compilation on Windows, Linux, macOS
- Functional game loop with rendering
- Input handling and gameplay mechanics working
- Audio and save systems operational
- Performance comparable to original target
