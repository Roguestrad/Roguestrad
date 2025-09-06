
#ifndef __GAMETYPEINFO_H__
#define __GAMETYPEINFO_H__

/*
===================================================================================

	This file has been generated with the Type Info Generator v1.1 (c) 2004 id Software

	958 constants
	92 enums
	471 classes/structs/unions
	3 templates
	8 max inheritance level for 'iceMonsterZombieSawyer'

===================================================================================
*/

typedef struct {
	const char * name;
	const char * type;
	const char * value;
} constantInfo_t;

typedef struct {
	const char * name;
	int value;
} enumValueInfo_t;

typedef struct {
	const char * typeName;
	const enumValueInfo_t * values;
} enumTypeInfo_t;

typedef struct {
	const char * type;
	const char * name;
	intptr_t offset;
	int size;
} classVariableInfo_t;

typedef struct {
	const char * typeName;
	const char * superType;
	int size;
	const classVariableInfo_t * variables;
} classTypeInfo_t;

static constantInfo_t constantInfo[] = {
	{ "const int", "MAX_CLIENTS", "0" },
	{ "const int", "MAX_CLIENTS_IN_PVS", "0>>3" },
	{ "const int", "GENTITYNUM_BITS", "12" },
	{ "const int", "MAX_GENTITIES", "1<<12" },
	{ "const int", "ENTITYNUM_NONE", "4096-1" },
	{ "const int", "ENTITYNUM_WORLD", "4096-2" },
	{ "const int", "ENTITYNUM_MAX_NORMAL", "4096-2" },
	{ "const int", "ENTITYNUM_FIRST_NON_REPLICATED", "4094-256" },
	{ "constexpr const char *", "BRUSH_ORIGIN_KEY", "__brushOrigin" },
	{ "int", "SRESULT_OK", "0" },
	{ "int", "SRESULT_ERROR", "1" },
	{ "int", "SRESULT_DONE", "2" },
	{ "int", "SRESULT_DONE_WAIT", "3" },
	{ "int", "SRESULT_WAIT", "4" },
	{ "int", "SRESULT_IDLE", "5" },
	{ "int", "SRESULT_SETSTAGE", "6" },
	{ "int", "SRESULT_DONE_FRAME", "7" },
	{ "int", "SRESULT_SETDELAY", "26" },
	{ "const int", "SFLAG_ONCLEAR", "0(0)" },
	{ "const int", "SFLAG_ONCLEARONLY", "0(1)" },
	{ "int", "ev_error", "-1" },
	{ "int", "ev_void", "0" },
	{ "int", "ev_scriptevent", "1" },
	{ "int", "ev_namespace", "2" },
	{ "int", "ev_string", "3" },
	{ "int", "ev_float", "4" },
	{ "int", "ev_vector", "5" },
	{ "int", "ev_entity", "6" },
	{ "int", "ev_field", "7" },
	{ "int", "ev_function", "8" },
	{ "int", "ev_virtualfunction", "9" },
	{ "int", "ev_pointer", "10" },
	{ "int", "ev_object", "11" },
	{ "int", "ev_jumpoffset", "12" },
	{ "int", "ev_argsize", "13" },
	{ "int", "ev_boolean", "14" },
	{ "int", "idVarDef::uninitialized", "0" },
	{ "int", "idVarDef::initializedVariable", "1" },
	{ "int", "idVarDef::initializedConstant", "2" },
	{ "int", "idVarDef::stackVariable", "3" },
	{ "int", "statement_s::FLAG_OBJECTCALL_IMPL_NOT_PARSED_YET", "1" },
	{ "const int", "ANIM_NumAnimChannels", "5" },
	{ "const int", "ANIM_MaxAnimsPerChannel", "3" },
	{ "const int", "ANIM_MaxSyncedAnims", "3" },
	{ "const int", "ANIMCHANNEL_ALL", "0" },
	{ "const int", "ANIMCHANNEL_TORSO", "1" },
	{ "const int", "ANIMCHANNEL_LEGS", "2" },
	{ "const int", "ANIMCHANNEL_HEAD", "3" },
	{ "const int", "ANIMCHANNEL_EYELIDS", "4" },
	{ "int", "JOINTMOD_NONE", "0" },
	{ "int", "JOINTMOD_LOCAL", "1" },
	{ "int", "JOINTMOD_LOCAL_OVERRIDE", "2" },
	{ "int", "JOINTMOD_WORLD", "3" },
	{ "int", "JOINTMOD_WORLD_OVERRIDE", "4" },
	{ "int", "FC_SCRIPTFUNCTION", "0" },
	{ "int", "FC_SCRIPTFUNCTIONOBJECT", "1" },
	{ "int", "FC_EVENTFUNCTION", "2" },
	{ "int", "FC_SOUND", "3" },
	{ "int", "FC_SOUND_VOICE", "4" },
	{ "int", "FC_SOUND_VOICE2", "5" },
	{ "int", "FC_SOUND_BODY", "6" },
	{ "int", "FC_SOUND_BODY2", "7" },
	{ "int", "FC_SOUND_BODY3", "8" },
	{ "int", "FC_SOUND_WEAPON", "9" },
	{ "int", "FC_SOUND_ITEM", "10" },
	{ "int", "FC_SOUND_GLOBAL", "11" },
	{ "int", "FC_SOUND_CHATTER", "12" },
	{ "int", "FC_SKIN", "13" },
	{ "int", "FC_TRIGGER", "14" },
	{ "int", "FC_TRIGGER_SMOKE_PARTICLE", "15" },
	{ "int", "FC_MELEE", "16" },
	{ "int", "FC_DIRECTDAMAGE", "17" },
	{ "int", "FC_BEGINATTACK", "18" },
	{ "int", "FC_ENDATTACK", "19" },
	{ "int", "FC_MUZZLEFLASH", "20" },
	{ "int", "FC_CREATEMISSILE", "21" },
	{ "int", "FC_LAUNCHMISSILE", "22" },
	{ "int", "FC_FIREMISSILEATTARGET", "23" },
	{ "int", "FC_FOOTSTEP", "24" },
	{ "int", "FC_LEFTFOOT", "25" },
	{ "int", "FC_RIGHTFOOT", "26" },
	{ "int", "FC_ENABLE_EYE_FOCUS", "27" },
	{ "int", "FC_DISABLE_EYE_FOCUS", "28" },
	{ "int", "FC_FX", "29" },
	{ "int", "FC_DISABLE_GRAVITY", "30" },
	{ "int", "FC_ENABLE_GRAVITY", "31" },
	{ "int", "FC_JUMP", "32" },
	{ "int", "FC_ENABLE_CLIP", "33" },
	{ "int", "FC_DISABLE_CLIP", "34" },
	{ "int", "FC_ENABLE_WALK_IK", "35" },
	{ "int", "FC_DISABLE_WALK_IK", "36" },
	{ "int", "FC_ENABLE_LEG_IK", "37" },
	{ "int", "FC_DISABLE_LEG_IK", "38" },
	{ "int", "FC_RECORDDEMO", "39" },
	{ "int", "FC_AVIGAME", "40" },
	{ "int", "FC_LAUNCH_PROJECTILE", "41" },
	{ "int", "FC_TRIGGER_FX", "42" },
	{ "int", "FC_START_EMITTER", "43" },
	{ "int", "FC_STOP_EMITTER", "44" },
	{ "int", "AF_JOINTMOD_AXIS", "0" },
	{ "int", "AF_JOINTMOD_ORIGIN", "1" },
	{ "int", "AF_JOINTMOD_BOTH", "2" },
	{ "int", "PATHTYPE_WALK", "0" },
	{ "int", "PATHTYPE_WALKOFFLEDGE", "1" },
	{ "int", "PATHTYPE_BARRIERJUMP", "2" },
	{ "int", "PATHTYPE_JUMP", "3" },
	{ "int", "PVS_NORMAL", "0" },
	{ "int", "PVS_ALL_PORTALS_OPEN", "1" },
	{ "int", "PVS_CONNECTED_AREAS", "2" },
	{ "int", "GAME_SP", "-2" },
	{ "int", "GAME_RANDOM", "-1" },
	{ "int", "GAME_DM", "0" },
	{ "int", "GAME_TOURNEY", "1" },
	{ "int", "GAME_TDM", "2" },
	{ "int", "GAME_LASTMAN", "3" },
	{ "int", "GAME_CTF", "4" },
	{ "int", "GAME_COUNT", "5" },
	{ "int", "FLAGSTATUS_INBASE", "0" },
	{ "int", "FLAGSTATUS_TAKEN", "1" },
	{ "int", "FLAGSTATUS_STRAY", "2" },
	{ "int", "FLAGSTATUS_NONE", "3" },
	{ "int", "LEAD_STATUS_NOTSET", "0" },
	{ "int", "LEAD_STATUS_NOLEAD", "1" },
	{ "int", "LEAD_STATUS_INLEAD", "2" },
	{ "const int", "NUM_CHAT_NOTIFY", "5" },
	{ "const int", "CHAT_FADE_TIME", "400" },
	{ "const int", "FRAGLIMIT_DELAY", "2000" },
	{ "const int", "MP_PLAYER_MINFRAGS", "-100" },
	{ "const int", "MP_PLAYER_MAXFRAGS", "400" },
	{ "const int", "MP_PLAYER_MAXWINS", "100" },
	{ "const int", "MP_PLAYER_MAXPING", "999" },
	{ "const int", "MP_CTF_MAXPOINTS", "400" },
	{ "int", "SND_YOUWIN", "0" },
	{ "int", "SND_YOULOSE", "1" },
	{ "int", "SND_FIGHT", "2" },
	{ "int", "SND_THREE", "3" },
	{ "int", "SND_TWO", "4" },
	{ "int", "SND_ONE", "5" },
	{ "int", "SND_SUDDENDEATH", "6" },
	{ "int", "SND_FLAG_CAPTURED_YOURS", "7" },
	{ "int", "SND_FLAG_CAPTURED_THEIRS", "8" },
	{ "int", "SND_FLAG_RETURN", "9" },
	{ "int", "SND_FLAG_TAKEN_YOURS", "10" },
	{ "int", "SND_FLAG_TAKEN_THEIRS", "11" },
	{ "int", "SND_FLAG_DROPPED_YOURS", "12" },
	{ "int", "SND_FLAG_DROPPED_THEIRS", "13" },
	{ "int", "SND_LEADGAINED", "14" },
	{ "int", "SND_LEADLOST", "15" },
	{ "int", "SND_LEADTIED", "16" },
	{ "int", "SND_WELCOMEDOM", "17" },
	{ "int", "SND_ONEFRAG", "18" },
	{ "int", "SND_TWOFRAG", "19" },
	{ "int", "SND_THREEFRAG", "20" },
	{ "int", "SND_PREPAREFORBATTLE", "21" },
	{ "int", "SND_COUNT", "22" },
	{ "int", "idMultiplayerGame::INACTIVE", "0" },
	{ "int", "idMultiplayerGame::WARMUP", "1" },
	{ "int", "idMultiplayerGame::COUNTDOWN", "2" },
	{ "int", "idMultiplayerGame::GAMEON", "3" },
	{ "int", "idMultiplayerGame::SUDDENDEATH", "4" },
	{ "int", "idMultiplayerGame::GAMEREVIEW", "5" },
	{ "int", "idMultiplayerGame::NEXTGAME", "6" },
	{ "int", "idMultiplayerGame::STATE_COUNT", "7" },
	{ "int", "idMultiplayerGame::MSG_SUICIDE", "0" },
	{ "int", "idMultiplayerGame::MSG_KILLED", "1" },
	{ "int", "idMultiplayerGame::MSG_KILLEDTEAM", "2" },
	{ "int", "idMultiplayerGame::MSG_DIED", "3" },
	{ "int", "idMultiplayerGame::MSG_SUDDENDEATH", "4" },
	{ "int", "idMultiplayerGame::MSG_JOINEDSPEC", "5" },
	{ "int", "idMultiplayerGame::MSG_TIMELIMIT", "6" },
	{ "int", "idMultiplayerGame::MSG_FRAGLIMIT", "7" },
	{ "int", "idMultiplayerGame::MSG_TELEFRAGGED", "8" },
	{ "int", "idMultiplayerGame::MSG_JOINTEAM", "9" },
	{ "int", "idMultiplayerGame::MSG_HOLYSHIT", "10" },
	{ "int", "idMultiplayerGame::MSG_POINTLIMIT", "11" },
	{ "int", "idMultiplayerGame::MSG_FLAGTAKEN", "12" },
	{ "int", "idMultiplayerGame::MSG_FLAGDROP", "13" },
	{ "int", "idMultiplayerGame::MSG_FLAGRETURN", "14" },
	{ "int", "idMultiplayerGame::MSG_FLAGCAPTURE", "15" },
	{ "int", "idMultiplayerGame::MSG_SCOREUPDATE", "16" },
	{ "int", "idMultiplayerGame::MSG_LEFTGAME", "17" },
	{ "int", "idMultiplayerGame::MSG_COUNT", "18" },
	{ "const int", "MAX_GAME_MESSAGE_SIZE", "8192" },
	{ "const int", "MAX_ENTITY_STATE_SIZE", "512" },
	{ "const int", "ENTITY_PVS_SIZE", "((4096+31)>>5)" },
	{ "const int", "NUM_RENDER_PORTAL_BITS", "0(0)" },
	{ "const int", "MAX_EVENT_PARAM_SIZE", "128" },
	{ "int", "GAME_RELIABLE_MESSAGE_SYNCEDCVARS", "0" },
	{ "int", "GAME_RELIABLE_MESSAGE_SPAWN_PLAYER", "1" },
	{ "int", "GAME_RELIABLE_MESSAGE_CHAT", "2" },
	{ "int", "GAME_RELIABLE_MESSAGE_TCHAT", "3" },
	{ "int", "GAME_RELIABLE_MESSAGE_SOUND_EVENT", "4" },
	{ "int", "GAME_RELIABLE_MESSAGE_SOUND_INDEX", "5" },
	{ "int", "GAME_RELIABLE_MESSAGE_DB", "6" },
	{ "int", "GAME_RELIABLE_MESSAGE_DROPWEAPON", "7" },
	{ "int", "GAME_RELIABLE_MESSAGE_RESTART", "8" },
	{ "int", "GAME_RELIABLE_MESSAGE_TOURNEYLINE", "9" },
	{ "int", "GAME_RELIABLE_MESSAGE_VCHAT", "10" },
	{ "int", "GAME_RELIABLE_MESSAGE_STARTSTATE", "11" },
	{ "int", "GAME_RELIABLE_MESSAGE_WARMUPTIME", "12" },
	{ "int", "GAME_RELIABLE_MESSAGE_SPECTATE", "13" },
	{ "int", "GAME_RELIABLE_MESSAGE_EVENT", "14" },
	{ "int", "GAME_RELIABLE_MESSAGE_LOBBY_COUNTDOWN", "15" },
	{ "int", "GAME_RELIABLE_MESSAGE_RESPAWN_AVAILABLE", "16" },
	{ "int", "GAME_RELIABLE_MESSAGE_MATCH_STARTED_TIME", "17" },
	{ "int", "GAME_RELIABLE_MESSAGE_ACHIEVEMENT_UNLOCK", "18" },
	{ "int", "GAME_RELIABLE_MESSAGE_CLIENT_HITSCAN_HIT", "19" },
	{ "int", "GAMESTATE_UNINITIALIZED", "0" },
	{ "int", "GAMESTATE_NOMAP", "1" },
	{ "int", "GAMESTATE_STARTUP", "2" },
	{ "int", "GAMESTATE_ACTIVE", "3" },
	{ "int", "GAMESTATE_SHUTDOWN", "4" },
	{ "int", "idEventQueue::OUTOFORDER_IGNORE", "0" },
	{ "int", "idEventQueue::OUTOFORDER_DROP", "1" },
	{ "int", "idEventQueue::OUTOFORDER_SORT", "2" },
	{ "int", "SLOWMO_STATE_OFF", "0" },
	{ "int", "SLOWMO_STATE_RAMPUP", "1" },
	{ "int", "SLOWMO_STATE_ON", "2" },
	{ "int", "SLOWMO_STATE_RAMPDOWN", "3" },
	{ "int", "idGameLocal::GCMD_UNKNOWN", "0" },
	{ "int", "idGameLocal::GCMD_GAMETIME", "1" },
	{ ": const static int", "idGameLocal::INITIAL_SPAWN_COUNT", "1" },
	{ "int", "SND_CHANNEL_ANY", "0" },
	{ "int", "SND_CHANNEL_VOICE", "0" },
	{ "int", "SND_CHANNEL_VOICE2", "1" },
	{ "int", "SND_CHANNEL_BODY", "2" },
	{ "int", "SND_CHANNEL_BODY2", "3" },
	{ "int", "SND_CHANNEL_BODY3", "4" },
	{ "int", "SND_CHANNEL_WEAPON", "5" },
	{ "int", "SND_CHANNEL_ITEM", "6" },
	{ "int", "SND_CHANNEL_HEART", "7" },
	{ "int", "SND_CHANNEL_PDA_AUDIO", "8" },
	{ "int", "SND_CHANNEL_PDA_VIDEO", "9" },
	{ "int", "SND_CHANNEL_DEMONIC", "10" },
	{ "int", "SND_CHANNEL_RADIO", "11" },
	{ "int", "SND_CHANNEL_AMBIENT", "12" },
	{ "int", "SND_CHANNEL_DAMAGE", "13" },
	{ "int", "SND_CHANNEL_MUSIC", "14" },
	{ "const float", "DEFAULT_GRAVITY", "1066.0" },
	{ "const int", "CINEMATIC_SKIP_DELAY", "0(2.0)" },
	{ "int", "FORCEFIELD_UNIFORM", "0" },
	{ "int", "FORCEFIELD_EXPLOSION", "1" },
	{ "int", "FORCEFIELD_IMPLOSION", "2" },
	{ "int", "FORCEFIELD_APPLY_FORCE", "0" },
	{ "int", "FORCEFIELD_APPLY_VELOCITY", "1" },
	{ "int", "FORCEFIELD_APPLY_IMPULSE", "2" },
	{ "int", "MM_OK", "0" },
	{ "int", "MM_SLIDING", "1" },
	{ "int", "MM_BLOCKED", "2" },
	{ "int", "MM_STEPPED", "3" },
	{ "int", "MM_FALLING", "4" },
	{ "int", "PM_NORMAL", "0" },
	{ "int", "PM_DEAD", "1" },
	{ "int", "PM_SPECTATOR", "2" },
	{ "int", "PM_FREEZE", "3" },
	{ "int", "PM_NOCLIP", "4" },
	{ "int", "WATERLEVEL_NONE", "0" },
	{ "int", "WATERLEVEL_FEET", "1" },
	{ "int", "WATERLEVEL_WAIST", "2" },
	{ "int", "WATERLEVEL_HEAD", "3" },
	{ "int", "CONSTRAINT_INVALID", "0" },
	{ "int", "CONSTRAINT_FIXED", "1" },
	{ "int", "CONSTRAINT_BALLANDSOCKETJOINT", "2" },
	{ "int", "CONSTRAINT_UNIVERSALJOINT", "3" },
	{ "int", "CONSTRAINT_HINGE", "4" },
	{ "int", "CONSTRAINT_HINGESTEERING", "5" },
	{ "int", "CONSTRAINT_SLIDER", "6" },
	{ "int", "CONSTRAINT_CYLINDRICALJOINT", "7" },
	{ "int", "CONSTRAINT_LINE", "8" },
	{ "int", "CONSTRAINT_PLANE", "9" },
	{ "int", "CONSTRAINT_SPRING", "10" },
	{ "int", "CONSTRAINT_CONTACT", "11" },
	{ "int", "CONSTRAINT_FRICTION", "12" },
	{ "int", "CONSTRAINT_CONELIMIT", "13" },
	{ "int", "CONSTRAINT_PYRAMIDLIMIT", "14" },
	{ "int", "CONSTRAINT_SUSPENSION", "15" },
	{ "static const int", "idSmokeParticles::MAX_SMOKE_PARTICLES", "10000" },
	{ "static const int", "DELAY_DORMANT_TIME", "3000" },
	{ "int", "TH_ALL", "-1" },
	{ "int", "TH_THINK", "1" },
	{ "int", "TH_PHYSICS", "2" },
	{ "int", "TH_ANIMATE", "4" },
	{ "int", "TH_UPDATEVISUALS", "8" },
	{ "int", "TH_UPDATEPARTICLES", "16" },
	{ "int", "SIG_TOUCH", "0" },
	{ "int", "SIG_USE", "1" },
	{ "int", "SIG_TRIGGER", "2" },
	{ "int", "SIG_REMOVED", "3" },
	{ "int", "SIG_DAMAGE", "4" },
	{ "int", "SIG_BLOCKED", "5" },
	{ "int", "SIG_MOVER_POS1", "6" },
	{ "int", "SIG_MOVER_POS2", "7" },
	{ "int", "SIG_MOVER_1TO2", "8" },
	{ "int", "SIG_MOVER_2TO1", "9" },
	{ "int", "NUM_SIGNALS", "10" },
	{ ": static const int", "idEntity::MAX_PVS_AREAS", "4" },
	{ "static const uint32", "idEntity::INVALID_PREDICTION_KEY", "0xFFFFFFFF" },
	{ "int", "idEntity::EVENT_STARTSOUNDSHADER", "0" },
	{ "int", "idEntity::EVENT_STOPSOUNDSHADER", "1" },
	{ "int", "idEntity::EVENT_MAXEVENTS", "2" },
	{ "int", "idEntity::USE_NO_INTERPOLATION", "0" },
	{ "int", "idEntity::USE_LATEST_SNAP_ONLY", "1" },
	{ "int", "idEntity::USE_INTERPOLATION", "2" },
	{ "int", "idAnimatedEntity::EVENT_ADD_DAMAGE_EFFECT", "2" },
	{ "int", "idAnimatedEntity::EVENT_MAXEVENTS", "3" },
	{ ": static const int", "idIK_Walk::MAX_LEGS", "8" },
	{ ": static const int", "idIK_Reach::MAX_ARMS", "2" },
	{ "const int", "GIB_DELAY", "200" },
	{ "int", "idPlayerStart::EVENT_TELEPORTPLAYER", "2" },
	{ "int", "idPlayerStart::EVENT_MAXEVENTS", "3" },
	{ "int", "idProjectile::EVENT_DAMAGE_EFFECT", "2" },
	{ "int", "idProjectile::EVENT_MAXEVENTS", "3" },
	{ "static const int", "idProjectile::MAX_SIMULATED_PROJECTILES", "64" },
	{ "int", "idProjectile::SPAWNED", "0" },
	{ "int", "idProjectile::CREATED", "1" },
	{ "int", "idProjectile::LAUNCHED", "2" },
	{ "int", "idProjectile::FIZZLED", "3" },
	{ "int", "idProjectile::EXPLODED", "4" },
	{ "static const int", "AMMO_NUMTYPES", "16" },
	{ "static const int", "LIGHTID_WORLD_MUZZLE_FLASH", "1" },
	{ "static const int", "LIGHTID_VIEW_MUZZLE_FLASH", "100" },
	{ "int", "idWeapon::EVENT_RELOAD", "2" },
	{ "int", "idWeapon::EVENT_ENDRELOAD", "3" },
	{ "int", "idWeapon::EVENT_CHANGESKIN", "4" },
	{ "int", "idWeapon::EVENT_MAXEVENTS", "5" },
	{ "int", "idLight::EVENT_BECOMEBROKEN", "2" },
	{ "int", "idLight::EVENT_MAXEVENTS", "3" },
	{ "int", "EnvironmentProbe::EVENT_BECOMEBROKEN", "2" },
	{ "int", "EnvironmentProbe::EVENT_MAXEVENTS", "3" },
	{ "int", "ITEM_GIVE_FEEDBACK", "0" },
	{ "int", "ITEM_GIVE_UPDATE_STATE", "0" },
	{ "int", "ITEM_GIVE_FROM_WEAPON", "0" },
	{ "int", "idItem::EVENT_PICKUP", "2" },
	{ "int", "idItem::EVENT_RESPAWN", "3" },
	{ "int", "idItem::EVENT_RESPAWNFX", "4" },
	{ "int", "idItem::EVENT_TAKEFLAG", "5" },
	{ "int", "idItem::EVENT_DROPFLAG", "6" },
	{ "int", "idItem::EVENT_FLAGRETURN", "7" },
	{ "int", "idItem::EVENT_FLAGCAPTURE", "8" },
	{ "int", "idItem::EVENT_MAXEVENTS", "9" },
	{ "int", "FxFader::FX_STATE_OFF", "0" },
	{ "int", "FxFader::FX_STATE_RAMPUP", "1" },
	{ "int", "FxFader::FX_STATE_RAMPDOWN", "2" },
	{ "int", "FxFader::FX_STATE_ON", "3" },
	{ "int", "ICON_LAG", "0" },
	{ "int", "ICON_CHAT", "1" },
	{ "int", "ICON_TEAM_RED", "2" },
	{ "int", "ICON_TEAM_BLUE", "3" },
	{ "int", "ICON_NONE", "4" },
	{ "int", "ACHIEVEMENT_INVALID", "-1" },
	{ "int", "ACHIEVEMENT_EARN_ALL_50_TROPHIES", "0" },
	{ "int", "ACHIEVEMENT_COMPLETED_DIFFICULTY_0", "1" },
	{ "int", "ACHIEVEMENT_COMPLETED_DIFFICULTY_1", "2" },
	{ "int", "ACHIEVEMENT_COMPLETED_DIFFICULTY_2", "3" },
	{ "int", "ACHIEVEMENT_COMPLETED_DIFFICULTY_3", "4" },
	{ "int", "ACHIEVEMENT_PDAS_BASE", "5" },
	{ "int", "ACHIEVEMENT_WATCH_ALL_VIDEOS", "6" },
	{ "int", "ACHIEVEMENT_KILL_MONSTER_WITH_1_HEALTH_LEFT", "7" },
	{ "int", "ACHIEVEMENT_OPEN_ALL_LOCKERS", "8" },
	{ "int", "ACHIEVEMENT_KILL_20_ENEMY_FISTS_HANDS", "9" },
	{ "int", "ACHIEVEMENT_KILL_SCI_NEXT_TO_RCR", "10" },
	{ "int", "ACHIEVEMENT_KILL_TWO_IMPS_ONE_SHOTGUN", "11" },
	{ "int", "ACHIEVEMENT_SCORE_25000_TURKEY_PUNCHER", "12" },
	{ "int", "ACHIEVEMENT_DESTROY_BARRELS", "13" },
	{ "int", "ACHIEVEMENT_GET_BFG_FROM_SECURITY_OFFICE", "14" },
	{ "int", "ACHIEVEMENT_COMPLETE_LEVEL_WITHOUT_TAKING_DMG", "15" },
	{ "int", "ACHIEVEMENT_FIND_RAGE_LOGO", "16" },
	{ "int", "ACHIEVEMENT_SPEED_RUN", "17" },
	{ "int", "ACHIEVEMENT_DEFEAT_VAGARY_BOSS", "18" },
	{ "int", "ACHIEVEMENT_DEFEAT_GUARDIAN_BOSS", "19" },
	{ "int", "ACHIEVEMENT_DEFEAT_SABAOTH_BOSS", "20" },
	{ "int", "ACHIEVEMENT_DEFEAT_CYBERDEMON_BOSS", "21" },
	{ "int", "ACHIEVEMENT_SENTRY_BOT_ALIVE_TO_DEST", "22" },
	{ "int", "ACHIEVEMENT_KILL_20_ENEMY_WITH_CHAINSAW", "23" },
	{ "int", "ACHIEVEMENT_ID_LOGO_SECRET_ROOM", "24" },
	{ "int", "ACHIEVEMENT_BLOODY_HANDWORK_OF_BETRUGER", "25" },
	{ "int", "ACHIEVEMENT_TWO_DEMONS_FIGHT_EACH_OTHER", "26" },
	{ "int", "ACHIEVEMENT_USE_SOUL_CUBE_TO_DEFEAT_20_ENEMY", "27" },
	{ "int", "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_0", "28" },
	{ "int", "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_1", "29" },
	{ "int", "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_2", "30" },
	{ "int", "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_3", "31" },
	{ "int", "ACHIEVEMENT_PDAS_ROE", "32" },
	{ "int", "ACHIEVEMENT_KILL_5_ENEMY_HELL_TIME", "33" },
	{ "int", "ACHIEVEMENT_DEFEAT_HELLTIME_HUNTER", "34" },
	{ "int", "ACHIEVEMENT_DEFEAT_BERSERK_HUNTER", "35" },
	{ "int", "ACHIEVEMENT_DEFEAT_INVULNERABILITY_HUNTER", "36" },
	{ "int", "ACHIEVEMENT_DEFEAT_MALEDICT_BOSS", "37" },
	{ "int", "ACHIEVEMENT_GRABBER_KILL_20_ENEMY", "38" },
	{ "int", "ACHIEVEMENT_ARTIFACT_WITH_BERSERK_PUNCH_20", "39" },
	{ "int", "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_0", "40" },
	{ "int", "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_1", "41" },
	{ "int", "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_2", "42" },
	{ "int", "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_3", "43" },
	{ "int", "ACHIEVEMENT_PDAS_LE", "44" },
	{ "int", "ACHIEVEMENT_MP_KILL_PLAYER_VIA_TELEPORT", "45" },
	{ "int", "ACHIEVEMENT_MP_CATCH_ENEMY_IN_ROFC", "46" },
	{ "int", "ACHIEVEMENT_MP_KILL_5_PLAYERS_USING_INVIS", "47" },
	{ "int", "ACHIEVEMENT_MP_COMPLETE_MATCH_WITHOUT_DYING", "48" },
	{ "int", "ACHIEVEMENT_MP_USE_BERSERK_TO_KILL_PLAYER", "49" },
	{ "int", "ACHIEVEMENT_MP_KILL_2_GUYS_IN_ROOM_WITH_BFG", "50" },
	{ "int", "ACHIEVEMENT_DOOM1_NEOPHYTE_COMPLETE_ANY_LEVEL", "51" },
	{ "int", "ACHIEVEMENT_DOOM1_EPISODE1_COMPLETE_MEDIUM", "52" },
	{ "int", "ACHIEVEMENT_DOOM1_EPISODE2_COMPLETE_MEDIUM", "53" },
	{ "int", "ACHIEVEMENT_DOOM1_EPISODE3_COMPLETE_MEDIUM", "54" },
	{ "int", "ACHIEVEMENT_DOOM1_EPISODE4_COMPLETE_MEDIUM", "55" },
	{ "int", "ACHIEVEMENT_DOOM1_RAMPAGE_COMPLETE_ALL_HARD", "56" },
	{ "int", "ACHIEVEMENT_DOOM1_NIGHTMARE_COMPLETE_ANY_LEVEL_NIGHTMARE", "57" },
	{ "int", "ACHIEVEMENT_DOOM1_BURNING_OUT_OF_CONTROL_COMPLETE_KILLS_ITEMS_SECRETS", "58" },
	{ "int", "ACHIEVEMENT_DOOM2_JUST_GETTING_STARTED_COMPLETE_ANY_LEVEL", "59" },
	{ "int", "ACHIEVEMENT_DOOM2_FROM_EARTH_TO_HELL_COMPLETE_HELL_ON_EARTH", "60" },
	{ "int", "ACHIEVEMENT_DOOM2_AND_BACK_AGAIN_COMPLETE_NO_REST", "61" },
	{ "int", "ACHIEVEMENT_DOOM2_SUPERIOR_FIREPOWER_COMPLETE_ALL_HARD", "62" },
	{ "int", "ACHIEVEMENT_DOOM2_REALLY_BIG_GUN_FIND_BFG_SINGLEPLAYER", "63" },
	{ "int", "ACHIEVEMENT_DOOM2_BURNING_OUT_OF_CONTROL_COMPLETE_KILLS_ITEMS_SECRETS", "64" },
	{ "int", "ACHIEVEMENT_DOOM2_IMPORTANT_LOOKING_DOOR_FIND_ANY_SECRET", "65" },
	{ "int", "ACHIEVEMENTS_NUM", "66" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_1_MEDIUM", "67" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_2_MEDIUM", "68" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_3_MEDIUM", "69" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_4_MEDIUM", "70" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_1_HARD", "71" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_2_HARD", "72" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_3_HARD", "73" },
	{ "int", "STAT_DOOM_COMPLETED_EPISODE_4_HARD", "74" },
	{ "const float", "THIRD_PERSON_FOCUS_DISTANCE", "512.0" },
	{ "const int", "LAND_DEFLECT_TIME", "150" },
	{ "const int", "LAND_RETURN_TIME", "300" },
	{ "const int", "FOCUS_TIME", "300" },
	{ "const int", "FOCUS_GUI_TIME", "500" },
	{ "const int", "NUM_QUICK_SLOTS", "4" },
	{ "const int", "MAX_WEAPONS", "32" },
	{ "const int", "DEAD_HEARTRATE", "0" },
	{ "const int", "LOWHEALTH_HEARTRATE_ADJ", "20" },
	{ "const int", "DYING_HEARTRATE", "30" },
	{ "const int", "BASE_HEARTRATE", "70" },
	{ "const int", "ZEROSTAMINA_HEARTRATE", "115" },
	{ "const int", "MAX_HEARTRATE", "130" },
	{ "const int", "ZERO_VOLUME", "-40" },
	{ "const int", "DMG_VOLUME", "5" },
	{ "const int", "DEATH_VOLUME", "15" },
	{ "const int", "SAVING_THROW_TIME", "5000" },
	{ "const int", "ASYNC_PLAYER_INV_AMMO_BITS", "0(3000)" },
	{ "const int", "ASYNC_PLAYER_INV_CLIP_BITS", "-7" },
	{ "int", "GAME_BASE", "0" },
	{ "int", "GAME_D3XP", "1" },
	{ "int", "GAME_D3LE", "2" },
	{ "int", "GAME_UNKNOWN", "3" },
	{ "int", "BERSERK", "0" },
	{ "int", "INVISIBILITY", "1" },
	{ "int", "MEGAHEALTH", "2" },
	{ "int", "ADRENALINE", "3" },
	{ "int", "INVULNERABILITY", "4" },
	{ "int", "HELLTIME", "5" },
	{ "int", "ENVIROSUIT", "6" },
	{ "int", "ENVIROTIME", "7" },
	{ "int", "MAX_POWERUPS", "8" },
	{ "int", "SPEED", "0" },
	{ "int", "PROJECTILE_DAMAGE", "1" },
	{ "int", "MELEE_DAMAGE", "2" },
	{ "int", "MELEE_DISTANCE", "3" },
	{ "int", "INFLUENCE_NONE", "0" },
	{ "int", "INFLUENCE_LEVEL1", "1" },
	{ "int", "INFLUENCE_LEVEL2", "2" },
	{ "int", "INFLUENCE_LEVEL3", "3" },
	{ "int", "idPlayer::EVENT_IMPULSE", "2" },
	{ "int", "idPlayer::EVENT_EXIT_TELEPORTER", "3" },
	{ "int", "idPlayer::EVENT_ABORT_TELEPORTER", "4" },
	{ "int", "idPlayer::EVENT_POWERUP", "5" },
	{ "int", "idPlayer::EVENT_SPECTATE", "6" },
	{ "int", "idPlayer::EVENT_PICKUPNAME", "7" },
	{ "int", "idPlayer::EVENT_FORCE_ORIGIN", "8" },
	{ "int", "idPlayer::EVENT_KNOCKBACK", "9" },
	{ "int", "idPlayer::EVENT_MAXEVENTS", "10" },
	{ "static const int", "idPlayer::MAX_PLAYER_PDA", "100" },
	{ "static const int", "idPlayer::MAX_PLAYER_VIDEO", "100" },
	{ "static const int", "idPlayer::MAX_PLAYER_AUDIO", "100" },
	{ "static const int", "idPlayer::MAX_PLAYER_AUDIO_ENTRIES", "2" },
	{ "const static int", "idPlayer::MAX_SHAKE_BUFFER", "3" },
	{ "static const int", "idPlayer::NUM_LOGGED_VIEW_ANGLES", "64" },
	{ "static const int", "idPlayer::NUM_LOGGED_ACCELS", "16" },
	{ "int", "idMover::ACCELERATION_STAGE", "0" },
	{ "int", "idMover::LINEAR_STAGE", "1" },
	{ "int", "idMover::DECELERATION_STAGE", "2" },
	{ "int", "idMover::FINISHED_STAGE", "3" },
	{ "int", "idMover::MOVER_NONE", "0" },
	{ "int", "idMover::MOVER_ROTATING", "1" },
	{ "int", "idMover::MOVER_MOVING", "2" },
	{ "int", "idMover::MOVER_SPLINE", "3" },
	{ "int", "idMover::DIR_UP", "-1" },
	{ "int", "idMover::DIR_DOWN", "-2" },
	{ "int", "idMover::DIR_LEFT", "-3" },
	{ "int", "idMover::DIR_RIGHT", "-4" },
	{ "int", "idMover::DIR_FORWARD", "-5" },
	{ "int", "idMover::DIR_BACK", "-6" },
	{ "int", "idMover::DIR_REL_UP", "-7" },
	{ "int", "idMover::DIR_REL_DOWN", "-8" },
	{ "int", "idMover::DIR_REL_LEFT", "-9" },
	{ "int", "idMover::DIR_REL_RIGHT", "-10" },
	{ "int", "idMover::DIR_REL_FORWARD", "-11" },
	{ "int", "idMover::DIR_REL_BACK", "-12" },
	{ "int", "idElevator::INIT", "0" },
	{ "int", "idElevator::IDLE", "1" },
	{ "int", "idElevator::WAITING_ON_DOORS", "2" },
	{ "int", "MOVER_POS1", "0" },
	{ "int", "MOVER_POS2", "1" },
	{ "int", "MOVER_1TO2", "2" },
	{ "int", "MOVER_2TO1", "3" },
	{ "int", "idExplodingBarrel::EVENT_EXPLODE", "2" },
	{ "int", "idExplodingBarrel::EVENT_MAXEVENTS", "3" },
	{ "int", "idExplodingBarrel::NORMAL", "0" },
	{ "int", "idExplodingBarrel::BURNING", "1" },
	{ "int", "idExplodingBarrel::BURNEXPIRED", "2" },
	{ "int", "idExplodingBarrel::EXPLODING", "3" },
	{ "int", "idSecurityCamera::SCANNING", "0" },
	{ "int", "idSecurityCamera::LOSINGINTEREST", "1" },
	{ "int", "idSecurityCamera::ALERT", "2" },
	{ "int", "idSecurityCamera::ACTIVATED", "3" },
	{ "int", "idBrittleFracture::EVENT_PROJECT_DECAL", "2" },
	{ "int", "idBrittleFracture::EVENT_SHATTER", "3" },
	{ "int", "idBrittleFracture::EVENT_MAXEVENTS", "4" },
	{ "const float", "SQUARE_ROOT_OF_2", "1.414213562" },
	{ "const float", "AI_TURN_PREDICTION", "0.2" },
	{ "const float", "AI_TURN_SCALE", "60.0" },
	{ "const float", "AI_SEEK_PREDICTION", "0.3" },
	{ "const float", "AI_FLY_DAMPENING", "0.15" },
	{ "const float", "AI_HEARING_RANGE", "2048.0" },
	{ "const int", "DEFAULT_FLY_OFFSET", "68" },
	{ "int", "MOVETYPE_DEAD", "0" },
	{ "int", "MOVETYPE_ANIM", "1" },
	{ "int", "MOVETYPE_SLIDE", "2" },
	{ "int", "MOVETYPE_FLY", "3" },
	{ "int", "MOVETYPE_STATIC", "4" },
	{ "int", "NUM_MOVETYPES", "5" },
	{ "int", "MOVE_NONE", "0" },
	{ "int", "MOVE_FACE_ENEMY", "1" },
	{ "int", "MOVE_FACE_ENTITY", "2" },
	{ "int", "NUM_NONMOVING_COMMANDS", "3" },
	{ "int", "MOVE_TO_ENEMY", "3" },
	{ "int", "MOVE_TO_ENEMYHEIGHT", "4" },
	{ "int", "MOVE_TO_ENTITY", "5" },
	{ "int", "MOVE_OUT_OF_RANGE", "6" },
	{ "int", "MOVE_TO_ATTACK_POSITION", "7" },
	{ "int", "MOVE_TO_COVER", "8" },
	{ "int", "MOVE_TO_POSITION", "9" },
	{ "int", "MOVE_TO_POSITION_DIRECT", "10" },
	{ "int", "MOVE_SLIDE_TO_POSITION", "11" },
	{ "int", "MOVE_WANDER", "12" },
	{ "int", "NUM_MOVE_COMMANDS", "13" },
	{ "int", "TALK_NEVER", "0" },
	{ "int", "TALK_DEAD", "1" },
	{ "int", "TALK_OK", "2" },
	{ "int", "TALK_BUSY", "3" },
	{ "int", "NUM_TALK_STATES", "4" },
	{ "int", "MOVE_STATUS_DONE", "0" },
	{ "int", "MOVE_STATUS_MOVING", "1" },
	{ "int", "MOVE_STATUS_WAITING", "2" },
	{ "int", "MOVE_STATUS_DEST_NOT_FOUND", "3" },
	{ "int", "MOVE_STATUS_DEST_UNREACHABLE", "4" },
	{ "int", "MOVE_STATUS_BLOCKED_BY_WALL", "5" },
	{ "int", "MOVE_STATUS_BLOCKED_BY_OBJECT", "6" },
	{ "int", "MOVE_STATUS_BLOCKED_BY_ENEMY", "7" },
	{ "int", "MOVE_STATUS_BLOCKED_BY_MONSTER", "8" },
	{ "int", "SE_BLOCKED", "0" },
	{ "int", "SE_ENTER_LEDGE_AREA", "0" },
	{ "int", "SE_ENTER_OBSTACLE", "0" },
	{ "int", "SE_FALL", "0" },
	{ "int", "SE_LAND", "0" },
	{ "int", "NULLMOVEFLAG", "-1" },
	{ "int", "MOVE_PRONE", "0" },
	{ "int", "MOVE_CROUCH", "1" },
	{ "int", "MOVE_WALK", "2" },
	{ "int", "MOVE_RUN2", "3" },
	{ "int", "MOVE_SPRINT", "4" },
	{ "int", "MOVE_JUMP", "5" },
	{ "int", "KILL", "0" },
	{ "int", "DEATH", "1" },
	{ "int", "OPTION_INVALID", "-1" },
	{ "int", "OPTION_BUTTON_TEXT", "0" },
	{ "int", "OPTION_SLIDER_BAR", "1" },
	{ "int", "OPTION_SLIDER_TEXT", "2" },
	{ "int", "OPTION_SLIDER_TOGGLE", "3" },
	{ "int", "OPTION_BUTTON_INFO", "4" },
	{ "int", "OPTION_BUTTON_FULL_TEXT_SLIDER", "5" },
	{ "int", "MAX_MENU_OPTION_TYPES", "6" },
	{ "int", "WIDGET_EVENT_PRESS", "0" },
	{ "int", "WIDGET_EVENT_RELEASE", "1" },
	{ "int", "WIDGET_EVENT_ROLL_OVER", "2" },
	{ "int", "WIDGET_EVENT_ROLL_OUT", "3" },
	{ "int", "WIDGET_EVENT_FOCUS_ON", "4" },
	{ "int", "WIDGET_EVENT_FOCUS_OFF", "5" },
	{ "int", "WIDGET_EVENT_SCROLL_UP_LSTICK", "6" },
	{ "int", "WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE", "7" },
	{ "int", "WIDGET_EVENT_SCROLL_DOWN_LSTICK", "8" },
	{ "int", "WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE", "9" },
	{ "int", "WIDGET_EVENT_SCROLL_LEFT_LSTICK", "10" },
	{ "int", "WIDGET_EVENT_SCROLL_LEFT_LSTICK_RELEASE", "11" },
	{ "int", "WIDGET_EVENT_SCROLL_RIGHT_LSTICK", "12" },
	{ "int", "WIDGET_EVENT_SCROLL_RIGHT_LSTICK_RELEASE", "13" },
	{ "int", "WIDGET_EVENT_SCROLL_UP_RSTICK", "14" },
	{ "int", "WIDGET_EVENT_SCROLL_UP_RSTICK_RELEASE", "15" },
	{ "int", "WIDGET_EVENT_SCROLL_DOWN_RSTICK", "16" },
	{ "int", "WIDGET_EVENT_SCROLL_DOWN_RSTICK_RELEASE", "17" },
	{ "int", "WIDGET_EVENT_SCROLL_LEFT_RSTICK", "18" },
	{ "int", "WIDGET_EVENT_SCROLL_LEFT_RSTICK_RELEASE", "19" },
	{ "int", "WIDGET_EVENT_SCROLL_RIGHT_RSTICK", "20" },
	{ "int", "WIDGET_EVENT_SCROLL_RIGHT_RSTICK_RELEASE", "21" },
	{ "int", "WIDGET_EVENT_SCROLL_UP", "22" },
	{ "int", "WIDGET_EVENT_SCROLL_UP_RELEASE", "23" },
	{ "int", "WIDGET_EVENT_SCROLL_DOWN", "24" },
	{ "int", "WIDGET_EVENT_SCROLL_DOWN_RELEASE", "25" },
	{ "int", "WIDGET_EVENT_SCROLL_LEFT", "26" },
	{ "int", "WIDGET_EVENT_SCROLL_LEFT_RELEASE", "27" },
	{ "int", "WIDGET_EVENT_SCROLL_RIGHT", "28" },
	{ "int", "WIDGET_EVENT_SCROLL_RIGHT_RELEASE", "29" },
	{ "int", "WIDGET_EVENT_DRAG_START", "30" },
	{ "int", "WIDGET_EVENT_DRAG_STOP", "31" },
	{ "int", "WIDGET_EVENT_SCROLL_PAGEDWN", "32" },
	{ "int", "WIDGET_EVENT_SCROLL_PAGEDWN_RELEASE", "33" },
	{ "int", "WIDGET_EVENT_SCROLL_PAGEUP", "34" },
	{ "int", "WIDGET_EVENT_SCROLL_PAGEUP_RELEASE", "35" },
	{ "int", "WIDGET_EVENT_SCROLL", "36" },
	{ "int", "WIDGET_EVENT_SCROLL_RELEASE", "37" },
	{ "int", "WIDGET_EVENT_BACK", "38" },
	{ "int", "WIDGET_EVENT_COMMAND", "39" },
	{ "int", "WIDGET_EVENT_TAB_NEXT", "40" },
	{ "int", "WIDGET_EVENT_TAB_PREV", "41" },
	{ "int", "MAX_WIDGET_EVENT", "42" },
	{ "int", "SCROLL_SINGLE", "0" },
	{ "int", "SCROLL_PAGE", "1" },
	{ "int", "SCROLL_FULL", "2" },
	{ "int", "SCROLL_TOP", "3" },
	{ "int", "SCROLL_END", "4" },
	{ "int", "WIDGET_ACTION_NONE", "0" },
	{ "int", "WIDGET_ACTION_COMMAND", "1" },
	{ "int", "WIDGET_ACTION_FUNCTION", "2" },
	{ "int", "WIDGET_ACTION_SCROLL_VERTICAL", "3" },
	{ "int", "WIDGET_ACTION_SCROLL_VERTICAL_VARIABLE", "4" },
	{ "int", "WIDGET_ACTION_SCROLL_PAGE", "5" },
	{ "int", "WIDGET_ACTION_SCROLL_HORIZONTAL", "6" },
	{ "int", "WIDGET_ACTION_SCROLL_TAB", "7" },
	{ "int", "WIDGET_ACTION_START_REPEATER", "8" },
	{ "int", "WIDGET_ACTION_STOP_REPEATER", "9" },
	{ "int", "WIDGET_ACTION_ADJUST_FIELD", "10" },
	{ "int", "WIDGET_ACTION_PRESS_FOCUSED", "11" },
	{ "int", "WIDGET_ACTION_JOY3_ON_PRESS", "12" },
	{ "int", "WIDGET_ACTION_JOY4_ON_PRESS", "13" },
	{ "int", "WIDGET_ACTION_GOTO_MENU", "14" },
	{ "int", "WIDGET_ACTION_GO_BACK", "15" },
	{ "int", "WIDGET_ACTION_EXIT_GAME", "16" },
	{ "int", "WIDGET_ACTION_LAUNCH_MULTIPLAYER", "17" },
	{ "int", "WIDGET_ACTION_MENU_BAR_SELECT", "18" },
	{ "int", "WIDGET_ACTION_EMAIL_HOVER", "19" },
	{ "int", "WIDGET_ACTION_PDA_SELECT_USER", "20" },
	{ "int", "WIDGET_ACTION_SELECT_GAMERTAG", "21" },
	{ "int", "WIDGET_ACTION_PDA_SELECT_NAV", "22" },
	{ "int", "WIDGET_ACTION_SELECT_PDA_AUDIO", "23" },
	{ "int", "WIDGET_ACTION_SELECT_PDA_VIDEO", "24" },
	{ "int", "WIDGET_ACTION_SELECT_PDA_ITEM", "25" },
	{ "int", "WIDGET_ACTION_SCROLL_DRAG", "26" },
	{ "int", "WIDGET_ACTION_PDA_SELECT_EMAIL", "27" },
	{ "int", "WIDGET_ACTION_PDA_CLOSE", "28" },
	{ "int", "WIDGET_ACTION_REFRESH", "29" },
	{ "int", "WIDGET_ACTION_MUTE_PLAYER", "30" },
	{ "int", "MAX_WIDGET_ACTION", "31" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER", "0" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER_VARIABLE", "1" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER", "2" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER_VARIABLE", "3" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_LEFT_START_REPEATER", "4" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_RIGHT_START_REPEATER", "5" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_PAGE_DOWN_START_REPEATER", "6" },
	{ "int", "WIDGET_ACTION_EVENT_SCROLL_PAGE_UP_START_REPEATER", "7" },
	{ "int", "WIDGET_ACTION_EVENT_STOP_REPEATER", "8" },
	{ "int", "WIDGET_ACTION_EVENT_TAB_NEXT", "9" },
	{ "int", "WIDGET_ACTION_EVENT_TAB_PREV", "10" },
	{ "int", "WIDGET_ACTION_EVENT_DRAG_START", "11" },
	{ "int", "WIDGET_ACTION_EVENT_DRAG_STOP", "12" },
	{ "int", "WIDGET_ACTION_EVENT_JOY3_ON_PRESS", "13" },
	{ "int", "idMenuWidget::WIDGET_STATE_HIDDEN", "0" },
	{ "int", "idMenuWidget::WIDGET_STATE_NORMAL", "1" },
	{ "int", "idMenuWidget::WIDGET_STATE_SELECTING", "2" },
	{ "int", "idMenuWidget::WIDGET_STATE_SELECTED", "3" },
	{ "int", "idMenuWidget::WIDGET_STATE_DISABLED", "4" },
	{ "int", "idMenuWidget::WIDGET_STATE_MAX", "5" },
	{ "static const int", "idMenuWidget::INVALID_ACTION_INDEX", "-1" },
	{ "int", "idMenuWidget_Button::ANIM_STATE_UP", "0" },
	{ "int", "idMenuWidget_Button::ANIM_STATE_DOWN", "1" },
	{ "int", "idMenuWidget_Button::ANIM_STATE_OVER", "2" },
	{ "int", "idMenuWidget_Button::ANIM_STATE_MAX", "3" },
	{ "int", "idMenuWidget_NavButton::NAV_WIDGET_LEFT", "0" },
	{ "int", "idMenuWidget_NavButton::NAV_WIDGET_RIGHT", "1" },
	{ "int", "idMenuWidget_NavButton::NAV_WIDGET_SELECTED", "2" },
	{ "int", "idMenuWidget_CommandBar::BUTTON_JOY1", "0" },
	{ "int", "idMenuWidget_CommandBar::BUTTON_JOY2", "1" },
	{ "int", "idMenuWidget_CommandBar::BUTTON_JOY3", "2" },
	{ "int", "idMenuWidget_CommandBar::BUTTON_JOY4", "3" },
	{ "int", "idMenuWidget_CommandBar::BUTTON_JOY10", "4" },
	{ "int", "idMenuWidget_CommandBar::BUTTON_TAB", "5" },
	{ "int", "idMenuWidget_CommandBar::MAX_BUTTONS", "6" },
	{ "int", "idMenuWidget_CommandBar::LEFT", "0" },
	{ "int", "idMenuWidget_CommandBar::RIGHT", "1" },
	{ "int", "MENU_TRANSITION_INVALID", "-1" },
	{ "int", "MENU_TRANSITION_SIMPLE", "0" },
	{ "int", "MENU_TRANSITION_ADVANCE", "1" },
	{ "int", "MENU_TRANSITION_BACK", "2" },
	{ "int", "MENU_TRANSITION_FORCE", "3" },
	{ "int", "CURSOR_NONE", "0" },
	{ "int", "CURSOR_IN_COMBAT", "1" },
	{ "int", "CURSOR_TALK", "2" },
	{ "int", "CURSOR_GRABBER", "3" },
	{ "int", "CURSOR_ITEM", "4" },
	{ "int", "LEADERBOARD_FILTER_OVERALL", "0" },
	{ "int", "LEADERBOARD_FILTER_MYSCORE", "1" },
	{ "int", "LEADERBOARD_FILTER_FRIENDS", "2" },
	{ ": static const int", "idLBCache::NUM_ROW_BLOCKS", "5" },
	{ "static const leaderboardFilterMode_t", "idLBCache::DEFAULT_LEADERBOARD_FILTER", "0" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_FOV", "0" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_CHECKPOINTS", "1" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_AUTO_SWITCH", "2" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_AUTO_RELOAD", "3" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_AIM_ASSIST", "4" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_ALWAYS_SPRINT", "5" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_CLASSIC_FLASHLIGHT", "6" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::GAME_FIELD_MUZZLE_FLASHES", "7" },
	{ "int", "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::MAX_GAME_FIELDS", "8" },
	{ "int", "idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings::MATCH_FIELD_MODE", "0" },
	{ "int", "idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings::MATCH_FIELD_MAP", "1" },
	{ "int", "idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings::MATCH_FIELD_TIME", "2" },
	{ "int", "idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings::MATCH_FIELD_SCORE", "3" },
	{ "int", "idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings::MAX_MATCH_FIELDS", "4" },
	{ "int", "idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings::CONTROLS_FIELD_INVERT_MOUSE", "0" },
	{ "int", "idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings::CONTROLS_FIELD_GAMEPAD_ENABLED", "1" },
	{ "int", "idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings::CONTROLS_FIELD_MOUSE_SENS", "2" },
	{ "int", "idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings::MAX_CONTROL_FIELDS", "3" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::GAMEPAD_FIELD_LEFTY", "0" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::GAMEPAD_FIELD_INVERT", "1" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::GAMEPAD_FIELD_VIBRATE", "2" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::GAMEPAD_FIELD_HOR_SENS", "3" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::GAMEPAD_FIELD_VERT_SENS", "4" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::GAMEPAD_FIELD_ACCELERATION", "5" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::GAMEPAD_FIELD_THRESHOLD", "6" },
	{ "int", "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::MAX_GAMEPAD_FIELDS", "7" },
	{ "int", "idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings::LAYOUT_FIELD_LAYOUT", "0" },
	{ "int", "idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings::MAX_LAYOUT_FIELDS", "1" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_RENDERAPI", "0" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_FULLSCREEN", "1" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_FRAMERATE", "2" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_VSYNC", "3" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_ANTIALIASING", "4" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_RENDERMODE", "5" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_AMBIENT_BRIGHTNESS", "6" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_SSAO", "7" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_BLOOD_REFLECTIONS", "8" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_FILMIC_POSTFX", "9" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_CRT_POSTFX", "10" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_BRIGHTNESS", "11" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::SYSTEM_FIELD_VOLUME", "12" },
	{ "int", "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::MAX_SYSTEM_FIELDS", "13" },
	{ "int", "SHELL_AREA_INVALID", "-1" },
	{ "int", "SHELL_AREA_START", "0" },
	{ "int", "SHELL_AREA_ROOT", "1" },
	{ "int", "SHELL_AREA_DEV", "2" },
	{ "int", "SHELL_AREA_CAMPAIGN", "3" },
	{ "int", "SHELL_AREA_LOAD", "4" },
	{ "int", "SHELL_AREA_SAVE", "5" },
	{ "int", "SHELL_AREA_NEW_GAME", "6" },
	{ "int", "SHELL_AREA_GAME_OPTIONS", "7" },
	{ "int", "SHELL_AREA_SYSTEM_OPTIONS", "8" },
	{ "int", "SHELL_AREA_MULTIPLAYER", "9" },
	{ "int", "SHELL_AREA_GAME_LOBBY", "10" },
	{ "int", "SHELL_AREA_PARTY_LOBBY", "11" },
	{ "int", "SHELL_AREA_SETTINGS", "12" },
	{ "int", "SHELL_AREA_AUDIO", "13" },
	{ "int", "SHELL_AREA_VIDEO", "14" },
	{ "int", "SHELL_AREA_KEYBOARD", "15" },
	{ "int", "SHELL_AREA_CONTROLS", "16" },
	{ "int", "SHELL_AREA_CONTROLLER_LAYOUT", "17" },
	{ "int", "SHELL_AREA_GAMEPAD", "18" },
	{ "int", "SHELL_AREA_PAUSE", "19" },
	{ "int", "SHELL_AREA_LEADERBOARDS", "20" },
	{ "int", "SHELL_AREA_PLAYSTATION", "21" },
	{ "int", "SHELL_AREA_DIFFICULTY", "22" },
	{ "int", "SHELL_AREA_RESOLUTION", "23" },
	{ "int", "SHELL_AREA_MATCH_SETTINGS", "24" },
	{ "int", "SHELL_AREA_MODE_SELECT", "25" },
	{ "int", "SHELL_AREA_BROWSER", "26" },
	{ "int", "SHELL_AREA_CREDITS", "27" },
	{ "int", "SHELL_NUM_AREAS", "28" },
	{ "int", "SHELL_STATE_INVALID", "-1" },
	{ "int", "SHELL_STATE_PRESS_START", "0" },
	{ "int", "SHELL_STATE_IDLE", "1" },
	{ "int", "SHELL_STATE_PARTY_LOBBY", "2" },
	{ "int", "SHELL_STATE_GAME_LOBBY", "3" },
	{ "int", "SHELL_STATE_PAUSED", "4" },
	{ "int", "SHELL_STATE_CONNECTING", "5" },
	{ "int", "SHELL_STATE_SEARCHING", "6" },
	{ "int", "SHELL_STATE_LOADING", "7" },
	{ "int", "SHELL_STATE_BUSY", "8" },
	{ "int", "SHELL_STATE_IN_GAME", "9" },
	{ "int", "PDA_AREA_INVALID", "-1" },
	{ "int", "PDA_AREA_USER_DATA", "0" },
	{ "int", "PDA_AREA_USER_EMAIL", "1" },
	{ "int", "PDA_AREA_VIDEO_DISKS", "2" },
	{ "int", "PDA_AREA_INVENTORY", "3" },
	{ "int", "PDA_NUM_AREAS", "4" },
	{ "int", "HUD_AREA_INVALID", "-1" },
	{ "int", "HUD_AREA_PLAYING", "0" },
	{ "int", "HUD_NUM_AREAS", "1" },
	{ "int", "SCOREBOARD_AREA_INVALID", "-1" },
	{ "int", "SCOREBOARD_AREA_DEFAULT", "0" },
	{ "int", "SCOREBOARD_AREA_TEAM", "1" },
	{ "int", "SCOREBOARD_AREA_CTF", "2" },
	{ "int", "SCOREBOARD_NUM_AREAS", "3" },
	{ "int", "PDA_WIDGET_NAV_BAR", "0" },
	{ "int", "PDA_WIDGET_PDA_LIST", "1" },
	{ "int", "PDA_WIDGET_PDA_LIST_SCROLLBAR", "2" },
	{ "int", "PDA_WIDGET_CMD_BAR", "3" },
	{ "int", "SCOREBOARD_WIDGET_CMD_BAR", "0" },
	{ "int", "GUI_SOUND_MUSIC", "0" },
	{ "int", "GUI_SOUND_SCROLL", "1" },
	{ "int", "GUI_SOUND_ADVANCE", "2" },
	{ "int", "GUI_SOUND_BACK", "3" },
	{ "int", "GUI_SOUND_BUILD_ON", "4" },
	{ "int", "GUI_SOUND_BUILD_OFF", "5" },
	{ "int", "GUI_SOUND_FOCUS", "6" },
	{ "int", "GUI_SOUND_ROLL_OVER", "7" },
	{ "int", "GUI_SOUND_ROLL_OUT", "8" },
	{ "int", "NUM_GUI_SOUNDS", "9" },
	{ "static const int", "MAX_SCREEN_AREAS", "32" },
	{ "static const int", "DEFAULT_REPEAT_TIME", "150" },
	{ "static const int", "WAIT_START_TIME_LONG", "30000" },
	{ "static const int", "WAIT_START_TIME_SHORT", "5000" },
	{ "const char * const", "RESULT_STRING", "<RESULT>" },
	{ "int", "OP_RETURN", "0" },
	{ "int", "OP_UINC_F", "1" },
	{ "int", "OP_UINCP_F", "2" },
	{ "int", "OP_UDEC_F", "3" },
	{ "int", "OP_UDECP_F", "4" },
	{ "int", "OP_COMP_F", "5" },
	{ "int", "OP_MUL_F", "6" },
	{ "int", "OP_MUL_V", "7" },
	{ "int", "OP_MUL_FV", "8" },
	{ "int", "OP_MUL_VF", "9" },
	{ "int", "OP_DIV_F", "10" },
	{ "int", "OP_MOD_F", "11" },
	{ "int", "OP_ADD_F", "12" },
	{ "int", "OP_ADD_V", "13" },
	{ "int", "OP_ADD_S", "14" },
	{ "int", "OP_ADD_FS", "15" },
	{ "int", "OP_ADD_SF", "16" },
	{ "int", "OP_ADD_VS", "17" },
	{ "int", "OP_ADD_SV", "18" },
	{ "int", "OP_SUB_F", "19" },
	{ "int", "OP_SUB_V", "20" },
	{ "int", "OP_EQ_F", "21" },
	{ "int", "OP_EQ_V", "22" },
	{ "int", "OP_EQ_S", "23" },
	{ "int", "OP_EQ_E", "24" },
	{ "int", "OP_EQ_EO", "25" },
	{ "int", "OP_EQ_OE", "26" },
	{ "int", "OP_EQ_OO", "27" },
	{ "int", "OP_NE_F", "28" },
	{ "int", "OP_NE_V", "29" },
	{ "int", "OP_NE_S", "30" },
	{ "int", "OP_NE_E", "31" },
	{ "int", "OP_NE_EO", "32" },
	{ "int", "OP_NE_OE", "33" },
	{ "int", "OP_NE_OO", "34" },
	{ "int", "OP_LE", "35" },
	{ "int", "OP_GE", "36" },
	{ "int", "OP_LT", "37" },
	{ "int", "OP_GT", "38" },
	{ "int", "OP_INDIRECT_F", "39" },
	{ "int", "OP_INDIRECT_V", "40" },
	{ "int", "OP_INDIRECT_S", "41" },
	{ "int", "OP_INDIRECT_ENT", "42" },
	{ "int", "OP_INDIRECT_BOOL", "43" },
	{ "int", "OP_INDIRECT_OBJ", "44" },
	{ "int", "OP_ADDRESS", "45" },
	{ "int", "OP_EVENTCALL", "46" },
	{ "int", "OP_OBJECTCALL", "47" },
	{ "int", "OP_SYSCALL", "48" },
	{ "int", "OP_STORE_F", "49" },
	{ "int", "OP_STORE_V", "50" },
	{ "int", "OP_STORE_S", "51" },
	{ "int", "OP_STORE_ENT", "52" },
	{ "int", "OP_STORE_BOOL", "53" },
	{ "int", "OP_STORE_OBJENT", "54" },
	{ "int", "OP_STORE_OBJ", "55" },
	{ "int", "OP_STORE_ENTOBJ", "56" },
	{ "int", "OP_STORE_FTOS", "57" },
	{ "int", "OP_STORE_BTOS", "58" },
	{ "int", "OP_STORE_VTOS", "59" },
	{ "int", "OP_STORE_FTOBOOL", "60" },
	{ "int", "OP_STORE_BOOLTOF", "61" },
	{ "int", "OP_STOREP_F", "62" },
	{ "int", "OP_STOREP_V", "63" },
	{ "int", "OP_STOREP_S", "64" },
	{ "int", "OP_STOREP_ENT", "65" },
	{ "int", "OP_STOREP_FLD", "66" },
	{ "int", "OP_STOREP_BOOL", "67" },
	{ "int", "OP_STOREP_OBJ", "68" },
	{ "int", "OP_STOREP_OBJENT", "69" },
	{ "int", "OP_STOREP_FTOS", "70" },
	{ "int", "OP_STOREP_BTOS", "71" },
	{ "int", "OP_STOREP_VTOS", "72" },
	{ "int", "OP_STOREP_FTOBOOL", "73" },
	{ "int", "OP_STOREP_BOOLTOF", "74" },
	{ "int", "OP_UMUL_F", "75" },
	{ "int", "OP_UMUL_V", "76" },
	{ "int", "OP_UDIV_F", "77" },
	{ "int", "OP_UDIV_V", "78" },
	{ "int", "OP_UMOD_F", "79" },
	{ "int", "OP_UADD_F", "80" },
	{ "int", "OP_UADD_V", "81" },
	{ "int", "OP_USUB_F", "82" },
	{ "int", "OP_USUB_V", "83" },
	{ "int", "OP_UAND_F", "84" },
	{ "int", "OP_UOR_F", "85" },
	{ "int", "OP_NOT_BOOL", "86" },
	{ "int", "OP_NOT_F", "87" },
	{ "int", "OP_NOT_V", "88" },
	{ "int", "OP_NOT_S", "89" },
	{ "int", "OP_NOT_ENT", "90" },
	{ "int", "OP_NEG_F", "91" },
	{ "int", "OP_NEG_V", "92" },
	{ "int", "OP_INT_F", "93" },
	{ "int", "OP_IF", "94" },
	{ "int", "OP_IFNOT", "95" },
	{ "int", "OP_CALL", "96" },
	{ "int", "OP_THREAD", "97" },
	{ "int", "OP_OBJTHREAD", "98" },
	{ "int", "OP_PUSH_F", "99" },
	{ "int", "OP_PUSH_V", "100" },
	{ "int", "OP_PUSH_S", "101" },
	{ "int", "OP_PUSH_ENT", "102" },
	{ "int", "OP_PUSH_OBJ", "103" },
	{ "int", "OP_PUSH_OBJENT", "104" },
	{ "int", "OP_PUSH_FTOS", "105" },
	{ "int", "OP_PUSH_BTOF", "106" },
	{ "int", "OP_PUSH_FTOB", "107" },
	{ "int", "OP_PUSH_VTOS", "108" },
	{ "int", "OP_PUSH_BTOS", "109" },
	{ "int", "OP_GOTO", "110" },
	{ "int", "OP_AND", "111" },
	{ "int", "OP_AND_BOOLF", "112" },
	{ "int", "OP_AND_FBOOL", "113" },
	{ "int", "OP_AND_BOOLBOOL", "114" },
	{ "int", "OP_OR", "115" },
	{ "int", "OP_OR_BOOLF", "116" },
	{ "int", "OP_OR_FBOOL", "117" },
	{ "int", "OP_OR_BOOLBOOL", "118" },
	{ "int", "OP_BITAND", "119" },
	{ "int", "OP_BITOR", "120" },
	{ "int", "OP_BREAK", "121" },
	{ "int", "OP_CONTINUE", "122" },
	{ "int", "NUM_OPCODES", "123" },
	{ NULL, NULL, NULL }
};

static enumValueInfo_t stateResult_t_typeInfo[] = {
	{ "SRESULT_OK", 0 },
	{ "SRESULT_ERROR", 1 },
	{ "SRESULT_DONE", 2 },
	{ "SRESULT_DONE_WAIT", 3 },
	{ "SRESULT_WAIT", 4 },
	{ "SRESULT_IDLE", 5 },
	{ "SRESULT_SETSTAGE", 6 },
	{ "SRESULT_DONE_FRAME", 7 },
	{ "SRESULT_SETDELAY", 26 },
	{ NULL, 0 }
};

static enumValueInfo_t etype_t_typeInfo[] = {
	{ "ev_error", -1 },
	{ "ev_void", 0 },
	{ "ev_scriptevent", 1 },
	{ "ev_namespace", 2 },
	{ "ev_string", 3 },
	{ "ev_float", 4 },
	{ "ev_vector", 5 },
	{ "ev_entity", 6 },
	{ "ev_field", 7 },
	{ "ev_function", 8 },
	{ "ev_virtualfunction", 9 },
	{ "ev_pointer", 10 },
	{ "ev_object", 11 },
	{ "ev_jumpoffset", 12 },
	{ "ev_argsize", 13 },
	{ "ev_boolean", 14 },
	{ NULL, 0 }
};

static enumValueInfo_t idVarDef_initialized_t_typeInfo[] = {
	{ "uninitialized", 0 },
	{ "initializedVariable", 1 },
	{ "initializedConstant", 2 },
	{ "stackVariable", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t statement_s_enum_3_typeInfo[] = {
	{ "FLAG_OBJECTCALL_IMPL_NOT_PARSED_YET", 1 },
	{ NULL, 0 }
};

static enumValueInfo_t jointModTransform_t_typeInfo[] = {
	{ "JOINTMOD_NONE", 0 },
	{ "JOINTMOD_LOCAL", 1 },
	{ "JOINTMOD_LOCAL_OVERRIDE", 2 },
	{ "JOINTMOD_WORLD", 3 },
	{ "JOINTMOD_WORLD_OVERRIDE", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t frameCommandType_t_typeInfo[] = {
	{ "FC_SCRIPTFUNCTION", 0 },
	{ "FC_SCRIPTFUNCTIONOBJECT", 1 },
	{ "FC_EVENTFUNCTION", 2 },
	{ "FC_SOUND", 3 },
	{ "FC_SOUND_VOICE", 4 },
	{ "FC_SOUND_VOICE2", 5 },
	{ "FC_SOUND_BODY", 6 },
	{ "FC_SOUND_BODY2", 7 },
	{ "FC_SOUND_BODY3", 8 },
	{ "FC_SOUND_WEAPON", 9 },
	{ "FC_SOUND_ITEM", 10 },
	{ "FC_SOUND_GLOBAL", 11 },
	{ "FC_SOUND_CHATTER", 12 },
	{ "FC_SKIN", 13 },
	{ "FC_TRIGGER", 14 },
	{ "FC_TRIGGER_SMOKE_PARTICLE", 15 },
	{ "FC_MELEE", 16 },
	{ "FC_DIRECTDAMAGE", 17 },
	{ "FC_BEGINATTACK", 18 },
	{ "FC_ENDATTACK", 19 },
	{ "FC_MUZZLEFLASH", 20 },
	{ "FC_CREATEMISSILE", 21 },
	{ "FC_LAUNCHMISSILE", 22 },
	{ "FC_FIREMISSILEATTARGET", 23 },
	{ "FC_FOOTSTEP", 24 },
	{ "FC_LEFTFOOT", 25 },
	{ "FC_RIGHTFOOT", 26 },
	{ "FC_ENABLE_EYE_FOCUS", 27 },
	{ "FC_DISABLE_EYE_FOCUS", 28 },
	{ "FC_FX", 29 },
	{ "FC_DISABLE_GRAVITY", 30 },
	{ "FC_ENABLE_GRAVITY", 31 },
	{ "FC_JUMP", 32 },
	{ "FC_ENABLE_CLIP", 33 },
	{ "FC_DISABLE_CLIP", 34 },
	{ "FC_ENABLE_WALK_IK", 35 },
	{ "FC_DISABLE_WALK_IK", 36 },
	{ "FC_ENABLE_LEG_IK", 37 },
	{ "FC_DISABLE_LEG_IK", 38 },
	{ "FC_RECORDDEMO", 39 },
	{ "FC_AVIGAME", 40 },
	{ "FC_LAUNCH_PROJECTILE", 41 },
	{ "FC_TRIGGER_FX", 42 },
	{ "FC_START_EMITTER", 43 },
	{ "FC_STOP_EMITTER", 44 },
	{ NULL, 0 }
};

static enumValueInfo_t AFJointModType_t_typeInfo[] = {
	{ "AF_JOINTMOD_AXIS", 0 },
	{ "AF_JOINTMOD_ORIGIN", 1 },
	{ "AF_JOINTMOD_BOTH", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t enum_7_typeInfo[] = {
	{ "PATHTYPE_WALK", 0 },
	{ "PATHTYPE_WALKOFFLEDGE", 1 },
	{ "PATHTYPE_BARRIERJUMP", 2 },
	{ "PATHTYPE_JUMP", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t pvsType_t_typeInfo[] = {
	{ "PVS_NORMAL", 0 },
	{ "PVS_ALL_PORTALS_OPEN", 1 },
	{ "PVS_CONNECTED_AREAS", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t gameType_t_typeInfo[] = {
	{ "GAME_SP", -2 },
	{ "GAME_RANDOM", -1 },
	{ "GAME_DM", 0 },
	{ "GAME_TOURNEY", 1 },
	{ "GAME_TDM", 2 },
	{ "GAME_LASTMAN", 3 },
	{ "GAME_CTF", 4 },
	{ "GAME_COUNT", 5 },
	{ NULL, 0 }
};

static enumValueInfo_t flagStatus_t_typeInfo[] = {
	{ "FLAGSTATUS_INBASE", 0 },
	{ "FLAGSTATUS_TAKEN", 1 },
	{ "FLAGSTATUS_STRAY", 2 },
	{ "FLAGSTATUS_NONE", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t mpLeaderStatus_t_typeInfo[] = {
	{ "LEAD_STATUS_NOTSET", 0 },
	{ "LEAD_STATUS_NOLEAD", 1 },
	{ "LEAD_STATUS_INLEAD", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t snd_evt_t_typeInfo[] = {
	{ "SND_YOUWIN", 0 },
	{ "SND_YOULOSE", 1 },
	{ "SND_FIGHT", 2 },
	{ "SND_THREE", 3 },
	{ "SND_TWO", 4 },
	{ "SND_ONE", 5 },
	{ "SND_SUDDENDEATH", 6 },
	{ "SND_FLAG_CAPTURED_YOURS", 7 },
	{ "SND_FLAG_CAPTURED_THEIRS", 8 },
	{ "SND_FLAG_RETURN", 9 },
	{ "SND_FLAG_TAKEN_YOURS", 10 },
	{ "SND_FLAG_TAKEN_THEIRS", 11 },
	{ "SND_FLAG_DROPPED_YOURS", 12 },
	{ "SND_FLAG_DROPPED_THEIRS", 13 },
	{ "SND_LEADGAINED", 14 },
	{ "SND_LEADLOST", 15 },
	{ "SND_LEADTIED", 16 },
	{ "SND_WELCOMEDOM", 17 },
	{ "SND_ONEFRAG", 18 },
	{ "SND_TWOFRAG", 19 },
	{ "SND_THREEFRAG", 20 },
	{ "SND_PREPAREFORBATTLE", 21 },
	{ "SND_COUNT", 22 },
	{ NULL, 0 }
};

static enumValueInfo_t idMultiplayerGame_gameState_t_typeInfo[] = {
	{ "INACTIVE", 0 },
	{ "WARMUP", 1 },
	{ "COUNTDOWN", 2 },
	{ "GAMEON", 3 },
	{ "SUDDENDEATH", 4 },
	{ "GAMEREVIEW", 5 },
	{ "NEXTGAME", 6 },
	{ "STATE_COUNT", 7 },
	{ NULL, 0 }
};

static enumValueInfo_t idMultiplayerGame_msg_evt_t_typeInfo[] = {
	{ "MSG_SUICIDE", 0 },
	{ "MSG_KILLED", 1 },
	{ "MSG_KILLEDTEAM", 2 },
	{ "MSG_DIED", 3 },
	{ "MSG_SUDDENDEATH", 4 },
	{ "MSG_JOINEDSPEC", 5 },
	{ "MSG_TIMELIMIT", 6 },
	{ "MSG_FRAGLIMIT", 7 },
	{ "MSG_TELEFRAGGED", 8 },
	{ "MSG_JOINTEAM", 9 },
	{ "MSG_HOLYSHIT", 10 },
	{ "MSG_POINTLIMIT", 11 },
	{ "MSG_FLAGTAKEN", 12 },
	{ "MSG_FLAGDROP", 13 },
	{ "MSG_FLAGRETURN", 14 },
	{ "MSG_FLAGCAPTURE", 15 },
	{ "MSG_SCOREUPDATE", 16 },
	{ "MSG_LEFTGAME", 17 },
	{ "MSG_COUNT", 18 },
	{ NULL, 0 }
};

static enumValueInfo_t enum_15_typeInfo[] = {
	{ "GAME_RELIABLE_MESSAGE_SYNCEDCVARS", 0 },
	{ "GAME_RELIABLE_MESSAGE_SPAWN_PLAYER", 1 },
	{ "GAME_RELIABLE_MESSAGE_CHAT", 2 },
	{ "GAME_RELIABLE_MESSAGE_TCHAT", 3 },
	{ "GAME_RELIABLE_MESSAGE_SOUND_EVENT", 4 },
	{ "GAME_RELIABLE_MESSAGE_SOUND_INDEX", 5 },
	{ "GAME_RELIABLE_MESSAGE_DB", 6 },
	{ "GAME_RELIABLE_MESSAGE_DROPWEAPON", 7 },
	{ "GAME_RELIABLE_MESSAGE_RESTART", 8 },
	{ "GAME_RELIABLE_MESSAGE_TOURNEYLINE", 9 },
	{ "GAME_RELIABLE_MESSAGE_VCHAT", 10 },
	{ "GAME_RELIABLE_MESSAGE_STARTSTATE", 11 },
	{ "GAME_RELIABLE_MESSAGE_WARMUPTIME", 12 },
	{ "GAME_RELIABLE_MESSAGE_SPECTATE", 13 },
	{ "GAME_RELIABLE_MESSAGE_EVENT", 14 },
	{ "GAME_RELIABLE_MESSAGE_LOBBY_COUNTDOWN", 15 },
	{ "GAME_RELIABLE_MESSAGE_RESPAWN_AVAILABLE", 16 },
	{ "GAME_RELIABLE_MESSAGE_MATCH_STARTED_TIME", 17 },
	{ "GAME_RELIABLE_MESSAGE_ACHIEVEMENT_UNLOCK", 18 },
	{ "GAME_RELIABLE_MESSAGE_CLIENT_HITSCAN_HIT", 19 },
	{ NULL, 0 }
};

static enumValueInfo_t gameState_t_typeInfo[] = {
	{ "GAMESTATE_UNINITIALIZED", 0 },
	{ "GAMESTATE_NOMAP", 1 },
	{ "GAMESTATE_STARTUP", 2 },
	{ "GAMESTATE_ACTIVE", 3 },
	{ "GAMESTATE_SHUTDOWN", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t idEventQueue_outOfOrderBehaviour_t_typeInfo[] = {
	{ "OUTOFORDER_IGNORE", 0 },
	{ "OUTOFORDER_DROP", 1 },
	{ "OUTOFORDER_SORT", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t slowmoState_t_typeInfo[] = {
	{ "SLOWMO_STATE_OFF", 0 },
	{ "SLOWMO_STATE_RAMPUP", 1 },
	{ "SLOWMO_STATE_ON", 2 },
	{ "SLOWMO_STATE_RAMPDOWN", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idGameLocal_gameDemoCommand_t_typeInfo[] = {
	{ "GCMD_UNKNOWN", 0 },
	{ "GCMD_GAMETIME", 1 },
	{ NULL, 0 }
};

static enumValueInfo_t gameSoundChannel_t_typeInfo[] = {
	{ "SND_CHANNEL_ANY", 0 },
	{ "SND_CHANNEL_VOICE", 0 },
	{ "SND_CHANNEL_VOICE2", 1 },
	{ "SND_CHANNEL_BODY", 2 },
	{ "SND_CHANNEL_BODY2", 3 },
	{ "SND_CHANNEL_BODY3", 4 },
	{ "SND_CHANNEL_WEAPON", 5 },
	{ "SND_CHANNEL_ITEM", 6 },
	{ "SND_CHANNEL_HEART", 7 },
	{ "SND_CHANNEL_PDA_AUDIO", 8 },
	{ "SND_CHANNEL_PDA_VIDEO", 9 },
	{ "SND_CHANNEL_DEMONIC", 10 },
	{ "SND_CHANNEL_RADIO", 11 },
	{ "SND_CHANNEL_AMBIENT", 12 },
	{ "SND_CHANNEL_DAMAGE", 13 },
	{ "SND_CHANNEL_MUSIC", 14 },
	{ NULL, 0 }
};

static enumValueInfo_t forceFieldType_typeInfo[] = {
	{ "FORCEFIELD_UNIFORM", 0 },
	{ "FORCEFIELD_EXPLOSION", 1 },
	{ "FORCEFIELD_IMPLOSION", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t forceFieldApplyType_typeInfo[] = {
	{ "FORCEFIELD_APPLY_FORCE", 0 },
	{ "FORCEFIELD_APPLY_VELOCITY", 1 },
	{ "FORCEFIELD_APPLY_IMPULSE", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t monsterMoveResult_t_typeInfo[] = {
	{ "MM_OK", 0 },
	{ "MM_SLIDING", 1 },
	{ "MM_BLOCKED", 2 },
	{ "MM_STEPPED", 3 },
	{ "MM_FALLING", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t pmtype_t_typeInfo[] = {
	{ "PM_NORMAL", 0 },
	{ "PM_DEAD", 1 },
	{ "PM_SPECTATOR", 2 },
	{ "PM_FREEZE", 3 },
	{ "PM_NOCLIP", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t waterLevel_t_typeInfo[] = {
	{ "WATERLEVEL_NONE", 0 },
	{ "WATERLEVEL_FEET", 1 },
	{ "WATERLEVEL_WAIST", 2 },
	{ "WATERLEVEL_HEAD", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t constraintType_t_typeInfo[] = {
	{ "CONSTRAINT_INVALID", 0 },
	{ "CONSTRAINT_FIXED", 1 },
	{ "CONSTRAINT_BALLANDSOCKETJOINT", 2 },
	{ "CONSTRAINT_UNIVERSALJOINT", 3 },
	{ "CONSTRAINT_HINGE", 4 },
	{ "CONSTRAINT_HINGESTEERING", 5 },
	{ "CONSTRAINT_SLIDER", 6 },
	{ "CONSTRAINT_CYLINDRICALJOINT", 7 },
	{ "CONSTRAINT_LINE", 8 },
	{ "CONSTRAINT_PLANE", 9 },
	{ "CONSTRAINT_SPRING", 10 },
	{ "CONSTRAINT_CONTACT", 11 },
	{ "CONSTRAINT_FRICTION", 12 },
	{ "CONSTRAINT_CONELIMIT", 13 },
	{ "CONSTRAINT_PYRAMIDLIMIT", 14 },
	{ "CONSTRAINT_SUSPENSION", 15 },
	{ NULL, 0 }
};

static enumValueInfo_t enum_27_typeInfo[] = {
	{ "TH_ALL", -1 },
	{ "TH_THINK", 1 },
	{ "TH_PHYSICS", 2 },
	{ "TH_ANIMATE", 4 },
	{ "TH_UPDATEVISUALS", 8 },
	{ "TH_UPDATEPARTICLES", 16 },
	{ NULL, 0 }
};

static enumValueInfo_t signalNum_t_typeInfo[] = {
	{ "SIG_TOUCH", 0 },
	{ "SIG_USE", 1 },
	{ "SIG_TRIGGER", 2 },
	{ "SIG_REMOVED", 3 },
	{ "SIG_DAMAGE", 4 },
	{ "SIG_BLOCKED", 5 },
	{ "SIG_MOVER_POS1", 6 },
	{ "SIG_MOVER_POS2", 7 },
	{ "SIG_MOVER_1TO2", 8 },
	{ "SIG_MOVER_2TO1", 9 },
	{ "NUM_SIGNALS", 10 },
	{ NULL, 0 }
};

static enumValueInfo_t idEntity_enum_29_typeInfo[] = {
	{ "EVENT_STARTSOUNDSHADER", 0 },
	{ "EVENT_STOPSOUNDSHADER", 1 },
	{ "EVENT_MAXEVENTS", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t idEntity_interpolationBehavior_t_typeInfo[] = {
	{ "USE_NO_INTERPOLATION", 0 },
	{ "USE_LATEST_SNAP_ONLY", 1 },
	{ "USE_INTERPOLATION", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t idAnimatedEntity_enum_31_typeInfo[] = {
	{ "EVENT_ADD_DAMAGE_EFFECT", 2 },
	{ "EVENT_MAXEVENTS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idPlayerStart_enum_32_typeInfo[] = {
	{ "EVENT_TELEPORTPLAYER", 2 },
	{ "EVENT_MAXEVENTS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idProjectile_enum_33_typeInfo[] = {
	{ "EVENT_DAMAGE_EFFECT", 2 },
	{ "EVENT_MAXEVENTS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idProjectile_projectileState_t_typeInfo[] = {
	{ "SPAWNED", 0 },
	{ "CREATED", 1 },
	{ "LAUNCHED", 2 },
	{ "FIZZLED", 3 },
	{ "EXPLODED", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t idWeapon_enum_35_typeInfo[] = {
	{ "EVENT_RELOAD", 2 },
	{ "EVENT_ENDRELOAD", 3 },
	{ "EVENT_CHANGESKIN", 4 },
	{ "EVENT_MAXEVENTS", 5 },
	{ NULL, 0 }
};

static enumValueInfo_t idLight_enum_36_typeInfo[] = {
	{ "EVENT_BECOMEBROKEN", 2 },
	{ "EVENT_MAXEVENTS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t EnvironmentProbe_enum_37_typeInfo[] = {
	{ "EVENT_BECOMEBROKEN", 2 },
	{ "EVENT_MAXEVENTS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t itemGiveFlags_t_typeInfo[] = {
	{ "ITEM_GIVE_FEEDBACK", 0 },
	{ "ITEM_GIVE_UPDATE_STATE", 0 },
	{ "ITEM_GIVE_FROM_WEAPON", 0 },
	{ NULL, 0 }
};

static enumValueInfo_t idItem_enum_39_typeInfo[] = {
	{ "EVENT_PICKUP", 2 },
	{ "EVENT_RESPAWN", 3 },
	{ "EVENT_RESPAWNFX", 4 },
	{ "EVENT_TAKEFLAG", 5 },
	{ "EVENT_DROPFLAG", 6 },
	{ "EVENT_FLAGRETURN", 7 },
	{ "EVENT_FLAGCAPTURE", 8 },
	{ "EVENT_MAXEVENTS", 9 },
	{ NULL, 0 }
};

static enumValueInfo_t FxFader_enum_40_typeInfo[] = {
	{ "FX_STATE_OFF", 0 },
	{ "FX_STATE_RAMPUP", 1 },
	{ "FX_STATE_RAMPDOWN", 2 },
	{ "FX_STATE_ON", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t playerIconType_t_typeInfo[] = {
	{ "ICON_LAG", 0 },
	{ "ICON_CHAT", 1 },
	{ "ICON_TEAM_RED", 2 },
	{ "ICON_TEAM_BLUE", 3 },
	{ "ICON_NONE", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t achievement_t_typeInfo[] = {
	{ "ACHIEVEMENT_INVALID", -1 },
	{ "ACHIEVEMENT_EARN_ALL_50_TROPHIES", 0 },
	{ "ACHIEVEMENT_COMPLETED_DIFFICULTY_0", 1 },
	{ "ACHIEVEMENT_COMPLETED_DIFFICULTY_1", 2 },
	{ "ACHIEVEMENT_COMPLETED_DIFFICULTY_2", 3 },
	{ "ACHIEVEMENT_COMPLETED_DIFFICULTY_3", 4 },
	{ "ACHIEVEMENT_PDAS_BASE", 5 },
	{ "ACHIEVEMENT_WATCH_ALL_VIDEOS", 6 },
	{ "ACHIEVEMENT_KILL_MONSTER_WITH_1_HEALTH_LEFT", 7 },
	{ "ACHIEVEMENT_OPEN_ALL_LOCKERS", 8 },
	{ "ACHIEVEMENT_KILL_20_ENEMY_FISTS_HANDS", 9 },
	{ "ACHIEVEMENT_KILL_SCI_NEXT_TO_RCR", 10 },
	{ "ACHIEVEMENT_KILL_TWO_IMPS_ONE_SHOTGUN", 11 },
	{ "ACHIEVEMENT_SCORE_25000_TURKEY_PUNCHER", 12 },
	{ "ACHIEVEMENT_DESTROY_BARRELS", 13 },
	{ "ACHIEVEMENT_GET_BFG_FROM_SECURITY_OFFICE", 14 },
	{ "ACHIEVEMENT_COMPLETE_LEVEL_WITHOUT_TAKING_DMG", 15 },
	{ "ACHIEVEMENT_FIND_RAGE_LOGO", 16 },
	{ "ACHIEVEMENT_SPEED_RUN", 17 },
	{ "ACHIEVEMENT_DEFEAT_VAGARY_BOSS", 18 },
	{ "ACHIEVEMENT_DEFEAT_GUARDIAN_BOSS", 19 },
	{ "ACHIEVEMENT_DEFEAT_SABAOTH_BOSS", 20 },
	{ "ACHIEVEMENT_DEFEAT_CYBERDEMON_BOSS", 21 },
	{ "ACHIEVEMENT_SENTRY_BOT_ALIVE_TO_DEST", 22 },
	{ "ACHIEVEMENT_KILL_20_ENEMY_WITH_CHAINSAW", 23 },
	{ "ACHIEVEMENT_ID_LOGO_SECRET_ROOM", 24 },
	{ "ACHIEVEMENT_BLOODY_HANDWORK_OF_BETRUGER", 25 },
	{ "ACHIEVEMENT_TWO_DEMONS_FIGHT_EACH_OTHER", 26 },
	{ "ACHIEVEMENT_USE_SOUL_CUBE_TO_DEFEAT_20_ENEMY", 27 },
	{ "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_0", 28 },
	{ "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_1", 29 },
	{ "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_2", 30 },
	{ "ACHIEVEMENT_ROE_COMPLETED_DIFFICULTY_3", 31 },
	{ "ACHIEVEMENT_PDAS_ROE", 32 },
	{ "ACHIEVEMENT_KILL_5_ENEMY_HELL_TIME", 33 },
	{ "ACHIEVEMENT_DEFEAT_HELLTIME_HUNTER", 34 },
	{ "ACHIEVEMENT_DEFEAT_BERSERK_HUNTER", 35 },
	{ "ACHIEVEMENT_DEFEAT_INVULNERABILITY_HUNTER", 36 },
	{ "ACHIEVEMENT_DEFEAT_MALEDICT_BOSS", 37 },
	{ "ACHIEVEMENT_GRABBER_KILL_20_ENEMY", 38 },
	{ "ACHIEVEMENT_ARTIFACT_WITH_BERSERK_PUNCH_20", 39 },
	{ "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_0", 40 },
	{ "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_1", 41 },
	{ "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_2", 42 },
	{ "ACHIEVEMENT_LE_COMPLETED_DIFFICULTY_3", 43 },
	{ "ACHIEVEMENT_PDAS_LE", 44 },
	{ "ACHIEVEMENT_MP_KILL_PLAYER_VIA_TELEPORT", 45 },
	{ "ACHIEVEMENT_MP_CATCH_ENEMY_IN_ROFC", 46 },
	{ "ACHIEVEMENT_MP_KILL_5_PLAYERS_USING_INVIS", 47 },
	{ "ACHIEVEMENT_MP_COMPLETE_MATCH_WITHOUT_DYING", 48 },
	{ "ACHIEVEMENT_MP_USE_BERSERK_TO_KILL_PLAYER", 49 },
	{ "ACHIEVEMENT_MP_KILL_2_GUYS_IN_ROOM_WITH_BFG", 50 },
	{ "ACHIEVEMENT_DOOM1_NEOPHYTE_COMPLETE_ANY_LEVEL", 51 },
	{ "ACHIEVEMENT_DOOM1_EPISODE1_COMPLETE_MEDIUM", 52 },
	{ "ACHIEVEMENT_DOOM1_EPISODE2_COMPLETE_MEDIUM", 53 },
	{ "ACHIEVEMENT_DOOM1_EPISODE3_COMPLETE_MEDIUM", 54 },
	{ "ACHIEVEMENT_DOOM1_EPISODE4_COMPLETE_MEDIUM", 55 },
	{ "ACHIEVEMENT_DOOM1_RAMPAGE_COMPLETE_ALL_HARD", 56 },
	{ "ACHIEVEMENT_DOOM1_NIGHTMARE_COMPLETE_ANY_LEVEL_NIGHTMARE", 57 },
	{ "ACHIEVEMENT_DOOM1_BURNING_OUT_OF_CONTROL_COMPLETE_KILLS_ITEMS_SECRETS", 58 },
	{ "ACHIEVEMENT_DOOM2_JUST_GETTING_STARTED_COMPLETE_ANY_LEVEL", 59 },
	{ "ACHIEVEMENT_DOOM2_FROM_EARTH_TO_HELL_COMPLETE_HELL_ON_EARTH", 60 },
	{ "ACHIEVEMENT_DOOM2_AND_BACK_AGAIN_COMPLETE_NO_REST", 61 },
	{ "ACHIEVEMENT_DOOM2_SUPERIOR_FIREPOWER_COMPLETE_ALL_HARD", 62 },
	{ "ACHIEVEMENT_DOOM2_REALLY_BIG_GUN_FIND_BFG_SINGLEPLAYER", 63 },
	{ "ACHIEVEMENT_DOOM2_BURNING_OUT_OF_CONTROL_COMPLETE_KILLS_ITEMS_SECRETS", 64 },
	{ "ACHIEVEMENT_DOOM2_IMPORTANT_LOOKING_DOOR_FIND_ANY_SECRET", 65 },
	{ "ACHIEVEMENTS_NUM", 66 },
	{ "STAT_DOOM_COMPLETED_EPISODE_1_MEDIUM", 67 },
	{ "STAT_DOOM_COMPLETED_EPISODE_2_MEDIUM", 68 },
	{ "STAT_DOOM_COMPLETED_EPISODE_3_MEDIUM", 69 },
	{ "STAT_DOOM_COMPLETED_EPISODE_4_MEDIUM", 70 },
	{ "STAT_DOOM_COMPLETED_EPISODE_1_HARD", 71 },
	{ "STAT_DOOM_COMPLETED_EPISODE_2_HARD", 72 },
	{ "STAT_DOOM_COMPLETED_EPISODE_3_HARD", 73 },
	{ "STAT_DOOM_COMPLETED_EPISODE_4_HARD", 74 },
	{ NULL, 0 }
};

static enumValueInfo_t gameExpansionType_t_typeInfo[] = {
	{ "GAME_BASE", 0 },
	{ "GAME_D3XP", 1 },
	{ "GAME_D3LE", 2 },
	{ "GAME_UNKNOWN", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t enum_44_typeInfo[] = {
	{ "BERSERK", 0 },
	{ "INVISIBILITY", 1 },
	{ "MEGAHEALTH", 2 },
	{ "ADRENALINE", 3 },
	{ "INVULNERABILITY", 4 },
	{ "HELLTIME", 5 },
	{ "ENVIROSUIT", 6 },
	{ "ENVIROTIME", 7 },
	{ "MAX_POWERUPS", 8 },
	{ NULL, 0 }
};

static enumValueInfo_t enum_45_typeInfo[] = {
	{ "SPEED", 0 },
	{ "PROJECTILE_DAMAGE", 1 },
	{ "MELEE_DAMAGE", 2 },
	{ "MELEE_DISTANCE", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t enum_46_typeInfo[] = {
	{ "INFLUENCE_NONE", 0 },
	{ "INFLUENCE_LEVEL1", 1 },
	{ "INFLUENCE_LEVEL2", 2 },
	{ "INFLUENCE_LEVEL3", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idPlayer_enum_47_typeInfo[] = {
	{ "EVENT_IMPULSE", 2 },
	{ "EVENT_EXIT_TELEPORTER", 3 },
	{ "EVENT_ABORT_TELEPORTER", 4 },
	{ "EVENT_POWERUP", 5 },
	{ "EVENT_SPECTATE", 6 },
	{ "EVENT_PICKUPNAME", 7 },
	{ "EVENT_FORCE_ORIGIN", 8 },
	{ "EVENT_KNOCKBACK", 9 },
	{ "EVENT_MAXEVENTS", 10 },
	{ NULL, 0 }
};

static enumValueInfo_t idMover_moveStage_t_typeInfo[] = {
	{ "ACCELERATION_STAGE", 0 },
	{ "LINEAR_STAGE", 1 },
	{ "DECELERATION_STAGE", 2 },
	{ "FINISHED_STAGE", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idMover_moverCommand_t_typeInfo[] = {
	{ "MOVER_NONE", 0 },
	{ "MOVER_ROTATING", 1 },
	{ "MOVER_MOVING", 2 },
	{ "MOVER_SPLINE", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idMover_moverDir_t_typeInfo[] = {
	{ "DIR_UP", -1 },
	{ "DIR_DOWN", -2 },
	{ "DIR_LEFT", -3 },
	{ "DIR_RIGHT", -4 },
	{ "DIR_FORWARD", -5 },
	{ "DIR_BACK", -6 },
	{ "DIR_REL_UP", -7 },
	{ "DIR_REL_DOWN", -8 },
	{ "DIR_REL_LEFT", -9 },
	{ "DIR_REL_RIGHT", -10 },
	{ "DIR_REL_FORWARD", -11 },
	{ "DIR_REL_BACK", -12 },
	{ NULL, 0 }
};

static enumValueInfo_t idElevator_elevatorState_t_typeInfo[] = {
	{ "INIT", 0 },
	{ "IDLE", 1 },
	{ "WAITING_ON_DOORS", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t moverState_t_typeInfo[] = {
	{ "MOVER_POS1", 0 },
	{ "MOVER_POS2", 1 },
	{ "MOVER_1TO2", 2 },
	{ "MOVER_2TO1", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idExplodingBarrel_enum_53_typeInfo[] = {
	{ "EVENT_EXPLODE", 2 },
	{ "EVENT_MAXEVENTS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idExplodingBarrel_explode_state_t_typeInfo[] = {
	{ "NORMAL", 0 },
	{ "BURNING", 1 },
	{ "BURNEXPIRED", 2 },
	{ "EXPLODING", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idSecurityCamera_enum_55_typeInfo[] = {
	{ "SCANNING", 0 },
	{ "LOSINGINTEREST", 1 },
	{ "ALERT", 2 },
	{ "ACTIVATED", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idBrittleFracture_enum_56_typeInfo[] = {
	{ "EVENT_PROJECT_DECAL", 2 },
	{ "EVENT_SHATTER", 3 },
	{ "EVENT_MAXEVENTS", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t moveType_t_typeInfo[] = {
	{ "MOVETYPE_DEAD", 0 },
	{ "MOVETYPE_ANIM", 1 },
	{ "MOVETYPE_SLIDE", 2 },
	{ "MOVETYPE_FLY", 3 },
	{ "MOVETYPE_STATIC", 4 },
	{ "NUM_MOVETYPES", 5 },
	{ NULL, 0 }
};

static enumValueInfo_t moveCommand_t_typeInfo[] = {
	{ "MOVE_NONE", 0 },
	{ "MOVE_FACE_ENEMY", 1 },
	{ "MOVE_FACE_ENTITY", 2 },
	{ "NUM_NONMOVING_COMMANDS", 3 },
	{ "MOVE_TO_ENEMY", 3 },
	{ "MOVE_TO_ENEMYHEIGHT", 4 },
	{ "MOVE_TO_ENTITY", 5 },
	{ "MOVE_OUT_OF_RANGE", 6 },
	{ "MOVE_TO_ATTACK_POSITION", 7 },
	{ "MOVE_TO_COVER", 8 },
	{ "MOVE_TO_POSITION", 9 },
	{ "MOVE_TO_POSITION_DIRECT", 10 },
	{ "MOVE_SLIDE_TO_POSITION", 11 },
	{ "MOVE_WANDER", 12 },
	{ "NUM_MOVE_COMMANDS", 13 },
	{ NULL, 0 }
};

static enumValueInfo_t talkState_t_typeInfo[] = {
	{ "TALK_NEVER", 0 },
	{ "TALK_DEAD", 1 },
	{ "TALK_OK", 2 },
	{ "TALK_BUSY", 3 },
	{ "NUM_TALK_STATES", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t moveStatus_t_typeInfo[] = {
	{ "MOVE_STATUS_DONE", 0 },
	{ "MOVE_STATUS_MOVING", 1 },
	{ "MOVE_STATUS_WAITING", 2 },
	{ "MOVE_STATUS_DEST_NOT_FOUND", 3 },
	{ "MOVE_STATUS_DEST_UNREACHABLE", 4 },
	{ "MOVE_STATUS_BLOCKED_BY_WALL", 5 },
	{ "MOVE_STATUS_BLOCKED_BY_OBJECT", 6 },
	{ "MOVE_STATUS_BLOCKED_BY_ENEMY", 7 },
	{ "MOVE_STATUS_BLOCKED_BY_MONSTER", 8 },
	{ NULL, 0 }
};

static enumValueInfo_t stopEvent_t_typeInfo[] = {
	{ "SE_BLOCKED", 0 },
	{ "SE_ENTER_LEDGE_AREA", 0 },
	{ "SE_ENTER_OBSTACLE", 0 },
	{ "SE_FALL", 0 },
	{ "SE_LAND", 0 },
	{ NULL, 0 }
};

static enumValueInfo_t botMoveFlags_t_typeInfo[] = {
	{ "NULLMOVEFLAG", -1 },
	{ "MOVE_PRONE", 0 },
	{ "MOVE_CROUCH", 1 },
	{ "MOVE_WALK", 2 },
	{ "MOVE_RUN2", 3 },
	{ "MOVE_SPRINT", 4 },
	{ "MOVE_JUMP", 5 },
	{ NULL, 0 }
};

static enumValueInfo_t botChat_t_typeInfo[] = {
	{ "KILL", 0 },
	{ "DEATH", 1 },
	{ NULL, 0 }
};

static enumValueInfo_t menuOption_t_typeInfo[] = {
	{ "OPTION_INVALID", -1 },
	{ "OPTION_BUTTON_TEXT", 0 },
	{ "OPTION_SLIDER_BAR", 1 },
	{ "OPTION_SLIDER_TEXT", 2 },
	{ "OPTION_SLIDER_TOGGLE", 3 },
	{ "OPTION_BUTTON_INFO", 4 },
	{ "OPTION_BUTTON_FULL_TEXT_SLIDER", 5 },
	{ "MAX_MENU_OPTION_TYPES", 6 },
	{ NULL, 0 }
};

static enumValueInfo_t widgetEvent_t_typeInfo[] = {
	{ "WIDGET_EVENT_PRESS", 0 },
	{ "WIDGET_EVENT_RELEASE", 1 },
	{ "WIDGET_EVENT_ROLL_OVER", 2 },
	{ "WIDGET_EVENT_ROLL_OUT", 3 },
	{ "WIDGET_EVENT_FOCUS_ON", 4 },
	{ "WIDGET_EVENT_FOCUS_OFF", 5 },
	{ "WIDGET_EVENT_SCROLL_UP_LSTICK", 6 },
	{ "WIDGET_EVENT_SCROLL_UP_LSTICK_RELEASE", 7 },
	{ "WIDGET_EVENT_SCROLL_DOWN_LSTICK", 8 },
	{ "WIDGET_EVENT_SCROLL_DOWN_LSTICK_RELEASE", 9 },
	{ "WIDGET_EVENT_SCROLL_LEFT_LSTICK", 10 },
	{ "WIDGET_EVENT_SCROLL_LEFT_LSTICK_RELEASE", 11 },
	{ "WIDGET_EVENT_SCROLL_RIGHT_LSTICK", 12 },
	{ "WIDGET_EVENT_SCROLL_RIGHT_LSTICK_RELEASE", 13 },
	{ "WIDGET_EVENT_SCROLL_UP_RSTICK", 14 },
	{ "WIDGET_EVENT_SCROLL_UP_RSTICK_RELEASE", 15 },
	{ "WIDGET_EVENT_SCROLL_DOWN_RSTICK", 16 },
	{ "WIDGET_EVENT_SCROLL_DOWN_RSTICK_RELEASE", 17 },
	{ "WIDGET_EVENT_SCROLL_LEFT_RSTICK", 18 },
	{ "WIDGET_EVENT_SCROLL_LEFT_RSTICK_RELEASE", 19 },
	{ "WIDGET_EVENT_SCROLL_RIGHT_RSTICK", 20 },
	{ "WIDGET_EVENT_SCROLL_RIGHT_RSTICK_RELEASE", 21 },
	{ "WIDGET_EVENT_SCROLL_UP", 22 },
	{ "WIDGET_EVENT_SCROLL_UP_RELEASE", 23 },
	{ "WIDGET_EVENT_SCROLL_DOWN", 24 },
	{ "WIDGET_EVENT_SCROLL_DOWN_RELEASE", 25 },
	{ "WIDGET_EVENT_SCROLL_LEFT", 26 },
	{ "WIDGET_EVENT_SCROLL_LEFT_RELEASE", 27 },
	{ "WIDGET_EVENT_SCROLL_RIGHT", 28 },
	{ "WIDGET_EVENT_SCROLL_RIGHT_RELEASE", 29 },
	{ "WIDGET_EVENT_DRAG_START", 30 },
	{ "WIDGET_EVENT_DRAG_STOP", 31 },
	{ "WIDGET_EVENT_SCROLL_PAGEDWN", 32 },
	{ "WIDGET_EVENT_SCROLL_PAGEDWN_RELEASE", 33 },
	{ "WIDGET_EVENT_SCROLL_PAGEUP", 34 },
	{ "WIDGET_EVENT_SCROLL_PAGEUP_RELEASE", 35 },
	{ "WIDGET_EVENT_SCROLL", 36 },
	{ "WIDGET_EVENT_SCROLL_RELEASE", 37 },
	{ "WIDGET_EVENT_BACK", 38 },
	{ "WIDGET_EVENT_COMMAND", 39 },
	{ "WIDGET_EVENT_TAB_NEXT", 40 },
	{ "WIDGET_EVENT_TAB_PREV", 41 },
	{ "MAX_WIDGET_EVENT", 42 },
	{ NULL, 0 }
};

static enumValueInfo_t scrollType_t_typeInfo[] = {
	{ "SCROLL_SINGLE", 0 },
	{ "SCROLL_PAGE", 1 },
	{ "SCROLL_FULL", 2 },
	{ "SCROLL_TOP", 3 },
	{ "SCROLL_END", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t widgetAction_t_typeInfo[] = {
	{ "WIDGET_ACTION_NONE", 0 },
	{ "WIDGET_ACTION_COMMAND", 1 },
	{ "WIDGET_ACTION_FUNCTION", 2 },
	{ "WIDGET_ACTION_SCROLL_VERTICAL", 3 },
	{ "WIDGET_ACTION_SCROLL_VERTICAL_VARIABLE", 4 },
	{ "WIDGET_ACTION_SCROLL_PAGE", 5 },
	{ "WIDGET_ACTION_SCROLL_HORIZONTAL", 6 },
	{ "WIDGET_ACTION_SCROLL_TAB", 7 },
	{ "WIDGET_ACTION_START_REPEATER", 8 },
	{ "WIDGET_ACTION_STOP_REPEATER", 9 },
	{ "WIDGET_ACTION_ADJUST_FIELD", 10 },
	{ "WIDGET_ACTION_PRESS_FOCUSED", 11 },
	{ "WIDGET_ACTION_JOY3_ON_PRESS", 12 },
	{ "WIDGET_ACTION_JOY4_ON_PRESS", 13 },
	{ "WIDGET_ACTION_GOTO_MENU", 14 },
	{ "WIDGET_ACTION_GO_BACK", 15 },
	{ "WIDGET_ACTION_EXIT_GAME", 16 },
	{ "WIDGET_ACTION_LAUNCH_MULTIPLAYER", 17 },
	{ "WIDGET_ACTION_MENU_BAR_SELECT", 18 },
	{ "WIDGET_ACTION_EMAIL_HOVER", 19 },
	{ "WIDGET_ACTION_PDA_SELECT_USER", 20 },
	{ "WIDGET_ACTION_SELECT_GAMERTAG", 21 },
	{ "WIDGET_ACTION_PDA_SELECT_NAV", 22 },
	{ "WIDGET_ACTION_SELECT_PDA_AUDIO", 23 },
	{ "WIDGET_ACTION_SELECT_PDA_VIDEO", 24 },
	{ "WIDGET_ACTION_SELECT_PDA_ITEM", 25 },
	{ "WIDGET_ACTION_SCROLL_DRAG", 26 },
	{ "WIDGET_ACTION_PDA_SELECT_EMAIL", 27 },
	{ "WIDGET_ACTION_PDA_CLOSE", 28 },
	{ "WIDGET_ACTION_REFRESH", 29 },
	{ "WIDGET_ACTION_MUTE_PLAYER", 30 },
	{ "MAX_WIDGET_ACTION", 31 },
	{ NULL, 0 }
};

static enumValueInfo_t actionHandler_t_typeInfo[] = {
	{ "WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER", 0 },
	{ "WIDGET_ACTION_EVENT_SCROLL_UP_START_REPEATER_VARIABLE", 1 },
	{ "WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER", 2 },
	{ "WIDGET_ACTION_EVENT_SCROLL_DOWN_START_REPEATER_VARIABLE", 3 },
	{ "WIDGET_ACTION_EVENT_SCROLL_LEFT_START_REPEATER", 4 },
	{ "WIDGET_ACTION_EVENT_SCROLL_RIGHT_START_REPEATER", 5 },
	{ "WIDGET_ACTION_EVENT_SCROLL_PAGE_DOWN_START_REPEATER", 6 },
	{ "WIDGET_ACTION_EVENT_SCROLL_PAGE_UP_START_REPEATER", 7 },
	{ "WIDGET_ACTION_EVENT_STOP_REPEATER", 8 },
	{ "WIDGET_ACTION_EVENT_TAB_NEXT", 9 },
	{ "WIDGET_ACTION_EVENT_TAB_PREV", 10 },
	{ "WIDGET_ACTION_EVENT_DRAG_START", 11 },
	{ "WIDGET_ACTION_EVENT_DRAG_STOP", 12 },
	{ "WIDGET_ACTION_EVENT_JOY3_ON_PRESS", 13 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuWidget_widgetState_t_typeInfo[] = {
	{ "WIDGET_STATE_HIDDEN", 0 },
	{ "WIDGET_STATE_NORMAL", 1 },
	{ "WIDGET_STATE_SELECTING", 2 },
	{ "WIDGET_STATE_SELECTED", 3 },
	{ "WIDGET_STATE_DISABLED", 4 },
	{ "WIDGET_STATE_MAX", 5 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuWidget_Button_animState_t_typeInfo[] = {
	{ "ANIM_STATE_UP", 0 },
	{ "ANIM_STATE_DOWN", 1 },
	{ "ANIM_STATE_OVER", 2 },
	{ "ANIM_STATE_MAX", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuWidget_NavButton_navWidgetState_t_typeInfo[] = {
	{ "NAV_WIDGET_LEFT", 0 },
	{ "NAV_WIDGET_RIGHT", 1 },
	{ "NAV_WIDGET_SELECTED", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuWidget_CommandBar_button_t_typeInfo[] = {
	{ "BUTTON_JOY1", 0 },
	{ "BUTTON_JOY2", 1 },
	{ "BUTTON_JOY3", 2 },
	{ "BUTTON_JOY4", 3 },
	{ "BUTTON_JOY10", 4 },
	{ "BUTTON_TAB", 5 },
	{ "MAX_BUTTONS", 6 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuWidget_CommandBar_alignment_t_typeInfo[] = {
	{ "LEFT", 0 },
	{ "RIGHT", 1 },
	{ NULL, 0 }
};

static enumValueInfo_t mainMenuTransition_t_typeInfo[] = {
	{ "MENU_TRANSITION_INVALID", -1 },
	{ "MENU_TRANSITION_SIMPLE", 0 },
	{ "MENU_TRANSITION_ADVANCE", 1 },
	{ "MENU_TRANSITION_BACK", 2 },
	{ "MENU_TRANSITION_FORCE", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t cursorState_t_typeInfo[] = {
	{ "CURSOR_NONE", 0 },
	{ "CURSOR_IN_COMBAT", 1 },
	{ "CURSOR_TALK", 2 },
	{ "CURSOR_GRABBER", 3 },
	{ "CURSOR_ITEM", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t leaderboardFilterMode_t_typeInfo[] = {
	{ "LEADERBOARD_FILTER_OVERALL", 0 },
	{ "LEADERBOARD_FILTER_MYSCORE", 1 },
	{ "LEADERBOARD_FILTER_FRIENDS", 2 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuScreen_Shell_GameOptions_idMenuDataSource_GameSettings_gameSettingFields_t_typeInfo[] = {
	{ "GAME_FIELD_FOV", 0 },
	{ "GAME_FIELD_CHECKPOINTS", 1 },
	{ "GAME_FIELD_AUTO_SWITCH", 2 },
	{ "GAME_FIELD_AUTO_RELOAD", 3 },
	{ "GAME_FIELD_AIM_ASSIST", 4 },
	{ "GAME_FIELD_ALWAYS_SPRINT", 5 },
	{ "GAME_FIELD_CLASSIC_FLASHLIGHT", 6 },
	{ "GAME_FIELD_MUZZLE_FLASHES", 7 },
	{ "MAX_GAME_FIELDS", 8 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuScreen_Shell_MatchSettings_idMenuDataSource_MatchSettings_matchSettingFields_t_typeInfo[] = {
	{ "MATCH_FIELD_MODE", 0 },
	{ "MATCH_FIELD_MAP", 1 },
	{ "MATCH_FIELD_TIME", 2 },
	{ "MATCH_FIELD_SCORE", 3 },
	{ "MAX_MATCH_FIELDS", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuScreen_Shell_Controls_idMenuDataSource_ControlSettings_controlSettingFields_t_typeInfo[] = {
	{ "CONTROLS_FIELD_INVERT_MOUSE", 0 },
	{ "CONTROLS_FIELD_GAMEPAD_ENABLED", 1 },
	{ "CONTROLS_FIELD_MOUSE_SENS", 2 },
	{ "MAX_CONTROL_FIELDS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuScreen_Shell_Gamepad_idMenuDataSource_GamepadSettings_controlSettingFields_t_typeInfo[] = {
	{ "GAMEPAD_FIELD_LEFTY", 0 },
	{ "GAMEPAD_FIELD_INVERT", 1 },
	{ "GAMEPAD_FIELD_VIBRATE", 2 },
	{ "GAMEPAD_FIELD_HOR_SENS", 3 },
	{ "GAMEPAD_FIELD_VERT_SENS", 4 },
	{ "GAMEPAD_FIELD_ACCELERATION", 5 },
	{ "GAMEPAD_FIELD_THRESHOLD", 6 },
	{ "MAX_GAMEPAD_FIELDS", 7 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuScreen_Shell_ControllerLayout_idMenuDataSource_LayoutSettings_controlSettingFields_t_typeInfo[] = {
	{ "LAYOUT_FIELD_LAYOUT", 0 },
	{ "MAX_LAYOUT_FIELDS", 1 },
	{ NULL, 0 }
};

static enumValueInfo_t idMenuScreen_Shell_SystemOptions_idMenuDataSource_SystemSettings_systemSettingFields_t_typeInfo[] = {
	{ "SYSTEM_FIELD_RENDERAPI", 0 },
	{ "SYSTEM_FIELD_FULLSCREEN", 1 },
	{ "SYSTEM_FIELD_FRAMERATE", 2 },
	{ "SYSTEM_FIELD_VSYNC", 3 },
	{ "SYSTEM_FIELD_ANTIALIASING", 4 },
	{ "SYSTEM_FIELD_RENDERMODE", 5 },
	{ "SYSTEM_FIELD_AMBIENT_BRIGHTNESS", 6 },
	{ "SYSTEM_FIELD_SSAO", 7 },
	{ "SYSTEM_FIELD_BLOOD_REFLECTIONS", 8 },
	{ "SYSTEM_FIELD_FILMIC_POSTFX", 9 },
	{ "SYSTEM_FIELD_CRT_POSTFX", 10 },
	{ "SYSTEM_FIELD_BRIGHTNESS", 11 },
	{ "SYSTEM_FIELD_VOLUME", 12 },
	{ "MAX_SYSTEM_FIELDS", 13 },
	{ NULL, 0 }
};

static enumValueInfo_t shellAreas_t_typeInfo[] = {
	{ "SHELL_AREA_INVALID", -1 },
	{ "SHELL_AREA_START", 0 },
	{ "SHELL_AREA_ROOT", 1 },
	{ "SHELL_AREA_DEV", 2 },
	{ "SHELL_AREA_CAMPAIGN", 3 },
	{ "SHELL_AREA_LOAD", 4 },
	{ "SHELL_AREA_SAVE", 5 },
	{ "SHELL_AREA_NEW_GAME", 6 },
	{ "SHELL_AREA_GAME_OPTIONS", 7 },
	{ "SHELL_AREA_SYSTEM_OPTIONS", 8 },
	{ "SHELL_AREA_MULTIPLAYER", 9 },
	{ "SHELL_AREA_GAME_LOBBY", 10 },
	{ "SHELL_AREA_PARTY_LOBBY", 11 },
	{ "SHELL_AREA_SETTINGS", 12 },
	{ "SHELL_AREA_AUDIO", 13 },
	{ "SHELL_AREA_VIDEO", 14 },
	{ "SHELL_AREA_KEYBOARD", 15 },
	{ "SHELL_AREA_CONTROLS", 16 },
	{ "SHELL_AREA_CONTROLLER_LAYOUT", 17 },
	{ "SHELL_AREA_GAMEPAD", 18 },
	{ "SHELL_AREA_PAUSE", 19 },
	{ "SHELL_AREA_LEADERBOARDS", 20 },
	{ "SHELL_AREA_PLAYSTATION", 21 },
	{ "SHELL_AREA_DIFFICULTY", 22 },
	{ "SHELL_AREA_RESOLUTION", 23 },
	{ "SHELL_AREA_MATCH_SETTINGS", 24 },
	{ "SHELL_AREA_MODE_SELECT", 25 },
	{ "SHELL_AREA_BROWSER", 26 },
	{ "SHELL_AREA_CREDITS", 27 },
	{ "SHELL_NUM_AREAS", 28 },
	{ NULL, 0 }
};

static enumValueInfo_t shellState_t_typeInfo[] = {
	{ "SHELL_STATE_INVALID", -1 },
	{ "SHELL_STATE_PRESS_START", 0 },
	{ "SHELL_STATE_IDLE", 1 },
	{ "SHELL_STATE_PARTY_LOBBY", 2 },
	{ "SHELL_STATE_GAME_LOBBY", 3 },
	{ "SHELL_STATE_PAUSED", 4 },
	{ "SHELL_STATE_CONNECTING", 5 },
	{ "SHELL_STATE_SEARCHING", 6 },
	{ "SHELL_STATE_LOADING", 7 },
	{ "SHELL_STATE_BUSY", 8 },
	{ "SHELL_STATE_IN_GAME", 9 },
	{ NULL, 0 }
};

static enumValueInfo_t pdaAreas_t_typeInfo[] = {
	{ "PDA_AREA_INVALID", -1 },
	{ "PDA_AREA_USER_DATA", 0 },
	{ "PDA_AREA_USER_EMAIL", 1 },
	{ "PDA_AREA_VIDEO_DISKS", 2 },
	{ "PDA_AREA_INVENTORY", 3 },
	{ "PDA_NUM_AREAS", 4 },
	{ NULL, 0 }
};

static enumValueInfo_t hudArea_t_typeInfo[] = {
	{ "HUD_AREA_INVALID", -1 },
	{ "HUD_AREA_PLAYING", 0 },
	{ "HUD_NUM_AREAS", 1 },
	{ NULL, 0 }
};

static enumValueInfo_t scoreboardArea_t_typeInfo[] = {
	{ "SCOREBOARD_AREA_INVALID", -1 },
	{ "SCOREBOARD_AREA_DEFAULT", 0 },
	{ "SCOREBOARD_AREA_TEAM", 1 },
	{ "SCOREBOARD_AREA_CTF", 2 },
	{ "SCOREBOARD_NUM_AREAS", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t pdaHandlerWidgets_t_typeInfo[] = {
	{ "PDA_WIDGET_NAV_BAR", 0 },
	{ "PDA_WIDGET_PDA_LIST", 1 },
	{ "PDA_WIDGET_PDA_LIST_SCROLLBAR", 2 },
	{ "PDA_WIDGET_CMD_BAR", 3 },
	{ NULL, 0 }
};

static enumValueInfo_t scoreboardHandlerWidgets_t_typeInfo[] = {
	{ "SCOREBOARD_WIDGET_CMD_BAR", 0 },
	{ NULL, 0 }
};

static enumValueInfo_t menuSounds_t_typeInfo[] = {
	{ "GUI_SOUND_MUSIC", 0 },
	{ "GUI_SOUND_SCROLL", 1 },
	{ "GUI_SOUND_ADVANCE", 2 },
	{ "GUI_SOUND_BACK", 3 },
	{ "GUI_SOUND_BUILD_ON", 4 },
	{ "GUI_SOUND_BUILD_OFF", 5 },
	{ "GUI_SOUND_FOCUS", 6 },
	{ "GUI_SOUND_ROLL_OVER", 7 },
	{ "GUI_SOUND_ROLL_OUT", 8 },
	{ "NUM_GUI_SOUNDS", 9 },
	{ NULL, 0 }
};

static enumValueInfo_t enum_91_typeInfo[] = {
	{ "OP_RETURN", 0 },
	{ "OP_UINC_F", 1 },
	{ "OP_UINCP_F", 2 },
	{ "OP_UDEC_F", 3 },
	{ "OP_UDECP_F", 4 },
	{ "OP_COMP_F", 5 },
	{ "OP_MUL_F", 6 },
	{ "OP_MUL_V", 7 },
	{ "OP_MUL_FV", 8 },
	{ "OP_MUL_VF", 9 },
	{ "OP_DIV_F", 10 },
	{ "OP_MOD_F", 11 },
	{ "OP_ADD_F", 12 },
	{ "OP_ADD_V", 13 },
	{ "OP_ADD_S", 14 },
	{ "OP_ADD_FS", 15 },
	{ "OP_ADD_SF", 16 },
	{ "OP_ADD_VS", 17 },
	{ "OP_ADD_SV", 18 },
	{ "OP_SUB_F", 19 },
	{ "OP_SUB_V", 20 },
	{ "OP_EQ_F", 21 },
	{ "OP_EQ_V", 22 },
	{ "OP_EQ_S", 23 },
	{ "OP_EQ_E", 24 },
	{ "OP_EQ_EO", 25 },
	{ "OP_EQ_OE", 26 },
	{ "OP_EQ_OO", 27 },
	{ "OP_NE_F", 28 },
	{ "OP_NE_V", 29 },
	{ "OP_NE_S", 30 },
	{ "OP_NE_E", 31 },
	{ "OP_NE_EO", 32 },
	{ "OP_NE_OE", 33 },
	{ "OP_NE_OO", 34 },
	{ "OP_LE", 35 },
	{ "OP_GE", 36 },
	{ "OP_LT", 37 },
	{ "OP_GT", 38 },
	{ "OP_INDIRECT_F", 39 },
	{ "OP_INDIRECT_V", 40 },
	{ "OP_INDIRECT_S", 41 },
	{ "OP_INDIRECT_ENT", 42 },
	{ "OP_INDIRECT_BOOL", 43 },
	{ "OP_INDIRECT_OBJ", 44 },
	{ "OP_ADDRESS", 45 },
	{ "OP_EVENTCALL", 46 },
	{ "OP_OBJECTCALL", 47 },
	{ "OP_SYSCALL", 48 },
	{ "OP_STORE_F", 49 },
	{ "OP_STORE_V", 50 },
	{ "OP_STORE_S", 51 },
	{ "OP_STORE_ENT", 52 },
	{ "OP_STORE_BOOL", 53 },
	{ "OP_STORE_OBJENT", 54 },
	{ "OP_STORE_OBJ", 55 },
	{ "OP_STORE_ENTOBJ", 56 },
	{ "OP_STORE_FTOS", 57 },
	{ "OP_STORE_BTOS", 58 },
	{ "OP_STORE_VTOS", 59 },
	{ "OP_STORE_FTOBOOL", 60 },
	{ "OP_STORE_BOOLTOF", 61 },
	{ "OP_STOREP_F", 62 },
	{ "OP_STOREP_V", 63 },
	{ "OP_STOREP_S", 64 },
	{ "OP_STOREP_ENT", 65 },
	{ "OP_STOREP_FLD", 66 },
	{ "OP_STOREP_BOOL", 67 },
	{ "OP_STOREP_OBJ", 68 },
	{ "OP_STOREP_OBJENT", 69 },
	{ "OP_STOREP_FTOS", 70 },
	{ "OP_STOREP_BTOS", 71 },
	{ "OP_STOREP_VTOS", 72 },
	{ "OP_STOREP_FTOBOOL", 73 },
	{ "OP_STOREP_BOOLTOF", 74 },
	{ "OP_UMUL_F", 75 },
	{ "OP_UMUL_V", 76 },
	{ "OP_UDIV_F", 77 },
	{ "OP_UDIV_V", 78 },
	{ "OP_UMOD_F", 79 },
	{ "OP_UADD_F", 80 },
	{ "OP_UADD_V", 81 },
	{ "OP_USUB_F", 82 },
	{ "OP_USUB_V", 83 },
	{ "OP_UAND_F", 84 },
	{ "OP_UOR_F", 85 },
	{ "OP_NOT_BOOL", 86 },
	{ "OP_NOT_F", 87 },
	{ "OP_NOT_V", 88 },
	{ "OP_NOT_S", 89 },
	{ "OP_NOT_ENT", 90 },
	{ "OP_NEG_F", 91 },
	{ "OP_NEG_V", 92 },
	{ "OP_INT_F", 93 },
	{ "OP_IF", 94 },
	{ "OP_IFNOT", 95 },
	{ "OP_CALL", 96 },
	{ "OP_THREAD", 97 },
	{ "OP_OBJTHREAD", 98 },
	{ "OP_PUSH_F", 99 },
	{ "OP_PUSH_V", 100 },
	{ "OP_PUSH_S", 101 },
	{ "OP_PUSH_ENT", 102 },
	{ "OP_PUSH_OBJ", 103 },
	{ "OP_PUSH_OBJENT", 104 },
	{ "OP_PUSH_FTOS", 105 },
	{ "OP_PUSH_BTOF", 106 },
	{ "OP_PUSH_FTOB", 107 },
	{ "OP_PUSH_VTOS", 108 },
	{ "OP_PUSH_BTOS", 109 },
	{ "OP_GOTO", 110 },
	{ "OP_AND", 111 },
	{ "OP_AND_BOOLF", 112 },
	{ "OP_AND_FBOOL", 113 },
	{ "OP_AND_BOOLBOOL", 114 },
	{ "OP_OR", 115 },
	{ "OP_OR_BOOLF", 116 },
	{ "OP_OR_FBOOL", 117 },
	{ "OP_OR_BOOLBOOL", 118 },
	{ "OP_BITAND", 119 },
	{ "OP_BITOR", 120 },
	{ "OP_BREAK", 121 },
	{ "OP_CONTINUE", 122 },
	{ "NUM_OPCODES", 123 },
	{ NULL, 0 }
};

static enumTypeInfo_t enumTypeInfo[] = {
	{ "stateResult_t", stateResult_t_typeInfo },
	{ "etype_t", etype_t_typeInfo },
	{ "idVarDef::initialized_t", idVarDef_initialized_t_typeInfo },
	{ "statement_s::enum_3", statement_s_enum_3_typeInfo },
	{ "jointModTransform_t", jointModTransform_t_typeInfo },
	{ "frameCommandType_t", frameCommandType_t_typeInfo },
	{ "AFJointModType_t", AFJointModType_t_typeInfo },
	{ "enum_7", enum_7_typeInfo },
	{ "pvsType_t", pvsType_t_typeInfo },
	{ "gameType_t", gameType_t_typeInfo },
	{ "flagStatus_t", flagStatus_t_typeInfo },
	{ "mpLeaderStatus_t", mpLeaderStatus_t_typeInfo },
	{ "snd_evt_t", snd_evt_t_typeInfo },
	{ "idMultiplayerGame::gameState_t", idMultiplayerGame_gameState_t_typeInfo },
	{ "idMultiplayerGame::msg_evt_t", idMultiplayerGame_msg_evt_t_typeInfo },
	{ "enum_15", enum_15_typeInfo },
	{ "gameState_t", gameState_t_typeInfo },
	{ "idEventQueue::outOfOrderBehaviour_t", idEventQueue_outOfOrderBehaviour_t_typeInfo },
	{ "slowmoState_t", slowmoState_t_typeInfo },
	{ "idGameLocal::gameDemoCommand_t", idGameLocal_gameDemoCommand_t_typeInfo },
	{ "gameSoundChannel_t", gameSoundChannel_t_typeInfo },
	{ "forceFieldType", forceFieldType_typeInfo },
	{ "forceFieldApplyType", forceFieldApplyType_typeInfo },
	{ "monsterMoveResult_t", monsterMoveResult_t_typeInfo },
	{ "pmtype_t", pmtype_t_typeInfo },
	{ "waterLevel_t", waterLevel_t_typeInfo },
	{ "constraintType_t", constraintType_t_typeInfo },
	{ "enum_27", enum_27_typeInfo },
	{ "signalNum_t", signalNum_t_typeInfo },
	{ "idEntity::enum_29", idEntity_enum_29_typeInfo },
	{ "idEntity::interpolationBehavior_t", idEntity_interpolationBehavior_t_typeInfo },
	{ "idAnimatedEntity::enum_31", idAnimatedEntity_enum_31_typeInfo },
	{ "idPlayerStart::enum_32", idPlayerStart_enum_32_typeInfo },
	{ "idProjectile::enum_33", idProjectile_enum_33_typeInfo },
	{ "idProjectile::projectileState_t", idProjectile_projectileState_t_typeInfo },
	{ "idWeapon::enum_35", idWeapon_enum_35_typeInfo },
	{ "idLight::enum_36", idLight_enum_36_typeInfo },
	{ "EnvironmentProbe::enum_37", EnvironmentProbe_enum_37_typeInfo },
	{ "itemGiveFlags_t", itemGiveFlags_t_typeInfo },
	{ "idItem::enum_39", idItem_enum_39_typeInfo },
	{ "FxFader::enum_40", FxFader_enum_40_typeInfo },
	{ "playerIconType_t", playerIconType_t_typeInfo },
	{ "achievement_t", achievement_t_typeInfo },
	{ "gameExpansionType_t", gameExpansionType_t_typeInfo },
	{ "enum_44", enum_44_typeInfo },
	{ "enum_45", enum_45_typeInfo },
	{ "enum_46", enum_46_typeInfo },
	{ "idPlayer::enum_47", idPlayer_enum_47_typeInfo },
	{ "idMover::moveStage_t", idMover_moveStage_t_typeInfo },
	{ "idMover::moverCommand_t", idMover_moverCommand_t_typeInfo },
	{ "idMover::moverDir_t", idMover_moverDir_t_typeInfo },
	{ "idElevator::elevatorState_t", idElevator_elevatorState_t_typeInfo },
	{ "moverState_t", moverState_t_typeInfo },
	{ "idExplodingBarrel::enum_53", idExplodingBarrel_enum_53_typeInfo },
	{ "idExplodingBarrel::explode_state_t", idExplodingBarrel_explode_state_t_typeInfo },
	{ "idSecurityCamera::enum_55", idSecurityCamera_enum_55_typeInfo },
	{ "idBrittleFracture::enum_56", idBrittleFracture_enum_56_typeInfo },
	{ "moveType_t", moveType_t_typeInfo },
	{ "moveCommand_t", moveCommand_t_typeInfo },
	{ "talkState_t", talkState_t_typeInfo },
	{ "moveStatus_t", moveStatus_t_typeInfo },
	{ "stopEvent_t", stopEvent_t_typeInfo },
	{ "botMoveFlags_t", botMoveFlags_t_typeInfo },
	{ "botChat_t", botChat_t_typeInfo },
	{ "menuOption_t", menuOption_t_typeInfo },
	{ "widgetEvent_t", widgetEvent_t_typeInfo },
	{ "scrollType_t", scrollType_t_typeInfo },
	{ "widgetAction_t", widgetAction_t_typeInfo },
	{ "actionHandler_t", actionHandler_t_typeInfo },
	{ "idMenuWidget::widgetState_t", idMenuWidget_widgetState_t_typeInfo },
	{ "idMenuWidget_Button::animState_t", idMenuWidget_Button_animState_t_typeInfo },
	{ "idMenuWidget_NavButton::navWidgetState_t", idMenuWidget_NavButton_navWidgetState_t_typeInfo },
	{ "idMenuWidget_CommandBar::button_t", idMenuWidget_CommandBar_button_t_typeInfo },
	{ "idMenuWidget_CommandBar::alignment_t", idMenuWidget_CommandBar_alignment_t_typeInfo },
	{ "mainMenuTransition_t", mainMenuTransition_t_typeInfo },
	{ "cursorState_t", cursorState_t_typeInfo },
	{ "leaderboardFilterMode_t", leaderboardFilterMode_t_typeInfo },
	{ "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings::gameSettingFields_t", idMenuScreen_Shell_GameOptions_idMenuDataSource_GameSettings_gameSettingFields_t_typeInfo },
	{ "idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings::matchSettingFields_t", idMenuScreen_Shell_MatchSettings_idMenuDataSource_MatchSettings_matchSettingFields_t_typeInfo },
	{ "idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings::controlSettingFields_t", idMenuScreen_Shell_Controls_idMenuDataSource_ControlSettings_controlSettingFields_t_typeInfo },
	{ "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings::controlSettingFields_t", idMenuScreen_Shell_Gamepad_idMenuDataSource_GamepadSettings_controlSettingFields_t_typeInfo },
	{ "idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings::controlSettingFields_t", idMenuScreen_Shell_ControllerLayout_idMenuDataSource_LayoutSettings_controlSettingFields_t_typeInfo },
	{ "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings::systemSettingFields_t", idMenuScreen_Shell_SystemOptions_idMenuDataSource_SystemSettings_systemSettingFields_t_typeInfo },
	{ "shellAreas_t", shellAreas_t_typeInfo },
	{ "shellState_t", shellState_t_typeInfo },
	{ "pdaAreas_t", pdaAreas_t_typeInfo },
	{ "hudArea_t", hudArea_t_typeInfo },
	{ "scoreboardArea_t", scoreboardArea_t_typeInfo },
	{ "pdaHandlerWidgets_t", pdaHandlerWidgets_t_typeInfo },
	{ "scoreboardHandlerWidgets_t", scoreboardHandlerWidgets_t_typeInfo },
	{ "menuSounds_t", menuSounds_t_typeInfo },
	{ "enum_91", enum_91_typeInfo },
	{ NULL, NULL }
};

static classVariableInfo_t idEventDef_typeInfo[] = {
	{ ": const char *", "name", (intptr_t)(&((idEventDef *)0)->name), sizeof( ((idEventDef *)0)->name ) },
	{ "const char *", "formatspec", (intptr_t)(&((idEventDef *)0)->formatspec), sizeof( ((idEventDef *)0)->formatspec ) },
	{ "unsigned int", "formatspecIndex", (intptr_t)(&((idEventDef *)0)->formatspecIndex), sizeof( ((idEventDef *)0)->formatspecIndex ) },
	{ "int", "returnType", (intptr_t)(&((idEventDef *)0)->returnType), sizeof( ((idEventDef *)0)->returnType ) },
	{ "int", "numargs", (intptr_t)(&((idEventDef *)0)->numargs), sizeof( ((idEventDef *)0)->numargs ) },
	{ "size_t", "argsize", (intptr_t)(&((idEventDef *)0)->argsize), sizeof( ((idEventDef *)0)->argsize ) },
	{ "int[8]", "argOffset", (intptr_t)(&((idEventDef *)0)->argOffset), sizeof( ((idEventDef *)0)->argOffset ) },
	{ "int", "eventnum", (intptr_t)(&((idEventDef *)0)->eventnum), sizeof( ((idEventDef *)0)->eventnum ) },
	{ "const idEventDef *", "next", (intptr_t)(&((idEventDef *)0)->next), sizeof( ((idEventDef *)0)->next ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEvent_typeInfo[] = {
	{ ": const idEventDef *", "eventdef", (intptr_t)(&((idEvent *)0)->eventdef), sizeof( ((idEvent *)0)->eventdef ) },
	{ "byte *", "data", (intptr_t)(&((idEvent *)0)->data), sizeof( ((idEvent *)0)->data ) },
	{ "int", "time", (intptr_t)(&((idEvent *)0)->time), sizeof( ((idEvent *)0)->time ) },
	{ "idClass *", "object", (intptr_t)(&((idEvent *)0)->object), sizeof( ((idEvent *)0)->object ) },
	{ "const idTypeInfo *", "typeinfo", (intptr_t)(&((idEvent *)0)->typeinfo), sizeof( ((idEvent *)0)->typeinfo ) },
	{ "idLinkList < idEvent >", "eventNode", (intptr_t)(&((idEvent *)0)->eventNode), sizeof( ((idEvent *)0)->eventNode ) },
	{ NULL, 0 }
};

static classVariableInfo_t stateParms_t_typeInfo[] = {
	{ "int", "blendFrames", (intptr_t)(&((stateParms_t *)0)->blendFrames), sizeof( ((stateParms_t *)0)->blendFrames ) },
	{ "int", "time", (intptr_t)(&((stateParms_t *)0)->time), sizeof( ((stateParms_t *)0)->time ) },
	{ "int", "stage", (intptr_t)(&((stateParms_t *)0)->stage), sizeof( ((stateParms_t *)0)->stage ) },
	{ "int", "substage", (intptr_t)(&((stateParms_t *)0)->substage), sizeof( ((stateParms_t *)0)->substage ) },
	{ "float", "param1", (intptr_t)(&((stateParms_t *)0)->param1), sizeof( ((stateParms_t *)0)->param1 ) },
	{ "float", "param2", (intptr_t)(&((stateParms_t *)0)->param2), sizeof( ((stateParms_t *)0)->param2 ) },
	{ "float", "subparam1", (intptr_t)(&((stateParms_t *)0)->subparam1), sizeof( ((stateParms_t *)0)->subparam1 ) },
	{ "float", "subparam2", (intptr_t)(&((stateParms_t *)0)->subparam2), sizeof( ((stateParms_t *)0)->subparam2 ) },
	{ NULL, 0 }
};

static classVariableInfo_t stateCall_t_typeInfo[] = {
	{ "idStr", "state", (intptr_t)(&((stateCall_t *)0)->state), sizeof( ((stateCall_t *)0)->state ) },
	{ "idLinkList < stateCall_t >", "node", (intptr_t)(&((stateCall_t *)0)->node), sizeof( ((stateCall_t *)0)->node ) },
	{ "int", "flags", (intptr_t)(&((stateCall_t *)0)->flags), sizeof( ((stateCall_t *)0)->flags ) },
	{ "int", "delay", (intptr_t)(&((stateCall_t *)0)->delay), sizeof( ((stateCall_t *)0)->delay ) },
	{ "stateParms_t", "parms", (intptr_t)(&((stateCall_t *)0)->parms), sizeof( ((stateCall_t *)0)->parms ) },
	{ NULL, 0 }
};

static classVariableInfo_t rvStateThread_flags_typeInfo[] = {
//	{ "bool", "stateCleared", (intptr_t)(&((rvStateThread::flags *)0)->stateCleared), sizeof( ((rvStateThread::flags *)0)->stateCleared ) },
//	{ "bool", "stateInterrupted", (intptr_t)(&((rvStateThread::flags *)0)->stateInterrupted), sizeof( ((rvStateThread::flags *)0)->stateInterrupted ) },
//	{ "bool", "executing", (intptr_t)(&((rvStateThread::flags *)0)->executing), sizeof( ((rvStateThread::flags *)0)->executing ) },
	{ NULL, 0 }
};

static classVariableInfo_t rvStateThread_typeInfo[] = {
	{ "rvStateThread::flags", "fl", (intptr_t)(&((rvStateThread *)0)->fl), sizeof( ((rvStateThread *)0)->fl ) },
	{ "idStr", "name", (intptr_t)(&((rvStateThread *)0)->name), sizeof( ((rvStateThread *)0)->name ) },
	{ "idClass *", "owner", (intptr_t)(&((rvStateThread *)0)->owner), sizeof( ((rvStateThread *)0)->owner ) },
	{ "idLinkList < stateCall_t >", "states", (intptr_t)(&((rvStateThread *)0)->states), sizeof( ((rvStateThread *)0)->states ) },
	{ "idLinkList < stateCall_t >", "interrupted", (intptr_t)(&((rvStateThread *)0)->interrupted), sizeof( ((rvStateThread *)0)->interrupted ) },
	{ "stateCall_t *", "insertAfter", (intptr_t)(&((rvStateThread *)0)->insertAfter), sizeof( ((rvStateThread *)0)->insertAfter ) },
	{ "stateResult_t", "lastResult", (intptr_t)(&((rvStateThread *)0)->lastResult), sizeof( ((rvStateThread *)0)->lastResult ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEventArg_typeInfo[] = {
	{ ": int", "type", (intptr_t)(&((idEventArg *)0)->type), sizeof( ((idEventArg *)0)->type ) },
	{ "intptr_t", "value", (intptr_t)(&((idEventArg *)0)->value), sizeof( ((idEventArg *)0)->value ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAllocError_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idClass_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTypeInfo_typeInfo[] = {
	{ ": const char *", "classname", (intptr_t)(&((idTypeInfo *)0)->classname), sizeof( ((idTypeInfo *)0)->classname ) },
	{ "const char *", "superclass", (intptr_t)(&((idTypeInfo *)0)->superclass), sizeof( ((idTypeInfo *)0)->superclass ) },
	{ "idEventFunc < idClass > *", "eventCallbacks", (intptr_t)(&((idTypeInfo *)0)->eventCallbacks), sizeof( ((idTypeInfo *)0)->eventCallbacks ) },
	{ "eventCallback_t *", "eventMap", (intptr_t)(&((idTypeInfo *)0)->eventMap), sizeof( ((idTypeInfo *)0)->eventMap ) },
	{ "idTypeInfo *", "super", (intptr_t)(&((idTypeInfo *)0)->super), sizeof( ((idTypeInfo *)0)->super ) },
	{ "idTypeInfo *", "next", (intptr_t)(&((idTypeInfo *)0)->next), sizeof( ((idTypeInfo *)0)->next ) },
	{ "bool", "freeEventMap", (intptr_t)(&((idTypeInfo *)0)->freeEventMap), sizeof( ((idTypeInfo *)0)->freeEventMap ) },
	{ "int", "typeNum", (intptr_t)(&((idTypeInfo *)0)->typeNum), sizeof( ((idTypeInfo *)0)->typeNum ) },
	{ "int", "lastChild", (intptr_t)(&((idTypeInfo *)0)->lastChild), sizeof( ((idTypeInfo *)0)->lastChild ) },
	{ "idHierarchy < idTypeInfo >", "node", (intptr_t)(&((idTypeInfo *)0)->node), sizeof( ((idTypeInfo *)0)->node ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSaveGame_stringTableIndex_s_typeInfo[] = {
	{ "idStr", "string", (intptr_t)(&((idSaveGame::stringTableIndex_s *)0)->string), sizeof( ((idSaveGame::stringTableIndex_s *)0)->string ) },
	{ "int", "offset", (intptr_t)(&((idSaveGame::stringTableIndex_s *)0)->offset), sizeof( ((idSaveGame::stringTableIndex_s *)0)->offset ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSaveGame_typeInfo[] = {
	{ ": idFile *", "file", (intptr_t)(&((idSaveGame *)0)->file), sizeof( ((idSaveGame *)0)->file ) },
	{ "idFile *", "stringFile", (intptr_t)(&((idSaveGame *)0)->stringFile), sizeof( ((idSaveGame *)0)->stringFile ) },
	{ "idCompressor *", "compressor", (intptr_t)(&((idSaveGame *)0)->compressor), sizeof( ((idSaveGame *)0)->compressor ) },
	{ "idList < const idClass * >", "objects", (intptr_t)(&((idSaveGame *)0)->objects), sizeof( ((idSaveGame *)0)->objects ) },
	{ "int", "version", (intptr_t)(&((idSaveGame *)0)->version), sizeof( ((idSaveGame *)0)->version ) },
	{ "idHashIndex", "stringHash", (intptr_t)(&((idSaveGame *)0)->stringHash), sizeof( ((idSaveGame *)0)->stringHash ) },
	{ "idList < stringTableIndex_s >", "stringTable", (intptr_t)(&((idSaveGame *)0)->stringTable), sizeof( ((idSaveGame *)0)->stringTable ) },
	{ "int", "curStringTableOffset", (intptr_t)(&((idSaveGame *)0)->curStringTableOffset), sizeof( ((idSaveGame *)0)->curStringTableOffset ) },
	{ NULL, 0 }
};

static classVariableInfo_t idRestoreGame_typeInfo[] = {
	{ ": idFile *", "file", (intptr_t)(&((idRestoreGame *)0)->file), sizeof( ((idRestoreGame *)0)->file ) },
	{ "idFile *", "stringFile", (intptr_t)(&((idRestoreGame *)0)->stringFile), sizeof( ((idRestoreGame *)0)->stringFile ) },
	{ "idList < idClass * , TAG_SAVEGAMES >", "objects", (intptr_t)(&((idRestoreGame *)0)->objects), sizeof( ((idRestoreGame *)0)->objects ) },
	{ "int", "version", (intptr_t)(&((idRestoreGame *)0)->version), sizeof( ((idRestoreGame *)0)->version ) },
	{ "int", "stringTableOffset", (intptr_t)(&((idRestoreGame *)0)->stringTableOffset), sizeof( ((idRestoreGame *)0)->stringTableOffset ) },
	{ NULL, 0 }
};

static classVariableInfo_t function_t_typeInfo[] = {
	{ ": idStr", "name", (intptr_t)(&((function_t *)0)->name), sizeof( ((function_t *)0)->name ) },
	{ ": const idEventDef *", "eventdef", (intptr_t)(&((function_t *)0)->eventdef), sizeof( ((function_t *)0)->eventdef ) },
	{ "idVarDef *", "def", (intptr_t)(&((function_t *)0)->def), sizeof( ((function_t *)0)->def ) },
	{ "const idTypeDef *", "type", (intptr_t)(&((function_t *)0)->type), sizeof( ((function_t *)0)->type ) },
	{ "int", "firstStatement", (intptr_t)(&((function_t *)0)->firstStatement), sizeof( ((function_t *)0)->firstStatement ) },
	{ "int", "numStatements", (intptr_t)(&((function_t *)0)->numStatements), sizeof( ((function_t *)0)->numStatements ) },
	{ "int", "parmTotal", (intptr_t)(&((function_t *)0)->parmTotal), sizeof( ((function_t *)0)->parmTotal ) },
	{ "int", "locals", (intptr_t)(&((function_t *)0)->locals), sizeof( ((function_t *)0)->locals ) },
	{ "int", "filenum", (intptr_t)(&((function_t *)0)->filenum), sizeof( ((function_t *)0)->filenum ) },
	{ "idList < int , TAG_SCRIPT >", "parmSize", (intptr_t)(&((function_t *)0)->parmSize), sizeof( ((function_t *)0)->parmSize ) },
	{ NULL, 0 }
};

static classVariableInfo_t eval_t_typeInfo[] = {
	{ "const char *", "stringPtr", (intptr_t)(&((eval_t *)0)->stringPtr), sizeof( ((eval_t *)0)->stringPtr ) },
	{ "float", "_float", (intptr_t)(&((eval_t *)0)->_float), sizeof( ((eval_t *)0)->_float ) },
	{ "float[3]", "vector", (intptr_t)(&((eval_t *)0)->vector), sizeof( ((eval_t *)0)->vector ) },
	{ "function_t *", "function", (intptr_t)(&((eval_t *)0)->function), sizeof( ((eval_t *)0)->function ) },
	{ "int", "_int", (intptr_t)(&((eval_t *)0)->_int), sizeof( ((eval_t *)0)->_int ) },
	{ "int", "entity", (intptr_t)(&((eval_t *)0)->entity), sizeof( ((eval_t *)0)->entity ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTypeDef_typeInfo[] = {
	{ ": etype_t", "type", (intptr_t)(&((idTypeDef *)0)->type), sizeof( ((idTypeDef *)0)->type ) },
	{ "idStr", "name", (intptr_t)(&((idTypeDef *)0)->name), sizeof( ((idTypeDef *)0)->name ) },
	{ "int", "size", (intptr_t)(&((idTypeDef *)0)->size), sizeof( ((idTypeDef *)0)->size ) },
	{ "idTypeDef *", "auxType", (intptr_t)(&((idTypeDef *)0)->auxType), sizeof( ((idTypeDef *)0)->auxType ) },
	{ "idList < idTypeDef * , TAG_SCRIPT >", "parmTypes", (intptr_t)(&((idTypeDef *)0)->parmTypes), sizeof( ((idTypeDef *)0)->parmTypes ) },
	{ "idStrList", "parmNames", (intptr_t)(&((idTypeDef *)0)->parmNames), sizeof( ((idTypeDef *)0)->parmNames ) },
	{ "idList < const function_t * , TAG_SCRIPT >", "functions", (intptr_t)(&((idTypeDef *)0)->functions), sizeof( ((idTypeDef *)0)->functions ) },
	{ ": idVarDef *", "def", (intptr_t)(&((idTypeDef *)0)->def), sizeof( ((idTypeDef *)0)->def ) },
	{ NULL, 0 }
};

static classVariableInfo_t idScriptObject_typeInfo[] = {
	{ ": idTypeDef *", "type", (intptr_t)(&((idScriptObject *)0)->type), sizeof( ((idScriptObject *)0)->type ) },
	{ ": byte *", "data", (intptr_t)(&((idScriptObject *)0)->data), sizeof( ((idScriptObject *)0)->data ) },
	{ NULL, 0 }
};

static classVariableInfo_t idCompileError_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t varEval_t_typeInfo[] = {
	{ "idScriptObject * *", "objectPtrPtr", (intptr_t)(&((varEval_t *)0)->objectPtrPtr), sizeof( ((varEval_t *)0)->objectPtrPtr ) },
	{ "char *", "stringPtr", (intptr_t)(&((varEval_t *)0)->stringPtr), sizeof( ((varEval_t *)0)->stringPtr ) },
	{ "float *", "floatPtr", (intptr_t)(&((varEval_t *)0)->floatPtr), sizeof( ((varEval_t *)0)->floatPtr ) },
	{ "idVec3 *", "vectorPtr", (intptr_t)(&((varEval_t *)0)->vectorPtr), sizeof( ((varEval_t *)0)->vectorPtr ) },
	{ "function_t *", "functionPtr", (intptr_t)(&((varEval_t *)0)->functionPtr), sizeof( ((varEval_t *)0)->functionPtr ) },
	{ "int *", "intPtr", (intptr_t)(&((varEval_t *)0)->intPtr), sizeof( ((varEval_t *)0)->intPtr ) },
	{ "byte *", "bytePtr", (intptr_t)(&((varEval_t *)0)->bytePtr), sizeof( ((varEval_t *)0)->bytePtr ) },
	{ "int *", "entityNumberPtr", (intptr_t)(&((varEval_t *)0)->entityNumberPtr), sizeof( ((varEval_t *)0)->entityNumberPtr ) },
	{ "int", "virtualFunction", (intptr_t)(&((varEval_t *)0)->virtualFunction), sizeof( ((varEval_t *)0)->virtualFunction ) },
	{ "int", "jumpOffset", (intptr_t)(&((varEval_t *)0)->jumpOffset), sizeof( ((varEval_t *)0)->jumpOffset ) },
	{ "int", "stackOffset", (intptr_t)(&((varEval_t *)0)->stackOffset), sizeof( ((varEval_t *)0)->stackOffset ) },
	{ "int", "argSize", (intptr_t)(&((varEval_t *)0)->argSize), sizeof( ((varEval_t *)0)->argSize ) },
	{ "varEval_s *", "evalPtr", (intptr_t)(&((varEval_t *)0)->evalPtr), sizeof( ((varEval_t *)0)->evalPtr ) },
	{ "int", "ptrOffset", (intptr_t)(&((varEval_t *)0)->ptrOffset), sizeof( ((varEval_t *)0)->ptrOffset ) },
	{ NULL, 0 }
};

static classVariableInfo_t idVarDef_typeInfo[] = {
	{ ": int", "num", (intptr_t)(&((idVarDef *)0)->num), sizeof( ((idVarDef *)0)->num ) },
	{ "varEval_t", "value", (intptr_t)(&((idVarDef *)0)->value), sizeof( ((idVarDef *)0)->value ) },
	{ "idVarDef *", "scope", (intptr_t)(&((idVarDef *)0)->scope), sizeof( ((idVarDef *)0)->scope ) },
	{ "int", "numUsers", (intptr_t)(&((idVarDef *)0)->numUsers), sizeof( ((idVarDef *)0)->numUsers ) },
	{ "initialized_t", "initialized", (intptr_t)(&((idVarDef *)0)->initialized), sizeof( ((idVarDef *)0)->initialized ) },
	{ ": idTypeDef *", "typeDef", (intptr_t)(&((idVarDef *)0)->typeDef), sizeof( ((idVarDef *)0)->typeDef ) },
	{ "idVarDefName *", "name", (intptr_t)(&((idVarDef *)0)->name), sizeof( ((idVarDef *)0)->name ) },
	{ "idVarDef *", "next", (intptr_t)(&((idVarDef *)0)->next), sizeof( ((idVarDef *)0)->next ) },
	{ NULL, 0 }
};

static classVariableInfo_t idVarDefName_typeInfo[] = {
	{ ": idStr", "name", (intptr_t)(&((idVarDefName *)0)->name), sizeof( ((idVarDefName *)0)->name ) },
	{ "idVarDef *", "defs", (intptr_t)(&((idVarDefName *)0)->defs), sizeof( ((idVarDefName *)0)->defs ) },
	{ NULL, 0 }
};

static classVariableInfo_t statement_t_typeInfo[] = {
	{ "unsigned short", "op", (intptr_t)(&((statement_t *)0)->op), sizeof( ((statement_t *)0)->op ) },
	{ "unsigned short", "flags", (intptr_t)(&((statement_t *)0)->flags), sizeof( ((statement_t *)0)->flags ) },
	{ "unsigned short", "linenumber", (intptr_t)(&((statement_t *)0)->linenumber), sizeof( ((statement_t *)0)->linenumber ) },
	{ "unsigned short", "file", (intptr_t)(&((statement_t *)0)->file), sizeof( ((statement_t *)0)->file ) },
	{ "idVarDef *", "a", (intptr_t)(&((statement_t *)0)->a), sizeof( ((statement_t *)0)->a ) },
	{ "idVarDef *", "b", (intptr_t)(&((statement_t *)0)->b), sizeof( ((statement_t *)0)->b ) },
	{ "idVarDef *", "c", (intptr_t)(&((statement_t *)0)->c), sizeof( ((statement_t *)0)->c ) },
	{ NULL, 0 }
};

static classVariableInfo_t idProgram_typeInfo[] = {
	{ ": idStrList", "fileList", (intptr_t)(&((idProgram *)0)->fileList), sizeof( ((idProgram *)0)->fileList ) },
	{ "idStr", "filename", (intptr_t)(&((idProgram *)0)->filename), sizeof( ((idProgram *)0)->filename ) },
	{ "int", "filenum", (intptr_t)(&((idProgram *)0)->filenum), sizeof( ((idProgram *)0)->filenum ) },
	{ "int", "numVariables", (intptr_t)(&((idProgram *)0)->numVariables), sizeof( ((idProgram *)0)->numVariables ) },
	{ "byte[593216]", "variables", (intptr_t)(&((idProgram *)0)->variables), sizeof( ((idProgram *)0)->variables ) },
	{ "idStaticList < byte , 296608 * 2 >", "variableDefaults", (intptr_t)(&((idProgram *)0)->variableDefaults), sizeof( ((idProgram *)0)->variableDefaults ) },
	{ "idStaticList < function_t , 3584 >", "functions", (intptr_t)(&((idProgram *)0)->functions), sizeof( ((idProgram *)0)->functions ) },
	{ "idStaticList < statement_t , 131072 >", "statements", (intptr_t)(&((idProgram *)0)->statements), sizeof( ((idProgram *)0)->statements ) },
	{ "idList < idTypeDef * , TAG_SCRIPT >", "types", (intptr_t)(&((idProgram *)0)->types), sizeof( ((idProgram *)0)->types ) },
	{ "idHashIndex", "typesHash", (intptr_t)(&((idProgram *)0)->typesHash), sizeof( ((idProgram *)0)->typesHash ) },
	{ "idList < idVarDefName * , TAG_SCRIPT >", "varDefNames", (intptr_t)(&((idProgram *)0)->varDefNames), sizeof( ((idProgram *)0)->varDefNames ) },
	{ "idHashIndex", "varDefNameHash", (intptr_t)(&((idProgram *)0)->varDefNameHash), sizeof( ((idProgram *)0)->varDefNameHash ) },
	{ "idList < idVarDef * , TAG_SCRIPT >", "varDefs", (intptr_t)(&((idProgram *)0)->varDefs), sizeof( ((idProgram *)0)->varDefs ) },
	{ "idVarDef *", "sysDef", (intptr_t)(&((idProgram *)0)->sysDef), sizeof( ((idProgram *)0)->sysDef ) },
	{ "int", "top_functions", (intptr_t)(&((idProgram *)0)->top_functions), sizeof( ((idProgram *)0)->top_functions ) },
	{ "int", "top_statements", (intptr_t)(&((idProgram *)0)->top_statements), sizeof( ((idProgram *)0)->top_statements ) },
	{ "int", "top_types", (intptr_t)(&((idProgram *)0)->top_types), sizeof( ((idProgram *)0)->top_types ) },
	{ "int", "top_defs", (intptr_t)(&((idProgram *)0)->top_defs), sizeof( ((idProgram *)0)->top_defs ) },
	{ "int", "top_files", (intptr_t)(&((idProgram *)0)->top_files), sizeof( ((idProgram *)0)->top_files ) },
	{ ": idVarDef *", "returnDef", (intptr_t)(&((idProgram *)0)->returnDef), sizeof( ((idProgram *)0)->returnDef ) },
	{ "idVarDef *", "returnStringDef", (intptr_t)(&((idProgram *)0)->returnStringDef), sizeof( ((idProgram *)0)->returnStringDef ) },
	{ NULL, 0 }
};

static classVariableInfo_t frameBlend_t_typeInfo[] = {
	{ "int", "cycleCount", (intptr_t)(&((frameBlend_t *)0)->cycleCount), sizeof( ((frameBlend_t *)0)->cycleCount ) },
	{ "int", "frame1", (intptr_t)(&((frameBlend_t *)0)->frame1), sizeof( ((frameBlend_t *)0)->frame1 ) },
	{ "int", "frame2", (intptr_t)(&((frameBlend_t *)0)->frame2), sizeof( ((frameBlend_t *)0)->frame2 ) },
	{ "float", "frontlerp", (intptr_t)(&((frameBlend_t *)0)->frontlerp), sizeof( ((frameBlend_t *)0)->frontlerp ) },
	{ "float", "backlerp", (intptr_t)(&((frameBlend_t *)0)->backlerp), sizeof( ((frameBlend_t *)0)->backlerp ) },
	{ NULL, 0 }
};

static classVariableInfo_t jointAnimInfo_t_typeInfo[] = {
	{ "int", "nameIndex", (intptr_t)(&((jointAnimInfo_t *)0)->nameIndex), sizeof( ((jointAnimInfo_t *)0)->nameIndex ) },
	{ "int", "parentNum", (intptr_t)(&((jointAnimInfo_t *)0)->parentNum), sizeof( ((jointAnimInfo_t *)0)->parentNum ) },
	{ "int", "animBits", (intptr_t)(&((jointAnimInfo_t *)0)->animBits), sizeof( ((jointAnimInfo_t *)0)->animBits ) },
	{ "int", "firstComponent", (intptr_t)(&((jointAnimInfo_t *)0)->firstComponent), sizeof( ((jointAnimInfo_t *)0)->firstComponent ) },
	{ NULL, 0 }
};

static classVariableInfo_t jointInfo_t_typeInfo[] = {
	{ "jointHandle_t", "num", (intptr_t)(&((jointInfo_t *)0)->num), sizeof( ((jointInfo_t *)0)->num ) },
	{ "jointHandle_t", "parentNum", (intptr_t)(&((jointInfo_t *)0)->parentNum), sizeof( ((jointInfo_t *)0)->parentNum ) },
	{ "int", "channel", (intptr_t)(&((jointInfo_t *)0)->channel), sizeof( ((jointInfo_t *)0)->channel ) },
	{ NULL, 0 }
};

static classVariableInfo_t jointMod_t_typeInfo[] = {
	{ "jointHandle_t", "jointnum", (intptr_t)(&((jointMod_t *)0)->jointnum), sizeof( ((jointMod_t *)0)->jointnum ) },
	{ "idMat3", "mat", (intptr_t)(&((jointMod_t *)0)->mat), sizeof( ((jointMod_t *)0)->mat ) },
	{ "idVec3", "pos", (intptr_t)(&((jointMod_t *)0)->pos), sizeof( ((jointMod_t *)0)->pos ) },
	{ "jointModTransform_t", "transform_pos", (intptr_t)(&((jointMod_t *)0)->transform_pos), sizeof( ((jointMod_t *)0)->transform_pos ) },
	{ "jointModTransform_t", "transform_axis", (intptr_t)(&((jointMod_t *)0)->transform_axis), sizeof( ((jointMod_t *)0)->transform_axis ) },
	{ NULL, 0 }
};

static classVariableInfo_t frameLookup_t_typeInfo[] = {
	{ "int", "num", (intptr_t)(&((frameLookup_t *)0)->num), sizeof( ((frameLookup_t *)0)->num ) },
	{ "int", "firstCommand", (intptr_t)(&((frameLookup_t *)0)->firstCommand), sizeof( ((frameLookup_t *)0)->firstCommand ) },
	{ NULL, 0 }
};

static classVariableInfo_t class_29_class_29_typeInfo[] = {
//	{ "const idSoundShader *", "soundShader", (intptr_t)(&((class_29::class_29 *)0)->soundShader), sizeof( ((class_29::class_29 *)0)->soundShader ) },
//	{ "const function_t *", "function", (intptr_t)(&((class_29::class_29 *)0)->function), sizeof( ((class_29::class_29 *)0)->function ) },
//	{ "const idDeclSkin *", "skin", (intptr_t)(&((class_29::class_29 *)0)->skin), sizeof( ((class_29::class_29 *)0)->skin ) },
//	{ "int", "index", (intptr_t)(&((class_29::class_29 *)0)->index), sizeof( ((class_29::class_29 *)0)->index ) },
	{ NULL, 0 }
};

static classVariableInfo_t frameCommand_t_typeInfo[] = {
	{ "frameCommandType_t", "type", (intptr_t)(&((frameCommand_t *)0)->type), sizeof( ((frameCommand_t *)0)->type ) },
	{ "idStr *", "string", (intptr_t)(&((frameCommand_t *)0)->string), sizeof( ((frameCommand_t *)0)->string ) },
	{ NULL, 0 }
};

static classVariableInfo_t animFlags_t_typeInfo[] = {
//	{ "bool", "prevent_idle_override", (intptr_t)(&((animFlags_t *)0)->prevent_idle_override), sizeof( ((animFlags_t *)0)->prevent_idle_override ) },
//	{ "bool", "random_cycle_start", (intptr_t)(&((animFlags_t *)0)->random_cycle_start), sizeof( ((animFlags_t *)0)->random_cycle_start ) },
//	{ "bool", "ai_no_turn", (intptr_t)(&((animFlags_t *)0)->ai_no_turn), sizeof( ((animFlags_t *)0)->ai_no_turn ) },
//	{ "bool", "anim_turn", (intptr_t)(&((animFlags_t *)0)->anim_turn), sizeof( ((animFlags_t *)0)->anim_turn ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMD5Anim_typeInfo[] = {
	{ ": int", "numFrames", (intptr_t)(&((idMD5Anim *)0)->numFrames), sizeof( ((idMD5Anim *)0)->numFrames ) },
	{ "int", "frameRate", (intptr_t)(&((idMD5Anim *)0)->frameRate), sizeof( ((idMD5Anim *)0)->frameRate ) },
	{ "int", "animLength", (intptr_t)(&((idMD5Anim *)0)->animLength), sizeof( ((idMD5Anim *)0)->animLength ) },
	{ "int", "numJoints", (intptr_t)(&((idMD5Anim *)0)->numJoints), sizeof( ((idMD5Anim *)0)->numJoints ) },
	{ "int", "numAnimatedComponents", (intptr_t)(&((idMD5Anim *)0)->numAnimatedComponents), sizeof( ((idMD5Anim *)0)->numAnimatedComponents ) },
	{ "idList < idBounds , TAG_MD5_ANIM >", "bounds", (intptr_t)(&((idMD5Anim *)0)->bounds), sizeof( ((idMD5Anim *)0)->bounds ) },
	{ "idList < jointAnimInfo_t , TAG_MD5_ANIM >", "jointInfo", (intptr_t)(&((idMD5Anim *)0)->jointInfo), sizeof( ((idMD5Anim *)0)->jointInfo ) },
	{ "idList < idJointQuat , TAG_MD5_ANIM >", "baseFrame", (intptr_t)(&((idMD5Anim *)0)->baseFrame), sizeof( ((idMD5Anim *)0)->baseFrame ) },
	{ "idList < float , TAG_MD5_ANIM >", "componentFrames", (intptr_t)(&((idMD5Anim *)0)->componentFrames), sizeof( ((idMD5Anim *)0)->componentFrames ) },
	{ "idStr", "name", (intptr_t)(&((idMD5Anim *)0)->name), sizeof( ((idMD5Anim *)0)->name ) },
	{ "idVec3", "totaldelta", (intptr_t)(&((idMD5Anim *)0)->totaldelta), sizeof( ((idMD5Anim *)0)->totaldelta ) },
	{ "mutable int", "ref_count", (intptr_t)(&((idMD5Anim *)0)->ref_count), sizeof( ((idMD5Anim *)0)->ref_count ) },
	{ "idImportOptions", "importOptions", (intptr_t)(&((idMD5Anim *)0)->importOptions), sizeof( ((idMD5Anim *)0)->importOptions ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAnim_typeInfo[] = {
	{ ": const idDeclModelDef *", "modelDef", (intptr_t)(&((idAnim *)0)->modelDef), sizeof( ((idAnim *)0)->modelDef ) },
	{ "const idMD5Anim *[3]", "anims", (intptr_t)(&((idAnim *)0)->anims), sizeof( ((idAnim *)0)->anims ) },
	{ "int", "numAnims", (intptr_t)(&((idAnim *)0)->numAnims), sizeof( ((idAnim *)0)->numAnims ) },
	{ "idStr", "name", (intptr_t)(&((idAnim *)0)->name), sizeof( ((idAnim *)0)->name ) },
	{ "idStr", "realname", (intptr_t)(&((idAnim *)0)->realname), sizeof( ((idAnim *)0)->realname ) },
	{ "idList < frameLookup_t , TAG_ANIM >", "frameLookup", (intptr_t)(&((idAnim *)0)->frameLookup), sizeof( ((idAnim *)0)->frameLookup ) },
	{ "idList < frameCommand_t , TAG_ANIM >", "frameCommands", (intptr_t)(&((idAnim *)0)->frameCommands), sizeof( ((idAnim *)0)->frameCommands ) },
	{ "animFlags_t", "flags", (intptr_t)(&((idAnim *)0)->flags), sizeof( ((idAnim *)0)->flags ) },
	{ NULL, 0 }
};

static classVariableInfo_t idDeclModelDef_typeInfo[] = {
	{ ": idVec3", "offset", (intptr_t)(&((idDeclModelDef *)0)->offset), sizeof( ((idDeclModelDef *)0)->offset ) },
	{ "idList < jointInfo_t , TAG_ANIM >", "joints", (intptr_t)(&((idDeclModelDef *)0)->joints), sizeof( ((idDeclModelDef *)0)->joints ) },
	{ "idList < int , TAG_ANIM >", "jointParents", (intptr_t)(&((idDeclModelDef *)0)->jointParents), sizeof( ((idDeclModelDef *)0)->jointParents ) },
	{ "idList < int , TAG_ANIM >[5]", "channelJoints", (intptr_t)(&((idDeclModelDef *)0)->channelJoints), sizeof( ((idDeclModelDef *)0)->channelJoints ) },
	{ "idRenderModel *", "modelHandle", (intptr_t)(&((idDeclModelDef *)0)->modelHandle), sizeof( ((idDeclModelDef *)0)->modelHandle ) },
	{ "idList < idAnim * , TAG_ANIM >", "anims", (intptr_t)(&((idDeclModelDef *)0)->anims), sizeof( ((idDeclModelDef *)0)->anims ) },
	{ "const idDeclSkin *", "skin", (intptr_t)(&((idDeclModelDef *)0)->skin), sizeof( ((idDeclModelDef *)0)->skin ) },
	{ "bool", "hasCustomRotationSet", (intptr_t)(&((idDeclModelDef *)0)->hasCustomRotationSet), sizeof( ((idDeclModelDef *)0)->hasCustomRotationSet ) },
	{ "idAngles", "originalRotation", (intptr_t)(&((idDeclModelDef *)0)->originalRotation), sizeof( ((idDeclModelDef *)0)->originalRotation ) },
	{ "idImportOptions", "importOptions", (intptr_t)(&((idDeclModelDef *)0)->importOptions), sizeof( ((idDeclModelDef *)0)->importOptions ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAnimBlend_typeInfo[] = {
	{ ": const idDeclModelDef *", "modelDef", (intptr_t)(&((idAnimBlend *)0)->modelDef), sizeof( ((idAnimBlend *)0)->modelDef ) },
	{ "int", "starttime", (intptr_t)(&((idAnimBlend *)0)->starttime), sizeof( ((idAnimBlend *)0)->starttime ) },
	{ "int", "endtime", (intptr_t)(&((idAnimBlend *)0)->endtime), sizeof( ((idAnimBlend *)0)->endtime ) },
	{ "int", "timeOffset", (intptr_t)(&((idAnimBlend *)0)->timeOffset), sizeof( ((idAnimBlend *)0)->timeOffset ) },
	{ "float", "rate", (intptr_t)(&((idAnimBlend *)0)->rate), sizeof( ((idAnimBlend *)0)->rate ) },
	{ "int", "blendStartTime", (intptr_t)(&((idAnimBlend *)0)->blendStartTime), sizeof( ((idAnimBlend *)0)->blendStartTime ) },
	{ "int", "blendDuration", (intptr_t)(&((idAnimBlend *)0)->blendDuration), sizeof( ((idAnimBlend *)0)->blendDuration ) },
	{ "float", "blendStartValue", (intptr_t)(&((idAnimBlend *)0)->blendStartValue), sizeof( ((idAnimBlend *)0)->blendStartValue ) },
	{ "float", "blendEndValue", (intptr_t)(&((idAnimBlend *)0)->blendEndValue), sizeof( ((idAnimBlend *)0)->blendEndValue ) },
	{ "float[3]", "animWeights", (intptr_t)(&((idAnimBlend *)0)->animWeights), sizeof( ((idAnimBlend *)0)->animWeights ) },
	{ "short", "cycle", (intptr_t)(&((idAnimBlend *)0)->cycle), sizeof( ((idAnimBlend *)0)->cycle ) },
	{ "short", "frame", (intptr_t)(&((idAnimBlend *)0)->frame), sizeof( ((idAnimBlend *)0)->frame ) },
	{ "short", "animNum", (intptr_t)(&((idAnimBlend *)0)->animNum), sizeof( ((idAnimBlend *)0)->animNum ) },
	{ "bool", "allowMove", (intptr_t)(&((idAnimBlend *)0)->allowMove), sizeof( ((idAnimBlend *)0)->allowMove ) },
	{ "bool", "allowFrameCommands", (intptr_t)(&((idAnimBlend *)0)->allowFrameCommands), sizeof( ((idAnimBlend *)0)->allowFrameCommands ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFPoseJointMod_typeInfo[] = {
	{ "AFJointModType_t", "mod", (intptr_t)(&((idAFPoseJointMod *)0)->mod), sizeof( ((idAFPoseJointMod *)0)->mod ) },
	{ "idMat3", "axis", (intptr_t)(&((idAFPoseJointMod *)0)->axis), sizeof( ((idAFPoseJointMod *)0)->axis ) },
	{ "idVec3", "origin", (intptr_t)(&((idAFPoseJointMod *)0)->origin), sizeof( ((idAFPoseJointMod *)0)->origin ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAnimator_typeInfo[] = {
	{ ": const idDeclModelDef *", "modelDef", (intptr_t)(&((idAnimator *)0)->modelDef), sizeof( ((idAnimator *)0)->modelDef ) },
	{ "idEntity *", "entity", (intptr_t)(&((idAnimator *)0)->entity), sizeof( ((idAnimator *)0)->entity ) },
	{ "idAnimBlend[15]", "channels", (intptr_t)(&((idAnimator *)0)->channels), sizeof( ((idAnimator *)0)->channels ) },
	{ "idList < jointMod_t * , TAG_ANIM >", "jointMods", (intptr_t)(&((idAnimator *)0)->jointMods), sizeof( ((idAnimator *)0)->jointMods ) },
	{ "int", "numJoints", (intptr_t)(&((idAnimator *)0)->numJoints), sizeof( ((idAnimator *)0)->numJoints ) },
	{ "idJointMat *", "joints", (intptr_t)(&((idAnimator *)0)->joints), sizeof( ((idAnimator *)0)->joints ) },
	{ "mutable int", "lastTransformTime", (intptr_t)(&((idAnimator *)0)->lastTransformTime), sizeof( ((idAnimator *)0)->lastTransformTime ) },
	{ "mutable bool", "stoppedAnimatingUpdate", (intptr_t)(&((idAnimator *)0)->stoppedAnimatingUpdate), sizeof( ((idAnimator *)0)->stoppedAnimatingUpdate ) },
	{ "bool", "removeOriginOffset", (intptr_t)(&((idAnimator *)0)->removeOriginOffset), sizeof( ((idAnimator *)0)->removeOriginOffset ) },
	{ "bool", "forceUpdate", (intptr_t)(&((idAnimator *)0)->forceUpdate), sizeof( ((idAnimator *)0)->forceUpdate ) },
	{ "idBounds", "frameBounds", (intptr_t)(&((idAnimator *)0)->frameBounds), sizeof( ((idAnimator *)0)->frameBounds ) },
	{ "float", "AFPoseBlendWeight", (intptr_t)(&((idAnimator *)0)->AFPoseBlendWeight), sizeof( ((idAnimator *)0)->AFPoseBlendWeight ) },
	{ "idList < int , TAG_ANIM >", "AFPoseJoints", (intptr_t)(&((idAnimator *)0)->AFPoseJoints), sizeof( ((idAnimator *)0)->AFPoseJoints ) },
	{ "idList < idAFPoseJointMod , TAG_ANIM >", "AFPoseJointMods", (intptr_t)(&((idAnimator *)0)->AFPoseJointMods), sizeof( ((idAnimator *)0)->AFPoseJointMods ) },
	{ "idList < idJointQuat , TAG_ANIM >", "AFPoseJointFrame", (intptr_t)(&((idAnimator *)0)->AFPoseJointFrame), sizeof( ((idAnimator *)0)->AFPoseJointFrame ) },
	{ "idBounds", "AFPoseBounds", (intptr_t)(&((idAnimator *)0)->AFPoseBounds), sizeof( ((idAnimator *)0)->AFPoseBounds ) },
	{ "int", "AFPoseTime", (intptr_t)(&((idAnimator *)0)->AFPoseTime), sizeof( ((idAnimator *)0)->AFPoseTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAnimManager_typeInfo[] = {
	{ ": idHashTable < idMD5Anim * >", "animations", (intptr_t)(&((idAnimManager *)0)->animations), sizeof( ((idAnimManager *)0)->animations ) },
	{ "idStrList", "jointnames", (intptr_t)(&((idAnimManager *)0)->jointnames), sizeof( ((idAnimManager *)0)->jointnames ) },
	{ "idHashIndex", "jointnamesHash", (intptr_t)(&((idAnimManager *)0)->jointnamesHash), sizeof( ((idAnimManager *)0)->jointnamesHash ) },
	{ NULL, 0 }
};

static classVariableInfo_t aasPath_t_typeInfo[] = {
	{ "int", "type", (intptr_t)(&((aasPath_t *)0)->type), sizeof( ((aasPath_t *)0)->type ) },
	{ "idVec3", "moveGoal", (intptr_t)(&((aasPath_t *)0)->moveGoal), sizeof( ((aasPath_t *)0)->moveGoal ) },
	{ "int", "moveAreaNum", (intptr_t)(&((aasPath_t *)0)->moveAreaNum), sizeof( ((aasPath_t *)0)->moveAreaNum ) },
	{ "idVec3", "secondaryGoal", (intptr_t)(&((aasPath_t *)0)->secondaryGoal), sizeof( ((aasPath_t *)0)->secondaryGoal ) },
	{ "const idReachability *", "reachability", (intptr_t)(&((aasPath_t *)0)->reachability), sizeof( ((aasPath_t *)0)->reachability ) },
	{ NULL, 0 }
};

static classVariableInfo_t aasGoal_t_typeInfo[] = {
	{ "int", "areaNum", (intptr_t)(&((aasGoal_t *)0)->areaNum), sizeof( ((aasGoal_t *)0)->areaNum ) },
	{ "idVec3", "origin", (intptr_t)(&((aasGoal_t *)0)->origin), sizeof( ((aasGoal_t *)0)->origin ) },
	{ NULL, 0 }
};

static classVariableInfo_t aasObstacle_t_typeInfo[] = {
	{ "idBounds", "absBounds", (intptr_t)(&((aasObstacle_t *)0)->absBounds), sizeof( ((aasObstacle_t *)0)->absBounds ) },
	{ "idBounds", "expAbsBounds", (intptr_t)(&((aasObstacle_t *)0)->expAbsBounds), sizeof( ((aasObstacle_t *)0)->expAbsBounds ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAASCallback_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idAAS_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idClipModel_typeInfo[] = {
	{ ": bool", "enabled", (intptr_t)(&((idClipModel *)0)->enabled), sizeof( ((idClipModel *)0)->enabled ) },
	{ "idEntity *", "entity", (intptr_t)(&((idClipModel *)0)->entity), sizeof( ((idClipModel *)0)->entity ) },
	{ "int", "id", (intptr_t)(&((idClipModel *)0)->id), sizeof( ((idClipModel *)0)->id ) },
	{ "idEntity *", "owner", (intptr_t)(&((idClipModel *)0)->owner), sizeof( ((idClipModel *)0)->owner ) },
	{ "idVec3", "origin", (intptr_t)(&((idClipModel *)0)->origin), sizeof( ((idClipModel *)0)->origin ) },
	{ "idMat3", "axis", (intptr_t)(&((idClipModel *)0)->axis), sizeof( ((idClipModel *)0)->axis ) },
	{ "idBounds", "bounds", (intptr_t)(&((idClipModel *)0)->bounds), sizeof( ((idClipModel *)0)->bounds ) },
	{ "idBounds", "absBounds", (intptr_t)(&((idClipModel *)0)->absBounds), sizeof( ((idClipModel *)0)->absBounds ) },
	{ "const idMaterial *", "material", (intptr_t)(&((idClipModel *)0)->material), sizeof( ((idClipModel *)0)->material ) },
	{ "int", "contents", (intptr_t)(&((idClipModel *)0)->contents), sizeof( ((idClipModel *)0)->contents ) },
	{ "cmHandle_t", "collisionModelHandle", (intptr_t)(&((idClipModel *)0)->collisionModelHandle), sizeof( ((idClipModel *)0)->collisionModelHandle ) },
	{ "int", "traceModelIndex", (intptr_t)(&((idClipModel *)0)->traceModelIndex), sizeof( ((idClipModel *)0)->traceModelIndex ) },
	{ "int", "renderModelHandle", (intptr_t)(&((idClipModel *)0)->renderModelHandle), sizeof( ((idClipModel *)0)->renderModelHandle ) },
	{ "clipLink_s *", "clipLinks", (intptr_t)(&((idClipModel *)0)->clipLinks), sizeof( ((idClipModel *)0)->clipLinks ) },
	{ "int", "touchCount", (intptr_t)(&((idClipModel *)0)->touchCount), sizeof( ((idClipModel *)0)->touchCount ) },
	{ NULL, 0 }
};

static classVariableInfo_t idClip_typeInfo[] = {
	{ ": int", "numClipSectors", (intptr_t)(&((idClip *)0)->numClipSectors), sizeof( ((idClip *)0)->numClipSectors ) },
	{ "clipSector_s *", "clipSectors", (intptr_t)(&((idClip *)0)->clipSectors), sizeof( ((idClip *)0)->clipSectors ) },
	{ "idBounds", "worldBounds", (intptr_t)(&((idClip *)0)->worldBounds), sizeof( ((idClip *)0)->worldBounds ) },
	{ "idClipModel", "temporaryClipModel", (intptr_t)(&((idClip *)0)->temporaryClipModel), sizeof( ((idClip *)0)->temporaryClipModel ) },
	{ "idClipModel", "defaultClipModel", (intptr_t)(&((idClip *)0)->defaultClipModel), sizeof( ((idClip *)0)->defaultClipModel ) },
	{ "cmHandle_t", "worldCollisionModel", (intptr_t)(&((idClip *)0)->worldCollisionModel), sizeof( ((idClip *)0)->worldCollisionModel ) },
	{ "mutable int", "touchCount", (intptr_t)(&((idClip *)0)->touchCount), sizeof( ((idClip *)0)->touchCount ) },
	{ "int", "numTranslations", (intptr_t)(&((idClip *)0)->numTranslations), sizeof( ((idClip *)0)->numTranslations ) },
	{ "int", "numRotations", (intptr_t)(&((idClip *)0)->numRotations), sizeof( ((idClip *)0)->numRotations ) },
	{ "int", "numMotions", (intptr_t)(&((idClip *)0)->numMotions), sizeof( ((idClip *)0)->numMotions ) },
	{ "int", "numRenderModelTraces", (intptr_t)(&((idClip *)0)->numRenderModelTraces), sizeof( ((idClip *)0)->numRenderModelTraces ) },
	{ "int", "numContents", (intptr_t)(&((idClip *)0)->numContents), sizeof( ((idClip *)0)->numContents ) },
	{ "int", "numContacts", (intptr_t)(&((idClip *)0)->numContacts), sizeof( ((idClip *)0)->numContacts ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPush_pushed_s_typeInfo[] = {
	{ "idEntity *", "ent", (intptr_t)(&((idPush::pushed_s *)0)->ent), sizeof( ((idPush::pushed_s *)0)->ent ) },
	{ "idAngles", "deltaViewAngles", (intptr_t)(&((idPush::pushed_s *)0)->deltaViewAngles), sizeof( ((idPush::pushed_s *)0)->deltaViewAngles ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPush_pushedGroup_s_typeInfo[] = {
	{ "idEntity *", "ent", (intptr_t)(&((idPush::pushedGroup_s *)0)->ent), sizeof( ((idPush::pushedGroup_s *)0)->ent ) },
	{ "float", "fraction", (intptr_t)(&((idPush::pushedGroup_s *)0)->fraction), sizeof( ((idPush::pushedGroup_s *)0)->fraction ) },
	{ "bool", "groundContact", (intptr_t)(&((idPush::pushedGroup_s *)0)->groundContact), sizeof( ((idPush::pushedGroup_s *)0)->groundContact ) },
	{ "bool", "test", (intptr_t)(&((idPush::pushedGroup_s *)0)->test), sizeof( ((idPush::pushedGroup_s *)0)->test ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPush_typeInfo[] = {
	{ "idPush::pushed_s[4096]", "pushed", (intptr_t)(&((idPush *)0)->pushed), sizeof( ((idPush *)0)->pushed ) },
	{ "int", "numPushed", (intptr_t)(&((idPush *)0)->numPushed), sizeof( ((idPush *)0)->numPushed ) },
	{ "idPush::pushedGroup_s[4096]", "pushedGroup", (intptr_t)(&((idPush *)0)->pushedGroup), sizeof( ((idPush *)0)->pushedGroup ) },
	{ "int", "pushedGroupSize", (intptr_t)(&((idPush *)0)->pushedGroupSize), sizeof( ((idPush *)0)->pushedGroupSize ) },
	{ NULL, 0 }
};

static classVariableInfo_t pvsHandle_t_typeInfo[] = {
	{ "int", "i", (intptr_t)(&((pvsHandle_t *)0)->i), sizeof( ((pvsHandle_t *)0)->i ) },
	{ "unsigned int", "h", (intptr_t)(&((pvsHandle_t *)0)->h), sizeof( ((pvsHandle_t *)0)->h ) },
	{ NULL, 0 }
};

static classVariableInfo_t pvsCurrent_t_typeInfo[] = {
	{ "pvsHandle_t", "handle", (intptr_t)(&((pvsCurrent_t *)0)->handle), sizeof( ((pvsCurrent_t *)0)->handle ) },
	{ "byte *", "pvs", (intptr_t)(&((pvsCurrent_t *)0)->pvs), sizeof( ((pvsCurrent_t *)0)->pvs ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPVS_typeInfo[] = {
	{ ": int", "numAreas", (intptr_t)(&((idPVS *)0)->numAreas), sizeof( ((idPVS *)0)->numAreas ) },
	{ "int", "numPortals", (intptr_t)(&((idPVS *)0)->numPortals), sizeof( ((idPVS *)0)->numPortals ) },
	{ "bool *", "connectedAreas", (intptr_t)(&((idPVS *)0)->connectedAreas), sizeof( ((idPVS *)0)->connectedAreas ) },
	{ "int *", "areaQueue", (intptr_t)(&((idPVS *)0)->areaQueue), sizeof( ((idPVS *)0)->areaQueue ) },
	{ "byte *", "areaPVS", (intptr_t)(&((idPVS *)0)->areaPVS), sizeof( ((idPVS *)0)->areaPVS ) },
	{ "mutable pvsCurrent_t[64]", "currentPVS", (intptr_t)(&((idPVS *)0)->currentPVS), sizeof( ((idPVS *)0)->currentPVS ) },
	{ "int", "portalVisBytes", (intptr_t)(&((idPVS *)0)->portalVisBytes), sizeof( ((idPVS *)0)->portalVisBytes ) },
	{ "int", "portalVisLongs", (intptr_t)(&((idPVS *)0)->portalVisLongs), sizeof( ((idPVS *)0)->portalVisLongs ) },
	{ "int", "areaVisBytes", (intptr_t)(&((idPVS *)0)->areaVisBytes), sizeof( ((idPVS *)0)->areaVisBytes ) },
	{ "int", "areaVisLongs", (intptr_t)(&((idPVS *)0)->areaVisLongs), sizeof( ((idPVS *)0)->areaVisLongs ) },
	{ "pvsPortal_s *", "pvsPortals", (intptr_t)(&((idPVS *)0)->pvsPortals), sizeof( ((idPVS *)0)->pvsPortals ) },
	{ "pvsArea_s *", "pvsAreas", (intptr_t)(&((idPVS *)0)->pvsAreas), sizeof( ((idPVS *)0)->pvsAreas ) },
	{ NULL, 0 }
};

static classVariableInfo_t leaderboardStats_t_typeInfo[] = {
	{ "int", "frags", (intptr_t)(&((leaderboardStats_t *)0)->frags), sizeof( ((leaderboardStats_t *)0)->frags ) },
	{ "int", "wins", (intptr_t)(&((leaderboardStats_t *)0)->wins), sizeof( ((leaderboardStats_t *)0)->wins ) },
	{ "int", "teamfrags", (intptr_t)(&((leaderboardStats_t *)0)->teamfrags), sizeof( ((leaderboardStats_t *)0)->teamfrags ) },
	{ "int", "deaths", (intptr_t)(&((leaderboardStats_t *)0)->deaths), sizeof( ((leaderboardStats_t *)0)->deaths ) },
	{ NULL, 0 }
};

static classVariableInfo_t columnGameMode_t_typeInfo[] = {
	{ "columnDef_t *", "columnDef", (intptr_t)(&((columnGameMode_t *)0)->columnDef), sizeof( ((columnGameMode_t *)0)->columnDef ) },
	{ "int", "numColumns", (intptr_t)(&((columnGameMode_t *)0)->numColumns), sizeof( ((columnGameMode_t *)0)->numColumns ) },
	{ "rankOrder_t", "rankOrder", (intptr_t)(&((columnGameMode_t *)0)->rankOrder), sizeof( ((columnGameMode_t *)0)->rankOrder ) },
	{ "bool", "supportsAttachments", (intptr_t)(&((columnGameMode_t *)0)->supportsAttachments), sizeof( ((columnGameMode_t *)0)->supportsAttachments ) },
	{ "bool", "checkAgainstCurrent", (intptr_t)(&((columnGameMode_t *)0)->checkAgainstCurrent), sizeof( ((columnGameMode_t *)0)->checkAgainstCurrent ) },
	{ "const char *", "abrevName", (intptr_t)(&((columnGameMode_t *)0)->abrevName), sizeof( ((columnGameMode_t *)0)->abrevName ) },
	{ NULL, 0 }
};

static classVariableInfo_t mpPlayerState_t_typeInfo[] = {
	{ "int", "ping", (intptr_t)(&((mpPlayerState_t *)0)->ping), sizeof( ((mpPlayerState_t *)0)->ping ) },
	{ "int", "fragCount", (intptr_t)(&((mpPlayerState_t *)0)->fragCount), sizeof( ((mpPlayerState_t *)0)->fragCount ) },
	{ "int", "teamFragCount", (intptr_t)(&((mpPlayerState_t *)0)->teamFragCount), sizeof( ((mpPlayerState_t *)0)->teamFragCount ) },
	{ "int", "wins", (intptr_t)(&((mpPlayerState_t *)0)->wins), sizeof( ((mpPlayerState_t *)0)->wins ) },
	{ "bool", "scoreBoardUp", (intptr_t)(&((mpPlayerState_t *)0)->scoreBoardUp), sizeof( ((mpPlayerState_t *)0)->scoreBoardUp ) },
	{ "int", "deaths", (intptr_t)(&((mpPlayerState_t *)0)->deaths), sizeof( ((mpPlayerState_t *)0)->deaths ) },
	{ "int", "clientnum", (intptr_t)(&((mpPlayerState_t *)0)->clientnum), sizeof( ((mpPlayerState_t *)0)->clientnum ) },
	{ "mpLeaderStatus_t", "currentLeader", (intptr_t)(&((mpPlayerState_t *)0)->currentLeader), sizeof( ((mpPlayerState_t *)0)->currentLeader ) },
	{ "bool", "tiednotified", (intptr_t)(&((mpPlayerState_t *)0)->tiednotified), sizeof( ((mpPlayerState_t *)0)->tiednotified ) },
	{ NULL, 0 }
};

static classVariableInfo_t mpChatLine_t_typeInfo[] = {
	{ "idStr", "line", (intptr_t)(&((mpChatLine_t *)0)->line), sizeof( ((mpChatLine_t *)0)->line ) },
	{ "short", "fade", (intptr_t)(&((mpChatLine_t *)0)->fade), sizeof( ((mpChatLine_t *)0)->fade ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMultiplayerGame_typeInfo[] = {
	{ "int", "player_red_flag", (intptr_t)(&((idMultiplayerGame *)0)->player_red_flag), sizeof( ((idMultiplayerGame *)0)->player_red_flag ) },
	{ "int", "player_blue_flag", (intptr_t)(&((idMultiplayerGame *)0)->player_blue_flag), sizeof( ((idMultiplayerGame *)0)->player_blue_flag ) },
	{ "int", "killsRemainingMessageState", (intptr_t)(&((idMultiplayerGame *)0)->killsRemainingMessageState), sizeof( ((idMultiplayerGame *)0)->killsRemainingMessageState ) },
	{ "gameState_t", "gameState", (intptr_t)(&((idMultiplayerGame *)0)->gameState), sizeof( ((idMultiplayerGame *)0)->gameState ) },
	{ "gameState_t", "nextState", (intptr_t)(&((idMultiplayerGame *)0)->nextState), sizeof( ((idMultiplayerGame *)0)->nextState ) },
	{ "mpPlayerState_t[1]", "playerState", (intptr_t)(&((idMultiplayerGame *)0)->playerState), sizeof( ((idMultiplayerGame *)0)->playerState ) },
	{ "int", "nextStateSwitch", (intptr_t)(&((idMultiplayerGame *)0)->nextStateSwitch), sizeof( ((idMultiplayerGame *)0)->nextStateSwitch ) },
	{ "int", "warmupEndTime", (intptr_t)(&((idMultiplayerGame *)0)->warmupEndTime), sizeof( ((idMultiplayerGame *)0)->warmupEndTime ) },
	{ "int", "matchStartedTime", (intptr_t)(&((idMultiplayerGame *)0)->matchStartedTime), sizeof( ((idMultiplayerGame *)0)->matchStartedTime ) },
	{ "int[2]", "currentTourneyPlayer", (intptr_t)(&((idMultiplayerGame *)0)->currentTourneyPlayer), sizeof( ((idMultiplayerGame *)0)->currentTourneyPlayer ) },
	{ "int", "lastWinner", (intptr_t)(&((idMultiplayerGame *)0)->lastWinner), sizeof( ((idMultiplayerGame *)0)->lastWinner ) },
	{ "bool", "one", (intptr_t)(&((idMultiplayerGame *)0)->one), sizeof( ((idMultiplayerGame *)0)->one ) },
	{ "bool", "two", (intptr_t)(&((idMultiplayerGame *)0)->two), sizeof( ((idMultiplayerGame *)0)->two ) },
	{ "bool", "three", (intptr_t)(&((idMultiplayerGame *)0)->three), sizeof( ((idMultiplayerGame *)0)->three ) },
	{ "idMenuHandler_Scoreboard *", "scoreboardManager", (intptr_t)(&((idMultiplayerGame *)0)->scoreboardManager), sizeof( ((idMultiplayerGame *)0)->scoreboardManager ) },
	{ "mpChatLine_t[5]", "chatHistory", (intptr_t)(&((idMultiplayerGame *)0)->chatHistory), sizeof( ((idMultiplayerGame *)0)->chatHistory ) },
	{ "int", "chatHistoryIndex", (intptr_t)(&((idMultiplayerGame *)0)->chatHistoryIndex), sizeof( ((idMultiplayerGame *)0)->chatHistoryIndex ) },
	{ "int", "chatHistorySize", (intptr_t)(&((idMultiplayerGame *)0)->chatHistorySize), sizeof( ((idMultiplayerGame *)0)->chatHistorySize ) },
	{ "bool", "chatDataUpdated", (intptr_t)(&((idMultiplayerGame *)0)->chatDataUpdated), sizeof( ((idMultiplayerGame *)0)->chatDataUpdated ) },
	{ "int", "lastChatLineTime", (intptr_t)(&((idMultiplayerGame *)0)->lastChatLineTime), sizeof( ((idMultiplayerGame *)0)->lastChatLineTime ) },
	{ "int", "numRankedPlayers", (intptr_t)(&((idMultiplayerGame *)0)->numRankedPlayers), sizeof( ((idMultiplayerGame *)0)->numRankedPlayers ) },
	{ "idPlayer *[1]", "rankedPlayers", (intptr_t)(&((idMultiplayerGame *)0)->rankedPlayers), sizeof( ((idMultiplayerGame *)0)->rankedPlayers ) },
	{ "bool", "pureReady", (intptr_t)(&((idMultiplayerGame *)0)->pureReady), sizeof( ((idMultiplayerGame *)0)->pureReady ) },
	{ "int", "fragLimitTimeout", (intptr_t)(&((idMultiplayerGame *)0)->fragLimitTimeout), sizeof( ((idMultiplayerGame *)0)->fragLimitTimeout ) },
	{ "int", "voiceChatThrottle", (intptr_t)(&((idMultiplayerGame *)0)->voiceChatThrottle), sizeof( ((idMultiplayerGame *)0)->voiceChatThrottle ) },
	{ "int", "startFragLimit", (intptr_t)(&((idMultiplayerGame *)0)->startFragLimit), sizeof( ((idMultiplayerGame *)0)->startFragLimit ) },
	{ "idItemTeam *[2]", "teamFlags", (intptr_t)(&((idMultiplayerGame *)0)->teamFlags), sizeof( ((idMultiplayerGame *)0)->teamFlags ) },
	{ "int[2]", "teamPoints", (intptr_t)(&((idMultiplayerGame *)0)->teamPoints), sizeof( ((idMultiplayerGame *)0)->teamPoints ) },
	{ "bool", "flagMsgOn", (intptr_t)(&((idMultiplayerGame *)0)->flagMsgOn), sizeof( ((idMultiplayerGame *)0)->flagMsgOn ) },
	{ NULL, 0 }
};

static classVariableInfo_t entityNetEvent_t_typeInfo[] = {
	{ "int", "spawnId", (intptr_t)(&((entityNetEvent_t *)0)->spawnId), sizeof( ((entityNetEvent_t *)0)->spawnId ) },
	{ "int", "event", (intptr_t)(&((entityNetEvent_t *)0)->event), sizeof( ((entityNetEvent_t *)0)->event ) },
	{ "int", "time", (intptr_t)(&((entityNetEvent_t *)0)->time), sizeof( ((entityNetEvent_t *)0)->time ) },
	{ "int", "paramsSize", (intptr_t)(&((entityNetEvent_t *)0)->paramsSize), sizeof( ((entityNetEvent_t *)0)->paramsSize ) },
	{ "byte[128]", "paramsBuf", (intptr_t)(&((entityNetEvent_t *)0)->paramsBuf), sizeof( ((entityNetEvent_t *)0)->paramsBuf ) },
	{ "entityNetEvent_s *", "next", (intptr_t)(&((entityNetEvent_t *)0)->next), sizeof( ((entityNetEvent_t *)0)->next ) },
	{ "entityNetEvent_s *", "prev", (intptr_t)(&((entityNetEvent_t *)0)->prev), sizeof( ((entityNetEvent_t *)0)->prev ) },
	{ NULL, 0 }
};

static classVariableInfo_t spawnSpot_t_typeInfo[] = {
	{ "idEntity *", "ent", (intptr_t)(&((spawnSpot_t *)0)->ent), sizeof( ((spawnSpot_t *)0)->ent ) },
	{ "int", "dist", (intptr_t)(&((spawnSpot_t *)0)->dist), sizeof( ((spawnSpot_t *)0)->dist ) },
	{ "int", "team", (intptr_t)(&((spawnSpot_t *)0)->team), sizeof( ((spawnSpot_t *)0)->team ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEventQueue_typeInfo[] = {
	{ ": entityNetEvent_t *", "start", (intptr_t)(&((idEventQueue *)0)->start), sizeof( ((idEventQueue *)0)->start ) },
	{ "entityNetEvent_t *", "end", (intptr_t)(&((idEventQueue *)0)->end), sizeof( ((idEventQueue *)0)->end ) },
	{ "idBlockAlloc < entityNetEvent_t , 32 >", "eventAllocator", (intptr_t)(&((idEventQueue *)0)->eventAllocator), sizeof( ((idEventQueue *)0)->eventAllocator ) },
	{ NULL, 0 }
};

static classVariableInfo_t timeState_t_typeInfo[] = {
	{ "int", "time", (intptr_t)(&((timeState_t *)0)->time), sizeof( ((timeState_t *)0)->time ) },
	{ "int", "previousTime", (intptr_t)(&((timeState_t *)0)->previousTime), sizeof( ((timeState_t *)0)->previousTime ) },
	{ "int", "realClientTime", (intptr_t)(&((timeState_t *)0)->realClientTime), sizeof( ((timeState_t *)0)->realClientTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceGameDelayRemoveEntry_t_typeInfo[] = {
	{ "int32_t", "removeTime", (intptr_t)(&((iceGameDelayRemoveEntry_t *)0)->removeTime), sizeof( ((iceGameDelayRemoveEntry_t *)0)->removeTime ) },
	{ "idEntity *", "entity", (intptr_t)(&((iceGameDelayRemoveEntry_t *)0)->entity), sizeof( ((iceGameDelayRemoveEntry_t *)0)->entity ) },
	{ NULL, 0 }
};

static classVariableInfo_t idGameLocal_netInterpolationInfo_t_typeInfo[] = {
	{ "float", "pct", (intptr_t)(&((idGameLocal::netInterpolationInfo_t *)0)->pct), sizeof( ((idGameLocal::netInterpolationInfo_t *)0)->pct ) },
	{ "int", "serverGameMs", (intptr_t)(&((idGameLocal::netInterpolationInfo_t *)0)->serverGameMs), sizeof( ((idGameLocal::netInterpolationInfo_t *)0)->serverGameMs ) },
	{ "int", "previousServerGameMs", (intptr_t)(&((idGameLocal::netInterpolationInfo_t *)0)->previousServerGameMs), sizeof( ((idGameLocal::netInterpolationInfo_t *)0)->previousServerGameMs ) },
	{ "int", "ssStartTime", (intptr_t)(&((idGameLocal::netInterpolationInfo_t *)0)->ssStartTime), sizeof( ((idGameLocal::netInterpolationInfo_t *)0)->ssStartTime ) },
	{ "int", "ssEndTime", (intptr_t)(&((idGameLocal::netInterpolationInfo_t *)0)->ssEndTime), sizeof( ((idGameLocal::netInterpolationInfo_t *)0)->ssEndTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idGameLocal_typeInfo[] = {
	{ ": int", "previousServerTime", (intptr_t)(&((idGameLocal *)0)->previousServerTime), sizeof( ((idGameLocal *)0)->previousServerTime ) },
	{ "int", "serverTime", (intptr_t)(&((idGameLocal *)0)->serverTime), sizeof( ((idGameLocal *)0)->serverTime ) },
	{ "idDict", "serverInfo", (intptr_t)(&((idGameLocal *)0)->serverInfo), sizeof( ((idGameLocal *)0)->serverInfo ) },
	{ "int", "numClients", (intptr_t)(&((idGameLocal *)0)->numClients), sizeof( ((idGameLocal *)0)->numClients ) },
	{ "idArray < lobbyUserID_t , MAX_CLIENTS >", "lobbyUserIDs", (intptr_t)(&((idGameLocal *)0)->lobbyUserIDs), sizeof( ((idGameLocal *)0)->lobbyUserIDs ) },
	{ "idDict[1]", "persistentPlayerInfo", (intptr_t)(&((idGameLocal *)0)->persistentPlayerInfo), sizeof( ((idGameLocal *)0)->persistentPlayerInfo ) },
	{ "idEntity *[4096]", "entities", (intptr_t)(&((idGameLocal *)0)->entities), sizeof( ((idGameLocal *)0)->entities ) },
	{ "int[4096]", "spawnIds", (intptr_t)(&((idGameLocal *)0)->spawnIds), sizeof( ((idGameLocal *)0)->spawnIds ) },
	{ "idArray < int , 2 >", "firstFreeEntityIndex", (intptr_t)(&((idGameLocal *)0)->firstFreeEntityIndex), sizeof( ((idGameLocal *)0)->firstFreeEntityIndex ) },
	{ "int", "num_entities", (intptr_t)(&((idGameLocal *)0)->num_entities), sizeof( ((idGameLocal *)0)->num_entities ) },
	{ "idHashIndex", "entityHash", (intptr_t)(&((idGameLocal *)0)->entityHash), sizeof( ((idGameLocal *)0)->entityHash ) },
	{ "idWorldspawn *", "world", (intptr_t)(&((idGameLocal *)0)->world), sizeof( ((idGameLocal *)0)->world ) },
	{ "idLinkList < idEntity >", "spawnedEntities", (intptr_t)(&((idGameLocal *)0)->spawnedEntities), sizeof( ((idGameLocal *)0)->spawnedEntities ) },
	{ "idLinkList < idEntity >", "activeEntities", (intptr_t)(&((idGameLocal *)0)->activeEntities), sizeof( ((idGameLocal *)0)->activeEntities ) },
	{ "idLinkList < idEntity >", "aimAssistEntities", (intptr_t)(&((idGameLocal *)0)->aimAssistEntities), sizeof( ((idGameLocal *)0)->aimAssistEntities ) },
	{ "int", "numEntitiesToDeactivate", (intptr_t)(&((idGameLocal *)0)->numEntitiesToDeactivate), sizeof( ((idGameLocal *)0)->numEntitiesToDeactivate ) },
	{ "bool", "sortPushers", (intptr_t)(&((idGameLocal *)0)->sortPushers), sizeof( ((idGameLocal *)0)->sortPushers ) },
	{ "bool", "sortTeamMasters", (intptr_t)(&((idGameLocal *)0)->sortTeamMasters), sizeof( ((idGameLocal *)0)->sortTeamMasters ) },
	{ "idDict", "persistentLevelInfo", (intptr_t)(&((idGameLocal *)0)->persistentLevelInfo), sizeof( ((idGameLocal *)0)->persistentLevelInfo ) },
	{ "float[1]", "globalShaderParms", (intptr_t)(&((idGameLocal *)0)->globalShaderParms), sizeof( ((idGameLocal *)0)->globalShaderParms ) },
	{ "idRandom", "random", (intptr_t)(&((idGameLocal *)0)->random), sizeof( ((idGameLocal *)0)->random ) },
	{ "idProgram", "program", (intptr_t)(&((idGameLocal *)0)->program), sizeof( ((idGameLocal *)0)->program ) },
	{ "idThread *", "frameCommandThread", (intptr_t)(&((idGameLocal *)0)->frameCommandThread), sizeof( ((idGameLocal *)0)->frameCommandThread ) },
	{ "idClip", "clip", (intptr_t)(&((idGameLocal *)0)->clip), sizeof( ((idGameLocal *)0)->clip ) },
	{ "idPush", "push", (intptr_t)(&((idGameLocal *)0)->push), sizeof( ((idGameLocal *)0)->push ) },
	{ "idPVS", "pvs", (intptr_t)(&((idGameLocal *)0)->pvs), sizeof( ((idGameLocal *)0)->pvs ) },
	{ "idTestModel *", "testmodel", (intptr_t)(&((idGameLocal *)0)->testmodel), sizeof( ((idGameLocal *)0)->testmodel ) },
	{ "idEntityFx *", "testFx", (intptr_t)(&((idGameLocal *)0)->testFx), sizeof( ((idGameLocal *)0)->testFx ) },
	{ "idStr", "sessionCommand", (intptr_t)(&((idGameLocal *)0)->sessionCommand), sizeof( ((idGameLocal *)0)->sessionCommand ) },
	{ "idMultiplayerGame", "mpGame", (intptr_t)(&((idGameLocal *)0)->mpGame), sizeof( ((idGameLocal *)0)->mpGame ) },
	{ "idSmokeParticles *", "smokeParticles", (intptr_t)(&((idGameLocal *)0)->smokeParticles), sizeof( ((idGameLocal *)0)->smokeParticles ) },
	{ "idEditEntities *", "editEntities", (intptr_t)(&((idGameLocal *)0)->editEntities), sizeof( ((idGameLocal *)0)->editEntities ) },
	{ "int", "cinematicSkipTime", (intptr_t)(&((idGameLocal *)0)->cinematicSkipTime), sizeof( ((idGameLocal *)0)->cinematicSkipTime ) },
	{ "int", "cinematicStopTime", (intptr_t)(&((idGameLocal *)0)->cinematicStopTime), sizeof( ((idGameLocal *)0)->cinematicStopTime ) },
	{ "int", "cinematicMaxSkipTime", (intptr_t)(&((idGameLocal *)0)->cinematicMaxSkipTime), sizeof( ((idGameLocal *)0)->cinematicMaxSkipTime ) },
	{ "bool", "inCinematic", (intptr_t)(&((idGameLocal *)0)->inCinematic), sizeof( ((idGameLocal *)0)->inCinematic ) },
	{ "bool", "skipCinematic", (intptr_t)(&((idGameLocal *)0)->skipCinematic), sizeof( ((idGameLocal *)0)->skipCinematic ) },
	{ "int", "framenum", (intptr_t)(&((idGameLocal *)0)->framenum), sizeof( ((idGameLocal *)0)->framenum ) },
	{ "int", "time", (intptr_t)(&((idGameLocal *)0)->time), sizeof( ((idGameLocal *)0)->time ) },
	{ "int", "previousTime", (intptr_t)(&((idGameLocal *)0)->previousTime), sizeof( ((idGameLocal *)0)->previousTime ) },
	{ "int", "vacuumAreaNum", (intptr_t)(&((idGameLocal *)0)->vacuumAreaNum), sizeof( ((idGameLocal *)0)->vacuumAreaNum ) },
	{ "gameType_t", "gameType", (intptr_t)(&((idGameLocal *)0)->gameType), sizeof( ((idGameLocal *)0)->gameType ) },
	{ "idLinkList < idEntity >", "snapshotEntities", (intptr_t)(&((idGameLocal *)0)->snapshotEntities), sizeof( ((idGameLocal *)0)->snapshotEntities ) },
	{ "int", "realClientTime", (intptr_t)(&((idGameLocal *)0)->realClientTime), sizeof( ((idGameLocal *)0)->realClientTime ) },
	{ "bool", "isNewFrame", (intptr_t)(&((idGameLocal *)0)->isNewFrame), sizeof( ((idGameLocal *)0)->isNewFrame ) },
	{ "float", "clientSmoothing", (intptr_t)(&((idGameLocal *)0)->clientSmoothing), sizeof( ((idGameLocal *)0)->clientSmoothing ) },
	{ "int", "entityDefBits", (intptr_t)(&((idGameLocal *)0)->entityDefBits), sizeof( ((idGameLocal *)0)->entityDefBits ) },
	{ "idEntityPtr < idEntity >", "lastGUIEnt", (intptr_t)(&((idGameLocal *)0)->lastGUIEnt), sizeof( ((idGameLocal *)0)->lastGUIEnt ) },
	{ "int", "lastGUI", (intptr_t)(&((idGameLocal *)0)->lastGUI), sizeof( ((idGameLocal *)0)->lastGUI ) },
	{ "idEntityPtr < idPlayer >", "playerActivateFragChamber", (intptr_t)(&((idGameLocal *)0)->playerActivateFragChamber), sizeof( ((idGameLocal *)0)->playerActivateFragChamber ) },
	{ "idEntityPtr < idEntity >", "portalSkyEnt", (intptr_t)(&((idGameLocal *)0)->portalSkyEnt), sizeof( ((idGameLocal *)0)->portalSkyEnt ) },
	{ "bool", "portalSkyActive", (intptr_t)(&((idGameLocal *)0)->portalSkyActive), sizeof( ((idGameLocal *)0)->portalSkyActive ) },
	{ "timeState_t", "fast", (intptr_t)(&((idGameLocal *)0)->fast), sizeof( ((idGameLocal *)0)->fast ) },
	{ "timeState_t", "slow", (intptr_t)(&((idGameLocal *)0)->slow), sizeof( ((idGameLocal *)0)->slow ) },
	{ "int", "selectedGroup", (intptr_t)(&((idGameLocal *)0)->selectedGroup), sizeof( ((idGameLocal *)0)->selectedGroup ) },
	{ "slowmoState_t", "slowmoState", (intptr_t)(&((idGameLocal *)0)->slowmoState), sizeof( ((idGameLocal *)0)->slowmoState ) },
	{ "float", "slowmoScale", (intptr_t)(&((idGameLocal *)0)->slowmoScale), sizeof( ((idGameLocal *)0)->slowmoScale ) },
	{ "bool", "quickSlowmoReset", (intptr_t)(&((idGameLocal *)0)->quickSlowmoReset), sizeof( ((idGameLocal *)0)->quickSlowmoReset ) },
	{ "idStr", "mapFileName", (intptr_t)(&((idGameLocal *)0)->mapFileName), sizeof( ((idGameLocal *)0)->mapFileName ) },
	{ "idMapFile *", "mapFile", (intptr_t)(&((idGameLocal *)0)->mapFile), sizeof( ((idGameLocal *)0)->mapFile ) },
	{ "bool", "mapCycleLoaded", (intptr_t)(&((idGameLocal *)0)->mapCycleLoaded), sizeof( ((idGameLocal *)0)->mapCycleLoaded ) },
	{ "int", "spawnCount", (intptr_t)(&((idGameLocal *)0)->spawnCount), sizeof( ((idGameLocal *)0)->spawnCount ) },
	{ "int", "mapSpawnCount", (intptr_t)(&((idGameLocal *)0)->mapSpawnCount), sizeof( ((idGameLocal *)0)->mapSpawnCount ) },
	{ "idLocationEntity * *", "locationEntities", (intptr_t)(&((idGameLocal *)0)->locationEntities), sizeof( ((idGameLocal *)0)->locationEntities ) },
	{ "idCamera *", "camera", (intptr_t)(&((idGameLocal *)0)->camera), sizeof( ((idGameLocal *)0)->camera ) },
	{ "const idMaterial *", "globalMaterial", (intptr_t)(&((idGameLocal *)0)->globalMaterial), sizeof( ((idGameLocal *)0)->globalMaterial ) },
	{ "idList < idAAS * >", "aasList", (intptr_t)(&((idGameLocal *)0)->aasList), sizeof( ((idGameLocal *)0)->aasList ) },
	{ "idMenuHandler_Shell *", "shellHandler", (intptr_t)(&((idGameLocal *)0)->shellHandler), sizeof( ((idGameLocal *)0)->shellHandler ) },
	{ "idStrList", "aasNames", (intptr_t)(&((idGameLocal *)0)->aasNames), sizeof( ((idGameLocal *)0)->aasNames ) },
	{ "idEntityPtr < idActor >", "lastAIAlertEntity", (intptr_t)(&((idGameLocal *)0)->lastAIAlertEntity), sizeof( ((idGameLocal *)0)->lastAIAlertEntity ) },
	{ "int", "lastAIAlertTime", (intptr_t)(&((idGameLocal *)0)->lastAIAlertTime), sizeof( ((idGameLocal *)0)->lastAIAlertTime ) },
	{ "idDict", "spawnArgs", (intptr_t)(&((idGameLocal *)0)->spawnArgs), sizeof( ((idGameLocal *)0)->spawnArgs ) },
	{ "pvsHandle_t", "playerPVS", (intptr_t)(&((idGameLocal *)0)->playerPVS), sizeof( ((idGameLocal *)0)->playerPVS ) },
	{ "pvsHandle_t", "playerConnectedAreas", (intptr_t)(&((idGameLocal *)0)->playerConnectedAreas), sizeof( ((idGameLocal *)0)->playerConnectedAreas ) },
	{ "idVec3", "gravity", (intptr_t)(&((idGameLocal *)0)->gravity), sizeof( ((idGameLocal *)0)->gravity ) },
	{ "gameState_t", "gamestate", (intptr_t)(&((idGameLocal *)0)->gamestate), sizeof( ((idGameLocal *)0)->gamestate ) },
	{ "bool", "influenceActive", (intptr_t)(&((idGameLocal *)0)->influenceActive), sizeof( ((idGameLocal *)0)->influenceActive ) },
	{ "int", "nextGibTime", (intptr_t)(&((idGameLocal *)0)->nextGibTime), sizeof( ((idGameLocal *)0)->nextGibTime ) },
	{ "idEventQueue", "eventQueue", (intptr_t)(&((idGameLocal *)0)->eventQueue), sizeof( ((idGameLocal *)0)->eventQueue ) },
	{ "idEventQueue", "savedEventQueue", (intptr_t)(&((idGameLocal *)0)->savedEventQueue), sizeof( ((idGameLocal *)0)->savedEventQueue ) },
	{ "idStaticList < spawnSpot_t , MAX_GENTITIES >", "spawnSpots", (intptr_t)(&((idGameLocal *)0)->spawnSpots), sizeof( ((idGameLocal *)0)->spawnSpots ) },
	{ "idStaticList < idEntity * , MAX_GENTITIES >", "initialSpots", (intptr_t)(&((idGameLocal *)0)->initialSpots), sizeof( ((idGameLocal *)0)->initialSpots ) },
	{ "int", "currentInitialSpot", (intptr_t)(&((idGameLocal *)0)->currentInitialSpot), sizeof( ((idGameLocal *)0)->currentInitialSpot ) },
	{ "idStaticList < spawnSpot_t , MAX_GENTITIES >[2]", "teamSpawnSpots", (intptr_t)(&((idGameLocal *)0)->teamSpawnSpots), sizeof( ((idGameLocal *)0)->teamSpawnSpots ) },
	{ "idStaticList < idEntity * , MAX_GENTITIES >[2]", "teamInitialSpots", (intptr_t)(&((idGameLocal *)0)->teamInitialSpots), sizeof( ((idGameLocal *)0)->teamInitialSpots ) },
	{ "int[2]", "teamCurrentInitialSpot", (intptr_t)(&((idGameLocal *)0)->teamCurrentInitialSpot), sizeof( ((idGameLocal *)0)->teamCurrentInitialSpot ) },
	{ "netInterpolationInfo_t", "netInterpolationInfo", (intptr_t)(&((idGameLocal *)0)->netInterpolationInfo), sizeof( ((idGameLocal *)0)->netInterpolationInfo ) },
	{ "idDict", "newInfo", (intptr_t)(&((idGameLocal *)0)->newInfo), sizeof( ((idGameLocal *)0)->newInfo ) },
	{ "idArray < int , MAX_PLAYERS >", "usercmdLastClientMilliseconds", (intptr_t)(&((idGameLocal *)0)->usercmdLastClientMilliseconds), sizeof( ((idGameLocal *)0)->usercmdLastClientMilliseconds ) },
	{ "idArray < int , MAX_PLAYERS >", "lastCmdRunTimeOnClient", (intptr_t)(&((idGameLocal *)0)->lastCmdRunTimeOnClient), sizeof( ((idGameLocal *)0)->lastCmdRunTimeOnClient ) },
	{ "idArray < int , MAX_PLAYERS >", "lastCmdRunTimeOnServer", (intptr_t)(&((idGameLocal *)0)->lastCmdRunTimeOnServer), sizeof( ((idGameLocal *)0)->lastCmdRunTimeOnServer ) },
	{ "const idDeclEntityDef *", "botItemTable", (intptr_t)(&((idGameLocal *)0)->botItemTable), sizeof( ((idGameLocal *)0)->botItemTable ) },
	{ "idList < iceBot * >", "registeredBots", (intptr_t)(&((idGameLocal *)0)->registeredBots), sizeof( ((idGameLocal *)0)->registeredBots ) },
	{ "idList < iceGameDelayRemoveEntry_t >", "delayRemoveEntities", (intptr_t)(&((idGameLocal *)0)->delayRemoveEntities), sizeof( ((idGameLocal *)0)->delayRemoveEntities ) },
	{ "idAAS *", "bot_aas", (intptr_t)(&((idGameLocal *)0)->bot_aas), sizeof( ((idGameLocal *)0)->bot_aas ) },
	{ NULL, 0 }
};

static classVariableInfo_t idGameError_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idForce_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idForce_Constant_typeInfo[] = {
	{ ": idVec3", "force", (intptr_t)(&((idForce_Constant *)0)->force), sizeof( ((idForce_Constant *)0)->force ) },
	{ "idPhysics *", "physics", (intptr_t)(&((idForce_Constant *)0)->physics), sizeof( ((idForce_Constant *)0)->physics ) },
	{ "int", "id", (intptr_t)(&((idForce_Constant *)0)->id), sizeof( ((idForce_Constant *)0)->id ) },
	{ "idVec3", "point", (intptr_t)(&((idForce_Constant *)0)->point), sizeof( ((idForce_Constant *)0)->point ) },
	{ NULL, 0 }
};

static classVariableInfo_t idForce_Drag_typeInfo[] = {
	{ ": float", "damping", (intptr_t)(&((idForce_Drag *)0)->damping), sizeof( ((idForce_Drag *)0)->damping ) },
	{ "idPhysics *", "physics", (intptr_t)(&((idForce_Drag *)0)->physics), sizeof( ((idForce_Drag *)0)->physics ) },
	{ "int", "id", (intptr_t)(&((idForce_Drag *)0)->id), sizeof( ((idForce_Drag *)0)->id ) },
	{ "idVec3", "p", (intptr_t)(&((idForce_Drag *)0)->p), sizeof( ((idForce_Drag *)0)->p ) },
	{ "idVec3", "dragPosition", (intptr_t)(&((idForce_Drag *)0)->dragPosition), sizeof( ((idForce_Drag *)0)->dragPosition ) },
	{ NULL, 0 }
};

static classVariableInfo_t idForce_Grab_typeInfo[] = {
	{ ": float", "damping", (intptr_t)(&((idForce_Grab *)0)->damping), sizeof( ((idForce_Grab *)0)->damping ) },
	{ "idVec3", "goalPosition", (intptr_t)(&((idForce_Grab *)0)->goalPosition), sizeof( ((idForce_Grab *)0)->goalPosition ) },
	{ "float", "distanceToGoal", (intptr_t)(&((idForce_Grab *)0)->distanceToGoal), sizeof( ((idForce_Grab *)0)->distanceToGoal ) },
	{ "idPhysics *", "physics", (intptr_t)(&((idForce_Grab *)0)->physics), sizeof( ((idForce_Grab *)0)->physics ) },
	{ "int", "id", (intptr_t)(&((idForce_Grab *)0)->id), sizeof( ((idForce_Grab *)0)->id ) },
	{ NULL, 0 }
};

static classVariableInfo_t idForce_Field_typeInfo[] = {
	{ ": forceFieldType", "type", (intptr_t)(&((idForce_Field *)0)->type), sizeof( ((idForce_Field *)0)->type ) },
	{ "forceFieldApplyType", "applyType", (intptr_t)(&((idForce_Field *)0)->applyType), sizeof( ((idForce_Field *)0)->applyType ) },
	{ "float", "magnitude", (intptr_t)(&((idForce_Field *)0)->magnitude), sizeof( ((idForce_Field *)0)->magnitude ) },
	{ "idVec3", "dir", (intptr_t)(&((idForce_Field *)0)->dir), sizeof( ((idForce_Field *)0)->dir ) },
	{ "float", "randomTorque", (intptr_t)(&((idForce_Field *)0)->randomTorque), sizeof( ((idForce_Field *)0)->randomTorque ) },
	{ "bool", "playerOnly", (intptr_t)(&((idForce_Field *)0)->playerOnly), sizeof( ((idForce_Field *)0)->playerOnly ) },
	{ "bool", "monsterOnly", (intptr_t)(&((idForce_Field *)0)->monsterOnly), sizeof( ((idForce_Field *)0)->monsterOnly ) },
	{ "idClipModel *", "clipModel", (intptr_t)(&((idForce_Field *)0)->clipModel), sizeof( ((idForce_Field *)0)->clipModel ) },
	{ NULL, 0 }
};

static classVariableInfo_t idForce_Spring_typeInfo[] = {
	{ ": float", "Kstretch", (intptr_t)(&((idForce_Spring *)0)->Kstretch), sizeof( ((idForce_Spring *)0)->Kstretch ) },
	{ "float", "Kcompress", (intptr_t)(&((idForce_Spring *)0)->Kcompress), sizeof( ((idForce_Spring *)0)->Kcompress ) },
	{ "float", "damping", (intptr_t)(&((idForce_Spring *)0)->damping), sizeof( ((idForce_Spring *)0)->damping ) },
	{ "float", "restLength", (intptr_t)(&((idForce_Spring *)0)->restLength), sizeof( ((idForce_Spring *)0)->restLength ) },
	{ "idPhysics *", "physics1", (intptr_t)(&((idForce_Spring *)0)->physics1), sizeof( ((idForce_Spring *)0)->physics1 ) },
	{ "int", "id1", (intptr_t)(&((idForce_Spring *)0)->id1), sizeof( ((idForce_Spring *)0)->id1 ) },
	{ "idVec3", "p1", (intptr_t)(&((idForce_Spring *)0)->p1), sizeof( ((idForce_Spring *)0)->p1 ) },
	{ "idPhysics *", "physics2", (intptr_t)(&((idForce_Spring *)0)->physics2), sizeof( ((idForce_Spring *)0)->physics2 ) },
	{ "int", "id2", (intptr_t)(&((idForce_Spring *)0)->id2), sizeof( ((idForce_Spring *)0)->id2 ) },
	{ "idVec3", "p2", (intptr_t)(&((idForce_Spring *)0)->p2), sizeof( ((idForce_Spring *)0)->p2 ) },
	{ NULL, 0 }
};

static classVariableInfo_t impactInfo_t_typeInfo[] = {
	{ "float", "invMass", (intptr_t)(&((impactInfo_t *)0)->invMass), sizeof( ((impactInfo_t *)0)->invMass ) },
	{ "idMat3", "invInertiaTensor", (intptr_t)(&((impactInfo_t *)0)->invInertiaTensor), sizeof( ((impactInfo_t *)0)->invInertiaTensor ) },
	{ "idVec3", "position", (intptr_t)(&((impactInfo_t *)0)->position), sizeof( ((impactInfo_t *)0)->position ) },
	{ "idVec3", "velocity", (intptr_t)(&((impactInfo_t *)0)->velocity), sizeof( ((impactInfo_t *)0)->velocity ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t staticPState_t_typeInfo[] = {
	{ "idVec3", "origin", (intptr_t)(&((staticPState_t *)0)->origin), sizeof( ((staticPState_t *)0)->origin ) },
	{ "idMat3", "axis", (intptr_t)(&((staticPState_t *)0)->axis), sizeof( ((staticPState_t *)0)->axis ) },
	{ "idVec3", "localOrigin", (intptr_t)(&((staticPState_t *)0)->localOrigin), sizeof( ((staticPState_t *)0)->localOrigin ) },
	{ "idMat3", "localAxis", (intptr_t)(&((staticPState_t *)0)->localAxis), sizeof( ((staticPState_t *)0)->localAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t staticInterpolatePState_t_typeInfo[] = {
	{ "idVec3", "origin", (intptr_t)(&((staticInterpolatePState_t *)0)->origin), sizeof( ((staticInterpolatePState_t *)0)->origin ) },
	{ "idQuat", "axis", (intptr_t)(&((staticInterpolatePState_t *)0)->axis), sizeof( ((staticInterpolatePState_t *)0)->axis ) },
	{ "idVec3", "localOrigin", (intptr_t)(&((staticInterpolatePState_t *)0)->localOrigin), sizeof( ((staticInterpolatePState_t *)0)->localOrigin ) },
	{ "idQuat", "localAxis", (intptr_t)(&((staticInterpolatePState_t *)0)->localAxis), sizeof( ((staticInterpolatePState_t *)0)->localAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_Static_typeInfo[] = {
	{ ": idEntity *", "self", (intptr_t)(&((idPhysics_Static *)0)->self), sizeof( ((idPhysics_Static *)0)->self ) },
	{ "staticPState_t", "current", (intptr_t)(&((idPhysics_Static *)0)->current), sizeof( ((idPhysics_Static *)0)->current ) },
	{ "idClipModel *", "clipModel", (intptr_t)(&((idPhysics_Static *)0)->clipModel), sizeof( ((idPhysics_Static *)0)->clipModel ) },
	{ "staticInterpolatePState_t", "previous", (intptr_t)(&((idPhysics_Static *)0)->previous), sizeof( ((idPhysics_Static *)0)->previous ) },
	{ "staticInterpolatePState_t", "next", (intptr_t)(&((idPhysics_Static *)0)->next), sizeof( ((idPhysics_Static *)0)->next ) },
	{ "bool", "hasMaster", (intptr_t)(&((idPhysics_Static *)0)->hasMaster), sizeof( ((idPhysics_Static *)0)->hasMaster ) },
	{ "bool", "isOrientated", (intptr_t)(&((idPhysics_Static *)0)->isOrientated), sizeof( ((idPhysics_Static *)0)->isOrientated ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_StaticMulti_typeInfo[] = {
	{ ": idEntity *", "self", (intptr_t)(&((idPhysics_StaticMulti *)0)->self), sizeof( ((idPhysics_StaticMulti *)0)->self ) },
	{ "idList < staticPState_t , TAG_IDLIB_LIST_PHYSICS >", "current", (intptr_t)(&((idPhysics_StaticMulti *)0)->current), sizeof( ((idPhysics_StaticMulti *)0)->current ) },
	{ "idList < idClipModel * , TAG_IDLIB_LIST_PHYSICS >", "clipModels", (intptr_t)(&((idPhysics_StaticMulti *)0)->clipModels), sizeof( ((idPhysics_StaticMulti *)0)->clipModels ) },
	{ "idList < staticInterpolatePState_t , TAG_IDLIB_LIST_PHYSICS >", "previous", (intptr_t)(&((idPhysics_StaticMulti *)0)->previous), sizeof( ((idPhysics_StaticMulti *)0)->previous ) },
	{ "idList < staticInterpolatePState_t , TAG_IDLIB_LIST_PHYSICS >", "next", (intptr_t)(&((idPhysics_StaticMulti *)0)->next), sizeof( ((idPhysics_StaticMulti *)0)->next ) },
	{ "bool", "hasMaster", (intptr_t)(&((idPhysics_StaticMulti *)0)->hasMaster), sizeof( ((idPhysics_StaticMulti *)0)->hasMaster ) },
	{ "bool", "isOrientated", (intptr_t)(&((idPhysics_StaticMulti *)0)->isOrientated), sizeof( ((idPhysics_StaticMulti *)0)->isOrientated ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_Base_typeInfo[] = {
	{ ": idEntity *", "self", (intptr_t)(&((idPhysics_Base *)0)->self), sizeof( ((idPhysics_Base *)0)->self ) },
	{ "int", "clipMask", (intptr_t)(&((idPhysics_Base *)0)->clipMask), sizeof( ((idPhysics_Base *)0)->clipMask ) },
	{ "idVec3", "gravityVector", (intptr_t)(&((idPhysics_Base *)0)->gravityVector), sizeof( ((idPhysics_Base *)0)->gravityVector ) },
	{ "idVec3", "gravityNormal", (intptr_t)(&((idPhysics_Base *)0)->gravityNormal), sizeof( ((idPhysics_Base *)0)->gravityNormal ) },
	{ "idList < contactInfo_t , TAG_IDLIB_LIST_PHYSICS >", "contacts", (intptr_t)(&((idPhysics_Base *)0)->contacts), sizeof( ((idPhysics_Base *)0)->contacts ) },
	{ "idList < idEntityPtr < idEntity > , TAG_IDLIB_LIST_PHYSICS >", "contactEntities", (intptr_t)(&((idPhysics_Base *)0)->contactEntities), sizeof( ((idPhysics_Base *)0)->contactEntities ) },
	{ NULL, 0 }
};

static classVariableInfo_t physicsInterpolationState_t_typeInfo[] = {
	{ "idVec3", "origin", (intptr_t)(&((physicsInterpolationState_t *)0)->origin), sizeof( ((physicsInterpolationState_t *)0)->origin ) },
	{ "idQuat", "axis", (intptr_t)(&((physicsInterpolationState_t *)0)->axis), sizeof( ((physicsInterpolationState_t *)0)->axis ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_Actor_typeInfo[] = {
	{ ": idClipModel *", "clipModel", (intptr_t)(&((idPhysics_Actor *)0)->clipModel), sizeof( ((idPhysics_Actor *)0)->clipModel ) },
	{ "idMat3", "clipModelAxis", (intptr_t)(&((idPhysics_Actor *)0)->clipModelAxis), sizeof( ((idPhysics_Actor *)0)->clipModelAxis ) },
	{ "float", "mass", (intptr_t)(&((idPhysics_Actor *)0)->mass), sizeof( ((idPhysics_Actor *)0)->mass ) },
	{ "float", "invMass", (intptr_t)(&((idPhysics_Actor *)0)->invMass), sizeof( ((idPhysics_Actor *)0)->invMass ) },
	{ "idEntity *", "masterEntity", (intptr_t)(&((idPhysics_Actor *)0)->masterEntity), sizeof( ((idPhysics_Actor *)0)->masterEntity ) },
	{ "float", "masterYaw", (intptr_t)(&((idPhysics_Actor *)0)->masterYaw), sizeof( ((idPhysics_Actor *)0)->masterYaw ) },
	{ "float", "masterDeltaYaw", (intptr_t)(&((idPhysics_Actor *)0)->masterDeltaYaw), sizeof( ((idPhysics_Actor *)0)->masterDeltaYaw ) },
	{ "idEntityPtr < idEntity >", "groundEntityPtr", (intptr_t)(&((idPhysics_Actor *)0)->groundEntityPtr), sizeof( ((idPhysics_Actor *)0)->groundEntityPtr ) },
	{ NULL, 0 }
};

static classVariableInfo_t monsterPState_t_typeInfo[] = {
	{ "int", "atRest", (intptr_t)(&((monsterPState_t *)0)->atRest), sizeof( ((monsterPState_t *)0)->atRest ) },
	{ "bool", "onGround", (intptr_t)(&((monsterPState_t *)0)->onGround), sizeof( ((monsterPState_t *)0)->onGround ) },
	{ "idVec3", "origin", (intptr_t)(&((monsterPState_t *)0)->origin), sizeof( ((monsterPState_t *)0)->origin ) },
	{ "idVec3", "velocity", (intptr_t)(&((monsterPState_t *)0)->velocity), sizeof( ((monsterPState_t *)0)->velocity ) },
	{ "idVec3", "localOrigin", (intptr_t)(&((monsterPState_t *)0)->localOrigin), sizeof( ((monsterPState_t *)0)->localOrigin ) },
	{ "idVec3", "pushVelocity", (intptr_t)(&((monsterPState_t *)0)->pushVelocity), sizeof( ((monsterPState_t *)0)->pushVelocity ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_Monster_typeInfo[] = {
	{ ": monsterPState_t", "current", (intptr_t)(&((idPhysics_Monster *)0)->current), sizeof( ((idPhysics_Monster *)0)->current ) },
	{ "monsterPState_t", "saved", (intptr_t)(&((idPhysics_Monster *)0)->saved), sizeof( ((idPhysics_Monster *)0)->saved ) },
	{ "monsterPState_t", "previous", (intptr_t)(&((idPhysics_Monster *)0)->previous), sizeof( ((idPhysics_Monster *)0)->previous ) },
	{ "monsterPState_t", "next", (intptr_t)(&((idPhysics_Monster *)0)->next), sizeof( ((idPhysics_Monster *)0)->next ) },
	{ "float", "maxStepHeight", (intptr_t)(&((idPhysics_Monster *)0)->maxStepHeight), sizeof( ((idPhysics_Monster *)0)->maxStepHeight ) },
	{ "float", "minFloorCosine", (intptr_t)(&((idPhysics_Monster *)0)->minFloorCosine), sizeof( ((idPhysics_Monster *)0)->minFloorCosine ) },
	{ "idVec3", "delta", (intptr_t)(&((idPhysics_Monster *)0)->delta), sizeof( ((idPhysics_Monster *)0)->delta ) },
	{ "bool", "forceDeltaMove", (intptr_t)(&((idPhysics_Monster *)0)->forceDeltaMove), sizeof( ((idPhysics_Monster *)0)->forceDeltaMove ) },
	{ "bool", "fly", (intptr_t)(&((idPhysics_Monster *)0)->fly), sizeof( ((idPhysics_Monster *)0)->fly ) },
	{ "bool", "useVelocityMove", (intptr_t)(&((idPhysics_Monster *)0)->useVelocityMove), sizeof( ((idPhysics_Monster *)0)->useVelocityMove ) },
	{ "bool", "noImpact", (intptr_t)(&((idPhysics_Monster *)0)->noImpact), sizeof( ((idPhysics_Monster *)0)->noImpact ) },
	{ "monsterMoveResult_t", "moveResult", (intptr_t)(&((idPhysics_Monster *)0)->moveResult), sizeof( ((idPhysics_Monster *)0)->moveResult ) },
	{ "idEntity *", "blockingEntity", (intptr_t)(&((idPhysics_Monster *)0)->blockingEntity), sizeof( ((idPhysics_Monster *)0)->blockingEntity ) },
	{ NULL, 0 }
};

static classVariableInfo_t playerPState_t_typeInfo[] = {
	{ "idVec3", "origin", (intptr_t)(&((playerPState_t *)0)->origin), sizeof( ((playerPState_t *)0)->origin ) },
	{ "idVec3", "velocity", (intptr_t)(&((playerPState_t *)0)->velocity), sizeof( ((playerPState_t *)0)->velocity ) },
	{ "idVec3", "localOrigin", (intptr_t)(&((playerPState_t *)0)->localOrigin), sizeof( ((playerPState_t *)0)->localOrigin ) },
	{ "idVec3", "pushVelocity", (intptr_t)(&((playerPState_t *)0)->pushVelocity), sizeof( ((playerPState_t *)0)->pushVelocity ) },
	{ "float", "stepUp", (intptr_t)(&((playerPState_t *)0)->stepUp), sizeof( ((playerPState_t *)0)->stepUp ) },
	{ "int", "movementType", (intptr_t)(&((playerPState_t *)0)->movementType), sizeof( ((playerPState_t *)0)->movementType ) },
	{ "int", "movementFlags", (intptr_t)(&((playerPState_t *)0)->movementFlags), sizeof( ((playerPState_t *)0)->movementFlags ) },
	{ "int", "movementTime", (intptr_t)(&((playerPState_t *)0)->movementTime), sizeof( ((playerPState_t *)0)->movementTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_Player_typeInfo[] = {
	{ ": playerPState_t", "current", (intptr_t)(&((idPhysics_Player *)0)->current), sizeof( ((idPhysics_Player *)0)->current ) },
	{ "playerPState_t", "saved", (intptr_t)(&((idPhysics_Player *)0)->saved), sizeof( ((idPhysics_Player *)0)->saved ) },
	{ "playerPState_t", "previous", (intptr_t)(&((idPhysics_Player *)0)->previous), sizeof( ((idPhysics_Player *)0)->previous ) },
	{ "playerPState_t", "next", (intptr_t)(&((idPhysics_Player *)0)->next), sizeof( ((idPhysics_Player *)0)->next ) },
	{ "float", "walkSpeed", (intptr_t)(&((idPhysics_Player *)0)->walkSpeed), sizeof( ((idPhysics_Player *)0)->walkSpeed ) },
	{ "float", "crouchSpeed", (intptr_t)(&((idPhysics_Player *)0)->crouchSpeed), sizeof( ((idPhysics_Player *)0)->crouchSpeed ) },
	{ "float", "maxStepHeight", (intptr_t)(&((idPhysics_Player *)0)->maxStepHeight), sizeof( ((idPhysics_Player *)0)->maxStepHeight ) },
	{ "float", "maxJumpHeight", (intptr_t)(&((idPhysics_Player *)0)->maxJumpHeight), sizeof( ((idPhysics_Player *)0)->maxJumpHeight ) },
	{ "int", "debugLevel", (intptr_t)(&((idPhysics_Player *)0)->debugLevel), sizeof( ((idPhysics_Player *)0)->debugLevel ) },
	{ "usercmd_t", "command", (intptr_t)(&((idPhysics_Player *)0)->command), sizeof( ((idPhysics_Player *)0)->command ) },
	{ "idVec3", "commandForward", (intptr_t)(&((idPhysics_Player *)0)->commandForward), sizeof( ((idPhysics_Player *)0)->commandForward ) },
	{ "int", "framemsec", (intptr_t)(&((idPhysics_Player *)0)->framemsec), sizeof( ((idPhysics_Player *)0)->framemsec ) },
	{ "float", "frametime", (intptr_t)(&((idPhysics_Player *)0)->frametime), sizeof( ((idPhysics_Player *)0)->frametime ) },
	{ "float", "playerSpeed", (intptr_t)(&((idPhysics_Player *)0)->playerSpeed), sizeof( ((idPhysics_Player *)0)->playerSpeed ) },
	{ "idVec3", "viewForward", (intptr_t)(&((idPhysics_Player *)0)->viewForward), sizeof( ((idPhysics_Player *)0)->viewForward ) },
	{ "idVec3", "viewRight", (intptr_t)(&((idPhysics_Player *)0)->viewRight), sizeof( ((idPhysics_Player *)0)->viewRight ) },
	{ "bool", "walking", (intptr_t)(&((idPhysics_Player *)0)->walking), sizeof( ((idPhysics_Player *)0)->walking ) },
	{ "bool", "groundPlane", (intptr_t)(&((idPhysics_Player *)0)->groundPlane), sizeof( ((idPhysics_Player *)0)->groundPlane ) },
	{ "trace_t", "groundTrace", (intptr_t)(&((idPhysics_Player *)0)->groundTrace), sizeof( ((idPhysics_Player *)0)->groundTrace ) },
	{ "const idMaterial *", "groundMaterial", (intptr_t)(&((idPhysics_Player *)0)->groundMaterial), sizeof( ((idPhysics_Player *)0)->groundMaterial ) },
	{ "bool", "ladder", (intptr_t)(&((idPhysics_Player *)0)->ladder), sizeof( ((idPhysics_Player *)0)->ladder ) },
	{ "idVec3", "ladderNormal", (intptr_t)(&((idPhysics_Player *)0)->ladderNormal), sizeof( ((idPhysics_Player *)0)->ladderNormal ) },
	{ "waterLevel_t", "waterLevel", (intptr_t)(&((idPhysics_Player *)0)->waterLevel), sizeof( ((idPhysics_Player *)0)->waterLevel ) },
	{ "int", "waterType", (intptr_t)(&((idPhysics_Player *)0)->waterType), sizeof( ((idPhysics_Player *)0)->waterType ) },
	{ "bool", "clientPusherLocked", (intptr_t)(&((idPhysics_Player *)0)->clientPusherLocked), sizeof( ((idPhysics_Player *)0)->clientPusherLocked ) },
	{ NULL, 0 }
};

static classVariableInfo_t parametricPState_t_typeInfo[] = {
	{ "int", "time", (intptr_t)(&((parametricPState_t *)0)->time), sizeof( ((parametricPState_t *)0)->time ) },
	{ "int", "atRest", (intptr_t)(&((parametricPState_t *)0)->atRest), sizeof( ((parametricPState_t *)0)->atRest ) },
	{ "idVec3", "origin", (intptr_t)(&((parametricPState_t *)0)->origin), sizeof( ((parametricPState_t *)0)->origin ) },
	{ "idAngles", "angles", (intptr_t)(&((parametricPState_t *)0)->angles), sizeof( ((parametricPState_t *)0)->angles ) },
	{ "idMat3", "axis", (intptr_t)(&((parametricPState_t *)0)->axis), sizeof( ((parametricPState_t *)0)->axis ) },
	{ "idVec3", "localOrigin", (intptr_t)(&((parametricPState_t *)0)->localOrigin), sizeof( ((parametricPState_t *)0)->localOrigin ) },
	{ "idAngles", "localAngles", (intptr_t)(&((parametricPState_t *)0)->localAngles), sizeof( ((parametricPState_t *)0)->localAngles ) },
	{ "idExtrapolate < idVec3 >", "linearExtrapolation", (intptr_t)(&((parametricPState_t *)0)->linearExtrapolation), sizeof( ((parametricPState_t *)0)->linearExtrapolation ) },
	{ "idExtrapolate < idAngles >", "angularExtrapolation", (intptr_t)(&((parametricPState_t *)0)->angularExtrapolation), sizeof( ((parametricPState_t *)0)->angularExtrapolation ) },
	{ "idInterpolateAccelDecelLinear < idVec3 >", "linearInterpolation", (intptr_t)(&((parametricPState_t *)0)->linearInterpolation), sizeof( ((parametricPState_t *)0)->linearInterpolation ) },
	{ "idInterpolateAccelDecelLinear < idAngles >", "angularInterpolation", (intptr_t)(&((parametricPState_t *)0)->angularInterpolation), sizeof( ((parametricPState_t *)0)->angularInterpolation ) },
	{ "idCurve_Spline < idVec3 > *", "spline", (intptr_t)(&((parametricPState_t *)0)->spline), sizeof( ((parametricPState_t *)0)->spline ) },
	{ "idInterpolateAccelDecelLinear < float >", "splineInterpolate", (intptr_t)(&((parametricPState_t *)0)->splineInterpolate), sizeof( ((parametricPState_t *)0)->splineInterpolate ) },
	{ "bool", "useSplineAngles", (intptr_t)(&((parametricPState_t *)0)->useSplineAngles), sizeof( ((parametricPState_t *)0)->useSplineAngles ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_Parametric_typeInfo[] = {
	{ ": parametricPState_t", "current", (intptr_t)(&((idPhysics_Parametric *)0)->current), sizeof( ((idPhysics_Parametric *)0)->current ) },
	{ "parametricPState_t", "saved", (intptr_t)(&((idPhysics_Parametric *)0)->saved), sizeof( ((idPhysics_Parametric *)0)->saved ) },
	{ "physicsInterpolationState_t", "previous", (intptr_t)(&((idPhysics_Parametric *)0)->previous), sizeof( ((idPhysics_Parametric *)0)->previous ) },
	{ "physicsInterpolationState_t", "next", (intptr_t)(&((idPhysics_Parametric *)0)->next), sizeof( ((idPhysics_Parametric *)0)->next ) },
	{ "bool", "isPusher", (intptr_t)(&((idPhysics_Parametric *)0)->isPusher), sizeof( ((idPhysics_Parametric *)0)->isPusher ) },
	{ "idClipModel *", "clipModel", (intptr_t)(&((idPhysics_Parametric *)0)->clipModel), sizeof( ((idPhysics_Parametric *)0)->clipModel ) },
	{ "int", "pushFlags", (intptr_t)(&((idPhysics_Parametric *)0)->pushFlags), sizeof( ((idPhysics_Parametric *)0)->pushFlags ) },
	{ "trace_t", "pushResults", (intptr_t)(&((idPhysics_Parametric *)0)->pushResults), sizeof( ((idPhysics_Parametric *)0)->pushResults ) },
	{ "bool", "isBlocked", (intptr_t)(&((idPhysics_Parametric *)0)->isBlocked), sizeof( ((idPhysics_Parametric *)0)->isBlocked ) },
	{ "bool", "hasMaster", (intptr_t)(&((idPhysics_Parametric *)0)->hasMaster), sizeof( ((idPhysics_Parametric *)0)->hasMaster ) },
	{ "bool", "isOrientated", (intptr_t)(&((idPhysics_Parametric *)0)->isOrientated), sizeof( ((idPhysics_Parametric *)0)->isOrientated ) },
	{ NULL, 0 }
};

static classVariableInfo_t rigidBodyIState_t_typeInfo[] = {
	{ "idVec3", "position", (intptr_t)(&((rigidBodyIState_t *)0)->position), sizeof( ((rigidBodyIState_t *)0)->position ) },
	{ "idMat3", "orientation", (intptr_t)(&((rigidBodyIState_t *)0)->orientation), sizeof( ((rigidBodyIState_t *)0)->orientation ) },
	{ "idVec3", "linearMomentum", (intptr_t)(&((rigidBodyIState_t *)0)->linearMomentum), sizeof( ((rigidBodyIState_t *)0)->linearMomentum ) },
	{ "idVec3", "angularMomentum", (intptr_t)(&((rigidBodyIState_t *)0)->angularMomentum), sizeof( ((rigidBodyIState_t *)0)->angularMomentum ) },
	{ NULL, 0 }
};

static classVariableInfo_t rigidBodyPState_t_typeInfo[] = {
	{ "int", "atRest", (intptr_t)(&((rigidBodyPState_t *)0)->atRest), sizeof( ((rigidBodyPState_t *)0)->atRest ) },
	{ "float", "lastTimeStep", (intptr_t)(&((rigidBodyPState_t *)0)->lastTimeStep), sizeof( ((rigidBodyPState_t *)0)->lastTimeStep ) },
	{ "idVec3", "localOrigin", (intptr_t)(&((rigidBodyPState_t *)0)->localOrigin), sizeof( ((rigidBodyPState_t *)0)->localOrigin ) },
	{ "idMat3", "localAxis", (intptr_t)(&((rigidBodyPState_t *)0)->localAxis), sizeof( ((rigidBodyPState_t *)0)->localAxis ) },
	{ "idVec6", "pushVelocity", (intptr_t)(&((rigidBodyPState_t *)0)->pushVelocity), sizeof( ((rigidBodyPState_t *)0)->pushVelocity ) },
	{ "idVec3", "externalForce", (intptr_t)(&((rigidBodyPState_t *)0)->externalForce), sizeof( ((rigidBodyPState_t *)0)->externalForce ) },
	{ "idVec3", "externalTorque", (intptr_t)(&((rigidBodyPState_t *)0)->externalTorque), sizeof( ((rigidBodyPState_t *)0)->externalTorque ) },
	{ "rigidBodyIState_t", "i", (intptr_t)(&((rigidBodyPState_t *)0)->i), sizeof( ((rigidBodyPState_t *)0)->i ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_RigidBody_typeInfo[] = {
	{ ": rigidBodyPState_t", "current", (intptr_t)(&((idPhysics_RigidBody *)0)->current), sizeof( ((idPhysics_RigidBody *)0)->current ) },
	{ "rigidBodyPState_t", "saved", (intptr_t)(&((idPhysics_RigidBody *)0)->saved), sizeof( ((idPhysics_RigidBody *)0)->saved ) },
	{ "rigidBodyPState_t", "previous", (intptr_t)(&((idPhysics_RigidBody *)0)->previous), sizeof( ((idPhysics_RigidBody *)0)->previous ) },
	{ "rigidBodyPState_t", "next", (intptr_t)(&((idPhysics_RigidBody *)0)->next), sizeof( ((idPhysics_RigidBody *)0)->next ) },
	{ "float", "linearFriction", (intptr_t)(&((idPhysics_RigidBody *)0)->linearFriction), sizeof( ((idPhysics_RigidBody *)0)->linearFriction ) },
	{ "float", "angularFriction", (intptr_t)(&((idPhysics_RigidBody *)0)->angularFriction), sizeof( ((idPhysics_RigidBody *)0)->angularFriction ) },
	{ "float", "contactFriction", (intptr_t)(&((idPhysics_RigidBody *)0)->contactFriction), sizeof( ((idPhysics_RigidBody *)0)->contactFriction ) },
	{ "float", "bouncyness", (intptr_t)(&((idPhysics_RigidBody *)0)->bouncyness), sizeof( ((idPhysics_RigidBody *)0)->bouncyness ) },
	{ "idClipModel *", "clipModel", (intptr_t)(&((idPhysics_RigidBody *)0)->clipModel), sizeof( ((idPhysics_RigidBody *)0)->clipModel ) },
	{ "float", "mass", (intptr_t)(&((idPhysics_RigidBody *)0)->mass), sizeof( ((idPhysics_RigidBody *)0)->mass ) },
	{ "float", "inverseMass", (intptr_t)(&((idPhysics_RigidBody *)0)->inverseMass), sizeof( ((idPhysics_RigidBody *)0)->inverseMass ) },
	{ "idVec3", "centerOfMass", (intptr_t)(&((idPhysics_RigidBody *)0)->centerOfMass), sizeof( ((idPhysics_RigidBody *)0)->centerOfMass ) },
	{ "idMat3", "inertiaTensor", (intptr_t)(&((idPhysics_RigidBody *)0)->inertiaTensor), sizeof( ((idPhysics_RigidBody *)0)->inertiaTensor ) },
	{ "idMat3", "inverseInertiaTensor", (intptr_t)(&((idPhysics_RigidBody *)0)->inverseInertiaTensor), sizeof( ((idPhysics_RigidBody *)0)->inverseInertiaTensor ) },
	{ "idODE *", "integrator", (intptr_t)(&((idPhysics_RigidBody *)0)->integrator), sizeof( ((idPhysics_RigidBody *)0)->integrator ) },
	{ "bool", "dropToFloor", (intptr_t)(&((idPhysics_RigidBody *)0)->dropToFloor), sizeof( ((idPhysics_RigidBody *)0)->dropToFloor ) },
	{ "bool", "testSolid", (intptr_t)(&((idPhysics_RigidBody *)0)->testSolid), sizeof( ((idPhysics_RigidBody *)0)->testSolid ) },
	{ "bool", "noImpact", (intptr_t)(&((idPhysics_RigidBody *)0)->noImpact), sizeof( ((idPhysics_RigidBody *)0)->noImpact ) },
	{ "bool", "noContact", (intptr_t)(&((idPhysics_RigidBody *)0)->noContact), sizeof( ((idPhysics_RigidBody *)0)->noContact ) },
	{ "bool", "hasMaster", (intptr_t)(&((idPhysics_RigidBody *)0)->hasMaster), sizeof( ((idPhysics_RigidBody *)0)->hasMaster ) },
	{ "bool", "isOrientated", (intptr_t)(&((idPhysics_RigidBody *)0)->isOrientated), sizeof( ((idPhysics_RigidBody *)0)->isOrientated ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_constraintFlags_s_typeInfo[] = {
//	{ "bool", "allowPrimary", (intptr_t)(&((idAFConstraint::constraintFlags_s *)0)->allowPrimary), sizeof( ((idAFConstraint::constraintFlags_s *)0)->allowPrimary ) },
//	{ "bool", "frameConstraint", (intptr_t)(&((idAFConstraint::constraintFlags_s *)0)->frameConstraint), sizeof( ((idAFConstraint::constraintFlags_s *)0)->frameConstraint ) },
//	{ "bool", "noCollision", (intptr_t)(&((idAFConstraint::constraintFlags_s *)0)->noCollision), sizeof( ((idAFConstraint::constraintFlags_s *)0)->noCollision ) },
//	{ "bool", "isPrimary", (intptr_t)(&((idAFConstraint::constraintFlags_s *)0)->isPrimary), sizeof( ((idAFConstraint::constraintFlags_s *)0)->isPrimary ) },
//	{ "bool", "isZero", (intptr_t)(&((idAFConstraint::constraintFlags_s *)0)->isZero), sizeof( ((idAFConstraint::constraintFlags_s *)0)->isZero ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_typeInfo[] = {
	{ ": constraintType_t", "type", (intptr_t)(&((idAFConstraint *)0)->type), sizeof( ((idAFConstraint *)0)->type ) },
	{ "idStr", "name", (intptr_t)(&((idAFConstraint *)0)->name), sizeof( ((idAFConstraint *)0)->name ) },
	{ "idAFBody *", "body1", (intptr_t)(&((idAFConstraint *)0)->body1), sizeof( ((idAFConstraint *)0)->body1 ) },
	{ "idAFBody *", "body2", (intptr_t)(&((idAFConstraint *)0)->body2), sizeof( ((idAFConstraint *)0)->body2 ) },
	{ "idPhysics_AF *", "physics", (intptr_t)(&((idAFConstraint *)0)->physics), sizeof( ((idAFConstraint *)0)->physics ) },
	{ "idMatX", "J1", (intptr_t)(&((idAFConstraint *)0)->J1), sizeof( ((idAFConstraint *)0)->J1 ) },
	{ "idMatX", "J2", (intptr_t)(&((idAFConstraint *)0)->J2), sizeof( ((idAFConstraint *)0)->J2 ) },
	{ "idVecX", "c1", (intptr_t)(&((idAFConstraint *)0)->c1), sizeof( ((idAFConstraint *)0)->c1 ) },
	{ "idVecX", "c2", (intptr_t)(&((idAFConstraint *)0)->c2), sizeof( ((idAFConstraint *)0)->c2 ) },
	{ "idVecX", "lo", (intptr_t)(&((idAFConstraint *)0)->lo), sizeof( ((idAFConstraint *)0)->lo ) },
	{ "idVecX", "hi", (intptr_t)(&((idAFConstraint *)0)->hi), sizeof( ((idAFConstraint *)0)->hi ) },
	{ "idVecX", "e", (intptr_t)(&((idAFConstraint *)0)->e), sizeof( ((idAFConstraint *)0)->e ) },
	{ "idAFConstraint *", "boxConstraint", (intptr_t)(&((idAFConstraint *)0)->boxConstraint), sizeof( ((idAFConstraint *)0)->boxConstraint ) },
	{ "int[6]", "boxIndex", (intptr_t)(&((idAFConstraint *)0)->boxIndex), sizeof( ((idAFConstraint *)0)->boxIndex ) },
	{ "idMatX", "invI", (intptr_t)(&((idAFConstraint *)0)->invI), sizeof( ((idAFConstraint *)0)->invI ) },
	{ "idMatX", "J", (intptr_t)(&((idAFConstraint *)0)->J), sizeof( ((idAFConstraint *)0)->J ) },
	{ "idVecX", "s", (intptr_t)(&((idAFConstraint *)0)->s), sizeof( ((idAFConstraint *)0)->s ) },
	{ "idVecX", "lm", (intptr_t)(&((idAFConstraint *)0)->lm), sizeof( ((idAFConstraint *)0)->lm ) },
	{ "int", "firstIndex", (intptr_t)(&((idAFConstraint *)0)->firstIndex), sizeof( ((idAFConstraint *)0)->firstIndex ) },
	{ "idAFConstraint::constraintFlags_s", "fl", (intptr_t)(&((idAFConstraint *)0)->fl), sizeof( ((idAFConstraint *)0)->fl ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Fixed_typeInfo[] = {
	{ ": idVec3", "offset", (intptr_t)(&((idAFConstraint_Fixed *)0)->offset), sizeof( ((idAFConstraint_Fixed *)0)->offset ) },
	{ "idMat3", "relAxis", (intptr_t)(&((idAFConstraint_Fixed *)0)->relAxis), sizeof( ((idAFConstraint_Fixed *)0)->relAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_BallAndSocketJoint_typeInfo[] = {
	{ ": idVec3", "anchor1", (intptr_t)(&((idAFConstraint_BallAndSocketJoint *)0)->anchor1), sizeof( ((idAFConstraint_BallAndSocketJoint *)0)->anchor1 ) },
	{ "idVec3", "anchor2", (intptr_t)(&((idAFConstraint_BallAndSocketJoint *)0)->anchor2), sizeof( ((idAFConstraint_BallAndSocketJoint *)0)->anchor2 ) },
	{ "float", "friction", (intptr_t)(&((idAFConstraint_BallAndSocketJoint *)0)->friction), sizeof( ((idAFConstraint_BallAndSocketJoint *)0)->friction ) },
	{ "idAFConstraint_ConeLimit *", "coneLimit", (intptr_t)(&((idAFConstraint_BallAndSocketJoint *)0)->coneLimit), sizeof( ((idAFConstraint_BallAndSocketJoint *)0)->coneLimit ) },
	{ "idAFConstraint_PyramidLimit *", "pyramidLimit", (intptr_t)(&((idAFConstraint_BallAndSocketJoint *)0)->pyramidLimit), sizeof( ((idAFConstraint_BallAndSocketJoint *)0)->pyramidLimit ) },
	{ "idAFConstraint_BallAndSocketJointFriction *", "fc", (intptr_t)(&((idAFConstraint_BallAndSocketJoint *)0)->fc), sizeof( ((idAFConstraint_BallAndSocketJoint *)0)->fc ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_BallAndSocketJointFriction_typeInfo[] = {
	{ ": idAFConstraint_BallAndSocketJoint *", "joint", (intptr_t)(&((idAFConstraint_BallAndSocketJointFriction *)0)->joint), sizeof( ((idAFConstraint_BallAndSocketJointFriction *)0)->joint ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_UniversalJoint_typeInfo[] = {
	{ ": idVec3", "anchor1", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->anchor1), sizeof( ((idAFConstraint_UniversalJoint *)0)->anchor1 ) },
	{ "idVec3", "anchor2", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->anchor2), sizeof( ((idAFConstraint_UniversalJoint *)0)->anchor2 ) },
	{ "idVec3", "shaft1", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->shaft1), sizeof( ((idAFConstraint_UniversalJoint *)0)->shaft1 ) },
	{ "idVec3", "shaft2", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->shaft2), sizeof( ((idAFConstraint_UniversalJoint *)0)->shaft2 ) },
	{ "idVec3", "axis1", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->axis1), sizeof( ((idAFConstraint_UniversalJoint *)0)->axis1 ) },
	{ "idVec3", "axis2", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->axis2), sizeof( ((idAFConstraint_UniversalJoint *)0)->axis2 ) },
	{ "float", "friction", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->friction), sizeof( ((idAFConstraint_UniversalJoint *)0)->friction ) },
	{ "idAFConstraint_ConeLimit *", "coneLimit", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->coneLimit), sizeof( ((idAFConstraint_UniversalJoint *)0)->coneLimit ) },
	{ "idAFConstraint_PyramidLimit *", "pyramidLimit", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->pyramidLimit), sizeof( ((idAFConstraint_UniversalJoint *)0)->pyramidLimit ) },
	{ "idAFConstraint_UniversalJointFriction *", "fc", (intptr_t)(&((idAFConstraint_UniversalJoint *)0)->fc), sizeof( ((idAFConstraint_UniversalJoint *)0)->fc ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_UniversalJointFriction_typeInfo[] = {
	{ ": idAFConstraint_UniversalJoint *", "joint", (intptr_t)(&((idAFConstraint_UniversalJointFriction *)0)->joint), sizeof( ((idAFConstraint_UniversalJointFriction *)0)->joint ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_CylindricalJoint_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Hinge_typeInfo[] = {
	{ ": idVec3", "anchor1", (intptr_t)(&((idAFConstraint_Hinge *)0)->anchor1), sizeof( ((idAFConstraint_Hinge *)0)->anchor1 ) },
	{ "idVec3", "anchor2", (intptr_t)(&((idAFConstraint_Hinge *)0)->anchor2), sizeof( ((idAFConstraint_Hinge *)0)->anchor2 ) },
	{ "idVec3", "axis1", (intptr_t)(&((idAFConstraint_Hinge *)0)->axis1), sizeof( ((idAFConstraint_Hinge *)0)->axis1 ) },
	{ "idVec3", "axis2", (intptr_t)(&((idAFConstraint_Hinge *)0)->axis2), sizeof( ((idAFConstraint_Hinge *)0)->axis2 ) },
	{ "idMat3", "initialAxis", (intptr_t)(&((idAFConstraint_Hinge *)0)->initialAxis), sizeof( ((idAFConstraint_Hinge *)0)->initialAxis ) },
	{ "float", "friction", (intptr_t)(&((idAFConstraint_Hinge *)0)->friction), sizeof( ((idAFConstraint_Hinge *)0)->friction ) },
	{ "idAFConstraint_ConeLimit *", "coneLimit", (intptr_t)(&((idAFConstraint_Hinge *)0)->coneLimit), sizeof( ((idAFConstraint_Hinge *)0)->coneLimit ) },
	{ "idAFConstraint_HingeSteering *", "steering", (intptr_t)(&((idAFConstraint_Hinge *)0)->steering), sizeof( ((idAFConstraint_Hinge *)0)->steering ) },
	{ "idAFConstraint_HingeFriction *", "fc", (intptr_t)(&((idAFConstraint_Hinge *)0)->fc), sizeof( ((idAFConstraint_Hinge *)0)->fc ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_HingeFriction_typeInfo[] = {
	{ ": idAFConstraint_Hinge *", "hinge", (intptr_t)(&((idAFConstraint_HingeFriction *)0)->hinge), sizeof( ((idAFConstraint_HingeFriction *)0)->hinge ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_HingeSteering_typeInfo[] = {
	{ ": idAFConstraint_Hinge *", "hinge", (intptr_t)(&((idAFConstraint_HingeSteering *)0)->hinge), sizeof( ((idAFConstraint_HingeSteering *)0)->hinge ) },
	{ "float", "steerAngle", (intptr_t)(&((idAFConstraint_HingeSteering *)0)->steerAngle), sizeof( ((idAFConstraint_HingeSteering *)0)->steerAngle ) },
	{ "float", "steerSpeed", (intptr_t)(&((idAFConstraint_HingeSteering *)0)->steerSpeed), sizeof( ((idAFConstraint_HingeSteering *)0)->steerSpeed ) },
	{ "float", "epsilon", (intptr_t)(&((idAFConstraint_HingeSteering *)0)->epsilon), sizeof( ((idAFConstraint_HingeSteering *)0)->epsilon ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Slider_typeInfo[] = {
	{ ": idVec3", "axis", (intptr_t)(&((idAFConstraint_Slider *)0)->axis), sizeof( ((idAFConstraint_Slider *)0)->axis ) },
	{ "idVec3", "offset", (intptr_t)(&((idAFConstraint_Slider *)0)->offset), sizeof( ((idAFConstraint_Slider *)0)->offset ) },
	{ "idMat3", "relAxis", (intptr_t)(&((idAFConstraint_Slider *)0)->relAxis), sizeof( ((idAFConstraint_Slider *)0)->relAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Line_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Plane_typeInfo[] = {
	{ ": idVec3", "anchor1", (intptr_t)(&((idAFConstraint_Plane *)0)->anchor1), sizeof( ((idAFConstraint_Plane *)0)->anchor1 ) },
	{ "idVec3", "anchor2", (intptr_t)(&((idAFConstraint_Plane *)0)->anchor2), sizeof( ((idAFConstraint_Plane *)0)->anchor2 ) },
	{ "idVec3", "planeNormal", (intptr_t)(&((idAFConstraint_Plane *)0)->planeNormal), sizeof( ((idAFConstraint_Plane *)0)->planeNormal ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Spring_typeInfo[] = {
	{ ": idVec3", "anchor1", (intptr_t)(&((idAFConstraint_Spring *)0)->anchor1), sizeof( ((idAFConstraint_Spring *)0)->anchor1 ) },
	{ "idVec3", "anchor2", (intptr_t)(&((idAFConstraint_Spring *)0)->anchor2), sizeof( ((idAFConstraint_Spring *)0)->anchor2 ) },
	{ "float", "kstretch", (intptr_t)(&((idAFConstraint_Spring *)0)->kstretch), sizeof( ((idAFConstraint_Spring *)0)->kstretch ) },
	{ "float", "kcompress", (intptr_t)(&((idAFConstraint_Spring *)0)->kcompress), sizeof( ((idAFConstraint_Spring *)0)->kcompress ) },
	{ "float", "damping", (intptr_t)(&((idAFConstraint_Spring *)0)->damping), sizeof( ((idAFConstraint_Spring *)0)->damping ) },
	{ "float", "restLength", (intptr_t)(&((idAFConstraint_Spring *)0)->restLength), sizeof( ((idAFConstraint_Spring *)0)->restLength ) },
	{ "float", "minLength", (intptr_t)(&((idAFConstraint_Spring *)0)->minLength), sizeof( ((idAFConstraint_Spring *)0)->minLength ) },
	{ "float", "maxLength", (intptr_t)(&((idAFConstraint_Spring *)0)->maxLength), sizeof( ((idAFConstraint_Spring *)0)->maxLength ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Contact_typeInfo[] = {
	{ ": contactInfo_t", "contact", (intptr_t)(&((idAFConstraint_Contact *)0)->contact), sizeof( ((idAFConstraint_Contact *)0)->contact ) },
	{ "idAFConstraint_ContactFriction *", "fc", (intptr_t)(&((idAFConstraint_Contact *)0)->fc), sizeof( ((idAFConstraint_Contact *)0)->fc ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_ContactFriction_typeInfo[] = {
	{ ": idAFConstraint_Contact *", "cc", (intptr_t)(&((idAFConstraint_ContactFriction *)0)->cc), sizeof( ((idAFConstraint_ContactFriction *)0)->cc ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_ConeLimit_typeInfo[] = {
	{ ": idVec3", "coneAnchor", (intptr_t)(&((idAFConstraint_ConeLimit *)0)->coneAnchor), sizeof( ((idAFConstraint_ConeLimit *)0)->coneAnchor ) },
	{ "idVec3", "coneAxis", (intptr_t)(&((idAFConstraint_ConeLimit *)0)->coneAxis), sizeof( ((idAFConstraint_ConeLimit *)0)->coneAxis ) },
	{ "idVec3", "body1Axis", (intptr_t)(&((idAFConstraint_ConeLimit *)0)->body1Axis), sizeof( ((idAFConstraint_ConeLimit *)0)->body1Axis ) },
	{ "float", "cosAngle", (intptr_t)(&((idAFConstraint_ConeLimit *)0)->cosAngle), sizeof( ((idAFConstraint_ConeLimit *)0)->cosAngle ) },
	{ "float", "sinHalfAngle", (intptr_t)(&((idAFConstraint_ConeLimit *)0)->sinHalfAngle), sizeof( ((idAFConstraint_ConeLimit *)0)->sinHalfAngle ) },
	{ "float", "cosHalfAngle", (intptr_t)(&((idAFConstraint_ConeLimit *)0)->cosHalfAngle), sizeof( ((idAFConstraint_ConeLimit *)0)->cosHalfAngle ) },
	{ "float", "epsilon", (intptr_t)(&((idAFConstraint_ConeLimit *)0)->epsilon), sizeof( ((idAFConstraint_ConeLimit *)0)->epsilon ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_PyramidLimit_typeInfo[] = {
	{ ": idVec3", "pyramidAnchor", (intptr_t)(&((idAFConstraint_PyramidLimit *)0)->pyramidAnchor), sizeof( ((idAFConstraint_PyramidLimit *)0)->pyramidAnchor ) },
	{ "idMat3", "pyramidBasis", (intptr_t)(&((idAFConstraint_PyramidLimit *)0)->pyramidBasis), sizeof( ((idAFConstraint_PyramidLimit *)0)->pyramidBasis ) },
	{ "idVec3", "body1Axis", (intptr_t)(&((idAFConstraint_PyramidLimit *)0)->body1Axis), sizeof( ((idAFConstraint_PyramidLimit *)0)->body1Axis ) },
	{ "float[2]", "cosAngle", (intptr_t)(&((idAFConstraint_PyramidLimit *)0)->cosAngle), sizeof( ((idAFConstraint_PyramidLimit *)0)->cosAngle ) },
	{ "float[2]", "sinHalfAngle", (intptr_t)(&((idAFConstraint_PyramidLimit *)0)->sinHalfAngle), sizeof( ((idAFConstraint_PyramidLimit *)0)->sinHalfAngle ) },
	{ "float[2]", "cosHalfAngle", (intptr_t)(&((idAFConstraint_PyramidLimit *)0)->cosHalfAngle), sizeof( ((idAFConstraint_PyramidLimit *)0)->cosHalfAngle ) },
	{ "float", "epsilon", (intptr_t)(&((idAFConstraint_PyramidLimit *)0)->epsilon), sizeof( ((idAFConstraint_PyramidLimit *)0)->epsilon ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFConstraint_Suspension_typeInfo[] = {
	{ ": idVec3", "localOrigin", (intptr_t)(&((idAFConstraint_Suspension *)0)->localOrigin), sizeof( ((idAFConstraint_Suspension *)0)->localOrigin ) },
	{ "idMat3", "localAxis", (intptr_t)(&((idAFConstraint_Suspension *)0)->localAxis), sizeof( ((idAFConstraint_Suspension *)0)->localAxis ) },
	{ "float", "suspensionUp", (intptr_t)(&((idAFConstraint_Suspension *)0)->suspensionUp), sizeof( ((idAFConstraint_Suspension *)0)->suspensionUp ) },
	{ "float", "suspensionDown", (intptr_t)(&((idAFConstraint_Suspension *)0)->suspensionDown), sizeof( ((idAFConstraint_Suspension *)0)->suspensionDown ) },
	{ "float", "suspensionKCompress", (intptr_t)(&((idAFConstraint_Suspension *)0)->suspensionKCompress), sizeof( ((idAFConstraint_Suspension *)0)->suspensionKCompress ) },
	{ "float", "suspensionDamping", (intptr_t)(&((idAFConstraint_Suspension *)0)->suspensionDamping), sizeof( ((idAFConstraint_Suspension *)0)->suspensionDamping ) },
	{ "float", "steerAngle", (intptr_t)(&((idAFConstraint_Suspension *)0)->steerAngle), sizeof( ((idAFConstraint_Suspension *)0)->steerAngle ) },
	{ "float", "friction", (intptr_t)(&((idAFConstraint_Suspension *)0)->friction), sizeof( ((idAFConstraint_Suspension *)0)->friction ) },
	{ "bool", "motorEnabled", (intptr_t)(&((idAFConstraint_Suspension *)0)->motorEnabled), sizeof( ((idAFConstraint_Suspension *)0)->motorEnabled ) },
	{ "float", "motorForce", (intptr_t)(&((idAFConstraint_Suspension *)0)->motorForce), sizeof( ((idAFConstraint_Suspension *)0)->motorForce ) },
	{ "float", "motorVelocity", (intptr_t)(&((idAFConstraint_Suspension *)0)->motorVelocity), sizeof( ((idAFConstraint_Suspension *)0)->motorVelocity ) },
	{ "idClipModel *", "wheelModel", (intptr_t)(&((idAFConstraint_Suspension *)0)->wheelModel), sizeof( ((idAFConstraint_Suspension *)0)->wheelModel ) },
	{ "idVec3", "wheelOffset", (intptr_t)(&((idAFConstraint_Suspension *)0)->wheelOffset), sizeof( ((idAFConstraint_Suspension *)0)->wheelOffset ) },
	{ "trace_t", "trace", (intptr_t)(&((idAFConstraint_Suspension *)0)->trace), sizeof( ((idAFConstraint_Suspension *)0)->trace ) },
	{ "float", "epsilon", (intptr_t)(&((idAFConstraint_Suspension *)0)->epsilon), sizeof( ((idAFConstraint_Suspension *)0)->epsilon ) },
	{ NULL, 0 }
};

static classVariableInfo_t AFBodyPState_t_typeInfo[] = {
	{ "idVec3", "worldOrigin", (intptr_t)(&((AFBodyPState_t *)0)->worldOrigin), sizeof( ((AFBodyPState_t *)0)->worldOrigin ) },
	{ "idMat3", "worldAxis", (intptr_t)(&((AFBodyPState_t *)0)->worldAxis), sizeof( ((AFBodyPState_t *)0)->worldAxis ) },
	{ "idVec6", "spatialVelocity", (intptr_t)(&((AFBodyPState_t *)0)->spatialVelocity), sizeof( ((AFBodyPState_t *)0)->spatialVelocity ) },
	{ "idVec6", "externalForce", (intptr_t)(&((AFBodyPState_t *)0)->externalForce), sizeof( ((AFBodyPState_t *)0)->externalForce ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFBody_bodyFlags_s_typeInfo[] = {
//	{ "bool", "clipMaskSet", (intptr_t)(&((idAFBody::bodyFlags_s *)0)->clipMaskSet), sizeof( ((idAFBody::bodyFlags_s *)0)->clipMaskSet ) },
//	{ "bool", "selfCollision", (intptr_t)(&((idAFBody::bodyFlags_s *)0)->selfCollision), sizeof( ((idAFBody::bodyFlags_s *)0)->selfCollision ) },
//	{ "bool", "spatialInertiaSparse", (intptr_t)(&((idAFBody::bodyFlags_s *)0)->spatialInertiaSparse), sizeof( ((idAFBody::bodyFlags_s *)0)->spatialInertiaSparse ) },
//	{ "bool", "useFrictionDir", (intptr_t)(&((idAFBody::bodyFlags_s *)0)->useFrictionDir), sizeof( ((idAFBody::bodyFlags_s *)0)->useFrictionDir ) },
//	{ "bool", "useContactMotorDir", (intptr_t)(&((idAFBody::bodyFlags_s *)0)->useContactMotorDir), sizeof( ((idAFBody::bodyFlags_s *)0)->useContactMotorDir ) },
//	{ "bool", "isZero", (intptr_t)(&((idAFBody::bodyFlags_s *)0)->isZero), sizeof( ((idAFBody::bodyFlags_s *)0)->isZero ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFBody_typeInfo[] = {
	{ ": idStr", "name", (intptr_t)(&((idAFBody *)0)->name), sizeof( ((idAFBody *)0)->name ) },
	{ "idAFBody *", "parent", (intptr_t)(&((idAFBody *)0)->parent), sizeof( ((idAFBody *)0)->parent ) },
	{ "idList < idAFBody * , TAG_IDLIB_LIST_PHYSICS >", "children", (intptr_t)(&((idAFBody *)0)->children), sizeof( ((idAFBody *)0)->children ) },
	{ "idClipModel *", "clipModel", (intptr_t)(&((idAFBody *)0)->clipModel), sizeof( ((idAFBody *)0)->clipModel ) },
	{ "idAFConstraint *", "primaryConstraint", (intptr_t)(&((idAFBody *)0)->primaryConstraint), sizeof( ((idAFBody *)0)->primaryConstraint ) },
	{ "idList < idAFConstraint * , TAG_IDLIB_LIST_PHYSICS >", "constraints", (intptr_t)(&((idAFBody *)0)->constraints), sizeof( ((idAFBody *)0)->constraints ) },
	{ "idAFTree *", "tree", (intptr_t)(&((idAFBody *)0)->tree), sizeof( ((idAFBody *)0)->tree ) },
	{ "float", "linearFriction", (intptr_t)(&((idAFBody *)0)->linearFriction), sizeof( ((idAFBody *)0)->linearFriction ) },
	{ "float", "angularFriction", (intptr_t)(&((idAFBody *)0)->angularFriction), sizeof( ((idAFBody *)0)->angularFriction ) },
	{ "float", "contactFriction", (intptr_t)(&((idAFBody *)0)->contactFriction), sizeof( ((idAFBody *)0)->contactFriction ) },
	{ "float", "bouncyness", (intptr_t)(&((idAFBody *)0)->bouncyness), sizeof( ((idAFBody *)0)->bouncyness ) },
	{ "int", "clipMask", (intptr_t)(&((idAFBody *)0)->clipMask), sizeof( ((idAFBody *)0)->clipMask ) },
	{ "idVec3", "frictionDir", (intptr_t)(&((idAFBody *)0)->frictionDir), sizeof( ((idAFBody *)0)->frictionDir ) },
	{ "idVec3", "contactMotorDir", (intptr_t)(&((idAFBody *)0)->contactMotorDir), sizeof( ((idAFBody *)0)->contactMotorDir ) },
	{ "float", "contactMotorVelocity", (intptr_t)(&((idAFBody *)0)->contactMotorVelocity), sizeof( ((idAFBody *)0)->contactMotorVelocity ) },
	{ "float", "contactMotorForce", (intptr_t)(&((idAFBody *)0)->contactMotorForce), sizeof( ((idAFBody *)0)->contactMotorForce ) },
	{ "float", "mass", (intptr_t)(&((idAFBody *)0)->mass), sizeof( ((idAFBody *)0)->mass ) },
	{ "float", "invMass", (intptr_t)(&((idAFBody *)0)->invMass), sizeof( ((idAFBody *)0)->invMass ) },
	{ "idVec3", "centerOfMass", (intptr_t)(&((idAFBody *)0)->centerOfMass), sizeof( ((idAFBody *)0)->centerOfMass ) },
	{ "idMat3", "inertiaTensor", (intptr_t)(&((idAFBody *)0)->inertiaTensor), sizeof( ((idAFBody *)0)->inertiaTensor ) },
	{ "idMat3", "inverseInertiaTensor", (intptr_t)(&((idAFBody *)0)->inverseInertiaTensor), sizeof( ((idAFBody *)0)->inverseInertiaTensor ) },
	{ "AFBodyPState_t[2]", "state", (intptr_t)(&((idAFBody *)0)->state), sizeof( ((idAFBody *)0)->state ) },
	{ "AFBodyPState_t *", "current", (intptr_t)(&((idAFBody *)0)->current), sizeof( ((idAFBody *)0)->current ) },
	{ "AFBodyPState_t *", "next", (intptr_t)(&((idAFBody *)0)->next), sizeof( ((idAFBody *)0)->next ) },
	{ "AFBodyPState_t", "saved", (intptr_t)(&((idAFBody *)0)->saved), sizeof( ((idAFBody *)0)->saved ) },
	{ "idVec3", "atRestOrigin", (intptr_t)(&((idAFBody *)0)->atRestOrigin), sizeof( ((idAFBody *)0)->atRestOrigin ) },
	{ "idMat3", "atRestAxis", (intptr_t)(&((idAFBody *)0)->atRestAxis), sizeof( ((idAFBody *)0)->atRestAxis ) },
	{ "idMatX", "inverseWorldSpatialInertia", (intptr_t)(&((idAFBody *)0)->inverseWorldSpatialInertia), sizeof( ((idAFBody *)0)->inverseWorldSpatialInertia ) },
	{ "idMatX", "I", (intptr_t)(&((idAFBody *)0)->I), sizeof( ((idAFBody *)0)->I ) },
	{ "idMatX", "invI", (intptr_t)(&((idAFBody *)0)->invI), sizeof( ((idAFBody *)0)->invI ) },
	{ "idMatX", "J", (intptr_t)(&((idAFBody *)0)->J), sizeof( ((idAFBody *)0)->J ) },
	{ "idVecX", "s", (intptr_t)(&((idAFBody *)0)->s), sizeof( ((idAFBody *)0)->s ) },
	{ "idVecX", "totalForce", (intptr_t)(&((idAFBody *)0)->totalForce), sizeof( ((idAFBody *)0)->totalForce ) },
	{ "idVecX", "auxForce", (intptr_t)(&((idAFBody *)0)->auxForce), sizeof( ((idAFBody *)0)->auxForce ) },
	{ "idVecX", "acceleration", (intptr_t)(&((idAFBody *)0)->acceleration), sizeof( ((idAFBody *)0)->acceleration ) },
	{ "float *", "response", (intptr_t)(&((idAFBody *)0)->response), sizeof( ((idAFBody *)0)->response ) },
	{ "int *", "responseIndex", (intptr_t)(&((idAFBody *)0)->responseIndex), sizeof( ((idAFBody *)0)->responseIndex ) },
	{ "int", "numResponses", (intptr_t)(&((idAFBody *)0)->numResponses), sizeof( ((idAFBody *)0)->numResponses ) },
	{ "int", "maxAuxiliaryIndex", (intptr_t)(&((idAFBody *)0)->maxAuxiliaryIndex), sizeof( ((idAFBody *)0)->maxAuxiliaryIndex ) },
	{ "int", "maxSubTreeAuxiliaryIndex", (intptr_t)(&((idAFBody *)0)->maxSubTreeAuxiliaryIndex), sizeof( ((idAFBody *)0)->maxSubTreeAuxiliaryIndex ) },
	{ "idAFBody::bodyFlags_s", "fl", (intptr_t)(&((idAFBody *)0)->fl), sizeof( ((idAFBody *)0)->fl ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFTree_typeInfo[] = {
	{ ": idList < idAFBody * , TAG_IDLIB_LIST_PHYSICS >", "sortedBodies", (intptr_t)(&((idAFTree *)0)->sortedBodies), sizeof( ((idAFTree *)0)->sortedBodies ) },
	{ NULL, 0 }
};

static classVariableInfo_t AFPState_t_typeInfo[] = {
	{ "int", "atRest", (intptr_t)(&((AFPState_t *)0)->atRest), sizeof( ((AFPState_t *)0)->atRest ) },
	{ "float", "noMoveTime", (intptr_t)(&((AFPState_t *)0)->noMoveTime), sizeof( ((AFPState_t *)0)->noMoveTime ) },
	{ "float", "activateTime", (intptr_t)(&((AFPState_t *)0)->activateTime), sizeof( ((AFPState_t *)0)->activateTime ) },
	{ "float", "lastTimeStep", (intptr_t)(&((AFPState_t *)0)->lastTimeStep), sizeof( ((AFPState_t *)0)->lastTimeStep ) },
	{ "idVec6", "pushVelocity", (intptr_t)(&((AFPState_t *)0)->pushVelocity), sizeof( ((AFPState_t *)0)->pushVelocity ) },
	{ NULL, 0 }
};

static classVariableInfo_t AFCollision_t_typeInfo[] = {
	{ "trace_t", "trace", (intptr_t)(&((AFCollision_t *)0)->trace), sizeof( ((AFCollision_t *)0)->trace ) },
	{ "idAFBody *", "body", (intptr_t)(&((AFCollision_t *)0)->body), sizeof( ((AFCollision_t *)0)->body ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhysics_AF_typeInfo[] = {
	{ ": idList < idAFTree * , TAG_IDLIB_LIST_PHYSICS >", "trees", (intptr_t)(&((idPhysics_AF *)0)->trees), sizeof( ((idPhysics_AF *)0)->trees ) },
	{ "idList < idAFBody * , TAG_IDLIB_LIST_PHYSICS >", "bodies", (intptr_t)(&((idPhysics_AF *)0)->bodies), sizeof( ((idPhysics_AF *)0)->bodies ) },
	{ "idList < idAFConstraint * , TAG_IDLIB_LIST_PHYSICS >", "constraints", (intptr_t)(&((idPhysics_AF *)0)->constraints), sizeof( ((idPhysics_AF *)0)->constraints ) },
	{ "idList < idAFConstraint * , TAG_IDLIB_LIST_PHYSICS >", "primaryConstraints", (intptr_t)(&((idPhysics_AF *)0)->primaryConstraints), sizeof( ((idPhysics_AF *)0)->primaryConstraints ) },
	{ "idList < idAFConstraint * , TAG_IDLIB_LIST_PHYSICS >", "auxiliaryConstraints", (intptr_t)(&((idPhysics_AF *)0)->auxiliaryConstraints), sizeof( ((idPhysics_AF *)0)->auxiliaryConstraints ) },
	{ "idList < idAFConstraint * , TAG_IDLIB_LIST_PHYSICS >", "frameConstraints", (intptr_t)(&((idPhysics_AF *)0)->frameConstraints), sizeof( ((idPhysics_AF *)0)->frameConstraints ) },
	{ "idList < idAFConstraint_Contact * , TAG_IDLIB_LIST_PHYSICS >", "contactConstraints", (intptr_t)(&((idPhysics_AF *)0)->contactConstraints), sizeof( ((idPhysics_AF *)0)->contactConstraints ) },
	{ "idList < int , TAG_IDLIB_LIST_PHYSICS >", "contactBodies", (intptr_t)(&((idPhysics_AF *)0)->contactBodies), sizeof( ((idPhysics_AF *)0)->contactBodies ) },
	{ "idList < AFCollision_t , TAG_IDLIB_LIST_PHYSICS >", "collisions", (intptr_t)(&((idPhysics_AF *)0)->collisions), sizeof( ((idPhysics_AF *)0)->collisions ) },
	{ "bool", "changedAF", (intptr_t)(&((idPhysics_AF *)0)->changedAF), sizeof( ((idPhysics_AF *)0)->changedAF ) },
	{ "float", "linearFriction", (intptr_t)(&((idPhysics_AF *)0)->linearFriction), sizeof( ((idPhysics_AF *)0)->linearFriction ) },
	{ "float", "angularFriction", (intptr_t)(&((idPhysics_AF *)0)->angularFriction), sizeof( ((idPhysics_AF *)0)->angularFriction ) },
	{ "float", "contactFriction", (intptr_t)(&((idPhysics_AF *)0)->contactFriction), sizeof( ((idPhysics_AF *)0)->contactFriction ) },
	{ "float", "bouncyness", (intptr_t)(&((idPhysics_AF *)0)->bouncyness), sizeof( ((idPhysics_AF *)0)->bouncyness ) },
	{ "float", "totalMass", (intptr_t)(&((idPhysics_AF *)0)->totalMass), sizeof( ((idPhysics_AF *)0)->totalMass ) },
	{ "float", "forceTotalMass", (intptr_t)(&((idPhysics_AF *)0)->forceTotalMass), sizeof( ((idPhysics_AF *)0)->forceTotalMass ) },
	{ "idVec2", "suspendVelocity", (intptr_t)(&((idPhysics_AF *)0)->suspendVelocity), sizeof( ((idPhysics_AF *)0)->suspendVelocity ) },
	{ "idVec2", "suspendAcceleration", (intptr_t)(&((idPhysics_AF *)0)->suspendAcceleration), sizeof( ((idPhysics_AF *)0)->suspendAcceleration ) },
	{ "float", "noMoveTime", (intptr_t)(&((idPhysics_AF *)0)->noMoveTime), sizeof( ((idPhysics_AF *)0)->noMoveTime ) },
	{ "float", "noMoveTranslation", (intptr_t)(&((idPhysics_AF *)0)->noMoveTranslation), sizeof( ((idPhysics_AF *)0)->noMoveTranslation ) },
	{ "float", "noMoveRotation", (intptr_t)(&((idPhysics_AF *)0)->noMoveRotation), sizeof( ((idPhysics_AF *)0)->noMoveRotation ) },
	{ "float", "minMoveTime", (intptr_t)(&((idPhysics_AF *)0)->minMoveTime), sizeof( ((idPhysics_AF *)0)->minMoveTime ) },
	{ "float", "maxMoveTime", (intptr_t)(&((idPhysics_AF *)0)->maxMoveTime), sizeof( ((idPhysics_AF *)0)->maxMoveTime ) },
	{ "float", "impulseThreshold", (intptr_t)(&((idPhysics_AF *)0)->impulseThreshold), sizeof( ((idPhysics_AF *)0)->impulseThreshold ) },
	{ "float", "timeScale", (intptr_t)(&((idPhysics_AF *)0)->timeScale), sizeof( ((idPhysics_AF *)0)->timeScale ) },
	{ "float", "timeScaleRampStart", (intptr_t)(&((idPhysics_AF *)0)->timeScaleRampStart), sizeof( ((idPhysics_AF *)0)->timeScaleRampStart ) },
	{ "float", "timeScaleRampEnd", (intptr_t)(&((idPhysics_AF *)0)->timeScaleRampEnd), sizeof( ((idPhysics_AF *)0)->timeScaleRampEnd ) },
	{ "float", "jointFrictionScale", (intptr_t)(&((idPhysics_AF *)0)->jointFrictionScale), sizeof( ((idPhysics_AF *)0)->jointFrictionScale ) },
	{ "float", "jointFrictionDent", (intptr_t)(&((idPhysics_AF *)0)->jointFrictionDent), sizeof( ((idPhysics_AF *)0)->jointFrictionDent ) },
	{ "float", "jointFrictionDentStart", (intptr_t)(&((idPhysics_AF *)0)->jointFrictionDentStart), sizeof( ((idPhysics_AF *)0)->jointFrictionDentStart ) },
	{ "float", "jointFrictionDentEnd", (intptr_t)(&((idPhysics_AF *)0)->jointFrictionDentEnd), sizeof( ((idPhysics_AF *)0)->jointFrictionDentEnd ) },
	{ "float", "jointFrictionDentScale", (intptr_t)(&((idPhysics_AF *)0)->jointFrictionDentScale), sizeof( ((idPhysics_AF *)0)->jointFrictionDentScale ) },
	{ "float", "contactFrictionScale", (intptr_t)(&((idPhysics_AF *)0)->contactFrictionScale), sizeof( ((idPhysics_AF *)0)->contactFrictionScale ) },
	{ "float", "contactFrictionDent", (intptr_t)(&((idPhysics_AF *)0)->contactFrictionDent), sizeof( ((idPhysics_AF *)0)->contactFrictionDent ) },
	{ "float", "contactFrictionDentStart", (intptr_t)(&((idPhysics_AF *)0)->contactFrictionDentStart), sizeof( ((idPhysics_AF *)0)->contactFrictionDentStart ) },
	{ "float", "contactFrictionDentEnd", (intptr_t)(&((idPhysics_AF *)0)->contactFrictionDentEnd), sizeof( ((idPhysics_AF *)0)->contactFrictionDentEnd ) },
	{ "float", "contactFrictionDentScale", (intptr_t)(&((idPhysics_AF *)0)->contactFrictionDentScale), sizeof( ((idPhysics_AF *)0)->contactFrictionDentScale ) },
	{ "bool", "enableCollision", (intptr_t)(&((idPhysics_AF *)0)->enableCollision), sizeof( ((idPhysics_AF *)0)->enableCollision ) },
	{ "bool", "selfCollision", (intptr_t)(&((idPhysics_AF *)0)->selfCollision), sizeof( ((idPhysics_AF *)0)->selfCollision ) },
	{ "bool", "comeToRest", (intptr_t)(&((idPhysics_AF *)0)->comeToRest), sizeof( ((idPhysics_AF *)0)->comeToRest ) },
	{ "bool", "linearTime", (intptr_t)(&((idPhysics_AF *)0)->linearTime), sizeof( ((idPhysics_AF *)0)->linearTime ) },
	{ "bool", "noImpact", (intptr_t)(&((idPhysics_AF *)0)->noImpact), sizeof( ((idPhysics_AF *)0)->noImpact ) },
	{ "bool", "worldConstraintsLocked", (intptr_t)(&((idPhysics_AF *)0)->worldConstraintsLocked), sizeof( ((idPhysics_AF *)0)->worldConstraintsLocked ) },
	{ "bool", "forcePushable", (intptr_t)(&((idPhysics_AF *)0)->forcePushable), sizeof( ((idPhysics_AF *)0)->forcePushable ) },
	{ "AFPState_t", "current", (intptr_t)(&((idPhysics_AF *)0)->current), sizeof( ((idPhysics_AF *)0)->current ) },
	{ "AFPState_t", "saved", (intptr_t)(&((idPhysics_AF *)0)->saved), sizeof( ((idPhysics_AF *)0)->saved ) },
	{ "idAFBody *", "masterBody", (intptr_t)(&((idPhysics_AF *)0)->masterBody), sizeof( ((idPhysics_AF *)0)->masterBody ) },
	{ "idLCP *", "lcp", (intptr_t)(&((idPhysics_AF *)0)->lcp), sizeof( ((idPhysics_AF *)0)->lcp ) },
	{ NULL, 0 }
};

static classVariableInfo_t singleSmoke_t_typeInfo[] = {
	{ "singleSmoke_s *", "next", (intptr_t)(&((singleSmoke_t *)0)->next), sizeof( ((singleSmoke_t *)0)->next ) },
	{ "int", "privateStartTime", (intptr_t)(&((singleSmoke_t *)0)->privateStartTime), sizeof( ((singleSmoke_t *)0)->privateStartTime ) },
	{ "int", "index", (intptr_t)(&((singleSmoke_t *)0)->index), sizeof( ((singleSmoke_t *)0)->index ) },
	{ "idRandom", "random", (intptr_t)(&((singleSmoke_t *)0)->random), sizeof( ((singleSmoke_t *)0)->random ) },
	{ "idVec3", "origin", (intptr_t)(&((singleSmoke_t *)0)->origin), sizeof( ((singleSmoke_t *)0)->origin ) },
	{ "idMat3", "axis", (intptr_t)(&((singleSmoke_t *)0)->axis), sizeof( ((singleSmoke_t *)0)->axis ) },
	{ "int", "timeGroup", (intptr_t)(&((singleSmoke_t *)0)->timeGroup), sizeof( ((singleSmoke_t *)0)->timeGroup ) },
	{ NULL, 0 }
};

static classVariableInfo_t activeSmokeStage_t_typeInfo[] = {
	{ "const idParticleStage *", "stage", (intptr_t)(&((activeSmokeStage_t *)0)->stage), sizeof( ((activeSmokeStage_t *)0)->stage ) },
	{ "singleSmoke_t *", "smokes", (intptr_t)(&((activeSmokeStage_t *)0)->smokes), sizeof( ((activeSmokeStage_t *)0)->smokes ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSmokeParticles_typeInfo[] = {
	{ ": bool", "initialized", (intptr_t)(&((idSmokeParticles *)0)->initialized), sizeof( ((idSmokeParticles *)0)->initialized ) },
	{ "renderEntity_t", "renderEntity", (intptr_t)(&((idSmokeParticles *)0)->renderEntity), sizeof( ((idSmokeParticles *)0)->renderEntity ) },
	{ "int", "renderEntityHandle", (intptr_t)(&((idSmokeParticles *)0)->renderEntityHandle), sizeof( ((idSmokeParticles *)0)->renderEntityHandle ) },
	{ "singleSmoke_t[10000]", "smokes", (intptr_t)(&((idSmokeParticles *)0)->smokes), sizeof( ((idSmokeParticles *)0)->smokes ) },
	{ "idList < activeSmokeStage_t , TAG_PARTICLE >", "activeStages", (intptr_t)(&((idSmokeParticles *)0)->activeStages), sizeof( ((idSmokeParticles *)0)->activeStages ) },
	{ "singleSmoke_t *", "freeSmokes", (intptr_t)(&((idSmokeParticles *)0)->freeSmokes), sizeof( ((idSmokeParticles *)0)->freeSmokes ) },
	{ "int", "numActiveSmokes", (intptr_t)(&((idSmokeParticles *)0)->numActiveSmokes), sizeof( ((idSmokeParticles *)0)->numActiveSmokes ) },
	{ "int", "currentParticleTime", (intptr_t)(&((idSmokeParticles *)0)->currentParticleTime), sizeof( ((idSmokeParticles *)0)->currentParticleTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t signal_t_typeInfo[] = {
	{ "int", "threadnum", (intptr_t)(&((signal_t *)0)->threadnum), sizeof( ((signal_t *)0)->threadnum ) },
	{ "const function_t *", "function", (intptr_t)(&((signal_t *)0)->function), sizeof( ((signal_t *)0)->function ) },
	{ NULL, 0 }
};

static classVariableInfo_t signalList_t_typeInfo[] = {
	{ ": idList < signal_t , TAG_ENTITY >[10]", "signal", (intptr_t)(&((signalList_t *)0)->signal), sizeof( ((signalList_t *)0)->signal ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEntity_entityFlags_s_typeInfo[] = {
//	{ "bool", "notarget", (intptr_t)(&((idEntity::entityFlags_s *)0)->notarget), sizeof( ((idEntity::entityFlags_s *)0)->notarget ) },
//	{ "bool", "noknockback", (intptr_t)(&((idEntity::entityFlags_s *)0)->noknockback), sizeof( ((idEntity::entityFlags_s *)0)->noknockback ) },
//	{ "bool", "takedamage", (intptr_t)(&((idEntity::entityFlags_s *)0)->takedamage), sizeof( ((idEntity::entityFlags_s *)0)->takedamage ) },
//	{ "bool", "hidden", (intptr_t)(&((idEntity::entityFlags_s *)0)->hidden), sizeof( ((idEntity::entityFlags_s *)0)->hidden ) },
//	{ "bool", "bindOrientated", (intptr_t)(&((idEntity::entityFlags_s *)0)->bindOrientated), sizeof( ((idEntity::entityFlags_s *)0)->bindOrientated ) },
//	{ "bool", "solidForTeam", (intptr_t)(&((idEntity::entityFlags_s *)0)->solidForTeam), sizeof( ((idEntity::entityFlags_s *)0)->solidForTeam ) },
//	{ "bool", "forcePhysicsUpdate", (intptr_t)(&((idEntity::entityFlags_s *)0)->forcePhysicsUpdate), sizeof( ((idEntity::entityFlags_s *)0)->forcePhysicsUpdate ) },
//	{ "bool", "selected", (intptr_t)(&((idEntity::entityFlags_s *)0)->selected), sizeof( ((idEntity::entityFlags_s *)0)->selected ) },
//	{ "bool", "neverDormant", (intptr_t)(&((idEntity::entityFlags_s *)0)->neverDormant), sizeof( ((idEntity::entityFlags_s *)0)->neverDormant ) },
//	{ "bool", "isDormant", (intptr_t)(&((idEntity::entityFlags_s *)0)->isDormant), sizeof( ((idEntity::entityFlags_s *)0)->isDormant ) },
//	{ "bool", "hasAwakened", (intptr_t)(&((idEntity::entityFlags_s *)0)->hasAwakened), sizeof( ((idEntity::entityFlags_s *)0)->hasAwakened ) },
//	{ "bool", "networkSync", (intptr_t)(&((idEntity::entityFlags_s *)0)->networkSync), sizeof( ((idEntity::entityFlags_s *)0)->networkSync ) },
//	{ "bool", "grabbed", (intptr_t)(&((idEntity::entityFlags_s *)0)->grabbed), sizeof( ((idEntity::entityFlags_s *)0)->grabbed ) },
//	{ "bool", "skipReplication", (intptr_t)(&((idEntity::entityFlags_s *)0)->skipReplication), sizeof( ((idEntity::entityFlags_s *)0)->skipReplication ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEntity_typeInfo[] = {
	{ "int", "entityNumber", (intptr_t)(&((idEntity *)0)->entityNumber), sizeof( ((idEntity *)0)->entityNumber ) },
	{ "int", "entityDefNumber", (intptr_t)(&((idEntity *)0)->entityDefNumber), sizeof( ((idEntity *)0)->entityDefNumber ) },
	{ "idLinkList < idEntity >", "spawnNode", (intptr_t)(&((idEntity *)0)->spawnNode), sizeof( ((idEntity *)0)->spawnNode ) },
	{ "idLinkList < idEntity >", "activeNode", (intptr_t)(&((idEntity *)0)->activeNode), sizeof( ((idEntity *)0)->activeNode ) },
	{ "idLinkList < idEntity >", "aimAssistNode", (intptr_t)(&((idEntity *)0)->aimAssistNode), sizeof( ((idEntity *)0)->aimAssistNode ) },
	{ "idLinkList < idEntity >", "snapshotNode", (intptr_t)(&((idEntity *)0)->snapshotNode), sizeof( ((idEntity *)0)->snapshotNode ) },
	{ "int", "snapshotChanged", (intptr_t)(&((idEntity *)0)->snapshotChanged), sizeof( ((idEntity *)0)->snapshotChanged ) },
	{ "int", "snapshotBits", (intptr_t)(&((idEntity *)0)->snapshotBits), sizeof( ((idEntity *)0)->snapshotBits ) },
	{ "bool", "snapshotStale", (intptr_t)(&((idEntity *)0)->snapshotStale), sizeof( ((idEntity *)0)->snapshotStale ) },
	{ "idStr", "name", (intptr_t)(&((idEntity *)0)->name), sizeof( ((idEntity *)0)->name ) },
	{ "idDict", "spawnArgs", (intptr_t)(&((idEntity *)0)->spawnArgs), sizeof( ((idEntity *)0)->spawnArgs ) },
	{ "idScriptObject", "scriptObject", (intptr_t)(&((idEntity *)0)->scriptObject), sizeof( ((idEntity *)0)->scriptObject ) },
	{ "int", "thinkFlags", (intptr_t)(&((idEntity *)0)->thinkFlags), sizeof( ((idEntity *)0)->thinkFlags ) },
	{ "int", "dormantStart", (intptr_t)(&((idEntity *)0)->dormantStart), sizeof( ((idEntity *)0)->dormantStart ) },
	{ "bool", "cinematic", (intptr_t)(&((idEntity *)0)->cinematic), sizeof( ((idEntity *)0)->cinematic ) },
	{ "renderView_t *", "renderView", (intptr_t)(&((idEntity *)0)->renderView), sizeof( ((idEntity *)0)->renderView ) },
	{ "idEntity *", "cameraTarget", (intptr_t)(&((idEntity *)0)->cameraTarget), sizeof( ((idEntity *)0)->cameraTarget ) },
	{ "idList < idEntityPtr < idEntity > , TAG_ENTITY >", "targets", (intptr_t)(&((idEntity *)0)->targets), sizeof( ((idEntity *)0)->targets ) },
	{ "int", "health", (intptr_t)(&((idEntity *)0)->health), sizeof( ((idEntity *)0)->health ) },
	{ "idEntity::entityFlags_s", "fl", (intptr_t)(&((idEntity *)0)->fl), sizeof( ((idEntity *)0)->fl ) },
	{ "int", "timeGroup", (intptr_t)(&((idEntity *)0)->timeGroup), sizeof( ((idEntity *)0)->timeGroup ) },
	{ "bool", "noGrab", (intptr_t)(&((idEntity *)0)->noGrab), sizeof( ((idEntity *)0)->noGrab ) },
	{ "renderEntity_t", "xrayEntity", (intptr_t)(&((idEntity *)0)->xrayEntity), sizeof( ((idEntity *)0)->xrayEntity ) },
	{ "qhandle_t", "xrayEntityHandle", (intptr_t)(&((idEntity *)0)->xrayEntityHandle), sizeof( ((idEntity *)0)->xrayEntityHandle ) },
	{ "const idDeclSkin *", "xraySkin", (intptr_t)(&((idEntity *)0)->xraySkin), sizeof( ((idEntity *)0)->xraySkin ) },
	{ ": renderEntity_t", "renderEntity", (intptr_t)(&((idEntity *)0)->renderEntity), sizeof( ((idEntity *)0)->renderEntity ) },
	{ "int", "modelDefHandle", (intptr_t)(&((idEntity *)0)->modelDefHandle), sizeof( ((idEntity *)0)->modelDefHandle ) },
	{ "refSound_t", "refSound", (intptr_t)(&((idEntity *)0)->refSound), sizeof( ((idEntity *)0)->refSound ) },
	{ ": idPhysics_Static", "defaultPhysicsObj", (intptr_t)(&((idEntity *)0)->defaultPhysicsObj), sizeof( ((idEntity *)0)->defaultPhysicsObj ) },
	{ "idPhysics *", "physics", (intptr_t)(&((idEntity *)0)->physics), sizeof( ((idEntity *)0)->physics ) },
	{ "idEntity *", "bindMaster", (intptr_t)(&((idEntity *)0)->bindMaster), sizeof( ((idEntity *)0)->bindMaster ) },
	{ "jointHandle_t", "bindJoint", (intptr_t)(&((idEntity *)0)->bindJoint), sizeof( ((idEntity *)0)->bindJoint ) },
	{ "int", "bindBody", (intptr_t)(&((idEntity *)0)->bindBody), sizeof( ((idEntity *)0)->bindBody ) },
	{ "idEntity *", "teamMaster", (intptr_t)(&((idEntity *)0)->teamMaster), sizeof( ((idEntity *)0)->teamMaster ) },
	{ "idEntity *", "teamChain", (intptr_t)(&((idEntity *)0)->teamChain), sizeof( ((idEntity *)0)->teamChain ) },
	{ "bool", "useClientInterpolation", (intptr_t)(&((idEntity *)0)->useClientInterpolation), sizeof( ((idEntity *)0)->useClientInterpolation ) },
	{ "int", "numPVSAreas", (intptr_t)(&((idEntity *)0)->numPVSAreas), sizeof( ((idEntity *)0)->numPVSAreas ) },
	{ "int[4]", "PVSAreas", (intptr_t)(&((idEntity *)0)->PVSAreas), sizeof( ((idEntity *)0)->PVSAreas ) },
	{ "signalList_t *", "signals", (intptr_t)(&((idEntity *)0)->signals), sizeof( ((idEntity *)0)->signals ) },
	{ "int", "mpGUIState", (intptr_t)(&((idEntity *)0)->mpGUIState), sizeof( ((idEntity *)0)->mpGUIState ) },
	{ "uint32", "predictionKey", (intptr_t)(&((idEntity *)0)->predictionKey), sizeof( ((idEntity *)0)->predictionKey ) },
	{ "idVec3", "originDelta", (intptr_t)(&((idEntity *)0)->originDelta), sizeof( ((idEntity *)0)->originDelta ) },
	{ "idMat3", "axisDelta", (intptr_t)(&((idEntity *)0)->axisDelta), sizeof( ((idEntity *)0)->axisDelta ) },
	{ "interpolationBehavior_t", "interpolationBehavior", (intptr_t)(&((idEntity *)0)->interpolationBehavior), sizeof( ((idEntity *)0)->interpolationBehavior ) },
	{ "unsigned int", "snapshotsReceived", (intptr_t)(&((idEntity *)0)->snapshotsReceived), sizeof( ((idEntity *)0)->snapshotsReceived ) },
	{ NULL, 0 }
};

static classVariableInfo_t damageEffect_t_typeInfo[] = {
	{ "jointHandle_t", "jointNum", (intptr_t)(&((damageEffect_t *)0)->jointNum), sizeof( ((damageEffect_t *)0)->jointNum ) },
	{ "idVec3", "localOrigin", (intptr_t)(&((damageEffect_t *)0)->localOrigin), sizeof( ((damageEffect_t *)0)->localOrigin ) },
	{ "idVec3", "localNormal", (intptr_t)(&((damageEffect_t *)0)->localNormal), sizeof( ((damageEffect_t *)0)->localNormal ) },
	{ "int", "time", (intptr_t)(&((damageEffect_t *)0)->time), sizeof( ((damageEffect_t *)0)->time ) },
	{ "const idDeclParticle *", "type", (intptr_t)(&((damageEffect_t *)0)->type), sizeof( ((damageEffect_t *)0)->type ) },
	{ "damageEffect_s *", "next", (intptr_t)(&((damageEffect_t *)0)->next), sizeof( ((damageEffect_t *)0)->next ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAnimatedEntity_typeInfo[] = {
	{ ": idAnimator", "animator", (intptr_t)(&((idAnimatedEntity *)0)->animator), sizeof( ((idAnimatedEntity *)0)->animator ) },
	{ "damageEffect_t *", "damageEffects", (intptr_t)(&((idAnimatedEntity *)0)->damageEffects), sizeof( ((idAnimatedEntity *)0)->damageEffects ) },
	{ NULL, 0 }
};

static classVariableInfo_t SetTimeState_typeInfo[] = {
	{ ": bool", "activated", (intptr_t)(&((SetTimeState *)0)->activated), sizeof( ((SetTimeState *)0)->activated ) },
	{ "bool", "previousFast", (intptr_t)(&((SetTimeState *)0)->previousFast), sizeof( ((SetTimeState *)0)->previousFast ) },
	{ "bool", "fast", (intptr_t)(&((SetTimeState *)0)->fast), sizeof( ((SetTimeState *)0)->fast ) },
	{ NULL, 0 }
};

static classVariableInfo_t idCursor3D_typeInfo[] = {
	{ "idForce_Drag", "drag", (intptr_t)(&((idCursor3D *)0)->drag), sizeof( ((idCursor3D *)0)->drag ) },
	{ "idVec3", "draggedPosition", (intptr_t)(&((idCursor3D *)0)->draggedPosition), sizeof( ((idCursor3D *)0)->draggedPosition ) },
	{ NULL, 0 }
};

static classVariableInfo_t idDragEntity_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "dragEnt", (intptr_t)(&((idDragEntity *)0)->dragEnt), sizeof( ((idDragEntity *)0)->dragEnt ) },
	{ "jointHandle_t", "joint", (intptr_t)(&((idDragEntity *)0)->joint), sizeof( ((idDragEntity *)0)->joint ) },
	{ "int", "id", (intptr_t)(&((idDragEntity *)0)->id), sizeof( ((idDragEntity *)0)->id ) },
	{ "idVec3", "localEntityPoint", (intptr_t)(&((idDragEntity *)0)->localEntityPoint), sizeof( ((idDragEntity *)0)->localEntityPoint ) },
	{ "idVec3", "localPlayerPoint", (intptr_t)(&((idDragEntity *)0)->localPlayerPoint), sizeof( ((idDragEntity *)0)->localPlayerPoint ) },
	{ "idStr", "bodyName", (intptr_t)(&((idDragEntity *)0)->bodyName), sizeof( ((idDragEntity *)0)->bodyName ) },
	{ "idCursor3D *", "cursor", (intptr_t)(&((idDragEntity *)0)->cursor), sizeof( ((idDragEntity *)0)->cursor ) },
	{ "idEntityPtr < idEntity >", "selected", (intptr_t)(&((idDragEntity *)0)->selected), sizeof( ((idDragEntity *)0)->selected ) },
	{ NULL, 0 }
};

static classVariableInfo_t selectedTypeInfo_t_typeInfo[] = {
	{ "idTypeInfo *", "typeInfo", (intptr_t)(&((selectedTypeInfo_t *)0)->typeInfo), sizeof( ((selectedTypeInfo_t *)0)->typeInfo ) },
	{ "idStr", "textKey", (intptr_t)(&((selectedTypeInfo_t *)0)->textKey), sizeof( ((selectedTypeInfo_t *)0)->textKey ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEditEntities_typeInfo[] = {
	{ ": int", "nextSelectTime", (intptr_t)(&((idEditEntities *)0)->nextSelectTime), sizeof( ((idEditEntities *)0)->nextSelectTime ) },
	{ "idList < selectedTypeInfo_t >", "selectableEntityClasses", (intptr_t)(&((idEditEntities *)0)->selectableEntityClasses), sizeof( ((idEditEntities *)0)->selectableEntityClasses ) },
	{ "idList < idEntity * >", "selectedEntities", (intptr_t)(&((idEditEntities *)0)->selectedEntities), sizeof( ((idEditEntities *)0)->selectedEntities ) },
	{ NULL, 0 }
};

static classVariableInfo_t idGrabber_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "dragEnt", (intptr_t)(&((idGrabber *)0)->dragEnt), sizeof( ((idGrabber *)0)->dragEnt ) },
	{ "idForce_Grab", "drag", (intptr_t)(&((idGrabber *)0)->drag), sizeof( ((idGrabber *)0)->drag ) },
	{ "idVec3", "saveGravity", (intptr_t)(&((idGrabber *)0)->saveGravity), sizeof( ((idGrabber *)0)->saveGravity ) },
	{ "int", "id", (intptr_t)(&((idGrabber *)0)->id), sizeof( ((idGrabber *)0)->id ) },
	{ "idVec3", "localPlayerPoint", (intptr_t)(&((idGrabber *)0)->localPlayerPoint), sizeof( ((idGrabber *)0)->localPlayerPoint ) },
	{ "idEntityPtr < idPlayer >", "owner", (intptr_t)(&((idGrabber *)0)->owner), sizeof( ((idGrabber *)0)->owner ) },
	{ "int", "oldImpulseSequence", (intptr_t)(&((idGrabber *)0)->oldImpulseSequence), sizeof( ((idGrabber *)0)->oldImpulseSequence ) },
	{ "bool", "holdingAF", (intptr_t)(&((idGrabber *)0)->holdingAF), sizeof( ((idGrabber *)0)->holdingAF ) },
	{ "bool", "shakeForceFlip", (intptr_t)(&((idGrabber *)0)->shakeForceFlip), sizeof( ((idGrabber *)0)->shakeForceFlip ) },
	{ "int", "endTime", (intptr_t)(&((idGrabber *)0)->endTime), sizeof( ((idGrabber *)0)->endTime ) },
	{ "int", "lastFiredTime", (intptr_t)(&((idGrabber *)0)->lastFiredTime), sizeof( ((idGrabber *)0)->lastFiredTime ) },
	{ "int", "dragFailTime", (intptr_t)(&((idGrabber *)0)->dragFailTime), sizeof( ((idGrabber *)0)->dragFailTime ) },
	{ "int", "startDragTime", (intptr_t)(&((idGrabber *)0)->startDragTime), sizeof( ((idGrabber *)0)->startDragTime ) },
	{ "float", "dragTraceDist", (intptr_t)(&((idGrabber *)0)->dragTraceDist), sizeof( ((idGrabber *)0)->dragTraceDist ) },
	{ "int", "savedContents", (intptr_t)(&((idGrabber *)0)->savedContents), sizeof( ((idGrabber *)0)->savedContents ) },
	{ "int", "savedClipmask", (intptr_t)(&((idGrabber *)0)->savedClipmask), sizeof( ((idGrabber *)0)->savedClipmask ) },
	{ "idBeam *", "beam", (intptr_t)(&((idGrabber *)0)->beam), sizeof( ((idGrabber *)0)->beam ) },
	{ "idBeam *", "beamTarget", (intptr_t)(&((idGrabber *)0)->beamTarget), sizeof( ((idGrabber *)0)->beamTarget ) },
	{ "int", "warpId", (intptr_t)(&((idGrabber *)0)->warpId), sizeof( ((idGrabber *)0)->warpId ) },
	{ NULL, 0 }
};

static classVariableInfo_t jointConversion_t_typeInfo[] = {
	{ "int", "bodyId", (intptr_t)(&((jointConversion_t *)0)->bodyId), sizeof( ((jointConversion_t *)0)->bodyId ) },
	{ "jointHandle_t", "jointHandle", (intptr_t)(&((jointConversion_t *)0)->jointHandle), sizeof( ((jointConversion_t *)0)->jointHandle ) },
	{ "AFJointModType_t", "jointMod", (intptr_t)(&((jointConversion_t *)0)->jointMod), sizeof( ((jointConversion_t *)0)->jointMod ) },
	{ "idVec3", "jointBodyOrigin", (intptr_t)(&((jointConversion_t *)0)->jointBodyOrigin), sizeof( ((jointConversion_t *)0)->jointBodyOrigin ) },
	{ "idMat3", "jointBodyAxis", (intptr_t)(&((jointConversion_t *)0)->jointBodyAxis), sizeof( ((jointConversion_t *)0)->jointBodyAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t afTouch_t_typeInfo[] = {
	{ "idEntity *", "touchedEnt", (intptr_t)(&((afTouch_t *)0)->touchedEnt), sizeof( ((afTouch_t *)0)->touchedEnt ) },
	{ "idClipModel *", "touchedClipModel", (intptr_t)(&((afTouch_t *)0)->touchedClipModel), sizeof( ((afTouch_t *)0)->touchedClipModel ) },
	{ "idAFBody *", "touchedByBody", (intptr_t)(&((afTouch_t *)0)->touchedByBody), sizeof( ((afTouch_t *)0)->touchedByBody ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAF_typeInfo[] = {
	{ ": idStr", "name", (intptr_t)(&((idAF *)0)->name), sizeof( ((idAF *)0)->name ) },
	{ "idPhysics_AF", "physicsObj", (intptr_t)(&((idAF *)0)->physicsObj), sizeof( ((idAF *)0)->physicsObj ) },
	{ "idEntity *", "self", (intptr_t)(&((idAF *)0)->self), sizeof( ((idAF *)0)->self ) },
	{ "idAnimator *", "animator", (intptr_t)(&((idAF *)0)->animator), sizeof( ((idAF *)0)->animator ) },
	{ "int", "modifiedAnim", (intptr_t)(&((idAF *)0)->modifiedAnim), sizeof( ((idAF *)0)->modifiedAnim ) },
	{ "idVec3", "baseOrigin", (intptr_t)(&((idAF *)0)->baseOrigin), sizeof( ((idAF *)0)->baseOrigin ) },
	{ "idMat3", "baseAxis", (intptr_t)(&((idAF *)0)->baseAxis), sizeof( ((idAF *)0)->baseAxis ) },
	{ "idList < jointConversion_t , TAG_AF >", "jointMods", (intptr_t)(&((idAF *)0)->jointMods), sizeof( ((idAF *)0)->jointMods ) },
	{ "idList < int , TAG_AF >", "jointBody", (intptr_t)(&((idAF *)0)->jointBody), sizeof( ((idAF *)0)->jointBody ) },
	{ "int", "poseTime", (intptr_t)(&((idAF *)0)->poseTime), sizeof( ((idAF *)0)->poseTime ) },
	{ "int", "restStartTime", (intptr_t)(&((idAF *)0)->restStartTime), sizeof( ((idAF *)0)->restStartTime ) },
	{ "bool", "isLoaded", (intptr_t)(&((idAF *)0)->isLoaded), sizeof( ((idAF *)0)->isLoaded ) },
	{ "bool", "isActive", (intptr_t)(&((idAF *)0)->isActive), sizeof( ((idAF *)0)->isActive ) },
	{ "bool", "hasBindConstraints", (intptr_t)(&((idAF *)0)->hasBindConstraints), sizeof( ((idAF *)0)->hasBindConstraints ) },
	{ NULL, 0 }
};

static classVariableInfo_t idIK_typeInfo[] = {
	{ ": bool", "initialized", (intptr_t)(&((idIK *)0)->initialized), sizeof( ((idIK *)0)->initialized ) },
	{ "bool", "ik_activate", (intptr_t)(&((idIK *)0)->ik_activate), sizeof( ((idIK *)0)->ik_activate ) },
	{ "idEntity *", "self", (intptr_t)(&((idIK *)0)->self), sizeof( ((idIK *)0)->self ) },
	{ "idAnimator *", "animator", (intptr_t)(&((idIK *)0)->animator), sizeof( ((idIK *)0)->animator ) },
	{ "int", "modifiedAnim", (intptr_t)(&((idIK *)0)->modifiedAnim), sizeof( ((idIK *)0)->modifiedAnim ) },
	{ "idVec3", "modelOffset", (intptr_t)(&((idIK *)0)->modelOffset), sizeof( ((idIK *)0)->modelOffset ) },
	{ NULL, 0 }
};

static classVariableInfo_t idIK_Walk_typeInfo[] = {
	{ "idClipModel *", "footModel", (intptr_t)(&((idIK_Walk *)0)->footModel), sizeof( ((idIK_Walk *)0)->footModel ) },
	{ "int", "numLegs", (intptr_t)(&((idIK_Walk *)0)->numLegs), sizeof( ((idIK_Walk *)0)->numLegs ) },
	{ "int", "enabledLegs", (intptr_t)(&((idIK_Walk *)0)->enabledLegs), sizeof( ((idIK_Walk *)0)->enabledLegs ) },
	{ "jointHandle_t[8]", "footJoints", (intptr_t)(&((idIK_Walk *)0)->footJoints), sizeof( ((idIK_Walk *)0)->footJoints ) },
	{ "jointHandle_t[8]", "ankleJoints", (intptr_t)(&((idIK_Walk *)0)->ankleJoints), sizeof( ((idIK_Walk *)0)->ankleJoints ) },
	{ "jointHandle_t[8]", "kneeJoints", (intptr_t)(&((idIK_Walk *)0)->kneeJoints), sizeof( ((idIK_Walk *)0)->kneeJoints ) },
	{ "jointHandle_t[8]", "hipJoints", (intptr_t)(&((idIK_Walk *)0)->hipJoints), sizeof( ((idIK_Walk *)0)->hipJoints ) },
	{ "jointHandle_t[8]", "dirJoints", (intptr_t)(&((idIK_Walk *)0)->dirJoints), sizeof( ((idIK_Walk *)0)->dirJoints ) },
	{ "jointHandle_t", "waistJoint", (intptr_t)(&((idIK_Walk *)0)->waistJoint), sizeof( ((idIK_Walk *)0)->waistJoint ) },
	{ "idVec3[8]", "hipForward", (intptr_t)(&((idIK_Walk *)0)->hipForward), sizeof( ((idIK_Walk *)0)->hipForward ) },
	{ "idVec3[8]", "kneeForward", (intptr_t)(&((idIK_Walk *)0)->kneeForward), sizeof( ((idIK_Walk *)0)->kneeForward ) },
	{ "float[8]", "upperLegLength", (intptr_t)(&((idIK_Walk *)0)->upperLegLength), sizeof( ((idIK_Walk *)0)->upperLegLength ) },
	{ "float[8]", "lowerLegLength", (intptr_t)(&((idIK_Walk *)0)->lowerLegLength), sizeof( ((idIK_Walk *)0)->lowerLegLength ) },
	{ "idMat3[8]", "upperLegToHipJoint", (intptr_t)(&((idIK_Walk *)0)->upperLegToHipJoint), sizeof( ((idIK_Walk *)0)->upperLegToHipJoint ) },
	{ "idMat3[8]", "lowerLegToKneeJoint", (intptr_t)(&((idIK_Walk *)0)->lowerLegToKneeJoint), sizeof( ((idIK_Walk *)0)->lowerLegToKneeJoint ) },
	{ "float", "smoothing", (intptr_t)(&((idIK_Walk *)0)->smoothing), sizeof( ((idIK_Walk *)0)->smoothing ) },
	{ "float", "waistSmoothing", (intptr_t)(&((idIK_Walk *)0)->waistSmoothing), sizeof( ((idIK_Walk *)0)->waistSmoothing ) },
	{ "float", "footShift", (intptr_t)(&((idIK_Walk *)0)->footShift), sizeof( ((idIK_Walk *)0)->footShift ) },
	{ "float", "waistShift", (intptr_t)(&((idIK_Walk *)0)->waistShift), sizeof( ((idIK_Walk *)0)->waistShift ) },
	{ "float", "minWaistFloorDist", (intptr_t)(&((idIK_Walk *)0)->minWaistFloorDist), sizeof( ((idIK_Walk *)0)->minWaistFloorDist ) },
	{ "float", "minWaistAnkleDist", (intptr_t)(&((idIK_Walk *)0)->minWaistAnkleDist), sizeof( ((idIK_Walk *)0)->minWaistAnkleDist ) },
	{ "float", "footUpTrace", (intptr_t)(&((idIK_Walk *)0)->footUpTrace), sizeof( ((idIK_Walk *)0)->footUpTrace ) },
	{ "float", "footDownTrace", (intptr_t)(&((idIK_Walk *)0)->footDownTrace), sizeof( ((idIK_Walk *)0)->footDownTrace ) },
	{ "bool", "tiltWaist", (intptr_t)(&((idIK_Walk *)0)->tiltWaist), sizeof( ((idIK_Walk *)0)->tiltWaist ) },
	{ "bool", "usePivot", (intptr_t)(&((idIK_Walk *)0)->usePivot), sizeof( ((idIK_Walk *)0)->usePivot ) },
	{ "int", "pivotFoot", (intptr_t)(&((idIK_Walk *)0)->pivotFoot), sizeof( ((idIK_Walk *)0)->pivotFoot ) },
	{ "float", "pivotYaw", (intptr_t)(&((idIK_Walk *)0)->pivotYaw), sizeof( ((idIK_Walk *)0)->pivotYaw ) },
	{ "idVec3", "pivotPos", (intptr_t)(&((idIK_Walk *)0)->pivotPos), sizeof( ((idIK_Walk *)0)->pivotPos ) },
	{ "bool", "oldHeightsValid", (intptr_t)(&((idIK_Walk *)0)->oldHeightsValid), sizeof( ((idIK_Walk *)0)->oldHeightsValid ) },
	{ "float", "oldWaistHeight", (intptr_t)(&((idIK_Walk *)0)->oldWaistHeight), sizeof( ((idIK_Walk *)0)->oldWaistHeight ) },
	{ "float[8]", "oldAnkleHeights", (intptr_t)(&((idIK_Walk *)0)->oldAnkleHeights), sizeof( ((idIK_Walk *)0)->oldAnkleHeights ) },
	{ "idVec3", "waistOffset", (intptr_t)(&((idIK_Walk *)0)->waistOffset), sizeof( ((idIK_Walk *)0)->waistOffset ) },
	{ NULL, 0 }
};

static classVariableInfo_t idIK_Reach_typeInfo[] = {
	{ "int", "numArms", (intptr_t)(&((idIK_Reach *)0)->numArms), sizeof( ((idIK_Reach *)0)->numArms ) },
	{ "int", "enabledArms", (intptr_t)(&((idIK_Reach *)0)->enabledArms), sizeof( ((idIK_Reach *)0)->enabledArms ) },
	{ "jointHandle_t[2]", "handJoints", (intptr_t)(&((idIK_Reach *)0)->handJoints), sizeof( ((idIK_Reach *)0)->handJoints ) },
	{ "jointHandle_t[2]", "elbowJoints", (intptr_t)(&((idIK_Reach *)0)->elbowJoints), sizeof( ((idIK_Reach *)0)->elbowJoints ) },
	{ "jointHandle_t[2]", "shoulderJoints", (intptr_t)(&((idIK_Reach *)0)->shoulderJoints), sizeof( ((idIK_Reach *)0)->shoulderJoints ) },
	{ "jointHandle_t[2]", "dirJoints", (intptr_t)(&((idIK_Reach *)0)->dirJoints), sizeof( ((idIK_Reach *)0)->dirJoints ) },
	{ "idVec3[2]", "shoulderForward", (intptr_t)(&((idIK_Reach *)0)->shoulderForward), sizeof( ((idIK_Reach *)0)->shoulderForward ) },
	{ "idVec3[2]", "elbowForward", (intptr_t)(&((idIK_Reach *)0)->elbowForward), sizeof( ((idIK_Reach *)0)->elbowForward ) },
	{ "float[2]", "upperArmLength", (intptr_t)(&((idIK_Reach *)0)->upperArmLength), sizeof( ((idIK_Reach *)0)->upperArmLength ) },
	{ "float[2]", "lowerArmLength", (intptr_t)(&((idIK_Reach *)0)->lowerArmLength), sizeof( ((idIK_Reach *)0)->lowerArmLength ) },
	{ "idMat3[2]", "upperArmToShoulderJoint", (intptr_t)(&((idIK_Reach *)0)->upperArmToShoulderJoint), sizeof( ((idIK_Reach *)0)->upperArmToShoulderJoint ) },
	{ "idMat3[2]", "lowerArmToElbowJoint", (intptr_t)(&((idIK_Reach *)0)->lowerArmToElbowJoint), sizeof( ((idIK_Reach *)0)->lowerArmToElbowJoint ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMultiModelAF_typeInfo[] = {
	{ ": idPhysics_AF", "physicsObj", (intptr_t)(&((idMultiModelAF *)0)->physicsObj), sizeof( ((idMultiModelAF *)0)->physicsObj ) },
	{ ": idList < idRenderModel * , TAG_AF >", "modelHandles", (intptr_t)(&((idMultiModelAF *)0)->modelHandles), sizeof( ((idMultiModelAF *)0)->modelHandles ) },
	{ "idList < int , TAG_AF >", "modelDefHandles", (intptr_t)(&((idMultiModelAF *)0)->modelDefHandles), sizeof( ((idMultiModelAF *)0)->modelDefHandles ) },
	{ NULL, 0 }
};

static classVariableInfo_t idChain_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idAFAttachment_typeInfo[] = {
	{ ": idEntity *", "body", (intptr_t)(&((idAFAttachment *)0)->body), sizeof( ((idAFAttachment *)0)->body ) },
	{ "idClipModel *", "combatModel", (intptr_t)(&((idAFAttachment *)0)->combatModel), sizeof( ((idAFAttachment *)0)->combatModel ) },
	{ "int", "idleAnim", (intptr_t)(&((idAFAttachment *)0)->idleAnim), sizeof( ((idAFAttachment *)0)->idleAnim ) },
	{ "jointHandle_t", "attachJoint", (intptr_t)(&((idAFAttachment *)0)->attachJoint), sizeof( ((idAFAttachment *)0)->attachJoint ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_Base_typeInfo[] = {
	{ ": idAF", "af", (intptr_t)(&((idAFEntity_Base *)0)->af), sizeof( ((idAFEntity_Base *)0)->af ) },
	{ "idClipModel *", "combatModel", (intptr_t)(&((idAFEntity_Base *)0)->combatModel), sizeof( ((idAFEntity_Base *)0)->combatModel ) },
	{ "int", "combatModelContents", (intptr_t)(&((idAFEntity_Base *)0)->combatModelContents), sizeof( ((idAFEntity_Base *)0)->combatModelContents ) },
	{ "idVec3", "spawnOrigin", (intptr_t)(&((idAFEntity_Base *)0)->spawnOrigin), sizeof( ((idAFEntity_Base *)0)->spawnOrigin ) },
	{ "idMat3", "spawnAxis", (intptr_t)(&((idAFEntity_Base *)0)->spawnAxis), sizeof( ((idAFEntity_Base *)0)->spawnAxis ) },
	{ "int", "nextSoundTime", (intptr_t)(&((idAFEntity_Base *)0)->nextSoundTime), sizeof( ((idAFEntity_Base *)0)->nextSoundTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_Gibbable_typeInfo[] = {
	{ ": idRenderModel *", "skeletonModel", (intptr_t)(&((idAFEntity_Gibbable *)0)->skeletonModel), sizeof( ((idAFEntity_Gibbable *)0)->skeletonModel ) },
	{ "int", "skeletonModelDefHandle", (intptr_t)(&((idAFEntity_Gibbable *)0)->skeletonModelDefHandle), sizeof( ((idAFEntity_Gibbable *)0)->skeletonModelDefHandle ) },
	{ "bool", "gibbed", (intptr_t)(&((idAFEntity_Gibbable *)0)->gibbed), sizeof( ((idAFEntity_Gibbable *)0)->gibbed ) },
	{ "bool", "wasThrown", (intptr_t)(&((idAFEntity_Gibbable *)0)->wasThrown), sizeof( ((idAFEntity_Gibbable *)0)->wasThrown ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_Generic_typeInfo[] = {
	{ "bool", "keepRunningPhysics", (intptr_t)(&((idAFEntity_Generic *)0)->keepRunningPhysics), sizeof( ((idAFEntity_Generic *)0)->keepRunningPhysics ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_WithAttachedHead_typeInfo[] = {
	{ ": idEntityPtr < idAFAttachment >", "head", (intptr_t)(&((idAFEntity_WithAttachedHead *)0)->head), sizeof( ((idAFEntity_WithAttachedHead *)0)->head ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_Vehicle_typeInfo[] = {
	{ ": idPlayer *", "player", (intptr_t)(&((idAFEntity_Vehicle *)0)->player), sizeof( ((idAFEntity_Vehicle *)0)->player ) },
	{ "jointHandle_t", "eyesJoint", (intptr_t)(&((idAFEntity_Vehicle *)0)->eyesJoint), sizeof( ((idAFEntity_Vehicle *)0)->eyesJoint ) },
	{ "jointHandle_t", "steeringWheelJoint", (intptr_t)(&((idAFEntity_Vehicle *)0)->steeringWheelJoint), sizeof( ((idAFEntity_Vehicle *)0)->steeringWheelJoint ) },
	{ "float", "wheelRadius", (intptr_t)(&((idAFEntity_Vehicle *)0)->wheelRadius), sizeof( ((idAFEntity_Vehicle *)0)->wheelRadius ) },
	{ "float", "steerAngle", (intptr_t)(&((idAFEntity_Vehicle *)0)->steerAngle), sizeof( ((idAFEntity_Vehicle *)0)->steerAngle ) },
	{ "float", "steerSpeed", (intptr_t)(&((idAFEntity_Vehicle *)0)->steerSpeed), sizeof( ((idAFEntity_Vehicle *)0)->steerSpeed ) },
	{ "const idDeclParticle *", "dustSmoke", (intptr_t)(&((idAFEntity_Vehicle *)0)->dustSmoke), sizeof( ((idAFEntity_Vehicle *)0)->dustSmoke ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_VehicleSimple_typeInfo[] = {
	{ ": idClipModel *", "wheelModel", (intptr_t)(&((idAFEntity_VehicleSimple *)0)->wheelModel), sizeof( ((idAFEntity_VehicleSimple *)0)->wheelModel ) },
	{ "idAFConstraint_Suspension *[4]", "suspension", (intptr_t)(&((idAFEntity_VehicleSimple *)0)->suspension), sizeof( ((idAFEntity_VehicleSimple *)0)->suspension ) },
	{ "jointHandle_t[4]", "wheelJoints", (intptr_t)(&((idAFEntity_VehicleSimple *)0)->wheelJoints), sizeof( ((idAFEntity_VehicleSimple *)0)->wheelJoints ) },
	{ "float[4]", "wheelAngles", (intptr_t)(&((idAFEntity_VehicleSimple *)0)->wheelAngles), sizeof( ((idAFEntity_VehicleSimple *)0)->wheelAngles ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_VehicleFourWheels_typeInfo[] = {
	{ ": idAFBody *[4]", "wheels", (intptr_t)(&((idAFEntity_VehicleFourWheels *)0)->wheels), sizeof( ((idAFEntity_VehicleFourWheels *)0)->wheels ) },
	{ "idAFConstraint_Hinge *[2]", "steering", (intptr_t)(&((idAFEntity_VehicleFourWheels *)0)->steering), sizeof( ((idAFEntity_VehicleFourWheels *)0)->steering ) },
	{ "jointHandle_t[4]", "wheelJoints", (intptr_t)(&((idAFEntity_VehicleFourWheels *)0)->wheelJoints), sizeof( ((idAFEntity_VehicleFourWheels *)0)->wheelJoints ) },
	{ "float[4]", "wheelAngles", (intptr_t)(&((idAFEntity_VehicleFourWheels *)0)->wheelAngles), sizeof( ((idAFEntity_VehicleFourWheels *)0)->wheelAngles ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_VehicleSixWheels_typeInfo[] = {
	{ "float", "force", (intptr_t)(&((idAFEntity_VehicleSixWheels *)0)->force), sizeof( ((idAFEntity_VehicleSixWheels *)0)->force ) },
	{ "float", "velocity", (intptr_t)(&((idAFEntity_VehicleSixWheels *)0)->velocity), sizeof( ((idAFEntity_VehicleSixWheels *)0)->velocity ) },
	{ "float", "steerAngle", (intptr_t)(&((idAFEntity_VehicleSixWheels *)0)->steerAngle), sizeof( ((idAFEntity_VehicleSixWheels *)0)->steerAngle ) },
	{ ": idAFBody *[6]", "wheels", (intptr_t)(&((idAFEntity_VehicleSixWheels *)0)->wheels), sizeof( ((idAFEntity_VehicleSixWheels *)0)->wheels ) },
	{ "idAFConstraint_Hinge *[4]", "steering", (intptr_t)(&((idAFEntity_VehicleSixWheels *)0)->steering), sizeof( ((idAFEntity_VehicleSixWheels *)0)->steering ) },
	{ "jointHandle_t[6]", "wheelJoints", (intptr_t)(&((idAFEntity_VehicleSixWheels *)0)->wheelJoints), sizeof( ((idAFEntity_VehicleSixWheels *)0)->wheelJoints ) },
	{ "float[6]", "wheelAngles", (intptr_t)(&((idAFEntity_VehicleSixWheels *)0)->wheelAngles), sizeof( ((idAFEntity_VehicleSixWheels *)0)->wheelAngles ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_VehicleAutomated_typeInfo[] = {
	{ ": idEntity *", "waypoint", (intptr_t)(&((idAFEntity_VehicleAutomated *)0)->waypoint), sizeof( ((idAFEntity_VehicleAutomated *)0)->waypoint ) },
	{ "float", "steeringSpeed", (intptr_t)(&((idAFEntity_VehicleAutomated *)0)->steeringSpeed), sizeof( ((idAFEntity_VehicleAutomated *)0)->steeringSpeed ) },
	{ "float", "currentSteering", (intptr_t)(&((idAFEntity_VehicleAutomated *)0)->currentSteering), sizeof( ((idAFEntity_VehicleAutomated *)0)->currentSteering ) },
	{ "float", "idealSteering", (intptr_t)(&((idAFEntity_VehicleAutomated *)0)->idealSteering), sizeof( ((idAFEntity_VehicleAutomated *)0)->idealSteering ) },
	{ "float", "originHeight", (intptr_t)(&((idAFEntity_VehicleAutomated *)0)->originHeight), sizeof( ((idAFEntity_VehicleAutomated *)0)->originHeight ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_SteamPipe_typeInfo[] = {
	{ ": int", "steamBody", (intptr_t)(&((idAFEntity_SteamPipe *)0)->steamBody), sizeof( ((idAFEntity_SteamPipe *)0)->steamBody ) },
	{ "float", "steamForce", (intptr_t)(&((idAFEntity_SteamPipe *)0)->steamForce), sizeof( ((idAFEntity_SteamPipe *)0)->steamForce ) },
	{ "float", "steamUpForce", (intptr_t)(&((idAFEntity_SteamPipe *)0)->steamUpForce), sizeof( ((idAFEntity_SteamPipe *)0)->steamUpForce ) },
	{ "idForce_Constant", "force", (intptr_t)(&((idAFEntity_SteamPipe *)0)->force), sizeof( ((idAFEntity_SteamPipe *)0)->force ) },
	{ "renderEntity_t", "steamRenderEntity", (intptr_t)(&((idAFEntity_SteamPipe *)0)->steamRenderEntity), sizeof( ((idAFEntity_SteamPipe *)0)->steamRenderEntity ) },
	{ "qhandle_t", "steamModelDefHandle", (intptr_t)(&((idAFEntity_SteamPipe *)0)->steamModelDefHandle), sizeof( ((idAFEntity_SteamPipe *)0)->steamModelDefHandle ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_ClawFourFingers_typeInfo[] = {
	{ ": idAFConstraint_Hinge *[4]", "fingers", (intptr_t)(&((idAFEntity_ClawFourFingers *)0)->fingers), sizeof( ((idAFEntity_ClawFourFingers *)0)->fingers ) },
	{ NULL, 0 }
};

static classVariableInfo_t idHarvestable_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "parentEnt", (intptr_t)(&((idHarvestable *)0)->parentEnt), sizeof( ((idHarvestable *)0)->parentEnt ) },
	{ "float", "triggersize", (intptr_t)(&((idHarvestable *)0)->triggersize), sizeof( ((idHarvestable *)0)->triggersize ) },
	{ "idClipModel *", "trigger", (intptr_t)(&((idHarvestable *)0)->trigger), sizeof( ((idHarvestable *)0)->trigger ) },
	{ "float", "giveDelay", (intptr_t)(&((idHarvestable *)0)->giveDelay), sizeof( ((idHarvestable *)0)->giveDelay ) },
	{ "float", "removeDelay", (intptr_t)(&((idHarvestable *)0)->removeDelay), sizeof( ((idHarvestable *)0)->removeDelay ) },
	{ "bool", "given", (intptr_t)(&((idHarvestable *)0)->given), sizeof( ((idHarvestable *)0)->given ) },
	{ "idEntityPtr < idPlayer >", "player", (intptr_t)(&((idHarvestable *)0)->player), sizeof( ((idHarvestable *)0)->player ) },
	{ "int", "startTime", (intptr_t)(&((idHarvestable *)0)->startTime), sizeof( ((idHarvestable *)0)->startTime ) },
	{ "bool", "fxFollowPlayer", (intptr_t)(&((idHarvestable *)0)->fxFollowPlayer), sizeof( ((idHarvestable *)0)->fxFollowPlayer ) },
	{ "idEntityPtr < idEntityFx >", "fx", (intptr_t)(&((idHarvestable *)0)->fx), sizeof( ((idHarvestable *)0)->fx ) },
	{ "idStr", "fxOrient", (intptr_t)(&((idHarvestable *)0)->fxOrient), sizeof( ((idHarvestable *)0)->fxOrient ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAFEntity_Harvest_typeInfo[] = {
	{ ": idEntityPtr < idHarvestable >", "harvestEnt", (intptr_t)(&((idAFEntity_Harvest *)0)->harvestEnt), sizeof( ((idAFEntity_Harvest *)0)->harvestEnt ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSpawnableEntity_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idPlayerStart_typeInfo[] = {
	{ ": int", "teleportStage", (intptr_t)(&((idPlayerStart *)0)->teleportStage), sizeof( ((idPlayerStart *)0)->teleportStage ) },
	{ NULL, 0 }
};

static classVariableInfo_t idActivator_typeInfo[] = {
	{ ": bool", "stay_on", (intptr_t)(&((idActivator *)0)->stay_on), sizeof( ((idActivator *)0)->stay_on ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPathCorner_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idDamagable_typeInfo[] = {
	{ ": int", "count", (intptr_t)(&((idDamagable *)0)->count), sizeof( ((idDamagable *)0)->count ) },
	{ "int", "nextTriggerTime", (intptr_t)(&((idDamagable *)0)->nextTriggerTime), sizeof( ((idDamagable *)0)->nextTriggerTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idExplodable_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idSpring_typeInfo[] = {
	{ ": idEntity *", "ent1", (intptr_t)(&((idSpring *)0)->ent1), sizeof( ((idSpring *)0)->ent1 ) },
	{ "idEntity *", "ent2", (intptr_t)(&((idSpring *)0)->ent2), sizeof( ((idSpring *)0)->ent2 ) },
	{ "int", "id1", (intptr_t)(&((idSpring *)0)->id1), sizeof( ((idSpring *)0)->id1 ) },
	{ "int", "id2", (intptr_t)(&((idSpring *)0)->id2), sizeof( ((idSpring *)0)->id2 ) },
	{ "idVec3", "p1", (intptr_t)(&((idSpring *)0)->p1), sizeof( ((idSpring *)0)->p1 ) },
	{ "idVec3", "p2", (intptr_t)(&((idSpring *)0)->p2), sizeof( ((idSpring *)0)->p2 ) },
	{ "idForce_Spring", "spring", (intptr_t)(&((idSpring *)0)->spring), sizeof( ((idSpring *)0)->spring ) },
	{ NULL, 0 }
};

static classVariableInfo_t idForceField_typeInfo[] = {
	{ ": idForce_Field", "forceField", (intptr_t)(&((idForceField *)0)->forceField), sizeof( ((idForceField *)0)->forceField ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAnimated_typeInfo[] = {
	{ ": int", "num_anims", (intptr_t)(&((idAnimated *)0)->num_anims), sizeof( ((idAnimated *)0)->num_anims ) },
	{ "int", "current_anim_index", (intptr_t)(&((idAnimated *)0)->current_anim_index), sizeof( ((idAnimated *)0)->current_anim_index ) },
	{ "int", "anim", (intptr_t)(&((idAnimated *)0)->anim), sizeof( ((idAnimated *)0)->anim ) },
	{ "int", "blendFrames", (intptr_t)(&((idAnimated *)0)->blendFrames), sizeof( ((idAnimated *)0)->blendFrames ) },
	{ "jointHandle_t", "soundJoint", (intptr_t)(&((idAnimated *)0)->soundJoint), sizeof( ((idAnimated *)0)->soundJoint ) },
	{ "idEntityPtr < idEntity >", "activator", (intptr_t)(&((idAnimated *)0)->activator), sizeof( ((idAnimated *)0)->activator ) },
	{ "bool", "activated", (intptr_t)(&((idAnimated *)0)->activated), sizeof( ((idAnimated *)0)->activated ) },
	{ "int", "achievement", (intptr_t)(&((idAnimated *)0)->achievement), sizeof( ((idAnimated *)0)->achievement ) },
	{ NULL, 0 }
};

static classVariableInfo_t idStaticEntity_typeInfo[] = {
	{ "int", "spawnTime", (intptr_t)(&((idStaticEntity *)0)->spawnTime), sizeof( ((idStaticEntity *)0)->spawnTime ) },
	{ "bool", "active", (intptr_t)(&((idStaticEntity *)0)->active), sizeof( ((idStaticEntity *)0)->active ) },
	{ "idVec4", "fadeFrom", (intptr_t)(&((idStaticEntity *)0)->fadeFrom), sizeof( ((idStaticEntity *)0)->fadeFrom ) },
	{ "idVec4", "fadeTo", (intptr_t)(&((idStaticEntity *)0)->fadeTo), sizeof( ((idStaticEntity *)0)->fadeTo ) },
	{ "int", "fadeStart", (intptr_t)(&((idStaticEntity *)0)->fadeStart), sizeof( ((idStaticEntity *)0)->fadeStart ) },
	{ "int", "fadeEnd", (intptr_t)(&((idStaticEntity *)0)->fadeEnd), sizeof( ((idStaticEntity *)0)->fadeEnd ) },
	{ "bool", "runGui", (intptr_t)(&((idStaticEntity *)0)->runGui), sizeof( ((idStaticEntity *)0)->runGui ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncEmitter_typeInfo[] = {
	{ ": bool", "hidden", (intptr_t)(&((idFuncEmitter *)0)->hidden), sizeof( ((idFuncEmitter *)0)->hidden ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncShootProjectile_typeInfo[] = {
	{ ": int", "mRespawnDelay", (intptr_t)(&((idFuncShootProjectile *)0)->mRespawnDelay), sizeof( ((idFuncShootProjectile *)0)->mRespawnDelay ) },
	{ "int", "mRespawnTime", (intptr_t)(&((idFuncShootProjectile *)0)->mRespawnTime), sizeof( ((idFuncShootProjectile *)0)->mRespawnTime ) },
	{ "float", "mShootSpeed", (intptr_t)(&((idFuncShootProjectile *)0)->mShootSpeed), sizeof( ((idFuncShootProjectile *)0)->mShootSpeed ) },
	{ "idVec3", "mShootDir", (intptr_t)(&((idFuncShootProjectile *)0)->mShootDir), sizeof( ((idFuncShootProjectile *)0)->mShootDir ) },
	{ "idStr", "mEntityDefName", (intptr_t)(&((idFuncShootProjectile *)0)->mEntityDefName), sizeof( ((idFuncShootProjectile *)0)->mEntityDefName ) },
	{ "idEntityPtr < idEntity >", "mLastProjectile", (intptr_t)(&((idFuncShootProjectile *)0)->mLastProjectile), sizeof( ((idFuncShootProjectile *)0)->mLastProjectile ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncSmoke_typeInfo[] = {
	{ ": int", "smokeTime", (intptr_t)(&((idFuncSmoke *)0)->smokeTime), sizeof( ((idFuncSmoke *)0)->smokeTime ) },
	{ "const idDeclParticle *", "smoke", (intptr_t)(&((idFuncSmoke *)0)->smoke), sizeof( ((idFuncSmoke *)0)->smoke ) },
	{ "bool", "restart", (intptr_t)(&((idFuncSmoke *)0)->restart), sizeof( ((idFuncSmoke *)0)->restart ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncSplat_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTextEntity_typeInfo[] = {
	{ ": idStr", "text", (intptr_t)(&((idTextEntity *)0)->text), sizeof( ((idTextEntity *)0)->text ) },
	{ "bool", "playerOriented", (intptr_t)(&((idTextEntity *)0)->playerOriented), sizeof( ((idTextEntity *)0)->playerOriented ) },
	{ NULL, 0 }
};

static classVariableInfo_t idLocationEntity_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idLocationSeparatorEntity_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idVacuumSeparatorEntity_typeInfo[] = {
	{ ": qhandle_t", "portal", (intptr_t)(&((idVacuumSeparatorEntity *)0)->portal), sizeof( ((idVacuumSeparatorEntity *)0)->portal ) },
	{ NULL, 0 }
};

static classVariableInfo_t idVacuumEntity_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idBeam_typeInfo[] = {
	{ "idEntityPtr < idBeam >", "target", (intptr_t)(&((idBeam *)0)->target), sizeof( ((idBeam *)0)->target ) },
	{ "idEntityPtr < idBeam >", "master", (intptr_t)(&((idBeam *)0)->master), sizeof( ((idBeam *)0)->master ) },
	{ NULL, 0 }
};

static classVariableInfo_t idLiquid_typeInfo[] = {
	{ "idRenderModelLiquid *", "model", (intptr_t)(&((idLiquid *)0)->model), sizeof( ((idLiquid *)0)->model ) },
	{ NULL, 0 }
};

static classVariableInfo_t idShaking_typeInfo[] = {
	{ ": idPhysics_Parametric", "physicsObj", (intptr_t)(&((idShaking *)0)->physicsObj), sizeof( ((idShaking *)0)->physicsObj ) },
	{ "bool", "active", (intptr_t)(&((idShaking *)0)->active), sizeof( ((idShaking *)0)->active ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEarthQuake_typeInfo[] = {
	{ ": int", "nextTriggerTime", (intptr_t)(&((idEarthQuake *)0)->nextTriggerTime), sizeof( ((idEarthQuake *)0)->nextTriggerTime ) },
	{ "int", "shakeStopTime", (intptr_t)(&((idEarthQuake *)0)->shakeStopTime), sizeof( ((idEarthQuake *)0)->shakeStopTime ) },
	{ "float", "wait", (intptr_t)(&((idEarthQuake *)0)->wait), sizeof( ((idEarthQuake *)0)->wait ) },
	{ "float", "random", (intptr_t)(&((idEarthQuake *)0)->random), sizeof( ((idEarthQuake *)0)->random ) },
	{ "bool", "triggered", (intptr_t)(&((idEarthQuake *)0)->triggered), sizeof( ((idEarthQuake *)0)->triggered ) },
	{ "bool", "playerOriented", (intptr_t)(&((idEarthQuake *)0)->playerOriented), sizeof( ((idEarthQuake *)0)->playerOriented ) },
	{ "bool", "disabled", (intptr_t)(&((idEarthQuake *)0)->disabled), sizeof( ((idEarthQuake *)0)->disabled ) },
	{ "float", "shakeTime", (intptr_t)(&((idEarthQuake *)0)->shakeTime), sizeof( ((idEarthQuake *)0)->shakeTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncPortal_typeInfo[] = {
	{ ": qhandle_t", "portal", (intptr_t)(&((idFuncPortal *)0)->portal), sizeof( ((idFuncPortal *)0)->portal ) },
	{ "bool", "state", (intptr_t)(&((idFuncPortal *)0)->state), sizeof( ((idFuncPortal *)0)->state ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncAASPortal_typeInfo[] = {
	{ ": bool", "state", (intptr_t)(&((idFuncAASPortal *)0)->state), sizeof( ((idFuncAASPortal *)0)->state ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncAASObstacle_typeInfo[] = {
	{ ": bool", "state", (intptr_t)(&((idFuncAASObstacle *)0)->state), sizeof( ((idFuncAASObstacle *)0)->state ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncRadioChatter_typeInfo[] = {
	{ ": float", "time", (intptr_t)(&((idFuncRadioChatter *)0)->time), sizeof( ((idFuncRadioChatter *)0)->time ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPhantomObjects_typeInfo[] = {
	{ "int", "end_time", (intptr_t)(&((idPhantomObjects *)0)->end_time), sizeof( ((idPhantomObjects *)0)->end_time ) },
	{ "float", "throw_time", (intptr_t)(&((idPhantomObjects *)0)->throw_time), sizeof( ((idPhantomObjects *)0)->throw_time ) },
	{ "float", "shake_time", (intptr_t)(&((idPhantomObjects *)0)->shake_time), sizeof( ((idPhantomObjects *)0)->shake_time ) },
	{ "idVec3", "shake_ang", (intptr_t)(&((idPhantomObjects *)0)->shake_ang), sizeof( ((idPhantomObjects *)0)->shake_ang ) },
	{ "float", "speed", (intptr_t)(&((idPhantomObjects *)0)->speed), sizeof( ((idPhantomObjects *)0)->speed ) },
	{ "int", "min_wait", (intptr_t)(&((idPhantomObjects *)0)->min_wait), sizeof( ((idPhantomObjects *)0)->min_wait ) },
	{ "int", "max_wait", (intptr_t)(&((idPhantomObjects *)0)->max_wait), sizeof( ((idPhantomObjects *)0)->max_wait ) },
	{ "idEntityPtr < idActor >", "target", (intptr_t)(&((idPhantomObjects *)0)->target), sizeof( ((idPhantomObjects *)0)->target ) },
	{ "idList < int >", "targetTime", (intptr_t)(&((idPhantomObjects *)0)->targetTime), sizeof( ((idPhantomObjects *)0)->targetTime ) },
	{ "idList < idVec3 >", "lastTargetPos", (intptr_t)(&((idPhantomObjects *)0)->lastTargetPos), sizeof( ((idPhantomObjects *)0)->lastTargetPos ) },
	{ NULL, 0 }
};

static classVariableInfo_t idShockwave_typeInfo[] = {
	{ "bool", "isActive", (intptr_t)(&((idShockwave *)0)->isActive), sizeof( ((idShockwave *)0)->isActive ) },
	{ "int", "startTime", (intptr_t)(&((idShockwave *)0)->startTime), sizeof( ((idShockwave *)0)->startTime ) },
	{ "int", "duration", (intptr_t)(&((idShockwave *)0)->duration), sizeof( ((idShockwave *)0)->duration ) },
	{ "float", "startSize", (intptr_t)(&((idShockwave *)0)->startSize), sizeof( ((idShockwave *)0)->startSize ) },
	{ "float", "endSize", (intptr_t)(&((idShockwave *)0)->endSize), sizeof( ((idShockwave *)0)->endSize ) },
	{ "float", "currentSize", (intptr_t)(&((idShockwave *)0)->currentSize), sizeof( ((idShockwave *)0)->currentSize ) },
	{ "float", "magnitude", (intptr_t)(&((idShockwave *)0)->magnitude), sizeof( ((idShockwave *)0)->magnitude ) },
	{ "float", "height", (intptr_t)(&((idShockwave *)0)->height), sizeof( ((idShockwave *)0)->height ) },
	{ "bool", "playerDamaged", (intptr_t)(&((idShockwave *)0)->playerDamaged), sizeof( ((idShockwave *)0)->playerDamaged ) },
	{ "float", "playerDamageSize", (intptr_t)(&((idShockwave *)0)->playerDamageSize), sizeof( ((idShockwave *)0)->playerDamageSize ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncMountedObject_typeInfo[] = {
	{ ": int", "harc", (intptr_t)(&((idFuncMountedObject *)0)->harc), sizeof( ((idFuncMountedObject *)0)->harc ) },
	{ "int", "varc", (intptr_t)(&((idFuncMountedObject *)0)->varc), sizeof( ((idFuncMountedObject *)0)->varc ) },
	{ ": bool", "isMounted", (intptr_t)(&((idFuncMountedObject *)0)->isMounted), sizeof( ((idFuncMountedObject *)0)->isMounted ) },
	{ "function_t *", "scriptFunction", (intptr_t)(&((idFuncMountedObject *)0)->scriptFunction), sizeof( ((idFuncMountedObject *)0)->scriptFunction ) },
	{ "idPlayer *", "mountedPlayer", (intptr_t)(&((idFuncMountedObject *)0)->mountedPlayer), sizeof( ((idFuncMountedObject *)0)->mountedPlayer ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFuncMountedWeapon_typeInfo[] = {
	{ ": idEntity *", "turret", (intptr_t)(&((idFuncMountedWeapon *)0)->turret), sizeof( ((idFuncMountedWeapon *)0)->turret ) },
	{ "idVec3", "muzzleOrigin", (intptr_t)(&((idFuncMountedWeapon *)0)->muzzleOrigin), sizeof( ((idFuncMountedWeapon *)0)->muzzleOrigin ) },
	{ "idMat3", "muzzleAxis", (intptr_t)(&((idFuncMountedWeapon *)0)->muzzleAxis), sizeof( ((idFuncMountedWeapon *)0)->muzzleAxis ) },
	{ "float", "weaponLastFireTime", (intptr_t)(&((idFuncMountedWeapon *)0)->weaponLastFireTime), sizeof( ((idFuncMountedWeapon *)0)->weaponLastFireTime ) },
	{ "float", "weaponFireDelay", (intptr_t)(&((idFuncMountedWeapon *)0)->weaponFireDelay), sizeof( ((idFuncMountedWeapon *)0)->weaponFireDelay ) },
	{ "const idDict *", "projectile", (intptr_t)(&((idFuncMountedWeapon *)0)->projectile), sizeof( ((idFuncMountedWeapon *)0)->projectile ) },
	{ "const idSoundShader *", "soundFireWeapon", (intptr_t)(&((idFuncMountedWeapon *)0)->soundFireWeapon), sizeof( ((idFuncMountedWeapon *)0)->soundFireWeapon ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPortalSky_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idAnimState_typeInfo[] = {
	{ ": bool", "idleAnim", (intptr_t)(&((idAnimState *)0)->idleAnim), sizeof( ((idAnimState *)0)->idleAnim ) },
	{ "idStr", "state", (intptr_t)(&((idAnimState *)0)->state), sizeof( ((idAnimState *)0)->state ) },
	{ "int", "animBlendFrames", (intptr_t)(&((idAnimState *)0)->animBlendFrames), sizeof( ((idAnimState *)0)->animBlendFrames ) },
	{ "int", "lastAnimBlendFrames", (intptr_t)(&((idAnimState *)0)->lastAnimBlendFrames), sizeof( ((idAnimState *)0)->lastAnimBlendFrames ) },
	{ ": idActor *", "self", (intptr_t)(&((idAnimState *)0)->self), sizeof( ((idAnimState *)0)->self ) },
	{ "idAnimator *", "animator", (intptr_t)(&((idAnimState *)0)->animator), sizeof( ((idAnimState *)0)->animator ) },
	{ "idThread *", "thread", (intptr_t)(&((idAnimState *)0)->thread), sizeof( ((idAnimState *)0)->thread ) },
	{ "rvStateThread", "stateThread", (intptr_t)(&((idAnimState *)0)->stateThread), sizeof( ((idAnimState *)0)->stateThread ) },
	{ "int", "channel", (intptr_t)(&((idAnimState *)0)->channel), sizeof( ((idAnimState *)0)->channel ) },
	{ "bool", "disabled", (intptr_t)(&((idAnimState *)0)->disabled), sizeof( ((idAnimState *)0)->disabled ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAttachInfo_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "ent", (intptr_t)(&((idAttachInfo *)0)->ent), sizeof( ((idAttachInfo *)0)->ent ) },
	{ "int", "channel", (intptr_t)(&((idAttachInfo *)0)->channel), sizeof( ((idAttachInfo *)0)->channel ) },
	{ NULL, 0 }
};

static classVariableInfo_t copyJoints_t_typeInfo[] = {
	{ "jointModTransform_t", "mod", (intptr_t)(&((copyJoints_t *)0)->mod), sizeof( ((copyJoints_t *)0)->mod ) },
	{ "jointHandle_t", "from", (intptr_t)(&((copyJoints_t *)0)->from), sizeof( ((copyJoints_t *)0)->from ) },
	{ "jointHandle_t", "to", (intptr_t)(&((copyJoints_t *)0)->to), sizeof( ((copyJoints_t *)0)->to ) },
	{ NULL, 0 }
};

static classVariableInfo_t idActor_typeInfo[] = {
	{ "int", "team", (intptr_t)(&((idActor *)0)->team), sizeof( ((idActor *)0)->team ) },
	{ "int", "rank", (intptr_t)(&((idActor *)0)->rank), sizeof( ((idActor *)0)->rank ) },
	{ "idMat3", "viewAxis", (intptr_t)(&((idActor *)0)->viewAxis), sizeof( ((idActor *)0)->viewAxis ) },
	{ "idLinkList < idActor >", "enemyNode", (intptr_t)(&((idActor *)0)->enemyNode), sizeof( ((idActor *)0)->enemyNode ) },
	{ "idLinkList < idActor >", "enemyList", (intptr_t)(&((idActor *)0)->enemyList), sizeof( ((idActor *)0)->enemyList ) },
	{ "float", "fovDot", (intptr_t)(&((idActor *)0)->fovDot), sizeof( ((idActor *)0)->fovDot ) },
	{ "idVec3", "eyeOffset", (intptr_t)(&((idActor *)0)->eyeOffset), sizeof( ((idActor *)0)->eyeOffset ) },
	{ "idVec3", "modelOffset", (intptr_t)(&((idActor *)0)->modelOffset), sizeof( ((idActor *)0)->modelOffset ) },
	{ "idAngles", "deltaViewAngles", (intptr_t)(&((idActor *)0)->deltaViewAngles), sizeof( ((idActor *)0)->deltaViewAngles ) },
	{ "int", "pain_debounce_time", (intptr_t)(&((idActor *)0)->pain_debounce_time), sizeof( ((idActor *)0)->pain_debounce_time ) },
	{ "int", "pain_delay", (intptr_t)(&((idActor *)0)->pain_delay), sizeof( ((idActor *)0)->pain_delay ) },
	{ "int", "pain_threshold", (intptr_t)(&((idActor *)0)->pain_threshold), sizeof( ((idActor *)0)->pain_threshold ) },
	{ "idStrList", "damageGroups", (intptr_t)(&((idActor *)0)->damageGroups), sizeof( ((idActor *)0)->damageGroups ) },
	{ "idList < float , TAG_ACTOR >", "damageScale", (intptr_t)(&((idActor *)0)->damageScale), sizeof( ((idActor *)0)->damageScale ) },
	{ "bool", "use_combat_bbox", (intptr_t)(&((idActor *)0)->use_combat_bbox), sizeof( ((idActor *)0)->use_combat_bbox ) },
	{ "idEntityPtr < idAFAttachment >", "head", (intptr_t)(&((idActor *)0)->head), sizeof( ((idActor *)0)->head ) },
	{ "idList < copyJoints_t , TAG_ACTOR >", "copyJoints", (intptr_t)(&((idActor *)0)->copyJoints), sizeof( ((idActor *)0)->copyJoints ) },
	{ "const function_t *", "state", (intptr_t)(&((idActor *)0)->state), sizeof( ((idActor *)0)->state ) },
	{ "const function_t *", "idealState", (intptr_t)(&((idActor *)0)->idealState), sizeof( ((idActor *)0)->idealState ) },
	{ "jointHandle_t", "leftEyeJoint", (intptr_t)(&((idActor *)0)->leftEyeJoint), sizeof( ((idActor *)0)->leftEyeJoint ) },
	{ "jointHandle_t", "rightEyeJoint", (intptr_t)(&((idActor *)0)->rightEyeJoint), sizeof( ((idActor *)0)->rightEyeJoint ) },
	{ "jointHandle_t", "soundJoint", (intptr_t)(&((idActor *)0)->soundJoint), sizeof( ((idActor *)0)->soundJoint ) },
	{ "idIK_Walk", "walkIK", (intptr_t)(&((idActor *)0)->walkIK), sizeof( ((idActor *)0)->walkIK ) },
	{ "idStr", "animPrefix", (intptr_t)(&((idActor *)0)->animPrefix), sizeof( ((idActor *)0)->animPrefix ) },
	{ "idStr", "painAnim", (intptr_t)(&((idActor *)0)->painAnim), sizeof( ((idActor *)0)->painAnim ) },
	{ "int", "blink_anim", (intptr_t)(&((idActor *)0)->blink_anim), sizeof( ((idActor *)0)->blink_anim ) },
	{ "int", "blink_time", (intptr_t)(&((idActor *)0)->blink_time), sizeof( ((idActor *)0)->blink_time ) },
	{ "int", "blink_min", (intptr_t)(&((idActor *)0)->blink_min), sizeof( ((idActor *)0)->blink_min ) },
	{ "int", "blink_max", (intptr_t)(&((idActor *)0)->blink_max), sizeof( ((idActor *)0)->blink_max ) },
	{ "idThread *", "scriptThread", (intptr_t)(&((idActor *)0)->scriptThread), sizeof( ((idActor *)0)->scriptThread ) },
	{ "idStr", "waitState", (intptr_t)(&((idActor *)0)->waitState), sizeof( ((idActor *)0)->waitState ) },
	{ "idAnimState", "headAnim", (intptr_t)(&((idActor *)0)->headAnim), sizeof( ((idActor *)0)->headAnim ) },
	{ "idAnimState", "torsoAnim", (intptr_t)(&((idActor *)0)->torsoAnim), sizeof( ((idActor *)0)->torsoAnim ) },
	{ "idAnimState", "legsAnim", (intptr_t)(&((idActor *)0)->legsAnim), sizeof( ((idActor *)0)->legsAnim ) },
	{ "rvStateThread", "stateThread", (intptr_t)(&((idActor *)0)->stateThread), sizeof( ((idActor *)0)->stateThread ) },
	{ "bool", "allowPain", (intptr_t)(&((idActor *)0)->allowPain), sizeof( ((idActor *)0)->allowPain ) },
	{ "bool", "allowEyeFocus", (intptr_t)(&((idActor *)0)->allowEyeFocus), sizeof( ((idActor *)0)->allowEyeFocus ) },
	{ "bool", "finalBoss", (intptr_t)(&((idActor *)0)->finalBoss), sizeof( ((idActor *)0)->finalBoss ) },
	{ "int", "painTime", (intptr_t)(&((idActor *)0)->painTime), sizeof( ((idActor *)0)->painTime ) },
	{ "bool", "damageNotByFists", (intptr_t)(&((idActor *)0)->damageNotByFists), sizeof( ((idActor *)0)->damageNotByFists ) },
	{ "idList < idAttachInfo , TAG_ACTOR >", "attachments", (intptr_t)(&((idActor *)0)->attachments), sizeof( ((idActor *)0)->attachments ) },
	{ "int", "damageCap", (intptr_t)(&((idActor *)0)->damageCap), sizeof( ((idActor *)0)->damageCap ) },
	{ NULL, 0 }
};

static classVariableInfo_t idProjectile_simulatedProjectile_t_typeInfo[] = {
	{ "idProjectile *", "projectile", (intptr_t)(&((idProjectile::simulatedProjectile_t *)0)->projectile), sizeof( ((idProjectile::simulatedProjectile_t *)0)->projectile ) },
	{ "int", "startTime", (intptr_t)(&((idProjectile::simulatedProjectile_t *)0)->startTime), sizeof( ((idProjectile::simulatedProjectile_t *)0)->startTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idProjectile_projectileFlags_s_typeInfo[] = {
//	{ "bool", "detonate_on_world", (intptr_t)(&((idProjectile::projectileFlags_s *)0)->detonate_on_world), sizeof( ((idProjectile::projectileFlags_s *)0)->detonate_on_world ) },
//	{ "bool", "detonate_on_actor", (intptr_t)(&((idProjectile::projectileFlags_s *)0)->detonate_on_actor), sizeof( ((idProjectile::projectileFlags_s *)0)->detonate_on_actor ) },
//	{ "bool", "randomShaderSpin", (intptr_t)(&((idProjectile::projectileFlags_s *)0)->randomShaderSpin), sizeof( ((idProjectile::projectileFlags_s *)0)->randomShaderSpin ) },
//	{ "bool", "isTracer", (intptr_t)(&((idProjectile::projectileFlags_s *)0)->isTracer), sizeof( ((idProjectile::projectileFlags_s *)0)->isTracer ) },
//	{ "bool", "noSplashDamage", (intptr_t)(&((idProjectile::projectileFlags_s *)0)->noSplashDamage), sizeof( ((idProjectile::projectileFlags_s *)0)->noSplashDamage ) },
	{ NULL, 0 }
};

static classVariableInfo_t idProjectile_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "owner", (intptr_t)(&((idProjectile *)0)->owner), sizeof( ((idProjectile *)0)->owner ) },
	{ "idProjectile::projectileFlags_s", "projectileFlags", (intptr_t)(&((idProjectile *)0)->projectileFlags), sizeof( ((idProjectile *)0)->projectileFlags ) },
	{ "bool", "launchedFromGrabber", (intptr_t)(&((idProjectile *)0)->launchedFromGrabber), sizeof( ((idProjectile *)0)->launchedFromGrabber ) },
	{ "float", "thrust", (intptr_t)(&((idProjectile *)0)->thrust), sizeof( ((idProjectile *)0)->thrust ) },
	{ "int", "thrust_end", (intptr_t)(&((idProjectile *)0)->thrust_end), sizeof( ((idProjectile *)0)->thrust_end ) },
	{ "float", "damagePower", (intptr_t)(&((idProjectile *)0)->damagePower), sizeof( ((idProjectile *)0)->damagePower ) },
	{ "renderLight_t", "renderLight", (intptr_t)(&((idProjectile *)0)->renderLight), sizeof( ((idProjectile *)0)->renderLight ) },
	{ "qhandle_t", "lightDefHandle", (intptr_t)(&((idProjectile *)0)->lightDefHandle), sizeof( ((idProjectile *)0)->lightDefHandle ) },
	{ "idVec3", "lightOffset", (intptr_t)(&((idProjectile *)0)->lightOffset), sizeof( ((idProjectile *)0)->lightOffset ) },
	{ "int", "lightStartTime", (intptr_t)(&((idProjectile *)0)->lightStartTime), sizeof( ((idProjectile *)0)->lightStartTime ) },
	{ "int", "lightEndTime", (intptr_t)(&((idProjectile *)0)->lightEndTime), sizeof( ((idProjectile *)0)->lightEndTime ) },
	{ "idVec3", "lightColor", (intptr_t)(&((idProjectile *)0)->lightColor), sizeof( ((idProjectile *)0)->lightColor ) },
	{ "idForce_Constant", "thruster", (intptr_t)(&((idProjectile *)0)->thruster), sizeof( ((idProjectile *)0)->thruster ) },
	{ "idPhysics_RigidBody", "physicsObj", (intptr_t)(&((idProjectile *)0)->physicsObj), sizeof( ((idProjectile *)0)->physicsObj ) },
	{ "const idDeclParticle *", "smokeFly", (intptr_t)(&((idProjectile *)0)->smokeFly), sizeof( ((idProjectile *)0)->smokeFly ) },
	{ "int", "smokeFlyTime", (intptr_t)(&((idProjectile *)0)->smokeFlyTime), sizeof( ((idProjectile *)0)->smokeFlyTime ) },
	{ "bool", "mNoExplodeDisappear", (intptr_t)(&((idProjectile *)0)->mNoExplodeDisappear), sizeof( ((idProjectile *)0)->mNoExplodeDisappear ) },
	{ "bool", "mTouchTriggers", (intptr_t)(&((idProjectile *)0)->mTouchTriggers), sizeof( ((idProjectile *)0)->mTouchTriggers ) },
	{ "int", "originalTimeGroup", (intptr_t)(&((idProjectile *)0)->originalTimeGroup), sizeof( ((idProjectile *)0)->originalTimeGroup ) },
	{ "projectileState_t", "state", (intptr_t)(&((idProjectile *)0)->state), sizeof( ((idProjectile *)0)->state ) },
	{ ": idVec3", "launchOrigin", (intptr_t)(&((idProjectile *)0)->launchOrigin), sizeof( ((idProjectile *)0)->launchOrigin ) },
	{ "idMat3", "launchAxis", (intptr_t)(&((idProjectile *)0)->launchAxis), sizeof( ((idProjectile *)0)->launchAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t idGuidedProjectile_typeInfo[] = {
	{ ": float", "speed", (intptr_t)(&((idGuidedProjectile *)0)->speed), sizeof( ((idGuidedProjectile *)0)->speed ) },
	{ "idEntityPtr < idEntity >", "enemy", (intptr_t)(&((idGuidedProjectile *)0)->enemy), sizeof( ((idGuidedProjectile *)0)->enemy ) },
	{ ": idAngles", "rndScale", (intptr_t)(&((idGuidedProjectile *)0)->rndScale), sizeof( ((idGuidedProjectile *)0)->rndScale ) },
	{ "idAngles", "rndAng", (intptr_t)(&((idGuidedProjectile *)0)->rndAng), sizeof( ((idGuidedProjectile *)0)->rndAng ) },
	{ "idAngles", "angles", (intptr_t)(&((idGuidedProjectile *)0)->angles), sizeof( ((idGuidedProjectile *)0)->angles ) },
	{ "int", "rndUpdateTime", (intptr_t)(&((idGuidedProjectile *)0)->rndUpdateTime), sizeof( ((idGuidedProjectile *)0)->rndUpdateTime ) },
	{ "float", "turn_max", (intptr_t)(&((idGuidedProjectile *)0)->turn_max), sizeof( ((idGuidedProjectile *)0)->turn_max ) },
	{ "float", "clamp_dist", (intptr_t)(&((idGuidedProjectile *)0)->clamp_dist), sizeof( ((idGuidedProjectile *)0)->clamp_dist ) },
	{ "bool", "burstMode", (intptr_t)(&((idGuidedProjectile *)0)->burstMode), sizeof( ((idGuidedProjectile *)0)->burstMode ) },
	{ "bool", "unGuided", (intptr_t)(&((idGuidedProjectile *)0)->unGuided), sizeof( ((idGuidedProjectile *)0)->unGuided ) },
	{ "float", "burstDist", (intptr_t)(&((idGuidedProjectile *)0)->burstDist), sizeof( ((idGuidedProjectile *)0)->burstDist ) },
	{ "float", "burstVelocity", (intptr_t)(&((idGuidedProjectile *)0)->burstVelocity), sizeof( ((idGuidedProjectile *)0)->burstVelocity ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSoulCubeMissile_typeInfo[] = {
	{ ": idVec3", "startingVelocity", (intptr_t)(&((idSoulCubeMissile *)0)->startingVelocity), sizeof( ((idSoulCubeMissile *)0)->startingVelocity ) },
	{ "idVec3", "endingVelocity", (intptr_t)(&((idSoulCubeMissile *)0)->endingVelocity), sizeof( ((idSoulCubeMissile *)0)->endingVelocity ) },
	{ "float", "accelTime", (intptr_t)(&((idSoulCubeMissile *)0)->accelTime), sizeof( ((idSoulCubeMissile *)0)->accelTime ) },
	{ "int", "launchTime", (intptr_t)(&((idSoulCubeMissile *)0)->launchTime), sizeof( ((idSoulCubeMissile *)0)->launchTime ) },
	{ "bool", "killPhase", (intptr_t)(&((idSoulCubeMissile *)0)->killPhase), sizeof( ((idSoulCubeMissile *)0)->killPhase ) },
	{ "bool", "returnPhase", (intptr_t)(&((idSoulCubeMissile *)0)->returnPhase), sizeof( ((idSoulCubeMissile *)0)->returnPhase ) },
	{ "idVec3", "destOrg", (intptr_t)(&((idSoulCubeMissile *)0)->destOrg), sizeof( ((idSoulCubeMissile *)0)->destOrg ) },
	{ "idVec3", "orbitOrg", (intptr_t)(&((idSoulCubeMissile *)0)->orbitOrg), sizeof( ((idSoulCubeMissile *)0)->orbitOrg ) },
	{ "int", "orbitTime", (intptr_t)(&((idSoulCubeMissile *)0)->orbitTime), sizeof( ((idSoulCubeMissile *)0)->orbitTime ) },
	{ "int", "smokeKillTime", (intptr_t)(&((idSoulCubeMissile *)0)->smokeKillTime), sizeof( ((idSoulCubeMissile *)0)->smokeKillTime ) },
	{ "const idDeclParticle *", "smokeKill", (intptr_t)(&((idSoulCubeMissile *)0)->smokeKill), sizeof( ((idSoulCubeMissile *)0)->smokeKill ) },
	{ NULL, 0 }
};

static classVariableInfo_t beamTarget_t_typeInfo[] = {
	{ "idEntityPtr < idEntity >", "target", (intptr_t)(&((beamTarget_t *)0)->target), sizeof( ((beamTarget_t *)0)->target ) },
	{ "renderEntity_t", "renderEntity", (intptr_t)(&((beamTarget_t *)0)->renderEntity), sizeof( ((beamTarget_t *)0)->renderEntity ) },
	{ "qhandle_t", "modelDefHandle", (intptr_t)(&((beamTarget_t *)0)->modelDefHandle), sizeof( ((beamTarget_t *)0)->modelDefHandle ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBFGProjectile_typeInfo[] = {
	{ ": idList < beamTarget_t , TAG_PROJECTILE >", "beamTargets", (intptr_t)(&((idBFGProjectile *)0)->beamTargets), sizeof( ((idBFGProjectile *)0)->beamTargets ) },
	{ "renderEntity_t", "secondModel", (intptr_t)(&((idBFGProjectile *)0)->secondModel), sizeof( ((idBFGProjectile *)0)->secondModel ) },
	{ "qhandle_t", "secondModelDefHandle", (intptr_t)(&((idBFGProjectile *)0)->secondModelDefHandle), sizeof( ((idBFGProjectile *)0)->secondModelDefHandle ) },
	{ "int", "nextDamageTime", (intptr_t)(&((idBFGProjectile *)0)->nextDamageTime), sizeof( ((idBFGProjectile *)0)->nextDamageTime ) },
	{ "idStr", "damageFreq", (intptr_t)(&((idBFGProjectile *)0)->damageFreq), sizeof( ((idBFGProjectile *)0)->damageFreq ) },
	{ NULL, 0 }
};

static classVariableInfo_t idHomingProjectile_typeInfo[] = {
	{ ": float", "speed", (intptr_t)(&((idHomingProjectile *)0)->speed), sizeof( ((idHomingProjectile *)0)->speed ) },
	{ "idEntityPtr < idEntity >", "enemy", (intptr_t)(&((idHomingProjectile *)0)->enemy), sizeof( ((idHomingProjectile *)0)->enemy ) },
	{ "idVec3", "seekPos", (intptr_t)(&((idHomingProjectile *)0)->seekPos), sizeof( ((idHomingProjectile *)0)->seekPos ) },
	{ ": idAngles", "rndScale", (intptr_t)(&((idHomingProjectile *)0)->rndScale), sizeof( ((idHomingProjectile *)0)->rndScale ) },
	{ "idAngles", "rndAng", (intptr_t)(&((idHomingProjectile *)0)->rndAng), sizeof( ((idHomingProjectile *)0)->rndAng ) },
	{ "idAngles", "angles", (intptr_t)(&((idHomingProjectile *)0)->angles), sizeof( ((idHomingProjectile *)0)->angles ) },
	{ "float", "turn_max", (intptr_t)(&((idHomingProjectile *)0)->turn_max), sizeof( ((idHomingProjectile *)0)->turn_max ) },
	{ "float", "clamp_dist", (intptr_t)(&((idHomingProjectile *)0)->clamp_dist), sizeof( ((idHomingProjectile *)0)->clamp_dist ) },
	{ "bool", "burstMode", (intptr_t)(&((idHomingProjectile *)0)->burstMode), sizeof( ((idHomingProjectile *)0)->burstMode ) },
	{ "bool", "unGuided", (intptr_t)(&((idHomingProjectile *)0)->unGuided), sizeof( ((idHomingProjectile *)0)->unGuided ) },
	{ "float", "burstDist", (intptr_t)(&((idHomingProjectile *)0)->burstDist), sizeof( ((idHomingProjectile *)0)->burstDist ) },
	{ "float", "burstVelocity", (intptr_t)(&((idHomingProjectile *)0)->burstVelocity), sizeof( ((idHomingProjectile *)0)->burstVelocity ) },
	{ NULL, 0 }
};

static classVariableInfo_t idDebris_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "owner", (intptr_t)(&((idDebris *)0)->owner), sizeof( ((idDebris *)0)->owner ) },
	{ "idPhysics_RigidBody", "physicsObj", (intptr_t)(&((idDebris *)0)->physicsObj), sizeof( ((idDebris *)0)->physicsObj ) },
	{ "const idDeclParticle *", "smokeFly", (intptr_t)(&((idDebris *)0)->smokeFly), sizeof( ((idDebris *)0)->smokeFly ) },
	{ "int", "smokeFlyTime", (intptr_t)(&((idDebris *)0)->smokeFlyTime), sizeof( ((idDebris *)0)->smokeFlyTime ) },
	{ "const idSoundShader *", "sndBounce", (intptr_t)(&((idDebris *)0)->sndBounce), sizeof( ((idDebris *)0)->sndBounce ) },
	{ NULL, 0 }
};

static classVariableInfo_t WeaponParticle_t_typeInfo[] = {
	{ "char[64]", "name", (intptr_t)(&((WeaponParticle_t *)0)->name), sizeof( ((WeaponParticle_t *)0)->name ) },
	{ "char[128]", "particlename", (intptr_t)(&((WeaponParticle_t *)0)->particlename), sizeof( ((WeaponParticle_t *)0)->particlename ) },
	{ "bool", "active", (intptr_t)(&((WeaponParticle_t *)0)->active), sizeof( ((WeaponParticle_t *)0)->active ) },
	{ "int", "startTime", (intptr_t)(&((WeaponParticle_t *)0)->startTime), sizeof( ((WeaponParticle_t *)0)->startTime ) },
	{ "jointHandle_t", "joint", (intptr_t)(&((WeaponParticle_t *)0)->joint), sizeof( ((WeaponParticle_t *)0)->joint ) },
	{ "bool", "smoke", (intptr_t)(&((WeaponParticle_t *)0)->smoke), sizeof( ((WeaponParticle_t *)0)->smoke ) },
	{ "const idDeclParticle *", "particle", (intptr_t)(&((WeaponParticle_t *)0)->particle), sizeof( ((WeaponParticle_t *)0)->particle ) },
	{ "idFuncEmitter *", "emitter", (intptr_t)(&((WeaponParticle_t *)0)->emitter), sizeof( ((WeaponParticle_t *)0)->emitter ) },
	{ NULL, 0 }
};

static classVariableInfo_t WeaponLight_t_typeInfo[] = {
	{ "char[64]", "name", (intptr_t)(&((WeaponLight_t *)0)->name), sizeof( ((WeaponLight_t *)0)->name ) },
	{ "bool", "active", (intptr_t)(&((WeaponLight_t *)0)->active), sizeof( ((WeaponLight_t *)0)->active ) },
	{ "int", "startTime", (intptr_t)(&((WeaponLight_t *)0)->startTime), sizeof( ((WeaponLight_t *)0)->startTime ) },
	{ "jointHandle_t", "joint", (intptr_t)(&((WeaponLight_t *)0)->joint), sizeof( ((WeaponLight_t *)0)->joint ) },
	{ "int", "lightHandle", (intptr_t)(&((WeaponLight_t *)0)->lightHandle), sizeof( ((WeaponLight_t *)0)->lightHandle ) },
	{ "renderLight_t", "light", (intptr_t)(&((WeaponLight_t *)0)->light), sizeof( ((WeaponLight_t *)0)->light ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponObject_typeInfo[] = {
	{ ": idWeapon *", "owner", (intptr_t)(&((iceWeaponObject *)0)->owner), sizeof( ((iceWeaponObject *)0)->owner ) },
	{ ": rvStateThread", "stateThread", (intptr_t)(&((iceWeaponObject *)0)->stateThread), sizeof( ((iceWeaponObject *)0)->stateThread ) },
	{ "float", "next_attack", (intptr_t)(&((iceWeaponObject *)0)->next_attack), sizeof( ((iceWeaponObject *)0)->next_attack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idWeapon_typeInfo[] = {
	{ ": int", "animBlendFrames", (intptr_t)(&((idWeapon *)0)->animBlendFrames), sizeof( ((idWeapon *)0)->animBlendFrames ) },
	{ "int", "animDoneTime", (intptr_t)(&((idWeapon *)0)->animDoneTime), sizeof( ((idWeapon *)0)->animDoneTime ) },
	{ "bool", "isLinked", (intptr_t)(&((idWeapon *)0)->isLinked), sizeof( ((idWeapon *)0)->isLinked ) },
	{ "bool", "isPlayerFlashlight", (intptr_t)(&((idWeapon *)0)->isPlayerFlashlight), sizeof( ((idWeapon *)0)->isPlayerFlashlight ) },
	{ "idEntity *", "projectileEnt", (intptr_t)(&((idWeapon *)0)->projectileEnt), sizeof( ((idWeapon *)0)->projectileEnt ) },
	{ "idPlayer *", "owner", (intptr_t)(&((idWeapon *)0)->owner), sizeof( ((idWeapon *)0)->owner ) },
	{ "idEntityPtr < idAnimatedEntity >", "worldModel", (intptr_t)(&((idWeapon *)0)->worldModel), sizeof( ((idWeapon *)0)->worldModel ) },
	{ "int", "hideTime", (intptr_t)(&((idWeapon *)0)->hideTime), sizeof( ((idWeapon *)0)->hideTime ) },
	{ "float", "hideDistance", (intptr_t)(&((idWeapon *)0)->hideDistance), sizeof( ((idWeapon *)0)->hideDistance ) },
	{ "int", "hideStartTime", (intptr_t)(&((idWeapon *)0)->hideStartTime), sizeof( ((idWeapon *)0)->hideStartTime ) },
	{ "float", "hideStart", (intptr_t)(&((idWeapon *)0)->hideStart), sizeof( ((idWeapon *)0)->hideStart ) },
	{ "float", "hideEnd", (intptr_t)(&((idWeapon *)0)->hideEnd), sizeof( ((idWeapon *)0)->hideEnd ) },
	{ "float", "hideOffset", (intptr_t)(&((idWeapon *)0)->hideOffset), sizeof( ((idWeapon *)0)->hideOffset ) },
	{ "bool", "hide", (intptr_t)(&((idWeapon *)0)->hide), sizeof( ((idWeapon *)0)->hide ) },
	{ "bool", "disabled", (intptr_t)(&((idWeapon *)0)->disabled), sizeof( ((idWeapon *)0)->disabled ) },
	{ "bool", "isFlashLight", (intptr_t)(&((idWeapon *)0)->isFlashLight), sizeof( ((idWeapon *)0)->isFlashLight ) },
	{ "int", "berserk", (intptr_t)(&((idWeapon *)0)->berserk), sizeof( ((idWeapon *)0)->berserk ) },
	{ "idVec3", "playerViewOrigin", (intptr_t)(&((idWeapon *)0)->playerViewOrigin), sizeof( ((idWeapon *)0)->playerViewOrigin ) },
	{ "idMat3", "playerViewAxis", (intptr_t)(&((idWeapon *)0)->playerViewAxis), sizeof( ((idWeapon *)0)->playerViewAxis ) },
	{ "idVec3", "viewWeaponOrigin", (intptr_t)(&((idWeapon *)0)->viewWeaponOrigin), sizeof( ((idWeapon *)0)->viewWeaponOrigin ) },
	{ "idMat3", "viewWeaponAxis", (intptr_t)(&((idWeapon *)0)->viewWeaponAxis), sizeof( ((idWeapon *)0)->viewWeaponAxis ) },
	{ "idVec3", "muzzleOrigin", (intptr_t)(&((idWeapon *)0)->muzzleOrigin), sizeof( ((idWeapon *)0)->muzzleOrigin ) },
	{ "idMat3", "muzzleAxis", (intptr_t)(&((idWeapon *)0)->muzzleAxis), sizeof( ((idWeapon *)0)->muzzleAxis ) },
	{ "idVec3", "pushVelocity", (intptr_t)(&((idWeapon *)0)->pushVelocity), sizeof( ((idWeapon *)0)->pushVelocity ) },
	{ "const idDeclEntityDef *", "weaponDef", (intptr_t)(&((idWeapon *)0)->weaponDef), sizeof( ((idWeapon *)0)->weaponDef ) },
	{ "const idDeclEntityDef *", "meleeDef", (intptr_t)(&((idWeapon *)0)->meleeDef), sizeof( ((idWeapon *)0)->meleeDef ) },
	{ "idDict", "projectileDict", (intptr_t)(&((idWeapon *)0)->projectileDict), sizeof( ((idWeapon *)0)->projectileDict ) },
	{ "float", "meleeDistance", (intptr_t)(&((idWeapon *)0)->meleeDistance), sizeof( ((idWeapon *)0)->meleeDistance ) },
	{ "idStr", "meleeDefName", (intptr_t)(&((idWeapon *)0)->meleeDefName), sizeof( ((idWeapon *)0)->meleeDefName ) },
	{ "idDict", "brassDict", (intptr_t)(&((idWeapon *)0)->brassDict), sizeof( ((idWeapon *)0)->brassDict ) },
	{ "int", "brassDelay", (intptr_t)(&((idWeapon *)0)->brassDelay), sizeof( ((idWeapon *)0)->brassDelay ) },
	{ "idStr", "icon", (intptr_t)(&((idWeapon *)0)->icon), sizeof( ((idWeapon *)0)->icon ) },
	{ "idStr", "pdaIcon", (intptr_t)(&((idWeapon *)0)->pdaIcon), sizeof( ((idWeapon *)0)->pdaIcon ) },
	{ "idStr", "displayName", (intptr_t)(&((idWeapon *)0)->displayName), sizeof( ((idWeapon *)0)->displayName ) },
	{ "idStr", "itemDesc", (intptr_t)(&((idWeapon *)0)->itemDesc), sizeof( ((idWeapon *)0)->itemDesc ) },
	{ "renderLight_t", "guiLight", (intptr_t)(&((idWeapon *)0)->guiLight), sizeof( ((idWeapon *)0)->guiLight ) },
	{ "int", "guiLightHandle", (intptr_t)(&((idWeapon *)0)->guiLightHandle), sizeof( ((idWeapon *)0)->guiLightHandle ) },
	{ "renderLight_t", "muzzleFlash", (intptr_t)(&((idWeapon *)0)->muzzleFlash), sizeof( ((idWeapon *)0)->muzzleFlash ) },
	{ "int", "muzzleFlashHandle", (intptr_t)(&((idWeapon *)0)->muzzleFlashHandle), sizeof( ((idWeapon *)0)->muzzleFlashHandle ) },
	{ "renderLight_t", "worldMuzzleFlash", (intptr_t)(&((idWeapon *)0)->worldMuzzleFlash), sizeof( ((idWeapon *)0)->worldMuzzleFlash ) },
	{ "int", "worldMuzzleFlashHandle", (intptr_t)(&((idWeapon *)0)->worldMuzzleFlashHandle), sizeof( ((idWeapon *)0)->worldMuzzleFlashHandle ) },
	{ "float", "fraccos", (intptr_t)(&((idWeapon *)0)->fraccos), sizeof( ((idWeapon *)0)->fraccos ) },
	{ "float", "fraccos2", (intptr_t)(&((idWeapon *)0)->fraccos2), sizeof( ((idWeapon *)0)->fraccos2 ) },
	{ "idVec3", "flashColor", (intptr_t)(&((idWeapon *)0)->flashColor), sizeof( ((idWeapon *)0)->flashColor ) },
	{ "int", "muzzleFlashEnd", (intptr_t)(&((idWeapon *)0)->muzzleFlashEnd), sizeof( ((idWeapon *)0)->muzzleFlashEnd ) },
	{ "int", "flashTime", (intptr_t)(&((idWeapon *)0)->flashTime), sizeof( ((idWeapon *)0)->flashTime ) },
	{ "bool", "lightOn", (intptr_t)(&((idWeapon *)0)->lightOn), sizeof( ((idWeapon *)0)->lightOn ) },
	{ "bool", "silent_fire", (intptr_t)(&((idWeapon *)0)->silent_fire), sizeof( ((idWeapon *)0)->silent_fire ) },
	{ "bool", "allowDrop", (intptr_t)(&((idWeapon *)0)->allowDrop), sizeof( ((idWeapon *)0)->allowDrop ) },
	{ "bool", "hasBloodSplat", (intptr_t)(&((idWeapon *)0)->hasBloodSplat), sizeof( ((idWeapon *)0)->hasBloodSplat ) },
	{ "int", "kick_endtime", (intptr_t)(&((idWeapon *)0)->kick_endtime), sizeof( ((idWeapon *)0)->kick_endtime ) },
	{ "int", "muzzle_kick_time", (intptr_t)(&((idWeapon *)0)->muzzle_kick_time), sizeof( ((idWeapon *)0)->muzzle_kick_time ) },
	{ "int", "muzzle_kick_maxtime", (intptr_t)(&((idWeapon *)0)->muzzle_kick_maxtime), sizeof( ((idWeapon *)0)->muzzle_kick_maxtime ) },
	{ "idAngles", "muzzle_kick_angles", (intptr_t)(&((idWeapon *)0)->muzzle_kick_angles), sizeof( ((idWeapon *)0)->muzzle_kick_angles ) },
	{ "idVec3", "muzzle_kick_offset", (intptr_t)(&((idWeapon *)0)->muzzle_kick_offset), sizeof( ((idWeapon *)0)->muzzle_kick_offset ) },
	{ "ammo_t", "ammoType", (intptr_t)(&((idWeapon *)0)->ammoType), sizeof( ((idWeapon *)0)->ammoType ) },
	{ "int", "ammoRequired", (intptr_t)(&((idWeapon *)0)->ammoRequired), sizeof( ((idWeapon *)0)->ammoRequired ) },
	{ "int", "clipSize", (intptr_t)(&((idWeapon *)0)->clipSize), sizeof( ((idWeapon *)0)->clipSize ) },
	{ "idPredictedValue < int >", "ammoClip", (intptr_t)(&((idWeapon *)0)->ammoClip), sizeof( ((idWeapon *)0)->ammoClip ) },
	{ "int", "lowAmmo", (intptr_t)(&((idWeapon *)0)->lowAmmo), sizeof( ((idWeapon *)0)->lowAmmo ) },
	{ "bool", "powerAmmo", (intptr_t)(&((idWeapon *)0)->powerAmmo), sizeof( ((idWeapon *)0)->powerAmmo ) },
	{ "bool", "isFiring", (intptr_t)(&((idWeapon *)0)->isFiring), sizeof( ((idWeapon *)0)->isFiring ) },
	{ "int", "zoomFov", (intptr_t)(&((idWeapon *)0)->zoomFov), sizeof( ((idWeapon *)0)->zoomFov ) },
	{ "jointHandle_t", "barrelJointView", (intptr_t)(&((idWeapon *)0)->barrelJointView), sizeof( ((idWeapon *)0)->barrelJointView ) },
	{ "jointHandle_t", "flashJointView", (intptr_t)(&((idWeapon *)0)->flashJointView), sizeof( ((idWeapon *)0)->flashJointView ) },
	{ "jointHandle_t", "ejectJointView", (intptr_t)(&((idWeapon *)0)->ejectJointView), sizeof( ((idWeapon *)0)->ejectJointView ) },
	{ "jointHandle_t", "guiLightJointView", (intptr_t)(&((idWeapon *)0)->guiLightJointView), sizeof( ((idWeapon *)0)->guiLightJointView ) },
	{ "jointHandle_t", "ventLightJointView", (intptr_t)(&((idWeapon *)0)->ventLightJointView), sizeof( ((idWeapon *)0)->ventLightJointView ) },
	{ "jointHandle_t", "flashJointWorld", (intptr_t)(&((idWeapon *)0)->flashJointWorld), sizeof( ((idWeapon *)0)->flashJointWorld ) },
	{ "jointHandle_t", "barrelJointWorld", (intptr_t)(&((idWeapon *)0)->barrelJointWorld), sizeof( ((idWeapon *)0)->barrelJointWorld ) },
	{ "jointHandle_t", "ejectJointWorld", (intptr_t)(&((idWeapon *)0)->ejectJointWorld), sizeof( ((idWeapon *)0)->ejectJointWorld ) },
	{ "jointHandle_t", "smokeJointView", (intptr_t)(&((idWeapon *)0)->smokeJointView), sizeof( ((idWeapon *)0)->smokeJointView ) },
	{ "idHashTable < WeaponParticle_t >", "weaponParticles", (intptr_t)(&((idWeapon *)0)->weaponParticles), sizeof( ((idWeapon *)0)->weaponParticles ) },
	{ "idHashTable < WeaponLight_t >", "weaponLights", (intptr_t)(&((idWeapon *)0)->weaponLights), sizeof( ((idWeapon *)0)->weaponLights ) },
	{ "const idSoundShader *", "sndHum", (intptr_t)(&((idWeapon *)0)->sndHum), sizeof( ((idWeapon *)0)->sndHum ) },
	{ "const idDeclParticle *", "weaponSmoke", (intptr_t)(&((idWeapon *)0)->weaponSmoke), sizeof( ((idWeapon *)0)->weaponSmoke ) },
	{ "int", "weaponSmokeStartTime", (intptr_t)(&((idWeapon *)0)->weaponSmokeStartTime), sizeof( ((idWeapon *)0)->weaponSmokeStartTime ) },
	{ "bool", "continuousSmoke", (intptr_t)(&((idWeapon *)0)->continuousSmoke), sizeof( ((idWeapon *)0)->continuousSmoke ) },
	{ "const idDeclParticle *", "strikeSmoke", (intptr_t)(&((idWeapon *)0)->strikeSmoke), sizeof( ((idWeapon *)0)->strikeSmoke ) },
	{ "int", "strikeSmokeStartTime", (intptr_t)(&((idWeapon *)0)->strikeSmokeStartTime), sizeof( ((idWeapon *)0)->strikeSmokeStartTime ) },
	{ "idVec3", "strikePos", (intptr_t)(&((idWeapon *)0)->strikePos), sizeof( ((idWeapon *)0)->strikePos ) },
	{ "idMat3", "strikeAxis", (intptr_t)(&((idWeapon *)0)->strikeAxis), sizeof( ((idWeapon *)0)->strikeAxis ) },
	{ "int", "nextStrikeFx", (intptr_t)(&((idWeapon *)0)->nextStrikeFx), sizeof( ((idWeapon *)0)->nextStrikeFx ) },
	{ "bool", "nozzleFx", (intptr_t)(&((idWeapon *)0)->nozzleFx), sizeof( ((idWeapon *)0)->nozzleFx ) },
	{ "int", "nozzleFxFade", (intptr_t)(&((idWeapon *)0)->nozzleFxFade), sizeof( ((idWeapon *)0)->nozzleFxFade ) },
	{ "int", "lastAttack", (intptr_t)(&((idWeapon *)0)->lastAttack), sizeof( ((idWeapon *)0)->lastAttack ) },
	{ "renderLight_t", "nozzleGlow", (intptr_t)(&((idWeapon *)0)->nozzleGlow), sizeof( ((idWeapon *)0)->nozzleGlow ) },
	{ "int", "nozzleGlowHandle", (intptr_t)(&((idWeapon *)0)->nozzleGlowHandle), sizeof( ((idWeapon *)0)->nozzleGlowHandle ) },
	{ "idVec3", "nozzleGlowColor", (intptr_t)(&((idWeapon *)0)->nozzleGlowColor), sizeof( ((idWeapon *)0)->nozzleGlowColor ) },
	{ "const idMaterial *", "nozzleGlowShader", (intptr_t)(&((idWeapon *)0)->nozzleGlowShader), sizeof( ((idWeapon *)0)->nozzleGlowShader ) },
	{ "float", "nozzleGlowRadius", (intptr_t)(&((idWeapon *)0)->nozzleGlowRadius), sizeof( ((idWeapon *)0)->nozzleGlowRadius ) },
	{ "int", "weaponAngleOffsetAverages", (intptr_t)(&((idWeapon *)0)->weaponAngleOffsetAverages), sizeof( ((idWeapon *)0)->weaponAngleOffsetAverages ) },
	{ "float", "weaponAngleOffsetScale", (intptr_t)(&((idWeapon *)0)->weaponAngleOffsetScale), sizeof( ((idWeapon *)0)->weaponAngleOffsetScale ) },
	{ "float", "weaponAngleOffsetMax", (intptr_t)(&((idWeapon *)0)->weaponAngleOffsetMax), sizeof( ((idWeapon *)0)->weaponAngleOffsetMax ) },
	{ "float", "weaponOffsetTime", (intptr_t)(&((idWeapon *)0)->weaponOffsetTime), sizeof( ((idWeapon *)0)->weaponOffsetTime ) },
	{ "float", "weaponOffsetScale", (intptr_t)(&((idWeapon *)0)->weaponOffsetScale), sizeof( ((idWeapon *)0)->weaponOffsetScale ) },
	{ ": idGrabber", "grabber", (intptr_t)(&((idWeapon *)0)->grabber), sizeof( ((idWeapon *)0)->grabber ) },
	{ "int", "grabberState", (intptr_t)(&((idWeapon *)0)->grabberState), sizeof( ((idWeapon *)0)->grabberState ) },
	{ ": iceWeaponObject *", "currentWeaponObject", (intptr_t)(&((idWeapon *)0)->currentWeaponObject), sizeof( ((idWeapon *)0)->currentWeaponObject ) },
	{ "bool", "OutOfAmmo", (intptr_t)(&((idWeapon *)0)->OutOfAmmo), sizeof( ((idWeapon *)0)->OutOfAmmo ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceLightStyleState_t_typeInfo[] = {
	{ "int", "dl_frame", (intptr_t)(&((iceLightStyleState_t *)0)->dl_frame), sizeof( ((iceLightStyleState_t *)0)->dl_frame ) },
	{ "float", "dl_framef", (intptr_t)(&((iceLightStyleState_t *)0)->dl_framef), sizeof( ((iceLightStyleState_t *)0)->dl_framef ) },
	{ "int", "dl_oldframe", (intptr_t)(&((iceLightStyleState_t *)0)->dl_oldframe), sizeof( ((iceLightStyleState_t *)0)->dl_oldframe ) },
	{ "int", "dl_time", (intptr_t)(&((iceLightStyleState_t *)0)->dl_time), sizeof( ((iceLightStyleState_t *)0)->dl_time ) },
	{ "float", "dl_backlerp", (intptr_t)(&((iceLightStyleState_t *)0)->dl_backlerp), sizeof( ((iceLightStyleState_t *)0)->dl_backlerp ) },
	{ NULL, 0 }
};

static classVariableInfo_t idLight_typeInfo[] = {
	{ ": renderLight_t", "renderLight", (intptr_t)(&((idLight *)0)->renderLight), sizeof( ((idLight *)0)->renderLight ) },
	{ "idVec3", "localLightOrigin", (intptr_t)(&((idLight *)0)->localLightOrigin), sizeof( ((idLight *)0)->localLightOrigin ) },
	{ "idMat3", "localLightAxis", (intptr_t)(&((idLight *)0)->localLightAxis), sizeof( ((idLight *)0)->localLightAxis ) },
	{ "qhandle_t", "lightDefHandle", (intptr_t)(&((idLight *)0)->lightDefHandle), sizeof( ((idLight *)0)->lightDefHandle ) },
	{ "idStr", "brokenModel", (intptr_t)(&((idLight *)0)->brokenModel), sizeof( ((idLight *)0)->brokenModel ) },
	{ "int", "levels", (intptr_t)(&((idLight *)0)->levels), sizeof( ((idLight *)0)->levels ) },
	{ "int", "currentLevel", (intptr_t)(&((idLight *)0)->currentLevel), sizeof( ((idLight *)0)->currentLevel ) },
	{ "idVec3", "baseColor", (intptr_t)(&((idLight *)0)->baseColor), sizeof( ((idLight *)0)->baseColor ) },
	{ "idVec3", "previousBaseColor", (intptr_t)(&((idLight *)0)->previousBaseColor), sizeof( ((idLight *)0)->previousBaseColor ) },
	{ "idVec3", "nextBaseColor", (intptr_t)(&((idLight *)0)->nextBaseColor), sizeof( ((idLight *)0)->nextBaseColor ) },
	{ "bool", "breakOnTrigger", (intptr_t)(&((idLight *)0)->breakOnTrigger), sizeof( ((idLight *)0)->breakOnTrigger ) },
	{ "int", "count", (intptr_t)(&((idLight *)0)->count), sizeof( ((idLight *)0)->count ) },
	{ "int", "lightStyle", (intptr_t)(&((idLight *)0)->lightStyle), sizeof( ((idLight *)0)->lightStyle ) },
	{ "int", "lightStyleFrameTime", (intptr_t)(&((idLight *)0)->lightStyleFrameTime), sizeof( ((idLight *)0)->lightStyleFrameTime ) },
	{ "idVec3", "lightStyleBase", (intptr_t)(&((idLight *)0)->lightStyleBase), sizeof( ((idLight *)0)->lightStyleBase ) },
	{ "int", "triggercount", (intptr_t)(&((idLight *)0)->triggercount), sizeof( ((idLight *)0)->triggercount ) },
	{ "idEntity *", "lightParent", (intptr_t)(&((idLight *)0)->lightParent), sizeof( ((idLight *)0)->lightParent ) },
	{ "idVec4", "fadeFrom", (intptr_t)(&((idLight *)0)->fadeFrom), sizeof( ((idLight *)0)->fadeFrom ) },
	{ "idVec4", "fadeTo", (intptr_t)(&((idLight *)0)->fadeTo), sizeof( ((idLight *)0)->fadeTo ) },
	{ "int", "fadeStart", (intptr_t)(&((idLight *)0)->fadeStart), sizeof( ((idLight *)0)->fadeStart ) },
	{ "int", "fadeEnd", (intptr_t)(&((idLight *)0)->fadeEnd), sizeof( ((idLight *)0)->fadeEnd ) },
	{ "bool", "soundWasPlaying", (intptr_t)(&((idLight *)0)->soundWasPlaying), sizeof( ((idLight *)0)->soundWasPlaying ) },
	{ "idEntityPtr < idStaticEntity >", "modelTarget", (intptr_t)(&((idLight *)0)->modelTarget), sizeof( ((idLight *)0)->modelTarget ) },
	{ "idList < idStr >", "light_styles", (intptr_t)(&((idLight *)0)->light_styles), sizeof( ((idLight *)0)->light_styles ) },
	{ "iceLightStyleState_t", "lightStyleState", (intptr_t)(&((idLight *)0)->lightStyleState), sizeof( ((idLight *)0)->lightStyleState ) },
	{ NULL, 0 }
};

static classVariableInfo_t EnvironmentProbe_typeInfo[] = {
	{ ": renderEnvironmentProbe_t", "renderEnvprobe", (intptr_t)(&((EnvironmentProbe *)0)->renderEnvprobe), sizeof( ((EnvironmentProbe *)0)->renderEnvprobe ) },
	{ "idVec3", "localEnvprobeOrigin", (intptr_t)(&((EnvironmentProbe *)0)->localEnvprobeOrigin), sizeof( ((EnvironmentProbe *)0)->localEnvprobeOrigin ) },
	{ "idMat3", "localEnvprobeAxis", (intptr_t)(&((EnvironmentProbe *)0)->localEnvprobeAxis), sizeof( ((EnvironmentProbe *)0)->localEnvprobeAxis ) },
	{ "qhandle_t", "envprobeDefHandle", (intptr_t)(&((EnvironmentProbe *)0)->envprobeDefHandle), sizeof( ((EnvironmentProbe *)0)->envprobeDefHandle ) },
	{ "int", "levels", (intptr_t)(&((EnvironmentProbe *)0)->levels), sizeof( ((EnvironmentProbe *)0)->levels ) },
	{ "int", "currentLevel", (intptr_t)(&((EnvironmentProbe *)0)->currentLevel), sizeof( ((EnvironmentProbe *)0)->currentLevel ) },
	{ "idVec3", "baseColor", (intptr_t)(&((EnvironmentProbe *)0)->baseColor), sizeof( ((EnvironmentProbe *)0)->baseColor ) },
	{ "idVec3", "previousBaseColor", (intptr_t)(&((EnvironmentProbe *)0)->previousBaseColor), sizeof( ((EnvironmentProbe *)0)->previousBaseColor ) },
	{ "idVec3", "nextBaseColor", (intptr_t)(&((EnvironmentProbe *)0)->nextBaseColor), sizeof( ((EnvironmentProbe *)0)->nextBaseColor ) },
	{ "int", "count", (intptr_t)(&((EnvironmentProbe *)0)->count), sizeof( ((EnvironmentProbe *)0)->count ) },
	{ "int", "triggercount", (intptr_t)(&((EnvironmentProbe *)0)->triggercount), sizeof( ((EnvironmentProbe *)0)->triggercount ) },
	{ "idEntity *", "lightParent", (intptr_t)(&((EnvironmentProbe *)0)->lightParent), sizeof( ((EnvironmentProbe *)0)->lightParent ) },
	{ "idVec4", "fadeFrom", (intptr_t)(&((EnvironmentProbe *)0)->fadeFrom), sizeof( ((EnvironmentProbe *)0)->fadeFrom ) },
	{ "idVec4", "fadeTo", (intptr_t)(&((EnvironmentProbe *)0)->fadeTo), sizeof( ((EnvironmentProbe *)0)->fadeTo ) },
	{ "int", "fadeStart", (intptr_t)(&((EnvironmentProbe *)0)->fadeStart), sizeof( ((EnvironmentProbe *)0)->fadeStart ) },
	{ "int", "fadeEnd", (intptr_t)(&((EnvironmentProbe *)0)->fadeEnd), sizeof( ((EnvironmentProbe *)0)->fadeEnd ) },
	{ NULL, 0 }
};

static classVariableInfo_t idWorldspawn_typeInfo[] = {
	{ "idStr", "musicTrack", (intptr_t)(&((idWorldspawn *)0)->musicTrack), sizeof( ((idWorldspawn *)0)->musicTrack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idItem_typeInfo[] = {
	{ ": idVec3", "orgOrigin", (intptr_t)(&((idItem *)0)->orgOrigin), sizeof( ((idItem *)0)->orgOrigin ) },
	{ "bool", "spin", (intptr_t)(&((idItem *)0)->spin), sizeof( ((idItem *)0)->spin ) },
	{ "bool", "pulse", (intptr_t)(&((idItem *)0)->pulse), sizeof( ((idItem *)0)->pulse ) },
	{ "bool", "canPickUp", (intptr_t)(&((idItem *)0)->canPickUp), sizeof( ((idItem *)0)->canPickUp ) },
	{ "int", "modelindex", (intptr_t)(&((idItem *)0)->modelindex), sizeof( ((idItem *)0)->modelindex ) },
	{ "int", "itemShellHandle", (intptr_t)(&((idItem *)0)->itemShellHandle), sizeof( ((idItem *)0)->itemShellHandle ) },
	{ "const idMaterial *", "shellMaterial", (intptr_t)(&((idItem *)0)->shellMaterial), sizeof( ((idItem *)0)->shellMaterial ) },
	{ "mutable bool", "inView", (intptr_t)(&((idItem *)0)->inView), sizeof( ((idItem *)0)->inView ) },
	{ "mutable int", "inViewTime", (intptr_t)(&((idItem *)0)->inViewTime), sizeof( ((idItem *)0)->inViewTime ) },
	{ "mutable int", "lastCycle", (intptr_t)(&((idItem *)0)->lastCycle), sizeof( ((idItem *)0)->lastCycle ) },
	{ "mutable int", "lastRenderViewTime", (intptr_t)(&((idItem *)0)->lastRenderViewTime), sizeof( ((idItem *)0)->lastRenderViewTime ) },
	{ "int", "clientPredictPickupMilliseconds", (intptr_t)(&((idItem *)0)->clientPredictPickupMilliseconds), sizeof( ((idItem *)0)->clientPredictPickupMilliseconds ) },
	{ NULL, 0 }
};

static classVariableInfo_t idItemPowerup_typeInfo[] = {
	{ ": int", "time", (intptr_t)(&((idItemPowerup *)0)->time), sizeof( ((idItemPowerup *)0)->time ) },
	{ "int", "type", (intptr_t)(&((idItemPowerup *)0)->type), sizeof( ((idItemPowerup *)0)->type ) },
	{ NULL, 0 }
};

static classVariableInfo_t idObjective_typeInfo[] = {
	{ ": idVec3", "playerPos", (intptr_t)(&((idObjective *)0)->playerPos), sizeof( ((idObjective *)0)->playerPos ) },
	{ "const idMaterial *", "screenshot", (intptr_t)(&((idObjective *)0)->screenshot), sizeof( ((idObjective *)0)->screenshot ) },
	{ NULL, 0 }
};

static classVariableInfo_t idVideoCDItem_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idPDAItem_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idMoveableItem_typeInfo[] = {
	{ ": idPhysics_RigidBody", "physicsObj", (intptr_t)(&((idMoveableItem *)0)->physicsObj), sizeof( ((idMoveableItem *)0)->physicsObj ) },
	{ "idClipModel *", "trigger", (intptr_t)(&((idMoveableItem *)0)->trigger), sizeof( ((idMoveableItem *)0)->trigger ) },
	{ "const idDeclParticle *", "smoke", (intptr_t)(&((idMoveableItem *)0)->smoke), sizeof( ((idMoveableItem *)0)->smoke ) },
	{ "int", "smokeTime", (intptr_t)(&((idMoveableItem *)0)->smokeTime), sizeof( ((idMoveableItem *)0)->smokeTime ) },
	{ "int", "nextSoundTime", (intptr_t)(&((idMoveableItem *)0)->nextSoundTime), sizeof( ((idMoveableItem *)0)->nextSoundTime ) },
	{ "bool", "repeatSmoke", (intptr_t)(&((idMoveableItem *)0)->repeatSmoke), sizeof( ((idMoveableItem *)0)->repeatSmoke ) },
	{ NULL, 0 }
};

static classVariableInfo_t idItemTeam_typeInfo[] = {
	{ ": int", "team", (intptr_t)(&((idItemTeam *)0)->team), sizeof( ((idItemTeam *)0)->team ) },
	{ "bool", "carried", (intptr_t)(&((idItemTeam *)0)->carried), sizeof( ((idItemTeam *)0)->carried ) },
	{ "bool", "dropped", (intptr_t)(&((idItemTeam *)0)->dropped), sizeof( ((idItemTeam *)0)->dropped ) },
	{ ": idVec3", "returnOrigin", (intptr_t)(&((idItemTeam *)0)->returnOrigin), sizeof( ((idItemTeam *)0)->returnOrigin ) },
	{ "idMat3", "returnAxis", (intptr_t)(&((idItemTeam *)0)->returnAxis), sizeof( ((idItemTeam *)0)->returnAxis ) },
	{ "int", "lastDrop", (intptr_t)(&((idItemTeam *)0)->lastDrop), sizeof( ((idItemTeam *)0)->lastDrop ) },
	{ "const idDeclSkin *", "skinDefault", (intptr_t)(&((idItemTeam *)0)->skinDefault), sizeof( ((idItemTeam *)0)->skinDefault ) },
	{ "const idDeclSkin *", "skinCarried", (intptr_t)(&((idItemTeam *)0)->skinCarried), sizeof( ((idItemTeam *)0)->skinCarried ) },
	{ "const function_t *", "scriptTaken", (intptr_t)(&((idItemTeam *)0)->scriptTaken), sizeof( ((idItemTeam *)0)->scriptTaken ) },
	{ "const function_t *", "scriptDropped", (intptr_t)(&((idItemTeam *)0)->scriptDropped), sizeof( ((idItemTeam *)0)->scriptDropped ) },
	{ "const function_t *", "scriptReturned", (intptr_t)(&((idItemTeam *)0)->scriptReturned), sizeof( ((idItemTeam *)0)->scriptReturned ) },
	{ "const function_t *", "scriptCaptured", (intptr_t)(&((idItemTeam *)0)->scriptCaptured), sizeof( ((idItemTeam *)0)->scriptCaptured ) },
	{ "renderLight_t", "itemGlow", (intptr_t)(&((idItemTeam *)0)->itemGlow), sizeof( ((idItemTeam *)0)->itemGlow ) },
	{ "int", "itemGlowHandle", (intptr_t)(&((idItemTeam *)0)->itemGlowHandle), sizeof( ((idItemTeam *)0)->itemGlowHandle ) },
	{ "int", "lastNuggetDrop", (intptr_t)(&((idItemTeam *)0)->lastNuggetDrop), sizeof( ((idItemTeam *)0)->lastNuggetDrop ) },
	{ "const char *", "nuggetName", (intptr_t)(&((idItemTeam *)0)->nuggetName), sizeof( ((idItemTeam *)0)->nuggetName ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMoveablePDAItem_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idItemRemover_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idObjectiveComplete_typeInfo[] = {
	{ ": idVec3", "playerPos", (intptr_t)(&((idObjectiveComplete *)0)->playerPos), sizeof( ((idObjectiveComplete *)0)->playerPos ) },
	{ NULL, 0 }
};

static classVariableInfo_t screenBlob_t_typeInfo[] = {
	{ "const idMaterial *", "material", (intptr_t)(&((screenBlob_t *)0)->material), sizeof( ((screenBlob_t *)0)->material ) },
	{ "float", "x", (intptr_t)(&((screenBlob_t *)0)->x), sizeof( ((screenBlob_t *)0)->x ) },
	{ "float", "y", (intptr_t)(&((screenBlob_t *)0)->y), sizeof( ((screenBlob_t *)0)->y ) },
	{ "float", "w", (intptr_t)(&((screenBlob_t *)0)->w), sizeof( ((screenBlob_t *)0)->w ) },
	{ "float", "h", (intptr_t)(&((screenBlob_t *)0)->h), sizeof( ((screenBlob_t *)0)->h ) },
	{ "float", "s1", (intptr_t)(&((screenBlob_t *)0)->s1), sizeof( ((screenBlob_t *)0)->s1 ) },
	{ "float", "t1", (intptr_t)(&((screenBlob_t *)0)->t1), sizeof( ((screenBlob_t *)0)->t1 ) },
	{ "float", "s2", (intptr_t)(&((screenBlob_t *)0)->s2), sizeof( ((screenBlob_t *)0)->s2 ) },
	{ "float", "t2", (intptr_t)(&((screenBlob_t *)0)->t2), sizeof( ((screenBlob_t *)0)->t2 ) },
	{ "int", "finishTime", (intptr_t)(&((screenBlob_t *)0)->finishTime), sizeof( ((screenBlob_t *)0)->finishTime ) },
	{ "int", "startFadeTime", (intptr_t)(&((screenBlob_t *)0)->startFadeTime), sizeof( ((screenBlob_t *)0)->startFadeTime ) },
	{ "float", "driftAmount", (intptr_t)(&((screenBlob_t *)0)->driftAmount), sizeof( ((screenBlob_t *)0)->driftAmount ) },
	{ NULL, 0 }
};

static classVariableInfo_t WarpPolygon_t_typeInfo[] = {
	{ ": idVec4", "outer1", (intptr_t)(&((WarpPolygon_t *)0)->outer1), sizeof( ((WarpPolygon_t *)0)->outer1 ) },
	{ "idVec4", "outer2", (intptr_t)(&((WarpPolygon_t *)0)->outer2), sizeof( ((WarpPolygon_t *)0)->outer2 ) },
	{ "idVec4", "center", (intptr_t)(&((WarpPolygon_t *)0)->center), sizeof( ((WarpPolygon_t *)0)->center ) },
	{ NULL, 0 }
};

static classVariableInfo_t Warp_t_typeInfo[] = {
	{ ": int", "id", (intptr_t)(&((Warp_t *)0)->id), sizeof( ((Warp_t *)0)->id ) },
	{ "bool", "active", (intptr_t)(&((Warp_t *)0)->active), sizeof( ((Warp_t *)0)->active ) },
	{ "int", "startTime", (intptr_t)(&((Warp_t *)0)->startTime), sizeof( ((Warp_t *)0)->startTime ) },
	{ "float", "initialRadius", (intptr_t)(&((Warp_t *)0)->initialRadius), sizeof( ((Warp_t *)0)->initialRadius ) },
	{ "idVec3", "worldOrigin", (intptr_t)(&((Warp_t *)0)->worldOrigin), sizeof( ((Warp_t *)0)->worldOrigin ) },
	{ "idVec2", "screenOrigin", (intptr_t)(&((Warp_t *)0)->screenOrigin), sizeof( ((Warp_t *)0)->screenOrigin ) },
	{ "int", "durationMsec", (intptr_t)(&((Warp_t *)0)->durationMsec), sizeof( ((Warp_t *)0)->durationMsec ) },
	{ "idList < WarpPolygon_t , TAG_IDLIB_LIST_PLAYER >", "polys", (intptr_t)(&((Warp_t *)0)->polys), sizeof( ((Warp_t *)0)->polys ) },
	{ NULL, 0 }
};

static classVariableInfo_t FxFader_typeInfo[] = {
	{ "int", "time", (intptr_t)(&((FxFader *)0)->time), sizeof( ((FxFader *)0)->time ) },
	{ "int", "state", (intptr_t)(&((FxFader *)0)->state), sizeof( ((FxFader *)0)->state ) },
	{ "float", "alpha", (intptr_t)(&((FxFader *)0)->alpha), sizeof( ((FxFader *)0)->alpha ) },
	{ "int", "msec", (intptr_t)(&((FxFader *)0)->msec), sizeof( ((FxFader *)0)->msec ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_typeInfo[] = {
	{ ": idStr", "name", (intptr_t)(&((FullscreenFX *)0)->name), sizeof( ((FullscreenFX *)0)->name ) },
	{ "FxFader", "fader", (intptr_t)(&((FullscreenFX *)0)->fader), sizeof( ((FullscreenFX *)0)->fader ) },
	{ "FullscreenFXManager *", "fxman", (intptr_t)(&((FullscreenFX *)0)->fxman), sizeof( ((FullscreenFX *)0)->fxman ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_Helltime_typeInfo[] = {
	{ ": const idMaterial *", "initMaterial", (intptr_t)(&((FullscreenFX_Helltime *)0)->initMaterial), sizeof( ((FullscreenFX_Helltime *)0)->initMaterial ) },
	{ "const idMaterial *[3]", "captureMaterials", (intptr_t)(&((FullscreenFX_Helltime *)0)->captureMaterials), sizeof( ((FullscreenFX_Helltime *)0)->captureMaterials ) },
	{ "const idMaterial *", "drawMaterial", (intptr_t)(&((FullscreenFX_Helltime *)0)->drawMaterial), sizeof( ((FullscreenFX_Helltime *)0)->drawMaterial ) },
	{ "bool", "clearAccumBuffer", (intptr_t)(&((FullscreenFX_Helltime *)0)->clearAccumBuffer), sizeof( ((FullscreenFX_Helltime *)0)->clearAccumBuffer ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_Multiplayer_typeInfo[] = {
	{ ": const idMaterial *", "initMaterial", (intptr_t)(&((FullscreenFX_Multiplayer *)0)->initMaterial), sizeof( ((FullscreenFX_Multiplayer *)0)->initMaterial ) },
	{ "const idMaterial *", "captureMaterial", (intptr_t)(&((FullscreenFX_Multiplayer *)0)->captureMaterial), sizeof( ((FullscreenFX_Multiplayer *)0)->captureMaterial ) },
	{ "const idMaterial *", "drawMaterial", (intptr_t)(&((FullscreenFX_Multiplayer *)0)->drawMaterial), sizeof( ((FullscreenFX_Multiplayer *)0)->drawMaterial ) },
	{ "bool", "clearAccumBuffer", (intptr_t)(&((FullscreenFX_Multiplayer *)0)->clearAccumBuffer), sizeof( ((FullscreenFX_Multiplayer *)0)->clearAccumBuffer ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_Warp_typeInfo[] = {
	{ ": const idMaterial *", "material", (intptr_t)(&((FullscreenFX_Warp *)0)->material), sizeof( ((FullscreenFX_Warp *)0)->material ) },
	{ "bool", "grabberEnabled", (intptr_t)(&((FullscreenFX_Warp *)0)->grabberEnabled), sizeof( ((FullscreenFX_Warp *)0)->grabberEnabled ) },
	{ "int", "startWarpTime", (intptr_t)(&((FullscreenFX_Warp *)0)->startWarpTime), sizeof( ((FullscreenFX_Warp *)0)->startWarpTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_EnviroSuit_typeInfo[] = {
	{ ": const idMaterial *", "material", (intptr_t)(&((FullscreenFX_EnviroSuit *)0)->material), sizeof( ((FullscreenFX_EnviroSuit *)0)->material ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_DoubleVision_typeInfo[] = {
	{ ": const idMaterial *", "material", (intptr_t)(&((FullscreenFX_DoubleVision *)0)->material), sizeof( ((FullscreenFX_DoubleVision *)0)->material ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_InfluenceVision_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFX_Bloom_typeInfo[] = {
	{ ": const idMaterial *", "drawMaterial", (intptr_t)(&((FullscreenFX_Bloom *)0)->drawMaterial), sizeof( ((FullscreenFX_Bloom *)0)->drawMaterial ) },
	{ "const idMaterial *", "initMaterial", (intptr_t)(&((FullscreenFX_Bloom *)0)->initMaterial), sizeof( ((FullscreenFX_Bloom *)0)->initMaterial ) },
	{ "float", "currentIntensity", (intptr_t)(&((FullscreenFX_Bloom *)0)->currentIntensity), sizeof( ((FullscreenFX_Bloom *)0)->currentIntensity ) },
	{ "float", "targetIntensity", (intptr_t)(&((FullscreenFX_Bloom *)0)->targetIntensity), sizeof( ((FullscreenFX_Bloom *)0)->targetIntensity ) },
	{ NULL, 0 }
};

static classVariableInfo_t FullscreenFXManager_typeInfo[] = {
	{ ": idList < FullscreenFX * , TAG_FX >", "fx", (intptr_t)(&((FullscreenFXManager *)0)->fx), sizeof( ((FullscreenFXManager *)0)->fx ) },
	{ "idPlayerView *", "playerView", (intptr_t)(&((FullscreenFXManager *)0)->playerView), sizeof( ((FullscreenFXManager *)0)->playerView ) },
	{ "const idMaterial *", "blendBackMaterial", (intptr_t)(&((FullscreenFXManager *)0)->blendBackMaterial), sizeof( ((FullscreenFXManager *)0)->blendBackMaterial ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPlayerView_typeInfo[] = {
	{ "screenBlob_t[8]", "screenBlobs", (intptr_t)(&((idPlayerView *)0)->screenBlobs), sizeof( ((idPlayerView *)0)->screenBlobs ) },
	{ ": int", "dvFinishTime", (intptr_t)(&((idPlayerView *)0)->dvFinishTime), sizeof( ((idPlayerView *)0)->dvFinishTime ) },
	{ "int", "kickFinishTime", (intptr_t)(&((idPlayerView *)0)->kickFinishTime), sizeof( ((idPlayerView *)0)->kickFinishTime ) },
	{ "idAngles", "kickAngles", (intptr_t)(&((idPlayerView *)0)->kickAngles), sizeof( ((idPlayerView *)0)->kickAngles ) },
	{ "bool", "bfgVision", (intptr_t)(&((idPlayerView *)0)->bfgVision), sizeof( ((idPlayerView *)0)->bfgVision ) },
	{ "const idMaterial *", "tunnelMaterial", (intptr_t)(&((idPlayerView *)0)->tunnelMaterial), sizeof( ((idPlayerView *)0)->tunnelMaterial ) },
	{ "const idMaterial *", "armorMaterial", (intptr_t)(&((idPlayerView *)0)->armorMaterial), sizeof( ((idPlayerView *)0)->armorMaterial ) },
	{ "const idMaterial *", "berserkMaterial", (intptr_t)(&((idPlayerView *)0)->berserkMaterial), sizeof( ((idPlayerView *)0)->berserkMaterial ) },
	{ "const idMaterial *", "irGogglesMaterial", (intptr_t)(&((idPlayerView *)0)->irGogglesMaterial), sizeof( ((idPlayerView *)0)->irGogglesMaterial ) },
	{ "const idMaterial *", "bloodSprayMaterial", (intptr_t)(&((idPlayerView *)0)->bloodSprayMaterial), sizeof( ((idPlayerView *)0)->bloodSprayMaterial ) },
	{ "const idMaterial *", "bfgMaterial", (intptr_t)(&((idPlayerView *)0)->bfgMaterial), sizeof( ((idPlayerView *)0)->bfgMaterial ) },
	{ "float", "lastDamageTime", (intptr_t)(&((idPlayerView *)0)->lastDamageTime), sizeof( ((idPlayerView *)0)->lastDamageTime ) },
	{ "idVec4", "fadeColor", (intptr_t)(&((idPlayerView *)0)->fadeColor), sizeof( ((idPlayerView *)0)->fadeColor ) },
	{ "idVec4", "fadeToColor", (intptr_t)(&((idPlayerView *)0)->fadeToColor), sizeof( ((idPlayerView *)0)->fadeToColor ) },
	{ "idVec4", "fadeFromColor", (intptr_t)(&((idPlayerView *)0)->fadeFromColor), sizeof( ((idPlayerView *)0)->fadeFromColor ) },
	{ "float", "fadeRate", (intptr_t)(&((idPlayerView *)0)->fadeRate), sizeof( ((idPlayerView *)0)->fadeRate ) },
	{ "int", "fadeTime", (intptr_t)(&((idPlayerView *)0)->fadeTime), sizeof( ((idPlayerView *)0)->fadeTime ) },
	{ "idAngles", "shakeAng", (intptr_t)(&((idPlayerView *)0)->shakeAng), sizeof( ((idPlayerView *)0)->shakeAng ) },
	{ "idPlayer *", "player", (intptr_t)(&((idPlayerView *)0)->player), sizeof( ((idPlayerView *)0)->player ) },
	{ "renderView_t", "view", (intptr_t)(&((idPlayerView *)0)->view), sizeof( ((idPlayerView *)0)->view ) },
	{ "FullscreenFXManager *", "fxManager", (intptr_t)(&((idPlayerView *)0)->fxManager), sizeof( ((idPlayerView *)0)->fxManager ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPlayerIcon_typeInfo[] = {
	{ ": playerIconType_t", "iconType", (intptr_t)(&((idPlayerIcon *)0)->iconType), sizeof( ((idPlayerIcon *)0)->iconType ) },
	{ "renderEntity_t", "renderEnt", (intptr_t)(&((idPlayerIcon *)0)->renderEnt), sizeof( ((idPlayerIcon *)0)->renderEnt ) },
	{ "qhandle_t", "iconHandle", (intptr_t)(&((idPlayerIcon *)0)->iconHandle), sizeof( ((idPlayerIcon *)0)->iconHandle ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAchievementManager_typeInfo[] = {
	{ ": idEntityPtr < idPlayer >", "owner", (intptr_t)(&((idAchievementManager *)0)->owner), sizeof( ((idAchievementManager *)0)->owner ) },
	{ "idArray < int , ACHIEVEMENTS_NUM >", "counts", (intptr_t)(&((idAchievementManager *)0)->counts), sizeof( ((idAchievementManager *)0)->counts ) },
	{ "int", "lastPlayerKilledTime", (intptr_t)(&((idAchievementManager *)0)->lastPlayerKilledTime), sizeof( ((idAchievementManager *)0)->lastPlayerKilledTime ) },
	{ "int", "lastImpKilledTime", (intptr_t)(&((idAchievementManager *)0)->lastImpKilledTime), sizeof( ((idAchievementManager *)0)->lastImpKilledTime ) },
	{ "bool", "playerTookDamage", (intptr_t)(&((idAchievementManager *)0)->playerTookDamage), sizeof( ((idAchievementManager *)0)->playerTookDamage ) },
	{ "int", "currentHellTimeKills", (intptr_t)(&((idAchievementManager *)0)->currentHellTimeKills), sizeof( ((idAchievementManager *)0)->currentHellTimeKills ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAimAssist_typeInfo[] = {
	{ "idPlayer *", "player", (intptr_t)(&((idAimAssist *)0)->player), sizeof( ((idAimAssist *)0)->player ) },
	{ "idAngles", "angleCorrection", (intptr_t)(&((idAimAssist *)0)->angleCorrection), sizeof( ((idAimAssist *)0)->angleCorrection ) },
	{ "float", "frictionScalar", (intptr_t)(&((idAimAssist *)0)->frictionScalar), sizeof( ((idAimAssist *)0)->frictionScalar ) },
	{ "idEntityPtr < idEntity >", "targetEntity", (intptr_t)(&((idAimAssist *)0)->targetEntity), sizeof( ((idAimAssist *)0)->targetEntity ) },
	{ "idVec3", "lastTargetPos", (intptr_t)(&((idAimAssist *)0)->lastTargetPos), sizeof( ((idAimAssist *)0)->lastTargetPos ) },
	{ NULL, 0 }
};

static classVariableInfo_t idObjectiveInfo_typeInfo[] = {
	{ "idStr", "title", (intptr_t)(&((idObjectiveInfo *)0)->title), sizeof( ((idObjectiveInfo *)0)->title ) },
	{ "idStr", "text", (intptr_t)(&((idObjectiveInfo *)0)->text), sizeof( ((idObjectiveInfo *)0)->text ) },
	{ "const idMaterial *", "screenshot", (intptr_t)(&((idObjectiveInfo *)0)->screenshot), sizeof( ((idObjectiveInfo *)0)->screenshot ) },
	{ NULL, 0 }
};

static classVariableInfo_t idLevelTriggerInfo_typeInfo[] = {
	{ "idStr", "levelName", (intptr_t)(&((idLevelTriggerInfo *)0)->levelName), sizeof( ((idLevelTriggerInfo *)0)->levelName ) },
	{ "idStr", "triggerName", (intptr_t)(&((idLevelTriggerInfo *)0)->triggerName), sizeof( ((idLevelTriggerInfo *)0)->triggerName ) },
	{ NULL, 0 }
};

static classVariableInfo_t RechargeAmmo_t_typeInfo[] = {
	{ "int", "ammo", (intptr_t)(&((RechargeAmmo_t *)0)->ammo), sizeof( ((RechargeAmmo_t *)0)->ammo ) },
	{ "int", "rechargeTime", (intptr_t)(&((RechargeAmmo_t *)0)->rechargeTime), sizeof( ((RechargeAmmo_t *)0)->rechargeTime ) },
	{ "char[128]", "ammoName", (intptr_t)(&((RechargeAmmo_t *)0)->ammoName), sizeof( ((RechargeAmmo_t *)0)->ammoName ) },
	{ NULL, 0 }
};

static classVariableInfo_t WeaponToggle_t_typeInfo[] = {
	{ "char[64]", "name", (intptr_t)(&((WeaponToggle_t *)0)->name), sizeof( ((WeaponToggle_t *)0)->name ) },
	{ "idList < int , TAG_IDLIB_LIST_PLAYER >", "toggleList", (intptr_t)(&((WeaponToggle_t *)0)->toggleList), sizeof( ((WeaponToggle_t *)0)->toggleList ) },
	{ "int", "lastUsed", (intptr_t)(&((WeaponToggle_t *)0)->lastUsed), sizeof( ((WeaponToggle_t *)0)->lastUsed ) },
	{ NULL, 0 }
};

static classVariableInfo_t idInventory_typeInfo[] = {
	{ ": int", "maxHealth", (intptr_t)(&((idInventory *)0)->maxHealth), sizeof( ((idInventory *)0)->maxHealth ) },
	{ "int", "weapons", (intptr_t)(&((idInventory *)0)->weapons), sizeof( ((idInventory *)0)->weapons ) },
	{ "int", "powerups", (intptr_t)(&((idInventory *)0)->powerups), sizeof( ((idInventory *)0)->powerups ) },
	{ "int", "armor", (intptr_t)(&((idInventory *)0)->armor), sizeof( ((idInventory *)0)->armor ) },
	{ "int", "maxarmor", (intptr_t)(&((idInventory *)0)->maxarmor), sizeof( ((idInventory *)0)->maxarmor ) },
	{ "int[8]", "powerupEndTime", (intptr_t)(&((idInventory *)0)->powerupEndTime), sizeof( ((idInventory *)0)->powerupEndTime ) },
	{ "RechargeAmmo_t[16]", "rechargeAmmo", (intptr_t)(&((idInventory *)0)->rechargeAmmo), sizeof( ((idInventory *)0)->rechargeAmmo ) },
	{ "int", "ammoPredictTime", (intptr_t)(&((idInventory *)0)->ammoPredictTime), sizeof( ((idInventory *)0)->ammoPredictTime ) },
	{ "int", "deplete_armor", (intptr_t)(&((idInventory *)0)->deplete_armor), sizeof( ((idInventory *)0)->deplete_armor ) },
	{ "float", "deplete_rate", (intptr_t)(&((idInventory *)0)->deplete_rate), sizeof( ((idInventory *)0)->deplete_rate ) },
	{ "int", "deplete_ammount", (intptr_t)(&((idInventory *)0)->deplete_ammount), sizeof( ((idInventory *)0)->deplete_ammount ) },
	{ "int", "nextArmorDepleteTime", (intptr_t)(&((idInventory *)0)->nextArmorDepleteTime), sizeof( ((idInventory *)0)->nextArmorDepleteTime ) },
	{ "int[4]", "pdasViewed", (intptr_t)(&((idInventory *)0)->pdasViewed), sizeof( ((idInventory *)0)->pdasViewed ) },
	{ "int", "selPDA", (intptr_t)(&((idInventory *)0)->selPDA), sizeof( ((idInventory *)0)->selPDA ) },
	{ "int", "selEMail", (intptr_t)(&((idInventory *)0)->selEMail), sizeof( ((idInventory *)0)->selEMail ) },
	{ "int", "selVideo", (intptr_t)(&((idInventory *)0)->selVideo), sizeof( ((idInventory *)0)->selVideo ) },
	{ "int", "selAudio", (intptr_t)(&((idInventory *)0)->selAudio), sizeof( ((idInventory *)0)->selAudio ) },
	{ "bool", "pdaOpened", (intptr_t)(&((idInventory *)0)->pdaOpened), sizeof( ((idInventory *)0)->pdaOpened ) },
	{ "idList < idDict * >", "items", (intptr_t)(&((idInventory *)0)->items), sizeof( ((idInventory *)0)->items ) },
	{ "idList < idStr >", "pdaSecurity", (intptr_t)(&((idInventory *)0)->pdaSecurity), sizeof( ((idInventory *)0)->pdaSecurity ) },
	{ "idList < const idDeclPDA * >", "pdas", (intptr_t)(&((idInventory *)0)->pdas), sizeof( ((idInventory *)0)->pdas ) },
	{ "idList < const idDeclVideo * >", "videos", (intptr_t)(&((idInventory *)0)->videos), sizeof( ((idInventory *)0)->videos ) },
	{ "idList < const idDeclEmail * >", "emails", (intptr_t)(&((idInventory *)0)->emails), sizeof( ((idInventory *)0)->emails ) },
	{ "bool", "ammoPulse", (intptr_t)(&((idInventory *)0)->ammoPulse), sizeof( ((idInventory *)0)->ammoPulse ) },
	{ "bool", "weaponPulse", (intptr_t)(&((idInventory *)0)->weaponPulse), sizeof( ((idInventory *)0)->weaponPulse ) },
	{ "bool", "armorPulse", (intptr_t)(&((idInventory *)0)->armorPulse), sizeof( ((idInventory *)0)->armorPulse ) },
	{ "int", "lastGiveTime", (intptr_t)(&((idInventory *)0)->lastGiveTime), sizeof( ((idInventory *)0)->lastGiveTime ) },
	{ "idList < idLevelTriggerInfo , TAG_IDLIB_LIST_PLAYER >", "levelTriggers", (intptr_t)(&((idInventory *)0)->levelTriggers), sizeof( ((idInventory *)0)->levelTriggers ) },
	{ "int", "nextItemPickup", (intptr_t)(&((idInventory *)0)->nextItemPickup), sizeof( ((idInventory *)0)->nextItemPickup ) },
	{ "int", "nextItemNum", (intptr_t)(&((idInventory *)0)->nextItemNum), sizeof( ((idInventory *)0)->nextItemNum ) },
	{ "int", "onePickupTime", (intptr_t)(&((idInventory *)0)->onePickupTime), sizeof( ((idInventory *)0)->onePickupTime ) },
	{ "idList < idStr >", "pickupItemNames", (intptr_t)(&((idInventory *)0)->pickupItemNames), sizeof( ((idInventory *)0)->pickupItemNames ) },
	{ "idList < idObjectiveInfo >", "objectiveNames", (intptr_t)(&((idInventory *)0)->objectiveNames), sizeof( ((idInventory *)0)->objectiveNames ) },
	{ ": idArray < idPredictedValue < int > , AMMO_NUMTYPES >", "ammo", (intptr_t)(&((idInventory *)0)->ammo), sizeof( ((idInventory *)0)->ammo ) },
	{ "idArray < idPredictedValue < int > , MAX_WEAPONS >", "clip", (intptr_t)(&((idInventory *)0)->clip), sizeof( ((idInventory *)0)->clip ) },
	{ NULL, 0 }
};

static classVariableInfo_t loggedAccel_t_typeInfo[] = {
	{ "int", "time", (intptr_t)(&((loggedAccel_t *)0)->time), sizeof( ((loggedAccel_t *)0)->time ) },
	{ "idVec3", "dir", (intptr_t)(&((loggedAccel_t *)0)->dir), sizeof( ((loggedAccel_t *)0)->dir ) },
	{ NULL, 0 }
};

static classVariableInfo_t aasLocation_t_typeInfo[] = {
	{ "int", "areaNum", (intptr_t)(&((aasLocation_t *)0)->areaNum), sizeof( ((aasLocation_t *)0)->areaNum ) },
	{ "idVec3", "pos", (intptr_t)(&((aasLocation_t *)0)->pos), sizeof( ((aasLocation_t *)0)->pos ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPlayer_typeInfo[] = {
	{ "usercmd_t", "oldCmd", (intptr_t)(&((idPlayer *)0)->oldCmd), sizeof( ((idPlayer *)0)->oldCmd ) },
	{ "usercmd_t", "usercmd", (intptr_t)(&((idPlayer *)0)->usercmd), sizeof( ((idPlayer *)0)->usercmd ) },
	{ "idPlayerView", "playerView", (intptr_t)(&((idPlayer *)0)->playerView), sizeof( ((idPlayer *)0)->playerView ) },
	{ "renderEntity_t", "laserSightRenderEntity", (intptr_t)(&((idPlayer *)0)->laserSightRenderEntity), sizeof( ((idPlayer *)0)->laserSightRenderEntity ) },
	{ "qhandle_t", "laserSightHandle", (intptr_t)(&((idPlayer *)0)->laserSightHandle), sizeof( ((idPlayer *)0)->laserSightHandle ) },
	{ "bool", "noclip", (intptr_t)(&((idPlayer *)0)->noclip), sizeof( ((idPlayer *)0)->noclip ) },
	{ "bool", "godmode", (intptr_t)(&((idPlayer *)0)->godmode), sizeof( ((idPlayer *)0)->godmode ) },
	{ "bool", "spawnAnglesSet", (intptr_t)(&((idPlayer *)0)->spawnAnglesSet), sizeof( ((idPlayer *)0)->spawnAnglesSet ) },
	{ "idAngles", "spawnAngles", (intptr_t)(&((idPlayer *)0)->spawnAngles), sizeof( ((idPlayer *)0)->spawnAngles ) },
	{ "idAngles", "viewAngles", (intptr_t)(&((idPlayer *)0)->viewAngles), sizeof( ((idPlayer *)0)->viewAngles ) },
	{ "idAngles", "cmdAngles", (intptr_t)(&((idPlayer *)0)->cmdAngles), sizeof( ((idPlayer *)0)->cmdAngles ) },
	{ "float", "independentWeaponPitchAngle", (intptr_t)(&((idPlayer *)0)->independentWeaponPitchAngle), sizeof( ((idPlayer *)0)->independentWeaponPitchAngle ) },
	{ "idQuat", "previousViewQuat", (intptr_t)(&((idPlayer *)0)->previousViewQuat), sizeof( ((idPlayer *)0)->previousViewQuat ) },
	{ "idQuat", "nextViewQuat", (intptr_t)(&((idPlayer *)0)->nextViewQuat), sizeof( ((idPlayer *)0)->nextViewQuat ) },
	{ "int", "buttonMask", (intptr_t)(&((idPlayer *)0)->buttonMask), sizeof( ((idPlayer *)0)->buttonMask ) },
	{ "int", "oldButtons", (intptr_t)(&((idPlayer *)0)->oldButtons), sizeof( ((idPlayer *)0)->oldButtons ) },
	{ "int", "oldImpulseSequence", (intptr_t)(&((idPlayer *)0)->oldImpulseSequence), sizeof( ((idPlayer *)0)->oldImpulseSequence ) },
	{ "int", "lastHitTime", (intptr_t)(&((idPlayer *)0)->lastHitTime), sizeof( ((idPlayer *)0)->lastHitTime ) },
	{ "int", "lastSndHitTime", (intptr_t)(&((idPlayer *)0)->lastSndHitTime), sizeof( ((idPlayer *)0)->lastSndHitTime ) },
	{ "int", "lastSavingThrowTime", (intptr_t)(&((idPlayer *)0)->lastSavingThrowTime), sizeof( ((idPlayer *)0)->lastSavingThrowTime ) },
	{ "bool[100]", "pdaHasBeenRead", (intptr_t)(&((idPlayer *)0)->pdaHasBeenRead), sizeof( ((idPlayer *)0)->pdaHasBeenRead ) },
	{ "bool[100]", "videoHasBeenViewed", (intptr_t)(&((idPlayer *)0)->videoHasBeenViewed), sizeof( ((idPlayer *)0)->videoHasBeenViewed ) },
	{ "bool[200]", "audioHasBeenHeard", (intptr_t)(&((idPlayer *)0)->audioHasBeenHeard), sizeof( ((idPlayer *)0)->audioHasBeenHeard ) },
	{ "idScriptBool", "AI_FORWARD", (intptr_t)(&((idPlayer *)0)->AI_FORWARD), sizeof( ((idPlayer *)0)->AI_FORWARD ) },
	{ "idScriptBool", "AI_BACKWARD", (intptr_t)(&((idPlayer *)0)->AI_BACKWARD), sizeof( ((idPlayer *)0)->AI_BACKWARD ) },
	{ "idScriptBool", "AI_STRAFE_LEFT", (intptr_t)(&((idPlayer *)0)->AI_STRAFE_LEFT), sizeof( ((idPlayer *)0)->AI_STRAFE_LEFT ) },
	{ "idScriptBool", "AI_STRAFE_RIGHT", (intptr_t)(&((idPlayer *)0)->AI_STRAFE_RIGHT), sizeof( ((idPlayer *)0)->AI_STRAFE_RIGHT ) },
	{ "idScriptBool", "AI_ATTACK_HELD", (intptr_t)(&((idPlayer *)0)->AI_ATTACK_HELD), sizeof( ((idPlayer *)0)->AI_ATTACK_HELD ) },
	{ "idScriptBool", "AI_WEAPON_FIRED", (intptr_t)(&((idPlayer *)0)->AI_WEAPON_FIRED), sizeof( ((idPlayer *)0)->AI_WEAPON_FIRED ) },
	{ "idScriptBool", "AI_JUMP", (intptr_t)(&((idPlayer *)0)->AI_JUMP), sizeof( ((idPlayer *)0)->AI_JUMP ) },
	{ "idScriptBool", "AI_CROUCH", (intptr_t)(&((idPlayer *)0)->AI_CROUCH), sizeof( ((idPlayer *)0)->AI_CROUCH ) },
	{ "idScriptBool", "AI_ONGROUND", (intptr_t)(&((idPlayer *)0)->AI_ONGROUND), sizeof( ((idPlayer *)0)->AI_ONGROUND ) },
	{ "idScriptBool", "AI_ONLADDER", (intptr_t)(&((idPlayer *)0)->AI_ONLADDER), sizeof( ((idPlayer *)0)->AI_ONLADDER ) },
	{ "idScriptBool", "AI_DEAD", (intptr_t)(&((idPlayer *)0)->AI_DEAD), sizeof( ((idPlayer *)0)->AI_DEAD ) },
	{ "idScriptBool", "AI_RUN", (intptr_t)(&((idPlayer *)0)->AI_RUN), sizeof( ((idPlayer *)0)->AI_RUN ) },
	{ "idScriptBool", "AI_PAIN", (intptr_t)(&((idPlayer *)0)->AI_PAIN), sizeof( ((idPlayer *)0)->AI_PAIN ) },
	{ "idScriptBool", "AI_HARDLANDING", (intptr_t)(&((idPlayer *)0)->AI_HARDLANDING), sizeof( ((idPlayer *)0)->AI_HARDLANDING ) },
	{ "idScriptBool", "AI_SOFTLANDING", (intptr_t)(&((idPlayer *)0)->AI_SOFTLANDING), sizeof( ((idPlayer *)0)->AI_SOFTLANDING ) },
	{ "idScriptBool", "AI_RELOAD", (intptr_t)(&((idPlayer *)0)->AI_RELOAD), sizeof( ((idPlayer *)0)->AI_RELOAD ) },
	{ "idScriptBool", "AI_TELEPORT", (intptr_t)(&((idPlayer *)0)->AI_TELEPORT), sizeof( ((idPlayer *)0)->AI_TELEPORT ) },
	{ "idScriptBool", "AI_TURN_LEFT", (intptr_t)(&((idPlayer *)0)->AI_TURN_LEFT), sizeof( ((idPlayer *)0)->AI_TURN_LEFT ) },
	{ "idScriptBool", "AI_TURN_RIGHT", (intptr_t)(&((idPlayer *)0)->AI_TURN_RIGHT), sizeof( ((idPlayer *)0)->AI_TURN_RIGHT ) },
	{ "idInventory", "inventory", (intptr_t)(&((idPlayer *)0)->inventory), sizeof( ((idPlayer *)0)->inventory ) },
	{ "idTarget_SetPrimaryObjective *", "primaryObjective", (intptr_t)(&((idPlayer *)0)->primaryObjective), sizeof( ((idPlayer *)0)->primaryObjective ) },
	{ "bool", "flashlightReset", (intptr_t)(&((idPlayer *)0)->flashlightReset), sizeof( ((idPlayer *)0)->flashlightReset ) },
	{ "int", "flashlightBattery", (intptr_t)(&((idPlayer *)0)->flashlightBattery), sizeof( ((idPlayer *)0)->flashlightBattery ) },
	{ "idEntityPtr < idWeapon >", "flashlight", (intptr_t)(&((idPlayer *)0)->flashlight), sizeof( ((idPlayer *)0)->flashlight ) },
	{ "idEntityPtr < idWeapon >", "weapon", (intptr_t)(&((idPlayer *)0)->weapon), sizeof( ((idPlayer *)0)->weapon ) },
	{ "idMenuHandler_HUD *", "hudManager", (intptr_t)(&((idPlayer *)0)->hudManager), sizeof( ((idPlayer *)0)->hudManager ) },
	{ "idMenuScreen_HUD *", "hud", (intptr_t)(&((idPlayer *)0)->hud), sizeof( ((idPlayer *)0)->hud ) },
	{ "idMenuHandler_PDA *", "pdaMenu", (intptr_t)(&((idPlayer *)0)->pdaMenu), sizeof( ((idPlayer *)0)->pdaMenu ) },
	{ "idSWF *", "mpMessages", (intptr_t)(&((idPlayer *)0)->mpMessages), sizeof( ((idPlayer *)0)->mpMessages ) },
	{ "bool", "objectiveSystemOpen", (intptr_t)(&((idPlayer *)0)->objectiveSystemOpen), sizeof( ((idPlayer *)0)->objectiveSystemOpen ) },
	{ "int[4]", "quickSlot", (intptr_t)(&((idPlayer *)0)->quickSlot), sizeof( ((idPlayer *)0)->quickSlot ) },
	{ "int", "weapon_soulcube", (intptr_t)(&((idPlayer *)0)->weapon_soulcube), sizeof( ((idPlayer *)0)->weapon_soulcube ) },
	{ "int", "weapon_pda", (intptr_t)(&((idPlayer *)0)->weapon_pda), sizeof( ((idPlayer *)0)->weapon_pda ) },
	{ "int", "weapon_fists", (intptr_t)(&((idPlayer *)0)->weapon_fists), sizeof( ((idPlayer *)0)->weapon_fists ) },
	{ "int", "weapon_flashlight", (intptr_t)(&((idPlayer *)0)->weapon_flashlight), sizeof( ((idPlayer *)0)->weapon_flashlight ) },
	{ "int", "weapon_chainsaw", (intptr_t)(&((idPlayer *)0)->weapon_chainsaw), sizeof( ((idPlayer *)0)->weapon_chainsaw ) },
	{ "int", "weapon_bloodstone", (intptr_t)(&((idPlayer *)0)->weapon_bloodstone), sizeof( ((idPlayer *)0)->weapon_bloodstone ) },
	{ "int", "weapon_bloodstone_active1", (intptr_t)(&((idPlayer *)0)->weapon_bloodstone_active1), sizeof( ((idPlayer *)0)->weapon_bloodstone_active1 ) },
	{ "int", "weapon_bloodstone_active2", (intptr_t)(&((idPlayer *)0)->weapon_bloodstone_active2), sizeof( ((idPlayer *)0)->weapon_bloodstone_active2 ) },
	{ "int", "weapon_bloodstone_active3", (intptr_t)(&((idPlayer *)0)->weapon_bloodstone_active3), sizeof( ((idPlayer *)0)->weapon_bloodstone_active3 ) },
	{ "bool", "harvest_lock", (intptr_t)(&((idPlayer *)0)->harvest_lock), sizeof( ((idPlayer *)0)->harvest_lock ) },
	{ "int", "heartRate", (intptr_t)(&((idPlayer *)0)->heartRate), sizeof( ((idPlayer *)0)->heartRate ) },
	{ "idInterpolate < float >", "heartInfo", (intptr_t)(&((idPlayer *)0)->heartInfo), sizeof( ((idPlayer *)0)->heartInfo ) },
	{ "int", "lastHeartAdjust", (intptr_t)(&((idPlayer *)0)->lastHeartAdjust), sizeof( ((idPlayer *)0)->lastHeartAdjust ) },
	{ "int", "lastHeartBeat", (intptr_t)(&((idPlayer *)0)->lastHeartBeat), sizeof( ((idPlayer *)0)->lastHeartBeat ) },
	{ "int", "lastDmgTime", (intptr_t)(&((idPlayer *)0)->lastDmgTime), sizeof( ((idPlayer *)0)->lastDmgTime ) },
	{ "int", "deathClearContentsTime", (intptr_t)(&((idPlayer *)0)->deathClearContentsTime), sizeof( ((idPlayer *)0)->deathClearContentsTime ) },
	{ "bool", "doingDeathSkin", (intptr_t)(&((idPlayer *)0)->doingDeathSkin), sizeof( ((idPlayer *)0)->doingDeathSkin ) },
	{ "int", "lastArmorPulse", (intptr_t)(&((idPlayer *)0)->lastArmorPulse), sizeof( ((idPlayer *)0)->lastArmorPulse ) },
	{ "float", "stamina", (intptr_t)(&((idPlayer *)0)->stamina), sizeof( ((idPlayer *)0)->stamina ) },
	{ "float", "healthPool", (intptr_t)(&((idPlayer *)0)->healthPool), sizeof( ((idPlayer *)0)->healthPool ) },
	{ "int", "nextHealthPulse", (intptr_t)(&((idPlayer *)0)->nextHealthPulse), sizeof( ((idPlayer *)0)->nextHealthPulse ) },
	{ "bool", "healthPulse", (intptr_t)(&((idPlayer *)0)->healthPulse), sizeof( ((idPlayer *)0)->healthPulse ) },
	{ "bool", "healthTake", (intptr_t)(&((idPlayer *)0)->healthTake), sizeof( ((idPlayer *)0)->healthTake ) },
	{ "int", "nextHealthTake", (intptr_t)(&((idPlayer *)0)->nextHealthTake), sizeof( ((idPlayer *)0)->nextHealthTake ) },
	{ "float[3]", "controllerShakeHighMag", (intptr_t)(&((idPlayer *)0)->controllerShakeHighMag), sizeof( ((idPlayer *)0)->controllerShakeHighMag ) },
	{ "float[3]", "controllerShakeLowMag", (intptr_t)(&((idPlayer *)0)->controllerShakeLowMag), sizeof( ((idPlayer *)0)->controllerShakeLowMag ) },
	{ "int[3]", "controllerShakeHighTime", (intptr_t)(&((idPlayer *)0)->controllerShakeHighTime), sizeof( ((idPlayer *)0)->controllerShakeHighTime ) },
	{ "int[3]", "controllerShakeLowTime", (intptr_t)(&((idPlayer *)0)->controllerShakeLowTime), sizeof( ((idPlayer *)0)->controllerShakeLowTime ) },
	{ "int", "controllerShakeTimeGroup", (intptr_t)(&((idPlayer *)0)->controllerShakeTimeGroup), sizeof( ((idPlayer *)0)->controllerShakeTimeGroup ) },
	{ "bool", "hiddenWeapon", (intptr_t)(&((idPlayer *)0)->hiddenWeapon), sizeof( ((idPlayer *)0)->hiddenWeapon ) },
	{ "idEntityPtr < idProjectile >", "soulCubeProjectile", (intptr_t)(&((idPlayer *)0)->soulCubeProjectile), sizeof( ((idPlayer *)0)->soulCubeProjectile ) },
	{ "idAimAssist", "aimAssist", (intptr_t)(&((idPlayer *)0)->aimAssist), sizeof( ((idPlayer *)0)->aimAssist ) },
	{ "int", "spectator", (intptr_t)(&((idPlayer *)0)->spectator), sizeof( ((idPlayer *)0)->spectator ) },
	{ "bool", "forceScoreBoard", (intptr_t)(&((idPlayer *)0)->forceScoreBoard), sizeof( ((idPlayer *)0)->forceScoreBoard ) },
	{ "bool", "forceRespawn", (intptr_t)(&((idPlayer *)0)->forceRespawn), sizeof( ((idPlayer *)0)->forceRespawn ) },
	{ "bool", "spectating", (intptr_t)(&((idPlayer *)0)->spectating), sizeof( ((idPlayer *)0)->spectating ) },
	{ "int", "lastSpectateTeleport", (intptr_t)(&((idPlayer *)0)->lastSpectateTeleport), sizeof( ((idPlayer *)0)->lastSpectateTeleport ) },
	{ "bool", "lastHitToggle", (intptr_t)(&((idPlayer *)0)->lastHitToggle), sizeof( ((idPlayer *)0)->lastHitToggle ) },
	{ "bool", "wantSpectate", (intptr_t)(&((idPlayer *)0)->wantSpectate), sizeof( ((idPlayer *)0)->wantSpectate ) },
	{ "bool", "weaponGone", (intptr_t)(&((idPlayer *)0)->weaponGone), sizeof( ((idPlayer *)0)->weaponGone ) },
	{ "bool", "useInitialSpawns", (intptr_t)(&((idPlayer *)0)->useInitialSpawns), sizeof( ((idPlayer *)0)->useInitialSpawns ) },
	{ "int", "tourneyRank", (intptr_t)(&((idPlayer *)0)->tourneyRank), sizeof( ((idPlayer *)0)->tourneyRank ) },
	{ "int", "tourneyLine", (intptr_t)(&((idPlayer *)0)->tourneyLine), sizeof( ((idPlayer *)0)->tourneyLine ) },
	{ "int", "spawnedTime", (intptr_t)(&((idPlayer *)0)->spawnedTime), sizeof( ((idPlayer *)0)->spawnedTime ) },
	{ "idStr", "netname", (intptr_t)(&((idPlayer *)0)->netname), sizeof( ((idPlayer *)0)->netname ) },
	{ "bool", "carryingFlag", (intptr_t)(&((idPlayer *)0)->carryingFlag), sizeof( ((idPlayer *)0)->carryingFlag ) },
	{ "idEntityPtr < idEntity >", "teleportEntity", (intptr_t)(&((idPlayer *)0)->teleportEntity), sizeof( ((idPlayer *)0)->teleportEntity ) },
	{ "int", "teleportKiller", (intptr_t)(&((idPlayer *)0)->teleportKiller), sizeof( ((idPlayer *)0)->teleportKiller ) },
	{ "bool", "lastManOver", (intptr_t)(&((idPlayer *)0)->lastManOver), sizeof( ((idPlayer *)0)->lastManOver ) },
	{ "bool", "lastManPlayAgain", (intptr_t)(&((idPlayer *)0)->lastManPlayAgain), sizeof( ((idPlayer *)0)->lastManPlayAgain ) },
	{ "bool", "lastManPresent", (intptr_t)(&((idPlayer *)0)->lastManPresent), sizeof( ((idPlayer *)0)->lastManPresent ) },
	{ "bool", "isLagged", (intptr_t)(&((idPlayer *)0)->isLagged), sizeof( ((idPlayer *)0)->isLagged ) },
	{ "int", "isChatting", (intptr_t)(&((idPlayer *)0)->isChatting), sizeof( ((idPlayer *)0)->isChatting ) },
	{ "int", "minRespawnTime", (intptr_t)(&((idPlayer *)0)->minRespawnTime), sizeof( ((idPlayer *)0)->minRespawnTime ) },
	{ "int", "maxRespawnTime", (intptr_t)(&((idPlayer *)0)->maxRespawnTime), sizeof( ((idPlayer *)0)->maxRespawnTime ) },
	{ "idVec3", "firstPersonViewOrigin", (intptr_t)(&((idPlayer *)0)->firstPersonViewOrigin), sizeof( ((idPlayer *)0)->firstPersonViewOrigin ) },
	{ "idMat3", "firstPersonViewAxis", (intptr_t)(&((idPlayer *)0)->firstPersonViewAxis), sizeof( ((idPlayer *)0)->firstPersonViewAxis ) },
	{ "idDragEntity", "dragEntity", (intptr_t)(&((idPlayer *)0)->dragEntity), sizeof( ((idPlayer *)0)->dragEntity ) },
	{ "idFuncMountedObject *", "mountedObject", (intptr_t)(&((idPlayer *)0)->mountedObject), sizeof( ((idPlayer *)0)->mountedObject ) },
	{ "idEntityPtr < idLight >", "enviroSuitLight", (intptr_t)(&((idPlayer *)0)->enviroSuitLight), sizeof( ((idPlayer *)0)->enviroSuitLight ) },
	{ "bool", "healthRecharge", (intptr_t)(&((idPlayer *)0)->healthRecharge), sizeof( ((idPlayer *)0)->healthRecharge ) },
	{ "int", "lastHealthRechargeTime", (intptr_t)(&((idPlayer *)0)->lastHealthRechargeTime), sizeof( ((idPlayer *)0)->lastHealthRechargeTime ) },
	{ "int", "rechargeSpeed", (intptr_t)(&((idPlayer *)0)->rechargeSpeed), sizeof( ((idPlayer *)0)->rechargeSpeed ) },
	{ "float", "new_g_damageScale", (intptr_t)(&((idPlayer *)0)->new_g_damageScale), sizeof( ((idPlayer *)0)->new_g_damageScale ) },
	{ "bool", "bloomEnabled", (intptr_t)(&((idPlayer *)0)->bloomEnabled), sizeof( ((idPlayer *)0)->bloomEnabled ) },
	{ "float", "bloomSpeed", (intptr_t)(&((idPlayer *)0)->bloomSpeed), sizeof( ((idPlayer *)0)->bloomSpeed ) },
	{ "float", "bloomIntensity", (intptr_t)(&((idPlayer *)0)->bloomIntensity), sizeof( ((idPlayer *)0)->bloomIntensity ) },
	{ ": idAchievementManager", "achievementManager", (intptr_t)(&((idPlayer *)0)->achievementManager), sizeof( ((idPlayer *)0)->achievementManager ) },
	{ "int", "playedTimeSecs", (intptr_t)(&((idPlayer *)0)->playedTimeSecs), sizeof( ((idPlayer *)0)->playedTimeSecs ) },
	{ "int", "playedTimeResidual", (intptr_t)(&((idPlayer *)0)->playedTimeResidual), sizeof( ((idPlayer *)0)->playedTimeResidual ) },
	{ "jointHandle_t", "hipJoint", (intptr_t)(&((idPlayer *)0)->hipJoint), sizeof( ((idPlayer *)0)->hipJoint ) },
	{ "jointHandle_t", "chestJoint", (intptr_t)(&((idPlayer *)0)->chestJoint), sizeof( ((idPlayer *)0)->chestJoint ) },
	{ "jointHandle_t", "headJoint", (intptr_t)(&((idPlayer *)0)->headJoint), sizeof( ((idPlayer *)0)->headJoint ) },
	{ "idPhysics_Player", "physicsObj", (intptr_t)(&((idPlayer *)0)->physicsObj), sizeof( ((idPlayer *)0)->physicsObj ) },
	{ "idList < aasLocation_t , TAG_IDLIB_LIST_PLAYER >", "aasLocation", (intptr_t)(&((idPlayer *)0)->aasLocation), sizeof( ((idPlayer *)0)->aasLocation ) },
	{ "int", "bobFoot", (intptr_t)(&((idPlayer *)0)->bobFoot), sizeof( ((idPlayer *)0)->bobFoot ) },
	{ "float", "bobFrac", (intptr_t)(&((idPlayer *)0)->bobFrac), sizeof( ((idPlayer *)0)->bobFrac ) },
	{ "float", "bobfracsin", (intptr_t)(&((idPlayer *)0)->bobfracsin), sizeof( ((idPlayer *)0)->bobfracsin ) },
	{ "int", "bobCycle", (intptr_t)(&((idPlayer *)0)->bobCycle), sizeof( ((idPlayer *)0)->bobCycle ) },
	{ "float", "xyspeed", (intptr_t)(&((idPlayer *)0)->xyspeed), sizeof( ((idPlayer *)0)->xyspeed ) },
	{ "int", "stepUpTime", (intptr_t)(&((idPlayer *)0)->stepUpTime), sizeof( ((idPlayer *)0)->stepUpTime ) },
	{ "float", "stepUpDelta", (intptr_t)(&((idPlayer *)0)->stepUpDelta), sizeof( ((idPlayer *)0)->stepUpDelta ) },
	{ "float", "idealLegsYaw", (intptr_t)(&((idPlayer *)0)->idealLegsYaw), sizeof( ((idPlayer *)0)->idealLegsYaw ) },
	{ "float", "legsYaw", (intptr_t)(&((idPlayer *)0)->legsYaw), sizeof( ((idPlayer *)0)->legsYaw ) },
	{ "bool", "legsForward", (intptr_t)(&((idPlayer *)0)->legsForward), sizeof( ((idPlayer *)0)->legsForward ) },
	{ "float", "oldViewYaw", (intptr_t)(&((idPlayer *)0)->oldViewYaw), sizeof( ((idPlayer *)0)->oldViewYaw ) },
	{ "idAngles", "viewBobAngles", (intptr_t)(&((idPlayer *)0)->viewBobAngles), sizeof( ((idPlayer *)0)->viewBobAngles ) },
	{ "idVec3", "viewBob", (intptr_t)(&((idPlayer *)0)->viewBob), sizeof( ((idPlayer *)0)->viewBob ) },
	{ "int", "landChange", (intptr_t)(&((idPlayer *)0)->landChange), sizeof( ((idPlayer *)0)->landChange ) },
	{ "int", "landTime", (intptr_t)(&((idPlayer *)0)->landTime), sizeof( ((idPlayer *)0)->landTime ) },
	{ "int", "currentWeapon", (intptr_t)(&((idPlayer *)0)->currentWeapon), sizeof( ((idPlayer *)0)->currentWeapon ) },
	{ "idPredictedValue < int >", "idealWeapon", (intptr_t)(&((idPlayer *)0)->idealWeapon), sizeof( ((idPlayer *)0)->idealWeapon ) },
	{ "int", "previousWeapon", (intptr_t)(&((idPlayer *)0)->previousWeapon), sizeof( ((idPlayer *)0)->previousWeapon ) },
	{ "int", "weaponSwitchTime", (intptr_t)(&((idPlayer *)0)->weaponSwitchTime), sizeof( ((idPlayer *)0)->weaponSwitchTime ) },
	{ "bool", "weaponEnabled", (intptr_t)(&((idPlayer *)0)->weaponEnabled), sizeof( ((idPlayer *)0)->weaponEnabled ) },
	{ "int", "skinIndex", (intptr_t)(&((idPlayer *)0)->skinIndex), sizeof( ((idPlayer *)0)->skinIndex ) },
	{ "const idDeclSkin *", "skin", (intptr_t)(&((idPlayer *)0)->skin), sizeof( ((idPlayer *)0)->skin ) },
	{ "const idDeclSkin *", "powerUpSkin", (intptr_t)(&((idPlayer *)0)->powerUpSkin), sizeof( ((idPlayer *)0)->powerUpSkin ) },
	{ "int", "numProjectilesFired", (intptr_t)(&((idPlayer *)0)->numProjectilesFired), sizeof( ((idPlayer *)0)->numProjectilesFired ) },
	{ "int", "numProjectileHits", (intptr_t)(&((idPlayer *)0)->numProjectileHits), sizeof( ((idPlayer *)0)->numProjectileHits ) },
	{ "int", "numProjectileKills", (intptr_t)(&((idPlayer *)0)->numProjectileKills), sizeof( ((idPlayer *)0)->numProjectileKills ) },
	{ "bool", "airless", (intptr_t)(&((idPlayer *)0)->airless), sizeof( ((idPlayer *)0)->airless ) },
	{ "int", "airMsec", (intptr_t)(&((idPlayer *)0)->airMsec), sizeof( ((idPlayer *)0)->airMsec ) },
	{ "int", "lastAirDamage", (intptr_t)(&((idPlayer *)0)->lastAirDamage), sizeof( ((idPlayer *)0)->lastAirDamage ) },
	{ "bool", "gibDeath", (intptr_t)(&((idPlayer *)0)->gibDeath), sizeof( ((idPlayer *)0)->gibDeath ) },
	{ "bool", "gibsLaunched", (intptr_t)(&((idPlayer *)0)->gibsLaunched), sizeof( ((idPlayer *)0)->gibsLaunched ) },
	{ "idVec3", "gibsDir", (intptr_t)(&((idPlayer *)0)->gibsDir), sizeof( ((idPlayer *)0)->gibsDir ) },
	{ "idInterpolate < float >", "zoomFov", (intptr_t)(&((idPlayer *)0)->zoomFov), sizeof( ((idPlayer *)0)->zoomFov ) },
	{ "idInterpolate < float >", "centerView", (intptr_t)(&((idPlayer *)0)->centerView), sizeof( ((idPlayer *)0)->centerView ) },
	{ "bool", "fxFov", (intptr_t)(&((idPlayer *)0)->fxFov), sizeof( ((idPlayer *)0)->fxFov ) },
	{ "float", "influenceFov", (intptr_t)(&((idPlayer *)0)->influenceFov), sizeof( ((idPlayer *)0)->influenceFov ) },
	{ "int", "influenceActive", (intptr_t)(&((idPlayer *)0)->influenceActive), sizeof( ((idPlayer *)0)->influenceActive ) },
	{ "idEntity *", "influenceEntity", (intptr_t)(&((idPlayer *)0)->influenceEntity), sizeof( ((idPlayer *)0)->influenceEntity ) },
	{ "const idMaterial *", "influenceMaterial", (intptr_t)(&((idPlayer *)0)->influenceMaterial), sizeof( ((idPlayer *)0)->influenceMaterial ) },
	{ "float", "influenceRadius", (intptr_t)(&((idPlayer *)0)->influenceRadius), sizeof( ((idPlayer *)0)->influenceRadius ) },
	{ "const idDeclSkin *", "influenceSkin", (intptr_t)(&((idPlayer *)0)->influenceSkin), sizeof( ((idPlayer *)0)->influenceSkin ) },
	{ "idCamera *", "privateCameraView", (intptr_t)(&((idPlayer *)0)->privateCameraView), sizeof( ((idPlayer *)0)->privateCameraView ) },
	{ "idAngles[64]", "loggedViewAngles", (intptr_t)(&((idPlayer *)0)->loggedViewAngles), sizeof( ((idPlayer *)0)->loggedViewAngles ) },
	{ "loggedAccel_t[16]", "loggedAccel", (intptr_t)(&((idPlayer *)0)->loggedAccel), sizeof( ((idPlayer *)0)->loggedAccel ) },
	{ "int", "currentLoggedAccel", (intptr_t)(&((idPlayer *)0)->currentLoggedAccel), sizeof( ((idPlayer *)0)->currentLoggedAccel ) },
	{ "idEntity *", "focusGUIent", (intptr_t)(&((idPlayer *)0)->focusGUIent), sizeof( ((idPlayer *)0)->focusGUIent ) },
	{ "idUserInterface *", "focusUI", (intptr_t)(&((idPlayer *)0)->focusUI), sizeof( ((idPlayer *)0)->focusUI ) },
	{ "idAI *", "focusCharacter", (intptr_t)(&((idPlayer *)0)->focusCharacter), sizeof( ((idPlayer *)0)->focusCharacter ) },
	{ "int", "talkCursor", (intptr_t)(&((idPlayer *)0)->talkCursor), sizeof( ((idPlayer *)0)->talkCursor ) },
	{ "int", "focusTime", (intptr_t)(&((idPlayer *)0)->focusTime), sizeof( ((idPlayer *)0)->focusTime ) },
	{ "idAFEntity_Vehicle *", "focusVehicle", (intptr_t)(&((idPlayer *)0)->focusVehicle), sizeof( ((idPlayer *)0)->focusVehicle ) },
	{ "idUserInterface *", "cursor", (intptr_t)(&((idPlayer *)0)->cursor), sizeof( ((idPlayer *)0)->cursor ) },
	{ "int", "oldMouseX", (intptr_t)(&((idPlayer *)0)->oldMouseX), sizeof( ((idPlayer *)0)->oldMouseX ) },
	{ "int", "oldMouseY", (intptr_t)(&((idPlayer *)0)->oldMouseY), sizeof( ((idPlayer *)0)->oldMouseY ) },
	{ "const idMaterial *", "pdaVideoMat", (intptr_t)(&((idPlayer *)0)->pdaVideoMat), sizeof( ((idPlayer *)0)->pdaVideoMat ) },
	{ "bool", "tipUp", (intptr_t)(&((idPlayer *)0)->tipUp), sizeof( ((idPlayer *)0)->tipUp ) },
	{ "bool", "objectiveUp", (intptr_t)(&((idPlayer *)0)->objectiveUp), sizeof( ((idPlayer *)0)->objectiveUp ) },
	{ "int", "lastDamageDef", (intptr_t)(&((idPlayer *)0)->lastDamageDef), sizeof( ((idPlayer *)0)->lastDamageDef ) },
	{ "idVec3", "lastDamageDir", (intptr_t)(&((idPlayer *)0)->lastDamageDir), sizeof( ((idPlayer *)0)->lastDamageDir ) },
	{ "int", "lastDamageLocation", (intptr_t)(&((idPlayer *)0)->lastDamageLocation), sizeof( ((idPlayer *)0)->lastDamageLocation ) },
	{ "int", "smoothedFrame", (intptr_t)(&((idPlayer *)0)->smoothedFrame), sizeof( ((idPlayer *)0)->smoothedFrame ) },
	{ "bool", "smoothedOriginUpdated", (intptr_t)(&((idPlayer *)0)->smoothedOriginUpdated), sizeof( ((idPlayer *)0)->smoothedOriginUpdated ) },
	{ "idVec3", "smoothedOrigin", (intptr_t)(&((idPlayer *)0)->smoothedOrigin), sizeof( ((idPlayer *)0)->smoothedOrigin ) },
	{ "idAngles", "smoothedAngles", (intptr_t)(&((idPlayer *)0)->smoothedAngles), sizeof( ((idPlayer *)0)->smoothedAngles ) },
	{ "idHashTable < WeaponToggle_t >", "weaponToggles", (intptr_t)(&((idPlayer *)0)->weaponToggles), sizeof( ((idPlayer *)0)->weaponToggles ) },
	{ "int", "hudPowerup", (intptr_t)(&((idPlayer *)0)->hudPowerup), sizeof( ((idPlayer *)0)->hudPowerup ) },
	{ "int", "lastHudPowerup", (intptr_t)(&((idPlayer *)0)->lastHudPowerup), sizeof( ((idPlayer *)0)->lastHudPowerup ) },
	{ "int", "hudPowerupDuration", (intptr_t)(&((idPlayer *)0)->hudPowerupDuration), sizeof( ((idPlayer *)0)->hudPowerupDuration ) },
	{ "bool", "respawning", (intptr_t)(&((idPlayer *)0)->respawning), sizeof( ((idPlayer *)0)->respawning ) },
	{ "bool", "leader", (intptr_t)(&((idPlayer *)0)->leader), sizeof( ((idPlayer *)0)->leader ) },
	{ "int", "lastSpectateChange", (intptr_t)(&((idPlayer *)0)->lastSpectateChange), sizeof( ((idPlayer *)0)->lastSpectateChange ) },
	{ "int", "lastTeleFX", (intptr_t)(&((idPlayer *)0)->lastTeleFX), sizeof( ((idPlayer *)0)->lastTeleFX ) },
	{ "bool", "weaponCatchup", (intptr_t)(&((idPlayer *)0)->weaponCatchup), sizeof( ((idPlayer *)0)->weaponCatchup ) },
	{ "int", "MPAim", (intptr_t)(&((idPlayer *)0)->MPAim), sizeof( ((idPlayer *)0)->MPAim ) },
	{ "int", "lastMPAim", (intptr_t)(&((idPlayer *)0)->lastMPAim), sizeof( ((idPlayer *)0)->lastMPAim ) },
	{ "int", "lastMPAimTime", (intptr_t)(&((idPlayer *)0)->lastMPAimTime), sizeof( ((idPlayer *)0)->lastMPAimTime ) },
	{ "int", "MPAimFadeTime", (intptr_t)(&((idPlayer *)0)->MPAimFadeTime), sizeof( ((idPlayer *)0)->MPAimFadeTime ) },
	{ "bool", "MPAimHighlight", (intptr_t)(&((idPlayer *)0)->MPAimHighlight), sizeof( ((idPlayer *)0)->MPAimHighlight ) },
	{ "bool", "isTelefragged", (intptr_t)(&((idPlayer *)0)->isTelefragged), sizeof( ((idPlayer *)0)->isTelefragged ) },
	{ "int", "serverOverridePositionTime", (intptr_t)(&((idPlayer *)0)->serverOverridePositionTime), sizeof( ((idPlayer *)0)->serverOverridePositionTime ) },
	{ "int", "clientFireCount", (intptr_t)(&((idPlayer *)0)->clientFireCount), sizeof( ((idPlayer *)0)->clientFireCount ) },
	{ "idPlayerIcon", "playerIcon", (intptr_t)(&((idPlayer *)0)->playerIcon), sizeof( ((idPlayer *)0)->playerIcon ) },
	{ "bool", "selfSmooth", (intptr_t)(&((idPlayer *)0)->selfSmooth), sizeof( ((idPlayer *)0)->selfSmooth ) },
	{ "netBoolEvent_t", "respawn_netEvent", (intptr_t)(&((idPlayer *)0)->respawn_netEvent), sizeof( ((idPlayer *)0)->respawn_netEvent ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMover_moveState_t_typeInfo[] = {
	{ "moveStage_t", "stage", (intptr_t)(&((idMover::moveState_t *)0)->stage), sizeof( ((idMover::moveState_t *)0)->stage ) },
	{ "int", "acceleration", (intptr_t)(&((idMover::moveState_t *)0)->acceleration), sizeof( ((idMover::moveState_t *)0)->acceleration ) },
	{ "int", "movetime", (intptr_t)(&((idMover::moveState_t *)0)->movetime), sizeof( ((idMover::moveState_t *)0)->movetime ) },
	{ "int", "deceleration", (intptr_t)(&((idMover::moveState_t *)0)->deceleration), sizeof( ((idMover::moveState_t *)0)->deceleration ) },
	{ "idVec3", "dir", (intptr_t)(&((idMover::moveState_t *)0)->dir), sizeof( ((idMover::moveState_t *)0)->dir ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMover_rotationState_t_typeInfo[] = {
	{ "moveStage_t", "stage", (intptr_t)(&((idMover::rotationState_t *)0)->stage), sizeof( ((idMover::rotationState_t *)0)->stage ) },
	{ "int", "acceleration", (intptr_t)(&((idMover::rotationState_t *)0)->acceleration), sizeof( ((idMover::rotationState_t *)0)->acceleration ) },
	{ "int", "movetime", (intptr_t)(&((idMover::rotationState_t *)0)->movetime), sizeof( ((idMover::rotationState_t *)0)->movetime ) },
	{ "int", "deceleration", (intptr_t)(&((idMover::rotationState_t *)0)->deceleration), sizeof( ((idMover::rotationState_t *)0)->deceleration ) },
	{ "idAngles", "rot", (intptr_t)(&((idMover::rotationState_t *)0)->rot), sizeof( ((idMover::rotationState_t *)0)->rot ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMover_typeInfo[] = {
	{ "idPhysics_Parametric", "physicsObj", (intptr_t)(&((idMover *)0)->physicsObj), sizeof( ((idMover *)0)->physicsObj ) },
	{ "moveState_t", "move", (intptr_t)(&((idMover *)0)->move), sizeof( ((idMover *)0)->move ) },
	{ ": rotationState_t", "rot", (intptr_t)(&((idMover *)0)->rot), sizeof( ((idMover *)0)->rot ) },
	{ "int", "move_thread", (intptr_t)(&((idMover *)0)->move_thread), sizeof( ((idMover *)0)->move_thread ) },
	{ "int", "rotate_thread", (intptr_t)(&((idMover *)0)->rotate_thread), sizeof( ((idMover *)0)->rotate_thread ) },
	{ "idAngles", "dest_angles", (intptr_t)(&((idMover *)0)->dest_angles), sizeof( ((idMover *)0)->dest_angles ) },
	{ "idAngles", "angle_delta", (intptr_t)(&((idMover *)0)->angle_delta), sizeof( ((idMover *)0)->angle_delta ) },
	{ "idVec3", "dest_position", (intptr_t)(&((idMover *)0)->dest_position), sizeof( ((idMover *)0)->dest_position ) },
	{ "idVec3", "move_delta", (intptr_t)(&((idMover *)0)->move_delta), sizeof( ((idMover *)0)->move_delta ) },
	{ "float", "move_speed", (intptr_t)(&((idMover *)0)->move_speed), sizeof( ((idMover *)0)->move_speed ) },
	{ "int", "move_time", (intptr_t)(&((idMover *)0)->move_time), sizeof( ((idMover *)0)->move_time ) },
	{ "int", "deceltime", (intptr_t)(&((idMover *)0)->deceltime), sizeof( ((idMover *)0)->deceltime ) },
	{ "int", "acceltime", (intptr_t)(&((idMover *)0)->acceltime), sizeof( ((idMover *)0)->acceltime ) },
	{ "bool", "stopRotation", (intptr_t)(&((idMover *)0)->stopRotation), sizeof( ((idMover *)0)->stopRotation ) },
	{ "bool", "useSplineAngles", (intptr_t)(&((idMover *)0)->useSplineAngles), sizeof( ((idMover *)0)->useSplineAngles ) },
	{ "idEntityPtr < idEntity >", "splineEnt", (intptr_t)(&((idMover *)0)->splineEnt), sizeof( ((idMover *)0)->splineEnt ) },
	{ "moverCommand_t", "lastCommand", (intptr_t)(&((idMover *)0)->lastCommand), sizeof( ((idMover *)0)->lastCommand ) },
	{ "float", "damage", (intptr_t)(&((idMover *)0)->damage), sizeof( ((idMover *)0)->damage ) },
	{ "qhandle_t", "areaPortal", (intptr_t)(&((idMover *)0)->areaPortal), sizeof( ((idMover *)0)->areaPortal ) },
	{ "idList < idEntityPtr < idEntity > , TAG_MOVER >", "guiTargets", (intptr_t)(&((idMover *)0)->guiTargets), sizeof( ((idMover *)0)->guiTargets ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSplinePath_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t floorInfo_s_typeInfo[] = {
	{ "idVec3", "pos", (intptr_t)(&((floorInfo_s *)0)->pos), sizeof( ((floorInfo_s *)0)->pos ) },
	{ "idStr", "door", (intptr_t)(&((floorInfo_s *)0)->door), sizeof( ((floorInfo_s *)0)->door ) },
	{ "int", "floor", (intptr_t)(&((floorInfo_s *)0)->floor), sizeof( ((floorInfo_s *)0)->floor ) },
	{ NULL, 0 }
};

static classVariableInfo_t idElevator_typeInfo[] = {
	{ "elevatorState_t", "state", (intptr_t)(&((idElevator *)0)->state), sizeof( ((idElevator *)0)->state ) },
	{ "idList < floorInfo_s , TAG_MOVER >", "floorInfo", (intptr_t)(&((idElevator *)0)->floorInfo), sizeof( ((idElevator *)0)->floorInfo ) },
	{ "int", "currentFloor", (intptr_t)(&((idElevator *)0)->currentFloor), sizeof( ((idElevator *)0)->currentFloor ) },
	{ "int", "pendingFloor", (intptr_t)(&((idElevator *)0)->pendingFloor), sizeof( ((idElevator *)0)->pendingFloor ) },
	{ "int", "lastFloor", (intptr_t)(&((idElevator *)0)->lastFloor), sizeof( ((idElevator *)0)->lastFloor ) },
	{ "bool", "controlsDisabled", (intptr_t)(&((idElevator *)0)->controlsDisabled), sizeof( ((idElevator *)0)->controlsDisabled ) },
	{ "float", "returnTime", (intptr_t)(&((idElevator *)0)->returnTime), sizeof( ((idElevator *)0)->returnTime ) },
	{ "int", "returnFloor", (intptr_t)(&((idElevator *)0)->returnFloor), sizeof( ((idElevator *)0)->returnFloor ) },
	{ "int", "lastTouchTime", (intptr_t)(&((idElevator *)0)->lastTouchTime), sizeof( ((idElevator *)0)->lastTouchTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMover_Binary_typeInfo[] = {
	{ ": idVec3", "pos1", (intptr_t)(&((idMover_Binary *)0)->pos1), sizeof( ((idMover_Binary *)0)->pos1 ) },
	{ "idVec3", "pos2", (intptr_t)(&((idMover_Binary *)0)->pos2), sizeof( ((idMover_Binary *)0)->pos2 ) },
	{ "moverState_t", "moverState", (intptr_t)(&((idMover_Binary *)0)->moverState), sizeof( ((idMover_Binary *)0)->moverState ) },
	{ "idMover_Binary *", "moveMaster", (intptr_t)(&((idMover_Binary *)0)->moveMaster), sizeof( ((idMover_Binary *)0)->moveMaster ) },
	{ "idMover_Binary *", "activateChain", (intptr_t)(&((idMover_Binary *)0)->activateChain), sizeof( ((idMover_Binary *)0)->activateChain ) },
	{ "int", "soundPos1", (intptr_t)(&((idMover_Binary *)0)->soundPos1), sizeof( ((idMover_Binary *)0)->soundPos1 ) },
	{ "int", "sound1to2", (intptr_t)(&((idMover_Binary *)0)->sound1to2), sizeof( ((idMover_Binary *)0)->sound1to2 ) },
	{ "int", "sound2to1", (intptr_t)(&((idMover_Binary *)0)->sound2to1), sizeof( ((idMover_Binary *)0)->sound2to1 ) },
	{ "int", "soundPos2", (intptr_t)(&((idMover_Binary *)0)->soundPos2), sizeof( ((idMover_Binary *)0)->soundPos2 ) },
	{ "int", "soundLoop", (intptr_t)(&((idMover_Binary *)0)->soundLoop), sizeof( ((idMover_Binary *)0)->soundLoop ) },
	{ "float", "wait", (intptr_t)(&((idMover_Binary *)0)->wait), sizeof( ((idMover_Binary *)0)->wait ) },
	{ "float", "damage", (intptr_t)(&((idMover_Binary *)0)->damage), sizeof( ((idMover_Binary *)0)->damage ) },
	{ "int", "duration", (intptr_t)(&((idMover_Binary *)0)->duration), sizeof( ((idMover_Binary *)0)->duration ) },
	{ "int", "accelTime", (intptr_t)(&((idMover_Binary *)0)->accelTime), sizeof( ((idMover_Binary *)0)->accelTime ) },
	{ "int", "decelTime", (intptr_t)(&((idMover_Binary *)0)->decelTime), sizeof( ((idMover_Binary *)0)->decelTime ) },
	{ "idEntityPtr < idEntity >", "activatedBy", (intptr_t)(&((idMover_Binary *)0)->activatedBy), sizeof( ((idMover_Binary *)0)->activatedBy ) },
	{ "int", "stateStartTime", (intptr_t)(&((idMover_Binary *)0)->stateStartTime), sizeof( ((idMover_Binary *)0)->stateStartTime ) },
	{ "idStr", "team", (intptr_t)(&((idMover_Binary *)0)->team), sizeof( ((idMover_Binary *)0)->team ) },
	{ "bool", "enabled", (intptr_t)(&((idMover_Binary *)0)->enabled), sizeof( ((idMover_Binary *)0)->enabled ) },
	{ "int", "move_thread", (intptr_t)(&((idMover_Binary *)0)->move_thread), sizeof( ((idMover_Binary *)0)->move_thread ) },
	{ "int", "updateStatus", (intptr_t)(&((idMover_Binary *)0)->updateStatus), sizeof( ((idMover_Binary *)0)->updateStatus ) },
	{ "idStrList", "buddies", (intptr_t)(&((idMover_Binary *)0)->buddies), sizeof( ((idMover_Binary *)0)->buddies ) },
	{ "idPhysics_Parametric", "physicsObj", (intptr_t)(&((idMover_Binary *)0)->physicsObj), sizeof( ((idMover_Binary *)0)->physicsObj ) },
	{ "qhandle_t", "areaPortal", (intptr_t)(&((idMover_Binary *)0)->areaPortal), sizeof( ((idMover_Binary *)0)->areaPortal ) },
	{ "bool", "blocked", (intptr_t)(&((idMover_Binary *)0)->blocked), sizeof( ((idMover_Binary *)0)->blocked ) },
	{ "bool", "playerOnly", (intptr_t)(&((idMover_Binary *)0)->playerOnly), sizeof( ((idMover_Binary *)0)->playerOnly ) },
	{ "idList < idEntityPtr < idEntity > , TAG_MOVER >", "guiTargets", (intptr_t)(&((idMover_Binary *)0)->guiTargets), sizeof( ((idMover_Binary *)0)->guiTargets ) },
	{ NULL, 0 }
};

static classVariableInfo_t idDoor_typeInfo[] = {
	{ ": float", "triggersize", (intptr_t)(&((idDoor *)0)->triggersize), sizeof( ((idDoor *)0)->triggersize ) },
	{ "bool", "crusher", (intptr_t)(&((idDoor *)0)->crusher), sizeof( ((idDoor *)0)->crusher ) },
	{ "bool", "noTouch", (intptr_t)(&((idDoor *)0)->noTouch), sizeof( ((idDoor *)0)->noTouch ) },
	{ "bool", "aas_area_closed", (intptr_t)(&((idDoor *)0)->aas_area_closed), sizeof( ((idDoor *)0)->aas_area_closed ) },
	{ "idStr", "buddyStr", (intptr_t)(&((idDoor *)0)->buddyStr), sizeof( ((idDoor *)0)->buddyStr ) },
	{ "idClipModel *", "trigger", (intptr_t)(&((idDoor *)0)->trigger), sizeof( ((idDoor *)0)->trigger ) },
	{ "idClipModel *", "sndTrigger", (intptr_t)(&((idDoor *)0)->sndTrigger), sizeof( ((idDoor *)0)->sndTrigger ) },
	{ "int", "nextSndTriggerTime", (intptr_t)(&((idDoor *)0)->nextSndTriggerTime), sizeof( ((idDoor *)0)->nextSndTriggerTime ) },
	{ "idVec3", "localTriggerOrigin", (intptr_t)(&((idDoor *)0)->localTriggerOrigin), sizeof( ((idDoor *)0)->localTriggerOrigin ) },
	{ "idMat3", "localTriggerAxis", (intptr_t)(&((idDoor *)0)->localTriggerAxis), sizeof( ((idDoor *)0)->localTriggerAxis ) },
	{ "idStr", "requires", (intptr_t)(&((idDoor *)0)->requires), sizeof( ((idDoor *)0)->requires ) },
	{ "int", "removeItem", (intptr_t)(&((idDoor *)0)->removeItem), sizeof( ((idDoor *)0)->removeItem ) },
	{ "idStr", "syncLock", (intptr_t)(&((idDoor *)0)->syncLock), sizeof( ((idDoor *)0)->syncLock ) },
	{ "int", "normalAxisIndex", (intptr_t)(&((idDoor *)0)->normalAxisIndex), sizeof( ((idDoor *)0)->normalAxisIndex ) },
	{ "idDoor *", "companionDoor", (intptr_t)(&((idDoor *)0)->companionDoor), sizeof( ((idDoor *)0)->companionDoor ) },
	{ NULL, 0 }
};

static classVariableInfo_t idPlat_typeInfo[] = {
	{ ": idClipModel *", "trigger", (intptr_t)(&((idPlat *)0)->trigger), sizeof( ((idPlat *)0)->trigger ) },
	{ "idVec3", "localTriggerOrigin", (intptr_t)(&((idPlat *)0)->localTriggerOrigin), sizeof( ((idPlat *)0)->localTriggerOrigin ) },
	{ "idMat3", "localTriggerAxis", (intptr_t)(&((idPlat *)0)->localTriggerAxis), sizeof( ((idPlat *)0)->localTriggerAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMover_Periodic_typeInfo[] = {
	{ ": idPhysics_Parametric", "physicsObj", (intptr_t)(&((idMover_Periodic *)0)->physicsObj), sizeof( ((idMover_Periodic *)0)->physicsObj ) },
	{ "float", "damage", (intptr_t)(&((idMover_Periodic *)0)->damage), sizeof( ((idMover_Periodic *)0)->damage ) },
	{ NULL, 0 }
};

static classVariableInfo_t idRotater_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "activatedBy", (intptr_t)(&((idRotater *)0)->activatedBy), sizeof( ((idRotater *)0)->activatedBy ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBobber_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idPendulum_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idRiser_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idCamera_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idCameraView_typeInfo[] = {
	{ "float", "fov", (intptr_t)(&((idCameraView *)0)->fov), sizeof( ((idCameraView *)0)->fov ) },
	{ "idEntity *", "attachedTo", (intptr_t)(&((idCameraView *)0)->attachedTo), sizeof( ((idCameraView *)0)->attachedTo ) },
	{ "idEntity *", "attachedView", (intptr_t)(&((idCameraView *)0)->attachedView), sizeof( ((idCameraView *)0)->attachedView ) },
	{ NULL, 0 }
};

static classVariableInfo_t cameraFrame_t_typeInfo[] = {
	{ "idCQuat", "q", (intptr_t)(&((cameraFrame_t *)0)->q), sizeof( ((cameraFrame_t *)0)->q ) },
	{ "idVec3", "t", (intptr_t)(&((cameraFrame_t *)0)->t), sizeof( ((cameraFrame_t *)0)->t ) },
	{ "float", "fov", (intptr_t)(&((cameraFrame_t *)0)->fov), sizeof( ((cameraFrame_t *)0)->fov ) },
	{ NULL, 0 }
};

static classVariableInfo_t idCameraAnim_typeInfo[] = {
	{ ": int", "threadNum", (intptr_t)(&((idCameraAnim *)0)->threadNum), sizeof( ((idCameraAnim *)0)->threadNum ) },
	{ "idVec3", "offset", (intptr_t)(&((idCameraAnim *)0)->offset), sizeof( ((idCameraAnim *)0)->offset ) },
	{ "int", "frameRate", (intptr_t)(&((idCameraAnim *)0)->frameRate), sizeof( ((idCameraAnim *)0)->frameRate ) },
	{ "int", "starttime", (intptr_t)(&((idCameraAnim *)0)->starttime), sizeof( ((idCameraAnim *)0)->starttime ) },
	{ "int", "cycle", (intptr_t)(&((idCameraAnim *)0)->cycle), sizeof( ((idCameraAnim *)0)->cycle ) },
	{ "idList < int >", "cameraCuts", (intptr_t)(&((idCameraAnim *)0)->cameraCuts), sizeof( ((idCameraAnim *)0)->cameraCuts ) },
	{ "idList < cameraFrame_t >", "camera", (intptr_t)(&((idCameraAnim *)0)->camera), sizeof( ((idCameraAnim *)0)->camera ) },
	{ "idEntityPtr < idEntity >", "activator", (intptr_t)(&((idCameraAnim *)0)->activator), sizeof( ((idCameraAnim *)0)->activator ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMoveable_typeInfo[] = {
	{ ": idPhysics_RigidBody", "physicsObj", (intptr_t)(&((idMoveable *)0)->physicsObj), sizeof( ((idMoveable *)0)->physicsObj ) },
	{ "idStr", "brokenModel", (intptr_t)(&((idMoveable *)0)->brokenModel), sizeof( ((idMoveable *)0)->brokenModel ) },
	{ "idStr", "damage", (intptr_t)(&((idMoveable *)0)->damage), sizeof( ((idMoveable *)0)->damage ) },
	{ "idStr", "monsterDamage", (intptr_t)(&((idMoveable *)0)->monsterDamage), sizeof( ((idMoveable *)0)->monsterDamage ) },
	{ "idEntity *", "attacker", (intptr_t)(&((idMoveable *)0)->attacker), sizeof( ((idMoveable *)0)->attacker ) },
	{ "idStr", "fxCollide", (intptr_t)(&((idMoveable *)0)->fxCollide), sizeof( ((idMoveable *)0)->fxCollide ) },
	{ "int", "nextCollideFxTime", (intptr_t)(&((idMoveable *)0)->nextCollideFxTime), sizeof( ((idMoveable *)0)->nextCollideFxTime ) },
	{ "float", "minDamageVelocity", (intptr_t)(&((idMoveable *)0)->minDamageVelocity), sizeof( ((idMoveable *)0)->minDamageVelocity ) },
	{ "float", "maxDamageVelocity", (intptr_t)(&((idMoveable *)0)->maxDamageVelocity), sizeof( ((idMoveable *)0)->maxDamageVelocity ) },
	{ "idCurve_Spline < idVec3 > *", "initialSpline", (intptr_t)(&((idMoveable *)0)->initialSpline), sizeof( ((idMoveable *)0)->initialSpline ) },
	{ "idVec3", "initialSplineDir", (intptr_t)(&((idMoveable *)0)->initialSplineDir), sizeof( ((idMoveable *)0)->initialSplineDir ) },
	{ "bool", "explode", (intptr_t)(&((idMoveable *)0)->explode), sizeof( ((idMoveable *)0)->explode ) },
	{ "bool", "unbindOnDeath", (intptr_t)(&((idMoveable *)0)->unbindOnDeath), sizeof( ((idMoveable *)0)->unbindOnDeath ) },
	{ "bool", "allowStep", (intptr_t)(&((idMoveable *)0)->allowStep), sizeof( ((idMoveable *)0)->allowStep ) },
	{ "bool", "canDamage", (intptr_t)(&((idMoveable *)0)->canDamage), sizeof( ((idMoveable *)0)->canDamage ) },
	{ "int", "nextDamageTime", (intptr_t)(&((idMoveable *)0)->nextDamageTime), sizeof( ((idMoveable *)0)->nextDamageTime ) },
	{ "int", "nextSoundTime", (intptr_t)(&((idMoveable *)0)->nextSoundTime), sizeof( ((idMoveable *)0)->nextSoundTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBarrel_typeInfo[] = {
	{ ": float", "radius", (intptr_t)(&((idBarrel *)0)->radius), sizeof( ((idBarrel *)0)->radius ) },
	{ "int", "barrelAxis", (intptr_t)(&((idBarrel *)0)->barrelAxis), sizeof( ((idBarrel *)0)->barrelAxis ) },
	{ "idVec3", "lastOrigin", (intptr_t)(&((idBarrel *)0)->lastOrigin), sizeof( ((idBarrel *)0)->lastOrigin ) },
	{ "idMat3", "lastAxis", (intptr_t)(&((idBarrel *)0)->lastAxis), sizeof( ((idBarrel *)0)->lastAxis ) },
	{ "float", "additionalRotation", (intptr_t)(&((idBarrel *)0)->additionalRotation), sizeof( ((idBarrel *)0)->additionalRotation ) },
	{ "idMat3", "additionalAxis", (intptr_t)(&((idBarrel *)0)->additionalAxis), sizeof( ((idBarrel *)0)->additionalAxis ) },
	{ NULL, 0 }
};

static classVariableInfo_t idExplodingBarrel_typeInfo[] = {
	{ "explode_state_t", "state", (intptr_t)(&((idExplodingBarrel *)0)->state), sizeof( ((idExplodingBarrel *)0)->state ) },
	{ "idVec3", "spawnOrigin", (intptr_t)(&((idExplodingBarrel *)0)->spawnOrigin), sizeof( ((idExplodingBarrel *)0)->spawnOrigin ) },
	{ "idMat3", "spawnAxis", (intptr_t)(&((idExplodingBarrel *)0)->spawnAxis), sizeof( ((idExplodingBarrel *)0)->spawnAxis ) },
	{ "qhandle_t", "particleModelDefHandle", (intptr_t)(&((idExplodingBarrel *)0)->particleModelDefHandle), sizeof( ((idExplodingBarrel *)0)->particleModelDefHandle ) },
	{ "qhandle_t", "lightDefHandle", (intptr_t)(&((idExplodingBarrel *)0)->lightDefHandle), sizeof( ((idExplodingBarrel *)0)->lightDefHandle ) },
	{ "renderEntity_t", "particleRenderEntity", (intptr_t)(&((idExplodingBarrel *)0)->particleRenderEntity), sizeof( ((idExplodingBarrel *)0)->particleRenderEntity ) },
	{ "renderLight_t", "light", (intptr_t)(&((idExplodingBarrel *)0)->light), sizeof( ((idExplodingBarrel *)0)->light ) },
	{ "int", "particleTime", (intptr_t)(&((idExplodingBarrel *)0)->particleTime), sizeof( ((idExplodingBarrel *)0)->particleTime ) },
	{ "int", "lightTime", (intptr_t)(&((idExplodingBarrel *)0)->lightTime), sizeof( ((idExplodingBarrel *)0)->lightTime ) },
	{ "float", "time", (intptr_t)(&((idExplodingBarrel *)0)->time), sizeof( ((idExplodingBarrel *)0)->time ) },
	{ "bool", "isStable", (intptr_t)(&((idExplodingBarrel *)0)->isStable), sizeof( ((idExplodingBarrel *)0)->isStable ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_Remove_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_Show_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_Damage_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SessionCommand_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_EndLevel_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_WaitForButton_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetGlobalShaderTime_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetShaderParm_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetShaderTime_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_FadeEntity_typeInfo[] = {
	{ ": idVec4", "fadeFrom", (intptr_t)(&((idTarget_FadeEntity *)0)->fadeFrom), sizeof( ((idTarget_FadeEntity *)0)->fadeFrom ) },
	{ "int", "fadeStart", (intptr_t)(&((idTarget_FadeEntity *)0)->fadeStart), sizeof( ((idTarget_FadeEntity *)0)->fadeStart ) },
	{ "int", "fadeEnd", (intptr_t)(&((idTarget_FadeEntity *)0)->fadeEnd), sizeof( ((idTarget_FadeEntity *)0)->fadeEnd ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_LightFadeIn_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_LightFadeOut_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_Give_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_GiveEmail_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetModel_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t SavedGui_t_typeInfo[] = {
	{ "idUserInterface *[1]", "gui", (intptr_t)(&((SavedGui_t *)0)->gui), sizeof( ((SavedGui_t *)0)->gui ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetInfluence_typeInfo[] = {
	{ "idList < int , TAG_TARGET >", "lightList", (intptr_t)(&((idTarget_SetInfluence *)0)->lightList), sizeof( ((idTarget_SetInfluence *)0)->lightList ) },
	{ "idList < int , TAG_TARGET >", "guiList", (intptr_t)(&((idTarget_SetInfluence *)0)->guiList), sizeof( ((idTarget_SetInfluence *)0)->guiList ) },
	{ "idList < int , TAG_TARGET >", "soundList", (intptr_t)(&((idTarget_SetInfluence *)0)->soundList), sizeof( ((idTarget_SetInfluence *)0)->soundList ) },
	{ "idList < int , TAG_TARGET >", "genericList", (intptr_t)(&((idTarget_SetInfluence *)0)->genericList), sizeof( ((idTarget_SetInfluence *)0)->genericList ) },
	{ "float", "flashIn", (intptr_t)(&((idTarget_SetInfluence *)0)->flashIn), sizeof( ((idTarget_SetInfluence *)0)->flashIn ) },
	{ "float", "flashOut", (intptr_t)(&((idTarget_SetInfluence *)0)->flashOut), sizeof( ((idTarget_SetInfluence *)0)->flashOut ) },
	{ "float", "delay", (intptr_t)(&((idTarget_SetInfluence *)0)->delay), sizeof( ((idTarget_SetInfluence *)0)->delay ) },
	{ "idStr", "flashInSound", (intptr_t)(&((idTarget_SetInfluence *)0)->flashInSound), sizeof( ((idTarget_SetInfluence *)0)->flashInSound ) },
	{ "idStr", "flashOutSound", (intptr_t)(&((idTarget_SetInfluence *)0)->flashOutSound), sizeof( ((idTarget_SetInfluence *)0)->flashOutSound ) },
	{ "idEntity *", "switchToCamera", (intptr_t)(&((idTarget_SetInfluence *)0)->switchToCamera), sizeof( ((idTarget_SetInfluence *)0)->switchToCamera ) },
	{ "idInterpolate < float >", "fovSetting", (intptr_t)(&((idTarget_SetInfluence *)0)->fovSetting), sizeof( ((idTarget_SetInfluence *)0)->fovSetting ) },
	{ "bool", "soundFaded", (intptr_t)(&((idTarget_SetInfluence *)0)->soundFaded), sizeof( ((idTarget_SetInfluence *)0)->soundFaded ) },
	{ "bool", "restoreOnTrigger", (intptr_t)(&((idTarget_SetInfluence *)0)->restoreOnTrigger), sizeof( ((idTarget_SetInfluence *)0)->restoreOnTrigger ) },
	{ "idList < SavedGui_t , TAG_TARGET >", "savedGuiList", (intptr_t)(&((idTarget_SetInfluence *)0)->savedGuiList), sizeof( ((idTarget_SetInfluence *)0)->savedGuiList ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetKeyVal_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetFov_typeInfo[] = {
	{ ": idInterpolate < float >", "fovSetting", (intptr_t)(&((idTarget_SetFov *)0)->fovSetting), sizeof( ((idTarget_SetFov *)0)->fovSetting ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_SetPrimaryObjective_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_LockDoor_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_CallObjectFunction_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_EnableLevelWeapons_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_Tip_typeInfo[] = {
	{ ": idVec3", "playerPos", (intptr_t)(&((idTarget_Tip *)0)->playerPos), sizeof( ((idTarget_Tip *)0)->playerPos ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_GiveSecurity_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_RemoveWeapons_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_LevelTrigger_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_Checkpoint_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_EnableStamina_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_FadeSoundClass_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_RumbleJoystick_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTarget_Achievement_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_typeInfo[] = {
	{ "const function_t *", "scriptFunction", (intptr_t)(&((idTrigger *)0)->scriptFunction), sizeof( ((idTrigger *)0)->scriptFunction ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_Multi_typeInfo[] = {
	{ ": float", "wait", (intptr_t)(&((idTrigger_Multi *)0)->wait), sizeof( ((idTrigger_Multi *)0)->wait ) },
	{ "float", "random", (intptr_t)(&((idTrigger_Multi *)0)->random), sizeof( ((idTrigger_Multi *)0)->random ) },
	{ "float", "delay", (intptr_t)(&((idTrigger_Multi *)0)->delay), sizeof( ((idTrigger_Multi *)0)->delay ) },
	{ "float", "random_delay", (intptr_t)(&((idTrigger_Multi *)0)->random_delay), sizeof( ((idTrigger_Multi *)0)->random_delay ) },
	{ "int", "nextTriggerTime", (intptr_t)(&((idTrigger_Multi *)0)->nextTriggerTime), sizeof( ((idTrigger_Multi *)0)->nextTriggerTime ) },
	{ "idStr", "requires", (intptr_t)(&((idTrigger_Multi *)0)->requires), sizeof( ((idTrigger_Multi *)0)->requires ) },
	{ "int", "removeItem", (intptr_t)(&((idTrigger_Multi *)0)->removeItem), sizeof( ((idTrigger_Multi *)0)->removeItem ) },
	{ "bool", "touchClient", (intptr_t)(&((idTrigger_Multi *)0)->touchClient), sizeof( ((idTrigger_Multi *)0)->touchClient ) },
	{ "bool", "touchOther", (intptr_t)(&((idTrigger_Multi *)0)->touchOther), sizeof( ((idTrigger_Multi *)0)->touchOther ) },
	{ "bool", "triggerFirst", (intptr_t)(&((idTrigger_Multi *)0)->triggerFirst), sizeof( ((idTrigger_Multi *)0)->triggerFirst ) },
	{ "bool", "triggerWithSelf", (intptr_t)(&((idTrigger_Multi *)0)->triggerWithSelf), sizeof( ((idTrigger_Multi *)0)->triggerWithSelf ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_EntityName_typeInfo[] = {
	{ ": float", "wait", (intptr_t)(&((idTrigger_EntityName *)0)->wait), sizeof( ((idTrigger_EntityName *)0)->wait ) },
	{ "float", "random", (intptr_t)(&((idTrigger_EntityName *)0)->random), sizeof( ((idTrigger_EntityName *)0)->random ) },
	{ "float", "delay", (intptr_t)(&((idTrigger_EntityName *)0)->delay), sizeof( ((idTrigger_EntityName *)0)->delay ) },
	{ "float", "random_delay", (intptr_t)(&((idTrigger_EntityName *)0)->random_delay), sizeof( ((idTrigger_EntityName *)0)->random_delay ) },
	{ "int", "nextTriggerTime", (intptr_t)(&((idTrigger_EntityName *)0)->nextTriggerTime), sizeof( ((idTrigger_EntityName *)0)->nextTriggerTime ) },
	{ "bool", "triggerFirst", (intptr_t)(&((idTrigger_EntityName *)0)->triggerFirst), sizeof( ((idTrigger_EntityName *)0)->triggerFirst ) },
	{ "idStr", "entityName", (intptr_t)(&((idTrigger_EntityName *)0)->entityName), sizeof( ((idTrigger_EntityName *)0)->entityName ) },
	{ "bool", "testPartialName", (intptr_t)(&((idTrigger_EntityName *)0)->testPartialName), sizeof( ((idTrigger_EntityName *)0)->testPartialName ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_Timer_typeInfo[] = {
	{ ": float", "random", (intptr_t)(&((idTrigger_Timer *)0)->random), sizeof( ((idTrigger_Timer *)0)->random ) },
	{ "float", "wait", (intptr_t)(&((idTrigger_Timer *)0)->wait), sizeof( ((idTrigger_Timer *)0)->wait ) },
	{ "bool", "on", (intptr_t)(&((idTrigger_Timer *)0)->on), sizeof( ((idTrigger_Timer *)0)->on ) },
	{ "float", "delay", (intptr_t)(&((idTrigger_Timer *)0)->delay), sizeof( ((idTrigger_Timer *)0)->delay ) },
	{ "idStr", "onName", (intptr_t)(&((idTrigger_Timer *)0)->onName), sizeof( ((idTrigger_Timer *)0)->onName ) },
	{ "idStr", "offName", (intptr_t)(&((idTrigger_Timer *)0)->offName), sizeof( ((idTrigger_Timer *)0)->offName ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_Count_typeInfo[] = {
	{ ": int", "goal", (intptr_t)(&((idTrigger_Count *)0)->goal), sizeof( ((idTrigger_Count *)0)->goal ) },
	{ "int", "count", (intptr_t)(&((idTrigger_Count *)0)->count), sizeof( ((idTrigger_Count *)0)->count ) },
	{ "float", "delay", (intptr_t)(&((idTrigger_Count *)0)->delay), sizeof( ((idTrigger_Count *)0)->delay ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_Hurt_typeInfo[] = {
	{ ": bool", "on", (intptr_t)(&((idTrigger_Hurt *)0)->on), sizeof( ((idTrigger_Hurt *)0)->on ) },
	{ "float", "delay", (intptr_t)(&((idTrigger_Hurt *)0)->delay), sizeof( ((idTrigger_Hurt *)0)->delay ) },
	{ "int", "nextTime", (intptr_t)(&((idTrigger_Hurt *)0)->nextTime), sizeof( ((idTrigger_Hurt *)0)->nextTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_Fade_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_Touch_typeInfo[] = {
	{ ": idClipModel *", "clipModel", (intptr_t)(&((idTrigger_Touch *)0)->clipModel), sizeof( ((idTrigger_Touch *)0)->clipModel ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTrigger_Flag_typeInfo[] = {
	{ ": int", "team", (intptr_t)(&((idTrigger_Flag *)0)->team), sizeof( ((idTrigger_Flag *)0)->team ) },
	{ "bool", "player", (intptr_t)(&((idTrigger_Flag *)0)->player), sizeof( ((idTrigger_Flag *)0)->player ) },
	{ "const idEventDef *", "eventFlag", (intptr_t)(&((idTrigger_Flag *)0)->eventFlag), sizeof( ((idTrigger_Flag *)0)->eventFlag ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSound_typeInfo[] = {
	{ ": float", "lastSoundVol", (intptr_t)(&((idSound *)0)->lastSoundVol), sizeof( ((idSound *)0)->lastSoundVol ) },
	{ "float", "soundVol", (intptr_t)(&((idSound *)0)->soundVol), sizeof( ((idSound *)0)->soundVol ) },
	{ "float", "random", (intptr_t)(&((idSound *)0)->random), sizeof( ((idSound *)0)->random ) },
	{ "float", "wait", (intptr_t)(&((idSound *)0)->wait), sizeof( ((idSound *)0)->wait ) },
	{ "bool", "timerOn", (intptr_t)(&((idSound *)0)->timerOn), sizeof( ((idSound *)0)->timerOn ) },
	{ "idVec3", "shakeTranslate", (intptr_t)(&((idSound *)0)->shakeTranslate), sizeof( ((idSound *)0)->shakeTranslate ) },
	{ "idAngles", "shakeRotate", (intptr_t)(&((idSound *)0)->shakeRotate), sizeof( ((idSound *)0)->shakeRotate ) },
	{ "int", "playingUntilTime", (intptr_t)(&((idSound *)0)->playingUntilTime), sizeof( ((idSound *)0)->playingUntilTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t idFXLocalAction_typeInfo[] = {
	{ "renderLight_t", "renderLight", (intptr_t)(&((idFXLocalAction *)0)->renderLight), sizeof( ((idFXLocalAction *)0)->renderLight ) },
	{ "qhandle_t", "lightDefHandle", (intptr_t)(&((idFXLocalAction *)0)->lightDefHandle), sizeof( ((idFXLocalAction *)0)->lightDefHandle ) },
	{ "renderEntity_t", "renderEntity", (intptr_t)(&((idFXLocalAction *)0)->renderEntity), sizeof( ((idFXLocalAction *)0)->renderEntity ) },
	{ "int", "modelDefHandle", (intptr_t)(&((idFXLocalAction *)0)->modelDefHandle), sizeof( ((idFXLocalAction *)0)->modelDefHandle ) },
	{ "float", "delay", (intptr_t)(&((idFXLocalAction *)0)->delay), sizeof( ((idFXLocalAction *)0)->delay ) },
	{ "int", "particleSystem", (intptr_t)(&((idFXLocalAction *)0)->particleSystem), sizeof( ((idFXLocalAction *)0)->particleSystem ) },
	{ "int", "start", (intptr_t)(&((idFXLocalAction *)0)->start), sizeof( ((idFXLocalAction *)0)->start ) },
	{ "bool", "soundStarted", (intptr_t)(&((idFXLocalAction *)0)->soundStarted), sizeof( ((idFXLocalAction *)0)->soundStarted ) },
	{ "bool", "shakeStarted", (intptr_t)(&((idFXLocalAction *)0)->shakeStarted), sizeof( ((idFXLocalAction *)0)->shakeStarted ) },
	{ "bool", "decalDropped", (intptr_t)(&((idFXLocalAction *)0)->decalDropped), sizeof( ((idFXLocalAction *)0)->decalDropped ) },
	{ "bool", "launched", (intptr_t)(&((idFXLocalAction *)0)->launched), sizeof( ((idFXLocalAction *)0)->launched ) },
	{ NULL, 0 }
};

static classVariableInfo_t idEntityFx_typeInfo[] = {
	{ "int", "started", (intptr_t)(&((idEntityFx *)0)->started), sizeof( ((idEntityFx *)0)->started ) },
	{ "int", "nextTriggerTime", (intptr_t)(&((idEntityFx *)0)->nextTriggerTime), sizeof( ((idEntityFx *)0)->nextTriggerTime ) },
	{ "const idDeclFX *", "fxEffect", (intptr_t)(&((idEntityFx *)0)->fxEffect), sizeof( ((idEntityFx *)0)->fxEffect ) },
	{ "idList < idFXLocalAction , TAG_FX >", "actions", (intptr_t)(&((idEntityFx *)0)->actions), sizeof( ((idEntityFx *)0)->actions ) },
	{ "idStr", "systemName", (intptr_t)(&((idEntityFx *)0)->systemName), sizeof( ((idEntityFx *)0)->systemName ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTeleporter_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idSecurityCamera_typeInfo[] = {
	{ "float", "angle", (intptr_t)(&((idSecurityCamera *)0)->angle), sizeof( ((idSecurityCamera *)0)->angle ) },
	{ "float", "sweepAngle", (intptr_t)(&((idSecurityCamera *)0)->sweepAngle), sizeof( ((idSecurityCamera *)0)->sweepAngle ) },
	{ "int", "modelAxis", (intptr_t)(&((idSecurityCamera *)0)->modelAxis), sizeof( ((idSecurityCamera *)0)->modelAxis ) },
	{ "bool", "flipAxis", (intptr_t)(&((idSecurityCamera *)0)->flipAxis), sizeof( ((idSecurityCamera *)0)->flipAxis ) },
	{ "float", "scanDist", (intptr_t)(&((idSecurityCamera *)0)->scanDist), sizeof( ((idSecurityCamera *)0)->scanDist ) },
	{ "float", "scanFov", (intptr_t)(&((idSecurityCamera *)0)->scanFov), sizeof( ((idSecurityCamera *)0)->scanFov ) },
	{ "float", "sweepStart", (intptr_t)(&((idSecurityCamera *)0)->sweepStart), sizeof( ((idSecurityCamera *)0)->sweepStart ) },
	{ "float", "sweepEnd", (intptr_t)(&((idSecurityCamera *)0)->sweepEnd), sizeof( ((idSecurityCamera *)0)->sweepEnd ) },
	{ "bool", "negativeSweep", (intptr_t)(&((idSecurityCamera *)0)->negativeSweep), sizeof( ((idSecurityCamera *)0)->negativeSweep ) },
	{ "bool", "sweeping", (intptr_t)(&((idSecurityCamera *)0)->sweeping), sizeof( ((idSecurityCamera *)0)->sweeping ) },
	{ "int", "alertMode", (intptr_t)(&((idSecurityCamera *)0)->alertMode), sizeof( ((idSecurityCamera *)0)->alertMode ) },
	{ "float", "stopSweeping", (intptr_t)(&((idSecurityCamera *)0)->stopSweeping), sizeof( ((idSecurityCamera *)0)->stopSweeping ) },
	{ "float", "scanFovCos", (intptr_t)(&((idSecurityCamera *)0)->scanFovCos), sizeof( ((idSecurityCamera *)0)->scanFovCos ) },
	{ "idVec3", "viewOffset", (intptr_t)(&((idSecurityCamera *)0)->viewOffset), sizeof( ((idSecurityCamera *)0)->viewOffset ) },
	{ "int", "pvsArea", (intptr_t)(&((idSecurityCamera *)0)->pvsArea), sizeof( ((idSecurityCamera *)0)->pvsArea ) },
	{ "idPhysics_RigidBody", "physicsObj", (intptr_t)(&((idSecurityCamera *)0)->physicsObj), sizeof( ((idSecurityCamera *)0)->physicsObj ) },
	{ "idTraceModel", "trm", (intptr_t)(&((idSecurityCamera *)0)->trm), sizeof( ((idSecurityCamera *)0)->trm ) },
	{ NULL, 0 }
};

static classVariableInfo_t shard_t_typeInfo[] = {
	{ "idClipModel *", "clipModel", (intptr_t)(&((shard_t *)0)->clipModel), sizeof( ((shard_t *)0)->clipModel ) },
	{ "idFixedWinding", "winding", (intptr_t)(&((shard_t *)0)->winding), sizeof( ((shard_t *)0)->winding ) },
	{ "idList < idFixedWinding * , TAG_PHYSICS_BRITTLE >", "decals", (intptr_t)(&((shard_t *)0)->decals), sizeof( ((shard_t *)0)->decals ) },
	{ "idList < bool >", "edgeHasNeighbour", (intptr_t)(&((shard_t *)0)->edgeHasNeighbour), sizeof( ((shard_t *)0)->edgeHasNeighbour ) },
	{ "idList < struct shard_s * , TAG_PHYSICS_BRITTLE >", "neighbours", (intptr_t)(&((shard_t *)0)->neighbours), sizeof( ((shard_t *)0)->neighbours ) },
	{ "idPhysics_RigidBody", "physicsObj", (intptr_t)(&((shard_t *)0)->physicsObj), sizeof( ((shard_t *)0)->physicsObj ) },
	{ "int", "droppedTime", (intptr_t)(&((shard_t *)0)->droppedTime), sizeof( ((shard_t *)0)->droppedTime ) },
	{ "bool", "atEdge", (intptr_t)(&((shard_t *)0)->atEdge), sizeof( ((shard_t *)0)->atEdge ) },
	{ "int", "islandNum", (intptr_t)(&((shard_t *)0)->islandNum), sizeof( ((shard_t *)0)->islandNum ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBrittleFracture_fractureEvent_s_typeInfo[] = {
	{ "int", "eventType", (intptr_t)(&((idBrittleFracture::fractureEvent_s *)0)->eventType), sizeof( ((idBrittleFracture::fractureEvent_s *)0)->eventType ) },
	{ "idVec3", "point", (intptr_t)(&((idBrittleFracture::fractureEvent_s *)0)->point), sizeof( ((idBrittleFracture::fractureEvent_s *)0)->point ) },
	{ "idVec3", "vector", (intptr_t)(&((idBrittleFracture::fractureEvent_s *)0)->vector), sizeof( ((idBrittleFracture::fractureEvent_s *)0)->vector ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBrittleFracture_typeInfo[] = {
	{ ": const idMaterial *", "material", (intptr_t)(&((idBrittleFracture *)0)->material), sizeof( ((idBrittleFracture *)0)->material ) },
	{ "const idMaterial *", "decalMaterial", (intptr_t)(&((idBrittleFracture *)0)->decalMaterial), sizeof( ((idBrittleFracture *)0)->decalMaterial ) },
	{ "float", "decalSize", (intptr_t)(&((idBrittleFracture *)0)->decalSize), sizeof( ((idBrittleFracture *)0)->decalSize ) },
	{ "float", "maxShardArea", (intptr_t)(&((idBrittleFracture *)0)->maxShardArea), sizeof( ((idBrittleFracture *)0)->maxShardArea ) },
	{ "float", "maxShatterRadius", (intptr_t)(&((idBrittleFracture *)0)->maxShatterRadius), sizeof( ((idBrittleFracture *)0)->maxShatterRadius ) },
	{ "float", "minShatterRadius", (intptr_t)(&((idBrittleFracture *)0)->minShatterRadius), sizeof( ((idBrittleFracture *)0)->minShatterRadius ) },
	{ "float", "linearVelocityScale", (intptr_t)(&((idBrittleFracture *)0)->linearVelocityScale), sizeof( ((idBrittleFracture *)0)->linearVelocityScale ) },
	{ "float", "angularVelocityScale", (intptr_t)(&((idBrittleFracture *)0)->angularVelocityScale), sizeof( ((idBrittleFracture *)0)->angularVelocityScale ) },
	{ "float", "shardMass", (intptr_t)(&((idBrittleFracture *)0)->shardMass), sizeof( ((idBrittleFracture *)0)->shardMass ) },
	{ "float", "density", (intptr_t)(&((idBrittleFracture *)0)->density), sizeof( ((idBrittleFracture *)0)->density ) },
	{ "float", "friction", (intptr_t)(&((idBrittleFracture *)0)->friction), sizeof( ((idBrittleFracture *)0)->friction ) },
	{ "float", "bouncyness", (intptr_t)(&((idBrittleFracture *)0)->bouncyness), sizeof( ((idBrittleFracture *)0)->bouncyness ) },
	{ "idStr", "fxFracture", (intptr_t)(&((idBrittleFracture *)0)->fxFracture), sizeof( ((idBrittleFracture *)0)->fxFracture ) },
	{ "idList < fractureEvent_s >", "storedEvents", (intptr_t)(&((idBrittleFracture *)0)->storedEvents), sizeof( ((idBrittleFracture *)0)->storedEvents ) },
	{ "bool", "processStoredEvents", (intptr_t)(&((idBrittleFracture *)0)->processStoredEvents), sizeof( ((idBrittleFracture *)0)->processStoredEvents ) },
	{ "idRenderModel *", "defaultRenderModel", (intptr_t)(&((idBrittleFracture *)0)->defaultRenderModel), sizeof( ((idBrittleFracture *)0)->defaultRenderModel ) },
	{ "bool", "isXraySurface", (intptr_t)(&((idBrittleFracture *)0)->isXraySurface), sizeof( ((idBrittleFracture *)0)->isXraySurface ) },
	{ "idPhysics_StaticMulti", "physicsObj", (intptr_t)(&((idBrittleFracture *)0)->physicsObj), sizeof( ((idBrittleFracture *)0)->physicsObj ) },
	{ "idList < shard_t * , TAG_PHYSICS_BRITTLE >", "shards", (intptr_t)(&((idBrittleFracture *)0)->shards), sizeof( ((idBrittleFracture *)0)->shards ) },
	{ "idBounds", "bounds", (intptr_t)(&((idBrittleFracture *)0)->bounds), sizeof( ((idBrittleFracture *)0)->bounds ) },
	{ "bool", "disableFracture", (intptr_t)(&((idBrittleFracture *)0)->disableFracture), sizeof( ((idBrittleFracture *)0)->disableFracture ) },
	{ "mutable int", "lastRenderEntityUpdate", (intptr_t)(&((idBrittleFracture *)0)->lastRenderEntityUpdate), sizeof( ((idBrittleFracture *)0)->lastRenderEntityUpdate ) },
	{ "mutable bool", "changed", (intptr_t)(&((idBrittleFracture *)0)->changed), sizeof( ((idBrittleFracture *)0)->changed ) },
	{ NULL, 0 }
};

static classVariableInfo_t ballistics_t_typeInfo[] = {
	{ "float", "angle", (intptr_t)(&((ballistics_t *)0)->angle), sizeof( ((ballistics_t *)0)->angle ) },
	{ "float", "time", (intptr_t)(&((ballistics_t *)0)->time), sizeof( ((ballistics_t *)0)->time ) },
	{ NULL, 0 }
};

static classVariableInfo_t obstaclePath_t_typeInfo[] = {
	{ "idVec3", "seekPos", (intptr_t)(&((obstaclePath_t *)0)->seekPos), sizeof( ((obstaclePath_t *)0)->seekPos ) },
	{ "idEntity *", "firstObstacle", (intptr_t)(&((obstaclePath_t *)0)->firstObstacle), sizeof( ((obstaclePath_t *)0)->firstObstacle ) },
	{ "idVec3", "startPosOutsideObstacles", (intptr_t)(&((obstaclePath_t *)0)->startPosOutsideObstacles), sizeof( ((obstaclePath_t *)0)->startPosOutsideObstacles ) },
	{ "idEntity *", "startPosObstacle", (intptr_t)(&((obstaclePath_t *)0)->startPosObstacle), sizeof( ((obstaclePath_t *)0)->startPosObstacle ) },
	{ "idVec3", "seekPosOutsideObstacles", (intptr_t)(&((obstaclePath_t *)0)->seekPosOutsideObstacles), sizeof( ((obstaclePath_t *)0)->seekPosOutsideObstacles ) },
	{ "idEntity *", "seekPosObstacle", (intptr_t)(&((obstaclePath_t *)0)->seekPosObstacle), sizeof( ((obstaclePath_t *)0)->seekPosObstacle ) },
	{ NULL, 0 }
};

static classVariableInfo_t predictedPath_t_typeInfo[] = {
	{ "idVec3", "endPos", (intptr_t)(&((predictedPath_t *)0)->endPos), sizeof( ((predictedPath_t *)0)->endPos ) },
	{ "idVec3", "endVelocity", (intptr_t)(&((predictedPath_t *)0)->endVelocity), sizeof( ((predictedPath_t *)0)->endVelocity ) },
	{ "idVec3", "endNormal", (intptr_t)(&((predictedPath_t *)0)->endNormal), sizeof( ((predictedPath_t *)0)->endNormal ) },
	{ "int", "endTime", (intptr_t)(&((predictedPath_t *)0)->endTime), sizeof( ((predictedPath_t *)0)->endTime ) },
	{ "int", "endEvent", (intptr_t)(&((predictedPath_t *)0)->endEvent), sizeof( ((predictedPath_t *)0)->endEvent ) },
	{ "const idEntity *", "blockingEntity", (intptr_t)(&((predictedPath_t *)0)->blockingEntity), sizeof( ((predictedPath_t *)0)->blockingEntity ) },
	{ NULL, 0 }
};

static classVariableInfo_t particleEmitter_t_typeInfo[] = {
	{ "const idDeclParticle *", "particle", (intptr_t)(&((particleEmitter_t *)0)->particle), sizeof( ((particleEmitter_t *)0)->particle ) },
	{ "int", "time", (intptr_t)(&((particleEmitter_t *)0)->time), sizeof( ((particleEmitter_t *)0)->time ) },
	{ "jointHandle_t", "joint", (intptr_t)(&((particleEmitter_t *)0)->joint), sizeof( ((particleEmitter_t *)0)->joint ) },
	{ NULL, 0 }
};

static classVariableInfo_t funcEmitter_t_typeInfo[] = {
	{ "char[64]", "name", (intptr_t)(&((funcEmitter_t *)0)->name), sizeof( ((funcEmitter_t *)0)->name ) },
	{ "idFuncEmitter *", "particle", (intptr_t)(&((funcEmitter_t *)0)->particle), sizeof( ((funcEmitter_t *)0)->particle ) },
	{ "jointHandle_t", "joint", (intptr_t)(&((funcEmitter_t *)0)->joint), sizeof( ((funcEmitter_t *)0)->joint ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMoveState_typeInfo[] = {
	{ "moveType_t", "moveType", (intptr_t)(&((idMoveState *)0)->moveType), sizeof( ((idMoveState *)0)->moveType ) },
	{ "moveCommand_t", "moveCommand", (intptr_t)(&((idMoveState *)0)->moveCommand), sizeof( ((idMoveState *)0)->moveCommand ) },
	{ "moveStatus_t", "moveStatus", (intptr_t)(&((idMoveState *)0)->moveStatus), sizeof( ((idMoveState *)0)->moveStatus ) },
	{ "idVec3", "moveDest", (intptr_t)(&((idMoveState *)0)->moveDest), sizeof( ((idMoveState *)0)->moveDest ) },
	{ "idVec3", "moveDir", (intptr_t)(&((idMoveState *)0)->moveDir), sizeof( ((idMoveState *)0)->moveDir ) },
	{ "idEntityPtr < idEntity >", "goalEntity", (intptr_t)(&((idMoveState *)0)->goalEntity), sizeof( ((idMoveState *)0)->goalEntity ) },
	{ "idVec3", "goalEntityOrigin", (intptr_t)(&((idMoveState *)0)->goalEntityOrigin), sizeof( ((idMoveState *)0)->goalEntityOrigin ) },
	{ "int", "toAreaNum", (intptr_t)(&((idMoveState *)0)->toAreaNum), sizeof( ((idMoveState *)0)->toAreaNum ) },
	{ "int", "startTime", (intptr_t)(&((idMoveState *)0)->startTime), sizeof( ((idMoveState *)0)->startTime ) },
	{ "int", "duration", (intptr_t)(&((idMoveState *)0)->duration), sizeof( ((idMoveState *)0)->duration ) },
	{ "float", "speed", (intptr_t)(&((idMoveState *)0)->speed), sizeof( ((idMoveState *)0)->speed ) },
	{ "float", "range", (intptr_t)(&((idMoveState *)0)->range), sizeof( ((idMoveState *)0)->range ) },
	{ "float", "wanderYaw", (intptr_t)(&((idMoveState *)0)->wanderYaw), sizeof( ((idMoveState *)0)->wanderYaw ) },
	{ "int", "nextWanderTime", (intptr_t)(&((idMoveState *)0)->nextWanderTime), sizeof( ((idMoveState *)0)->nextWanderTime ) },
	{ "int", "blockTime", (intptr_t)(&((idMoveState *)0)->blockTime), sizeof( ((idMoveState *)0)->blockTime ) },
	{ "idEntityPtr < idEntity >", "obstacle", (intptr_t)(&((idMoveState *)0)->obstacle), sizeof( ((idMoveState *)0)->obstacle ) },
	{ "idVec3", "lastMoveOrigin", (intptr_t)(&((idMoveState *)0)->lastMoveOrigin), sizeof( ((idMoveState *)0)->lastMoveOrigin ) },
	{ "int", "lastMoveTime", (intptr_t)(&((idMoveState *)0)->lastMoveTime), sizeof( ((idMoveState *)0)->lastMoveTime ) },
	{ "int", "anim", (intptr_t)(&((idMoveState *)0)->anim), sizeof( ((idMoveState *)0)->anim ) },
	{ NULL, 0 }
};

static classVariableInfo_t idAI_typeInfo[] = {
	{ ": idScriptBool", "ambush", (intptr_t)(&((idAI *)0)->ambush), sizeof( ((idAI *)0)->ambush ) },
	{ "idScriptBool", "ignoreEnemies", (intptr_t)(&((idAI *)0)->ignoreEnemies), sizeof( ((idAI *)0)->ignoreEnemies ) },
	{ "idScriptBool", "stay_on_attackpath", (intptr_t)(&((idAI *)0)->stay_on_attackpath), sizeof( ((idAI *)0)->stay_on_attackpath ) },
	{ "idScriptBool", "ignore_sight", (intptr_t)(&((idAI *)0)->ignore_sight), sizeof( ((idAI *)0)->ignore_sight ) },
	{ "idScriptBool", "idle_sight_fov", (intptr_t)(&((idAI *)0)->idle_sight_fov), sizeof( ((idAI *)0)->idle_sight_fov ) },
	{ "idAAS *", "aas", (intptr_t)(&((idAI *)0)->aas), sizeof( ((idAI *)0)->aas ) },
	{ "int", "travelFlags", (intptr_t)(&((idAI *)0)->travelFlags), sizeof( ((idAI *)0)->travelFlags ) },
	{ "idMoveState", "move", (intptr_t)(&((idAI *)0)->move), sizeof( ((idAI *)0)->move ) },
	{ "idMoveState", "savedMove", (intptr_t)(&((idAI *)0)->savedMove), sizeof( ((idAI *)0)->savedMove ) },
	{ "float", "kickForce", (intptr_t)(&((idAI *)0)->kickForce), sizeof( ((idAI *)0)->kickForce ) },
	{ "bool", "ignore_obstacles", (intptr_t)(&((idAI *)0)->ignore_obstacles), sizeof( ((idAI *)0)->ignore_obstacles ) },
	{ "float", "blockedRadius", (intptr_t)(&((idAI *)0)->blockedRadius), sizeof( ((idAI *)0)->blockedRadius ) },
	{ "int", "blockedMoveTime", (intptr_t)(&((idAI *)0)->blockedMoveTime), sizeof( ((idAI *)0)->blockedMoveTime ) },
	{ "int", "blockedAttackTime", (intptr_t)(&((idAI *)0)->blockedAttackTime), sizeof( ((idAI *)0)->blockedAttackTime ) },
	{ "float", "ideal_yaw", (intptr_t)(&((idAI *)0)->ideal_yaw), sizeof( ((idAI *)0)->ideal_yaw ) },
	{ "float", "current_yaw", (intptr_t)(&((idAI *)0)->current_yaw), sizeof( ((idAI *)0)->current_yaw ) },
	{ "float", "turnRate", (intptr_t)(&((idAI *)0)->turnRate), sizeof( ((idAI *)0)->turnRate ) },
	{ "float", "turnVel", (intptr_t)(&((idAI *)0)->turnVel), sizeof( ((idAI *)0)->turnVel ) },
	{ "float", "anim_turn_yaw", (intptr_t)(&((idAI *)0)->anim_turn_yaw), sizeof( ((idAI *)0)->anim_turn_yaw ) },
	{ "float", "anim_turn_amount", (intptr_t)(&((idAI *)0)->anim_turn_amount), sizeof( ((idAI *)0)->anim_turn_amount ) },
	{ "float", "anim_turn_angles", (intptr_t)(&((idAI *)0)->anim_turn_angles), sizeof( ((idAI *)0)->anim_turn_angles ) },
	{ "idPhysics_Monster", "physicsObj", (intptr_t)(&((idAI *)0)->physicsObj), sizeof( ((idAI *)0)->physicsObj ) },
	{ "jointHandle_t", "flyTiltJoint", (intptr_t)(&((idAI *)0)->flyTiltJoint), sizeof( ((idAI *)0)->flyTiltJoint ) },
	{ "float", "fly_speed", (intptr_t)(&((idAI *)0)->fly_speed), sizeof( ((idAI *)0)->fly_speed ) },
	{ "float", "fly_bob_strength", (intptr_t)(&((idAI *)0)->fly_bob_strength), sizeof( ((idAI *)0)->fly_bob_strength ) },
	{ "float", "fly_bob_vert", (intptr_t)(&((idAI *)0)->fly_bob_vert), sizeof( ((idAI *)0)->fly_bob_vert ) },
	{ "float", "fly_bob_horz", (intptr_t)(&((idAI *)0)->fly_bob_horz), sizeof( ((idAI *)0)->fly_bob_horz ) },
	{ "int", "fly_offset", (intptr_t)(&((idAI *)0)->fly_offset), sizeof( ((idAI *)0)->fly_offset ) },
	{ "float", "fly_seek_scale", (intptr_t)(&((idAI *)0)->fly_seek_scale), sizeof( ((idAI *)0)->fly_seek_scale ) },
	{ "float", "fly_roll_scale", (intptr_t)(&((idAI *)0)->fly_roll_scale), sizeof( ((idAI *)0)->fly_roll_scale ) },
	{ "float", "fly_roll_max", (intptr_t)(&((idAI *)0)->fly_roll_max), sizeof( ((idAI *)0)->fly_roll_max ) },
	{ "float", "fly_roll", (intptr_t)(&((idAI *)0)->fly_roll), sizeof( ((idAI *)0)->fly_roll ) },
	{ "float", "fly_pitch_scale", (intptr_t)(&((idAI *)0)->fly_pitch_scale), sizeof( ((idAI *)0)->fly_pitch_scale ) },
	{ "float", "fly_pitch_max", (intptr_t)(&((idAI *)0)->fly_pitch_max), sizeof( ((idAI *)0)->fly_pitch_max ) },
	{ "float", "fly_pitch", (intptr_t)(&((idAI *)0)->fly_pitch), sizeof( ((idAI *)0)->fly_pitch ) },
	{ "bool", "allowMove", (intptr_t)(&((idAI *)0)->allowMove), sizeof( ((idAI *)0)->allowMove ) },
	{ "bool", "allowHiddenMovement", (intptr_t)(&((idAI *)0)->allowHiddenMovement), sizeof( ((idAI *)0)->allowHiddenMovement ) },
	{ "bool", "disableGravity", (intptr_t)(&((idAI *)0)->disableGravity), sizeof( ((idAI *)0)->disableGravity ) },
	{ "bool", "af_push_moveables", (intptr_t)(&((idAI *)0)->af_push_moveables), sizeof( ((idAI *)0)->af_push_moveables ) },
	{ "bool", "lastHitCheckResult", (intptr_t)(&((idAI *)0)->lastHitCheckResult), sizeof( ((idAI *)0)->lastHitCheckResult ) },
	{ "int", "lastHitCheckTime", (intptr_t)(&((idAI *)0)->lastHitCheckTime), sizeof( ((idAI *)0)->lastHitCheckTime ) },
	{ "int", "lastAttackTime", (intptr_t)(&((idAI *)0)->lastAttackTime), sizeof( ((idAI *)0)->lastAttackTime ) },
	{ "float", "melee_range", (intptr_t)(&((idAI *)0)->melee_range), sizeof( ((idAI *)0)->melee_range ) },
	{ "float", "projectile_height_to_distance_ratio", (intptr_t)(&((idAI *)0)->projectile_height_to_distance_ratio), sizeof( ((idAI *)0)->projectile_height_to_distance_ratio ) },
	{ "idList < idVec3 , TAG_AI >", "missileLaunchOffset", (intptr_t)(&((idAI *)0)->missileLaunchOffset), sizeof( ((idAI *)0)->missileLaunchOffset ) },
	{ "const idDict *", "projectileDef", (intptr_t)(&((idAI *)0)->projectileDef), sizeof( ((idAI *)0)->projectileDef ) },
	{ "mutable idClipModel *", "projectileClipModel", (intptr_t)(&((idAI *)0)->projectileClipModel), sizeof( ((idAI *)0)->projectileClipModel ) },
	{ "float", "projectileRadius", (intptr_t)(&((idAI *)0)->projectileRadius), sizeof( ((idAI *)0)->projectileRadius ) },
	{ "float", "projectileSpeed", (intptr_t)(&((idAI *)0)->projectileSpeed), sizeof( ((idAI *)0)->projectileSpeed ) },
	{ "idVec3", "projectileVelocity", (intptr_t)(&((idAI *)0)->projectileVelocity), sizeof( ((idAI *)0)->projectileVelocity ) },
	{ "idVec3", "projectileGravity", (intptr_t)(&((idAI *)0)->projectileGravity), sizeof( ((idAI *)0)->projectileGravity ) },
	{ "idEntityPtr < idProjectile >", "projectile", (intptr_t)(&((idAI *)0)->projectile), sizeof( ((idAI *)0)->projectile ) },
	{ "idStr", "attack", (intptr_t)(&((idAI *)0)->attack), sizeof( ((idAI *)0)->attack ) },
	{ "idVec3", "homingMissileGoal", (intptr_t)(&((idAI *)0)->homingMissileGoal), sizeof( ((idAI *)0)->homingMissileGoal ) },
	{ "const idSoundShader *", "chat_snd", (intptr_t)(&((idAI *)0)->chat_snd), sizeof( ((idAI *)0)->chat_snd ) },
	{ "int", "chat_min", (intptr_t)(&((idAI *)0)->chat_min), sizeof( ((idAI *)0)->chat_min ) },
	{ "int", "chat_max", (intptr_t)(&((idAI *)0)->chat_max), sizeof( ((idAI *)0)->chat_max ) },
	{ "int", "chat_time", (intptr_t)(&((idAI *)0)->chat_time), sizeof( ((idAI *)0)->chat_time ) },
	{ "talkState_t", "talk_state", (intptr_t)(&((idAI *)0)->talk_state), sizeof( ((idAI *)0)->talk_state ) },
	{ "idEntityPtr < idActor >", "talkTarget", (intptr_t)(&((idAI *)0)->talkTarget), sizeof( ((idAI *)0)->talkTarget ) },
	{ "int", "num_cinematics", (intptr_t)(&((idAI *)0)->num_cinematics), sizeof( ((idAI *)0)->num_cinematics ) },
	{ "int", "current_cinematic", (intptr_t)(&((idAI *)0)->current_cinematic), sizeof( ((idAI *)0)->current_cinematic ) },
	{ "bool", "allowJointMod", (intptr_t)(&((idAI *)0)->allowJointMod), sizeof( ((idAI *)0)->allowJointMod ) },
	{ "idEntityPtr < idEntity >", "focusEntity", (intptr_t)(&((idAI *)0)->focusEntity), sizeof( ((idAI *)0)->focusEntity ) },
	{ "idVec3", "currentFocusPos", (intptr_t)(&((idAI *)0)->currentFocusPos), sizeof( ((idAI *)0)->currentFocusPos ) },
	{ "int", "focusTime", (intptr_t)(&((idAI *)0)->focusTime), sizeof( ((idAI *)0)->focusTime ) },
	{ "int", "alignHeadTime", (intptr_t)(&((idAI *)0)->alignHeadTime), sizeof( ((idAI *)0)->alignHeadTime ) },
	{ "int", "forceAlignHeadTime", (intptr_t)(&((idAI *)0)->forceAlignHeadTime), sizeof( ((idAI *)0)->forceAlignHeadTime ) },
	{ "idAngles", "eyeAng", (intptr_t)(&((idAI *)0)->eyeAng), sizeof( ((idAI *)0)->eyeAng ) },
	{ "idAngles", "lookAng", (intptr_t)(&((idAI *)0)->lookAng), sizeof( ((idAI *)0)->lookAng ) },
	{ "idAngles", "destLookAng", (intptr_t)(&((idAI *)0)->destLookAng), sizeof( ((idAI *)0)->destLookAng ) },
	{ "idAngles", "lookMin", (intptr_t)(&((idAI *)0)->lookMin), sizeof( ((idAI *)0)->lookMin ) },
	{ "idAngles", "lookMax", (intptr_t)(&((idAI *)0)->lookMax), sizeof( ((idAI *)0)->lookMax ) },
	{ "idList < jointHandle_t , TAG_AI >", "lookJoints", (intptr_t)(&((idAI *)0)->lookJoints), sizeof( ((idAI *)0)->lookJoints ) },
	{ "idList < idAngles , TAG_AI >", "lookJointAngles", (intptr_t)(&((idAI *)0)->lookJointAngles), sizeof( ((idAI *)0)->lookJointAngles ) },
	{ "float", "eyeVerticalOffset", (intptr_t)(&((idAI *)0)->eyeVerticalOffset), sizeof( ((idAI *)0)->eyeVerticalOffset ) },
	{ "float", "eyeHorizontalOffset", (intptr_t)(&((idAI *)0)->eyeHorizontalOffset), sizeof( ((idAI *)0)->eyeHorizontalOffset ) },
	{ "float", "eyeFocusRate", (intptr_t)(&((idAI *)0)->eyeFocusRate), sizeof( ((idAI *)0)->eyeFocusRate ) },
	{ "float", "headFocusRate", (intptr_t)(&((idAI *)0)->headFocusRate), sizeof( ((idAI *)0)->headFocusRate ) },
	{ "int", "focusAlignTime", (intptr_t)(&((idAI *)0)->focusAlignTime), sizeof( ((idAI *)0)->focusAlignTime ) },
	{ "bool", "restartParticles", (intptr_t)(&((idAI *)0)->restartParticles), sizeof( ((idAI *)0)->restartParticles ) },
	{ "bool", "useBoneAxis", (intptr_t)(&((idAI *)0)->useBoneAxis), sizeof( ((idAI *)0)->useBoneAxis ) },
	{ "idList < particleEmitter_t , TAG_AI >", "particles", (intptr_t)(&((idAI *)0)->particles), sizeof( ((idAI *)0)->particles ) },
	{ "renderLight_t", "worldMuzzleFlash", (intptr_t)(&((idAI *)0)->worldMuzzleFlash), sizeof( ((idAI *)0)->worldMuzzleFlash ) },
	{ "int", "worldMuzzleFlashHandle", (intptr_t)(&((idAI *)0)->worldMuzzleFlashHandle), sizeof( ((idAI *)0)->worldMuzzleFlashHandle ) },
	{ "jointHandle_t", "flashJointWorld", (intptr_t)(&((idAI *)0)->flashJointWorld), sizeof( ((idAI *)0)->flashJointWorld ) },
	{ "int", "muzzleFlashEnd", (intptr_t)(&((idAI *)0)->muzzleFlashEnd), sizeof( ((idAI *)0)->muzzleFlashEnd ) },
	{ "int", "flashTime", (intptr_t)(&((idAI *)0)->flashTime), sizeof( ((idAI *)0)->flashTime ) },
	{ "idAngles", "eyeMin", (intptr_t)(&((idAI *)0)->eyeMin), sizeof( ((idAI *)0)->eyeMin ) },
	{ "idAngles", "eyeMax", (intptr_t)(&((idAI *)0)->eyeMax), sizeof( ((idAI *)0)->eyeMax ) },
	{ "jointHandle_t", "focusJoint", (intptr_t)(&((idAI *)0)->focusJoint), sizeof( ((idAI *)0)->focusJoint ) },
	{ "jointHandle_t", "orientationJoint", (intptr_t)(&((idAI *)0)->orientationJoint), sizeof( ((idAI *)0)->orientationJoint ) },
	{ "idEntityPtr < idActor >", "enemy", (intptr_t)(&((idAI *)0)->enemy), sizeof( ((idAI *)0)->enemy ) },
	{ "idVec3", "lastVisibleEnemyPos", (intptr_t)(&((idAI *)0)->lastVisibleEnemyPos), sizeof( ((idAI *)0)->lastVisibleEnemyPos ) },
	{ "idVec3", "lastVisibleEnemyEyeOffset", (intptr_t)(&((idAI *)0)->lastVisibleEnemyEyeOffset), sizeof( ((idAI *)0)->lastVisibleEnemyEyeOffset ) },
	{ "idVec3", "lastVisibleReachableEnemyPos", (intptr_t)(&((idAI *)0)->lastVisibleReachableEnemyPos), sizeof( ((idAI *)0)->lastVisibleReachableEnemyPos ) },
	{ "idVec3", "lastReachableEnemyPos", (intptr_t)(&((idAI *)0)->lastReachableEnemyPos), sizeof( ((idAI *)0)->lastReachableEnemyPos ) },
	{ "bool", "wakeOnFlashlight", (intptr_t)(&((idAI *)0)->wakeOnFlashlight), sizeof( ((idAI *)0)->wakeOnFlashlight ) },
	{ "bool", "spawnClearMoveables", (intptr_t)(&((idAI *)0)->spawnClearMoveables), sizeof( ((idAI *)0)->spawnClearMoveables ) },
	{ "bool", "isAwake", (intptr_t)(&((idAI *)0)->isAwake), sizeof( ((idAI *)0)->isAwake ) },
	{ "idHashTable < funcEmitter_t >", "funcEmitters", (intptr_t)(&((idAI *)0)->funcEmitters), sizeof( ((idAI *)0)->funcEmitters ) },
	{ "idEntityPtr < idHarvestable >", "harvestEnt", (intptr_t)(&((idAI *)0)->harvestEnt), sizeof( ((idAI *)0)->harvestEnt ) },
	{ "idScriptBool", "AI_TALK", (intptr_t)(&((idAI *)0)->AI_TALK), sizeof( ((idAI *)0)->AI_TALK ) },
	{ "idScriptBool", "AI_DAMAGE", (intptr_t)(&((idAI *)0)->AI_DAMAGE), sizeof( ((idAI *)0)->AI_DAMAGE ) },
	{ "idScriptBool", "AI_PAIN", (intptr_t)(&((idAI *)0)->AI_PAIN), sizeof( ((idAI *)0)->AI_PAIN ) },
	{ "idScriptFloat", "AI_SPECIAL_DAMAGE", (intptr_t)(&((idAI *)0)->AI_SPECIAL_DAMAGE), sizeof( ((idAI *)0)->AI_SPECIAL_DAMAGE ) },
	{ "idScriptBool", "AI_DEAD", (intptr_t)(&((idAI *)0)->AI_DEAD), sizeof( ((idAI *)0)->AI_DEAD ) },
	{ "idScriptBool", "AI_RUN", (intptr_t)(&((idAI *)0)->AI_RUN), sizeof( ((idAI *)0)->AI_RUN ) },
	{ "idScriptBool", "blocked", (intptr_t)(&((idAI *)0)->blocked), sizeof( ((idAI *)0)->blocked ) },
	{ "idScriptBool", "AI_ATTACKING", (intptr_t)(&((idAI *)0)->AI_ATTACKING), sizeof( ((idAI *)0)->AI_ATTACKING ) },
	{ "idScriptBool", "AI_ENEMY_VISIBLE", (intptr_t)(&((idAI *)0)->AI_ENEMY_VISIBLE), sizeof( ((idAI *)0)->AI_ENEMY_VISIBLE ) },
	{ "idScriptBool", "AI_ENEMY_IN_FOV", (intptr_t)(&((idAI *)0)->AI_ENEMY_IN_FOV), sizeof( ((idAI *)0)->AI_ENEMY_IN_FOV ) },
	{ "idScriptBool", "AI_ENEMY_DEAD", (intptr_t)(&((idAI *)0)->AI_ENEMY_DEAD), sizeof( ((idAI *)0)->AI_ENEMY_DEAD ) },
	{ "idScriptBool", "AI_MOVE_DONE", (intptr_t)(&((idAI *)0)->AI_MOVE_DONE), sizeof( ((idAI *)0)->AI_MOVE_DONE ) },
	{ "idScriptBool", "AI_ONGROUND", (intptr_t)(&((idAI *)0)->AI_ONGROUND), sizeof( ((idAI *)0)->AI_ONGROUND ) },
	{ "idScriptBool", "AI_ACTIVATED", (intptr_t)(&((idAI *)0)->AI_ACTIVATED), sizeof( ((idAI *)0)->AI_ACTIVATED ) },
	{ "idScriptBool", "AI_FORWARD", (intptr_t)(&((idAI *)0)->AI_FORWARD), sizeof( ((idAI *)0)->AI_FORWARD ) },
	{ "idScriptBool", "AI_JUMP", (intptr_t)(&((idAI *)0)->AI_JUMP), sizeof( ((idAI *)0)->AI_JUMP ) },
	{ "idScriptBool", "AI_ENEMY_REACHABLE", (intptr_t)(&((idAI *)0)->AI_ENEMY_REACHABLE), sizeof( ((idAI *)0)->AI_ENEMY_REACHABLE ) },
	{ "idScriptBool", "AI_BLOCKED", (intptr_t)(&((idAI *)0)->AI_BLOCKED), sizeof( ((idAI *)0)->AI_BLOCKED ) },
	{ "idScriptBool", "AI_OBSTACLE_IN_PATH", (intptr_t)(&((idAI *)0)->AI_OBSTACLE_IN_PATH), sizeof( ((idAI *)0)->AI_OBSTACLE_IN_PATH ) },
	{ "idScriptBool", "AI_DEST_UNREACHABLE", (intptr_t)(&((idAI *)0)->AI_DEST_UNREACHABLE), sizeof( ((idAI *)0)->AI_DEST_UNREACHABLE ) },
	{ "idScriptBool", "AI_HIT_ENEMY", (intptr_t)(&((idAI *)0)->AI_HIT_ENEMY), sizeof( ((idAI *)0)->AI_HIT_ENEMY ) },
	{ "idScriptBool", "AI_PUSHED", (intptr_t)(&((idAI *)0)->AI_PUSHED), sizeof( ((idAI *)0)->AI_PUSHED ) },
	{ "idScriptFloat", "run_distance", (intptr_t)(&((idAI *)0)->run_distance), sizeof( ((idAI *)0)->run_distance ) },
	{ "idScriptFloat", "walk_turn", (intptr_t)(&((idAI *)0)->walk_turn), sizeof( ((idAI *)0)->walk_turn ) },
	{ ": float", "allow_attack", (intptr_t)(&((idAI *)0)->allow_attack), sizeof( ((idAI *)0)->allow_attack ) },
	{ "float", "lost_time", (intptr_t)(&((idAI *)0)->lost_time), sizeof( ((idAI *)0)->lost_time ) },
	{ "idEntity *", "lost_combat_node", (intptr_t)(&((idAI *)0)->lost_combat_node), sizeof( ((idAI *)0)->lost_combat_node ) },
	{ "float", "attack_flags", (intptr_t)(&((idAI *)0)->attack_flags), sizeof( ((idAI *)0)->attack_flags ) },
	{ "bool", "supportsNative", (intptr_t)(&((idAI *)0)->supportsNative), sizeof( ((idAI *)0)->supportsNative ) },
	{ "idStr", "lastStateName", (intptr_t)(&((idAI *)0)->lastStateName), sizeof( ((idAI *)0)->lastStateName ) },
	{ "stateParms_t", "storedState", (intptr_t)(&((idAI *)0)->storedState), sizeof( ((idAI *)0)->storedState ) },
	{ NULL, 0 }
};

static classVariableInfo_t idCombatNode_typeInfo[] = {
	{ ": float", "min_dist", (intptr_t)(&((idCombatNode *)0)->min_dist), sizeof( ((idCombatNode *)0)->min_dist ) },
	{ "float", "max_dist", (intptr_t)(&((idCombatNode *)0)->max_dist), sizeof( ((idCombatNode *)0)->max_dist ) },
	{ "float", "cone_dist", (intptr_t)(&((idCombatNode *)0)->cone_dist), sizeof( ((idCombatNode *)0)->cone_dist ) },
	{ "float", "min_height", (intptr_t)(&((idCombatNode *)0)->min_height), sizeof( ((idCombatNode *)0)->min_height ) },
	{ "float", "max_height", (intptr_t)(&((idCombatNode *)0)->max_height), sizeof( ((idCombatNode *)0)->max_height ) },
	{ "idVec3", "cone_left", (intptr_t)(&((idCombatNode *)0)->cone_left), sizeof( ((idCombatNode *)0)->cone_left ) },
	{ "idVec3", "cone_right", (intptr_t)(&((idCombatNode *)0)->cone_right), sizeof( ((idCombatNode *)0)->cone_right ) },
	{ "idVec3", "offset", (intptr_t)(&((idCombatNode *)0)->offset), sizeof( ((idCombatNode *)0)->offset ) },
	{ "bool", "disabled", (intptr_t)(&((idCombatNode *)0)->disabled), sizeof( ((idCombatNode *)0)->disabled ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceAI_Follower_typeInfo[] = {
	{ ": bool", "inCustomAnim", (intptr_t)(&((iceAI_Follower *)0)->inCustomAnim), sizeof( ((iceAI_Follower *)0)->inCustomAnim ) },
	{ "idEntity *", "leader", (intptr_t)(&((iceAI_Follower *)0)->leader), sizeof( ((iceAI_Follower *)0)->leader ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterBossVagary_typeInfo[] = {
	{ ": float", "nextDodge", (intptr_t)(&((iceMonsterBossVagary *)0)->nextDodge), sizeof( ((iceMonsterBossVagary *)0)->nextDodge ) },
	{ "float", "nextAttack", (intptr_t)(&((iceMonsterBossVagary *)0)->nextAttack), sizeof( ((iceMonsterBossVagary *)0)->nextAttack ) },
	{ "float", "nextNoFOVAttack", (intptr_t)(&((iceMonsterBossVagary *)0)->nextNoFOVAttack), sizeof( ((iceMonsterBossVagary *)0)->nextNoFOVAttack ) },
	{ "idEntity *", "combat_node", (intptr_t)(&((iceMonsterBossVagary *)0)->combat_node), sizeof( ((iceMonsterBossVagary *)0)->combat_node ) },
	{ "idEntity *", "throwEntity", (intptr_t)(&((iceMonsterBossVagary *)0)->throwEntity), sizeof( ((iceMonsterBossVagary *)0)->throwEntity ) },
	{ "int", "num", (intptr_t)(&((iceMonsterBossVagary *)0)->num), sizeof( ((iceMonsterBossVagary *)0)->num ) },
	{ "int", "i", (intptr_t)(&((iceMonsterBossVagary *)0)->i), sizeof( ((iceMonsterBossVagary *)0)->i ) },
	{ "idVec3", "pos", (intptr_t)(&((iceMonsterBossVagary *)0)->pos), sizeof( ((iceMonsterBossVagary *)0)->pos ) },
	{ "float", "waitTime", (intptr_t)(&((iceMonsterBossVagary *)0)->waitTime), sizeof( ((iceMonsterBossVagary *)0)->waitTime ) },
	{ "float", "t", (intptr_t)(&((iceMonsterBossVagary *)0)->t), sizeof( ((iceMonsterBossVagary *)0)->t ) },
	{ "idVec3", "offset", (intptr_t)(&((iceMonsterBossVagary *)0)->offset), sizeof( ((iceMonsterBossVagary *)0)->offset ) },
	{ "idVec3", "vel", (intptr_t)(&((iceMonsterBossVagary *)0)->vel), sizeof( ((iceMonsterBossVagary *)0)->vel ) },
	{ "float", "start_offset", (intptr_t)(&((iceMonsterBossVagary *)0)->start_offset), sizeof( ((iceMonsterBossVagary *)0)->start_offset ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterDemonHellknight_typeInfo[] = {
	{ ": float", "nextAttack", (intptr_t)(&((iceMonsterDemonHellknight *)0)->nextAttack), sizeof( ((iceMonsterDemonHellknight *)0)->nextAttack ) },
	{ "float", "nextNoFOVAttack", (intptr_t)(&((iceMonsterDemonHellknight *)0)->nextNoFOVAttack), sizeof( ((iceMonsterDemonHellknight *)0)->nextNoFOVAttack ) },
	{ "idEntity *", "combat_node", (intptr_t)(&((iceMonsterDemonHellknight *)0)->combat_node), sizeof( ((iceMonsterDemonHellknight *)0)->combat_node ) },
	{ "idScriptString", "range_attack_anim", (intptr_t)(&((iceMonsterDemonHellknight *)0)->range_attack_anim), sizeof( ((iceMonsterDemonHellknight *)0)->range_attack_anim ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterDemonImp_typeInfo[] = {
	{ ": float", "nextDodge", (intptr_t)(&((iceMonsterDemonImp *)0)->nextDodge), sizeof( ((iceMonsterDemonImp *)0)->nextDodge ) },
	{ "float", "nextAttack", (intptr_t)(&((iceMonsterDemonImp *)0)->nextAttack), sizeof( ((iceMonsterDemonImp *)0)->nextAttack ) },
	{ "float", "nextLeap", (intptr_t)(&((iceMonsterDemonImp *)0)->nextLeap), sizeof( ((iceMonsterDemonImp *)0)->nextLeap ) },
	{ "float", "nextNoFOVAttack", (intptr_t)(&((iceMonsterDemonImp *)0)->nextNoFOVAttack), sizeof( ((iceMonsterDemonImp *)0)->nextNoFOVAttack ) },
	{ "idEntity *", "combat_node", (intptr_t)(&((iceMonsterDemonImp *)0)->combat_node), sizeof( ((iceMonsterDemonImp *)0)->combat_node ) },
	{ "idScriptString", "range_attack_anim", (intptr_t)(&((iceMonsterDemonImp *)0)->range_attack_anim), sizeof( ((iceMonsterDemonImp *)0)->range_attack_anim ) },
	{ "idScriptVector", "jumpVelocity", (intptr_t)(&((iceMonsterDemonImp *)0)->jumpVelocity), sizeof( ((iceMonsterDemonImp *)0)->jumpVelocity ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterLostSoul_typeInfo[] = {
	{ ": float", "nextAttack", (intptr_t)(&((iceMonsterLostSoul *)0)->nextAttack), sizeof( ((iceMonsterLostSoul *)0)->nextAttack ) },
	{ "float", "nextNoFOVAttack", (intptr_t)(&((iceMonsterLostSoul *)0)->nextNoFOVAttack), sizeof( ((iceMonsterLostSoul *)0)->nextNoFOVAttack ) },
	{ "float", "noMeleeTime", (intptr_t)(&((iceMonsterLostSoul *)0)->noMeleeTime), sizeof( ((iceMonsterLostSoul *)0)->noMeleeTime ) },
	{ "float", "fly_offset", (intptr_t)(&((iceMonsterLostSoul *)0)->fly_offset), sizeof( ((iceMonsterLostSoul *)0)->fly_offset ) },
	{ ": idVec3", "vel", (intptr_t)(&((iceMonsterLostSoul *)0)->vel), sizeof( ((iceMonsterLostSoul *)0)->vel ) },
	{ "idVec3", "pos", (intptr_t)(&((iceMonsterLostSoul *)0)->pos), sizeof( ((iceMonsterLostSoul *)0)->pos ) },
	{ "float", "endtime", (intptr_t)(&((iceMonsterLostSoul *)0)->endtime), sizeof( ((iceMonsterLostSoul *)0)->endtime ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterFlyingCacodemon_typeInfo[] = {
	{ ": float", "nextAttack", (intptr_t)(&((iceMonsterFlyingCacodemon *)0)->nextAttack), sizeof( ((iceMonsterFlyingCacodemon *)0)->nextAttack ) },
	{ "float", "nextNoFOVAttack", (intptr_t)(&((iceMonsterFlyingCacodemon *)0)->nextNoFOVAttack), sizeof( ((iceMonsterFlyingCacodemon *)0)->nextNoFOVAttack ) },
	{ "idEntity *", "combat_node", (intptr_t)(&((iceMonsterFlyingCacodemon *)0)->combat_node), sizeof( ((iceMonsterFlyingCacodemon *)0)->combat_node ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterZombie_typeInfo[] = {
	{ ": idScriptBool", "can_run", (intptr_t)(&((iceMonsterZombie *)0)->can_run), sizeof( ((iceMonsterZombie *)0)->can_run ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterZombieSawyer_typeInfo[] = {
	{ "float", "next_hit_time", (intptr_t)(&((iceMonsterZombieSawyer *)0)->next_hit_time), sizeof( ((iceMonsterZombieSawyer *)0)->next_hit_time ) },
	{ "int", "smoke_frames", (intptr_t)(&((iceMonsterZombieSawyer *)0)->smoke_frames), sizeof( ((iceMonsterZombieSawyer *)0)->smoke_frames ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterZombieBernie_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterZombieMorgue_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterZombieSecurityPistol_typeInfo[] = {
	{ ": idEntity *", "combat_node", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->combat_node), sizeof( ((iceMonsterZombieSecurityPistol *)0)->combat_node ) },
	{ "idScriptBool", "fire", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->fire), sizeof( ((iceMonsterZombieSecurityPistol *)0)->fire ) },
	{ "idScriptBool", "crouch_fire", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->crouch_fire), sizeof( ((iceMonsterZombieSecurityPistol *)0)->crouch_fire ) },
	{ "idScriptBool", "run_attack", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->run_attack), sizeof( ((iceMonsterZombieSecurityPistol *)0)->run_attack ) },
	{ "idScriptFloat", "nextDodge", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->nextDodge), sizeof( ((iceMonsterZombieSecurityPistol *)0)->nextDodge ) },
	{ "idScriptFloat", "nextAttack", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->nextAttack), sizeof( ((iceMonsterZombieSecurityPistol *)0)->nextAttack ) },
	{ "idScriptFloat", "nextNoFOVAttack", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->nextNoFOVAttack), sizeof( ((iceMonsterZombieSecurityPistol *)0)->nextNoFOVAttack ) },
	{ "float", "zsecp_num_stand_attacks", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->zsecp_num_stand_attacks), sizeof( ((iceMonsterZombieSecurityPistol *)0)->zsecp_num_stand_attacks ) },
	{ "float", "zsecp_num_crouch_attacks", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->zsecp_num_crouch_attacks), sizeof( ((iceMonsterZombieSecurityPistol *)0)->zsecp_num_crouch_attacks ) },
	{ ": float", "attackTime", (intptr_t)(&((iceMonsterZombieSecurityPistol *)0)->attackTime), sizeof( ((iceMonsterZombieSecurityPistol *)0)->attackTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterZombieCommandoTentacle_typeInfo[] = {
	{ ": float", "nextAttack", (intptr_t)(&((iceMonsterZombieCommandoTentacle *)0)->nextAttack), sizeof( ((iceMonsterZombieCommandoTentacle *)0)->nextAttack ) },
	{ "float", "nextNoFOVAttack", (intptr_t)(&((iceMonsterZombieCommandoTentacle *)0)->nextNoFOVAttack), sizeof( ((iceMonsterZombieCommandoTentacle *)0)->nextNoFOVAttack ) },
	{ "bool", "tentacleDamage", (intptr_t)(&((iceMonsterZombieCommandoTentacle *)0)->tentacleDamage), sizeof( ((iceMonsterZombieCommandoTentacle *)0)->tentacleDamage ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonsterZombieCommandoChaingun_typeInfo[] = {
	{ ": idScriptBool", "fire", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->fire), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->fire ) },
	{ "idScriptBool", "crouch_fire", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->crouch_fire), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->crouch_fire ) },
	{ "idScriptBool", "step_left", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->step_left), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->step_left ) },
	{ "idScriptBool", "step_right", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->step_right), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->step_right ) },
	{ "idScriptFloat", "nextDodge", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->nextDodge), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->nextDodge ) },
	{ "idScriptFloat", "nextAttack", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->nextAttack), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->nextAttack ) },
	{ "idScriptFloat", "nextNoFOVAttack", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->nextNoFOVAttack), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->nextNoFOVAttack ) },
	{ ": idEntity *", "combat_node", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->combat_node), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->combat_node ) },
	{ "float", "attackTime", (intptr_t)(&((iceMonsterZombieCommandoChaingun *)0)->attackTime), sizeof( ((iceMonsterZombieCommandoChaingun *)0)->attackTime ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceMonster_Turret_typeInfo[] = {
	{ "bool", "fire", (intptr_t)(&((iceMonster_Turret *)0)->fire), sizeof( ((iceMonster_Turret *)0)->fire ) },
	{ "bool", "attack_monsters", (intptr_t)(&((iceMonster_Turret *)0)->attack_monsters), sizeof( ((iceMonster_Turret *)0)->attack_monsters ) },
	{ "idEntity", "light", (intptr_t)(&((iceMonster_Turret *)0)->light), sizeof( ((iceMonster_Turret *)0)->light ) },
	{ "bool", "light_is_on", (intptr_t)(&((iceMonster_Turret *)0)->light_is_on), sizeof( ((iceMonster_Turret *)0)->light_is_on ) },
	{ "float", "attackTime", (intptr_t)(&((iceMonster_Turret *)0)->attackTime), sizeof( ((iceMonster_Turret *)0)->attackTime ) },
	{ "int", "barrelCount", (intptr_t)(&((iceMonster_Turret *)0)->barrelCount), sizeof( ((iceMonster_Turret *)0)->barrelCount ) },
	{ "int", "currentBarrel", (intptr_t)(&((iceMonster_Turret *)0)->currentBarrel), sizeof( ((iceMonster_Turret *)0)->currentBarrel ) },
	{ "idStr", "currentBarrelStr", (intptr_t)(&((iceMonster_Turret *)0)->currentBarrelStr), sizeof( ((iceMonster_Turret *)0)->currentBarrelStr ) },
	{ "idList < jointHandle_t >", "flashJointWorldHandles", (intptr_t)(&((iceMonster_Turret *)0)->flashJointWorldHandles), sizeof( ((iceMonster_Turret *)0)->flashJointWorldHandles ) },
	{ NULL, 0 }
};

static classVariableInfo_t fuzzyseperator_t_typeInfo[] = {
	{ "bool", "inUse", (intptr_t)(&((fuzzyseperator_t *)0)->inUse), sizeof( ((fuzzyseperator_t *)0)->inUse ) },
	{ "int", "index", (intptr_t)(&((fuzzyseperator_t *)0)->index), sizeof( ((fuzzyseperator_t *)0)->index ) },
	{ "int", "value", (intptr_t)(&((fuzzyseperator_t *)0)->value), sizeof( ((fuzzyseperator_t *)0)->value ) },
	{ "int", "type", (intptr_t)(&((fuzzyseperator_t *)0)->type), sizeof( ((fuzzyseperator_t *)0)->type ) },
	{ "float", "weight", (intptr_t)(&((fuzzyseperator_t *)0)->weight), sizeof( ((fuzzyseperator_t *)0)->weight ) },
	{ "float", "minweight", (intptr_t)(&((fuzzyseperator_t *)0)->minweight), sizeof( ((fuzzyseperator_t *)0)->minweight ) },
	{ "float", "maxweight", (intptr_t)(&((fuzzyseperator_t *)0)->maxweight), sizeof( ((fuzzyseperator_t *)0)->maxweight ) },
	{ "fuzzyseperator_t *", "child", (intptr_t)(&((fuzzyseperator_t *)0)->child), sizeof( ((fuzzyseperator_t *)0)->child ) },
	{ "fuzzyseperator_t *", "next", (intptr_t)(&((fuzzyseperator_t *)0)->next), sizeof( ((fuzzyseperator_t *)0)->next ) },
	{ NULL, 0 }
};

static classVariableInfo_t weight_t_typeInfo[] = {
	{ "idStr", "name", (intptr_t)(&((weight_t *)0)->name), sizeof( ((weight_t *)0)->name ) },
	{ "fuzzyseperator_t *", "firstseperator", (intptr_t)(&((weight_t *)0)->firstseperator), sizeof( ((weight_t *)0)->firstseperator ) },
	{ NULL, 0 }
};

static classVariableInfo_t weightconfig_t_typeInfo[] = {
	{ "bool", "inUse", (intptr_t)(&((weightconfig_t *)0)->inUse), sizeof( ((weightconfig_t *)0)->inUse ) },
	{ "int", "numweights", (intptr_t)(&((weightconfig_t *)0)->numweights), sizeof( ((weightconfig_t *)0)->numweights ) },
	{ "weight_t[128]", "weights", (intptr_t)(&((weightconfig_t *)0)->weights), sizeof( ((weightconfig_t *)0)->weights ) },
	{ "idStr", "filename", (intptr_t)(&((weightconfig_t *)0)->filename), sizeof( ((weightconfig_t *)0)->filename ) },
	{ NULL, 0 }
};

static classVariableInfo_t cvalue_typeInfo[] = {
	{ "int", "integer", (intptr_t)(&((cvalue *)0)->integer), sizeof( ((cvalue *)0)->integer ) },
	{ "float", "_float", (intptr_t)(&((cvalue *)0)->_float), sizeof( ((cvalue *)0)->_float ) },
	{ "idStr", "string", (intptr_t)(&((cvalue *)0)->string), sizeof( ((cvalue *)0)->string ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_characteristic_t_typeInfo[] = {
	{ "char", "type", (intptr_t)(&((bot_characteristic_t *)0)->type), sizeof( ((bot_characteristic_t *)0)->type ) },
	{ "cvalue", "value", (intptr_t)(&((bot_characteristic_t *)0)->value), sizeof( ((bot_characteristic_t *)0)->value ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_character_t_typeInfo[] = {
	{ "idStr", "filename", (intptr_t)(&((bot_character_t *)0)->filename), sizeof( ((bot_character_t *)0)->filename ) },
	{ "bool", "inUse", (intptr_t)(&((bot_character_t *)0)->inUse), sizeof( ((bot_character_t *)0)->inUse ) },
	{ "float", "skill", (intptr_t)(&((bot_character_t *)0)->skill), sizeof( ((bot_character_t *)0)->skill ) },
	{ "bot_characteristic_t[80]", "c", (intptr_t)(&((bot_character_t *)0)->c), sizeof( ((bot_character_t *)0)->c ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_input_t_typeInfo[] = {
	{ "float", "thinktime", (intptr_t)(&((bot_input_t *)0)->thinktime), sizeof( ((bot_input_t *)0)->thinktime ) },
	{ "idVec3", "dir", (intptr_t)(&((bot_input_t *)0)->dir), sizeof( ((bot_input_t *)0)->dir ) },
	{ "float", "speed", (intptr_t)(&((bot_input_t *)0)->speed), sizeof( ((bot_input_t *)0)->speed ) },
	{ "idAngles", "viewangles", (intptr_t)(&((bot_input_t *)0)->viewangles), sizeof( ((bot_input_t *)0)->viewangles ) },
	{ "int", "actionflags", (intptr_t)(&((bot_input_t *)0)->actionflags), sizeof( ((bot_input_t *)0)->actionflags ) },
	{ "int", "weapon", (intptr_t)(&((bot_input_t *)0)->weapon), sizeof( ((bot_input_t *)0)->weapon ) },
	{ "int", "lastWeaponNum", (intptr_t)(&((bot_input_t *)0)->lastWeaponNum), sizeof( ((bot_input_t *)0)->lastWeaponNum ) },
	{ "bool", "respawn", (intptr_t)(&((bot_input_t *)0)->respawn), sizeof( ((bot_input_t *)0)->respawn ) },
	{ NULL, 0 }
};

static classVariableInfo_t rvmBotUtil_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idBotCharacterStatsManager_typeInfo[] = {
	{ "bot_character_t *", "default_char_profile", (intptr_t)(&((idBotCharacterStatsManager *)0)->default_char_profile), sizeof( ((idBotCharacterStatsManager *)0)->default_char_profile ) },
	{ "bot_character_t[256]", "charStatsList", (intptr_t)(&((idBotCharacterStatsManager *)0)->charStatsList), sizeof( ((idBotCharacterStatsManager *)0)->charStatsList ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBotFuzzyWeightManager_typeInfo[] = {
	{ "weightconfig_t[128]", "weightFileList", (intptr_t)(&((idBotFuzzyWeightManager *)0)->weightFileList), sizeof( ((idBotFuzzyWeightManager *)0)->weightFileList ) },
	{ "fuzzyseperator_t[8192]", "fuzzyseperators", (intptr_t)(&((idBotFuzzyWeightManager *)0)->fuzzyseperators), sizeof( ((idBotFuzzyWeightManager *)0)->fuzzyseperators ) },
	{ NULL, 0 }
};

static classVariableInfo_t projectileinfo_t_typeInfo[] = {
	{ "idStr", "name", (intptr_t)(&((projectileinfo_t *)0)->name), sizeof( ((projectileinfo_t *)0)->name ) },
	{ "idStr", "model", (intptr_t)(&((projectileinfo_t *)0)->model), sizeof( ((projectileinfo_t *)0)->model ) },
	{ "int", "flags", (intptr_t)(&((projectileinfo_t *)0)->flags), sizeof( ((projectileinfo_t *)0)->flags ) },
	{ "float", "gravity", (intptr_t)(&((projectileinfo_t *)0)->gravity), sizeof( ((projectileinfo_t *)0)->gravity ) },
	{ "int", "damage", (intptr_t)(&((projectileinfo_t *)0)->damage), sizeof( ((projectileinfo_t *)0)->damage ) },
	{ "float", "radius", (intptr_t)(&((projectileinfo_t *)0)->radius), sizeof( ((projectileinfo_t *)0)->radius ) },
	{ "int", "visdamage", (intptr_t)(&((projectileinfo_t *)0)->visdamage), sizeof( ((projectileinfo_t *)0)->visdamage ) },
	{ "int", "damagetype", (intptr_t)(&((projectileinfo_t *)0)->damagetype), sizeof( ((projectileinfo_t *)0)->damagetype ) },
	{ "int", "healthinc", (intptr_t)(&((projectileinfo_t *)0)->healthinc), sizeof( ((projectileinfo_t *)0)->healthinc ) },
	{ "float", "push", (intptr_t)(&((projectileinfo_t *)0)->push), sizeof( ((projectileinfo_t *)0)->push ) },
	{ "float", "detonation", (intptr_t)(&((projectileinfo_t *)0)->detonation), sizeof( ((projectileinfo_t *)0)->detonation ) },
	{ "float", "bounce", (intptr_t)(&((projectileinfo_t *)0)->bounce), sizeof( ((projectileinfo_t *)0)->bounce ) },
	{ "float", "bouncefric", (intptr_t)(&((projectileinfo_t *)0)->bouncefric), sizeof( ((projectileinfo_t *)0)->bouncefric ) },
	{ "float", "bouncestop", (intptr_t)(&((projectileinfo_t *)0)->bouncestop), sizeof( ((projectileinfo_t *)0)->bouncestop ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_weaponinfo_t_typeInfo[] = {
	{ "int", "valid", (intptr_t)(&((bot_weaponinfo_t *)0)->valid), sizeof( ((bot_weaponinfo_t *)0)->valid ) },
	{ "int", "number", (intptr_t)(&((bot_weaponinfo_t *)0)->number), sizeof( ((bot_weaponinfo_t *)0)->number ) },
	{ "idStr", "name", (intptr_t)(&((bot_weaponinfo_t *)0)->name), sizeof( ((bot_weaponinfo_t *)0)->name ) },
	{ "idStr", "model", (intptr_t)(&((bot_weaponinfo_t *)0)->model), sizeof( ((bot_weaponinfo_t *)0)->model ) },
	{ "int", "level", (intptr_t)(&((bot_weaponinfo_t *)0)->level), sizeof( ((bot_weaponinfo_t *)0)->level ) },
	{ "int", "weaponindex", (intptr_t)(&((bot_weaponinfo_t *)0)->weaponindex), sizeof( ((bot_weaponinfo_t *)0)->weaponindex ) },
	{ "int", "flags", (intptr_t)(&((bot_weaponinfo_t *)0)->flags), sizeof( ((bot_weaponinfo_t *)0)->flags ) },
	{ "idStr", "projectile", (intptr_t)(&((bot_weaponinfo_t *)0)->projectile), sizeof( ((bot_weaponinfo_t *)0)->projectile ) },
	{ "int", "numprojectiles", (intptr_t)(&((bot_weaponinfo_t *)0)->numprojectiles), sizeof( ((bot_weaponinfo_t *)0)->numprojectiles ) },
	{ "float", "hspread", (intptr_t)(&((bot_weaponinfo_t *)0)->hspread), sizeof( ((bot_weaponinfo_t *)0)->hspread ) },
	{ "float", "vspread", (intptr_t)(&((bot_weaponinfo_t *)0)->vspread), sizeof( ((bot_weaponinfo_t *)0)->vspread ) },
	{ "float", "speed", (intptr_t)(&((bot_weaponinfo_t *)0)->speed), sizeof( ((bot_weaponinfo_t *)0)->speed ) },
	{ "float", "acceleration", (intptr_t)(&((bot_weaponinfo_t *)0)->acceleration), sizeof( ((bot_weaponinfo_t *)0)->acceleration ) },
	{ "idVec3", "recoil", (intptr_t)(&((bot_weaponinfo_t *)0)->recoil), sizeof( ((bot_weaponinfo_t *)0)->recoil ) },
	{ "idVec3", "offset", (intptr_t)(&((bot_weaponinfo_t *)0)->offset), sizeof( ((bot_weaponinfo_t *)0)->offset ) },
	{ "idVec3", "angleoffset", (intptr_t)(&((bot_weaponinfo_t *)0)->angleoffset), sizeof( ((bot_weaponinfo_t *)0)->angleoffset ) },
	{ "float", "extrazvelocity", (intptr_t)(&((bot_weaponinfo_t *)0)->extrazvelocity), sizeof( ((bot_weaponinfo_t *)0)->extrazvelocity ) },
	{ "int", "ammoamount", (intptr_t)(&((bot_weaponinfo_t *)0)->ammoamount), sizeof( ((bot_weaponinfo_t *)0)->ammoamount ) },
	{ "int", "ammoindex", (intptr_t)(&((bot_weaponinfo_t *)0)->ammoindex), sizeof( ((bot_weaponinfo_t *)0)->ammoindex ) },
	{ "float", "activate", (intptr_t)(&((bot_weaponinfo_t *)0)->activate), sizeof( ((bot_weaponinfo_t *)0)->activate ) },
	{ "float", "reload", (intptr_t)(&((bot_weaponinfo_t *)0)->reload), sizeof( ((bot_weaponinfo_t *)0)->reload ) },
	{ "float", "spinup", (intptr_t)(&((bot_weaponinfo_t *)0)->spinup), sizeof( ((bot_weaponinfo_t *)0)->spinup ) },
	{ "float", "spindown", (intptr_t)(&((bot_weaponinfo_t *)0)->spindown), sizeof( ((bot_weaponinfo_t *)0)->spindown ) },
	{ "projectileinfo_t", "proj", (intptr_t)(&((bot_weaponinfo_t *)0)->proj), sizeof( ((bot_weaponinfo_t *)0)->proj ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_weaponstate_t_typeInfo[] = {
	{ "bool", "inUse", (intptr_t)(&((bot_weaponstate_t *)0)->inUse), sizeof( ((bot_weaponstate_t *)0)->inUse ) },
	{ "weightconfig_t *", "weaponweightconfig", (intptr_t)(&((bot_weaponstate_t *)0)->weaponweightconfig), sizeof( ((bot_weaponstate_t *)0)->weaponweightconfig ) },
	{ "int[32]", "weaponweightindex", (intptr_t)(&((bot_weaponstate_t *)0)->weaponweightindex), sizeof( ((bot_weaponstate_t *)0)->weaponweightindex ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBotWeaponInfoManager_typeInfo[] = {
	{ ": idList < projectileinfo_t >", "projectileinfo", (intptr_t)(&((idBotWeaponInfoManager *)0)->projectileinfo), sizeof( ((idBotWeaponInfoManager *)0)->projectileinfo ) },
	{ "bot_weaponstate_t[1]", "botweaponstates", (intptr_t)(&((idBotWeaponInfoManager *)0)->botweaponstates), sizeof( ((idBotWeaponInfoManager *)0)->botweaponstates ) },
	{ "bot_weaponinfo_t[32]", "weaponinfo", (intptr_t)(&((idBotWeaponInfoManager *)0)->weaponinfo), sizeof( ((idBotWeaponInfoManager *)0)->weaponinfo ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_goal_t_typeInfo[] = {
	{ "int", "framenum", (intptr_t)(&((bot_goal_t *)0)->framenum), sizeof( ((bot_goal_t *)0)->framenum ) },
	{ "idVec3", "origin", (intptr_t)(&((bot_goal_t *)0)->origin), sizeof( ((bot_goal_t *)0)->origin ) },
	{ "int", "areanum", (intptr_t)(&((bot_goal_t *)0)->areanum), sizeof( ((bot_goal_t *)0)->areanum ) },
	{ "idVec3", "mins", (intptr_t)(&((bot_goal_t *)0)->mins), sizeof( ((bot_goal_t *)0)->mins ) },
	{ "idVec3", "maxs", (intptr_t)(&((bot_goal_t *)0)->maxs), sizeof( ((bot_goal_t *)0)->maxs ) },
	{ "int", "entitynum", (intptr_t)(&((bot_goal_t *)0)->entitynum), sizeof( ((bot_goal_t *)0)->entitynum ) },
	{ "int", "number", (intptr_t)(&((bot_goal_t *)0)->number), sizeof( ((bot_goal_t *)0)->number ) },
	{ "int", "flags", (intptr_t)(&((bot_goal_t *)0)->flags), sizeof( ((bot_goal_t *)0)->flags ) },
	{ "int", "iteminfo", (intptr_t)(&((bot_goal_t *)0)->iteminfo), sizeof( ((bot_goal_t *)0)->iteminfo ) },
	{ NULL, 0 }
};

static classVariableInfo_t maplocation_t_typeInfo[] = {
	{ "idVec3", "origin", (intptr_t)(&((maplocation_t *)0)->origin), sizeof( ((maplocation_t *)0)->origin ) },
	{ "int", "areanum", (intptr_t)(&((maplocation_t *)0)->areanum), sizeof( ((maplocation_t *)0)->areanum ) },
	{ "idStr", "name", (intptr_t)(&((maplocation_t *)0)->name), sizeof( ((maplocation_t *)0)->name ) },
	{ "maplocation_t *", "next", (intptr_t)(&((maplocation_t *)0)->next), sizeof( ((maplocation_t *)0)->next ) },
	{ NULL, 0 }
};

static classVariableInfo_t campspot_t_typeInfo[] = {
	{ "idVec3", "origin", (intptr_t)(&((campspot_t *)0)->origin), sizeof( ((campspot_t *)0)->origin ) },
	{ "int", "areanum", (intptr_t)(&((campspot_t *)0)->areanum), sizeof( ((campspot_t *)0)->areanum ) },
	{ "idStr", "name", (intptr_t)(&((campspot_t *)0)->name), sizeof( ((campspot_t *)0)->name ) },
	{ "float", "range", (intptr_t)(&((campspot_t *)0)->range), sizeof( ((campspot_t *)0)->range ) },
	{ "float", "weight", (intptr_t)(&((campspot_t *)0)->weight), sizeof( ((campspot_t *)0)->weight ) },
	{ "float", "wait", (intptr_t)(&((campspot_t *)0)->wait), sizeof( ((campspot_t *)0)->wait ) },
	{ "float", "random", (intptr_t)(&((campspot_t *)0)->random), sizeof( ((campspot_t *)0)->random ) },
	{ NULL, 0 }
};

static classVariableInfo_t levelitem_t_typeInfo[] = {
	{ "idStr", "name", (intptr_t)(&((levelitem_t *)0)->name), sizeof( ((levelitem_t *)0)->name ) },
	{ "int", "number", (intptr_t)(&((levelitem_t *)0)->number), sizeof( ((levelitem_t *)0)->number ) },
	{ "int", "iteminfo", (intptr_t)(&((levelitem_t *)0)->iteminfo), sizeof( ((levelitem_t *)0)->iteminfo ) },
	{ "int", "flags", (intptr_t)(&((levelitem_t *)0)->flags), sizeof( ((levelitem_t *)0)->flags ) },
	{ "float", "weight", (intptr_t)(&((levelitem_t *)0)->weight), sizeof( ((levelitem_t *)0)->weight ) },
	{ "idVec3", "origin", (intptr_t)(&((levelitem_t *)0)->origin), sizeof( ((levelitem_t *)0)->origin ) },
	{ "idVec3", "goalorigin", (intptr_t)(&((levelitem_t *)0)->goalorigin), sizeof( ((levelitem_t *)0)->goalorigin ) },
	{ "idItem *", "item", (intptr_t)(&((levelitem_t *)0)->item), sizeof( ((levelitem_t *)0)->item ) },
	{ "float", "timeout", (intptr_t)(&((levelitem_t *)0)->timeout), sizeof( ((levelitem_t *)0)->timeout ) },
	{ "levelitem_t *", "prev", (intptr_t)(&((levelitem_t *)0)->prev), sizeof( ((levelitem_t *)0)->prev ) },
	{ "levelitem_t *", "next", (intptr_t)(&((levelitem_t *)0)->next), sizeof( ((levelitem_t *)0)->next ) },
	{ NULL, 0 }
};

static classVariableInfo_t iteminfo_t_typeInfo[] = {
	{ "idStr", "classname", (intptr_t)(&((iteminfo_t *)0)->classname), sizeof( ((iteminfo_t *)0)->classname ) },
	{ "idStr", "name", (intptr_t)(&((iteminfo_t *)0)->name), sizeof( ((iteminfo_t *)0)->name ) },
	{ "idStr", "model", (intptr_t)(&((iteminfo_t *)0)->model), sizeof( ((iteminfo_t *)0)->model ) },
	{ "int", "modelindex", (intptr_t)(&((iteminfo_t *)0)->modelindex), sizeof( ((iteminfo_t *)0)->modelindex ) },
	{ "int", "type", (intptr_t)(&((iteminfo_t *)0)->type), sizeof( ((iteminfo_t *)0)->type ) },
	{ "int", "index", (intptr_t)(&((iteminfo_t *)0)->index), sizeof( ((iteminfo_t *)0)->index ) },
	{ "float", "respawntime", (intptr_t)(&((iteminfo_t *)0)->respawntime), sizeof( ((iteminfo_t *)0)->respawntime ) },
	{ "idVec3", "mins", (intptr_t)(&((iteminfo_t *)0)->mins), sizeof( ((iteminfo_t *)0)->mins ) },
	{ "idVec3", "maxs", (intptr_t)(&((iteminfo_t *)0)->maxs), sizeof( ((iteminfo_t *)0)->maxs ) },
	{ "int", "number", (intptr_t)(&((iteminfo_t *)0)->number), sizeof( ((iteminfo_t *)0)->number ) },
	{ NULL, 0 }
};

static classVariableInfo_t itemconfig_t_typeInfo[] = {
	{ "int", "numiteminfo", (intptr_t)(&((itemconfig_t *)0)->numiteminfo), sizeof( ((itemconfig_t *)0)->numiteminfo ) },
	{ "iteminfo_t[256]", "iteminfo", (intptr_t)(&((itemconfig_t *)0)->iteminfo), sizeof( ((itemconfig_t *)0)->iteminfo ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_goalstate_t_typeInfo[] = {
	{ "weightconfig_t *", "itemweightconfig", (intptr_t)(&((bot_goalstate_t *)0)->itemweightconfig), sizeof( ((bot_goalstate_t *)0)->itemweightconfig ) },
	{ "int *", "itemweightindex", (intptr_t)(&((bot_goalstate_t *)0)->itemweightindex), sizeof( ((bot_goalstate_t *)0)->itemweightindex ) },
	{ "int", "client", (intptr_t)(&((bot_goalstate_t *)0)->client), sizeof( ((bot_goalstate_t *)0)->client ) },
	{ "int", "lastreachabilityarea", (intptr_t)(&((bot_goalstate_t *)0)->lastreachabilityarea), sizeof( ((bot_goalstate_t *)0)->lastreachabilityarea ) },
	{ "bot_goal_t[8]", "goalstack", (intptr_t)(&((bot_goalstate_t *)0)->goalstack), sizeof( ((bot_goalstate_t *)0)->goalstack ) },
	{ "int", "goalstacktop", (intptr_t)(&((bot_goalstate_t *)0)->goalstacktop), sizeof( ((bot_goalstate_t *)0)->goalstacktop ) },
	{ "int[256]", "avoidgoals", (intptr_t)(&((bot_goalstate_t *)0)->avoidgoals), sizeof( ((bot_goalstate_t *)0)->avoidgoals ) },
	{ "float[256]", "avoidgoaltimes", (intptr_t)(&((bot_goalstate_t *)0)->avoidgoaltimes), sizeof( ((bot_goalstate_t *)0)->avoidgoaltimes ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBotGoalManager_typeInfo[] = {
	{ ": bot_goalstate_t[1]", "botgoalstates", (intptr_t)(&((idBotGoalManager *)0)->botgoalstates), sizeof( ((idBotGoalManager *)0)->botgoalstates ) },
	{ "itemconfig_t", "itemconfiglocal", (intptr_t)(&((idBotGoalManager *)0)->itemconfiglocal), sizeof( ((idBotGoalManager *)0)->itemconfiglocal ) },
	{ "itemconfig_t *", "itemconfig", (intptr_t)(&((idBotGoalManager *)0)->itemconfig), sizeof( ((idBotGoalManager *)0)->itemconfig ) },
	{ "levelitem_t[256]", "levelitemheap", (intptr_t)(&((idBotGoalManager *)0)->levelitemheap), sizeof( ((idBotGoalManager *)0)->levelitemheap ) },
	{ "levelitem_t *", "freelevelitems", (intptr_t)(&((idBotGoalManager *)0)->freelevelitems), sizeof( ((idBotGoalManager *)0)->freelevelitems ) },
	{ "levelitem_t *", "levelitems", (intptr_t)(&((idBotGoalManager *)0)->levelitems), sizeof( ((idBotGoalManager *)0)->levelitems ) },
	{ "int", "numlevelitems", (intptr_t)(&((idBotGoalManager *)0)->numlevelitems), sizeof( ((idBotGoalManager *)0)->numlevelitems ) },
	{ "idList < maplocation_t >", "maplocations", (intptr_t)(&((idBotGoalManager *)0)->maplocations), sizeof( ((idBotGoalManager *)0)->maplocations ) },
	{ "idList < campspot_t >", "campspots", (intptr_t)(&((idBotGoalManager *)0)->campspots), sizeof( ((idBotGoalManager *)0)->campspots ) },
	{ NULL, 0 }
};

static classVariableInfo_t bot_state_t_typeInfo[] = {
	{ "bot_character_t *", "character", (intptr_t)(&((bot_state_t *)0)->character), sizeof( ((bot_state_t *)0)->character ) },
	{ "int", "gs", (intptr_t)(&((bot_state_t *)0)->gs), sizeof( ((bot_state_t *)0)->gs ) },
	{ "int", "ws", (intptr_t)(&((bot_state_t *)0)->ws), sizeof( ((bot_state_t *)0)->ws ) },
	{ "int", "enemy", (intptr_t)(&((bot_state_t *)0)->enemy), sizeof( ((bot_state_t *)0)->enemy ) },
	{ "int", "client", (intptr_t)(&((bot_state_t *)0)->client), sizeof( ((bot_state_t *)0)->client ) },
	{ "idEntity *", "attackerEntity", (intptr_t)(&((bot_state_t *)0)->attackerEntity), sizeof( ((bot_state_t *)0)->attackerEntity ) },
	{ "int", "lasthealth", (intptr_t)(&((bot_state_t *)0)->lasthealth), sizeof( ((bot_state_t *)0)->lasthealth ) },
	{ "int", "entitynum", (intptr_t)(&((bot_state_t *)0)->entitynum), sizeof( ((bot_state_t *)0)->entitynum ) },
	{ "int", "setupcount", (intptr_t)(&((bot_state_t *)0)->setupcount), sizeof( ((bot_state_t *)0)->setupcount ) },
	{ "int", "ltg_time", (intptr_t)(&((bot_state_t *)0)->ltg_time), sizeof( ((bot_state_t *)0)->ltg_time ) },
	{ "int", "flags", (intptr_t)(&((bot_state_t *)0)->flags), sizeof( ((bot_state_t *)0)->flags ) },
	{ "int", "weaponnum", (intptr_t)(&((bot_state_t *)0)->weaponnum), sizeof( ((bot_state_t *)0)->weaponnum ) },
	{ "bool", "useRandomPosition", (intptr_t)(&((bot_state_t *)0)->useRandomPosition), sizeof( ((bot_state_t *)0)->useRandomPosition ) },
	{ "float", "thinktime", (intptr_t)(&((bot_state_t *)0)->thinktime), sizeof( ((bot_state_t *)0)->thinktime ) },
	{ "float", "chase_time", (intptr_t)(&((bot_state_t *)0)->chase_time), sizeof( ((bot_state_t *)0)->chase_time ) },
	{ "float", "attackjump_time", (intptr_t)(&((bot_state_t *)0)->attackjump_time), sizeof( ((bot_state_t *)0)->attackjump_time ) },
	{ "float", "attackcrouch_time", (intptr_t)(&((bot_state_t *)0)->attackcrouch_time), sizeof( ((bot_state_t *)0)->attackcrouch_time ) },
	{ "float", "attackstrafe_time", (intptr_t)(&((bot_state_t *)0)->attackstrafe_time), sizeof( ((bot_state_t *)0)->attackstrafe_time ) },
	{ "float", "attackchase_time", (intptr_t)(&((bot_state_t *)0)->attackchase_time), sizeof( ((bot_state_t *)0)->attackchase_time ) },
	{ "float", "firethrottlewait_time", (intptr_t)(&((bot_state_t *)0)->firethrottlewait_time), sizeof( ((bot_state_t *)0)->firethrottlewait_time ) },
	{ "float", "firethrottleshoot_time", (intptr_t)(&((bot_state_t *)0)->firethrottleshoot_time), sizeof( ((bot_state_t *)0)->firethrottleshoot_time ) },
	{ "float", "nbg_time", (intptr_t)(&((bot_state_t *)0)->nbg_time), sizeof( ((bot_state_t *)0)->nbg_time ) },
	{ "float", "entergame_time", (intptr_t)(&((bot_state_t *)0)->entergame_time), sizeof( ((bot_state_t *)0)->entergame_time ) },
	{ "float", "weaponchange_time", (intptr_t)(&((bot_state_t *)0)->weaponchange_time), sizeof( ((bot_state_t *)0)->weaponchange_time ) },
	{ "float", "check_time", (intptr_t)(&((bot_state_t *)0)->check_time), sizeof( ((bot_state_t *)0)->check_time ) },
	{ "float", "teleport_time", (intptr_t)(&((bot_state_t *)0)->teleport_time), sizeof( ((bot_state_t *)0)->teleport_time ) },
	{ "float", "enemyvisible_time", (intptr_t)(&((bot_state_t *)0)->enemyvisible_time), sizeof( ((bot_state_t *)0)->enemyvisible_time ) },
	{ "int", "enemysuicide", (intptr_t)(&((bot_state_t *)0)->enemysuicide), sizeof( ((bot_state_t *)0)->enemysuicide ) },
	{ "float", "enemysight_time", (intptr_t)(&((bot_state_t *)0)->enemysight_time), sizeof( ((bot_state_t *)0)->enemysight_time ) },
	{ "float", "enemydeath_time", (intptr_t)(&((bot_state_t *)0)->enemydeath_time), sizeof( ((bot_state_t *)0)->enemydeath_time ) },
	{ "float", "aggressiveAttackTime", (intptr_t)(&((bot_state_t *)0)->aggressiveAttackTime), sizeof( ((bot_state_t *)0)->aggressiveAttackTime ) },
	{ "idVec3", "origin", (intptr_t)(&((bot_state_t *)0)->origin), sizeof( ((bot_state_t *)0)->origin ) },
	{ "idVec3", "aimtarget", (intptr_t)(&((bot_state_t *)0)->aimtarget), sizeof( ((bot_state_t *)0)->aimtarget ) },
	{ "idVec3", "random_move_position", (intptr_t)(&((bot_state_t *)0)->random_move_position), sizeof( ((bot_state_t *)0)->random_move_position ) },
	{ "idVec3", "last_enemy_visible_position", (intptr_t)(&((bot_state_t *)0)->last_enemy_visible_position), sizeof( ((bot_state_t *)0)->last_enemy_visible_position ) },
	{ "idAngles", "viewangles", (intptr_t)(&((bot_state_t *)0)->viewangles), sizeof( ((bot_state_t *)0)->viewangles ) },
	{ "idVec3", "enemyorigin", (intptr_t)(&((bot_state_t *)0)->enemyorigin), sizeof( ((bot_state_t *)0)->enemyorigin ) },
	{ "idVec3", "eye", (intptr_t)(&((bot_state_t *)0)->eye), sizeof( ((bot_state_t *)0)->eye ) },
	{ "idVec3", "lastenemyorigin", (intptr_t)(&((bot_state_t *)0)->lastenemyorigin), sizeof( ((bot_state_t *)0)->lastenemyorigin ) },
	{ "int[256]", "inventory", (intptr_t)(&((bot_state_t *)0)->inventory), sizeof( ((bot_state_t *)0)->inventory ) },
	{ "bot_goal_t", "currentGoal", (intptr_t)(&((bot_state_t *)0)->currentGoal), sizeof( ((bot_state_t *)0)->currentGoal ) },
	{ "bot_input_t", "botinput", (intptr_t)(&((bot_state_t *)0)->botinput), sizeof( ((bot_state_t *)0)->botinput ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceBot_typeInfo[] = {
	{ ": bot_state_t", "bs", (intptr_t)(&((iceBot *)0)->bs), sizeof( ((iceBot *)0)->bs ) },
	{ "bool", "hasSpawned", (intptr_t)(&((iceBot *)0)->hasSpawned), sizeof( ((iceBot *)0)->hasSpawned ) },
	{ ": int", "weapon_machinegun", (intptr_t)(&((iceBot *)0)->weapon_machinegun), sizeof( ((iceBot *)0)->weapon_machinegun ) },
	{ "int", "weapon_shotgun", (intptr_t)(&((iceBot *)0)->weapon_shotgun), sizeof( ((iceBot *)0)->weapon_shotgun ) },
	{ "int", "weapon_plasmagun", (intptr_t)(&((iceBot *)0)->weapon_plasmagun), sizeof( ((iceBot *)0)->weapon_plasmagun ) },
	{ "int", "weapon_rocketlauncher", (intptr_t)(&((iceBot *)0)->weapon_rocketlauncher), sizeof( ((iceBot *)0)->weapon_rocketlauncher ) },
	{ ": idAAS *", "aas", (intptr_t)(&((iceBot *)0)->aas), sizeof( ((iceBot *)0)->aas ) },
	{ NULL, 0 }
};

static classVariableInfo_t idTestModel_typeInfo[] = {
	{ ": idEntityPtr < idEntity >", "head", (intptr_t)(&((idTestModel *)0)->head), sizeof( ((idTestModel *)0)->head ) },
	{ "idAnimator *", "headAnimator", (intptr_t)(&((idTestModel *)0)->headAnimator), sizeof( ((idTestModel *)0)->headAnimator ) },
	{ "idAnim", "customAnim", (intptr_t)(&((idTestModel *)0)->customAnim), sizeof( ((idTestModel *)0)->customAnim ) },
	{ "idPhysics_Parametric", "physicsObj", (intptr_t)(&((idTestModel *)0)->physicsObj), sizeof( ((idTestModel *)0)->physicsObj ) },
	{ "idStr", "animname", (intptr_t)(&((idTestModel *)0)->animname), sizeof( ((idTestModel *)0)->animname ) },
	{ "int", "anim", (intptr_t)(&((idTestModel *)0)->anim), sizeof( ((idTestModel *)0)->anim ) },
	{ "int", "headAnim", (intptr_t)(&((idTestModel *)0)->headAnim), sizeof( ((idTestModel *)0)->headAnim ) },
	{ "int", "mode", (intptr_t)(&((idTestModel *)0)->mode), sizeof( ((idTestModel *)0)->mode ) },
	{ "int", "frame", (intptr_t)(&((idTestModel *)0)->frame), sizeof( ((idTestModel *)0)->frame ) },
	{ "int", "starttime", (intptr_t)(&((idTestModel *)0)->starttime), sizeof( ((idTestModel *)0)->starttime ) },
	{ "int", "animtime", (intptr_t)(&((idTestModel *)0)->animtime), sizeof( ((idTestModel *)0)->animtime ) },
	{ "idList < copyJoints_t >", "copyJoints", (intptr_t)(&((idTestModel *)0)->copyJoints), sizeof( ((idTestModel *)0)->copyJoints ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponFist_typeInfo[] = {
	{ "bool", "side", (intptr_t)(&((iceWeaponFist *)0)->side), sizeof( ((iceWeaponFist *)0)->side ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponPistol_typeInfo[] = {
	{ ": float", "spread", (intptr_t)(&((iceWeaponPistol *)0)->spread), sizeof( ((iceWeaponPistol *)0)->spread ) },
	{ "const idSoundShader *", "snd_lowammo", (intptr_t)(&((iceWeaponPistol *)0)->snd_lowammo), sizeof( ((iceWeaponPistol *)0)->snd_lowammo ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponFlashlight_typeInfo[] = {
	{ "bool", "on", (intptr_t)(&((iceWeaponFlashlight *)0)->on), sizeof( ((iceWeaponFlashlight *)0)->on ) },
	{ "float", "intensity", (intptr_t)(&((iceWeaponFlashlight *)0)->intensity), sizeof( ((iceWeaponFlashlight *)0)->intensity ) },
	{ "idStr", "skin_on", (intptr_t)(&((iceWeaponFlashlight *)0)->skin_on), sizeof( ((iceWeaponFlashlight *)0)->skin_on ) },
	{ "idStr", "skin_on_invis", (intptr_t)(&((iceWeaponFlashlight *)0)->skin_on_invis), sizeof( ((iceWeaponFlashlight *)0)->skin_on_invis ) },
	{ "idStr", "skin_off", (intptr_t)(&((iceWeaponFlashlight *)0)->skin_off), sizeof( ((iceWeaponFlashlight *)0)->skin_off ) },
	{ "idStr", "skin_off_invis", (intptr_t)(&((iceWeaponFlashlight *)0)->skin_off_invis), sizeof( ((iceWeaponFlashlight *)0)->skin_off_invis ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponPDA_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponShotgun_typeInfo[] = {
	{ ": float", "spread", (intptr_t)(&((iceWeaponShotgun *)0)->spread), sizeof( ((iceWeaponShotgun *)0)->spread ) },
	{ "const idSoundShader *", "snd_lowammo", (intptr_t)(&((iceWeaponShotgun *)0)->snd_lowammo), sizeof( ((iceWeaponShotgun *)0)->snd_lowammo ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponDoubleShotgun_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponMachineGun_typeInfo[] = {
	{ ": float", "spread", (intptr_t)(&((iceWeaponMachineGun *)0)->spread), sizeof( ((iceWeaponMachineGun *)0)->spread ) },
	{ "const idSoundShader *", "snd_lowammo", (intptr_t)(&((iceWeaponMachineGun *)0)->snd_lowammo), sizeof( ((iceWeaponMachineGun *)0)->snd_lowammo ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponPlasmaGun_typeInfo[] = {
	{ ": float", "spread", (intptr_t)(&((iceWeaponPlasmaGun *)0)->spread), sizeof( ((iceWeaponPlasmaGun *)0)->spread ) },
	{ "const idSoundShader *", "snd_lowammo", (intptr_t)(&((iceWeaponPlasmaGun *)0)->snd_lowammo), sizeof( ((iceWeaponPlasmaGun *)0)->snd_lowammo ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponChainGun_typeInfo[] = {
	{ ": idAnimatedEntity *", "world_model", (intptr_t)(&((iceWeaponChainGun *)0)->world_model), sizeof( ((iceWeaponChainGun *)0)->world_model ) },
	{ "jointHandle_t", "world_barrel_joint", (intptr_t)(&((iceWeaponChainGun *)0)->world_barrel_joint), sizeof( ((iceWeaponChainGun *)0)->world_barrel_joint ) },
	{ "jointHandle_t", "barrel_joint", (intptr_t)(&((iceWeaponChainGun *)0)->barrel_joint), sizeof( ((iceWeaponChainGun *)0)->barrel_joint ) },
	{ "float", "barrel_angle", (intptr_t)(&((iceWeaponChainGun *)0)->barrel_angle), sizeof( ((iceWeaponChainGun *)0)->barrel_angle ) },
	{ "float", "current_rate", (intptr_t)(&((iceWeaponChainGun *)0)->current_rate), sizeof( ((iceWeaponChainGun *)0)->current_rate ) },
	{ "float", "start_rate", (intptr_t)(&((iceWeaponChainGun *)0)->start_rate), sizeof( ((iceWeaponChainGun *)0)->start_rate ) },
	{ "float", "end_rate", (intptr_t)(&((iceWeaponChainGun *)0)->end_rate), sizeof( ((iceWeaponChainGun *)0)->end_rate ) },
	{ "float", "spin_start", (intptr_t)(&((iceWeaponChainGun *)0)->spin_start), sizeof( ((iceWeaponChainGun *)0)->spin_start ) },
	{ "float", "spin_end", (intptr_t)(&((iceWeaponChainGun *)0)->spin_end), sizeof( ((iceWeaponChainGun *)0)->spin_end ) },
	{ "float", "spread", (intptr_t)(&((iceWeaponChainGun *)0)->spread), sizeof( ((iceWeaponChainGun *)0)->spread ) },
	{ "int", "numSkipFrames", (intptr_t)(&((iceWeaponChainGun *)0)->numSkipFrames), sizeof( ((iceWeaponChainGun *)0)->numSkipFrames ) },
	{ "const idSoundShader *", "snd_windup", (intptr_t)(&((iceWeaponChainGun *)0)->snd_windup), sizeof( ((iceWeaponChainGun *)0)->snd_windup ) },
	{ "const idSoundShader *", "snd_winddown", (intptr_t)(&((iceWeaponChainGun *)0)->snd_winddown), sizeof( ((iceWeaponChainGun *)0)->snd_winddown ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponRocketLauncher_typeInfo[] = {
	{ "float", "spread", (intptr_t)(&((iceWeaponRocketLauncher *)0)->spread), sizeof( ((iceWeaponRocketLauncher *)0)->spread ) },
	{ "idStr", "skin_invisible", (intptr_t)(&((iceWeaponRocketLauncher *)0)->skin_invisible), sizeof( ((iceWeaponRocketLauncher *)0)->skin_invisible ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponBFG_typeInfo[] = {
	{ "float", "spread", (intptr_t)(&((iceWeaponBFG *)0)->spread), sizeof( ((iceWeaponBFG *)0)->spread ) },
	{ "float", "fuse_start", (intptr_t)(&((iceWeaponBFG *)0)->fuse_start), sizeof( ((iceWeaponBFG *)0)->fuse_start ) },
	{ "float", "fuse_end", (intptr_t)(&((iceWeaponBFG *)0)->fuse_end), sizeof( ((iceWeaponBFG *)0)->fuse_end ) },
	{ "float", "powerLevel", (intptr_t)(&((iceWeaponBFG *)0)->powerLevel), sizeof( ((iceWeaponBFG *)0)->powerLevel ) },
	{ "float", "fire_time", (intptr_t)(&((iceWeaponBFG *)0)->fire_time), sizeof( ((iceWeaponBFG *)0)->fire_time ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponHandgrenade_typeInfo[] = {
	{ ": float", "spread", (intptr_t)(&((iceWeaponHandgrenade *)0)->spread), sizeof( ((iceWeaponHandgrenade *)0)->spread ) },
	{ "float", "fuse_start", (intptr_t)(&((iceWeaponHandgrenade *)0)->fuse_start), sizeof( ((iceWeaponHandgrenade *)0)->fuse_start ) },
	{ "idStr", "skin_nade", (intptr_t)(&((iceWeaponHandgrenade *)0)->skin_nade), sizeof( ((iceWeaponHandgrenade *)0)->skin_nade ) },
	{ "idStr", "skin_nade_invis", (intptr_t)(&((iceWeaponHandgrenade *)0)->skin_nade_invis), sizeof( ((iceWeaponHandgrenade *)0)->skin_nade_invis ) },
	{ "idStr", "skin_nonade", (intptr_t)(&((iceWeaponHandgrenade *)0)->skin_nonade), sizeof( ((iceWeaponHandgrenade *)0)->skin_nonade ) },
	{ "idStr", "skin_nonade_invis", (intptr_t)(&((iceWeaponHandgrenade *)0)->skin_nonade_invis), sizeof( ((iceWeaponHandgrenade *)0)->skin_nonade_invis ) },
	{ "idProjectile *", "projectile", (intptr_t)(&((iceWeaponHandgrenade *)0)->projectile), sizeof( ((iceWeaponHandgrenade *)0)->projectile ) },
	{ "bool", "show_grenade", (intptr_t)(&((iceWeaponHandgrenade *)0)->show_grenade), sizeof( ((iceWeaponHandgrenade *)0)->show_grenade ) },
	{ ": float", "fuse_end", (intptr_t)(&((iceWeaponHandgrenade *)0)->fuse_end), sizeof( ((iceWeaponHandgrenade *)0)->fuse_end ) },
	{ "float", "current_time", (intptr_t)(&((iceWeaponHandgrenade *)0)->current_time), sizeof( ((iceWeaponHandgrenade *)0)->current_time ) },
	{ "float", "time_held", (intptr_t)(&((iceWeaponHandgrenade *)0)->time_held), sizeof( ((iceWeaponHandgrenade *)0)->time_held ) },
	{ "float", "power", (intptr_t)(&((iceWeaponHandgrenade *)0)->power), sizeof( ((iceWeaponHandgrenade *)0)->power ) },
	{ "bool", "exploded", (intptr_t)(&((iceWeaponHandgrenade *)0)->exploded), sizeof( ((iceWeaponHandgrenade *)0)->exploded ) },
	{ "const idSoundShader *", "snd_lowammo", (intptr_t)(&((iceWeaponHandgrenade *)0)->snd_lowammo), sizeof( ((iceWeaponHandgrenade *)0)->snd_lowammo ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponChainsaw_typeInfo[] = {
	{ ": bool", "side", (intptr_t)(&((iceWeaponChainsaw *)0)->side), sizeof( ((iceWeaponChainsaw *)0)->side ) },
	{ NULL, 0 }
};

static classVariableInfo_t iceWeaponGrabber_typeInfo[] = {
	{ "const idSoundShader *", "snd_fireloop", (intptr_t)(&((iceWeaponGrabber *)0)->snd_fireloop), sizeof( ((iceWeaponGrabber *)0)->snd_fireloop ) },
	{ "const idSoundShader *", "snd_electroloop", (intptr_t)(&((iceWeaponGrabber *)0)->snd_electroloop), sizeof( ((iceWeaponGrabber *)0)->snd_electroloop ) },
	{ "const idSoundShader *", "snd_mainfire", (intptr_t)(&((iceWeaponGrabber *)0)->snd_mainfire), sizeof( ((iceWeaponGrabber *)0)->snd_mainfire ) },
	{ "const idSoundShader *", "snd_cangrab", (intptr_t)(&((iceWeaponGrabber *)0)->snd_cangrab), sizeof( ((iceWeaponGrabber *)0)->snd_cangrab ) },
	{ "const idSoundShader *", "snd_warning", (intptr_t)(&((iceWeaponGrabber *)0)->snd_warning), sizeof( ((iceWeaponGrabber *)0)->snd_warning ) },
	{ "const idSoundShader *", "snd_stopfire", (intptr_t)(&((iceWeaponGrabber *)0)->snd_stopfire), sizeof( ((iceWeaponGrabber *)0)->snd_stopfire ) },
	{ ": float", "next_attack", (intptr_t)(&((iceWeaponGrabber *)0)->next_attack), sizeof( ((iceWeaponGrabber *)0)->next_attack ) },
	{ "float", "fireStartTime", (intptr_t)(&((iceWeaponGrabber *)0)->fireStartTime), sizeof( ((iceWeaponGrabber *)0)->fireStartTime ) },
	{ "bool", "warningBeep1", (intptr_t)(&((iceWeaponGrabber *)0)->warningBeep1), sizeof( ((iceWeaponGrabber *)0)->warningBeep1 ) },
	{ "bool", "warningBeep2", (intptr_t)(&((iceWeaponGrabber *)0)->warningBeep2), sizeof( ((iceWeaponGrabber *)0)->warningBeep2 ) },
	{ "bool", "warningBeep3", (intptr_t)(&((iceWeaponGrabber *)0)->warningBeep3), sizeof( ((iceWeaponGrabber *)0)->warningBeep3 ) },
	{ "bool", "warningBeep4", (intptr_t)(&((iceWeaponGrabber *)0)->warningBeep4), sizeof( ((iceWeaponGrabber *)0)->warningBeep4 ) },
	{ "float", "grabberState", (intptr_t)(&((iceWeaponGrabber *)0)->grabberState), sizeof( ((iceWeaponGrabber *)0)->grabberState ) },
	{ NULL, 0 }
};

static classVariableInfo_t widgetTransition_t_typeInfo[] = {
	{ "const char *", "animationName", (intptr_t)(&((widgetTransition_t *)0)->animationName), sizeof( ((widgetTransition_t *)0)->animationName ) },
	{ "idStaticList < const char * , 4 >", "prefixes", (intptr_t)(&((widgetTransition_t *)0)->prefixes), sizeof( ((widgetTransition_t *)0)->prefixes ) },
	{ NULL, 0 }
};

static classVariableInfo_t scoreboardInfo_t_typeInfo[] = {
	{ "idList < idStr , TAG_IDLIB_LIST_MENU >", "values", (intptr_t)(&((scoreboardInfo_t *)0)->values), sizeof( ((scoreboardInfo_t *)0)->values ) },
	{ "int", "index", (intptr_t)(&((scoreboardInfo_t *)0)->index), sizeof( ((scoreboardInfo_t *)0)->index ) },
	{ "voiceStateDisplay_t", "voiceState", (intptr_t)(&((scoreboardInfo_t *)0)->voiceState), sizeof( ((scoreboardInfo_t *)0)->voiceState ) },
	{ NULL, 0 }
};

static classVariableInfo_t idSort_SavesByDate_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idMenuDataSource_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idWidgetEvent_typeInfo[] = {
	{ "widgetEvent_t", "type", (intptr_t)(&((idWidgetEvent *)0)->type), sizeof( ((idWidgetEvent *)0)->type ) },
	{ "int", "arg", (intptr_t)(&((idWidgetEvent *)0)->arg), sizeof( ((idWidgetEvent *)0)->arg ) },
	{ "idSWFScriptObject *", "thisObject", (intptr_t)(&((idWidgetEvent *)0)->thisObject), sizeof( ((idWidgetEvent *)0)->thisObject ) },
	{ "idSWFParmList", "parms", (intptr_t)(&((idWidgetEvent *)0)->parms), sizeof( ((idWidgetEvent *)0)->parms ) },
	{ NULL, 0 }
};

static classVariableInfo_t idWidgetAction_typeInfo[] = {
	{ ": widgetAction_t", "action", (intptr_t)(&((idWidgetAction *)0)->action), sizeof( ((idWidgetAction *)0)->action ) },
	{ "idSWFParmList", "parms", (intptr_t)(&((idWidgetAction *)0)->parms), sizeof( ((idWidgetAction *)0)->parms ) },
	{ "idSWFScriptFunction *", "scriptFunction", (intptr_t)(&((idWidgetAction *)0)->scriptFunction), sizeof( ((idWidgetAction *)0)->scriptFunction ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_WrapWidgetSWFEvent_typeInfo[] = {
	{ ": idMenuWidget *", "targetWidget", (intptr_t)(&((idMenuWidget::WrapWidgetSWFEvent *)0)->targetWidget), sizeof( ((idMenuWidget::WrapWidgetSWFEvent *)0)->targetWidget ) },
	{ "widgetEvent_t", "targetEvent", (intptr_t)(&((idMenuWidget::WrapWidgetSWFEvent *)0)->targetEvent), sizeof( ((idMenuWidget::WrapWidgetSWFEvent *)0)->targetEvent ) },
	{ "int", "targetEventArg", (intptr_t)(&((idMenuWidget::WrapWidgetSWFEvent *)0)->targetEventArg), sizeof( ((idMenuWidget::WrapWidgetSWFEvent *)0)->targetEventArg ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_typeInfo[] = {
	{ ": bool", "handlerIsParent", (intptr_t)(&((idMenuWidget *)0)->handlerIsParent), sizeof( ((idMenuWidget *)0)->handlerIsParent ) },
	{ "idMenuHandler *", "menuData", (intptr_t)(&((idMenuWidget *)0)->menuData), sizeof( ((idMenuWidget *)0)->menuData ) },
	{ "idSWF *", "swfObj", (intptr_t)(&((idMenuWidget *)0)->swfObj), sizeof( ((idMenuWidget *)0)->swfObj ) },
	{ "idSWFSpriteInstance *", "boundSprite", (intptr_t)(&((idMenuWidget *)0)->boundSprite), sizeof( ((idMenuWidget *)0)->boundSprite ) },
	{ "idMenuWidget *", "parent", (intptr_t)(&((idMenuWidget *)0)->parent), sizeof( ((idMenuWidget *)0)->parent ) },
	{ "idList < idStr , TAG_IDLIB_LIST_MENU >", "spritePath", (intptr_t)(&((idMenuWidget *)0)->spritePath), sizeof( ((idMenuWidget *)0)->spritePath ) },
	{ "idMenuWidgetList", "children", (intptr_t)(&((idMenuWidget *)0)->children), sizeof( ((idMenuWidget *)0)->children ) },
	{ "idMenuWidgetList", "observers", (intptr_t)(&((idMenuWidget *)0)->observers), sizeof( ((idMenuWidget *)0)->observers ) },
	{ "idList < idList < idWidgetAction , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "eventActions", (intptr_t)(&((idMenuWidget *)0)->eventActions), sizeof( ((idMenuWidget *)0)->eventActions ) },
	{ "idStaticList < int , MAX_WIDGET_EVENT >", "eventActionLookup", (intptr_t)(&((idMenuWidget *)0)->eventActionLookup), sizeof( ((idMenuWidget *)0)->eventActionLookup ) },
	{ "idMenuDataSource *", "dataSource", (intptr_t)(&((idMenuWidget *)0)->dataSource), sizeof( ((idMenuWidget *)0)->dataSource ) },
	{ "int", "dataSourceFieldIndex", (intptr_t)(&((idMenuWidget *)0)->dataSourceFieldIndex), sizeof( ((idMenuWidget *)0)->dataSourceFieldIndex ) },
	{ "int", "focusIndex", (intptr_t)(&((idMenuWidget *)0)->focusIndex), sizeof( ((idMenuWidget *)0)->focusIndex ) },
	{ "widgetState_t", "widgetState", (intptr_t)(&((idMenuWidget *)0)->widgetState), sizeof( ((idMenuWidget *)0)->widgetState ) },
	{ "int", "refCount", (intptr_t)(&((idMenuWidget *)0)->refCount), sizeof( ((idMenuWidget *)0)->refCount ) },
	{ "bool", "noAutoFree", (intptr_t)(&((idMenuWidget *)0)->noAutoFree), sizeof( ((idMenuWidget *)0)->noAutoFree ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_Button_typeInfo[] = {
	{ "idList < idStr , TAG_IDLIB_LIST_MENU >", "values", (intptr_t)(&((idMenuWidget_Button *)0)->values), sizeof( ((idMenuWidget_Button *)0)->values ) },
	{ "idStr", "btnLabel", (intptr_t)(&((idMenuWidget_Button *)0)->btnLabel), sizeof( ((idMenuWidget_Button *)0)->btnLabel ) },
	{ "idStr", "description", (intptr_t)(&((idMenuWidget_Button *)0)->description), sizeof( ((idMenuWidget_Button *)0)->description ) },
	{ "animState_t", "animState", (intptr_t)(&((idMenuWidget_Button *)0)->animState), sizeof( ((idMenuWidget_Button *)0)->animState ) },
	{ "const idMaterial *", "img", (intptr_t)(&((idMenuWidget_Button *)0)->img), sizeof( ((idMenuWidget_Button *)0)->img ) },
	{ "idSWFScriptFunction *", "scriptFunction", (intptr_t)(&((idMenuWidget_Button *)0)->scriptFunction), sizeof( ((idMenuWidget_Button *)0)->scriptFunction ) },
	{ "bool", "ignoreColor", (intptr_t)(&((idMenuWidget_Button *)0)->ignoreColor), sizeof( ((idMenuWidget_Button *)0)->ignoreColor ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_LobbyButton_typeInfo[] = {
	{ ": idStr", "name", (intptr_t)(&((idMenuWidget_LobbyButton *)0)->name), sizeof( ((idMenuWidget_LobbyButton *)0)->name ) },
	{ "voiceStateDisplay_t", "voiceState", (intptr_t)(&((idMenuWidget_LobbyButton *)0)->voiceState), sizeof( ((idMenuWidget_LobbyButton *)0)->voiceState ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_ScoreboardButton_typeInfo[] = {
	{ ": voiceStateDisplay_t", "voiceState", (intptr_t)(&((idMenuWidget_ScoreboardButton *)0)->voiceState), sizeof( ((idMenuWidget_ScoreboardButton *)0)->voiceState ) },
	{ "int", "index", (intptr_t)(&((idMenuWidget_ScoreboardButton *)0)->index), sizeof( ((idMenuWidget_ScoreboardButton *)0)->index ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_ControlButton_typeInfo[] = {
	{ ": menuOption_t", "optionType", (intptr_t)(&((idMenuWidget_ControlButton *)0)->optionType), sizeof( ((idMenuWidget_ControlButton *)0)->optionType ) },
	{ "bool", "disabled", (intptr_t)(&((idMenuWidget_ControlButton *)0)->disabled), sizeof( ((idMenuWidget_ControlButton *)0)->disabled ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_ServerButton_typeInfo[] = {
	{ ": idStr", "serverName", (intptr_t)(&((idMenuWidget_ServerButton *)0)->serverName), sizeof( ((idMenuWidget_ServerButton *)0)->serverName ) },
	{ "int", "index", (intptr_t)(&((idMenuWidget_ServerButton *)0)->index), sizeof( ((idMenuWidget_ServerButton *)0)->index ) },
	{ "int", "players", (intptr_t)(&((idMenuWidget_ServerButton *)0)->players), sizeof( ((idMenuWidget_ServerButton *)0)->players ) },
	{ "int", "maxPlayers", (intptr_t)(&((idMenuWidget_ServerButton *)0)->maxPlayers), sizeof( ((idMenuWidget_ServerButton *)0)->maxPlayers ) },
	{ "bool", "joinable", (intptr_t)(&((idMenuWidget_ServerButton *)0)->joinable), sizeof( ((idMenuWidget_ServerButton *)0)->joinable ) },
	{ "bool", "validMap", (intptr_t)(&((idMenuWidget_ServerButton *)0)->validMap), sizeof( ((idMenuWidget_ServerButton *)0)->validMap ) },
	{ "idStrId", "mapName", (intptr_t)(&((idMenuWidget_ServerButton *)0)->mapName), sizeof( ((idMenuWidget_ServerButton *)0)->mapName ) },
	{ "idStr", "modeName", (intptr_t)(&((idMenuWidget_ServerButton *)0)->modeName), sizeof( ((idMenuWidget_ServerButton *)0)->modeName ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_NavButton_typeInfo[] = {
	{ ": int", "navIndex", (intptr_t)(&((idMenuWidget_NavButton *)0)->navIndex), sizeof( ((idMenuWidget_NavButton *)0)->navIndex ) },
	{ "float", "xPos", (intptr_t)(&((idMenuWidget_NavButton *)0)->xPos), sizeof( ((idMenuWidget_NavButton *)0)->xPos ) },
	{ "navWidgetState_t", "navState", (intptr_t)(&((idMenuWidget_NavButton *)0)->navState), sizeof( ((idMenuWidget_NavButton *)0)->navState ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_MenuButton_typeInfo[] = {
	{ ": float", "xPos", (intptr_t)(&((idMenuWidget_MenuButton *)0)->xPos), sizeof( ((idMenuWidget_MenuButton *)0)->xPos ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_List_typeInfo[] = {
	{ ": int", "numVisibleOptions", (intptr_t)(&((idMenuWidget_List *)0)->numVisibleOptions), sizeof( ((idMenuWidget_List *)0)->numVisibleOptions ) },
	{ "int", "viewOffset", (intptr_t)(&((idMenuWidget_List *)0)->viewOffset), sizeof( ((idMenuWidget_List *)0)->viewOffset ) },
	{ "int", "viewIndex", (intptr_t)(&((idMenuWidget_List *)0)->viewIndex), sizeof( ((idMenuWidget_List *)0)->viewIndex ) },
	{ "bool", "allowWrapping", (intptr_t)(&((idMenuWidget_List *)0)->allowWrapping), sizeof( ((idMenuWidget_List *)0)->allowWrapping ) },
	{ NULL, 0 }
};

static classVariableInfo_t idBrowserEntry_t_typeInfo[] = {
	{ ": idStr", "serverName", (intptr_t)(&((idBrowserEntry_t *)0)->serverName), sizeof( ((idBrowserEntry_t *)0)->serverName ) },
	{ "int", "index", (intptr_t)(&((idBrowserEntry_t *)0)->index), sizeof( ((idBrowserEntry_t *)0)->index ) },
	{ "int", "players", (intptr_t)(&((idBrowserEntry_t *)0)->players), sizeof( ((idBrowserEntry_t *)0)->players ) },
	{ "int", "maxPlayers", (intptr_t)(&((idBrowserEntry_t *)0)->maxPlayers), sizeof( ((idBrowserEntry_t *)0)->maxPlayers ) },
	{ "bool", "joinable", (intptr_t)(&((idBrowserEntry_t *)0)->joinable), sizeof( ((idBrowserEntry_t *)0)->joinable ) },
	{ "bool", "validMap", (intptr_t)(&((idBrowserEntry_t *)0)->validMap), sizeof( ((idBrowserEntry_t *)0)->validMap ) },
	{ "idStrId", "mapName", (intptr_t)(&((idBrowserEntry_t *)0)->mapName), sizeof( ((idBrowserEntry_t *)0)->mapName ) },
	{ "idStr", "modeName", (intptr_t)(&((idBrowserEntry_t *)0)->modeName), sizeof( ((idBrowserEntry_t *)0)->modeName ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_GameBrowserList_typeInfo[] = {
	{ ": idList < idBrowserEntry_t >", "games", (intptr_t)(&((idMenuWidget_GameBrowserList *)0)->games), sizeof( ((idMenuWidget_GameBrowserList *)0)->games ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_Carousel_typeInfo[] = {
	{ ": int", "numVisibleOptions", (intptr_t)(&((idMenuWidget_Carousel *)0)->numVisibleOptions), sizeof( ((idMenuWidget_Carousel *)0)->numVisibleOptions ) },
	{ "int", "viewIndex", (intptr_t)(&((idMenuWidget_Carousel *)0)->viewIndex), sizeof( ((idMenuWidget_Carousel *)0)->viewIndex ) },
	{ "int", "moveToIndex", (intptr_t)(&((idMenuWidget_Carousel *)0)->moveToIndex), sizeof( ((idMenuWidget_Carousel *)0)->moveToIndex ) },
	{ "int", "moveDiff", (intptr_t)(&((idMenuWidget_Carousel *)0)->moveDiff), sizeof( ((idMenuWidget_Carousel *)0)->moveDiff ) },
	{ "bool", "fastScroll", (intptr_t)(&((idMenuWidget_Carousel *)0)->fastScroll), sizeof( ((idMenuWidget_Carousel *)0)->fastScroll ) },
	{ "bool", "scrollLeft", (intptr_t)(&((idMenuWidget_Carousel *)0)->scrollLeft), sizeof( ((idMenuWidget_Carousel *)0)->scrollLeft ) },
	{ "idList < const idMaterial * >", "imgList", (intptr_t)(&((idMenuWidget_Carousel *)0)->imgList), sizeof( ((idMenuWidget_Carousel *)0)->imgList ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_Help_typeInfo[] = {
	{ ": idStr", "lastFocusedMessage", (intptr_t)(&((idMenuWidget_Help *)0)->lastFocusedMessage), sizeof( ((idMenuWidget_Help *)0)->lastFocusedMessage ) },
	{ "idStr", "lastHoveredMessage", (intptr_t)(&((idMenuWidget_Help *)0)->lastHoveredMessage), sizeof( ((idMenuWidget_Help *)0)->lastHoveredMessage ) },
	{ "bool", "hideMessage", (intptr_t)(&((idMenuWidget_Help *)0)->hideMessage), sizeof( ((idMenuWidget_Help *)0)->hideMessage ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_CommandBar_buttonInfo_t_typeInfo[] = {
	{ "idStr", "label", (intptr_t)(&((idMenuWidget_CommandBar::buttonInfo_t *)0)->label), sizeof( ((idMenuWidget_CommandBar::buttonInfo_t *)0)->label ) },
	{ "idWidgetAction", "action", (intptr_t)(&((idMenuWidget_CommandBar::buttonInfo_t *)0)->action), sizeof( ((idMenuWidget_CommandBar::buttonInfo_t *)0)->action ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_CommandBar_typeInfo[] = {
	{ ": idStaticList < buttonInfo_t , MAX_BUTTONS >", "buttons", (intptr_t)(&((idMenuWidget_CommandBar *)0)->buttons), sizeof( ((idMenuWidget_CommandBar *)0)->buttons ) },
	{ "alignment_t", "alignment", (intptr_t)(&((idMenuWidget_CommandBar *)0)->alignment), sizeof( ((idMenuWidget_CommandBar *)0)->alignment ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_LobbyList_typeInfo[] = {
	{ ": idList < idStr , TAG_IDLIB_LIST_MENU >", "headings", (intptr_t)(&((idMenuWidget_LobbyList *)0)->headings), sizeof( ((idMenuWidget_LobbyList *)0)->headings ) },
	{ "int", "numEntries", (intptr_t)(&((idMenuWidget_LobbyList *)0)->numEntries), sizeof( ((idMenuWidget_LobbyList *)0)->numEntries ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_DynamicList_typeInfo[] = {
	{ ": idList < idList < idStr , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "listItemInfo", (intptr_t)(&((idMenuWidget_DynamicList *)0)->listItemInfo), sizeof( ((idMenuWidget_DynamicList *)0)->listItemInfo ) },
	{ "bool", "controlList", (intptr_t)(&((idMenuWidget_DynamicList *)0)->controlList), sizeof( ((idMenuWidget_DynamicList *)0)->controlList ) },
	{ "bool", "ignoreColor", (intptr_t)(&((idMenuWidget_DynamicList *)0)->ignoreColor), sizeof( ((idMenuWidget_DynamicList *)0)->ignoreColor ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_ScoreboardList_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_SystemOptionsList_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_NavBar_typeInfo[] = {
	{ ": idList < idStr , TAG_IDLIB_LIST_MENU >", "headings", (intptr_t)(&((idMenuWidget_NavBar *)0)->headings), sizeof( ((idMenuWidget_NavBar *)0)->headings ) },
	{ "float", "initialPos", (intptr_t)(&((idMenuWidget_NavBar *)0)->initialPos), sizeof( ((idMenuWidget_NavBar *)0)->initialPos ) },
	{ "float", "buttonPos", (intptr_t)(&((idMenuWidget_NavBar *)0)->buttonPos), sizeof( ((idMenuWidget_NavBar *)0)->buttonPos ) },
	{ "float", "leftSpacer", (intptr_t)(&((idMenuWidget_NavBar *)0)->leftSpacer), sizeof( ((idMenuWidget_NavBar *)0)->leftSpacer ) },
	{ "float", "rightSpacer", (intptr_t)(&((idMenuWidget_NavBar *)0)->rightSpacer), sizeof( ((idMenuWidget_NavBar *)0)->rightSpacer ) },
	{ "float", "selectedSpacer", (intptr_t)(&((idMenuWidget_NavBar *)0)->selectedSpacer), sizeof( ((idMenuWidget_NavBar *)0)->selectedSpacer ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_MenuBar_typeInfo[] = {
	{ ": idList < idStr , TAG_IDLIB_LIST_MENU >", "headings", (intptr_t)(&((idMenuWidget_MenuBar *)0)->headings), sizeof( ((idMenuWidget_MenuBar *)0)->headings ) },
	{ "float", "totalWidth", (intptr_t)(&((idMenuWidget_MenuBar *)0)->totalWidth), sizeof( ((idMenuWidget_MenuBar *)0)->totalWidth ) },
	{ "float", "buttonPos", (intptr_t)(&((idMenuWidget_MenuBar *)0)->buttonPos), sizeof( ((idMenuWidget_MenuBar *)0)->buttonPos ) },
	{ "float", "rightSpacer", (intptr_t)(&((idMenuWidget_MenuBar *)0)->rightSpacer), sizeof( ((idMenuWidget_MenuBar *)0)->rightSpacer ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_PDA_UserData_typeInfo[] = {
	{ ": int", "pdaIndex", (intptr_t)(&((idMenuWidget_PDA_UserData *)0)->pdaIndex), sizeof( ((idMenuWidget_PDA_UserData *)0)->pdaIndex ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_ScrollBar_typeInfo[] = {
	{ "float", "yTop", (intptr_t)(&((idMenuWidget_ScrollBar *)0)->yTop), sizeof( ((idMenuWidget_ScrollBar *)0)->yTop ) },
	{ "float", "yBot", (intptr_t)(&((idMenuWidget_ScrollBar *)0)->yBot), sizeof( ((idMenuWidget_ScrollBar *)0)->yBot ) },
	{ "bool", "dragging", (intptr_t)(&((idMenuWidget_ScrollBar *)0)->dragging), sizeof( ((idMenuWidget_ScrollBar *)0)->dragging ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_InfoBox_typeInfo[] = {
	{ ": idMenuWidget_ScrollBar *", "scrollbar", (intptr_t)(&((idMenuWidget_InfoBox *)0)->scrollbar), sizeof( ((idMenuWidget_InfoBox *)0)->scrollbar ) },
	{ "idStr", "heading", (intptr_t)(&((idMenuWidget_InfoBox *)0)->heading), sizeof( ((idMenuWidget_InfoBox *)0)->heading ) },
	{ "idStr", "info", (intptr_t)(&((idMenuWidget_InfoBox *)0)->info), sizeof( ((idMenuWidget_InfoBox *)0)->info ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_PDA_Objective_typeInfo[] = {
	{ ": int", "pdaIndex", (intptr_t)(&((idMenuWidget_PDA_Objective *)0)->pdaIndex), sizeof( ((idMenuWidget_PDA_Objective *)0)->pdaIndex ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_Shell_SaveInfo_typeInfo[] = {
	{ ": int", "loadIndex", (intptr_t)(&((idMenuWidget_Shell_SaveInfo *)0)->loadIndex), sizeof( ((idMenuWidget_Shell_SaveInfo *)0)->loadIndex ) },
	{ "bool", "forSaveScreen", (intptr_t)(&((idMenuWidget_Shell_SaveInfo *)0)->forSaveScreen), sizeof( ((idMenuWidget_Shell_SaveInfo *)0)->forSaveScreen ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_PDA_AudioFiles_typeInfo[] = {
	{ ": int", "pdaIndex", (intptr_t)(&((idMenuWidget_PDA_AudioFiles *)0)->pdaIndex), sizeof( ((idMenuWidget_PDA_AudioFiles *)0)->pdaIndex ) },
	{ "idList < idList < idStr , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "audioFileNames", (intptr_t)(&((idMenuWidget_PDA_AudioFiles *)0)->audioFileNames), sizeof( ((idMenuWidget_PDA_AudioFiles *)0)->audioFileNames ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_PDA_EmailInbox_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "emailList", (intptr_t)(&((idMenuWidget_PDA_EmailInbox *)0)->emailList), sizeof( ((idMenuWidget_PDA_EmailInbox *)0)->emailList ) },
	{ "idMenuWidget_ScrollBar *", "scrollbar", (intptr_t)(&((idMenuWidget_PDA_EmailInbox *)0)->scrollbar), sizeof( ((idMenuWidget_PDA_EmailInbox *)0)->scrollbar ) },
	{ "int", "pdaIndex", (intptr_t)(&((idMenuWidget_PDA_EmailInbox *)0)->pdaIndex), sizeof( ((idMenuWidget_PDA_EmailInbox *)0)->pdaIndex ) },
	{ "idList < idList < idStr , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "emailInfo", (intptr_t)(&((idMenuWidget_PDA_EmailInbox *)0)->emailInfo), sizeof( ((idMenuWidget_PDA_EmailInbox *)0)->emailInfo ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_ItemAssignment_typeInfo[] = {
	{ ": const idMaterial *[4]", "images", (intptr_t)(&((idMenuWidget_ItemAssignment *)0)->images), sizeof( ((idMenuWidget_ItemAssignment *)0)->images ) },
	{ "int", "slotIndex", (intptr_t)(&((idMenuWidget_ItemAssignment *)0)->slotIndex), sizeof( ((idMenuWidget_ItemAssignment *)0)->slotIndex ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuWidget_PDA_VideoInfo_typeInfo[] = {
	{ ": int", "videoIndex", (intptr_t)(&((idMenuWidget_PDA_VideoInfo *)0)->videoIndex), sizeof( ((idMenuWidget_PDA_VideoInfo *)0)->videoIndex ) },
	{ NULL, 0 }
};

static classVariableInfo_t idWidgetActionHandler_typeInfo[] = {
	{ ": idMenuWidget *", "targetWidget", (intptr_t)(&((idWidgetActionHandler *)0)->targetWidget), sizeof( ((idWidgetActionHandler *)0)->targetWidget ) },
	{ "actionHandler_t", "type", (intptr_t)(&((idWidgetActionHandler *)0)->type), sizeof( ((idWidgetActionHandler *)0)->type ) },
	{ "widgetEvent_t", "targetEvent", (intptr_t)(&((idWidgetActionHandler *)0)->targetEvent), sizeof( ((idWidgetActionHandler *)0)->targetEvent ) },
	{ NULL, 0 }
};

static classVariableInfo_t idLBRowBlock_typeInfo[] = {
	{ "int", "lastTime", (intptr_t)(&((idLBRowBlock *)0)->lastTime), sizeof( ((idLBRowBlock *)0)->lastTime ) },
	{ "int", "startIndex", (intptr_t)(&((idLBRowBlock *)0)->startIndex), sizeof( ((idLBRowBlock *)0)->startIndex ) },
	{ "idList < idLeaderboardCallback :: row_t >", "rows", (intptr_t)(&((idLBRowBlock *)0)->rows), sizeof( ((idLBRowBlock *)0)->rows ) },
	{ NULL, 0 }
};

static classVariableInfo_t idLBCache_typeInfo[] = {
	{ "idLBRowBlock[5]", "rowBlocks", (intptr_t)(&((idLBCache *)0)->rowBlocks), sizeof( ((idLBCache *)0)->rowBlocks ) },
	{ "const leaderboardDefinition_t *", "def", (intptr_t)(&((idLBCache *)0)->def), sizeof( ((idLBCache *)0)->def ) },
	{ "leaderboardFilterMode_t", "filter", (intptr_t)(&((idLBCache *)0)->filter), sizeof( ((idLBCache *)0)->filter ) },
	{ "const leaderboardDefinition_t *", "pendingDef", (intptr_t)(&((idLBCache *)0)->pendingDef), sizeof( ((idLBCache *)0)->pendingDef ) },
	{ "leaderboardFilterMode_t", "pendingFilter", (intptr_t)(&((idLBCache *)0)->pendingFilter), sizeof( ((idLBCache *)0)->pendingFilter ) },
	{ "bool", "requestingRows", (intptr_t)(&((idLBCache *)0)->requestingRows), sizeof( ((idLBCache *)0)->requestingRows ) },
	{ "bool", "loadingNewLeaderboard", (intptr_t)(&((idLBCache *)0)->loadingNewLeaderboard), sizeof( ((idLBCache *)0)->loadingNewLeaderboard ) },
	{ "int", "numRowsInLeaderboard", (intptr_t)(&((idLBCache *)0)->numRowsInLeaderboard), sizeof( ((idLBCache *)0)->numRowsInLeaderboard ) },
	{ "int", "entryIndex", (intptr_t)(&((idLBCache *)0)->entryIndex), sizeof( ((idLBCache *)0)->entryIndex ) },
	{ "int", "rowOffset", (intptr_t)(&((idLBCache *)0)->rowOffset), sizeof( ((idLBCache *)0)->rowOffset ) },
	{ "int", "localIndex", (intptr_t)(&((idLBCache *)0)->localIndex), sizeof( ((idLBCache *)0)->localIndex ) },
	{ "leaderboardDisplayError_t", "errorCode", (intptr_t)(&((idLBCache *)0)->errorCode), sizeof( ((idLBCache *)0)->errorCode ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_typeInfo[] = {
	{ ": idSWF *", "menuGUI", (intptr_t)(&((idMenuScreen *)0)->menuGUI), sizeof( ((idMenuScreen *)0)->menuGUI ) },
	{ "mainMenuTransition_t", "transition", (intptr_t)(&((idMenuScreen *)0)->transition), sizeof( ((idMenuScreen *)0)->transition ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_PDA_UserData_typeInfo[] = {
	{ ": idMenuWidget_PDA_UserData", "pdaUserData", (intptr_t)(&((idMenuScreen_PDA_UserData *)0)->pdaUserData), sizeof( ((idMenuScreen_PDA_UserData *)0)->pdaUserData ) },
	{ "idMenuWidget_PDA_Objective", "pdaObjectiveSimple", (intptr_t)(&((idMenuScreen_PDA_UserData *)0)->pdaObjectiveSimple), sizeof( ((idMenuScreen_PDA_UserData *)0)->pdaObjectiveSimple ) },
	{ "idMenuWidget_PDA_AudioFiles", "pdaAudioFiles", (intptr_t)(&((idMenuScreen_PDA_UserData *)0)->pdaAudioFiles), sizeof( ((idMenuScreen_PDA_UserData *)0)->pdaAudioFiles ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_PDA_UserEmails_typeInfo[] = {
	{ ": idMenuWidget_PDA_EmailInbox", "pdaInbox", (intptr_t)(&((idMenuScreen_PDA_UserEmails *)0)->pdaInbox), sizeof( ((idMenuScreen_PDA_UserEmails *)0)->pdaInbox ) },
	{ "idMenuWidget_InfoBox", "emailInfo", (intptr_t)(&((idMenuScreen_PDA_UserEmails *)0)->emailInfo), sizeof( ((idMenuScreen_PDA_UserEmails *)0)->emailInfo ) },
	{ "idMenuWidget_ScrollBar", "emailScrollbar", (intptr_t)(&((idMenuScreen_PDA_UserEmails *)0)->emailScrollbar), sizeof( ((idMenuScreen_PDA_UserEmails *)0)->emailScrollbar ) },
	{ "bool", "readingEmails", (intptr_t)(&((idMenuScreen_PDA_UserEmails *)0)->readingEmails), sizeof( ((idMenuScreen_PDA_UserEmails *)0)->readingEmails ) },
	{ "bool", "scrollEmailInfo", (intptr_t)(&((idMenuScreen_PDA_UserEmails *)0)->scrollEmailInfo), sizeof( ((idMenuScreen_PDA_UserEmails *)0)->scrollEmailInfo ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_PDA_VideoDisks_typeInfo[] = {
	{ ": idMenuWidget_ScrollBar", "scrollbar", (intptr_t)(&((idMenuScreen_PDA_VideoDisks *)0)->scrollbar), sizeof( ((idMenuScreen_PDA_VideoDisks *)0)->scrollbar ) },
	{ "idMenuWidget_DynamicList", "pdaVideoList", (intptr_t)(&((idMenuScreen_PDA_VideoDisks *)0)->pdaVideoList), sizeof( ((idMenuScreen_PDA_VideoDisks *)0)->pdaVideoList ) },
	{ "idMenuWidget_PDA_VideoInfo", "videoDetails", (intptr_t)(&((idMenuScreen_PDA_VideoDisks *)0)->videoDetails), sizeof( ((idMenuScreen_PDA_VideoDisks *)0)->videoDetails ) },
	{ "idList < idList < idStr , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "videoItems", (intptr_t)(&((idMenuScreen_PDA_VideoDisks *)0)->videoItems), sizeof( ((idMenuScreen_PDA_VideoDisks *)0)->videoItems ) },
	{ "const idDeclVideo *", "activeVideo", (intptr_t)(&((idMenuScreen_PDA_VideoDisks *)0)->activeVideo), sizeof( ((idMenuScreen_PDA_VideoDisks *)0)->activeVideo ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_PDA_Inventory_typeInfo[] = {
	{ ": idMenuWidget_Carousel", "itemList", (intptr_t)(&((idMenuScreen_PDA_Inventory *)0)->itemList), sizeof( ((idMenuScreen_PDA_Inventory *)0)->itemList ) },
	{ "idMenuWidget_InfoBox", "infoBox", (intptr_t)(&((idMenuScreen_PDA_Inventory *)0)->infoBox), sizeof( ((idMenuScreen_PDA_Inventory *)0)->infoBox ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Root_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Root *)0)->options), sizeof( ((idMenuScreen_Shell_Root *)0)->options ) },
	{ "idMenuWidget_Help *", "helpWidget", (intptr_t)(&((idMenuScreen_Shell_Root *)0)->helpWidget), sizeof( ((idMenuScreen_Shell_Root *)0)->helpWidget ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Pause_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Pause *)0)->options), sizeof( ((idMenuScreen_Shell_Pause *)0)->options ) },
	{ "bool", "isMpPause", (intptr_t)(&((idMenuScreen_Shell_Pause *)0)->isMpPause), sizeof( ((idMenuScreen_Shell_Pause *)0)->isMpPause ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Singleplayer_typeInfo[] = {
	{ ": bool", "canContinue", (intptr_t)(&((idMenuScreen_Shell_Singleplayer *)0)->canContinue), sizeof( ((idMenuScreen_Shell_Singleplayer *)0)->canContinue ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Singleplayer *)0)->options), sizeof( ((idMenuScreen_Shell_Singleplayer *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Singleplayer *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Singleplayer *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Settings_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Settings *)0)->options), sizeof( ((idMenuScreen_Shell_Settings *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Settings *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Settings *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t creditInfo_t_typeInfo[] = {
	{ "int", "type", (intptr_t)(&((creditInfo_t *)0)->type), sizeof( ((creditInfo_t *)0)->type ) },
	{ "idStr", "entry", (intptr_t)(&((creditInfo_t *)0)->entry), sizeof( ((creditInfo_t *)0)->entry ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Credits_typeInfo[] = {
	{ ": idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Credits *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Credits *)0)->btnBack ) },
	{ "idList < creditInfo_t >", "creditList", (intptr_t)(&((idMenuScreen_Shell_Credits *)0)->creditList), sizeof( ((idMenuScreen_Shell_Credits *)0)->creditList ) },
	{ "int", "creditIndex", (intptr_t)(&((idMenuScreen_Shell_Credits *)0)->creditIndex), sizeof( ((idMenuScreen_Shell_Credits *)0)->creditIndex ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Resolution_optionData_t_typeInfo[] = {
	{ "int", "fullscreen", (intptr_t)(&((idMenuScreen_Shell_Resolution::optionData_t *)0)->fullscreen), sizeof( ((idMenuScreen_Shell_Resolution::optionData_t *)0)->fullscreen ) },
	{ "int", "vidmode", (intptr_t)(&((idMenuScreen_Shell_Resolution::optionData_t *)0)->vidmode), sizeof( ((idMenuScreen_Shell_Resolution::optionData_t *)0)->vidmode ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Resolution_typeInfo[] = {
	{ "idList < optionData_t >", "optionData", (intptr_t)(&((idMenuScreen_Shell_Resolution *)0)->optionData), sizeof( ((idMenuScreen_Shell_Resolution *)0)->optionData ) },
	{ "optionData_t", "originalOption", (intptr_t)(&((idMenuScreen_Shell_Resolution *)0)->originalOption), sizeof( ((idMenuScreen_Shell_Resolution *)0)->originalOption ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Resolution *)0)->options), sizeof( ((idMenuScreen_Shell_Resolution *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Resolution *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Resolution *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Difficulty_typeInfo[] = {
	{ ": bool", "nightmareUnlocked", (intptr_t)(&((idMenuScreen_Shell_Difficulty *)0)->nightmareUnlocked), sizeof( ((idMenuScreen_Shell_Difficulty *)0)->nightmareUnlocked ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Difficulty *)0)->options), sizeof( ((idMenuScreen_Shell_Difficulty *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Difficulty *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Difficulty *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Playstation_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Playstation *)0)->options), sizeof( ((idMenuScreen_Shell_Playstation *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Playstation *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Playstation *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_ModeSelect_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_ModeSelect *)0)->options), sizeof( ((idMenuScreen_Shell_ModeSelect *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_ModeSelect *)0)->btnBack), sizeof( ((idMenuScreen_Shell_ModeSelect *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_GameBrowser_typeInfo[] = {
	{ ": idMenuWidget_GameBrowserList *", "listWidget", (intptr_t)(&((idMenuScreen_Shell_GameBrowser *)0)->listWidget), sizeof( ((idMenuScreen_Shell_GameBrowser *)0)->listWidget ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_GameBrowser *)0)->btnBack), sizeof( ((idMenuScreen_Shell_GameBrowser *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Leaderboards_doomLeaderboard_t_typeInfo[] = {
	{ "const leaderboardDefinition_t *", "lb", (intptr_t)(&((idMenuScreen_Shell_Leaderboards::doomLeaderboard_t *)0)->lb), sizeof( ((idMenuScreen_Shell_Leaderboards::doomLeaderboard_t *)0)->lb ) },
	{ "idStr", "name", (intptr_t)(&((idMenuScreen_Shell_Leaderboards::doomLeaderboard_t *)0)->name), sizeof( ((idMenuScreen_Shell_Leaderboards::doomLeaderboard_t *)0)->name ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Leaderboards_typeInfo[] = {
	{ "idList < doomLeaderboard_t >", "leaderboards", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->leaderboards), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->leaderboards ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->options), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->btnBack ) },
	{ "idMenuWidget_Button *", "btnPrev", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->btnPrev), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->btnPrev ) },
	{ "idMenuWidget_Button *", "btnNext", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->btnNext), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->btnNext ) },
	{ "idMenuWidget_Button *", "btnPageDwn", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->btnPageDwn), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->btnPageDwn ) },
	{ "idMenuWidget_Button *", "btnPageUp", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->btnPageUp), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->btnPageUp ) },
	{ "idLBCache *", "lbCache", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->lbCache), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->lbCache ) },
	{ "idSWFTextInstance *", "lbHeading", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->lbHeading), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->lbHeading ) },
	{ "int", "lbIndex", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->lbIndex), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->lbIndex ) },
	{ "bool", "refreshLeaderboard", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->refreshLeaderboard), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->refreshLeaderboard ) },
	{ "bool", "refreshWhenMasterIsOnline", (intptr_t)(&((idMenuScreen_Shell_Leaderboards *)0)->refreshWhenMasterIsOnline), sizeof( ((idMenuScreen_Shell_Leaderboards *)0)->refreshWhenMasterIsOnline ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Bindings_typeInfo[] = {
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Bindings *)0)->options), sizeof( ((idMenuScreen_Shell_Bindings *)0)->options ) },
	{ "idMenuWidget_Button *", "restoreDefault", (intptr_t)(&((idMenuScreen_Shell_Bindings *)0)->restoreDefault), sizeof( ((idMenuScreen_Shell_Bindings *)0)->restoreDefault ) },
	{ "idSWFSpriteInstance *", "blinder", (intptr_t)(&((idMenuScreen_Shell_Bindings *)0)->blinder), sizeof( ((idMenuScreen_Shell_Bindings *)0)->blinder ) },
	{ "idSWFSpriteInstance *", "txtBlinder", (intptr_t)(&((idMenuScreen_Shell_Bindings *)0)->txtBlinder), sizeof( ((idMenuScreen_Shell_Bindings *)0)->txtBlinder ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Bindings *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Bindings *)0)->btnBack ) },
	{ "bool", "bindingsChanged", (intptr_t)(&((idMenuScreen_Shell_Bindings *)0)->bindingsChanged), sizeof( ((idMenuScreen_Shell_Bindings *)0)->bindingsChanged ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Dev_devOption_t_typeInfo[] = {
	{ "const char *", "map", (intptr_t)(&((idMenuScreen_Shell_Dev::devOption_t *)0)->map), sizeof( ((idMenuScreen_Shell_Dev::devOption_t *)0)->map ) },
	{ "const char *", "name", (intptr_t)(&((idMenuScreen_Shell_Dev::devOption_t *)0)->name), sizeof( ((idMenuScreen_Shell_Dev::devOption_t *)0)->name ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Dev_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Dev *)0)->options), sizeof( ((idMenuScreen_Shell_Dev *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Dev *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Dev *)0)->btnBack ) },
	{ "idList < devOption_t , TAG_IDLIB_LIST_MENU >", "devOptions", (intptr_t)(&((idMenuScreen_Shell_Dev *)0)->devOptions), sizeof( ((idMenuScreen_Shell_Dev *)0)->devOptions ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_NewGame_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_NewGame *)0)->options), sizeof( ((idMenuScreen_Shell_NewGame *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_NewGame *)0)->btnBack), sizeof( ((idMenuScreen_Shell_NewGame *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Load_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Load *)0)->options), sizeof( ((idMenuScreen_Shell_Load *)0)->options ) },
	{ "idMenuWidget_Shell_SaveInfo *", "saveInfo", (intptr_t)(&((idMenuScreen_Shell_Load *)0)->saveInfo), sizeof( ((idMenuScreen_Shell_Load *)0)->saveInfo ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Load *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Load *)0)->btnBack ) },
	{ "idMenuWidget_Button *", "btnDelete", (intptr_t)(&((idMenuScreen_Shell_Load *)0)->btnDelete), sizeof( ((idMenuScreen_Shell_Load *)0)->btnDelete ) },
	{ "saveGameDetailsList_t", "sortedSaves", (intptr_t)(&((idMenuScreen_Shell_Load *)0)->sortedSaves), sizeof( ((idMenuScreen_Shell_Load *)0)->sortedSaves ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Save_typeInfo[] = {
	{ ": idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Save *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Save *)0)->btnBack ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Save *)0)->options), sizeof( ((idMenuScreen_Shell_Save *)0)->options ) },
	{ "idMenuWidget_Shell_SaveInfo *", "saveInfo", (intptr_t)(&((idMenuScreen_Shell_Save *)0)->saveInfo), sizeof( ((idMenuScreen_Shell_Save *)0)->saveInfo ) },
	{ "idMenuWidget_Button *", "btnDelete", (intptr_t)(&((idMenuScreen_Shell_Save *)0)->btnDelete), sizeof( ((idMenuScreen_Shell_Save *)0)->btnDelete ) },
	{ "saveGameDetailsList_t", "sortedSaves", (intptr_t)(&((idMenuScreen_Shell_Save *)0)->sortedSaves), sizeof( ((idMenuScreen_Shell_Save *)0)->sortedSaves ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_GameOptions_idMenuDataSource_GameSettings_typeInfo[] = {
	{ ": idStaticList < idSWFScriptVar , MAX_GAME_FIELDS >", "fields", (intptr_t)(&((idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings *)0)->fields), sizeof( ((idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings *)0)->fields ) },
	{ "idStaticList < idSWFScriptVar , MAX_GAME_FIELDS >", "originalFields", (intptr_t)(&((idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings *)0)->originalFields), sizeof( ((idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings *)0)->originalFields ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_GameOptions_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_GameOptions *)0)->options), sizeof( ((idMenuScreen_Shell_GameOptions *)0)->options ) },
	{ "idMenuDataSource_GameSettings", "systemData", (intptr_t)(&((idMenuScreen_Shell_GameOptions *)0)->systemData), sizeof( ((idMenuScreen_Shell_GameOptions *)0)->systemData ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_GameOptions *)0)->btnBack), sizeof( ((idMenuScreen_Shell_GameOptions *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_MatchSettings_idMenuDataSource_MatchSettings_typeInfo[] = {
	{ "idStaticList < idSWFScriptVar , MAX_MATCH_FIELDS >", "fields", (intptr_t)(&((idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings *)0)->fields), sizeof( ((idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings *)0)->fields ) },
	{ "idStaticList < idSWFScriptVar , MAX_MATCH_FIELDS >", "originalFields", (intptr_t)(&((idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings *)0)->originalFields), sizeof( ((idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings *)0)->originalFields ) },
	{ "bool", "updateMap", (intptr_t)(&((idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings *)0)->updateMap), sizeof( ((idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings *)0)->updateMap ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_MatchSettings_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_MatchSettings *)0)->options), sizeof( ((idMenuScreen_Shell_MatchSettings *)0)->options ) },
	{ "idMenuDataSource_MatchSettings", "matchData", (intptr_t)(&((idMenuScreen_Shell_MatchSettings *)0)->matchData), sizeof( ((idMenuScreen_Shell_MatchSettings *)0)->matchData ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_MatchSettings *)0)->btnBack), sizeof( ((idMenuScreen_Shell_MatchSettings *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Controls_idMenuDataSource_ControlSettings_typeInfo[] = {
	{ ": idStaticList < idSWFScriptVar , MAX_CONTROL_FIELDS >", "fields", (intptr_t)(&((idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings *)0)->fields), sizeof( ((idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings *)0)->fields ) },
	{ "idStaticList < idSWFScriptVar , MAX_CONTROL_FIELDS >", "originalFields", (intptr_t)(&((idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings *)0)->originalFields), sizeof( ((idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings *)0)->originalFields ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Controls_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Controls *)0)->options), sizeof( ((idMenuScreen_Shell_Controls *)0)->options ) },
	{ "idMenuDataSource_ControlSettings", "controlData", (intptr_t)(&((idMenuScreen_Shell_Controls *)0)->controlData), sizeof( ((idMenuScreen_Shell_Controls *)0)->controlData ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Controls *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Controls *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Gamepad_idMenuDataSource_GamepadSettings_typeInfo[] = {
	{ ": idStaticList < idSWFScriptVar , MAX_GAMEPAD_FIELDS >", "fields", (intptr_t)(&((idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings *)0)->fields), sizeof( ((idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings *)0)->fields ) },
	{ "idStaticList < idSWFScriptVar , MAX_GAMEPAD_FIELDS >", "originalFields", (intptr_t)(&((idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings *)0)->originalFields), sizeof( ((idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings *)0)->originalFields ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_Gamepad_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_Gamepad *)0)->options), sizeof( ((idMenuScreen_Shell_Gamepad *)0)->options ) },
	{ "idMenuDataSource_GamepadSettings", "gamepadData", (intptr_t)(&((idMenuScreen_Shell_Gamepad *)0)->gamepadData), sizeof( ((idMenuScreen_Shell_Gamepad *)0)->gamepadData ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_Gamepad *)0)->btnBack), sizeof( ((idMenuScreen_Shell_Gamepad *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_ControllerLayout_idMenuDataSource_LayoutSettings_typeInfo[] = {
	{ ": idStaticList < idSWFScriptVar , MAX_LAYOUT_FIELDS >", "fields", (intptr_t)(&((idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings *)0)->fields), sizeof( ((idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings *)0)->fields ) },
	{ "idStaticList < idSWFScriptVar , MAX_LAYOUT_FIELDS >", "originalFields", (intptr_t)(&((idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings *)0)->originalFields), sizeof( ((idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings *)0)->originalFields ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_ControllerLayout_typeInfo[] = {
	{ ": idMenuDataSource_LayoutSettings", "layoutData", (intptr_t)(&((idMenuScreen_Shell_ControllerLayout *)0)->layoutData), sizeof( ((idMenuScreen_Shell_ControllerLayout *)0)->layoutData ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_ControllerLayout *)0)->options), sizeof( ((idMenuScreen_Shell_ControllerLayout *)0)->options ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_ControllerLayout *)0)->btnBack), sizeof( ((idMenuScreen_Shell_ControllerLayout *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_SystemOptions_idMenuDataSource_SystemSettings_typeInfo[] = {
	{ ": idStr", "originalRenderAPI", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalRenderAPI), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalRenderAPI ) },
	{ "int", "originalFramerate", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalFramerate), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalFramerate ) },
	{ "int", "originalAntialias", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalAntialias), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalAntialias ) },
	{ "int", "originalVsync", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalVsync), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalVsync ) },
	{ "float", "originalBrightness", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalBrightness), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalBrightness ) },
	{ "float", "originalVolume", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalVolume), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalVolume ) },
	{ "int", "originalRenderMode", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalRenderMode), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalRenderMode ) },
	{ "float", "originalAmbientBrightness", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalAmbientBrightness), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalAmbientBrightness ) },
	{ "int", "originalSSAO", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalSSAO), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalSSAO ) },
	{ "int", "originalBloodReflections", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalBloodReflections), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalBloodReflections ) },
	{ "int", "originalPostProcessing", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalPostProcessing), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalPostProcessing ) },
	{ "int", "originalCRTPostFX", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalCRTPostFX), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->originalCRTPostFX ) },
	{ "idList < vidMode_t >", "modeList", (intptr_t)(&((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->modeList), sizeof( ((idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings *)0)->modeList ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_SystemOptions_typeInfo[] = {
	{ ": idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_SystemOptions *)0)->options), sizeof( ((idMenuScreen_Shell_SystemOptions *)0)->options ) },
	{ "idMenuDataSource_SystemSettings", "systemData", (intptr_t)(&((idMenuScreen_Shell_SystemOptions *)0)->systemData), sizeof( ((idMenuScreen_Shell_SystemOptions *)0)->systemData ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_SystemOptions *)0)->btnBack), sizeof( ((idMenuScreen_Shell_SystemOptions *)0)->btnBack ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_PartyLobby_typeInfo[] = {
	{ ": bool", "isHost", (intptr_t)(&((idMenuScreen_Shell_PartyLobby *)0)->isHost), sizeof( ((idMenuScreen_Shell_PartyLobby *)0)->isHost ) },
	{ "bool", "isPeer", (intptr_t)(&((idMenuScreen_Shell_PartyLobby *)0)->isPeer), sizeof( ((idMenuScreen_Shell_PartyLobby *)0)->isPeer ) },
	{ "bool", "inParty", (intptr_t)(&((idMenuScreen_Shell_PartyLobby *)0)->inParty), sizeof( ((idMenuScreen_Shell_PartyLobby *)0)->inParty ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_PartyLobby *)0)->options), sizeof( ((idMenuScreen_Shell_PartyLobby *)0)->options ) },
	{ "idMenuWidget_LobbyList *", "lobby", (intptr_t)(&((idMenuScreen_Shell_PartyLobby *)0)->lobby), sizeof( ((idMenuScreen_Shell_PartyLobby *)0)->lobby ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_PartyLobby *)0)->btnBack), sizeof( ((idMenuScreen_Shell_PartyLobby *)0)->btnBack ) },
	{ "idList < idList < idStr , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "menuOptions", (intptr_t)(&((idMenuScreen_Shell_PartyLobby *)0)->menuOptions), sizeof( ((idMenuScreen_Shell_PartyLobby *)0)->menuOptions ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Shell_GameLobby_typeInfo[] = {
	{ ": int", "longCountdown", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->longCountdown), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->longCountdown ) },
	{ "int", "longCountRemaining", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->longCountRemaining), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->longCountRemaining ) },
	{ "int", "shortCountdown", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->shortCountdown), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->shortCountdown ) },
	{ "bool", "isHost", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->isHost), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->isHost ) },
	{ "bool", "isPeer", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->isPeer), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->isPeer ) },
	{ "bool", "privateGameLobby", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->privateGameLobby), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->privateGameLobby ) },
	{ "idMenuWidget_DynamicList *", "options", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->options), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->options ) },
	{ "idMenuWidget_LobbyList *", "lobby", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->lobby), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->lobby ) },
	{ "idMenuWidget_Button *", "btnBack", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->btnBack), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->btnBack ) },
	{ "idList < idList < idStr , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "menuOptions", (intptr_t)(&((idMenuScreen_Shell_GameLobby *)0)->menuOptions), sizeof( ((idMenuScreen_Shell_GameLobby *)0)->menuOptions ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_HUD_typeInfo[] = {
	{ ": idSWFScriptObject *", "weaponInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->weaponInfo), sizeof( ((idMenuScreen_HUD *)0)->weaponInfo ) },
	{ "idSWFScriptObject *", "playerInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->playerInfo), sizeof( ((idMenuScreen_HUD *)0)->playerInfo ) },
	{ "idSWFScriptObject *", "stamina", (intptr_t)(&((idMenuScreen_HUD *)0)->stamina), sizeof( ((idMenuScreen_HUD *)0)->stamina ) },
	{ "idSWFScriptObject *", "weaponName", (intptr_t)(&((idMenuScreen_HUD *)0)->weaponName), sizeof( ((idMenuScreen_HUD *)0)->weaponName ) },
	{ "idSWFScriptObject *", "weaponPills", (intptr_t)(&((idMenuScreen_HUD *)0)->weaponPills), sizeof( ((idMenuScreen_HUD *)0)->weaponPills ) },
	{ "idSWFScriptObject *", "downloadPda", (intptr_t)(&((idMenuScreen_HUD *)0)->downloadPda), sizeof( ((idMenuScreen_HUD *)0)->downloadPda ) },
	{ "idSWFScriptObject *", "downloadVideo", (intptr_t)(&((idMenuScreen_HUD *)0)->downloadVideo), sizeof( ((idMenuScreen_HUD *)0)->downloadVideo ) },
	{ "idSWFScriptObject *", "tipInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->tipInfo), sizeof( ((idMenuScreen_HUD *)0)->tipInfo ) },
	{ "idSWFScriptObject *", "mpChat", (intptr_t)(&((idMenuScreen_HUD *)0)->mpChat), sizeof( ((idMenuScreen_HUD *)0)->mpChat ) },
	{ "idSWFScriptObject *", "mpWeapons", (intptr_t)(&((idMenuScreen_HUD *)0)->mpWeapons), sizeof( ((idMenuScreen_HUD *)0)->mpWeapons ) },
	{ "idSWFSpriteInstance *", "healthBorder", (intptr_t)(&((idMenuScreen_HUD *)0)->healthBorder), sizeof( ((idMenuScreen_HUD *)0)->healthBorder ) },
	{ "idSWFSpriteInstance *", "healthPulse", (intptr_t)(&((idMenuScreen_HUD *)0)->healthPulse), sizeof( ((idMenuScreen_HUD *)0)->healthPulse ) },
	{ "idSWFSpriteInstance *", "armorFrame", (intptr_t)(&((idMenuScreen_HUD *)0)->armorFrame), sizeof( ((idMenuScreen_HUD *)0)->armorFrame ) },
	{ "idSWFSpriteInstance *", "security", (intptr_t)(&((idMenuScreen_HUD *)0)->security), sizeof( ((idMenuScreen_HUD *)0)->security ) },
	{ "idSWFSpriteInstance *", "newPDADownload", (intptr_t)(&((idMenuScreen_HUD *)0)->newPDADownload), sizeof( ((idMenuScreen_HUD *)0)->newPDADownload ) },
	{ "idSWFSpriteInstance *", "newVideoDownload", (intptr_t)(&((idMenuScreen_HUD *)0)->newVideoDownload), sizeof( ((idMenuScreen_HUD *)0)->newVideoDownload ) },
	{ "idSWFSpriteInstance *", "newPDA", (intptr_t)(&((idMenuScreen_HUD *)0)->newPDA), sizeof( ((idMenuScreen_HUD *)0)->newPDA ) },
	{ "idSWFSpriteInstance *", "newVideo", (intptr_t)(&((idMenuScreen_HUD *)0)->newVideo), sizeof( ((idMenuScreen_HUD *)0)->newVideo ) },
	{ "idSWFSpriteInstance *", "audioLog", (intptr_t)(&((idMenuScreen_HUD *)0)->audioLog), sizeof( ((idMenuScreen_HUD *)0)->audioLog ) },
	{ "idSWFSpriteInstance *", "communication", (intptr_t)(&((idMenuScreen_HUD *)0)->communication), sizeof( ((idMenuScreen_HUD *)0)->communication ) },
	{ "idSWFSpriteInstance *", "oxygen", (intptr_t)(&((idMenuScreen_HUD *)0)->oxygen), sizeof( ((idMenuScreen_HUD *)0)->oxygen ) },
	{ "idSWFSpriteInstance *", "objective", (intptr_t)(&((idMenuScreen_HUD *)0)->objective), sizeof( ((idMenuScreen_HUD *)0)->objective ) },
	{ "idSWFSpriteInstance *", "objectiveComplete", (intptr_t)(&((idMenuScreen_HUD *)0)->objectiveComplete), sizeof( ((idMenuScreen_HUD *)0)->objectiveComplete ) },
	{ "idSWFSpriteInstance *", "ammoInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->ammoInfo), sizeof( ((idMenuScreen_HUD *)0)->ammoInfo ) },
	{ "idSWFSpriteInstance *", "weaponImg", (intptr_t)(&((idMenuScreen_HUD *)0)->weaponImg), sizeof( ((idMenuScreen_HUD *)0)->weaponImg ) },
	{ "idSWFSpriteInstance *", "newWeapon", (intptr_t)(&((idMenuScreen_HUD *)0)->newWeapon), sizeof( ((idMenuScreen_HUD *)0)->newWeapon ) },
	{ "idSWFSpriteInstance *", "pickupInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->pickupInfo), sizeof( ((idMenuScreen_HUD *)0)->pickupInfo ) },
	{ "idSWFSpriteInstance *", "talkCursor", (intptr_t)(&((idMenuScreen_HUD *)0)->talkCursor), sizeof( ((idMenuScreen_HUD *)0)->talkCursor ) },
	{ "idSWFSpriteInstance *", "combatCursor", (intptr_t)(&((idMenuScreen_HUD *)0)->combatCursor), sizeof( ((idMenuScreen_HUD *)0)->combatCursor ) },
	{ "idSWFSpriteInstance *", "grabberCursor", (intptr_t)(&((idMenuScreen_HUD *)0)->grabberCursor), sizeof( ((idMenuScreen_HUD *)0)->grabberCursor ) },
	{ "idSWFSpriteInstance *", "bsInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->bsInfo), sizeof( ((idMenuScreen_HUD *)0)->bsInfo ) },
	{ "idSWFSpriteInstance *", "soulcubeInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->soulcubeInfo), sizeof( ((idMenuScreen_HUD *)0)->soulcubeInfo ) },
	{ "idSWFSpriteInstance *", "newItem", (intptr_t)(&((idMenuScreen_HUD *)0)->newItem), sizeof( ((idMenuScreen_HUD *)0)->newItem ) },
	{ "idSWFSpriteInstance *", "respawnMessage", (intptr_t)(&((idMenuScreen_HUD *)0)->respawnMessage), sizeof( ((idMenuScreen_HUD *)0)->respawnMessage ) },
	{ "idSWFSpriteInstance *", "flashlight", (intptr_t)(&((idMenuScreen_HUD *)0)->flashlight), sizeof( ((idMenuScreen_HUD *)0)->flashlight ) },
	{ "idSWFSpriteInstance *", "mpChatObject", (intptr_t)(&((idMenuScreen_HUD *)0)->mpChatObject), sizeof( ((idMenuScreen_HUD *)0)->mpChatObject ) },
	{ "idSWFSpriteInstance *", "mpConnection", (intptr_t)(&((idMenuScreen_HUD *)0)->mpConnection), sizeof( ((idMenuScreen_HUD *)0)->mpConnection ) },
	{ "idSWFSpriteInstance *", "mpInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->mpInfo), sizeof( ((idMenuScreen_HUD *)0)->mpInfo ) },
	{ "idSWFSpriteInstance *", "mpHitInfo", (intptr_t)(&((idMenuScreen_HUD *)0)->mpHitInfo), sizeof( ((idMenuScreen_HUD *)0)->mpHitInfo ) },
	{ "idSWFTextInstance *", "locationName", (intptr_t)(&((idMenuScreen_HUD *)0)->locationName), sizeof( ((idMenuScreen_HUD *)0)->locationName ) },
	{ "idSWFTextInstance *", "securityText", (intptr_t)(&((idMenuScreen_HUD *)0)->securityText), sizeof( ((idMenuScreen_HUD *)0)->securityText ) },
	{ "idSWFTextInstance *", "newPDAName", (intptr_t)(&((idMenuScreen_HUD *)0)->newPDAName), sizeof( ((idMenuScreen_HUD *)0)->newPDAName ) },
	{ "idSWFTextInstance *", "newPDAHeading", (intptr_t)(&((idMenuScreen_HUD *)0)->newPDAHeading), sizeof( ((idMenuScreen_HUD *)0)->newPDAHeading ) },
	{ "idSWFTextInstance *", "newVideoHeading", (intptr_t)(&((idMenuScreen_HUD *)0)->newVideoHeading), sizeof( ((idMenuScreen_HUD *)0)->newVideoHeading ) },
	{ "idSWFTextInstance *", "mpMessage", (intptr_t)(&((idMenuScreen_HUD *)0)->mpMessage), sizeof( ((idMenuScreen_HUD *)0)->mpMessage ) },
	{ "idSWFTextInstance *", "mpTime", (intptr_t)(&((idMenuScreen_HUD *)0)->mpTime), sizeof( ((idMenuScreen_HUD *)0)->mpTime ) },
	{ "int", "audioLogPrevTime", (intptr_t)(&((idMenuScreen_HUD *)0)->audioLogPrevTime), sizeof( ((idMenuScreen_HUD *)0)->audioLogPrevTime ) },
	{ "int", "commPrevTime", (intptr_t)(&((idMenuScreen_HUD *)0)->commPrevTime), sizeof( ((idMenuScreen_HUD *)0)->commPrevTime ) },
	{ "bool", "oxygenComm", (intptr_t)(&((idMenuScreen_HUD *)0)->oxygenComm), sizeof( ((idMenuScreen_HUD *)0)->oxygenComm ) },
	{ "bool", "inVaccuum", (intptr_t)(&((idMenuScreen_HUD *)0)->inVaccuum), sizeof( ((idMenuScreen_HUD *)0)->inVaccuum ) },
	{ "idStr", "objTitle", (intptr_t)(&((idMenuScreen_HUD *)0)->objTitle), sizeof( ((idMenuScreen_HUD *)0)->objTitle ) },
	{ "idStr", "objDesc", (intptr_t)(&((idMenuScreen_HUD *)0)->objDesc), sizeof( ((idMenuScreen_HUD *)0)->objDesc ) },
	{ "const idMaterial *", "objScreenshot", (intptr_t)(&((idMenuScreen_HUD *)0)->objScreenshot), sizeof( ((idMenuScreen_HUD *)0)->objScreenshot ) },
	{ "idStr", "objCompleteTitle", (intptr_t)(&((idMenuScreen_HUD *)0)->objCompleteTitle), sizeof( ((idMenuScreen_HUD *)0)->objCompleteTitle ) },
	{ "cursorState_t", "cursorState", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorState), sizeof( ((idMenuScreen_HUD *)0)->cursorState ) },
	{ "int", "cursorInCombat", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorInCombat), sizeof( ((idMenuScreen_HUD *)0)->cursorInCombat ) },
	{ "int", "cursorTalking", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorTalking), sizeof( ((idMenuScreen_HUD *)0)->cursorTalking ) },
	{ "int", "cursorItem", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorItem), sizeof( ((idMenuScreen_HUD *)0)->cursorItem ) },
	{ "int", "cursorGrabber", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorGrabber), sizeof( ((idMenuScreen_HUD *)0)->cursorGrabber ) },
	{ "int", "cursorNone", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorNone), sizeof( ((idMenuScreen_HUD *)0)->cursorNone ) },
	{ "idStr", "cursorAction", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorAction), sizeof( ((idMenuScreen_HUD *)0)->cursorAction ) },
	{ "idStr", "cursorFocus", (intptr_t)(&((idMenuScreen_HUD *)0)->cursorFocus), sizeof( ((idMenuScreen_HUD *)0)->cursorFocus ) },
	{ "bool", "showSoulCubeInfoOnLoad", (intptr_t)(&((idMenuScreen_HUD *)0)->showSoulCubeInfoOnLoad), sizeof( ((idMenuScreen_HUD *)0)->showSoulCubeInfoOnLoad ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Scoreboard_typeInfo[] = {
	{ ": idMenuWidget_ScoreboardList *", "playerList", (intptr_t)(&((idMenuScreen_Scoreboard *)0)->playerList), sizeof( ((idMenuScreen_Scoreboard *)0)->playerList ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Scoreboard_CTF_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t idMenuScreen_Scoreboard_Team_typeInfo[] = {
	{ NULL, 0 }
};

static classVariableInfo_t actionRepeater_t_typeInfo[] = {
	{ "idMenuWidget *", "widget", (intptr_t)(&((actionRepeater_t *)0)->widget), sizeof( ((actionRepeater_t *)0)->widget ) },
	{ "idWidgetEvent", "event", (intptr_t)(&((actionRepeater_t *)0)->event), sizeof( ((actionRepeater_t *)0)->event ) },
	{ "idWidgetAction", "action", (intptr_t)(&((actionRepeater_t *)0)->action), sizeof( ((actionRepeater_t *)0)->action ) },
	{ "int", "numRepetitions", (intptr_t)(&((actionRepeater_t *)0)->numRepetitions), sizeof( ((actionRepeater_t *)0)->numRepetitions ) },
	{ "int", "nextRepeatTime", (intptr_t)(&((actionRepeater_t *)0)->nextRepeatTime), sizeof( ((actionRepeater_t *)0)->nextRepeatTime ) },
	{ "int", "repeatDelay", (intptr_t)(&((actionRepeater_t *)0)->repeatDelay), sizeof( ((actionRepeater_t *)0)->repeatDelay ) },
	{ "int", "screenIndex", (intptr_t)(&((actionRepeater_t *)0)->screenIndex), sizeof( ((actionRepeater_t *)0)->screenIndex ) },
	{ "bool", "isActive", (intptr_t)(&((actionRepeater_t *)0)->isActive), sizeof( ((actionRepeater_t *)0)->isActive ) },
	{ NULL, 0 }
};

static classVariableInfo_t mpScoreboardInfo_typeInfo[] = {
	{ "voiceStateDisplay_t", "voiceState", (intptr_t)(&((mpScoreboardInfo *)0)->voiceState), sizeof( ((mpScoreboardInfo *)0)->voiceState ) },
	{ "int", "score", (intptr_t)(&((mpScoreboardInfo *)0)->score), sizeof( ((mpScoreboardInfo *)0)->score ) },
	{ "int", "wins", (intptr_t)(&((mpScoreboardInfo *)0)->wins), sizeof( ((mpScoreboardInfo *)0)->wins ) },
	{ "int", "ping", (intptr_t)(&((mpScoreboardInfo *)0)->ping), sizeof( ((mpScoreboardInfo *)0)->ping ) },
	{ "int", "team", (intptr_t)(&((mpScoreboardInfo *)0)->team), sizeof( ((mpScoreboardInfo *)0)->team ) },
	{ "int", "playerNum", (intptr_t)(&((mpScoreboardInfo *)0)->playerNum), sizeof( ((mpScoreboardInfo *)0)->playerNum ) },
	{ "idStr", "spectateData", (intptr_t)(&((mpScoreboardInfo *)0)->spectateData), sizeof( ((mpScoreboardInfo *)0)->spectateData ) },
	{ "idStr", "name", (intptr_t)(&((mpScoreboardInfo *)0)->name), sizeof( ((mpScoreboardInfo *)0)->name ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuHandler_typeInfo[] = {
	{ ": bool", "scrollingMenu", (intptr_t)(&((idMenuHandler *)0)->scrollingMenu), sizeof( ((idMenuHandler *)0)->scrollingMenu ) },
	{ "int", "scrollCounter", (intptr_t)(&((idMenuHandler *)0)->scrollCounter), sizeof( ((idMenuHandler *)0)->scrollCounter ) },
	{ "int", "activeScreen", (intptr_t)(&((idMenuHandler *)0)->activeScreen), sizeof( ((idMenuHandler *)0)->activeScreen ) },
	{ "int", "nextScreen", (intptr_t)(&((idMenuHandler *)0)->nextScreen), sizeof( ((idMenuHandler *)0)->nextScreen ) },
	{ "int", "transition", (intptr_t)(&((idMenuHandler *)0)->transition), sizeof( ((idMenuHandler *)0)->transition ) },
	{ "int", "platform", (intptr_t)(&((idMenuHandler *)0)->platform), sizeof( ((idMenuHandler *)0)->platform ) },
	{ "idSWF *", "gui", (intptr_t)(&((idMenuHandler *)0)->gui), sizeof( ((idMenuHandler *)0)->gui ) },
	{ "actionRepeater_t", "actionRepeater", (intptr_t)(&((idMenuHandler *)0)->actionRepeater), sizeof( ((idMenuHandler *)0)->actionRepeater ) },
	{ "idMenuScreen *[32]", "menuScreens", (intptr_t)(&((idMenuHandler *)0)->menuScreens), sizeof( ((idMenuHandler *)0)->menuScreens ) },
	{ "idList < idMenuWidget * , TAG_IDLIB_LIST_MENU >", "children", (intptr_t)(&((idMenuHandler *)0)->children), sizeof( ((idMenuHandler *)0)->children ) },
	{ "idStaticList < idStr , NUM_GUI_SOUNDS >", "sounds", (intptr_t)(&((idMenuHandler *)0)->sounds), sizeof( ((idMenuHandler *)0)->sounds ) },
	{ "idMenuWidget_CommandBar *", "cmdBar", (intptr_t)(&((idMenuHandler *)0)->cmdBar), sizeof( ((idMenuHandler *)0)->cmdBar ) },
	{ NULL, 0 }
};

static classVariableInfo_t lobbyPlayerInfo_t_typeInfo[] = {
	{ "idStr", "name", (intptr_t)(&((lobbyPlayerInfo_t *)0)->name), sizeof( ((lobbyPlayerInfo_t *)0)->name ) },
	{ "int", "partyToken", (intptr_t)(&((lobbyPlayerInfo_t *)0)->partyToken), sizeof( ((lobbyPlayerInfo_t *)0)->partyToken ) },
	{ "voiceStateDisplay_t", "voiceState", (intptr_t)(&((lobbyPlayerInfo_t *)0)->voiceState), sizeof( ((lobbyPlayerInfo_t *)0)->voiceState ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuHandler_Shell_typeInfo[] = {
	{ ": shellState_t", "state", (intptr_t)(&((idMenuHandler_Shell *)0)->state), sizeof( ((idMenuHandler_Shell *)0)->state ) },
	{ "shellState_t", "nextState", (intptr_t)(&((idMenuHandler_Shell *)0)->nextState), sizeof( ((idMenuHandler_Shell *)0)->nextState ) },
	{ "bool", "smallFrameShowing", (intptr_t)(&((idMenuHandler_Shell *)0)->smallFrameShowing), sizeof( ((idMenuHandler_Shell *)0)->smallFrameShowing ) },
	{ "bool", "largeFrameShowing", (intptr_t)(&((idMenuHandler_Shell *)0)->largeFrameShowing), sizeof( ((idMenuHandler_Shell *)0)->largeFrameShowing ) },
	{ "bool", "bgShowing", (intptr_t)(&((idMenuHandler_Shell *)0)->bgShowing), sizeof( ((idMenuHandler_Shell *)0)->bgShowing ) },
	{ "bool", "waitForBinding", (intptr_t)(&((idMenuHandler_Shell *)0)->waitForBinding), sizeof( ((idMenuHandler_Shell *)0)->waitForBinding ) },
	{ "const char *", "waitBind", (intptr_t)(&((idMenuHandler_Shell *)0)->waitBind), sizeof( ((idMenuHandler_Shell *)0)->waitBind ) },
	{ "idList < const char * , TAG_IDLIB_LIST_MENU >", "mpGameModes", (intptr_t)(&((idMenuHandler_Shell *)0)->mpGameModes), sizeof( ((idMenuHandler_Shell *)0)->mpGameModes ) },
	{ "idList < mpMap_t , TAG_IDLIB_LIST_MENU >", "mpGameMaps", (intptr_t)(&((idMenuHandler_Shell *)0)->mpGameMaps), sizeof( ((idMenuHandler_Shell *)0)->mpGameMaps ) },
	{ "idMenuWidget_MenuBar *", "menuBar", (intptr_t)(&((idMenuHandler_Shell *)0)->menuBar), sizeof( ((idMenuHandler_Shell *)0)->menuBar ) },
	{ "idMenuWidget *", "pacifier", (intptr_t)(&((idMenuHandler_Shell *)0)->pacifier), sizeof( ((idMenuHandler_Shell *)0)->pacifier ) },
	{ "int", "timeRemaining", (intptr_t)(&((idMenuHandler_Shell *)0)->timeRemaining), sizeof( ((idMenuHandler_Shell *)0)->timeRemaining ) },
	{ "int", "nextPeerUpdateMs", (intptr_t)(&((idMenuHandler_Shell *)0)->nextPeerUpdateMs), sizeof( ((idMenuHandler_Shell *)0)->nextPeerUpdateMs ) },
	{ "int", "newGameType", (intptr_t)(&((idMenuHandler_Shell *)0)->newGameType), sizeof( ((idMenuHandler_Shell *)0)->newGameType ) },
	{ "bool", "inGame", (intptr_t)(&((idMenuHandler_Shell *)0)->inGame), sizeof( ((idMenuHandler_Shell *)0)->inGame ) },
	{ "bool", "showingIntro", (intptr_t)(&((idMenuHandler_Shell *)0)->showingIntro), sizeof( ((idMenuHandler_Shell *)0)->showingIntro ) },
	{ "bool", "continueWaitForEnumerate", (intptr_t)(&((idMenuHandler_Shell *)0)->continueWaitForEnumerate), sizeof( ((idMenuHandler_Shell *)0)->continueWaitForEnumerate ) },
	{ "bool", "gameComplete", (intptr_t)(&((idMenuHandler_Shell *)0)->gameComplete), sizeof( ((idMenuHandler_Shell *)0)->gameComplete ) },
	{ "idSWF *", "introGui", (intptr_t)(&((idMenuHandler_Shell *)0)->introGui), sizeof( ((idMenuHandler_Shell *)0)->introGui ) },
	{ "const idSoundShader *", "typeSoundShader", (intptr_t)(&((idMenuHandler_Shell *)0)->typeSoundShader), sizeof( ((idMenuHandler_Shell *)0)->typeSoundShader ) },
	{ "const idMaterial *", "doom3Intro", (intptr_t)(&((idMenuHandler_Shell *)0)->doom3Intro), sizeof( ((idMenuHandler_Shell *)0)->doom3Intro ) },
	{ "const idMaterial *", "roeIntro", (intptr_t)(&((idMenuHandler_Shell *)0)->roeIntro), sizeof( ((idMenuHandler_Shell *)0)->roeIntro ) },
	{ "const idMaterial *", "lmIntro", (intptr_t)(&((idMenuHandler_Shell *)0)->lmIntro), sizeof( ((idMenuHandler_Shell *)0)->lmIntro ) },
	{ "const idMaterial *", "marsRotation", (intptr_t)(&((idMenuHandler_Shell *)0)->marsRotation), sizeof( ((idMenuHandler_Shell *)0)->marsRotation ) },
	{ "idList < idStr , TAG_IDLIB_LIST_MENU >", "navOptions", (intptr_t)(&((idMenuHandler_Shell *)0)->navOptions), sizeof( ((idMenuHandler_Shell *)0)->navOptions ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuHandler_PDA_typeInfo[] = {
	{ ": bool", "audioLogPlaying", (intptr_t)(&((idMenuHandler_PDA *)0)->audioLogPlaying), sizeof( ((idMenuHandler_PDA *)0)->audioLogPlaying ) },
	{ "bool", "videoPlaying", (intptr_t)(&((idMenuHandler_PDA *)0)->videoPlaying), sizeof( ((idMenuHandler_PDA *)0)->videoPlaying ) },
	{ "idList < idList < idStr , TAG_IDLIB_LIST_MENU > , TAG_IDLIB_LIST_MENU >", "pdaNames", (intptr_t)(&((idMenuHandler_PDA *)0)->pdaNames), sizeof( ((idMenuHandler_PDA *)0)->pdaNames ) },
	{ "idList < idStr , TAG_IDLIB_LIST_MENU >", "navOptions", (intptr_t)(&((idMenuHandler_PDA *)0)->navOptions), sizeof( ((idMenuHandler_PDA *)0)->navOptions ) },
	{ "const idDeclAudio *", "audioFile", (intptr_t)(&((idMenuHandler_PDA *)0)->audioFile), sizeof( ((idMenuHandler_PDA *)0)->audioFile ) },
	{ "idMenuWidget_ScrollBar", "pdaScrollBar", (intptr_t)(&((idMenuHandler_PDA *)0)->pdaScrollBar), sizeof( ((idMenuHandler_PDA *)0)->pdaScrollBar ) },
	{ "idMenuWidget_DynamicList", "pdaList", (intptr_t)(&((idMenuHandler_PDA *)0)->pdaList), sizeof( ((idMenuHandler_PDA *)0)->pdaList ) },
	{ "idMenuWidget_NavBar", "navBar", (intptr_t)(&((idMenuHandler_PDA *)0)->navBar), sizeof( ((idMenuHandler_PDA *)0)->navBar ) },
	{ "idMenuWidget_CommandBar", "commandBarWidget", (intptr_t)(&((idMenuHandler_PDA *)0)->commandBarWidget), sizeof( ((idMenuHandler_PDA *)0)->commandBarWidget ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuHandler_HUD_typeInfo[] = {
	{ ": bool", "autoHideTip", (intptr_t)(&((idMenuHandler_HUD *)0)->autoHideTip), sizeof( ((idMenuHandler_HUD *)0)->autoHideTip ) },
	{ "int", "tipStartTime", (intptr_t)(&((idMenuHandler_HUD *)0)->tipStartTime), sizeof( ((idMenuHandler_HUD *)0)->tipStartTime ) },
	{ "bool", "hiding", (intptr_t)(&((idMenuHandler_HUD *)0)->hiding), sizeof( ((idMenuHandler_HUD *)0)->hiding ) },
	{ "bool", "radioMessage", (intptr_t)(&((idMenuHandler_HUD *)0)->radioMessage), sizeof( ((idMenuHandler_HUD *)0)->radioMessage ) },
	{ NULL, 0 }
};

static classVariableInfo_t idMenuHandler_Scoreboard_typeInfo[] = {
	{ ": int", "redScore", (intptr_t)(&((idMenuHandler_Scoreboard *)0)->redScore), sizeof( ((idMenuHandler_Scoreboard *)0)->redScore ) },
	{ "int", "blueScore", (intptr_t)(&((idMenuHandler_Scoreboard *)0)->blueScore), sizeof( ((idMenuHandler_Scoreboard *)0)->blueScore ) },
	{ "int", "activationScreen", (intptr_t)(&((idMenuHandler_Scoreboard *)0)->activationScreen), sizeof( ((idMenuHandler_Scoreboard *)0)->activationScreen ) },
	{ "idList < mpScoreboardInfo >", "scoreboardInfo", (intptr_t)(&((idMenuHandler_Scoreboard *)0)->scoreboardInfo), sizeof( ((idMenuHandler_Scoreboard *)0)->scoreboardInfo ) },
	{ "idList < scoreboardInfo_t , TAG_IDLIB_LIST_MENU >", "redInfo", (intptr_t)(&((idMenuHandler_Scoreboard *)0)->redInfo), sizeof( ((idMenuHandler_Scoreboard *)0)->redInfo ) },
	{ "idList < scoreboardInfo_t , TAG_IDLIB_LIST_MENU >", "blueInfo", (intptr_t)(&((idMenuHandler_Scoreboard *)0)->blueInfo), sizeof( ((idMenuHandler_Scoreboard *)0)->blueInfo ) },
	{ NULL, 0 }
};

static classVariableInfo_t opcode_t_typeInfo[] = {
	{ "const char *", "name", (intptr_t)(&((opcode_t *)0)->name), sizeof( ((opcode_t *)0)->name ) },
	{ "const char *", "opname", (intptr_t)(&((opcode_t *)0)->opname), sizeof( ((opcode_t *)0)->opname ) },
	{ "int", "priority", (intptr_t)(&((opcode_t *)0)->priority), sizeof( ((opcode_t *)0)->priority ) },
	{ "bool", "rightAssociative", (intptr_t)(&((opcode_t *)0)->rightAssociative), sizeof( ((opcode_t *)0)->rightAssociative ) },
	{ "idVarDef *", "type_a", (intptr_t)(&((opcode_t *)0)->type_a), sizeof( ((opcode_t *)0)->type_a ) },
	{ "idVarDef *", "type_b", (intptr_t)(&((opcode_t *)0)->type_b), sizeof( ((opcode_t *)0)->type_b ) },
	{ "idVarDef *", "type_c", (intptr_t)(&((opcode_t *)0)->type_c), sizeof( ((opcode_t *)0)->type_c ) },
	{ NULL, 0 }
};

static classVariableInfo_t idCompiler_typeInfo[] = {
	{ "idParser", "parser", (intptr_t)(&((idCompiler *)0)->parser), sizeof( ((idCompiler *)0)->parser ) },
	{ "idParser *", "parserPtr", (intptr_t)(&((idCompiler *)0)->parserPtr), sizeof( ((idCompiler *)0)->parserPtr ) },
	{ "idToken", "token", (intptr_t)(&((idCompiler *)0)->token), sizeof( ((idCompiler *)0)->token ) },
	{ "idTypeDef *", "immediateType", (intptr_t)(&((idCompiler *)0)->immediateType), sizeof( ((idCompiler *)0)->immediateType ) },
	{ "eval_t", "immediate", (intptr_t)(&((idCompiler *)0)->immediate), sizeof( ((idCompiler *)0)->immediate ) },
	{ "bool", "eof", (intptr_t)(&((idCompiler *)0)->eof), sizeof( ((idCompiler *)0)->eof ) },
	{ "bool", "console", (intptr_t)(&((idCompiler *)0)->console), sizeof( ((idCompiler *)0)->console ) },
	{ "bool", "callthread", (intptr_t)(&((idCompiler *)0)->callthread), sizeof( ((idCompiler *)0)->callthread ) },
	{ "int", "braceDepth", (intptr_t)(&((idCompiler *)0)->braceDepth), sizeof( ((idCompiler *)0)->braceDepth ) },
	{ "int", "loopDepth", (intptr_t)(&((idCompiler *)0)->loopDepth), sizeof( ((idCompiler *)0)->loopDepth ) },
	{ "int", "currentLineNumber", (intptr_t)(&((idCompiler *)0)->currentLineNumber), sizeof( ((idCompiler *)0)->currentLineNumber ) },
	{ "int", "currentFileNumber", (intptr_t)(&((idCompiler *)0)->currentFileNumber), sizeof( ((idCompiler *)0)->currentFileNumber ) },
	{ "int", "errorCount", (intptr_t)(&((idCompiler *)0)->errorCount), sizeof( ((idCompiler *)0)->errorCount ) },
	{ "idVarDef *", "scope", (intptr_t)(&((idCompiler *)0)->scope), sizeof( ((idCompiler *)0)->scope ) },
	{ "const idVarDef *", "basetype", (intptr_t)(&((idCompiler *)0)->basetype), sizeof( ((idCompiler *)0)->basetype ) },
	{ NULL, 0 }
};

static classVariableInfo_t prstack_t_typeInfo[] = {
	{ "int", "s", (intptr_t)(&((prstack_t *)0)->s), sizeof( ((prstack_t *)0)->s ) },
	{ "const function_t *", "f", (intptr_t)(&((prstack_t *)0)->f), sizeof( ((prstack_t *)0)->f ) },
	{ "int", "stackbase", (intptr_t)(&((prstack_t *)0)->stackbase), sizeof( ((prstack_t *)0)->stackbase ) },
	{ NULL, 0 }
};

static classVariableInfo_t idInterpreter_typeInfo[] = {
	{ ": prstack_t[64]", "callStack", (intptr_t)(&((idInterpreter *)0)->callStack), sizeof( ((idInterpreter *)0)->callStack ) },
	{ "int", "callStackDepth", (intptr_t)(&((idInterpreter *)0)->callStackDepth), sizeof( ((idInterpreter *)0)->callStackDepth ) },
	{ "int", "maxStackDepth", (intptr_t)(&((idInterpreter *)0)->maxStackDepth), sizeof( ((idInterpreter *)0)->maxStackDepth ) },
	{ "byte[12288]", "localstack", (intptr_t)(&((idInterpreter *)0)->localstack), sizeof( ((idInterpreter *)0)->localstack ) },
	{ "int", "localstackUsed", (intptr_t)(&((idInterpreter *)0)->localstackUsed), sizeof( ((idInterpreter *)0)->localstackUsed ) },
	{ "int", "localstackBase", (intptr_t)(&((idInterpreter *)0)->localstackBase), sizeof( ((idInterpreter *)0)->localstackBase ) },
	{ "int", "maxLocalstackUsed", (intptr_t)(&((idInterpreter *)0)->maxLocalstackUsed), sizeof( ((idInterpreter *)0)->maxLocalstackUsed ) },
	{ "const function_t *", "currentFunction", (intptr_t)(&((idInterpreter *)0)->currentFunction), sizeof( ((idInterpreter *)0)->currentFunction ) },
	{ "int", "instructionPointer", (intptr_t)(&((idInterpreter *)0)->instructionPointer), sizeof( ((idInterpreter *)0)->instructionPointer ) },
	{ "int", "popParms", (intptr_t)(&((idInterpreter *)0)->popParms), sizeof( ((idInterpreter *)0)->popParms ) },
	{ "const idEventDef *", "multiFrameEvent", (intptr_t)(&((idInterpreter *)0)->multiFrameEvent), sizeof( ((idInterpreter *)0)->multiFrameEvent ) },
	{ "idEntity *", "eventEntity", (intptr_t)(&((idInterpreter *)0)->eventEntity), sizeof( ((idInterpreter *)0)->eventEntity ) },
	{ "idThread *", "thread", (intptr_t)(&((idInterpreter *)0)->thread), sizeof( ((idInterpreter *)0)->thread ) },
	{ ": bool", "doneProcessing", (intptr_t)(&((idInterpreter *)0)->doneProcessing), sizeof( ((idInterpreter *)0)->doneProcessing ) },
	{ "bool", "threadDying", (intptr_t)(&((idInterpreter *)0)->threadDying), sizeof( ((idInterpreter *)0)->threadDying ) },
	{ "bool", "terminateOnExit", (intptr_t)(&((idInterpreter *)0)->terminateOnExit), sizeof( ((idInterpreter *)0)->terminateOnExit ) },
	{ "bool", "debug", (intptr_t)(&((idInterpreter *)0)->debug), sizeof( ((idInterpreter *)0)->debug ) },
	{ NULL, 0 }
};

static classVariableInfo_t idThread_typeInfo[] = {
	{ "idThread *", "waitingForThread", (intptr_t)(&((idThread *)0)->waitingForThread), sizeof( ((idThread *)0)->waitingForThread ) },
	{ "int", "waitingFor", (intptr_t)(&((idThread *)0)->waitingFor), sizeof( ((idThread *)0)->waitingFor ) },
	{ "int", "waitingUntil", (intptr_t)(&((idThread *)0)->waitingUntil), sizeof( ((idThread *)0)->waitingUntil ) },
	{ "idInterpreter", "interpreter", (intptr_t)(&((idThread *)0)->interpreter), sizeof( ((idThread *)0)->interpreter ) },
	{ "idDict", "spawnArgs", (intptr_t)(&((idThread *)0)->spawnArgs), sizeof( ((idThread *)0)->spawnArgs ) },
	{ "int", "threadNum", (intptr_t)(&((idThread *)0)->threadNum), sizeof( ((idThread *)0)->threadNum ) },
	{ "idStr", "threadName", (intptr_t)(&((idThread *)0)->threadName), sizeof( ((idThread *)0)->threadName ) },
	{ "int", "lastExecuteTime", (intptr_t)(&((idThread *)0)->lastExecuteTime), sizeof( ((idThread *)0)->lastExecuteTime ) },
	{ "int", "creationTime", (intptr_t)(&((idThread *)0)->creationTime), sizeof( ((idThread *)0)->creationTime ) },
	{ "bool", "manualControl", (intptr_t)(&((idThread *)0)->manualControl), sizeof( ((idThread *)0)->manualControl ) },
	{ NULL, 0 }
};

static classTypeInfo_t classTypeInfo[] = {
	{ "idEventDef", "", sizeof(idEventDef), idEventDef_typeInfo },
	{ "idEvent", "", sizeof(idEvent), idEvent_typeInfo },
	{ "stateParms_t", "", sizeof(stateParms_t), stateParms_t_typeInfo },
	{ "stateCall_t", "", sizeof(stateCall_t), stateCall_t_typeInfo },
	{ "rvStateThread::flags", "", sizeof(rvStateThread::flags), rvStateThread_flags_typeInfo },
	{ "rvStateThread", "", sizeof(rvStateThread), rvStateThread_typeInfo },
	{ "idEventArg", "", sizeof(idEventArg), idEventArg_typeInfo },
	{ "idAllocError", "idException", sizeof(idAllocError), idAllocError_typeInfo },
	{ "idClass", "", sizeof(idClass), idClass_typeInfo },
	{ "idTypeInfo", "", sizeof(idTypeInfo), idTypeInfo_typeInfo },
	{ "idSaveGame::stringTableIndex_s", "", sizeof(idSaveGame::stringTableIndex_s), idSaveGame_stringTableIndex_s_typeInfo },
	{ "idSaveGame", "", sizeof(idSaveGame), idSaveGame_typeInfo },
	{ "idRestoreGame", "", sizeof(idRestoreGame), idRestoreGame_typeInfo },
	{ "function_t", "", sizeof(function_t), function_t_typeInfo },
	{ "eval_t", "", sizeof(eval_t), eval_t_typeInfo },
	{ "idTypeDef", "", sizeof(idTypeDef), idTypeDef_typeInfo },
	{ "idScriptObject", "", sizeof(idScriptObject), idScriptObject_typeInfo },
//	{ "idScriptVariable< class type , etype_t etype , class returnType >", "", sizeof(idScriptVariable< class type , etype_t etype , class returnType >), idScriptVariable_class_type_etype_t_etype_class_returnType__typeInfo },
	{ "idCompileError", "idException", sizeof(idCompileError), idCompileError_typeInfo },
	{ "varEval_t", "", sizeof(varEval_t), varEval_t_typeInfo },
	{ "idVarDef", "", sizeof(idVarDef), idVarDef_typeInfo },
	{ "idVarDefName", "", sizeof(idVarDefName), idVarDefName_typeInfo },
	{ "statement_t", "", sizeof(statement_t), statement_t_typeInfo },
	{ "idProgram", "", sizeof(idProgram), idProgram_typeInfo },
	{ "frameBlend_t", "", sizeof(frameBlend_t), frameBlend_t_typeInfo },
	{ "jointAnimInfo_t", "", sizeof(jointAnimInfo_t), jointAnimInfo_t_typeInfo },
	{ "jointInfo_t", "", sizeof(jointInfo_t), jointInfo_t_typeInfo },
	{ "jointMod_t", "", sizeof(jointMod_t), jointMod_t_typeInfo },
	{ "frameLookup_t", "", sizeof(frameLookup_t), frameLookup_t_typeInfo },
//	{ "class_29::class_29", "", sizeof(class_29::class_29), class_29_class_29_typeInfo },
	{ "frameCommand_t", "", sizeof(frameCommand_t), frameCommand_t_typeInfo },
	{ "animFlags_t", "", sizeof(animFlags_t), animFlags_t_typeInfo },
	{ "idMD5Anim", "", sizeof(idMD5Anim), idMD5Anim_typeInfo },
	{ "idAnim", "", sizeof(idAnim), idAnim_typeInfo },
	{ "idDeclModelDef", "idDecl", sizeof(idDeclModelDef), idDeclModelDef_typeInfo },
	{ "idAnimBlend", "", sizeof(idAnimBlend), idAnimBlend_typeInfo },
	{ "idAFPoseJointMod", "", sizeof(idAFPoseJointMod), idAFPoseJointMod_typeInfo },
	{ "idAnimator", "", sizeof(idAnimator), idAnimator_typeInfo },
	{ "idAnimManager", "", sizeof(idAnimManager), idAnimManager_typeInfo },
	{ "aasPath_t", "", sizeof(aasPath_t), aasPath_t_typeInfo },
	{ "aasGoal_t", "", sizeof(aasGoal_t), aasGoal_t_typeInfo },
	{ "aasObstacle_t", "", sizeof(aasObstacle_t), aasObstacle_t_typeInfo },
	{ "idAASCallback", "", sizeof(idAASCallback), idAASCallback_typeInfo },
	{ "idAAS", "", sizeof(idAAS), idAAS_typeInfo },
	{ "idClipModel", "", sizeof(idClipModel), idClipModel_typeInfo },
	{ "idClip", "", sizeof(idClip), idClip_typeInfo },
	{ "idPush::pushed_s", "", sizeof(idPush::pushed_s), idPush_pushed_s_typeInfo },
	{ "idPush::pushedGroup_s", "", sizeof(idPush::pushedGroup_s), idPush_pushedGroup_s_typeInfo },
	{ "idPush", "", sizeof(idPush), idPush_typeInfo },
	{ "pvsHandle_t", "", sizeof(pvsHandle_t), pvsHandle_t_typeInfo },
	{ "pvsCurrent_t", "", sizeof(pvsCurrent_t), pvsCurrent_t_typeInfo },
	{ "idPVS", "", sizeof(idPVS), idPVS_typeInfo },
	{ "leaderboardStats_t", "", sizeof(leaderboardStats_t), leaderboardStats_t_typeInfo },
	{ "columnGameMode_t", "", sizeof(columnGameMode_t), columnGameMode_t_typeInfo },
	{ "mpPlayerState_t", "", sizeof(mpPlayerState_t), mpPlayerState_t_typeInfo },
	{ "mpChatLine_t", "", sizeof(mpChatLine_t), mpChatLine_t_typeInfo },
	{ "idMultiplayerGame", "", sizeof(idMultiplayerGame), idMultiplayerGame_typeInfo },
	{ "entityNetEvent_t", "", sizeof(entityNetEvent_t), entityNetEvent_t_typeInfo },
	{ "spawnSpot_t", "", sizeof(spawnSpot_t), spawnSpot_t_typeInfo },
	{ "idEventQueue", "", sizeof(idEventQueue), idEventQueue_typeInfo },
//	{ "idEntityPtr< class type >", "", sizeof(idEntityPtr< class type >), idEntityPtr_class_type__typeInfo },
	{ "timeState_t", "", sizeof(timeState_t), timeState_t_typeInfo },
	{ "iceGameDelayRemoveEntry_t", "", sizeof(iceGameDelayRemoveEntry_t), iceGameDelayRemoveEntry_t_typeInfo },
	{ "idGameLocal::netInterpolationInfo_t", "", sizeof(idGameLocal::netInterpolationInfo_t), idGameLocal_netInterpolationInfo_t_typeInfo },
	{ "idGameLocal", "idGame", sizeof(idGameLocal), idGameLocal_typeInfo },
	{ "idGameError", "idException", sizeof(idGameError), idGameError_typeInfo },
	{ "idForce", "idClass", sizeof(idForce), idForce_typeInfo },
	{ "idForce_Constant", "idForce", sizeof(idForce_Constant), idForce_Constant_typeInfo },
	{ "idForce_Drag", "idForce", sizeof(idForce_Drag), idForce_Drag_typeInfo },
	{ "idForce_Grab", "idForce", sizeof(idForce_Grab), idForce_Grab_typeInfo },
	{ "idForce_Field", "idForce", sizeof(idForce_Field), idForce_Field_typeInfo },
	{ "idForce_Spring", "idForce", sizeof(idForce_Spring), idForce_Spring_typeInfo },
	{ "impactInfo_t", "", sizeof(impactInfo_t), impactInfo_t_typeInfo },
	{ "idPhysics", "idClass", sizeof(idPhysics), idPhysics_typeInfo },
	{ "staticPState_t", "", sizeof(staticPState_t), staticPState_t_typeInfo },
	{ "staticInterpolatePState_t", "", sizeof(staticInterpolatePState_t), staticInterpolatePState_t_typeInfo },
	{ "idPhysics_Static", "idPhysics", sizeof(idPhysics_Static), idPhysics_Static_typeInfo },
	{ "idPhysics_StaticMulti", "idPhysics", sizeof(idPhysics_StaticMulti), idPhysics_StaticMulti_typeInfo },
	{ "idPhysics_Base", "idPhysics", sizeof(idPhysics_Base), idPhysics_Base_typeInfo },
	{ "physicsInterpolationState_t", "", sizeof(physicsInterpolationState_t), physicsInterpolationState_t_typeInfo },
	{ "idPhysics_Actor", "idPhysics_Base", sizeof(idPhysics_Actor), idPhysics_Actor_typeInfo },
	{ "monsterPState_t", "", sizeof(monsterPState_t), monsterPState_t_typeInfo },
	{ "idPhysics_Monster", "idPhysics_Actor", sizeof(idPhysics_Monster), idPhysics_Monster_typeInfo },
	{ "playerPState_t", "", sizeof(playerPState_t), playerPState_t_typeInfo },
	{ "idPhysics_Player", "idPhysics_Actor", sizeof(idPhysics_Player), idPhysics_Player_typeInfo },
	{ "parametricPState_t", "", sizeof(parametricPState_t), parametricPState_t_typeInfo },
	{ "idPhysics_Parametric", "idPhysics_Base", sizeof(idPhysics_Parametric), idPhysics_Parametric_typeInfo },
	{ "rigidBodyIState_t", "", sizeof(rigidBodyIState_t), rigidBodyIState_t_typeInfo },
	{ "rigidBodyPState_t", "", sizeof(rigidBodyPState_t), rigidBodyPState_t_typeInfo },
	{ "idPhysics_RigidBody", "idPhysics_Base", sizeof(idPhysics_RigidBody), idPhysics_RigidBody_typeInfo },
	{ "idAFConstraint::constraintFlags_s", "", sizeof(idAFConstraint::constraintFlags_s), idAFConstraint_constraintFlags_s_typeInfo },
	{ "idAFConstraint", "", sizeof(idAFConstraint), idAFConstraint_typeInfo },
	{ "idAFConstraint_Fixed", "idAFConstraint", sizeof(idAFConstraint_Fixed), idAFConstraint_Fixed_typeInfo },
	{ "idAFConstraint_BallAndSocketJoint", "idAFConstraint", sizeof(idAFConstraint_BallAndSocketJoint), idAFConstraint_BallAndSocketJoint_typeInfo },
	{ "idAFConstraint_BallAndSocketJointFriction", "idAFConstraint", sizeof(idAFConstraint_BallAndSocketJointFriction), idAFConstraint_BallAndSocketJointFriction_typeInfo },
	{ "idAFConstraint_UniversalJoint", "idAFConstraint", sizeof(idAFConstraint_UniversalJoint), idAFConstraint_UniversalJoint_typeInfo },
	{ "idAFConstraint_UniversalJointFriction", "idAFConstraint", sizeof(idAFConstraint_UniversalJointFriction), idAFConstraint_UniversalJointFriction_typeInfo },
	{ "idAFConstraint_CylindricalJoint", "idAFConstraint", sizeof(idAFConstraint_CylindricalJoint), idAFConstraint_CylindricalJoint_typeInfo },
	{ "idAFConstraint_Hinge", "idAFConstraint", sizeof(idAFConstraint_Hinge), idAFConstraint_Hinge_typeInfo },
	{ "idAFConstraint_HingeFriction", "idAFConstraint", sizeof(idAFConstraint_HingeFriction), idAFConstraint_HingeFriction_typeInfo },
	{ "idAFConstraint_HingeSteering", "idAFConstraint", sizeof(idAFConstraint_HingeSteering), idAFConstraint_HingeSteering_typeInfo },
	{ "idAFConstraint_Slider", "idAFConstraint", sizeof(idAFConstraint_Slider), idAFConstraint_Slider_typeInfo },
	{ "idAFConstraint_Line", "idAFConstraint", sizeof(idAFConstraint_Line), idAFConstraint_Line_typeInfo },
	{ "idAFConstraint_Plane", "idAFConstraint", sizeof(idAFConstraint_Plane), idAFConstraint_Plane_typeInfo },
	{ "idAFConstraint_Spring", "idAFConstraint", sizeof(idAFConstraint_Spring), idAFConstraint_Spring_typeInfo },
	{ "idAFConstraint_Contact", "idAFConstraint", sizeof(idAFConstraint_Contact), idAFConstraint_Contact_typeInfo },
	{ "idAFConstraint_ContactFriction", "idAFConstraint", sizeof(idAFConstraint_ContactFriction), idAFConstraint_ContactFriction_typeInfo },
	{ "idAFConstraint_ConeLimit", "idAFConstraint", sizeof(idAFConstraint_ConeLimit), idAFConstraint_ConeLimit_typeInfo },
	{ "idAFConstraint_PyramidLimit", "idAFConstraint", sizeof(idAFConstraint_PyramidLimit), idAFConstraint_PyramidLimit_typeInfo },
	{ "idAFConstraint_Suspension", "idAFConstraint", sizeof(idAFConstraint_Suspension), idAFConstraint_Suspension_typeInfo },
	{ "AFBodyPState_t", "", sizeof(AFBodyPState_t), AFBodyPState_t_typeInfo },
	{ "idAFBody::bodyFlags_s", "", sizeof(idAFBody::bodyFlags_s), idAFBody_bodyFlags_s_typeInfo },
	{ "idAFBody", "", sizeof(idAFBody), idAFBody_typeInfo },
	{ "idAFTree", "", sizeof(idAFTree), idAFTree_typeInfo },
	{ "AFPState_t", "", sizeof(AFPState_t), AFPState_t_typeInfo },
	{ "AFCollision_t", "", sizeof(AFCollision_t), AFCollision_t_typeInfo },
	{ "idPhysics_AF", "idPhysics_Base", sizeof(idPhysics_AF), idPhysics_AF_typeInfo },
	{ "singleSmoke_t", "", sizeof(singleSmoke_t), singleSmoke_t_typeInfo },
	{ "activeSmokeStage_t", "", sizeof(activeSmokeStage_t), activeSmokeStage_t_typeInfo },
	{ "idSmokeParticles", "", sizeof(idSmokeParticles), idSmokeParticles_typeInfo },
	{ "signal_t", "", sizeof(signal_t), signal_t_typeInfo },
	{ "signalList_t", "", sizeof(signalList_t), signalList_t_typeInfo },
	{ "idEntity::entityFlags_s", "", sizeof(idEntity::entityFlags_s), idEntity_entityFlags_s_typeInfo },
	{ "idEntity", "idClass", sizeof(idEntity), idEntity_typeInfo },
	{ "damageEffect_t", "", sizeof(damageEffect_t), damageEffect_t_typeInfo },
	{ "idAnimatedEntity", "idEntity", sizeof(idAnimatedEntity), idAnimatedEntity_typeInfo },
	{ "SetTimeState", "", sizeof(SetTimeState), SetTimeState_typeInfo },
	{ "idCursor3D", "idEntity", sizeof(idCursor3D), idCursor3D_typeInfo },
	{ "idDragEntity", "", sizeof(idDragEntity), idDragEntity_typeInfo },
	{ "selectedTypeInfo_t", "", sizeof(selectedTypeInfo_t), selectedTypeInfo_t_typeInfo },
	{ "idEditEntities", "", sizeof(idEditEntities), idEditEntities_typeInfo },
	{ "idGrabber", "idEntity", sizeof(idGrabber), idGrabber_typeInfo },
	{ "jointConversion_t", "", sizeof(jointConversion_t), jointConversion_t_typeInfo },
	{ "afTouch_t", "", sizeof(afTouch_t), afTouch_t_typeInfo },
	{ "idAF", "", sizeof(idAF), idAF_typeInfo },
	{ "idIK", "", sizeof(idIK), idIK_typeInfo },
	{ "idIK_Walk", "idIK", sizeof(idIK_Walk), idIK_Walk_typeInfo },
	{ "idIK_Reach", "idIK", sizeof(idIK_Reach), idIK_Reach_typeInfo },
	{ "idMultiModelAF", "idEntity", sizeof(idMultiModelAF), idMultiModelAF_typeInfo },
	{ "idChain", "idMultiModelAF", sizeof(idChain), idChain_typeInfo },
	{ "idAFAttachment", "idAnimatedEntity", sizeof(idAFAttachment), idAFAttachment_typeInfo },
	{ "idAFEntity_Base", "idAnimatedEntity", sizeof(idAFEntity_Base), idAFEntity_Base_typeInfo },
	{ "idAFEntity_Gibbable", "idAFEntity_Base", sizeof(idAFEntity_Gibbable), idAFEntity_Gibbable_typeInfo },
	{ "idAFEntity_Generic", "idAFEntity_Gibbable", sizeof(idAFEntity_Generic), idAFEntity_Generic_typeInfo },
	{ "idAFEntity_WithAttachedHead", "idAFEntity_Gibbable", sizeof(idAFEntity_WithAttachedHead), idAFEntity_WithAttachedHead_typeInfo },
	{ "idAFEntity_Vehicle", "idAFEntity_Base", sizeof(idAFEntity_Vehicle), idAFEntity_Vehicle_typeInfo },
	{ "idAFEntity_VehicleSimple", "idAFEntity_Vehicle", sizeof(idAFEntity_VehicleSimple), idAFEntity_VehicleSimple_typeInfo },
	{ "idAFEntity_VehicleFourWheels", "idAFEntity_Vehicle", sizeof(idAFEntity_VehicleFourWheels), idAFEntity_VehicleFourWheels_typeInfo },
	{ "idAFEntity_VehicleSixWheels", "idAFEntity_Vehicle", sizeof(idAFEntity_VehicleSixWheels), idAFEntity_VehicleSixWheels_typeInfo },
	{ "idAFEntity_VehicleAutomated", "idAFEntity_VehicleSixWheels", sizeof(idAFEntity_VehicleAutomated), idAFEntity_VehicleAutomated_typeInfo },
	{ "idAFEntity_SteamPipe", "idAFEntity_Base", sizeof(idAFEntity_SteamPipe), idAFEntity_SteamPipe_typeInfo },
	{ "idAFEntity_ClawFourFingers", "idAFEntity_Base", sizeof(idAFEntity_ClawFourFingers), idAFEntity_ClawFourFingers_typeInfo },
	{ "idHarvestable", "idEntity", sizeof(idHarvestable), idHarvestable_typeInfo },
	{ "idAFEntity_Harvest", "idAFEntity_WithAttachedHead", sizeof(idAFEntity_Harvest), idAFEntity_Harvest_typeInfo },
	{ "idSpawnableEntity", "idEntity", sizeof(idSpawnableEntity), idSpawnableEntity_typeInfo },
	{ "idPlayerStart", "idEntity", sizeof(idPlayerStart), idPlayerStart_typeInfo },
	{ "idActivator", "idEntity", sizeof(idActivator), idActivator_typeInfo },
	{ "idPathCorner", "idEntity", sizeof(idPathCorner), idPathCorner_typeInfo },
	{ "idDamagable", "idEntity", sizeof(idDamagable), idDamagable_typeInfo },
	{ "idExplodable", "idEntity", sizeof(idExplodable), idExplodable_typeInfo },
	{ "idSpring", "idEntity", sizeof(idSpring), idSpring_typeInfo },
	{ "idForceField", "idEntity", sizeof(idForceField), idForceField_typeInfo },
	{ "idAnimated", "idAFEntity_Gibbable", sizeof(idAnimated), idAnimated_typeInfo },
	{ "idStaticEntity", "idEntity", sizeof(idStaticEntity), idStaticEntity_typeInfo },
	{ "idFuncEmitter", "idStaticEntity", sizeof(idFuncEmitter), idFuncEmitter_typeInfo },
	{ "idFuncShootProjectile", "idStaticEntity", sizeof(idFuncShootProjectile), idFuncShootProjectile_typeInfo },
	{ "idFuncSmoke", "idEntity", sizeof(idFuncSmoke), idFuncSmoke_typeInfo },
	{ "idFuncSplat", "idFuncEmitter", sizeof(idFuncSplat), idFuncSplat_typeInfo },
	{ "idTextEntity", "idEntity", sizeof(idTextEntity), idTextEntity_typeInfo },
	{ "idLocationEntity", "idEntity", sizeof(idLocationEntity), idLocationEntity_typeInfo },
	{ "idLocationSeparatorEntity", "idEntity", sizeof(idLocationSeparatorEntity), idLocationSeparatorEntity_typeInfo },
	{ "idVacuumSeparatorEntity", "idEntity", sizeof(idVacuumSeparatorEntity), idVacuumSeparatorEntity_typeInfo },
	{ "idVacuumEntity", "idEntity", sizeof(idVacuumEntity), idVacuumEntity_typeInfo },
	{ "idBeam", "idEntity", sizeof(idBeam), idBeam_typeInfo },
	{ "idLiquid", "idEntity", sizeof(idLiquid), idLiquid_typeInfo },
	{ "idShaking", "idEntity", sizeof(idShaking), idShaking_typeInfo },
	{ "idEarthQuake", "idEntity", sizeof(idEarthQuake), idEarthQuake_typeInfo },
	{ "idFuncPortal", "idEntity", sizeof(idFuncPortal), idFuncPortal_typeInfo },
	{ "idFuncAASPortal", "idEntity", sizeof(idFuncAASPortal), idFuncAASPortal_typeInfo },
	{ "idFuncAASObstacle", "idEntity", sizeof(idFuncAASObstacle), idFuncAASObstacle_typeInfo },
	{ "idFuncRadioChatter", "idEntity", sizeof(idFuncRadioChatter), idFuncRadioChatter_typeInfo },
	{ "idPhantomObjects", "idEntity", sizeof(idPhantomObjects), idPhantomObjects_typeInfo },
	{ "idShockwave", "idEntity", sizeof(idShockwave), idShockwave_typeInfo },
	{ "idFuncMountedObject", "idEntity", sizeof(idFuncMountedObject), idFuncMountedObject_typeInfo },
	{ "idFuncMountedWeapon", "idFuncMountedObject", sizeof(idFuncMountedWeapon), idFuncMountedWeapon_typeInfo },
	{ "idPortalSky", "idEntity", sizeof(idPortalSky), idPortalSky_typeInfo },
	{ "idAnimState", "", sizeof(idAnimState), idAnimState_typeInfo },
	{ "idAttachInfo", "", sizeof(idAttachInfo), idAttachInfo_typeInfo },
	{ "copyJoints_t", "", sizeof(copyJoints_t), copyJoints_t_typeInfo },
	{ "idActor", "idAFEntity_Gibbable", sizeof(idActor), idActor_typeInfo },
	{ "idProjectile::simulatedProjectile_t", "", sizeof(idProjectile::simulatedProjectile_t), idProjectile_simulatedProjectile_t_typeInfo },
	{ "idProjectile::projectileFlags_s", "", sizeof(idProjectile::projectileFlags_s), idProjectile_projectileFlags_s_typeInfo },
	{ "idProjectile", "idEntity", sizeof(idProjectile), idProjectile_typeInfo },
	{ "idGuidedProjectile", "idProjectile", sizeof(idGuidedProjectile), idGuidedProjectile_typeInfo },
	{ "idSoulCubeMissile", "idGuidedProjectile", sizeof(idSoulCubeMissile), idSoulCubeMissile_typeInfo },
	{ "beamTarget_t", "", sizeof(beamTarget_t), beamTarget_t_typeInfo },
	{ "idBFGProjectile", "idProjectile", sizeof(idBFGProjectile), idBFGProjectile_typeInfo },
	{ "idHomingProjectile", "idProjectile", sizeof(idHomingProjectile), idHomingProjectile_typeInfo },
	{ "idDebris", "idEntity", sizeof(idDebris), idDebris_typeInfo },
//	{ "idPredictedValue< class type_ >", "", sizeof(idPredictedValue< class type_ >), idPredictedValue_class_type___typeInfo },
	{ "WeaponParticle_t", "", sizeof(WeaponParticle_t), WeaponParticle_t_typeInfo },
	{ "WeaponLight_t", "", sizeof(WeaponLight_t), WeaponLight_t_typeInfo },
	{ "iceWeaponObject", "idClass", sizeof(iceWeaponObject), iceWeaponObject_typeInfo },
	{ "idWeapon", "idAnimatedEntity", sizeof(idWeapon), idWeapon_typeInfo },
	{ "iceLightStyleState_t", "", sizeof(iceLightStyleState_t), iceLightStyleState_t_typeInfo },
	{ "idLight", "idEntity", sizeof(idLight), idLight_typeInfo },
	{ "EnvironmentProbe", "idEntity", sizeof(EnvironmentProbe), EnvironmentProbe_typeInfo },
	{ "idWorldspawn", "idEntity", sizeof(idWorldspawn), idWorldspawn_typeInfo },
	{ "idItem", "idEntity", sizeof(idItem), idItem_typeInfo },
	{ "idItemPowerup", "idItem", sizeof(idItemPowerup), idItemPowerup_typeInfo },
	{ "idObjective", "idItem", sizeof(idObjective), idObjective_typeInfo },
	{ "idVideoCDItem", "idItem", sizeof(idVideoCDItem), idVideoCDItem_typeInfo },
	{ "idPDAItem", "idItem", sizeof(idPDAItem), idPDAItem_typeInfo },
	{ "idMoveableItem", "idItem", sizeof(idMoveableItem), idMoveableItem_typeInfo },
	{ "idItemTeam", "idMoveableItem", sizeof(idItemTeam), idItemTeam_typeInfo },
	{ "idMoveablePDAItem", "idMoveableItem", sizeof(idMoveablePDAItem), idMoveablePDAItem_typeInfo },
	{ "idItemRemover", "idEntity", sizeof(idItemRemover), idItemRemover_typeInfo },
	{ "idObjectiveComplete", "idItemRemover", sizeof(idObjectiveComplete), idObjectiveComplete_typeInfo },
	{ "screenBlob_t", "", sizeof(screenBlob_t), screenBlob_t_typeInfo },
	{ "WarpPolygon_t", "", sizeof(WarpPolygon_t), WarpPolygon_t_typeInfo },
	{ "Warp_t", "", sizeof(Warp_t), Warp_t_typeInfo },
	{ "FxFader", "", sizeof(FxFader), FxFader_typeInfo },
	{ "FullscreenFX", "", sizeof(FullscreenFX), FullscreenFX_typeInfo },
	{ "FullscreenFX_Helltime", "FullscreenFX", sizeof(FullscreenFX_Helltime), FullscreenFX_Helltime_typeInfo },
	{ "FullscreenFX_Multiplayer", "FullscreenFX", sizeof(FullscreenFX_Multiplayer), FullscreenFX_Multiplayer_typeInfo },
	{ "FullscreenFX_Warp", "FullscreenFX", sizeof(FullscreenFX_Warp), FullscreenFX_Warp_typeInfo },
	{ "FullscreenFX_EnviroSuit", "FullscreenFX", sizeof(FullscreenFX_EnviroSuit), FullscreenFX_EnviroSuit_typeInfo },
	{ "FullscreenFX_DoubleVision", "FullscreenFX", sizeof(FullscreenFX_DoubleVision), FullscreenFX_DoubleVision_typeInfo },
	{ "FullscreenFX_InfluenceVision", "FullscreenFX", sizeof(FullscreenFX_InfluenceVision), FullscreenFX_InfluenceVision_typeInfo },
	{ "FullscreenFX_Bloom", "FullscreenFX", sizeof(FullscreenFX_Bloom), FullscreenFX_Bloom_typeInfo },
	{ "FullscreenFXManager", "", sizeof(FullscreenFXManager), FullscreenFXManager_typeInfo },
	{ "idPlayerView", "", sizeof(idPlayerView), idPlayerView_typeInfo },
	{ "idPlayerIcon", "", sizeof(idPlayerIcon), idPlayerIcon_typeInfo },
	{ "idAchievementManager", "", sizeof(idAchievementManager), idAchievementManager_typeInfo },
	{ "idAimAssist", "", sizeof(idAimAssist), idAimAssist_typeInfo },
	{ "idObjectiveInfo", "", sizeof(idObjectiveInfo), idObjectiveInfo_typeInfo },
	{ "idLevelTriggerInfo", "", sizeof(idLevelTriggerInfo), idLevelTriggerInfo_typeInfo },
	{ "RechargeAmmo_t", "", sizeof(RechargeAmmo_t), RechargeAmmo_t_typeInfo },
	{ "WeaponToggle_t", "", sizeof(WeaponToggle_t), WeaponToggle_t_typeInfo },
	{ "idInventory", "", sizeof(idInventory), idInventory_typeInfo },
	{ "loggedAccel_t", "", sizeof(loggedAccel_t), loggedAccel_t_typeInfo },
	{ "aasLocation_t", "", sizeof(aasLocation_t), aasLocation_t_typeInfo },
	{ "idPlayer", "idActor", sizeof(idPlayer), idPlayer_typeInfo },
	{ "idMover::moveState_t", "", sizeof(idMover::moveState_t), idMover_moveState_t_typeInfo },
	{ "idMover::rotationState_t", "", sizeof(idMover::rotationState_t), idMover_rotationState_t_typeInfo },
	{ "idMover", "idEntity", sizeof(idMover), idMover_typeInfo },
	{ "idSplinePath", "idEntity", sizeof(idSplinePath), idSplinePath_typeInfo },
	{ "floorInfo_s", "", sizeof(floorInfo_s), floorInfo_s_typeInfo },
	{ "idElevator", "idMover", sizeof(idElevator), idElevator_typeInfo },
	{ "idMover_Binary", "idEntity", sizeof(idMover_Binary), idMover_Binary_typeInfo },
	{ "idDoor", "idMover_Binary", sizeof(idDoor), idDoor_typeInfo },
	{ "idPlat", "idMover_Binary", sizeof(idPlat), idPlat_typeInfo },
	{ "idMover_Periodic", "idEntity", sizeof(idMover_Periodic), idMover_Periodic_typeInfo },
	{ "idRotater", "idMover_Periodic", sizeof(idRotater), idRotater_typeInfo },
	{ "idBobber", "idMover_Periodic", sizeof(idBobber), idBobber_typeInfo },
	{ "idPendulum", "idMover_Periodic", sizeof(idPendulum), idPendulum_typeInfo },
	{ "idRiser", "idMover_Periodic", sizeof(idRiser), idRiser_typeInfo },
	{ "idCamera", "idEntity", sizeof(idCamera), idCamera_typeInfo },
	{ "idCameraView", "idCamera", sizeof(idCameraView), idCameraView_typeInfo },
	{ "cameraFrame_t", "", sizeof(cameraFrame_t), cameraFrame_t_typeInfo },
	{ "idCameraAnim", "idCamera", sizeof(idCameraAnim), idCameraAnim_typeInfo },
	{ "idMoveable", "idEntity", sizeof(idMoveable), idMoveable_typeInfo },
	{ "idBarrel", "idMoveable", sizeof(idBarrel), idBarrel_typeInfo },
	{ "idExplodingBarrel", "idBarrel", sizeof(idExplodingBarrel), idExplodingBarrel_typeInfo },
	{ "idTarget", "idEntity", sizeof(idTarget), idTarget_typeInfo },
	{ "idTarget_Remove", "idTarget", sizeof(idTarget_Remove), idTarget_Remove_typeInfo },
	{ "idTarget_Show", "idTarget", sizeof(idTarget_Show), idTarget_Show_typeInfo },
	{ "idTarget_Damage", "idTarget", sizeof(idTarget_Damage), idTarget_Damage_typeInfo },
	{ "idTarget_SessionCommand", "idTarget", sizeof(idTarget_SessionCommand), idTarget_SessionCommand_typeInfo },
	{ "idTarget_EndLevel", "idTarget", sizeof(idTarget_EndLevel), idTarget_EndLevel_typeInfo },
	{ "idTarget_WaitForButton", "idTarget", sizeof(idTarget_WaitForButton), idTarget_WaitForButton_typeInfo },
	{ "idTarget_SetGlobalShaderTime", "idTarget", sizeof(idTarget_SetGlobalShaderTime), idTarget_SetGlobalShaderTime_typeInfo },
	{ "idTarget_SetShaderParm", "idTarget", sizeof(idTarget_SetShaderParm), idTarget_SetShaderParm_typeInfo },
	{ "idTarget_SetShaderTime", "idTarget", sizeof(idTarget_SetShaderTime), idTarget_SetShaderTime_typeInfo },
	{ "idTarget_FadeEntity", "idTarget", sizeof(idTarget_FadeEntity), idTarget_FadeEntity_typeInfo },
	{ "idTarget_LightFadeIn", "idTarget", sizeof(idTarget_LightFadeIn), idTarget_LightFadeIn_typeInfo },
	{ "idTarget_LightFadeOut", "idTarget", sizeof(idTarget_LightFadeOut), idTarget_LightFadeOut_typeInfo },
	{ "idTarget_Give", "idTarget", sizeof(idTarget_Give), idTarget_Give_typeInfo },
	{ "idTarget_GiveEmail", "idTarget", sizeof(idTarget_GiveEmail), idTarget_GiveEmail_typeInfo },
	{ "idTarget_SetModel", "idTarget", sizeof(idTarget_SetModel), idTarget_SetModel_typeInfo },
	{ "SavedGui_t", "", sizeof(SavedGui_t), SavedGui_t_typeInfo },
	{ "idTarget_SetInfluence", "idTarget", sizeof(idTarget_SetInfluence), idTarget_SetInfluence_typeInfo },
	{ "idTarget_SetKeyVal", "idTarget", sizeof(idTarget_SetKeyVal), idTarget_SetKeyVal_typeInfo },
	{ "idTarget_SetFov", "idTarget", sizeof(idTarget_SetFov), idTarget_SetFov_typeInfo },
	{ "idTarget_SetPrimaryObjective", "idTarget", sizeof(idTarget_SetPrimaryObjective), idTarget_SetPrimaryObjective_typeInfo },
	{ "idTarget_LockDoor", "idTarget", sizeof(idTarget_LockDoor), idTarget_LockDoor_typeInfo },
	{ "idTarget_CallObjectFunction", "idTarget", sizeof(idTarget_CallObjectFunction), idTarget_CallObjectFunction_typeInfo },
	{ "idTarget_EnableLevelWeapons", "idTarget", sizeof(idTarget_EnableLevelWeapons), idTarget_EnableLevelWeapons_typeInfo },
	{ "idTarget_Tip", "idTarget", sizeof(idTarget_Tip), idTarget_Tip_typeInfo },
	{ "idTarget_GiveSecurity", "idTarget", sizeof(idTarget_GiveSecurity), idTarget_GiveSecurity_typeInfo },
	{ "idTarget_RemoveWeapons", "idTarget", sizeof(idTarget_RemoveWeapons), idTarget_RemoveWeapons_typeInfo },
	{ "idTarget_LevelTrigger", "idTarget", sizeof(idTarget_LevelTrigger), idTarget_LevelTrigger_typeInfo },
	{ "idTarget_Checkpoint", "idTarget", sizeof(idTarget_Checkpoint), idTarget_Checkpoint_typeInfo },
	{ "idTarget_EnableStamina", "idTarget", sizeof(idTarget_EnableStamina), idTarget_EnableStamina_typeInfo },
	{ "idTarget_FadeSoundClass", "idTarget", sizeof(idTarget_FadeSoundClass), idTarget_FadeSoundClass_typeInfo },
	{ "idTarget_RumbleJoystick", "idTarget", sizeof(idTarget_RumbleJoystick), idTarget_RumbleJoystick_typeInfo },
	{ "idTarget_Achievement", "idTarget", sizeof(idTarget_Achievement), idTarget_Achievement_typeInfo },
	{ "idTrigger", "idEntity", sizeof(idTrigger), idTrigger_typeInfo },
	{ "idTrigger_Multi", "idTrigger", sizeof(idTrigger_Multi), idTrigger_Multi_typeInfo },
	{ "idTrigger_EntityName", "idTrigger", sizeof(idTrigger_EntityName), idTrigger_EntityName_typeInfo },
	{ "idTrigger_Timer", "idTrigger", sizeof(idTrigger_Timer), idTrigger_Timer_typeInfo },
	{ "idTrigger_Count", "idTrigger", sizeof(idTrigger_Count), idTrigger_Count_typeInfo },
	{ "idTrigger_Hurt", "idTrigger", sizeof(idTrigger_Hurt), idTrigger_Hurt_typeInfo },
	{ "idTrigger_Fade", "idTrigger", sizeof(idTrigger_Fade), idTrigger_Fade_typeInfo },
	{ "idTrigger_Touch", "idTrigger", sizeof(idTrigger_Touch), idTrigger_Touch_typeInfo },
	{ "idTrigger_Flag", "idTrigger_Multi", sizeof(idTrigger_Flag), idTrigger_Flag_typeInfo },
	{ "idSound", "idEntity", sizeof(idSound), idSound_typeInfo },
	{ "idFXLocalAction", "", sizeof(idFXLocalAction), idFXLocalAction_typeInfo },
	{ "idEntityFx", "idEntity", sizeof(idEntityFx), idEntityFx_typeInfo },
	{ "idTeleporter", "idEntityFx", sizeof(idTeleporter), idTeleporter_typeInfo },
	{ "idSecurityCamera", "idEntity", sizeof(idSecurityCamera), idSecurityCamera_typeInfo },
	{ "shard_t", "", sizeof(shard_t), shard_t_typeInfo },
	{ "idBrittleFracture::fractureEvent_s", "", sizeof(idBrittleFracture::fractureEvent_s), idBrittleFracture_fractureEvent_s_typeInfo },
	{ "idBrittleFracture", "idEntity", sizeof(idBrittleFracture), idBrittleFracture_typeInfo },
	{ "ballistics_t", "", sizeof(ballistics_t), ballistics_t_typeInfo },
	{ "obstaclePath_t", "", sizeof(obstaclePath_t), obstaclePath_t_typeInfo },
	{ "predictedPath_t", "", sizeof(predictedPath_t), predictedPath_t_typeInfo },
	{ "particleEmitter_t", "", sizeof(particleEmitter_t), particleEmitter_t_typeInfo },
	{ "funcEmitter_t", "", sizeof(funcEmitter_t), funcEmitter_t_typeInfo },
	{ "idMoveState", "", sizeof(idMoveState), idMoveState_typeInfo },
	{ "idAI", "idActor", sizeof(idAI), idAI_typeInfo },
	{ "idCombatNode", "idEntity", sizeof(idCombatNode), idCombatNode_typeInfo },
	{ "iceAI_Follower", "idAI", sizeof(iceAI_Follower), iceAI_Follower_typeInfo },
	{ "iceMonsterBossVagary", "idAI", sizeof(iceMonsterBossVagary), iceMonsterBossVagary_typeInfo },
	{ "iceMonsterDemonHellknight", "idAI", sizeof(iceMonsterDemonHellknight), iceMonsterDemonHellknight_typeInfo },
	{ "iceMonsterDemonImp", "idAI", sizeof(iceMonsterDemonImp), iceMonsterDemonImp_typeInfo },
	{ "iceMonsterLostSoul", "idAI", sizeof(iceMonsterLostSoul), iceMonsterLostSoul_typeInfo },
	{ "iceMonsterFlyingCacodemon", "idAI", sizeof(iceMonsterFlyingCacodemon), iceMonsterFlyingCacodemon_typeInfo },
	{ "iceMonsterZombie", "idAI", sizeof(iceMonsterZombie), iceMonsterZombie_typeInfo },
	{ "iceMonsterZombieSawyer", "iceMonsterZombie", sizeof(iceMonsterZombieSawyer), iceMonsterZombieSawyer_typeInfo },
	{ "iceMonsterZombieBernie", "iceMonsterZombie", sizeof(iceMonsterZombieBernie), iceMonsterZombieBernie_typeInfo },
	{ "iceMonsterZombieMorgue", "iceMonsterZombie", sizeof(iceMonsterZombieMorgue), iceMonsterZombieMorgue_typeInfo },
	{ "iceMonsterZombieSecurityPistol", "iceMonsterZombie", sizeof(iceMonsterZombieSecurityPistol), iceMonsterZombieSecurityPistol_typeInfo },
	{ "iceMonsterZombieCommandoTentacle", "iceMonsterZombie", sizeof(iceMonsterZombieCommandoTentacle), iceMonsterZombieCommandoTentacle_typeInfo },
	{ "iceMonsterZombieCommandoChaingun", "idAI", sizeof(iceMonsterZombieCommandoChaingun), iceMonsterZombieCommandoChaingun_typeInfo },
	{ "iceMonster_Turret", "idAI", sizeof(iceMonster_Turret), iceMonster_Turret_typeInfo },
	{ "fuzzyseperator_t", "", sizeof(fuzzyseperator_t), fuzzyseperator_t_typeInfo },
	{ "weight_t", "", sizeof(weight_t), weight_t_typeInfo },
	{ "weightconfig_t", "", sizeof(weightconfig_t), weightconfig_t_typeInfo },
	{ "cvalue", "", sizeof(cvalue), cvalue_typeInfo },
	{ "bot_characteristic_t", "", sizeof(bot_characteristic_t), bot_characteristic_t_typeInfo },
	{ "bot_character_t", "", sizeof(bot_character_t), bot_character_t_typeInfo },
	{ "bot_input_t", "", sizeof(bot_input_t), bot_input_t_typeInfo },
	{ "rvmBotUtil", "", sizeof(rvmBotUtil), rvmBotUtil_typeInfo },
	{ "idBotCharacterStatsManager", "", sizeof(idBotCharacterStatsManager), idBotCharacterStatsManager_typeInfo },
	{ "idBotFuzzyWeightManager", "", sizeof(idBotFuzzyWeightManager), idBotFuzzyWeightManager_typeInfo },
	{ "projectileinfo_t", "", sizeof(projectileinfo_t), projectileinfo_t_typeInfo },
	{ "bot_weaponinfo_t", "", sizeof(bot_weaponinfo_t), bot_weaponinfo_t_typeInfo },
	{ "bot_weaponstate_t", "", sizeof(bot_weaponstate_t), bot_weaponstate_t_typeInfo },
	{ "idBotWeaponInfoManager", "", sizeof(idBotWeaponInfoManager), idBotWeaponInfoManager_typeInfo },
	{ "bot_goal_t", "", sizeof(bot_goal_t), bot_goal_t_typeInfo },
	{ "maplocation_t", "", sizeof(maplocation_t), maplocation_t_typeInfo },
	{ "campspot_t", "", sizeof(campspot_t), campspot_t_typeInfo },
	{ "levelitem_t", "", sizeof(levelitem_t), levelitem_t_typeInfo },
	{ "iteminfo_t", "", sizeof(iteminfo_t), iteminfo_t_typeInfo },
	{ "itemconfig_t", "", sizeof(itemconfig_t), itemconfig_t_typeInfo },
	{ "bot_goalstate_t", "", sizeof(bot_goalstate_t), bot_goalstate_t_typeInfo },
	{ "idBotGoalManager", "", sizeof(idBotGoalManager), idBotGoalManager_typeInfo },
	{ "bot_state_t", "", sizeof(bot_state_t), bot_state_t_typeInfo },
	{ "iceBot", "idPlayer", sizeof(iceBot), iceBot_typeInfo },
	{ "idTestModel", "idAnimatedEntity", sizeof(idTestModel), idTestModel_typeInfo },
	{ "iceWeaponFist", "iceWeaponObject", sizeof(iceWeaponFist), iceWeaponFist_typeInfo },
	{ "iceWeaponPistol", "iceWeaponObject", sizeof(iceWeaponPistol), iceWeaponPistol_typeInfo },
	{ "iceWeaponFlashlight", "iceWeaponObject", sizeof(iceWeaponFlashlight), iceWeaponFlashlight_typeInfo },
	{ "iceWeaponPDA", "iceWeaponObject", sizeof(iceWeaponPDA), iceWeaponPDA_typeInfo },
	{ "iceWeaponShotgun", "iceWeaponObject", sizeof(iceWeaponShotgun), iceWeaponShotgun_typeInfo },
	{ "iceWeaponDoubleShotgun", "iceWeaponObject", sizeof(iceWeaponDoubleShotgun), iceWeaponDoubleShotgun_typeInfo },
	{ "iceWeaponMachineGun", "iceWeaponObject", sizeof(iceWeaponMachineGun), iceWeaponMachineGun_typeInfo },
	{ "iceWeaponPlasmaGun", "iceWeaponObject", sizeof(iceWeaponPlasmaGun), iceWeaponPlasmaGun_typeInfo },
	{ "iceWeaponChainGun", "iceWeaponObject", sizeof(iceWeaponChainGun), iceWeaponChainGun_typeInfo },
	{ "iceWeaponRocketLauncher", "iceWeaponObject", sizeof(iceWeaponRocketLauncher), iceWeaponRocketLauncher_typeInfo },
	{ "iceWeaponBFG", "iceWeaponObject", sizeof(iceWeaponBFG), iceWeaponBFG_typeInfo },
	{ "iceWeaponHandgrenade", "iceWeaponObject", sizeof(iceWeaponHandgrenade), iceWeaponHandgrenade_typeInfo },
	{ "iceWeaponChainsaw", "iceWeaponObject", sizeof(iceWeaponChainsaw), iceWeaponChainsaw_typeInfo },
	{ "iceWeaponGrabber", "iceWeaponObject", sizeof(iceWeaponGrabber), iceWeaponGrabber_typeInfo },
	{ "widgetTransition_t", "", sizeof(widgetTransition_t), widgetTransition_t_typeInfo },
	{ "scoreboardInfo_t", "", sizeof(scoreboardInfo_t), scoreboardInfo_t_typeInfo },
	{ "idSort_SavesByDate", "idSort_Quick< idSaveGameDetails , idSort_SavesByDate >", sizeof(idSort_SavesByDate), idSort_SavesByDate_typeInfo },
	{ "idMenuDataSource", "", sizeof(idMenuDataSource), idMenuDataSource_typeInfo },
	{ "idWidgetEvent", "", sizeof(idWidgetEvent), idWidgetEvent_typeInfo },
	{ "idWidgetAction", "", sizeof(idWidgetAction), idWidgetAction_typeInfo },
	{ "idMenuWidget::WrapWidgetSWFEvent", "idSWFScriptFunction_RefCounted", sizeof(idMenuWidget::WrapWidgetSWFEvent), idMenuWidget_WrapWidgetSWFEvent_typeInfo },
	{ "idMenuWidget", "", sizeof(idMenuWidget), idMenuWidget_typeInfo },
	{ "idMenuWidget_Button", "idMenuWidget", sizeof(idMenuWidget_Button), idMenuWidget_Button_typeInfo },
	{ "idMenuWidget_LobbyButton", "idMenuWidget_Button", sizeof(idMenuWidget_LobbyButton), idMenuWidget_LobbyButton_typeInfo },
	{ "idMenuWidget_ScoreboardButton", "idMenuWidget_Button", sizeof(idMenuWidget_ScoreboardButton), idMenuWidget_ScoreboardButton_typeInfo },
	{ "idMenuWidget_ControlButton", "idMenuWidget_Button", sizeof(idMenuWidget_ControlButton), idMenuWidget_ControlButton_typeInfo },
	{ "idMenuWidget_ServerButton", "idMenuWidget_Button", sizeof(idMenuWidget_ServerButton), idMenuWidget_ServerButton_typeInfo },
	{ "idMenuWidget_NavButton", "idMenuWidget_Button", sizeof(idMenuWidget_NavButton), idMenuWidget_NavButton_typeInfo },
	{ "idMenuWidget_MenuButton", "idMenuWidget_Button", sizeof(idMenuWidget_MenuButton), idMenuWidget_MenuButton_typeInfo },
	{ "idMenuWidget_List", "idMenuWidget", sizeof(idMenuWidget_List), idMenuWidget_List_typeInfo },
	{ "idBrowserEntry_t", "", sizeof(idBrowserEntry_t), idBrowserEntry_t_typeInfo },
	{ "idMenuWidget_GameBrowserList", "idMenuWidget_List", sizeof(idMenuWidget_GameBrowserList), idMenuWidget_GameBrowserList_typeInfo },
	{ "idMenuWidget_Carousel", "idMenuWidget", sizeof(idMenuWidget_Carousel), idMenuWidget_Carousel_typeInfo },
	{ "idMenuWidget_Help", "idMenuWidget", sizeof(idMenuWidget_Help), idMenuWidget_Help_typeInfo },
	{ "idMenuWidget_CommandBar::buttonInfo_t", "", sizeof(idMenuWidget_CommandBar::buttonInfo_t), idMenuWidget_CommandBar_buttonInfo_t_typeInfo },
	{ "idMenuWidget_CommandBar", "idMenuWidget", sizeof(idMenuWidget_CommandBar), idMenuWidget_CommandBar_typeInfo },
	{ "idMenuWidget_LobbyList", "idMenuWidget_List", sizeof(idMenuWidget_LobbyList), idMenuWidget_LobbyList_typeInfo },
	{ "idMenuWidget_DynamicList", "idMenuWidget_List", sizeof(idMenuWidget_DynamicList), idMenuWidget_DynamicList_typeInfo },
	{ "idMenuWidget_ScoreboardList", "idMenuWidget_DynamicList", sizeof(idMenuWidget_ScoreboardList), idMenuWidget_ScoreboardList_typeInfo },
	{ "idMenuWidget_SystemOptionsList", "idMenuWidget_DynamicList", sizeof(idMenuWidget_SystemOptionsList), idMenuWidget_SystemOptionsList_typeInfo },
	{ "idMenuWidget_NavBar", "idMenuWidget_DynamicList", sizeof(idMenuWidget_NavBar), idMenuWidget_NavBar_typeInfo },
	{ "idMenuWidget_MenuBar", "idMenuWidget_DynamicList", sizeof(idMenuWidget_MenuBar), idMenuWidget_MenuBar_typeInfo },
	{ "idMenuWidget_PDA_UserData", "idMenuWidget", sizeof(idMenuWidget_PDA_UserData), idMenuWidget_PDA_UserData_typeInfo },
	{ "idMenuWidget_ScrollBar", "idMenuWidget", sizeof(idMenuWidget_ScrollBar), idMenuWidget_ScrollBar_typeInfo },
	{ "idMenuWidget_InfoBox", "idMenuWidget", sizeof(idMenuWidget_InfoBox), idMenuWidget_InfoBox_typeInfo },
	{ "idMenuWidget_PDA_Objective", "idMenuWidget", sizeof(idMenuWidget_PDA_Objective), idMenuWidget_PDA_Objective_typeInfo },
	{ "idMenuWidget_Shell_SaveInfo", "idMenuWidget", sizeof(idMenuWidget_Shell_SaveInfo), idMenuWidget_Shell_SaveInfo_typeInfo },
	{ "idMenuWidget_PDA_AudioFiles", "idMenuWidget", sizeof(idMenuWidget_PDA_AudioFiles), idMenuWidget_PDA_AudioFiles_typeInfo },
	{ "idMenuWidget_PDA_EmailInbox", "idMenuWidget", sizeof(idMenuWidget_PDA_EmailInbox), idMenuWidget_PDA_EmailInbox_typeInfo },
	{ "idMenuWidget_ItemAssignment", "idMenuWidget", sizeof(idMenuWidget_ItemAssignment), idMenuWidget_ItemAssignment_typeInfo },
	{ "idMenuWidget_PDA_VideoInfo", "idMenuWidget", sizeof(idMenuWidget_PDA_VideoInfo), idMenuWidget_PDA_VideoInfo_typeInfo },
	{ "idWidgetActionHandler", "idSWFScriptFunction_RefCounted", sizeof(idWidgetActionHandler), idWidgetActionHandler_typeInfo },
	{ "idLBRowBlock", "", sizeof(idLBRowBlock), idLBRowBlock_typeInfo },
	{ "idLBCache", "", sizeof(idLBCache), idLBCache_typeInfo },
	{ "idMenuScreen", "idMenuWidget", sizeof(idMenuScreen), idMenuScreen_typeInfo },
	{ "idMenuScreen_PDA_UserData", "idMenuScreen", sizeof(idMenuScreen_PDA_UserData), idMenuScreen_PDA_UserData_typeInfo },
	{ "idMenuScreen_PDA_UserEmails", "idMenuScreen", sizeof(idMenuScreen_PDA_UserEmails), idMenuScreen_PDA_UserEmails_typeInfo },
	{ "idMenuScreen_PDA_VideoDisks", "idMenuScreen", sizeof(idMenuScreen_PDA_VideoDisks), idMenuScreen_PDA_VideoDisks_typeInfo },
	{ "idMenuScreen_PDA_Inventory", "idMenuScreen", sizeof(idMenuScreen_PDA_Inventory), idMenuScreen_PDA_Inventory_typeInfo },
	{ "idMenuScreen_Shell_Root", "idMenuScreen", sizeof(idMenuScreen_Shell_Root), idMenuScreen_Shell_Root_typeInfo },
	{ "idMenuScreen_Shell_Pause", "idMenuScreen", sizeof(idMenuScreen_Shell_Pause), idMenuScreen_Shell_Pause_typeInfo },
	{ "idMenuScreen_Shell_Singleplayer", "idMenuScreen", sizeof(idMenuScreen_Shell_Singleplayer), idMenuScreen_Shell_Singleplayer_typeInfo },
	{ "idMenuScreen_Shell_Settings", "idMenuScreen", sizeof(idMenuScreen_Shell_Settings), idMenuScreen_Shell_Settings_typeInfo },
	{ "creditInfo_t", "", sizeof(creditInfo_t), creditInfo_t_typeInfo },
	{ "idMenuScreen_Shell_Credits", "idMenuScreen", sizeof(idMenuScreen_Shell_Credits), idMenuScreen_Shell_Credits_typeInfo },
	{ "idMenuScreen_Shell_Resolution::optionData_t", "", sizeof(idMenuScreen_Shell_Resolution::optionData_t), idMenuScreen_Shell_Resolution_optionData_t_typeInfo },
	{ "idMenuScreen_Shell_Resolution", "idMenuScreen", sizeof(idMenuScreen_Shell_Resolution), idMenuScreen_Shell_Resolution_typeInfo },
	{ "idMenuScreen_Shell_Difficulty", "idMenuScreen", sizeof(idMenuScreen_Shell_Difficulty), idMenuScreen_Shell_Difficulty_typeInfo },
	{ "idMenuScreen_Shell_Playstation", "idMenuScreen", sizeof(idMenuScreen_Shell_Playstation), idMenuScreen_Shell_Playstation_typeInfo },
	{ "idMenuScreen_Shell_ModeSelect", "idMenuScreen", sizeof(idMenuScreen_Shell_ModeSelect), idMenuScreen_Shell_ModeSelect_typeInfo },
	{ "idMenuScreen_Shell_GameBrowser", "idMenuScreen", sizeof(idMenuScreen_Shell_GameBrowser), idMenuScreen_Shell_GameBrowser_typeInfo },
	{ "idMenuScreen_Shell_Leaderboards::doomLeaderboard_t", "", sizeof(idMenuScreen_Shell_Leaderboards::doomLeaderboard_t), idMenuScreen_Shell_Leaderboards_doomLeaderboard_t_typeInfo },
	{ "idMenuScreen_Shell_Leaderboards", "idMenuScreen", sizeof(idMenuScreen_Shell_Leaderboards), idMenuScreen_Shell_Leaderboards_typeInfo },
	{ "idMenuScreen_Shell_Bindings", "idMenuScreen", sizeof(idMenuScreen_Shell_Bindings), idMenuScreen_Shell_Bindings_typeInfo },
	{ "idMenuScreen_Shell_Dev::devOption_t", "", sizeof(idMenuScreen_Shell_Dev::devOption_t), idMenuScreen_Shell_Dev_devOption_t_typeInfo },
	{ "idMenuScreen_Shell_Dev", "idMenuScreen", sizeof(idMenuScreen_Shell_Dev), idMenuScreen_Shell_Dev_typeInfo },
	{ "idMenuScreen_Shell_NewGame", "idMenuScreen", sizeof(idMenuScreen_Shell_NewGame), idMenuScreen_Shell_NewGame_typeInfo },
	{ "idMenuScreen_Shell_Load", "idMenuScreen", sizeof(idMenuScreen_Shell_Load), idMenuScreen_Shell_Load_typeInfo },
	{ "idMenuScreen_Shell_Save", "idMenuScreen", sizeof(idMenuScreen_Shell_Save), idMenuScreen_Shell_Save_typeInfo },
	{ "idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings", "idMenuDataSource", sizeof(idMenuScreen_Shell_GameOptions::idMenuDataSource_GameSettings), idMenuScreen_Shell_GameOptions_idMenuDataSource_GameSettings_typeInfo },
	{ "idMenuScreen_Shell_GameOptions", "idMenuScreen", sizeof(idMenuScreen_Shell_GameOptions), idMenuScreen_Shell_GameOptions_typeInfo },
	{ "idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings", "idMenuDataSource", sizeof(idMenuScreen_Shell_MatchSettings::idMenuDataSource_MatchSettings), idMenuScreen_Shell_MatchSettings_idMenuDataSource_MatchSettings_typeInfo },
	{ "idMenuScreen_Shell_MatchSettings", "idMenuScreen", sizeof(idMenuScreen_Shell_MatchSettings), idMenuScreen_Shell_MatchSettings_typeInfo },
	{ "idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings", "idMenuDataSource", sizeof(idMenuScreen_Shell_Controls::idMenuDataSource_ControlSettings), idMenuScreen_Shell_Controls_idMenuDataSource_ControlSettings_typeInfo },
	{ "idMenuScreen_Shell_Controls", "idMenuScreen", sizeof(idMenuScreen_Shell_Controls), idMenuScreen_Shell_Controls_typeInfo },
	{ "idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings", "idMenuDataSource", sizeof(idMenuScreen_Shell_Gamepad::idMenuDataSource_GamepadSettings), idMenuScreen_Shell_Gamepad_idMenuDataSource_GamepadSettings_typeInfo },
	{ "idMenuScreen_Shell_Gamepad", "idMenuScreen", sizeof(idMenuScreen_Shell_Gamepad), idMenuScreen_Shell_Gamepad_typeInfo },
	{ "idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings", "idMenuDataSource", sizeof(idMenuScreen_Shell_ControllerLayout::idMenuDataSource_LayoutSettings), idMenuScreen_Shell_ControllerLayout_idMenuDataSource_LayoutSettings_typeInfo },
	{ "idMenuScreen_Shell_ControllerLayout", "idMenuScreen", sizeof(idMenuScreen_Shell_ControllerLayout), idMenuScreen_Shell_ControllerLayout_typeInfo },
	{ "idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings", "idMenuDataSource", sizeof(idMenuScreen_Shell_SystemOptions::idMenuDataSource_SystemSettings), idMenuScreen_Shell_SystemOptions_idMenuDataSource_SystemSettings_typeInfo },
	{ "idMenuScreen_Shell_SystemOptions", "idMenuScreen", sizeof(idMenuScreen_Shell_SystemOptions), idMenuScreen_Shell_SystemOptions_typeInfo },
	{ "idMenuScreen_Shell_PartyLobby", "idMenuScreen", sizeof(idMenuScreen_Shell_PartyLobby), idMenuScreen_Shell_PartyLobby_typeInfo },
	{ "idMenuScreen_Shell_GameLobby", "idMenuScreen", sizeof(idMenuScreen_Shell_GameLobby), idMenuScreen_Shell_GameLobby_typeInfo },
	{ "idMenuScreen_HUD", "idMenuScreen", sizeof(idMenuScreen_HUD), idMenuScreen_HUD_typeInfo },
	{ "idMenuScreen_Scoreboard", "idMenuScreen", sizeof(idMenuScreen_Scoreboard), idMenuScreen_Scoreboard_typeInfo },
	{ "idMenuScreen_Scoreboard_CTF", "idMenuScreen_Scoreboard", sizeof(idMenuScreen_Scoreboard_CTF), idMenuScreen_Scoreboard_CTF_typeInfo },
	{ "idMenuScreen_Scoreboard_Team", "idMenuScreen_Scoreboard", sizeof(idMenuScreen_Scoreboard_Team), idMenuScreen_Scoreboard_Team_typeInfo },
	{ "actionRepeater_t", "", sizeof(actionRepeater_t), actionRepeater_t_typeInfo },
	{ "mpScoreboardInfo", "", sizeof(mpScoreboardInfo), mpScoreboardInfo_typeInfo },
	{ "idMenuHandler", "", sizeof(idMenuHandler), idMenuHandler_typeInfo },
	{ "lobbyPlayerInfo_t", "", sizeof(lobbyPlayerInfo_t), lobbyPlayerInfo_t_typeInfo },
	{ "idMenuHandler_Shell", "idMenuHandler", sizeof(idMenuHandler_Shell), idMenuHandler_Shell_typeInfo },
	{ "idMenuHandler_PDA", "idMenuHandler", sizeof(idMenuHandler_PDA), idMenuHandler_PDA_typeInfo },
	{ "idMenuHandler_HUD", "idMenuHandler", sizeof(idMenuHandler_HUD), idMenuHandler_HUD_typeInfo },
	{ "idMenuHandler_Scoreboard", "idMenuHandler", sizeof(idMenuHandler_Scoreboard), idMenuHandler_Scoreboard_typeInfo },
	{ "opcode_t", "", sizeof(opcode_t), opcode_t_typeInfo },
	{ "idCompiler", "", sizeof(idCompiler), idCompiler_typeInfo },
	{ "prstack_t", "", sizeof(prstack_t), prstack_t_typeInfo },
	{ "idInterpreter", "", sizeof(idInterpreter), idInterpreter_typeInfo },
	{ "idThread", "idClass", sizeof(idThread), idThread_typeInfo },
	{ NULL, NULL, 0, NULL }
};

#endif /* !__GAMETYPEINFO_H__ */
