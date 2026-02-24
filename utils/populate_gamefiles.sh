#!/bin/bash
# Populate gamefiles structure to match re3 style

set -e

echo "=== Populating gamefiles structure ==="
echo ""

# Move assets/objdiff.png to docs/research/
echo "Moving project assets to docs..."
if [ -f "assets/objdiff.png" ]; then
    mv assets/objdiff.png docs/research/objdiff.png
    echo "✓ Moved objdiff.png to docs/research/"
fi

# Remove empty assets directory
if [ -d "assets" ]; then
    rmdir assets 2>/dev/null && echo "✓ Removed empty assets/ directory" || echo "⚠ assets/ not empty or already removed"
fi
echo ""

# Create gamefiles subdirectories matching re3
echo "Creating gamefiles subdirectories..."
mkdir -p gamefiles/TEXT
mkdir -p gamefiles/data
mkdir -p gamefiles/models
echo "✓ Subdirectories created"
echo ""

# Create placeholder files explaining what goes where
echo "Creating placeholder documentation..."

cat > gamefiles/TEXT/README.md << 'EOF'
# TEXT Directory

This directory contains localized text files in GXT format.

## Expected Files

Extract from your copy of Rockstar Presents Table Tennis:
- `american.gxt` - American English text
- `english.gxt` - British English text
- `french.gxt` - French text
- `german.gxt` - German text
- `italian.gxt` - Italian text
- `spanish.gxt` - Spanish text
- `japanese.gxt` - Japanese text (if available)

## GXT Format

GXT files contain game text strings indexed by key. The format is specific to Rockstar games and varies between RAGE engine versions.

## Extraction

Use tools from `utils/` to extract GXT files from the original game's XEX or data files.
EOF

cat > gamefiles/data/README.md << 'EOF'
# Data Directory

This directory contains game data files.

## Expected Files

Extract from your copy of Rockstar Presents Table Tennis:
- Configuration files (`.cfg`, `.dat`)
- Script files (if any)
- Physics parameters
- Game rules and settings

## Custom Files

You can place modified data files here to customize game behavior:
- Modified physics parameters
- Custom game modes
- Tweaked AI settings

## Format

Data files are typically plain text or binary formats specific to the RAGE engine.
EOF

cat > gamefiles/models/README.md << 'EOF'
# Models Directory

This directory contains texture files and 3D model data.

## Expected Files

Extract from your copy of Rockstar Presents Table Tennis:
- Texture dictionaries (`.txd` files)
- Model files
- Font textures
- UI elements

## Custom Textures

Like re3, you can place custom texture files here to enhance the game:
- HD texture packs
- Custom fonts
- Modified UI elements
- Controller button prompts

## Format

Textures use Rockstar's TXD format (Texture Dictionary), which can be edited with tools like Magic.TXD or SparkIV.
EOF

echo "✓ Documentation created"
echo ""

# Create a gamefiles/gamecontrollerdb.txt like re3
echo "Creating gamecontrollerdb.txt..."
cat > gamefiles/gamecontrollerdb.txt << 'EOF'
# Game Controller DB
# SDL2 game controller mappings for various controllers
# This file is used by SDL2 to map controller inputs

# Xbox 360 Controller
030000005e0400008e02000014010000,Xbox 360 Controller,platform:Mac OS X,a:b0,b:b1,x:b2,y:b3,back:b9,guide:b10,start:b8,leftstick:b6,rightstick:b7,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:a4,righttrigger:a5,

# Xbox One Controller
030000005e040000d102000001010000,Xbox One Controller,platform:Mac OS X,a:b0,b:b1,x:b2,y:b3,back:b9,guide:b10,start:b8,leftstick:b6,rightstick:b7,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:a4,righttrigger:a5,

# PS4 Controller
030000004c050000c405000000010000,PS4 Controller,platform:Mac OS X,a:b1,b:b2,x:b0,y:b3,back:b8,guide:b12,start:b9,leftstick:b10,rightstick:b11,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a2,righty:a5,lefttrigger:a3,righttrigger:a4,

# Add more controller mappings as needed
# Format: GUID,Name,platform:Platform,mappings...
EOF
echo "✓ gamecontrollerdb.txt created"
echo ""

echo "=== Gamefiles structure complete ==="
echo ""
echo "Structure:"
tree gamefiles 2>/dev/null || find gamefiles -type f -o -type d | sort
echo ""
echo "Note: gamefiles/ is now ready for game assets."
echo "Users should extract original game files here to run the compiled game."
