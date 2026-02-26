#!/usr/bin/env python3
"""
Analyze where symbols came from: original binary vs recovered
"""

import re

# Check symbols.txt for hints about source
with open('/Users/Ozordi/Downloads/tt-decomp/config/434C4803/symbols.txt', 'r') as f:
    symbols = f.readlines()

print("=" * 80)
print("SYMBOL SOURCE ANALYSIS")
print("=" * 80)
print()

# Analyze symbol naming patterns
named_symbols = 0
sub_functions = 0  # sub_XXXXXXXX pattern (auto-generated)
meaningful_names = 0
import_stubs = 0

sample_named = []
sample_subs = []

for line in symbols[:1000]:  # Sample first 1000
    line = line.strip()
    if not line or '=' not in line:
        continue
    
    name = line.split('=')[0].strip()
    
    if '__imp_' in name:
        import_stubs += 1
    elif re.match(r'sub_[0-9A-Fa-f]{8}', name):
        sub_functions += 1
        if len(sample_subs) < 5:
            sample_subs.append(name)
    elif re.match(r'FUN_[0-9A-Fa-f]{8}', name):
        sub_functions += 1
    else:
        meaningful_names += 1
        if len(sample_named) < 10:
            sample_named.append(name)

print("SAMPLE ANALYSIS (first 1000 entries):")
print(f"  Meaningful names: {meaningful_names}")
print(f"  Auto-generated (sub_/FUN_): {sub_functions}")
print(f"  Import stubs: {import_stubs}")
print()

print("Sample meaningful names:")
for name in sample_named[:10]:
    print(f"  - {name}")
print()

print("Sample auto-generated names:")
for name in sample_subs[:5]:
    print(f"  - {name}")
print()

# Check autonamed file
print("=" * 80)
print("CHECKING AUTO-NAMING HISTORY")
print("=" * 80)
print()

try:
    with open('/Users/Ozordi/Downloads/tt-decomp/config/434C4803/symbols_autonamed_v18.txt', 'r') as f:
        autonamed = f.readlines()
    
    print(f"symbols_autonamed_v18.txt exists with {len(autonamed)} entries")
    print()
    print("This suggests symbols were RECOVERED through analysis, not in original binary")
except:
    print("No autonamed file found")

print()
print("=" * 80)
print("CHECKING NAMING RESEARCH DOCS")
print("=" * 80)
print()

# Check if naming research docs exist
import os
naming_docs = []
docs_path = '/Users/Ozordi/Downloads/tt-decomp/docs/naming'
if os.path.exists(docs_path):
    for f in os.listdir(docs_path):
        if f.endswith('.md') or f.endswith('.py'):
            naming_docs.append(f)

if naming_docs:
    print(f"Found {len(naming_docs)} naming research files:")
    for doc in sorted(naming_docs)[:10]:
        print(f"  - {doc}")
    print()
    print("This confirms extensive symbol recovery work was done")
else:
    print("No naming research docs found")

print()
print("=" * 80)
print("CONCLUSION")
print("=" * 80)
print()

