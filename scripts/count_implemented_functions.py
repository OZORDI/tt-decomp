#!/usr/bin/env python3
"""
Count implemented functions in tt-decomp by looking for address markers.
This script counts functions that have been lifted from assembly by finding
the @ 0xADDRESS markers in comments.
"""

import os
import re
from pathlib import Path
from collections import defaultdict

def count_functions_in_file(filepath):
    """Count functions with address markers in a file."""
    functions = []
    
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except Exception as e:
        return functions
    
    # Look for function definitions with @ 0xADDRESS markers
    # Pattern 1: In comments like "// @ 0x82345678" or "@ 0x82345678"
    # Pattern 2: Function headers with address documentation
    
    # Find all address markers
    address_pattern = r'@\s*0x([0-9A-Fa-f]{8})'
    addresses = re.findall(address_pattern, content)
    
    # Also look for function implementations (not just declarations)
    # Match function definitions that have bodies
    func_impl_pattern = r'(?:^|\n)\s*(?:[\w:*&<>,\s]+\s+)?(\w+(?:::\w+)?)\s*\([^)]*\)\s*(?:const\s*)?{'
    
    # Count unique addresses (each represents one lifted function)
    unique_addresses = set(addresses)
    
    # Try to extract function names near address markers
    lines = content.split('\n')
    for i, line in enumerate(lines):
        if '@' in line and '0x' in line:
            # Look for function name in nearby lines
            for j in range(max(0, i-5), min(len(lines), i+5)):
                func_match = re.search(r'(\w+(?:::\w+)?)\s*\([^)]*\)\s*(?:const\s*)?{', lines[j])
                if func_match:
                    func_name = func_match.group(1)
                    addr_match = re.search(r'0x([0-9A-Fa-f]{8})', line)
                    if addr_match:
                        functions.append({
                            'name': func_name,
                            'address': '0x' + addr_match.group(1),
                            'file': str(filepath)
                        })
                        break
    
    return functions

def main():
    src_dir = Path('/Users/Ozordi/Downloads/tt-decomp/src')
    
    if not src_dir.exists():
        print(f"Error: {src_dir} does not exist")
        return
    
    all_functions = []
    files_by_subsystem = defaultdict(list)
    
    # Walk through all C/C++ files
    for root, dirs, files in os.walk(src_dir):
        # Skip certain directories
        if 'stubs' in root or 'generated' in root:
            continue
            
        for filename in files:
            if filename.endswith(('.c', '.cpp', '.cc', '.cxx')):
                filepath = Path(root) / filename
                rel_path = filepath.relative_to(src_dir)
                subsystem = rel_path.parts[0] if len(rel_path.parts) > 1 else 'root'
                
                functions = count_functions_in_file(filepath)
                
                if functions:
                    all_functions.extend(functions)
                    files_by_subsystem[subsystem].extend(functions)
    
    # Remove duplicates based on address
    unique_functions = {}
    for func in all_functions:
        addr = func['address']
        if addr not in unique_functions:
            unique_functions[addr] = func
    
    # Print results
    print("=" * 80)
    print("ROCKSTAR TABLE TENNIS - IMPLEMENTED FUNCTIONS COUNT")
    print("=" * 80)
    print()
    
    total_in_binary = 16653  # From symbols.txt
    implemented = len(unique_functions)
    remaining = total_in_binary - implemented
    percentage = (implemented / total_in_binary) * 100
    
    print(f"Total functions in binary: {total_in_binary:,}")
    print(f"Functions implemented:     {implemented:,}")
    print(f"Functions remaining:       {remaining:,}")
    print(f"Progress:                  {percentage:.2f}%")
    print()
    
    print("=" * 80)
    print("BREAKDOWN BY SUBSYSTEM")
    print("=" * 80)
    print()
    
    for subsystem in sorted(files_by_subsystem.keys()):
        funcs = files_by_subsystem[subsystem]
        # Count unique addresses in this subsystem
        unique_addrs = len(set(f['address'] for f in funcs))
        print(f"{subsystem:20s}: {unique_addrs:4d} functions")
    
    print()
    print("=" * 80)
    print("SAMPLE OF IMPLEMENTED FUNCTIONS (first 20)")
    print("=" * 80)
    print()
    
    sorted_funcs = sorted(unique_functions.values(), key=lambda x: x['address'])
    for func in sorted_funcs[:20]:
        rel_file = Path(func['file']).relative_to(src_dir)
        print(f"{func['address']}  {func['name']:40s}  {rel_file}")
    
    if len(sorted_funcs) > 20:
        print(f"\n... and {len(sorted_funcs) - 20} more")
    
    print()
    print("=" * 80)
    print(f"TOTAL: {implemented:,} / {total_in_binary:,} functions ({percentage:.2f}%)")
    print("=" * 80)

if __name__ == '__main__':
    main()
