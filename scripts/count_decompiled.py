#!/usr/bin/env python3
"""
Count decompiled functions and classes in the tt-decomp project.
Analyzes C/C++ source files in src/ directory.
"""

import os
import re
from pathlib import Path
from collections import defaultdict

def analyze_file(filepath):
    """Analyze a single C/C++ file for functions and classes."""
    functions = []
    classes = set()
    
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except Exception as e:
        print(f"Error reading {filepath}: {e}")
        return functions, classes
    
    # Remove comments to avoid false matches
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
    content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
    
    # Find class definitions
    class_pattern = r'\b(?:class|struct)\s+(\w+)\s*(?::\s*public\s+\w+\s*)?{'
    for match in re.finditer(class_pattern, content):
        classes.add(match.group(1))
    
    # Find function definitions (not declarations)
    # Match: return_type function_name(params) {
    func_pattern = r'\b(\w+(?:::\w+)?)\s*\([^)]*\)\s*{'
    
    # Also match with return types
    func_with_return = r'\b(?:void|int|float|double|bool|char|uint\w*|int\w*|size_t|\w+\*?)\s+(\w+(?:::\w+)?)\s*\([^)]*\)\s*{'
    
    for match in re.finditer(func_with_return, content):
        func_name = match.group(1)
        # Skip constructors/destructors that are just class names
        if '::' in func_name or not any(func_name == cls for cls in classes):
            functions.append(func_name)
    
    return functions, classes

def main():
    src_dir = Path('/Users/Ozordi/Downloads/tt-decomp/src')
    
    if not src_dir.exists():
        print(f"Error: {src_dir} does not exist")
        return
    
    total_functions = []
    total_classes = set()
    files_by_subsystem = defaultdict(lambda: {'functions': [], 'classes': set(), 'files': []})
    
    # Walk through all C/C++ files
    for root, dirs, files in os.walk(src_dir):
        for filename in files:
            if filename.endswith(('.c', '.cpp', '.cc', '.cxx')):
                filepath = Path(root) / filename
                rel_path = filepath.relative_to(src_dir)
                subsystem = rel_path.parts[0] if len(rel_path.parts) > 1 else 'root'
                
                functions, classes = analyze_file(filepath)
                
                total_functions.extend(functions)
                total_classes.update(classes)
                
                files_by_subsystem[subsystem]['functions'].extend(functions)
                files_by_subsystem[subsystem]['classes'].update(classes)
                files_by_subsystem[subsystem]['files'].append(str(rel_path))
    
    # Print results
    print("=" * 80)
    print("ROCKSTAR TABLE TENNIS DECOMPILATION PROGRESS")
    print("=" * 80)
    print()
    
    print(f"Total Functions Decompiled: {len(total_functions)}")
    print(f"Total Classes Defined: {len(total_classes)}")
    print()
    
    print("=" * 80)
    print("BREAKDOWN BY SUBSYSTEM")
    print("=" * 80)
    print()
    
    for subsystem in sorted(files_by_subsystem.keys()):
        data = files_by_subsystem[subsystem]
        print(f"{subsystem.upper()}/")
        print(f"  Files: {len(data['files'])}")
        print(f"  Functions: {len(data['functions'])}")
        print(f"  Classes: {len(data['classes'])}")
        if data['classes']:
            print(f"  Class Names: {', '.join(sorted(data['classes']))}")
        print()
    
    print("=" * 80)
    print("DETAILED FUNCTION LIST")
    print("=" * 80)
    print()
    
    for subsystem in sorted(files_by_subsystem.keys()):
        data = files_by_subsystem[subsystem]
        if data['functions']:
            print(f"\n{subsystem.upper()}/ ({len(data['functions'])} functions):")
            for func in sorted(set(data['functions'])):
                print(f"  - {func}")
    
    print()
    print("=" * 80)
    print(f"SUMMARY: {len(total_functions)} functions, {len(total_classes)} classes")
    print("=" * 80)

if __name__ == '__main__':
    main()
