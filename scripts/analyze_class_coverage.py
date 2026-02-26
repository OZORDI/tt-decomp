#!/usr/bin/env python3
"""
Class Coverage Analyzer
Analyzes what percentage of RTTI classes have been implemented in the codebase.
"""

import json
import os
import re
from pathlib import Path
from collections import defaultdict

# Paths
PROJECT_ROOT = Path("/Users/Ozordi/Downloads/tt-decomp")
RTTI_VTABLE_MAP = PROJECT_ROOT / "config/434C4803/rtti_vtable_map.json"
SRC_DIR = PROJECT_ROOT / "src"
INCLUDE_DIR = PROJECT_ROOT / "include"

def load_rtti_classes():
    """Load all class names from RTTI vtable map."""
    with open(RTTI_VTABLE_MAP) as f:
        vtable_map = json.load(f)
    
    # Extract unique class names
    classes = set()
    for addr, class_name in vtable_map.items():
        clean_name = class_name.split('<')[0].strip()
        classes.add(clean_name)
    
    return sorted(classes)

def find_class_in_codebase(class_name, search_dirs):
    """Search for class implementation in source/header files."""
    found_in = []
    
    # Patterns to search for
    patterns = [
        rf'\bclass\s+{re.escape(class_name)}\b',
        rf'\bstruct\s+{re.escape(class_name)}\b',
        rf'\b{re.escape(class_name)}\s*{{',
    ]
    
    for search_dir in search_dirs:
        if not search_dir.exists():
            continue
            
        for file_path in search_dir.rglob('*'):
            if file_path.suffix not in ['.cpp', '.c', '.hpp', '.h']:
                continue
            
            try:
                content = file_path.read_text(encoding='utf-8', errors='ignore')
                
                for pattern in patterns:
                    if re.search(pattern, content):
                        rel_path = file_path.relative_to(PROJECT_ROOT)
                        found_in.append(str(rel_path))
                        break
                        
            except Exception:
                continue
    
    return len(found_in) > 0, found_in

def categorize_class(class_name):
    """Categorize class by namespace/prefix."""
    if '::' in class_name:
        return class_name.split('::')[0]
    elif class_name.startswith('pong'):
        return 'pong'
    elif class_name.startswith('gm'):
        return 'game'
    elif class_name.startswith('gd'):
        return 'gamedata'
    elif class_name.startswith('ph'):
        return 'physics'
    elif class_name.startswith('fx'):
        return 'effects'
    elif class_name.startswith('hud'):
        return 'ui'
    else:
        return 'other'

def main():
    print("=" * 80)
    print("CLASS COVERAGE ANALYSIS")
    print("=" * 80)
    print()
    
    all_classes = load_rtti_classes()
    print(f"Total unique classes in RTTI: {len(all_classes)}")
    print()
    
    search_dirs = [SRC_DIR, INCLUDE_DIR]
    
    implemented = []
    not_implemented = []
    by_category = defaultdict(lambda: {'total': 0, 'implemented': 0})
    
    for i, class_name in enumerate(all_classes, 1):
        if i % 50 == 0:
            print(f"  Progress: {i}/{len(all_classes)} classes analyzed...")
        
        found, files = find_class_in_codebase(class_name, search_dirs)
        category = categorize_class(class_name)
        
        by_category[category]['total'] += 1
        
        if found:
            implemented.append((class_name, files))
            by_category[category]['implemented'] += 1
        else:
            not_implemented.append(class_name)
    
    print()
    print("=" * 80)
    print("RESULTS")
    print("=" * 80)
    print()
    
    total = len(all_classes)
    impl_count = len(implemented)
    percentage = (impl_count / total * 100) if total > 0 else 0
    
    print(f"Total Classes:       {total}")
    print(f"Implemented:         {impl_count} ({percentage:.2f}%)")
    print(f"Not Implemented:     {len(not_implemented)} ({100-percentage:.2f}%)")
    print()
    
    print("=" * 80)
    print("BREAKDOWN BY CATEGORY")
    print("=" * 80)
    print()
    print(f"{'Category':<20} {'Total':<8} {'Impl':<8} {'%':<8}")
    print("-" * 80)
    
    for category in sorted(by_category.keys()):
        stats = by_category[category]
        cat_total = stats['total']
        cat_impl = stats['implemented']
        cat_pct = (cat_impl / cat_total * 100) if cat_total > 0 else 0
        print(f"{category:<20} {cat_total:<8} {cat_impl:<8} {cat_pct:>6.2f}%")
    
    print()
    print(f"Report complete!")

if __name__ == '__main__':
    main()
