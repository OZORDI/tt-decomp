/**
 * gd_data_strings.hpp — Original binary string constants for game data system
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All addresses verified from orig/434C4803/default_base.bin.
 */
#pragma once

// ────────────────────────────────────────────────────────────────────────────
// gdRivalryData
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdRivalryData_noCharName;        // @ 0x82044598
extern const char* g_str_gdRivalryData_noRivalName;       // @ 0x820445E0
extern const char* g_str_gdRivalryData_unknownRival;      // @ 0x82044620

// ────────────────────────────────────────────────────────────────────────────
// gdTierMember
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdTierMember_noCharName;         // @ 0x82044778
extern const char* g_str_gdTierMember_unknownChar;        // @ 0x820447BC

// ────────────────────────────────────────────────────────────────────────────
// gdTier
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdTier_noTierName;               // @ 0x8204480C

// ────────────────────────────────────────────────────────────────────────────
// gdLadderMatch
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdLadderMatch_noOpponent;        // @ 0x82044974
extern const char* g_str_gdLadderMatch_noArena;           // @ 0x820449B4
extern const char* g_str_gdLadderMatch_noTourneyLevel;    // @ 0x820449F0
extern const char* g_str_gdLadderMatch_noTourneyType;     // @ 0x82044A38
extern const char* g_str_gdLadderMatch_unknownOpponent;   // @ 0x82044A84
extern const char* g_str_gdLadderMatch_unknownArena;      // @ 0x82044AC8
extern const char* g_str_gdLadderMatch_unknownLevel;      // @ 0x82044B20
extern const char* g_str_gdLadderMatch_unknownType;       // @ 0x82044BA0
extern const char* g_str_gdLadderMatch_unknownCrowdSize;  // @ 0x82044BF0
extern const char* g_str_gdLadderMatch_unknownRowdiness;  // @ 0x82044C38

// ────────────────────────────────────────────────────────────────────────────
// gdDifficultyLadder
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdDifficultyLadder_invalidLevel; // @ 0x82044C98

// ────────────────────────────────────────────────────────────────────────────
// PostLoadChildren strings (verified from binary — many differ from source!)
// ────────────────────────────────────────────────────────────────────────────
extern const char* g_str_gdTier_unknownNodeType;              // @ 0x82044844
extern const char* g_str_gdLadder_duplicateLevel;             // @ 0x82044D30
extern const char* g_str_gdLadder_unknownNodeType;            // @ 0x82044D70
extern const char* g_str_gdLadder_missingLadder;              // @ 0x82044DA8
extern const char* g_str_gdRivalry_unknownChar;               // @ 0x82044670
extern const char* g_str_gdRivalry_duplicateChar;             // @ 0x820446A8
extern const char* g_str_gdRivalry_unknownNodeType;           // @ 0x820446F0
extern const char* g_str_gdRivalry_missingRival;              // @ 0x82044728
extern const char* g_str_gdDifficultyLadder_unknownNodeType;  // @ 0x82044CE0
