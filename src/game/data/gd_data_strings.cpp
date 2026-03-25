/**
 * gd_data_strings.cpp — Original binary string constant definitions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All string values verified byte-for-byte from orig/434C4803/default_base.bin.
 */

#include "game/gd_data_strings.hpp"

// gdRivalryData
const char* g_str_gdRivalryData_noCharName        = "gdRivalryData::PostLoadProperties() - 'CharacterName' not specified";
const char* g_str_gdRivalryData_noRivalName       = "gdRivalryData::PostLoadProperties() - 'RivalName' not specified";
const char* g_str_gdRivalryData_unknownRival      = "gdRivalryData::PostLoadProperties() - unknown rival character '%s'";

// gdTierMember
const char* g_str_gdTierMember_noCharName         = "gdTierMember::PostLoadProperties() - 'CharacterName' not specified";
const char* g_str_gdTierMember_unknownChar        = "gdTierMember::PostLoadProperties() - unknown character '%s'";

// gdTier
const char* g_str_gdTier_noTierName               = "gdTier::PostLoadProperties() - 'TierName' not specified";

// gdLadderMatch
const char* g_str_gdLadderMatch_noOpponent        = "gdLadderMatch::PostLoadProperties() - 'Opponent' not specified";
const char* g_str_gdLadderMatch_noArena           = "gdLadderMatch::PostLoadProperties() - 'Arena' not specified";
const char* g_str_gdLadderMatch_noTourneyLevel    = "gdLadderMatch::PostLoadProperties() - 'TourneyLevel' not specified";
const char* g_str_gdLadderMatch_noTourneyType     = "gdLadderMatch::PostLoadProperties() - 'TourneyType' not specified";
const char* g_str_gdLadderMatch_unknownOpponent   = "gdLadderMatch::PostLoadProperties() - unknown opponent '%s'";
const char* g_str_gdLadderMatch_unknownArena      = "gdLadderMatch::PostLoadProperties() - unknown arena '%s'";
const char* g_str_gdLadderMatch_unknownLevel      = "gdLadderMatch::PostLoadProperties() - unknown tourney level '%s'";
const char* g_str_gdLadderMatch_unknownType       = "gdLadderMatch::PostLoadProperties() - unknown tourney type '%s'";
const char* g_str_gdLadderMatch_unknownCrowdSize  = "gdLadderMatch::PostLoadProperties() - unknown crowd size '%s'";
const char* g_str_gdLadderMatch_unknownRowdiness  = "gdLadderMatch::PostLoadProperties() - unknown crowd rowdiness '%s'";

// gdDifficultyLadder
const char* g_str_gdDifficultyLadder_invalidLevel = "gdDifficultyLadder::PostLoadProperties() - invalid level value '%d'";

// PostLoadChildren strings (verified from binary — several differ from what was in source)
const char* g_str_gdTier_unknownNodeType              = "gdTier::PostLoadChildren() - unknown node type '%s'";
const char* g_str_gdLadder_duplicateLevel             = "gdLadder::PostLoadChildren() - duplicate entries for level '%d'";
const char* g_str_gdLadder_unknownNodeType            = "gdLadder::PostLoadChildren() - unknown node type '%s'";
const char* g_str_gdLadder_missingLadder              = "gdLadder::PostLoadChildren() - level '%d' does not have a ladder";
const char* g_str_gdRivalry_unknownChar               = "gdRivalry::PostLoadChildren() - unknown character '%s'";
const char* g_str_gdRivalry_duplicateChar             = "gdRivalry::PostLoadChildren() - duplicate entries for character '%s'";
const char* g_str_gdRivalry_unknownNodeType           = "gdRivalry::PostLoadChildren() - unknown node type '%s'";
const char* g_str_gdRivalry_missingRival              = "gdRivalry::PostLoadChildren() - '%s' does not have a rival";
const char* g_str_gdDifficultyLadder_unknownNodeType  = "gdDifficultyLadder::PostLoadChildren() - unknown node type '%s'";
