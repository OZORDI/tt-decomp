/**
 * Ladder/Rivalry System Classes
 * Rockstar Presents Table Tennis (Xbox 360)
 * 
 * Game data classes for tournament ladder, tier, and rivalry systems.
 * Manages competitive progression and character matchups.
 */

#pragma once

#include <stdint.h>

// Forward declarations
class xmlNodeStruct;
class gdCharData;

/**
 * gdRivalry
 * @ RTTI: 0x820325AC
 * 
 * Represents a rivalry relationship between two characters.
 * Used for special matchups and story progression.
 */
class gdRivalry : public xmlNodeStruct {
public:
    virtual ~gdRivalry() {}                              // vfn_0
    virtual void PostLoadProperties() {}                 // vfn_20
    virtual void PostLoadChildren();                     // vfn_22

private:
    // Rivalry data fields (to be determined from usage)
    uint8_t m_rivalryData[32];
};

/**
 * gdTierMember
 * @ RTTI: 0x82042E5C
 * 
 * Represents a character member within a tournament tier.
 * Links character data to tier position.
 */
class gdTierMember : public xmlNodeStruct {
public:
    virtual void PostLoadProperties();                   // vfn_20 @ 0x821F0C88
    virtual void Validate() {}                           // vfn_21
    virtual void PostLoadChildren() {}                   // vfn_22

private:
    const char* m_characterName;         // +0x10 - Character name string
    gdCharData* m_pCharData;             // +0x14 - Pointer to character data
};

/**
 * gdTier
 * @ RTTI: 0x82032614
 * 
 * Represents a tier/bracket in the tournament ladder.
 * Contains multiple tier members (characters at this level).
 */
class gdTier : public xmlNodeStruct {
public:
    virtual ~gdTier() {}                                 // vfn_0
    virtual void PostLoadProperties();                   // vfn_20 @ 0x821F0EA0
    virtual void Validate() {}                           // vfn_21
    virtual void PostLoadChildren();                     // vfn_22

private:
    const char* m_tierName;              // +0x10 - Tier name string
    uint32_t m_tierLevel;                // +0x14 - Tier difficulty level
    gdTierMember* m_pMembers;            // +0x18 - Array of tier members
    uint32_t m_numMembers;               // +0x1C - Number of members in tier
};

/**
 * gdLadder
 * @ RTTI: 0x8203267C
 * 
 * Main ladder/tournament structure.
 * Contains multiple tiers forming the competitive progression.
 */
class gdLadder : public xmlNodeStruct {
public:
    virtual ~gdLadder() {}                               // vfn_0
    virtual void PostLoadProperties() {}                 // vfn_20
    virtual void PostLoadChildren();                     // vfn_22

private:
    const char* m_ladderName;            // +0x10 - Ladder name
    gdTier* m_pTiers;                    // +0x14 - Array of tiers
    uint32_t m_numTiers;                 // +0x18 - Number of tiers
    uint32_t m_currentTier;              // +0x1C - Player's current tier index
};

// Compile-time size verification
static_assert(sizeof(gdTierMember) >= 24, "gdTierMember minimum size check");
static_assert(sizeof(gdTier) >= 32, "gdTier minimum size check");
static_assert(sizeof(gdLadder) >= 32, "gdLadder minimum size check");
