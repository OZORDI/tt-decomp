#!/usr/bin/env python3
"""
gen_headers_v3.py — Exhaustive class-to-file mapper for tt-decomp
Covers ALL 1,266 RTTI classes from rtti_vtable_map.json including:
  - rage::sn* session networking, rage::Evt*, rage::swf*, rage::rl*, rage::hsm*
  - rage::mf*, rage::spd*, rage::frag*, rage::rmc*, rage::rmPtx*
  - rage::cm* computation graph (full operator library)
  - All fx*, sg*, hud*, plr*, gm*, ph* etc.
  - Complex template RTTI names (rage::X::, rage::12::, rage::2::)
  - atSafeDLListSimple, Gamer, NetworkClient, etc.
"""
import json, os, re
from collections import defaultdict

BASE = '/Users/Ozordi/Downloads/tt-decomp'
SRC  = f'{BASE}/src'
CFG  = f'{BASE}/config/434C4803'

with open(f'{CFG}/rtti_vtable_map.json')       as f: vtmap    = json.load(f)
with open(f'{CFG}/vtable_layout_map.json')      as f: vtlayout = json.load(f)
with open(f'{CFG}/offset_cluster_map.json')     as f: offsets  = json.load(f)
with open(f'{CFG}/debug_string_field_map.json') as f: dbgmap   = json.load(f)
with open(f'{CFG}/rtti_class_hierarchy.json')   as f: hier     = json.load(f)

all_names = sorted(set(vtmap.values()))

# ─────────────────────────────────────────────────────────────────────────────
# ROUTING TABLE  (most-specific first — first match wins)
# ─────────────────────────────────────────────────────────────────────────────
ROUTES = [

    # ── Already fully implemented — skip generation ───────────────────────────
    # (handled in SKIP set below)

    # ── rage:: session networking (sn*, Evt*, rl*, hsm*) ─────────────────────
    ('rage::snSession',          ('rage', 'rage_sn')),
    ('rage::snCreate',           ('rage', 'rage_sn')),
    ('rage::snJoin',             ('rage', 'rage_sn')),
    ('rage::snLeave',            ('rage', 'rage_sn')),
    ('rage::snMigrate',          ('rage', 'rage_sn')),
    ('rage::snArb',              ('rage', 'rage_sn')),
    ('rage::snNotify',           ('rage', 'rage_sn')),
    ('rage::snGamer',            ('rage', 'rage_sn')),
    ('rage::snConnection',       ('rage', 'rage_sn')),
    ('rage::snSessionManager',   ('rage', 'rage_sn')),
    ('rage::snSessionFinder',    ('rage', 'rage_sn')),
    ('rage::snSession::',        ('rage', 'rage_sn')),
    ('rage::snSessionFinder::',  ('rage', 'rage_sn')),
    ('rage::snCreateMachine::',  ('rage', 'rage_sn')),
    ('rage::snJoinMachine::',    ('rage', 'rage_sn')),
    ('rage::snLeaveMachine::',   ('rage', 'rage_sn')),
    ('rage::snMigrateMachine::', ('rage', 'rage_sn')),
    ('rage::sn',                 ('rage', 'rage_sn')),

    # rage::Evt* events
    ('rage::Evt',                ('rage', 'rage_sn')),

    # rage::rl* online services
    ('rage::rlGamer',            ('rage', 'rage_sn')),
    ('rage::rlSession',          ('rage', 'rage_sn')),
    ('rage::rlStatsView',        ('rage', 'rage_sn')),
    ('rage::rlGamer::',          ('rage', 'rage_sn')),
    ('rage::rlSession::',        ('rage', 'rage_sn')),
    ('rage::rl',                 ('rage', 'rage_sn')),
    # complex template RTTI names for online notify handlers
    ('rage::X::rage::Vrl',       ('rage', 'rage_sn')),
    ('rage::X::rage::VrlSession',('rage', 'rage_sn')),
    ('rage::12::',               ('rage', 'rage_sn')),
    ('rage::2::Vsn',             ('rage', 'rage_sn')),
    ('rage::X::',                ('rage', 'rage_sn')),

    # rage::hsm* hierarchical state machine
    ('rage::hsmContext',         ('rage', 'rage_sn')),
    ('rage::hsmStateBase',       ('rage', 'rage_sn')),
    ('rage::hsmEvent',           ('rage', 'rage_sn')),
    ('rage::hsmContext::',       ('rage', 'rage_sn')),
    ('rage::hsm',                ('rage', 'rage_sn')),

    # ── rage:: SWF/Flash ──────────────────────────────────────────────────────
    ('rage::swf',                ('rage', 'rage_swf')),

    # ── rage:: animation core ─────────────────────────────────────────────────
    ('rage::crAnimChannel',      ('rage', 'rage_anim')),
    ('rage::crAnimDof',          ('rage', 'rage_anim')),
    ('rage::crAnimFrame',        ('rage', 'rage_anim')),
    ('rage::crAnimPlayer',       ('rage', 'rage_anim')),
    ('rage::crAnimWeight',       ('rage', 'rage_anim')),
    ('rage::crAnimation',        ('rage', 'rage_anim')),
    ('rage::crAnim',             ('rage', 'rage_anim')),
    ('rage::crIK',               ('rage', 'rage_anim')),
    ('rage::cr',                 ('rage', 'rage_anim')),

    # ── rage:: motion framework (mf*) ────────────────────────────────────────
    ('rage::mfMotion',           ('rage', 'rage_mf')),
    ('rage::mfAnimation',        ('rage', 'rage_mf')),
    ('rage::mfHierarchy',        ('rage', 'rage_mf')),
    ('rage::mfNative',           ('rage', 'rage_mf')),
    ('rage::mfRegistered',       ('rage', 'rage_mf')),
    ('rage::mf',                 ('rage', 'rage_mf')),

    # ── rage:: particle/renderable ────────────────────────────────────────────
    ('rage::rmcDrawable',        ('rage', 'rage_grm')),
    ('rage::rmcType',            ('rage', 'rage_grm')),
    ('rage::rmc',                ('rage', 'rage_grm')),
    ('rage::rmPtx',              ('rage', 'rage_grm')),
    ('rage::rm',                 ('rage', 'rage_grm')),

    # ── rage:: graphics/model ─────────────────────────────────────────────────
    ('rage::grmShaderFx',        ('rage', 'rage_grm')),
    ('rage::grmShaderGroup',     ('rage', 'rage_grm')),
    ('rage::grmShaderFactory',   ('rage', 'rage_grm')),
    ('rage::grmShaderPreset',    ('rage', 'rage_grm')),
    ('rage::grmShaderBasic',     ('rage', 'rage_grm')),
    ('rage::grmShader',          ('rage', 'rage_grm')),
    ('rage::grmModel',           ('rage', 'rage_grm')),
    ('rage::grmEdge',            ('rage', 'rage_grm')),
    ('rage::grmSetup',           ('rage', 'rage_grm')),
    ('rage::grm',                ('rage', 'rage_grm')),

    # ── rage:: scene graph ────────────────────────────────────────────────────
    ('rage::grc',                ('graphics', 'grc_render')),

    # ── rage:: physics ────────────────────────────────────────────────────────
    ('rage::phBound',            ('physics', 'ph_physics')),
    ('rage::phSimulator',        ('physics', 'ph_physics')),
    ('rage::phLevelNew',         ('physics', 'ph_physics')),
    ('rage::phLevelBase',        ('physics', 'ph_physics')),
    ('rage::phInst',             ('physics', 'ph_physics')),
    ('rage::phArch',             ('physics', 'ph_physics')),
    ('rage::phCollider',         ('physics', 'ph_physics')),
    ('rage::phConstraint',       ('physics', 'ph_physics')),
    ('rage::phJoint',            ('physics', 'ph_physics')),
    ('rage::phSleep',            ('physics', 'ph_physics')),
    ('rage::phCloth',            ('physics', 'ph_physics')),
    ('rage::phRope',             ('physics', 'ph_physics')),
    ('rage::phMorph',            ('physics', 'ph_physics')),
    ('rage::phMaterial',         ('physics', 'ph_physics')),
    ('rage::phSpring',           ('physics', 'ph_physics')),
    ('rage::phInstBehavior',     ('physics', 'ph_physics')),
    ('rage::environmentCloth',   ('physics', 'ph_physics')),
    ('rage::clothController',    ('physics', 'ph_physics')),
    ('rage::ph',                 ('physics', 'ph_physics')),

    # ── rage:: fragmented objects (frag*) ────────────────────────────────────
    ('rage::fragType',           ('rage', 'rage_grm')),
    ('rage::fragInst',           ('rage', 'rage_grm')),
    ('rage::fragTypeChild',      ('rage', 'rage_grm')),
    ('rage::fragDrawable',       ('rage', 'rage_grm')),
    ('rage::fragHeapAllocator',  ('rage', 'rage_grm')),
    ('rage::frag',               ('rage', 'rage_grm')),

    # ── rage:: spd* (spline/path) ─────────────────────────────────────────────
    ('rage::spd',                ('rage', 'rage_grm')),

    # ── rage:: audio ──────────────────────────────────────────────────────────
    ('rage::audControl',         ('rage', 'rage_audio')),
    ('rage::audBank',            ('rage', 'rage_audio')),
    ('rage::audVoice',           ('rage', 'rage_audio')),
    ('rage::audControl',         ('rage', 'rage_audio')),
    ('rage::audBankMgr',         ('rage', 'rage_audio')),
    ('rage::audControlWrapper',  ('rage', 'rage_audio')),
    ('rage::aud',                ('rage', 'rage_audio')),

    # ── rage:: file I/O ───────────────────────────────────────────────────────
    ('rage::fiDevice',           ('rage', 'rage_fi')),
    ('rage::fiSerialize',        ('rage', 'rage_fi')),
    ('rage::fiTokenizer',        ('rage', 'rage_fi')),
    ('rage::fiBin',              ('rage', 'rage_fi')),
    ('rage::fiAscii',            ('rage', 'rage_fi')),
    ('rage::fiPackfile',         ('rage', 'rage_fi')),
    ('rage::fiDirtyDisc',        ('rage', 'rage_fi')),
    ('rage::fi',                 ('rage', 'rage_fi')),

    # ── rage:: data / serialization ───────────────────────────────────────────
    ('rage::datParser',          ('rage', 'rage_dat')),
    ('rage::datSerialize',       ('rage', 'rage_dat')),
    ('rage::datBitBuffer',       ('rage', 'rage_dat')),
    ('rage::datBitBufferDefault',('rage', 'rage_dat')),
    ('rage::datBase',            ('rage', 'rage_dat')),
    ('rage::datParserRecord',    ('rage', 'rage_dat')),
    ('rage::dat',                ('rage', 'rage_dat')),
    ('rage::dc',                 ('rage', 'rage_dat')),
    ('rage::netBBAllocator',     ('rage', 'rage_dat')),

    # ── rage:: par* serialization ─────────────────────────────────────────────
    ('rage::parMember',          ('rage', 'rage_par')),
    ('rage::parStructure',       ('rage', 'rage_par')),
    ('rage::parRTStructure',     ('rage', 'rage_par')),
    ('rage::parFile',            ('rage', 'rage_par')),
    ('rage::parStream',          ('rage', 'rage_par')),
    ('rage::par',                ('rage', 'rage_par')),

    # ── rage:: xml ────────────────────────────────────────────────────────────
    ('rage::xmlTree',            ('rage', 'rage_xml')),
    ('rage::xmlNodeStruct',      ('rage', 'rage_xml')),
    ('rage::xmlNodeString',      ('rage', 'rage_xml')),
    ('rage::xmlNode',            ('rage', 'rage_xml')),
    ('rage::xml',                ('rage', 'rage_xml')),

    # ── rage:: cm* computation graph ──────────────────────────────────────────
    ('rage::cmSampleCam',        ('rage', 'rage_cm')),
    ('rage::cmMetafile',         ('rage', 'rage_cm')),
    ('rage::cmDiffTuning',       ('rage', 'rage_cm')),
    ('rage::cmNamed',            ('rage', 'rage_cm')),
    ('rage::cmCompiled',         ('rage', 'rage_cm')),
    ('rage::cmAnim',             ('rage', 'rage_cm')),
    ('rage::cmProbe',            ('rage', 'rage_cm')),
    ('rage::cmAction',           ('rage', 'rage_cm')),
    ('rage::cmSimpleAction',     ('rage', 'rage_cm')),
    ('rage::cmActionFactory',    ('rage', 'rage_cm')),
    ('rage::cmAdd',              ('rage', 'rage_cm')),
    ('rage::cmSubtract',         ('rage', 'rage_cm')),
    ('rage::cmMultiply',         ('rage', 'rage_cm')),
    ('rage::cmDivide',           ('rage', 'rage_cm')),
    ('rage::cmModulo',           ('rage', 'rage_cm')),
    ('rage::cmNegate',           ('rage', 'rage_cm')),
    ('rage::cmSine',             ('rage', 'rage_cm')),
    ('rage::cmCosine',           ('rage', 'rage_cm')),
    ('rage::cmArcCosine',        ('rage', 'rage_cm')),
    ('rage::cmTangent',          ('rage', 'rage_cm')),
    ('rage::cmArcTangent',       ('rage', 'rage_cm')),
    ('rage::cmInverse',          ('rage', 'rage_cm')),
    ('rage::cmAbs',              ('rage', 'rage_cm')),
    ('rage::cmSign',             ('rage', 'rage_cm')),
    ('rage::cmMax',              ('rage', 'rage_cm')),
    ('rage::cmMin',              ('rage', 'rage_cm')),
    ('rage::cmLerp',             ('rage', 'rage_cm')),
    ('rage::cmAngle',            ('rage', 'rage_cm')),
    ('rage::cmRange',            ('rage', 'rage_cm')),
    ('rage::cmClamp',            ('rage', 'rage_cm')),
    ('rage::cmSlowIn',           ('rage', 'rage_cm')),
    ('rage::cmSlowOut',          ('rage', 'rage_cm')),
    ('rage::cmSlowInOut',        ('rage', 'rage_cm')),
    ('rage::cmFOV',              ('rage', 'rage_cm')),
    ('rage::cmSqrt',             ('rage', 'rage_cm')),
    ('rage::cmPow',              ('rage', 'rage_cm')),
    ('rage::cmNoise',            ('rage', 'rage_cm')),
    ('rage::cmGNoise',           ('rage', 'rage_cm')),
    ('rage::cmVector',           ('rage', 'rage_cm')),
    ('rage::cmEquals',           ('rage', 'rage_cm')),
    ('rage::cmGreater',          ('rage', 'rage_cm')),
    ('rage::cmLess',             ('rage', 'rage_cm')),
    ('rage::cmOr',               ('rage', 'rage_cm')),
    ('rage::cmXor',              ('rage', 'rage_cm')),
    ('rage::cmAnd',              ('rage', 'rage_cm')),
    ('rage::cmNot',              ('rage', 'rage_cm')),
    ('rage::cmDiff',             ('rage', 'rage_cm')),
    ('rage::cmMemory',           ('rage', 'rage_cm')),
    ('rage::cmIntegrate',        ('rage', 'rage_cm')),
    ('rage::cmPower',            ('rage', 'rage_cm')),
    ('rage::cmApproach',         ('rage', 'rage_cm')),
    ('rage::cmInfo',             ('rage', 'rage_cm')),
    ('rage::cmDraw',             ('rage', 'rage_cm')),
    ('rage::cmDev',              ('rage', 'rage_cm')),
    ('rage::cmIf',               ('rage', 'rage_cm')),
    ('rage::cmLocal',            ('rage', 'rage_cm')),
    ('rage::cmSelect',           ('rage', 'rage_cm')),
    ('rage::cmIs',               ('rage', 'rage_cm')),
    ('rage::cmX',                ('rage', 'rage_cm')),
    ('rage::cmY',                ('rage', 'rage_cm')),
    ('rage::cmZ',                ('rage', 'rage_cm')),
    ('rage::cmViz',              ('rage', 'rage_cm')),
    ('rage::cmDot',              ('rage', 'rage_cm')),
    ('rage::cmFlat',             ('rage', 'rage_cm')),
    ('rage::cmCross',            ('rage', 'rage_cm')),
    ('rage::cmNormal',           ('rage', 'rage_cm')),
    ('rage::cmComp',             ('rage', 'rage_cm')),
    ('rage::cmMag',              ('rage', 'rage_cm')),
    ('rage::cmDist',             ('rage', 'rage_cm')),
    ('rage::cmRotate',           ('rage', 'rage_cm')),
    ('rage::cmScale',            ('rage', 'rage_cm')),
    ('rage::cmFlatten',          ('rage', 'rage_cm')),
    ('rage::cmWorld',            ('rage', 'rage_cm')),
    ('rage::cmScreen',           ('rage', 'rage_cm')),
    ('rage::cmLookAt',           ('rage', 'rage_cm')),
    ('rage::cmPassThrough',      ('rage', 'rage_cm')),
    ('rage::cmPrint',            ('rage', 'rage_cm')),
    ('rage::cmSPrint',           ('rage', 'rage_cm')),
    ('rage::cmGraph',            ('rage', 'rage_cm')),
    ('rage::cmOperatorSet',      ('rage', 'rage_cm')),
    ('rage::cmOperator',         ('rage', 'rage_cm')),
    ('rage::cmCached',           ('rage', 'rage_cm')),
    ('rage::cmSimple',           ('rage', 'rage_cm')),
    ('rage::cmMachine',          ('rage', 'rage_cm')),
    ('rage::cmGang',             ('rage', 'rage_cm')),
    ('rage::cmMachineBank',      ('rage', 'rage_cm')),
    ('rage::cmReporter',         ('rage', 'rage_cm')),
    ('rage::cmValueSet',         ('rage', 'rage_cm')),
    ('rage::cmTuning',           ('rage', 'rage_cm')),
    ('rage::cmBlock',            ('rage', 'rage_cm')),
    ('rage::cmChanged',          ('rage', 'rage_cm')),
    ('rage::cmTurns',            ('rage', 'rage_cm')),
    ('rage::cmDifferentiate',    ('rage', 'rage_cm')),
    ('rage::cmResettable',       ('rage', 'rage_cm')),
    ('rage::cmCapture',          ('rage', 'rage_cm')),
    ('rage::cmMotor',            ('rage', 'rage_cm')),
    ('rage::cmTunable',          ('rage', 'rage_cm')),
    ('rage::cmControlRef',       ('rage', 'rage_cm')),
    ('rage::cmSampleCamActions',  ('rage', 'rage_cm')),
    ('rage::cmSampleCamActions::',('rage', 'rage_cm')),
    ('rage::cm',                 ('rage', 'rage_cm')),
    # non-rage cm* (template ctors etc.)
    ('cmOperatorCtor',           ('game', 'cm_operators')),
    ('cmRefreshableCtor',        ('game', 'cm_operators')),
    ('cmWorldRefreshableCtor',   ('game', 'cm_operators')),
    ('cmWorldApproach2Ctor',     ('game', 'cm_operators')),
    ('cmApproach2Ctor',          ('game', 'cm_operators')),
    ('cmReporterCtor',           ('game', 'cm_operators')),
    ('cmActionCtor',             ('game', 'cm_operators')),
    ('cmCond',                   ('game', 'cm_operators')),
    ('cmLookup',                 ('game', 'cm_operators')),
    ('cmSwitch',                 ('game', 'cm_operators')),
    ('cmLinearRemap',            ('game', 'cm_operators')),
    ('cmSmoothRemap',            ('game', 'cm_operators')),
    ('cmNaryOperator',           ('game', 'cm_operators')),
    ('cmSumReporter',            ('game', 'cm_operators')),
    ('cmAvgReporter',            ('game', 'cm_operators')),
    ('cmNoneReporter',           ('game', 'cm_operators')),
    ('cmFirstReporter',          ('game', 'cm_operators')),
    ('cmLastReporter',           ('game', 'cm_operators')),
    ('cmAndReporter',            ('game', 'cm_operators')),
    ('cmOrReporter',             ('game', 'cm_operators')),
    ('cmBitwiseAndReporter',     ('game', 'cm_operators')),
    ('cmBitwiseOrReporter',      ('game', 'cm_operators')),
    ('cmMostReporter',           ('game', 'cm_operators')),
    ('cmMinReporter',            ('game', 'cm_operators')),
    ('cmMaxReporter',            ('game', 'cm_operators')),
    ('cmNearestToReporter',      ('game', 'cm_operators')),
    ('cmFurthestFromReporter',   ('game', 'cm_operators')),
    ('cmCMCamera',               ('game', 'cm_operators')),
    ('cm',                       ('game', 'cm_operators')),

    # ── rage:: memory / system ────────────────────────────────────────────────
    ('rage::sysMemSimple',       ('rage', 'rage_misc')),
    ('rage::sysMem',             ('rage', 'rage_misc')),
    ('rage::sysTime',            ('rage', 'rage_misc')),
    ('rage::sysThread',          ('rage', 'rage_misc')),
    ('rage::miniheap',           ('rage', 'rage_misc')),
    ('rage::evtSet',             ('rage', 'rage_misc')),
    ('rage::evtInstance',        ('rage', 'rage_misc')),
    ('rage::atAny',              ('rage', 'rage_misc')),
    ('rage::netConnection',      ('rage', 'rage_misc')),
    ('rage::netConnection::',    ('rage', 'rage_misc')),
    ('rage::dcamPolar',          ('rage', 'rage_misc')),
    ('rage::dcamCam',            ('rage', 'rage_misc')),
    ('rage::dcam',               ('rage', 'rage_misc')),

    # ── rage:: math ───────────────────────────────────────────────────────────
    ('rage::MatrixMND',          ('rage', 'rage_math')),
    ('rage::VectorNI',           ('rage', 'rage_math')),
    ('rage::VectorND',           ('rage', 'rage_math')),

    # ── rage:: pg* ────────────────────────────────────────────────────────────
    ('pgDictionary',             ('rage', 'rage_pg')),
    ('rage::pg',                 ('rage', 'rage_pg')),

    # ── rage:: at* (singleton template) ──────────────────────────────────────
    ('atSingleton',              ('rage', 'rage_at')),
    ('atSafeDLList',             ('rage', 'rage_at')),
    ('rage::at',                 ('rage', 'rage_at')),

    # ── rage:: catch-all ──────────────────────────────────────────────────────
    ('rage::',                   ('rage', 'rage_misc')),

    # ── scene graph (sg*) ─────────────────────────────────────────────────────
    ('sgNode',                   ('graphics', 'sg_scene')),
    ('sgPhysical',               ('graphics', 'sg_scene')),
    ('sgSceneGraph',             ('graphics', 'sg_scene')),
    ('sgTraverse',               ('graphics', 'sg_scene')),
    ('sgRMDrawable',             ('graphics', 'sg_scene')),
    ('sg',                       ('graphics', 'sg_scene')),

    # ── grc* render ───────────────────────────────────────────────────────────
    ('grc',                      ('graphics', 'grc_render')),

    # ── fx* effects ───────────────────────────────────────────────────────────
    ('fxCloth',                  ('graphics', 'fx_effects')),
    ('fxHair',                   ('graphics', 'fx_effects')),
    ('fxBallShadow',             ('graphics', 'fx_effects')),
    ('fxBallSplash',             ('graphics', 'fx_effects')),
    ('fxBallTrail',              ('graphics', 'fx_effects')),
    ('fxBallSpinTex',            ('graphics', 'fx_effects')),
    ('fxBallSpinLine',           ('graphics', 'fx_effects')),
    ('fxBallSpinWisp',           ('graphics', 'fx_effects')),
    ('fxBallSpinData',           ('graphics', 'fx_effects')),
    ('fxBallSpin',               ('graphics', 'fx_effects')),
    ('fxBallTrailData',          ('graphics', 'fx_effects')),
    ('fxBall',                   ('graphics', 'fx_effects')),
    ('fxLightShaft',             ('graphics', 'fx_effects')),
    ('fxAmbientMgr',             ('graphics', 'fx_effects')),
    ('fxAmbientAnim',            ('graphics', 'fx_effects')),
    ('fxAmbient',                ('graphics', 'fx_effects')),
    ('fxTrailData',              ('graphics', 'fx_effects')),
    ('fxTrail',                  ('graphics', 'fx_effects')),
    ('fxLvlAnimData',            ('graphics', 'fx_effects')),
    ('fxLvlAnimSet',             ('graphics', 'fx_effects')),
    ('fxBoobSpring',             ('graphics', 'fx_effects')),
    ('fxBoobs',                  ('graphics', 'fx_effects')),
    ('fxBoneSpring',             ('graphics', 'fx_effects')),
    ('fxReticle',                ('graphics', 'fx_effects')),
    ('fxCrowdGfx',               ('graphics', 'fx_effects')),
    ('fxCrowdAnim',              ('graphics', 'fx_effects')),
    ('fxCrowdArch',              ('graphics', 'fx_effects')),
    ('fxCrowdManager',           ('graphics', 'fx_effects')),
    ('fxCrowd',                  ('graphics', 'fx_effects')),
    ('fxSpecialFx',              ('graphics', 'fx_effects')),
    ('fxStringGroup',            ('graphics', 'fx_effects')),
    ('TxBallSpin',               ('graphics', 'fx_effects')),
    ('fx',                       ('graphics', 'fx_effects')),

    # ── hud* ──────────────────────────────────────────────────────────────────
    ('hudFlashBase',             ('ui', 'hud')),
    ('hudHUD',                   ('ui', 'hud')),
    ('hudTrainingHUD',           ('ui', 'hud')),
    ('hudTrainingPopUp',         ('ui', 'hud')),
    ('hudTrainingLoad',          ('ui', 'hud')),
    ('hudList',                  ('ui', 'hud')),
    ('hudBoot',                  ('ui', 'hud')),
    ('hudLegals',                ('ui', 'hud')),
    ('hudPause',                 ('ui', 'hud')),
    ('hudUnlocks',               ('ui', 'hud')),
    ('hudShell',                 ('ui', 'hud')),
    ('hudLogos',                 ('ui', 'hud')),
    ('hudLoading',               ('ui', 'hud')),
    ('hudController',            ('ui', 'hud')),
    ('hudCredits',               ('ui', 'hud')),
    ('hudCharView',              ('ui', 'hud')),
    ('hudDialog',                ('ui', 'hud')),
    ('hudFrontEnd',              ('ui', 'hud')),
    ('hudLeaderboard',           ('ui', 'hud')),
    ('SpectatorHudItem',         ('ui', 'hud')),
    ('SpectatorHud',             ('ui', 'hud')),
    ('uiList',                   ('ui', 'hud')),
    ('hud',                      ('ui', 'hud')),

    # ── cv* curves ────────────────────────────────────────────────────────────
    ('cvCurve',                  ('ui', 'cv_ui')),
    ('cv',                       ('ui', 'cv_ui')),

    # ── ph* physics (standalone, non-rage::) ─────────────────────────────────
    ('phDemoObject',             ('physics', 'ph_physics')),
    ('phDemoWorld',              ('physics', 'ph_physics')),
    ('phMaterialMgrImpl',        ('physics', 'ph_physics')),
    ('ph',                       ('physics', 'ph_physics')),

    # ── pong* creature & animation ────────────────────────────────────────────
    ('pongCreature',             ('game', 'pong_creature')),
    ('pongCreatureInst',         ('game', 'pong_creature')),
    ('pongCreatureType',         ('game', 'pong_creature')),
    ('pongAnimationInfo',        ('game', 'pong_creature')),
    ('pongLookAt',               ('game', 'pong_creature')),
    ('pongMover',                ('game', 'pong_creature')),
    ('pongBlend',                ('game', 'pong_creature')),
    ('pongBlender',              ('game', 'pong_creature')),
    ('pongLocomotion',           ('game', 'pong_creature')),
    ('pongSkelLookAt',           ('game', 'pong_creature')),
    ('pongHairData',             ('game', 'pong_creature')),
    ('pongPlayerShader',         ('game', 'pong_creature')),
    ('pcrFace',                  ('game', 'pong_creature')),
    ('pcrAnim',                  ('game', 'pong_creature')),
    ('pcrSwing',                 ('game', 'pong_creature')),
    ('pcrServe',                 ('game', 'pong_creature')),
    ('pcrEmote',                 ('game', 'pong_creature')),
    ('pcrPostPoint',             ('game', 'pong_creature')),
    ('pcrActivation',            ('game', 'pong_creature')),
    ('pcrJunkSwing',             ('game', 'pong_creature')),
    ('pcr',                      ('game', 'pong_creature')),
    ('LocomotionState',          ('game', 'pong_creature')),
    ('LocomotionController',     ('game', 'pong_creature')),
    ('LocomotionState::',        ('game', 'pong_creature')),
    ('LocomotionController::',   ('game', 'pong_creature')),

    # ── pong* ball ────────────────────────────────────────────────────────────
    ('pongBallInstance',         ('game', 'pong_ball')),
    ('pongBallHit',              ('game', 'pong_ball')),
    ('pongBallHitData',          ('game', 'pong_ball')),
    ('pongBallAudio',            ('game', 'pong_ball')),
    ('pongBall',                 ('game', 'pong_ball')),
    ('pongPaddle',               ('game', 'pong_ball')),
    ('gdBall',                   ('game', 'pong_ball')),
    ('gmBallRobot',              ('game', 'pong_ball')),
    ('gmBallNode',               ('game', 'pong_ball')),

    # ── pong* match & game logic ──────────────────────────────────────────────
    ('gmLogicSpectator',         ('game', 'pong_match')),
    ('gmLogicSpectator::',       ('game', 'pong_match')),
    ('gmLogicSinglesMatch',      ('game', 'pong_match')),
    ('gmLogicSinglesMatch::',    ('game', 'pong_match')),
    ('gmLogicSwingPractice',     ('game', 'pong_match')),
    ('gmLogicSwingPractice::',   ('game', 'pong_match')),
    ('gmLogic',                  ('game', 'pong_match')),
    ('gmBallRobot',              ('game', 'pong_match')),
    ('gmPracticeTarget',         ('game', 'pong_match')),
    ('MatchData',                ('game', 'pong_match')),
    ('gdGameData',               ('game', 'pong_match')),
    ('gdRivalry',                ('game', 'pong_match')),
    ('gdTier',                   ('game', 'pong_match')),
    ('gdLadder',                 ('game', 'pong_match')),
    ('gdDifficulty',             ('game', 'pong_match')),
    ('shotTarget',               ('game', 'pong_match')),
    ('powerZone',                ('game', 'pong_match')),

    # ── pong* drill & training ────────────────────────────────────────────────
    ('pongTrainingDrill',        ('game', 'pong_drill')),
    ('pongDrillMovement',        ('game', 'pong_drill')),
    ('pongDrillServeMeter',      ('game', 'pong_drill')),
    ('pongDrillServing',         ('game', 'pong_drill')),
    ('pongDrillReturn',          ('game', 'pong_drill')),
    ('pongDrillPlacement',       ('game', 'pong_drill')),
    ('pongDrillSoftShot',        ('game', 'pong_drill')),
    ('pongDrillCharging',        ('game', 'pong_drill')),
    ('pongDrillSpin',            ('game', 'pong_drill')),
    ('pongDrillFocusShot',       ('game', 'pong_drill')),
    ('pongDrillCounterSpin',     ('game', 'pong_drill')),
    ('pongDrillSmash',           ('game', 'pong_drill')),
    ('pongDrill',                ('game', 'pong_drill')),
    ('gdTrainingDrill',          ('game', 'pong_drill')),
    ('gdTrainingDrills',         ('game', 'pong_drill')),
    ('gdTrainingSave',           ('game', 'pong_drill')),
    ('gdDrillMovement',          ('game', 'pong_drill')),
    ('gdDrillServe',             ('game', 'pong_drill')),
    ('gdDrillReturn',            ('game', 'pong_drill')),
    ('gdDrillPlacement',         ('game', 'pong_drill')),
    ('gdDrillSoftShot',          ('game', 'pong_drill')),
    ('gdDrillCharging',          ('game', 'pong_drill')),
    ('gdDrillSpin',              ('game', 'pong_drill')),
    ('gdDrillFocusShot',         ('game', 'pong_drill')),
    ('gdDrillCounterSpin',       ('game', 'pong_drill')),
    ('gdDrillSmash',             ('game', 'pong_drill')),
    ('hitTipData',               ('game', 'pong_drill')),
    ('serveTipData',             ('game', 'pong_drill')),
    ('focusMeter',               ('game', 'pong_drill')),
    ('forcedFromTable',          ('game', 'pong_drill')),
    ('noSoftShots',              ('game', 'pong_drill')),
    ('tooFarFromTable',          ('game', 'pong_drill')),
    ('badSoftShot',              ('game', 'pong_drill')),
    ('backspin',                 ('game', 'pong_drill')),
    ('firstServe',               ('game', 'pong_drill')),
    ('reviewOnly',               ('game', 'pong_drill')),
    ('statsThreshold',           ('game', 'pong_drill')),

    # ── pong* network ─────────────────────────────────────────────────────────
    ('PongNet',                  ('game', 'pong_network')),
    ('pongNet',                  ('game', 'pong_network')),
    ('pongLive',                 ('game', 'pong_network')),
    ('pongPresence',             ('game', 'pong_network')),
    ('pongRemotePlayer',         ('game', 'pong_network')),
    # Standalone network classes (capital N Net* / standalone message types)
    ('NetworkClient',            ('game', 'pong_network')),
    ('NetTimedMessageSortedQueue',('game','pong_network')),
    ('NetTimer',                 ('game', 'pong_network')),
    ('NetBallHitManager',        ('game', 'pong_network')),
    ('NetDataQuery',             ('game', 'pong_network')),
    ('NetDataQuery::',           ('game', 'pong_network')),
    ('NetStateSync',             ('game', 'pong_network')),
    ('NetStateSync::',           ('game', 'pong_network')),
    ('NetTuningData',            ('game', 'pong_network')),
    ('NetLoadingThread',         ('game', 'pong_network')),
    ('netPoliciesDerived',       ('game', 'pong_network')),
    ('LogicNetwork',             ('game', 'pong_network')),
    ('SinglesNetwork',           ('game', 'pong_network')),
    ('SpectatorNetwork',         ('game', 'pong_network')),
    ('SessionMessage',           ('game', 'pong_network')),
    ('SessionMessage::',         ('game', 'pong_network')),
    ('FloatAverager',            ('game', 'pong_network')),
    ('FrameTimeEstimate',        ('game', 'pong_network')),
    ('AckHandling',              ('game', 'pong_network')),
    ('EventBase',                ('game', 'pong_network')),
    ('EventSwing',               ('game', 'pong_network')),
    ('pongNetMessageHolderBase', ('game', 'pong_network')),
    ('pongNetMessageHolder',     ('game', 'pong_network')),
    # All the *Message, *Request, *Response classes
    ('SwingStartedMessage',      ('game', 'pong_network')),
    ('HitDataMessage',           ('game', 'pong_network')),
    ('HitMessage',               ('game', 'pong_network')),
    ('MatchTimeSync',            ('game', 'pong_network')),
    ('ForceMatch',               ('game', 'pong_network')),
    ('SessionTimeSync',          ('game', 'pong_network')),
    ('ForceSession',             ('game', 'pong_network')),
    ('PlayerUpdate',             ('game', 'pong_network')),
    ('PlayerMovement',           ('game', 'pong_network')),
    ('PlayerStop',               ('game', 'pong_network')),
    ('PlayerCache',              ('game', 'pong_network')),
    ('Signal',                   ('game', 'pong_network')),
    ('ServeReady',               ('game', 'pong_network')),
    ('ServeAbort',               ('game', 'pong_network')),
    ('ServeStarted',             ('game', 'pong_network')),
    ('ServeFault',               ('game', 'pong_network')),
    ('ServeLet',                 ('game', 'pong_network')),
    ('ServeUnlock',              ('game', 'pong_network')),
    ('RemoteServe',              ('game', 'pong_network')),
    ('JunkSwing',                ('game', 'pong_network')),
    ('StateSync',                ('game', 'pong_network')),
    ('DataRequest',              ('game', 'pong_network')),
    ('DataSend',                 ('game', 'pong_network')),
    ('DataRequestFailed',        ('game', 'pong_network')),
    ('SinglesConnect',           ('game', 'pong_network')),
    ('SpectatorConnect',         ('game', 'pong_network')),
    ('AcceptMessage',            ('game', 'pong_network')),
    ('AcceptSpectator',          ('game', 'pong_network')),
    ('LevelConfig',              ('game', 'pong_network')),
    ('SkipReplay',               ('game', 'pong_network')),
    ('Disconnecting',            ('game', 'pong_network')),
    ('NextMatch',                ('game', 'pong_network')),
    ('MatchScore',               ('game', 'pong_network')),
    ('MatchDataSync',            ('game', 'pong_network')),
    ('PointDeclared',            ('game', 'pong_network')),
    ('LoadingStateRequest',      ('game', 'pong_network')),
    ('LoadingStateResponse',     ('game', 'pong_network')),
    ('ForfeitMatch',             ('game', 'pong_network')),
    ('BallHit',                  ('game', 'pong_network')),
    ('SpectatorBall',            ('game', 'pong_network')),
    ('TimedGameUpdateTimer',     ('game', 'pong_network')),
    ('ExitPostGame',             ('game', 'pong_network')),
    ('DropSpectator',            ('game', 'pong_network')),
    ('SpectatorQuit',            ('game', 'pong_network')),
    ('DenySpectator',            ('game', 'pong_network')),
    ('RemoveSpectator',          ('game', 'pong_network')),
    ('JoinSpectatorSession',     ('game', 'pong_network')),
    ('LeaveSpectatorSession',    ('game', 'pong_network')),
    ('UserRequestReplay',        ('game', 'pong_network')),
    ('NextMatchSpectator',       ('game', 'pong_network')),
    ('TournamentComplete',       ('game', 'pong_network')),
    ('PostPointExit',            ('game', 'pong_network')),
    ('ReplayEmote',              ('game', 'pong_network')),
    ('EnteringReplay',           ('game', 'pong_network')),
    ('ScoreMessage',             ('game', 'pong_network')),
    ('TerminateRally',           ('game', 'pong_network')),
    ('InternalMessageRelay',     ('game', 'pong_network')),
    ('LoadDataQuery',            ('game', 'pong_network')),
    ('GamerReady',               ('game', 'pong_network')),
    ('GamerUpdate',              ('game', 'pong_network')),
    ('MatchStats',               ('game', 'pong_network')),
    ('TwoMinuteWarning',         ('game', 'pong_network')),
    ('RequestData',              ('game', 'pong_network')),
    ('MatchData',                ('game', 'pong_network')),
    ('RoundRobinData',           ('game', 'pong_network')),
    ('StartLobby',               ('game', 'pong_network')),
    ('CancelLobby',              ('game', 'pong_network')),
    ('Gamer',                    ('game', 'pong_network')),
    ('PongNetGameMode',          ('game', 'pong_network')),
    ('PongNetExhibition',        ('game', 'pong_network')),
    ('PongNetRoundRobin',        ('game', 'pong_network')),
    ('PongNetProxy',             ('game', 'pong_network')),
    ('PongNetMessage',           ('game', 'pong_network')),

    # ── pong* camera ──────────────────────────────────────────────────────────
    ('pongCameraMgr',            ('game', 'pong_camera')),
    ('pongCameraState',          ('game', 'pong_camera')),
    ('pongCamGovernor',          ('game', 'pong_camera')),
    ('pongCineCamMgr',           ('game', 'pong_camera')),
    ('pongCMCamera',             ('game', 'pong_camera')),
    ('pongCam',                  ('game', 'pong_camera')),
    ('pongCine',                 ('game', 'pong_camera')),
    ('pongCamera',               ('game', 'pong_camera')),
    ('gdCamera',                 ('game', 'pong_camera')),
    ('camShot',                  ('game', 'pong_camera')),
    ('camBaseCS',                ('game', 'pong_camera')),
    ('camViewCS',                ('game', 'pong_camera')),
    ('charViewCS',               ('game', 'pong_camera')),
    ('cameraShake',              ('game', 'pong_camera')),
    ('CMtoCSAdapter',            ('game', 'pong_camera')),

    # ── pong* render ──────────────────────────────────────────────────────────
    ('pongRenderable',           ('game', 'pong_render')),
    ('pongDrawable',             ('game', 'pong_render')),
    ('pongDrawBucket',           ('game', 'pong_render')),
    ('pongDraw',                 ('game', 'pong_render')),
    ('pongRenderThread',         ('game', 'pong_render')),
    ('pongPostEffects',          ('game', 'pong_render')),
    ('pongShadowMap',            ('game', 'pong_render')),
    ('pongGammaControl',         ('game', 'pong_render')),
    ('pongLightGlow',            ('game', 'pong_render')),
    ('pongScrnTrans',            ('game', 'pong_render')),
    ('pongScreenCapture',        ('game', 'pong_render')),
    ('pongSurface',              ('game', 'pong_render')),

    # ── pong* states ──────────────────────────────────────────────────────────
    ('pongBootState',            ('game', 'pong_states')),
    ('pongAttractState',         ('game', 'pong_states')),
    ('pongAttractContext',       ('game', 'pong_states')),
    ('pongLogosState',           ('game', 'pong_states')),
    ('pongLogosContext',         ('game', 'pong_states')),
    ('pongTrainingUIState',      ('game', 'pong_states')),
    ('pongTrainingUIContext',    ('game', 'pong_states')),
    ('pongLoadingState',         ('game', 'pong_states')),
    ('pongLoadingContext',       ('game', 'pong_states')),
    ('pongTrainingLoading',      ('game', 'pong_states')),
    ('pongPauseState',           ('game', 'pong_states')),
    ('pongPauseContext',         ('game', 'pong_states')),
    ('pongCreditsContext',       ('game', 'pong_states')),
    ('pongCreditsState',         ('game', 'pong_states')),
    ('pongCharViewState',        ('game', 'pong_states')),
    ('pongCharViewContext',      ('game', 'pong_states')),
    ('pongDialogState',          ('game', 'pong_states')),
    ('pongDialogContext',        ('game', 'pong_states')),
    ('pongFrontendState',        ('game', 'pong_states')),
    ('pongFrontendContext',      ('game', 'pong_states')),
    ('pongFrontendLive',         ('game', 'pong_states')),
    ('pongLegalsContext',        ('game', 'pong_states')),
    ('pongLegalsState',          ('game', 'pong_states')),
    ('pongStatsContext',         ('game', 'pong_states')),
    ('pongLeaderboardContext',   ('game', 'pong_states')),
    ('charViewChar',             ('game', 'pong_states')),
    ('charViewData',             ('game', 'pong_states')),
    ('creditsData',              ('game', 'pong_states')),
    ('creditsSettings',          ('game', 'pong_states')),
    ('dialogData',               ('game', 'pong_states')),
    ('dialogManager',            ('game', 'pong_states')),
    ('frontendData',             ('game', 'pong_states')),
    ('rlConcreteStatsView',      ('game', 'pong_states')),
    ('hsmState',                 ('game', 'pong_states')),  # hsmState (non-rage::)

    # ── pong* HUD ─────────────────────────────────────────────────────────────
    ('pongHUDLive',              ('game', 'pong_hud')),

    # ── pong* cutscene ────────────────────────────────────────────────────────
    ('pongCutSceneManager',      ('game', 'pong_cutscene')),
    ('pongCutScene',             ('game', 'pong_cutscene')),
    ('pongCSAction',             ('game', 'pong_cutscene')),
    ('pongCSReplayDirector',     ('game', 'pong_cutscene')),
    ('pongCS',                   ('game', 'pong_cutscene')),
    ('pongInlineCinematics',     ('game', 'pong_cutscene')),
    ('gdCSCamAnim',              ('game', 'pong_cutscene')),
    ('gdCSCharCam',              ('game', 'pong_cutscene')),
    ('gdCSCharAnim',             ('game', 'pong_cutscene')),
    ('gdCSChar',                 ('game', 'pong_cutscene')),
    ('gdCSCam',                  ('game', 'pong_cutscene')),
    ('gdCSName',                 ('game', 'pong_cutscene')),
    ('gdCSAction',               ('game', 'pong_cutscene')),
    ('gdCutScene',               ('game', 'pong_cutscene')),
    ('gdCutSceneData',           ('game', 'pong_cutscene')),
    ('gdCutSceneNames',          ('game', 'pong_cutscene')),

    # ── pong* audio ───────────────────────────────────────────────────────────
    ('pongAudioManager',         ('game', 'pong_audio')),
    ('pongAmbiencePlayer',       ('game', 'pong_audio')),
    ('pongAudio',                ('game', 'pong_audio')),
    ('audMsgSink',               ('game', 'pong_audio')),
    ('CBiquadFilterEffect',      ('game', 'pong_audio')),
    ('CCompEffect',              ('game', 'pong_audio')),
    ('CDelayEffect',             ('game', 'pong_audio')),
    ('CPeakMeterEffect',         ('game', 'pong_audio')),
    ('CShelvingFilterEffect',    ('game', 'pong_audio')),
    ('CMtoCSAdapter',            ('game', 'pong_audio')),

    # ── pong* game shell ──────────────────────────────────────────────────────
    ('pongGame',                 ('game', 'pong_game')),
    ('pongShell',                ('game', 'pong_game')),
    ('pongControlGroup',         ('game', 'pong_game')),
    ('pongGameState',            ('game', 'pong_game')),
    ('pongGameContext',          ('game', 'pong_game')),
    ('pongRandomizer',           ('game', 'pong_game')),
    ('shShell',                  ('game', 'pong_game')),
    ('shInit',                   ('game', 'pong_game')),
    ('shEnd',                    ('game', 'pong_game')),
    ('lvlLevel',                 ('game', 'pong_game')),
    ('lvlLevelMgr',              ('game', 'pong_game')),
    ('lvlLevelPiece',            ('game', 'pong_game')),
    ('lvlTable',                 ('game', 'pong_game')),
    ('lvl',                      ('game', 'pong_game')),

    # ── pong* misc ────────────────────────────────────────────────────────────
    ('pongSaveFile',             ('game', 'pong_misc')),
    ('pongMovie',                ('game', 'pong_misc')),
    ('pongXMV',                  ('game', 'pong_misc')),
    ('pongBink',                 ('game', 'pong_misc')),
    ('pongStatsContext',         ('game', 'pong_misc')),
    ('pong',                     ('game', 'pong_misc')),

    # ── plr* player manager ───────────────────────────────────────────────────
    ('plrPlayerMgr',             ('game', 'gd_data')),
    ('plrSwingLogic',            ('game', 'gd_data')),
    ('plrPropMgr',               ('game', 'gd_data')),
    ('plr',                      ('game', 'gd_data')),

    # ── gd* game data ─────────────────────────────────────────────────────────
    ('gdaiData',                 ('game', 'gd_data')),
    ('gdaiDef',                  ('game', 'gd_data')),
    ('gdaiOff',                  ('game', 'gd_data')),
    ('gdaiMeter',                ('game', 'gd_data')),
    ('gdaiShot',                 ('game', 'gd_data')),
    ('gdaiSkill',                ('game', 'gd_data')),
    ('gdaiDifficulty',           ('game', 'gd_data')),
    ('gdaiStats',                ('game', 'gd_data')),
    ('gdaiDeficit',              ('game', 'gd_data')),
    ('gdaiLead',                 ('game', 'gd_data')),
    ('gdaiCome',                 ('game', 'gd_data')),
    ('gdaiDataMgr',              ('game', 'gd_data')),
    ('gdai',                     ('game', 'gd_data')),
    ('gdUnlock',                 ('game', 'gd_data')),
    ('gdStats',                  ('game', 'gd_data')),
    ('gdTourney',                ('game', 'gd_data')),
    ('gdNet',                    ('game', 'gd_data')),
    ('gdCr',                     ('game', 'gd_data')),
    ('gdBlend',                  ('game', 'gd_data')),
    ('gdBTgt',                   ('game', 'gd_data')),
    ('gdChar',                   ('game', 'gd_data')),
    ('gdBall',                   ('game', 'gd_data')),
    ('gdShot',                   ('game', 'gd_data')),
    ('gdMotion',                 ('game', 'gd_data')),
    ('gdLevel',                  ('game', 'gd_data')),
    ('gdInput',                  ('game', 'gd_data')),
    ('gdEnvelope',               ('game', 'gd_data')),
    ('gdCam',                    ('game', 'gd_data')),
    ('gdView',                   ('game', 'gd_data')),
    ('gdVib',                    ('game', 'gd_data')),
    ('gdProp',                   ('game', 'gd_data')),
    ('gdOptions',                ('game', 'gd_data')),
    ('gdProfile',                ('game', 'gd_data')),
    ('gdAsset',                  ('game', 'gd_data')),
    ('gdITZ',                    ('game', 'gd_data')),
    ('gdPattern',                ('game', 'gd_data')),
    ('gdTimeline',               ('game', 'gd_data')),
    ('gdAttribute',              ('game', 'gd_data')),
    ('gdAud',                    ('game', 'gd_data')),
    ('gdPlyr',                   ('game', 'gd_data')),
    ('gdLogical',                ('game', 'gd_data')),
    ('gdReplay',                 ('game', 'gd_data')),
    ('gdMovie',                  ('game', 'gd_data')),
    ('gdRivalry',                ('game', 'gd_data')),
    ('gd',                       ('game', 'gd_data')),

    # ── mc* memcard ───────────────────────────────────────────────────────────
    ('mcMemcardControl',         ('game', 'mc_memcard')),
    ('mcMemcardControl::',       ('game', 'mc_memcard')),
    ('mcSegment',                ('game', 'mc_memcard')),
    ('mcFile',                   ('game', 'mc_memcard')),
    ('mc',                       ('game', 'mc_memcard')),

    # ── ai* ────────────────────────────────────────────────────────────────────
    ('aiPlayer',                 ('game', 'pong_misc')),
    ('aiSpin',                   ('game', 'pong_misc')),
    ('ai',                       ('game', 'pong_misc')),

    # ── msg* ──────────────────────────────────────────────────────────────────
    ('msgMsgSink',               ('game', 'pong_misc')),
    ('msgEventHandler',          ('game', 'pong_misc')),
    ('msg',                      ('game', 'pong_misc')),

    # ── misc data classes ─────────────────────────────────────────────────────
    ('assetVersions',            ('game', 'pong_misc')),
    ('cellData',                 ('game', 'pong_misc')),
    ('ctxManager',               ('game', 'pong_misc')),
    ('fontData',                 ('game', 'pong_misc')),
    ('listData',                 ('game', 'pong_misc')),
    ('listItemData',             ('game', 'pong_misc')),
    ('fsmMachine',               ('game', 'pong_misc')),
    ('Timer',                    ('game', 'pong_misc')),
    ('TimerAlert',               ('game', 'pong_misc')),
    ('xmlNodeStructArray',       ('rage', 'rage_xml')),
    ('xmlNodeStructReverbMode',  ('rage', 'rage_xml')),
    ('xmlNodeStructAlignmentType',('rage','rage_xml')),
    ('xmlNodeStructAnim',        ('rage', 'rage_xml')),
    ('xmlRoot',                  ('rage', 'rage_xml')),
    ('xmlNodeStruct',            ('rage', 'rage_xml')),
    ('std::',                    ('rage', 'rage_misc')),
    ('sysThreadPool',            ('rage', 'rage_misc')),
    ('Holder',                   ('rage', 'rage_misc')),

    # ── catch-all ─────────────────────────────────────────────────────────────
    ('',                         ('game', 'pong_misc')),
]

def route(cls):
    for prefix, dest in ROUTES:
        if cls.startswith(prefix):
            return dest
    return ('game', 'pong_misc')

# ─────────────────────────────────────────────────────────────────────────────
# Build per-class info from config maps
# ─────────────────────────────────────────────────────────────────────────────
class_info = {}
for cls in all_names:
    cs = cls.split('::')[-1] if '::' in cls else cls
    ns = '::'.join(cls.split('::')[:-1]) if '::' in cls else ''
    info = {
        'ns':           ns,
        'cname':        cs,
        'slots':        {},
        'fields':       {},
        'debug':        [],
        'vtable_addrs': hier.get(cls, {}).get('vtable_addrs', []),
        'vtable_count': hier.get(cls, {}).get('vtable_count', 1),
    }
    for key in (cs, cls):
        if key in vtlayout:
            for k, v in vtlayout[key].items():
                info['slots'][int(k)] = v
            break
    for key in (cs, cls):
        if key in offsets:
            for k, v in offsets[key].items():
                info['fields'][int(k)] = v
            break
    for key in (cls, cs):
        if key in dbgmap:
            info['debug'] = dbgmap[key]
            break
    class_info[cls] = info

# ─────────────────────────────────────────────────────────────────────────────
# Group by destination file
# ─────────────────────────────────────────────────────────────────────────────
groups = defaultdict(list)
for cls in all_names:
    groups[route(cls)].append(cls)

# ─────────────────────────────────────────────────────────────────────────────
# Header generation
# ─────────────────────────────────────────────────────────────────────────────
def infer_type(e):
    w = e.get('widths', {})
    for t in ('F32','U64','U32','U16','U8'):
        if t in w:
            return {'F32':'float','U64':'uint64_t','U32':'uint32_t',
                    'U16':'uint16_t','U8':'uint8_t'}[t]
    return 'uint32_t'

def gen_header(classes, directory, filename):
    L = [
        f'/**',
        f' * {filename}.hpp — Skeleton class declarations',
        f' * Rockstar Presents Table Tennis (Xbox 360, 2006)',
        f' *',
        f' * {len(classes)} classes — auto-generated by gen_headers_v3.py',
        f' * Sources: rtti_vtable_map | vtable_layout_map | offset_cluster_map',
        f' *          | debug_string_field_map | rtti_class_hierarchy',
        f' *',
        f' * TODO: fill return types, params, inheritance.',
        f' *       Cross-ref recomp/structured_pass5_final/ for each method.',
        f' */',
        f'#pragma once',
        f'#include <stdint.h>',
        f'',
    ]
    open_ns = None

    for cls in classes:
        info  = class_info[cls]
        ns    = info['ns']
        cname = info['cname']

        if ns != open_ns:
            if open_ns:
                L.append(f'}} // namespace {open_ns}')
                L.append('')
            if ns:
                L.append(f'namespace {ns} {{')
                L.append('')
            open_ns = ns

        vc    = info['vtable_count']
        addrs = info['vtable_addrs']
        addr_note = (f'vtable @ {addrs[0]}' if addrs else '(no vtable addr)')
        if vc > 1:
            addr_note = f'{vc} vtables — template/MI'

        L.append(f'// ── {cls}  [{addr_note}] ──────────────────────────')

        dbg = info['debug']
        if dbg:
            meths = sorted(set(e['method'] for e in dbg))
            L.append(f'// Confirmed methods: {", ".join(meths[:6])}{"…" if len(meths)>6 else ""}')
            hints = {}
            for e in dbg:
                for h in e.get('field_hints', []):
                    hints[h['field']] = h['type']
            if hints:
                L.append(f'// Field hints: {", ".join(f"{k}:{v}" for k,v in sorted(hints.items())[:8])}')

        L.append(f'struct {cname} {{')
        L.append(f'    void**      vtable;           // +0x00')

        if info['fields']:
            L.append('')
            L.append('    // ── field access clusters ──')
            prev = 0
            for off in sorted(info['fields'].keys()):
                if off == 0:
                    continue
                e   = info['fields'][off]
                typ = infer_type(e)
                ops = e.get('ops', {})
                ro  = ops.get('LOAD', 0)
                wo  = ops.get('STORE', 0)
                gap = off - prev - 4 if prev > 0 else 0
                if gap > 4:
                    L.append(f'    uint8_t     _pad{off:#06x}[{gap}];')
                L.append(f'    {typ:<12} field_{off:#06x};  // +{off:#06x}  R:{ro} W:{wo}')
                prev = off

        if info['slots']:
            L.append('')
            L.append('    // ── virtual methods ──')
            for slot in sorted(info['slots'].keys()):
                sd   = info['slots'][slot]
                role = sd.get('inferred_role', '')
                sym  = sd.get('symbol', f'{cname}_vfn_{slot}')
                addr = sd.get('addr', '?')
                if role == 'destructor':
                    L.append(f'    virtual ~{cname}();                  // [{slot}] @ {addr}')
                elif role == 'scalar_destructor':
                    L.append(f'    virtual void ScalarDtor(int flags); // [{slot}] @ {addr}')
                else:
                    m = sym
                    for pfx in (f'{cname}_', f'{cname.lower()}_'):
                        if m.lower().startswith(pfx.lower()):
                            m = m[len(pfx):]
                            break
                    L.append(f'    virtual void {m}();  // [{slot}] @ {addr}')

        if dbg:
            seen = set()
            stubs = []
            for e in dbg:
                mn = e['method']
                if mn not in seen:
                    seen.add(mn)
                    ret = 'bool ' if mn[:2] in ('Is','Ha','Ca') else 'void '
                    stubs.append(f'    {ret}{mn}();')
            if stubs:
                L.append('')
                L.append('    // ── non-virtual methods (from debug strings) ──')
                L += stubs

        L.append('};')
        L.append('')

    if open_ns:
        L.append(f'}} // namespace {open_ns}')

    return '\n'.join(L)

# ─────────────────────────────────────────────────────────────────────────────
# Skip already-implemented files
# ─────────────────────────────────────────────────────────────────────────────
SKIP = {
    ('game', 'pong_player'),
    ('game', 'mc_memcard'),
}

stats = {}
for (d, fn), classes in sorted(groups.items()):
    dest = f'{SRC}/{d}/{fn}.hpp'
    label = f'src/{d}/{fn}.hpp'
    if (d, fn) in SKIP:
        stats[label] = f'SKIPPED — already implemented ({len(classes)} classes)'
        continue
    os.makedirs(f'{SRC}/{d}', exist_ok=True)
    content = gen_header(classes, d, fn)
    with open(dest, 'w') as f:
        f.write(content)
    kb = len(content) // 1024
    stats[label] = f'{len(classes):4d} classes  {kb:4d} KB → {dest}'

print('─' * 80)
print('Generated headers:')
for path in sorted(stats.keys()):
    print(f'  {path}: {stats[path]}')

# ── Coverage check ────────────────────────────────────────────────────────────
print()
covered = set()
for classes in groups.values():
    covered.update(classes)
missing = set(all_names) - covered
if missing:
    print(f'WARNING: {len(missing)} classes still uncovered:')
    for m in sorted(missing):
        print(f'  {m}')
else:
    lines_total = sum(
        len(open(f'{SRC}/{d}/{fn}.hpp').readlines())
        for (d,fn) in groups if (d,fn) not in SKIP
        if os.path.exists(f'{SRC}/{d}/{fn}.hpp')
    )
    print(f'✓ All {len(all_names)} classes covered across {len(groups)} files ({lines_total:,} lines total)')
