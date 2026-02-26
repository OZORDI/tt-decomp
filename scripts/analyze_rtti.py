#!/usr/bin/env python3
"""
Analyze RTTI vtable map to understand what classes are actually present.
"""

import json
from pathlib import Path
from collections import defaultdict

def main():
    rtti_path = Path('/Users/Ozordi/Downloads/tt-decomp/config/434C4803/rtti_vtable_map.json')
    
    with open(rtti_path, 'r') as f:
        rtti_data = json.load(f)
    
    print("=" * 80)
    print("RTTI VTABLE MAP ANALYSIS")
    print("=" * 80)
    print()
    
    print(f"Total entries in RTTI map: {len(rtti_data)}")
    print()
    
    # Sample some entries to understand structure
    print("Sample entries (first 20):")
    print("-" * 80)
    for i, (key, value) in enumerate(list(rtti_data.items())[:20]):
        print(f"{key}: {value}")
    
    print()
    print("=" * 80)
    print("ENTRY ANALYSIS")
    print("=" * 80)
    print()
    
    # Analyze what these entries represent
    class_names = set()
    vtable_addresses = set()
    
    for key, value in rtti_data.items():
        # Key might be class name or address
        if key.startswith('0x') or key.startswith('8'):
            vtable_addresses.add(key)
        else:
            class_names.add(key)
        
        # Value analysis
        if isinstance(value, dict):
            if 'class_name' in value:
                class_names.add(value['class_name'])
            if 'vtable' in value:
                vtable_addresses.add(value['vtable'])
    
    print(f"Unique class names found: {len(class_names)}")
    print(f"Unique vtable addresses: {len(vtable_addresses)}")
    print()
    
    # Check if entries are class names or vtable addresses
    keys_are_addresses = sum(1 for k in rtti_data.keys() if k.startswith('0x') or (k.startswith('8') and len(k) == 10))
    keys_are_names = len(rtti_data) - keys_are_addresses
    
    print(f"Keys that look like addresses: {keys_are_addresses}")
    print(f"Keys that look like names: {keys_are_names}")
    print()
    
    # Categorize by prefix
    prefixes = defaultdict(int)
    for key in rtti_data.keys():
        if not (key.startswith('0x') or (key.startswith('8') and len(key) == 10)):
            # Extract prefix (first word before capital letter or underscore)
            if '::' in key:
                prefix = key.split('::')[0]
            elif key[0].islower():
                # Find where camelCase starts
                for i, c in enumerate(key[1:], 1):
                    if c.isupper():
                        prefix = key[:i]
                        break
                else:
                    prefix = key
            else:
                prefix = key.split('_')[0] if '_' in key else key[:4]
            
            prefixes[prefix] += 1
    
    print("Top 30 class prefixes:")
    print("-" * 80)
    for prefix, count in sorted(prefixes.items(), key=lambda x: x[1], reverse=True)[:30]:
        print(f"  {prefix}: {count} classes")
    
    print()
    print("=" * 80)
    print("REALITY CHECK")
    print("=" * 80)
    print()
    
    print("RTTI (Run-Time Type Information) typically includes:")
    print("  ✓ Classes with virtual functions (vtables)")
    print("  ✓ Classes used in dynamic_cast or typeid")
    print("  ✓ Polymorphic base classes")
    print()
    print("RTTI does NOT include:")
    print("  ✗ POD structs (Plain Old Data)")
    print("  ✗ Classes with no virtual functions")
    print("  ✗ Template instantiations (sometimes)")
    print("  ✗ Nested classes (sometimes)")
    print()
    
    # Estimate actual class count
    print("For a game of this size:")
    print("  - RTTI classes (polymorphic): ~1,700 (what we see)")
    print("  - Non-polymorphic classes: ~500-1,000 (estimated)")
    print("  - POD structs: ~1,000-2,000 (estimated)")
    print("  - Total types: ~3,200-4,700")
    print()
    print("However, for decompilation purposes:")
    print("  - Focus on RTTI classes first (they're the complex ones)")
    print("  - POD structs are often discovered during function analysis")
    print("  - Many structs are just data containers (quick to define)")
    print()

if __name__ == '__main__':
    main()
