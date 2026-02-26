#!/usr/bin/env python3
"""
Calculate decompilation rate needed to complete all classes within a target timeframe.
"""

import json
import sys
from pathlib import Path

def main():
    # Load RTTI vtable map to get total classes
    rtti_path = Path('/Users/Ozordi/Downloads/tt-decomp/config/434C4803/rtti_vtable_map.json')
    
    if not rtti_path.exists():
        print(f"Error: {rtti_path} not found")
        sys.exit(1)
    
    with open(rtti_path, 'r') as f:
        rtti_data = json.load(f)
    
    total_classes = len(rtti_data)
    
    # Current progress (from previous analysis)
    classes_decompiled = 14
    
    # Calculate remaining
    classes_remaining = total_classes - classes_decompiled
    
    # Time targets
    days_in_week = 7
    hours_per_day_light = 4  # Light work schedule
    hours_per_day_moderate = 6  # Moderate work schedule
    hours_per_day_intense = 8  # Intense work schedule
    
    print("=" * 80)
    print("ROCKSTAR TABLE TENNIS - DECOMPILATION RATE CALCULATOR")
    print("=" * 80)
    print()
    
    print(f"Total Classes (from RTTI): {total_classes}")
    print(f"Classes Decompiled: {classes_decompiled}")
    print(f"Classes Remaining: {classes_remaining}")
    print(f"Progress: {(classes_decompiled / total_classes * 100):.2f}%")
    print()
    
    print("=" * 80)
    print("TARGET: Complete in 1 WEEK (7 days)")
    print("=" * 80)
    print()
    
    # Calculate rates
    classes_per_day = classes_remaining / days_in_week
    
    print(f"Classes per day needed: {classes_per_day:.1f}")
    print()
    
    print("BREAKDOWN BY WORK SCHEDULE:")
    print("-" * 80)
    
    # Light schedule
    total_hours_light = days_in_week * hours_per_day_light
    classes_per_hour_light = classes_remaining / total_hours_light
    minutes_per_class_light = 60 / classes_per_hour_light
    
    print(f"\nLight Schedule ({hours_per_day_light} hours/day):")
    print(f"  Total work hours: {total_hours_light} hours")
    print(f"  Classes per hour: {classes_per_hour_light:.2f}")
    print(f"  Minutes per class: {minutes_per_class_light:.1f} minutes")
    
    # Moderate schedule
    total_hours_moderate = days_in_week * hours_per_day_moderate
    classes_per_hour_moderate = classes_remaining / total_hours_moderate
    minutes_per_class_moderate = 60 / classes_per_hour_moderate
    
    print(f"\nModerate Schedule ({hours_per_day_moderate} hours/day):")
    print(f"  Total work hours: {total_hours_moderate} hours")
    print(f"  Classes per hour: {classes_per_hour_moderate:.2f}")
    print(f"  Minutes per class: {minutes_per_class_moderate:.1f} minutes")
    
    # Intense schedule
    total_hours_intense = days_in_week * hours_per_day_intense
    classes_per_hour_intense = classes_remaining / total_hours_intense
    minutes_per_class_intense = 60 / classes_per_hour_intense
    
    print(f"\nIntense Schedule ({hours_per_day_intense} hours/day):")
    print(f"  Total work hours: {total_hours_intense} hours")
    print(f"  Classes per hour: {classes_per_hour_intense:.2f}")
    print(f"  Minutes per class: {minutes_per_class_intense:.1f} minutes")
    
    print()
    print("=" * 80)
    print("ALTERNATIVE TIMEFRAMES")
    print("=" * 80)
    print()
    
    # Calculate for different timeframes
    timeframes = [14, 21, 30, 60, 90]
    
    for days in timeframes:
        classes_per_day_alt = classes_remaining / days
        hours_needed_moderate = classes_remaining / (classes_per_hour_moderate)
        days_at_6h = hours_needed_moderate / 6
        
        print(f"{days} days ({days//7} weeks):")
        print(f"  Classes per day: {classes_per_day_alt:.1f}")
        print(f"  At 6 hours/day: {classes_per_day_alt / 6 * 60:.1f} minutes per class")
        print()
    
    print("=" * 80)
    print("REALITY CHECK")
    print("=" * 80)
    print()
    
    print("Class complexity varies significantly:")
    print("  - Simple classes (getters/setters): 15-30 minutes")
    print("  - Medium classes (game logic): 1-3 hours")
    print("  - Complex classes (rendering/physics): 4-8 hours")
    print()
    
    # Estimate based on complexity distribution
    simple_pct = 0.30
    medium_pct = 0.50
    complex_pct = 0.20
    
    simple_count = int(classes_remaining * simple_pct)
    medium_count = int(classes_remaining * medium_pct)
    complex_count = int(classes_remaining * complex_pct)
    
    simple_hours = simple_count * 0.5  # 30 min avg
    medium_hours = medium_count * 2    # 2 hours avg
    complex_hours = complex_count * 6  # 6 hours avg
    
    total_estimated_hours = simple_hours + medium_hours + complex_hours
    
    print(f"Estimated complexity distribution:")
    print(f"  Simple ({simple_pct*100:.0f}%): {simple_count} classes × 30 min = {simple_hours:.0f} hours")
    print(f"  Medium ({medium_pct*100:.0f}%): {medium_count} classes × 2 hours = {medium_hours:.0f} hours")
    print(f"  Complex ({complex_pct*100:.0f}%): {complex_count} classes × 6 hours = {complex_hours:.0f} hours")
    print()
    print(f"Total estimated hours: {total_estimated_hours:.0f} hours")
    print(f"At 6 hours/day: {total_estimated_hours / 6:.0f} days ({total_estimated_hours / 6 / 7:.1f} weeks)")
    print(f"At 8 hours/day: {total_estimated_hours / 8:.0f} days ({total_estimated_hours / 8 / 7:.1f} weeks)")
    print()
    
    print("=" * 80)
    print("RECOMMENDATION")
    print("=" * 80)
    print()
    
    realistic_weeks = total_estimated_hours / (6 * 7)
    
    if realistic_weeks <= 1:
        print("✓ 1 week is ACHIEVABLE with intense focus (8+ hours/day)")
    elif realistic_weeks <= 2:
        print("⚠ 1 week is AGGRESSIVE. Consider 2 weeks for sustainable pace.")
    else:
        print(f"⚠ 1 week is UNREALISTIC. Realistic target: {realistic_weeks:.0f} weeks")
    
    print()
    print(f"Suggested approach:")
    print(f"  - Start with simple classes to build momentum")
    print(f"  - Batch similar classes together")
    print(f"  - Use automation tools where possible")
    print(f"  - Focus on critical path classes first (menu boot)")
    print()

if __name__ == '__main__':
    main()
