/**
 * gd_cutscene_strings.cpp — Original binary string constant definitions
 * Rockstar Presents Table Tennis (Xbox 360, 2006)
 *
 * All string values verified byte-for-byte from orig/434C4803/default_base.bin.
 */

#include "game/gd_cutscene_strings.hpp"

// gdCSCharAnimData
const char* g_str_gdCSCharAnimData_missingFileName    = "gdCSCharAnimData::PostLoadProperties() - missing property 'FileName'";
const char* g_str_gdCSCharAnimData_negTimeOffset      = "gdCSCharAnimData::PostLoadProperties() - 'TimeOffset' cannot be negative";
const char* g_str_gdCSCharAnimData_negWeight          = "gdCSCharAnimData::PostLoadProperties() - cannot have negative weight";
const char* g_str_gdCSCharAnimData_invalidEmote       = "gdCSCharAnimData::PostLoadProperties() - invalid emote range";

// gdCSCharAnimNames
const char* g_str_gdCSCharAnimNames_noAnimData        = "gdCSCharAnimNames::FindRandAnimData() - no animation data at all";
const char* g_str_gdCSCharAnimData_boneName           = "BoneName";

// gdCSCamAnimShotName
const char* g_str_gdCSCamAnimShotName_negWeight       = "gdCSCamAnimShotName::PostLoadProperties() - cannot have negative weight";
const char* g_str_gdCSCamAnimShotName_missingShotName = "gdCSCamAnimShotName::PostLoadProperties() - missing property 'ShotName'";
const char* g_str_gdCSCamAnimShotName_negTimeOffset   = "gdCSCamAnimShotName::PostLoadProperties() - 'TimeOffset' cannot be negative";
const char* g_str_gdCSCamAnimShotName_shotNotFound    = "gdCSCamAnimShotName::PostLoadProperties() - shot name '%s' does not exist in set '%s'";

// gdCSNameData / gdCutSceneData
const char* g_str_gdCSNameData_missingFileName        = "gdCSNameData::PostLoadProperties() - missing property 'FileName'";
const char* g_str_gdCSNameData_unknownTrigger         = "gdCSNameData::PostLoadProperties() - unknown trigger message '%s'";
const char* g_str_gdCutSceneData_unknownSkip          = "gdCutSceneData::PostLoadProperties() - unknown skip message '%s'";

// gdCSActionIfData
const char* g_str_gdCSActionIfData_unknownCondition   = "gdCSActionIfData::PostLoadProperties() - unknown if-statement condition '%s'";
const char* g_str_gdCSActionIfData_needsPlayerId      = "gdCSActionIfData::PostLoadProperties() - condition '%s' requires property 'PlayerID'";
const char* g_str_gdCSActionIfData_needsPercent       = "gdCSActionIfData::PostLoadProperties() - condition '%s' requires property 'Percent'";
const char* g_str_gdCSActionIfData_percentExceeds     = "gdCSActionIfData::PostLoadProperties() - property 'Percent' cannot exceed %d";
const char* g_str_gdCSActionIfData_needsGameType      = "gdCSActionIfData::PostLoadProperties() - condition '%s' requires property 'GameType'";
const char* g_str_gdCSActionIfData_invalidGameType    = "gdCSActionIfData::PostLoadProperties() - property 'GameType' has invalid value '%s'";
const char* g_str_gdCSActionIfData_needsArenaType     = "gdCSActionIfData::PostLoadProperties() - condition '%s' requires property 'ArenaType'";
const char* g_str_gdCSActionIfData_invalidArenaType   = "gdCSActionIfData::PostLoadProperties() - property 'ArenaType' has invalid value '%s'";

// gdCSAction*Data
const char* g_str_gdCSActionCamAnimData_missingFileName         = "gdCSActionCamAnimData::PostLoadProperties() - missing property 'FileName'";
const char* g_str_gdCSActionCharAnimData_missingPlayerId        = "gdCSActionCharAnimData::PostLoadProperties() - missing property 'PlayerID'";
const char* g_str_gdCSActionCharAnimData_missingFileName        = "gdCSActionCharAnimData::PostLoadProperties() - missing property 'FileName'";
const char* g_str_gdCSActionCharVisibleData_missingPlayerId     = "gdCSActionCharVisibleData::PostLoadProperties() - missing property 'PlayerID'";
const char* g_str_gdCSActionPlayAudioData_missingAudioType      = "gdCSActionPlayAudioData::PostLoadProperties() - missing property 'AudioEventType'";
const char* g_str_gdCSActionPlayAudioData_missingAudioEvent     = "gdCSActionPlayAudioData::PostLoadProperties() - missing property 'AudioEvent'";
const char* g_str_gdCSActionPlayAudioData_unknownAudioType      = "gdCSActionPlayAudioData::PostLoadProperties() - unknown audio event type: '%s'";
const char* g_str_gdCSActionPlayAudioData_unknownAudioEventId   = "gdCSActionPlayAudioData::PostLoadProperties() - unknown audio event ID: '%s'";

// gdCSAction Ambient Data
const char* g_str_gdCSActionLvlAnimData_missingAnimName         = "gdCSActionLvlAnimData::PostLoadProperties() - missing property 'AnimName'";
const char* g_str_gdCSActionLvlAmbAnimData_missingAmbName       = "gdCSActionLvlAmbAnimData::PostLoadProperties() - missing property 'AmbName'";
const char* g_str_gdCSActionLvlAmbAnimData_missingAnimName      = "gdCSActionLvlAmbAnimData::PostLoadProperties() - missing property 'AnimName'";
const char* g_str_gdCSActionCharAmbAnimData_missingPlayerId     = "gdCSActionCharAmbAnimData::PostLoadProperties() - missing property 'PlayerID'";
const char* g_str_gdCSActionCharAmbAnimData_missingAmbName      = "gdCSActionCharAmbAnimData::PostLoadProperties() - missing property 'AmbName'";
const char* g_str_gdCSActionCharAmbAnimData_missingAnimName     = "gdCSActionCharAmbAnimData::PostLoadProperties() - missing property 'AnimName'";
