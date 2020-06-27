

#pragma once
#include "MiscDefinitions.h"
#include "Vector.h"
#include "Utilities.h"
#include <map>

#define TEAM_CS_T 2
#define TEAM_CS_CT 3
#define BONE_USED_BY_HITBOX			0x00000100
#define BONE_USED_BY_ATTACHMENT      0x00000200    // bone (or child) is used by an attachment point
#define MAXSTUDIOBONES		128		// total bones actually used
#define TICK_INTERVAL			(g_Globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( g_Globals->interval_per_tick *( t ) )
enum class TeamID : int
{
	TEAM_UNASSIGNED,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST,
};

class IClientRenderable;
class IClientNetworkable;
class IClientUnknown;
class IClientThinkable;
class C_BaseEntity;
class CSWeaponInfo;

typedef unsigned long CBaseHandle;

struct RecvProp;

class DVariant
{
public:
	union
	{
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
		float m_Vector[3];
	};
};

class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp;
	char _pad[4];//csgo ( for l4d keep it commented out :) )
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

typedef void(*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

struct RecvTable
{
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

struct RecvProp
{
	char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;
	void* m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};



typedef IClientNetworkable*	(*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn m_pCreateEventFn;
	char			*m_pNetworkName;
	RecvTable		*m_pRecvTable;
	ClientClass		*m_pNext;
	int				m_ClassID;
};

#include "memesets.h"
#include "NetVarManager.h"


struct CHudTexture
{
	char	szShortName[64];	//0x0000
	char	szTextureFile[64];	//0x0040
	bool	bRenderUsingFont;	//0x0080
	bool	bPrecached;			//0x0081
	char	cCharacterInFont;	//0x0082
	BYTE	pad_0x0083;			//0x0083
	int		hFont;				//0x0084
	int		iTextureId;			//0x0088
	float	afTexCoords[4];		//0x008C
	int		iPosX[4];			//0x009C
}; //Size=0x00AC
class CCStrike15ItemDefinition;
class CSWeaponInfo
{
public:
	char _0x0000[20];
	__int32 max_clip;	//0x0014 
	char _0x0018[12];
	__int32 max_reserved_ammo;	//0x0024 
	char _0x0028[96];
	char* hud_name;//0x0088 
	char* weapon_name;	//0x008C 
	char _0x0090[60];
	__int32 type;//0x00CC 
	__int32 price;//0x00D0 
	__int32 reward;//0x00D4 
	char _0x00D8[20];
	BYTE full_auto;//0x00EC 
	char _0x00ED[3];
	__int32 damage;//0x00F0 
	float armor_ratio;	//0x00F4 
	__int32 bullets;	//0x00F8 
	float penetration;	//0x00FC 
	char _0x0100[8];
	float range;//0x0108 
	float range_modifier;//0x010C 
	char _0x0110[16];
	BYTE silencer;//0x0120 
	char _0x0121[15];
	float max_speed;	//0x0130 
	float max_speed_alt;//0x0134 
	char _0x0138[76];
	__int32 recoil_seed;//0x0184 
	char _0x0188[32];
};



enum class ClassID
{
	ClassId_CAI_BaseNPC = 0,
	ClassId_CAK47,
	ClassId_CBaseAnimating,
	ClassId_CBaseAnimatingOverlay,
	ClassId_CBaseAttributableItem,
	ClassId_CBaseButton,
	ClassId_CBaseCombatCharacter,
	ClassId_CBaseCombatWeapon,
	ClassId_CBaseCSGrenade,
	ClassId_CBaseCSGrenadeProjectile,
	ClassId_CBaseDoor,
	ClassId_CBaseEntity,
	ClassId_CBaseFlex,
	ClassId_CBaseGrenade,
	ClassId_CBaseParticleEntity,
	ClassId_CBasePlayer,
	ClassId_CBasePropDoor,
	ClassId_CBaseTeamObjectiveResource,
	ClassId_CBaseTempEntity,
	ClassId_CBaseToggle,
	ClassId_CBaseTrigger,
	ClassId_CBaseViewModel,
	ClassId_CBaseVPhysicsTrigger,
	ClassId_CBaseWeaponWorldModel,
	ClassId_CBeam,
	ClassId_CBeamSpotlight,
	ClassId_CBoneFollower,
	ClassId_CBRC4Target,
	ClassId_CBreachCharge,
	ClassId_CBreachChargeProjectile,
	ClassId_CBreakableProp,
	ClassId_CBreakableSurface,
	ClassId_CC4,
	ClassId_CCascadeLight,
	ClassId_CChicken,
	ClassId_CColorCorrection,
	ClassId_CColorCorrectionVolume,
	ClassId_CCSGameRulesProxy,
	ClassId_CCSPlayer,
	ClassId_CCSPlayerResource,
	ClassId_CCSRagdoll,
	ClassId_CCSTeam,
	ClassId_CDangerZone,
	ClassId_CDangerZoneController,
	ClassId_CDEagle,
	ClassId_CDecoyGrenade,
	ClassId_CDecoyProjectile,
	ClassId_CDrone,
	ClassId_CDronegun,
	ClassId_CDynamicLight,
	ClassId_CDynamicProp,
	ClassId_CEconEntity,
	ClassId_CEconWearable,
	ClassId_CEmbers,
	ClassId_CEntityDissolve,
	ClassId_CEntityFlame,
	ClassId_CEntityFreezing,
	ClassId_CEntityParticleTrail,
	ClassId_CEnvAmbientLight,
	ClassId_CEnvDetailController,
	ClassId_CEnvDOFController,
	ClassId_CEnvGasCanister,
	ClassId_CEnvParticleScript,
	ClassId_CEnvProjectedTexture,
	ClassId_CEnvQuadraticBeam,
	ClassId_CEnvScreenEffect,
	ClassId_CEnvScreenOverlay,
	ClassId_CEnvTonemapController,
	ClassId_CEnvWind,
	ClassId_CFEPlayerDecal,
	ClassId_CFireCrackerBlast,
	ClassId_CFireSmoke,
	ClassId_CFireTrail,
	ClassId_CFish,
	ClassId_CFists,
	ClassId_CFlashbang,
	ClassId_CFogController,
	ClassId_CFootstepControl,
	ClassId_CFunc_Dust,
	ClassId_CFunc_LOD,
	ClassId_CFuncAreaPortalWindow,
	ClassId_CFuncBrush,
	ClassId_CFuncConveyor,
	ClassId_CFuncLadder,
	ClassId_CFuncMonitor,
	ClassId_CFuncMoveLinear,
	ClassId_CFuncOccluder,
	ClassId_CFuncReflectiveGlass,
	ClassId_CFuncRotating,
	ClassId_CFuncSmokeVolume,
	ClassId_CFuncTrackTrain,
	ClassId_CGameRulesProxy,
	ClassId_CGrassBurn,
	ClassId_CHandleTest,
	ClassId_CHEGrenade,
	ClassId_CHostage,
	ClassId_CHostageCarriableProp,
	ClassId_CIncendiaryGrenade,
	ClassId_CInferno,
	ClassId_CInfoLadderDismount,
	ClassId_CInfoMapRegion,
	ClassId_CInfoOverlayAccessor,
	ClassId_CItem_Healthshot,
	ClassId_CItemCash,
	ClassId_CItemDogtags,
	ClassId_CKnife,
	ClassId_CKnifeGG,
	ClassId_CLightGlow,
	ClassId_CMaterialModifyControl,
	ClassId_CMelee,
	ClassId_CMolotovGrenade,
	ClassId_CMolotovProjectile,
	ClassId_CMovieDisplay,
	ClassId_CParadropChopper,
	ClassId_CParticleFire,
	ClassId_CParticlePerformanceMonitor,
	ClassId_CParticleSystem,
	ClassId_CPhysBox,
	ClassId_CPhysBoxMultiplayer,
	ClassId_CPhysicsProp,
	ClassId_CPhysicsPropMultiplayer,
	ClassId_CPhysMagnet,
	ClassId_CPhysPropAmmoBox,
	ClassId_CPhysPropLootCrate,
	ClassId_CPhysPropRadarJammer,
	ClassId_CPhysPropWeaponUpgrade,
	ClassId_CPlantedC4,
	ClassId_CPlasma,
	ClassId_CPlayerResource,
	ClassId_CPointCamera,
	ClassId_CPointCommentaryNode,
	ClassId_CPointWorldText,
	ClassId_CPoseController,
	ClassId_CPostProcessController,
	ClassId_CPrecipitation,
	ClassId_CPrecipitationBlocker,
	ClassId_CPredictedViewModel,
	ClassId_CProp_Hallucination,
	ClassId_CPropCounter,
	ClassId_CPropDoorRotating,
	ClassId_CPropJeep,
	ClassId_CPropVehicleDriveable,
	ClassId_CRagdollManager,
	ClassId_CRagdollProp,
	ClassId_CRagdollPropAttached,
	ClassId_CRopeKeyframe,
	ClassId_CSCAR17,
	ClassId_CSceneEntity,
	ClassId_CSensorGrenade,
	ClassId_CSensorGrenadeProjectile,
	ClassId_CShadowControl,
	ClassId_CSlideshowDisplay,
	ClassId_CSmokeGrenade,
	ClassId_CSmokeGrenadeProjectile,
	ClassId_CSmokeStack,
	ClassId_CSnowball,
	ClassId_CSnowballPile,
	ClassId_CSnowballProjectile,
	ClassId_CSpatialEntity,
	ClassId_CSpotlightEnd,
	ClassId_CSprite,
	ClassId_CSpriteOriented,
	ClassId_CSpriteTrail,
	ClassId_CStatueProp,
	ClassId_CSteamJet,
	ClassId_CSun,
	ClassId_CSunlightShadowControl,
	ClassId_CSurvivalSpawnChopper,
	ClassId_CTablet,
	ClassId_CTeam,
	ClassId_CTeamplayRoundBasedRulesProxy,
	ClassId_CTEArmorRicochet,
	ClassId_CTEBaseBeam,
	ClassId_CTEBeamEntPoint,
	ClassId_CTEBeamEnts,
	ClassId_CTEBeamFollow,
	ClassId_CTEBeamLaser,
	ClassId_CTEBeamPoints,
	ClassId_CTEBeamRing,
	ClassId_CTEBeamRingPoint,
	ClassId_CTEBeamSpline,
	ClassId_CTEBloodSprite,
	ClassId_CTEBloodStream,
	ClassId_CTEBreakModel,
	ClassId_CTEBSPDecal,
	ClassId_CTEBubbles,
	ClassId_CTEBubbleTrail,
	ClassId_CTEClientProjectile,
	ClassId_CTEDecal,
	ClassId_CTEDust,
	ClassId_CTEDynamicLight,
	ClassId_CTEEffectDispatch,
	ClassId_CTEEnergySplash,
	ClassId_CTEExplosion,
	ClassId_CTEFireBullets,
	ClassId_CTEFizz,
	ClassId_CTEFootprintDecal,
	ClassId_CTEFoundryHelpers,
	ClassId_CTEGaussExplosion,
	ClassId_CTEGlowSprite,
	ClassId_CTEImpact,
	ClassId_CTEKillPlayerAttachments,
	ClassId_CTELargeFunnel,
	ClassId_CTEMetalSparks,
	ClassId_CTEMuzzleFlash,
	ClassId_CTEParticleSystem,
	ClassId_CTEPhysicsProp,
	ClassId_CTEPlantBomb,
	ClassId_CTEPlayerAnimEvent,
	ClassId_CTEPlayerDecal,
	ClassId_CTEProjectedDecal,
	ClassId_CTERadioIcon,
	ClassId_CTEShatterSurface,
	ClassId_CTEShowLine,
	ClassId_CTesla,
	ClassId_CTESmoke,
	ClassId_CTESparks,
	ClassId_CTESprite,
	ClassId_CTESpriteSpray,
	ClassId_CTest_ProxyToggle_Networkable,
	ClassId_CTestTraceline,
	ClassId_CTEWorldDecal,
	ClassId_CTriggerPlayerMovement,
	ClassId_CTriggerSoundOperator,
	ClassId_CVGuiScreen,
	ClassId_CVoteController,
	ClassId_CWaterBullet,
	ClassId_CWaterLODControl,
	ClassId_CWeaponAug,
	ClassId_CWeaponAWP,
	ClassId_CWeaponBaseItem,
	ClassId_CWeaponBizon,
	ClassId_CWeaponCSBase,
	ClassId_CWeaponCSBaseGun,
	ClassId_CWeaponCycler,
	ClassId_CWeaponElite,
	ClassId_CWeaponFamas,
	ClassId_CWeaponFiveSeven,
	ClassId_CWeaponG3SG1,
	ClassId_CWeaponGalil,
	ClassId_CWeaponGalilAR,
	ClassId_CWeaponGlock,
	ClassId_CWeaponHKP2000,
	ClassId_CWeaponM249,
	ClassId_CWeaponM3,
	ClassId_CWeaponM4A1,
	ClassId_CWeaponMAC10,
	ClassId_CWeaponMag7,
	ClassId_CWeaponMP5Navy,
	ClassId_CWeaponMP7,
	ClassId_CWeaponMP9,
	ClassId_CWeaponNegev,
	ClassId_CWeaponNOVA,
	ClassId_CWeaponP228,
	ClassId_CWeaponP250,
	ClassId_CWeaponP90,
	ClassId_CWeaponSawedoff,
	ClassId_CWeaponSCAR20,
	ClassId_CWeaponScout,
	ClassId_CWeaponSG550,
	ClassId_CWeaponSG552,
	ClassId_CWeaponSG556,
	ClassId_CWeaponSSG08,
	ClassId_CWeaponTaser,
	ClassId_CWeaponTec9,
	ClassId_CWeaponTMP,
	ClassId_CWeaponUMP45,
	ClassId_CWeaponUSP,
	ClassId_CWeaponXM1014,
	ClassId_CWorld,
	ClassId_CWorldVguiText,
	ClassId_DustTrail,
	ClassId_MovieExplosion,
	ClassId_ParticleSmokeGrenade,
	ClassId_RocketTrail,
	ClassId_SmokeTrail,
	ClassId_SporeExplosion,
	ClassId_SporeTrail,
};

enum CSGOHitboxID : int
{
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm
};

enum class ItemDefinitionIndexx : int
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER
};


struct DefItem_t
{
	DefItem_t(const char* displayName,
		const char* entityName,
		const char* entityModel,
		const char* killIcon = nullptr)
	{
		this->displayName = displayName;
		this->entityName = entityName;
		this->entityModel = entityModel;
		this->killIcon = killIcon;
	}

	const char* displayName = nullptr;
	const char* entityName = nullptr;
	const char* entityModel = nullptr;
	const char* killIcon = nullptr;
};

const std::map<ItemDefinitionIndexx, DefItem_t> ItemDefinitionIndexMap = {
	{ ItemDefinitionIndexx::WEAPON_NONE,{ "<-Default->", "DEFAULT", "", "" } },
	{ ItemDefinitionIndexx::WEAPON_DEAGLE,{ "#SFUI_WPNHUD_DesertEagle", "weapon_deagle", "models/weapons/v_pist_deagle.mdl", "deagle" } },
	{ ItemDefinitionIndexx::WEAPON_ELITE,{ "#SFUI_WPNHUD_Elites", "weapon_elite", "models/weapons/v_pist_elite.mdl", "elite" } },
	{ ItemDefinitionIndexx::WEAPON_FIVESEVEN,{ "#SFUI_WPNHUD_FiveSeven", "weapon_fiveseven", "models/weapons/v_pist_fiveseven.mdl", "fiveseven" } },
	{ ItemDefinitionIndexx::WEAPON_GLOCK,{ "#SFUI_WPNHUD_Glock18", "weapon_glock", "models/weapons/v_pist_glock18.mdl", "glock" } },
	{ ItemDefinitionIndexx::WEAPON_AK47,{ "#SFUI_WPNHUD_AK47", "weapon_ak47", "models/weapons/v_rif_ak47.mdl", "ak47" } },
	{ ItemDefinitionIndexx::WEAPON_AUG,{ "#SFUI_WPNHUD_Aug", "weapon_aug", "models/weapons/v_rif_aug.mdl", "aug" } },
	{ ItemDefinitionIndexx::WEAPON_AWP,{ "#SFUI_WPNHUD_AWP", "weapon_awp", "models/weapons/v_snip_awp.mdl", "awp" } },
	{ ItemDefinitionIndexx::WEAPON_FAMAS,{ "#SFUI_WPNHUD_Famas", "weapon_famas", "models/weapons/v_rif_famas.mdl", "famas" } },
	{ ItemDefinitionIndexx::WEAPON_G3SG1,{ "#SFUI_WPNHUD_G3SG1", "weapon_g3sg1", "models/weapons/v_snip_g3sg1.mdl", "g3sg1" } },
	{ ItemDefinitionIndexx::WEAPON_GALILAR,{ "#SFUI_WPNHUD_GalilAR", "weapon_galilar", "models/weapons/v_rif_galilar.mdl", "galilar" } },
	{ ItemDefinitionIndexx::WEAPON_M249,{ "#SFUI_WPNHUD_M249", "weapon_m249", "models/weapons/v_mach_m249para.mdl", "m249" } },
	{ ItemDefinitionIndexx::WEAPON_M4A1,{ "#SFUI_WPNHUD_M4A1", "weapon_m4a1", "models/weapons/v_rif_m4a1.mdl", "m4a1" } },
	{ ItemDefinitionIndexx::WEAPON_MAC10,{ "#SFUI_WPNHUD_MAC10", "weapon_mac10", "models/weapons/v_smg_mac10.mdl", "mac10" } },
	{ ItemDefinitionIndexx::WEAPON_P90,{ "#SFUI_WPNHUD_P90", "weapon_p90", "models/weapons/v_smg_p90.mdl", "p90" } },
	{ ItemDefinitionIndexx::WEAPON_UMP45,{ "#SFUI_WPNHUD_UMP45", "weapon_ump45", "models/weapons/v_smg_ump45.mdl", "ump45" } },
	{ ItemDefinitionIndexx::WEAPON_XM1014,{ "#SFUI_WPNHUD_xm1014", "weapon_xm1014", "models/weapons/v_shot_xm1014.mdl", "xm1014" } },
	{ ItemDefinitionIndexx::WEAPON_BIZON,{ "#SFUI_WPNHUD_Bizon", "weapon_bizon", "models/weapons/v_smg_bizon.mdl", "bizon" } },
	{ ItemDefinitionIndexx::WEAPON_MAG7,{ "#SFUI_WPNHUD_Mag7", "weapon_mag7", "models/weapons/v_shot_mag7.mdl", "mag7" } },
	{ ItemDefinitionIndexx::WEAPON_NEGEV,{ "#SFUI_WPNHUD_Negev", "weapon_negev", "models/weapons/v_mach_negev.mdl", "negev" } },
	{ ItemDefinitionIndexx::WEAPON_SAWEDOFF,{ "#SFUI_WPNHUD_Sawedoff", "weapon_sawedoff", "models/weapons/v_shot_sawedoff.mdl", "sawedoff" } },
	{ ItemDefinitionIndexx::WEAPON_TEC9,{ "#SFUI_WPNHUD_Tec9", "weapon_tec9", "models/weapons/v_pist_tec9.mdl", "tec9" } },
	{ ItemDefinitionIndexx::WEAPON_TASER,{ "#SFUI_WPNHUD_Taser", "weapon_taser", "models/weapons/v_eq_taser.mdl", "taser" } },
	{ ItemDefinitionIndexx::WEAPON_HKP2000,{ "#SFUI_WPNHUD_HKP2000", "weapon_hkp2000", "models/weapons/v_pist_hkp2000.mdl", "hkp2000" } },
	{ ItemDefinitionIndexx::WEAPON_MP7,{ "#SFUI_WPNHUD_MP7", "weapon_mp7", "models/weapons/v_smg_mp7.mdl", "mp7" } },
	{ ItemDefinitionIndexx::WEAPON_MP9,{ "#SFUI_WPNHUD_MP9", "weapon_mp9", "models/weapons/v_smg_mp9.mdl", "mp9" } },
	{ ItemDefinitionIndexx::WEAPON_NOVA,{ "#SFUI_WPNHUD_Nova", "weapon_nova", "models/weapons/v_shot_nova.mdl", "nova" } },
	{ ItemDefinitionIndexx::WEAPON_P250,{ "#SFUI_WPNHUD_P250", "weapon_p250", "models/weapons/v_pist_p250.mdl", "p250" } },
	{ ItemDefinitionIndexx::WEAPON_SCAR20,{ "#SFUI_WPNHUD_SCAR20", "weapon_scar20", "models/weapons/v_snip_scar20.mdl", "scar20" } },
	{ ItemDefinitionIndexx::WEAPON_SG556,{ "#SFUI_WPNHUD_SG556", "weapon_sg556", "models/weapons/v_rif_sg556.mdl", "sg556" } },
	{ ItemDefinitionIndexx::WEAPON_SSG08,{ "#SFUI_WPNHUD_SSG08", "weapon_ssg08", "models/weapons/v_snip_ssg08.mdl", "ssg08" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE,{ "#SFUI_WPNHUD_Knife", "weapon_knife", "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
	{ ItemDefinitionIndexx::WEAPON_FLASHBANG,{ "#SFUI_WPNHUD_FLASHBANG", "weapon_flashbang", "models/weapons/v_eq_flashbang.mdl", "flashbang" } },
	{ ItemDefinitionIndexx::WEAPON_HEGRENADE,{ "#SFUI_WPNHUD_HE_Grenade", "weapon_hegrenade", "models/weapons/v_eq_fraggrenade.mdl", "hegrenade" } },
	{ ItemDefinitionIndexx::WEAPON_SMOKEGRENADE,{ "#SFUI_WPNHUD_Smoke_Grenade", "weapon_smokegrenade", "models/weapons/v_eq_smokegrenade.mdl", "smokegrenade" } },
	{ ItemDefinitionIndexx::WEAPON_MOLOTOV,{ "#SFUI_WPNHUD_MOLOTOV", "weapon_molotov", "models/weapons/v_eq_molotov.mdl", "inferno" } },
	{ ItemDefinitionIndexx::WEAPON_DECOY,{ "#SFUI_WPNHUD_DECOY", "weapon_decoy", "models/weapons/v_eq_decoy.mdl", "decoy" } },
	{ ItemDefinitionIndexx::WEAPON_INCGRENADE,{ "#SFUI_WPNHUD_IncGrenade", "weapon_incgrenade", "models/weapons/v_eq_incendiarygrenade.mdl", "inferno" } },
	{ ItemDefinitionIndexx::WEAPON_C4,{ "#SFUI_WPNHUD_C4", "weapon_c4", "models/weapons/v_ied.mdl" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_T,{ "#SFUI_WPNHUD_Knife", "weapon_knife_t", "models/weapons/v_knife_default_t.mdl", "knife_t" } },
	{ ItemDefinitionIndexx::WEAPON_M4A1_SILENCER,{ "#SFUI_WPNHUD_M4_SILENCER", "weapon_m4a1_silencer", "models/weapons/v_rif_m4a1_s.mdl", "m4a1_silencer" } },
	{ ItemDefinitionIndexx::WEAPON_USP_SILENCER,{ "#SFUI_WPNHUD_USP_SILENCER", "weapon_usp_silencer", "models/weapons/v_pist_223.mdl", "usp_silencer" } },
	{ ItemDefinitionIndexx::WEAPON_CZ75A,{ "#SFUI_WPNHUD_CZ75", "weapon_cz75a", "models/weapons/v_pist_cz_75.mdl", "cz75a" } },
	{ ItemDefinitionIndexx::WEAPON_REVOLVER,{ "#SFUI_WPNHUD_REVOLVER", "weapon_revolver", "models/weapons/v_pist_revolver.mdl", "revolver" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_BAYONET,{ "#SFUI_WPNHUD_KnifeBayonet", "weapon_knife_bayonet", "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_FLIP,{ "#SFUI_WPNHUD_KnifeFlip", "weapon_knife_flip", "models/weapons/v_knife_flip.mdl", "knife_flip" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_GUT,{ "#SFUI_WPNHUD_KnifeGut", "weapon_knife_gut", "models/weapons/v_knife_gut.mdl", "knife_gut" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_KARAMBIT,{ "#SFUI_WPNHUD_KnifeKaram", "weapon_knife_karambit", "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_M9_BAYONET,{ "#SFUI_WPNHUD_KnifeM9", "weapon_knife_m9_bayonet", "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_TACTICAL,{ "#SFUI_WPNHUD_KnifeTactical", "weapon_knife_tactical", "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_FALCHION,{ "#SFUI_WPNHUD_knife_falchion_advanced", "weapon_knife_falchion", "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_SURVIVAL_BOWIE,{ "#SFUI_WPNHUD_knife_survival_bowie", "weapon_knife_survival_bowie", "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_BUTTERFLY,{ "#SFUI_WPNHUD_Knife_Butterfly", "weapon_knife_butterfly", "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_PUSH,{ "#SFUI_WPNHUD_knife_push", "weapon_knife_push", "models/weapons/v_knife_push.mdl", "knife_push" } }
	
};

class CBaseAttributableItem
{
public:

	short* ItemDefinitionIndex()
	{
		return (short*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_iItemDefinitionIndex);
	}
	short GetItemDefinitionIndex()
	{
		return *(short*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_iItemDefinitionIndex);
	}
	int* ItemIDHigh()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_iItemIDHigh); // in DT_BaseAttributableItem -> m_iItemIDHigh 
	}
	int* ItemIDLow()
	{
		return (int*)((uintptr_t)this + 0x1F4);
	}

	int* FallbackPaintKit()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_nFallbackPaintKit);
	}

	int* FallbackSeed()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_nFallbackSeed);
	}

	float* FallbackWear()
	{
		return (float*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_flFallbackWear);
	}

	int* FallbackStatTrak()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_nFallbackStatTrak);
	}

	int* OwnerXuidLow()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_OriginalOwnerXuidLow);
	}
	int* OwnerXuidHigh()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_OriginalOwnerXuidHigh);
	}
	char* GetCustomName()
	{
		return (char*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_szCustomName);
	}
	int* GetEntityQuality() {
		return (int*)((DWORD)this + offsetz.DT_BaseAttributableItem.m_iEntityQuality);
	}

};

class AttributeContainer
{
public:

	CBaseAttributableItem* m_Item()
	{
		return (CBaseAttributableItem*)((uintptr_t)this + 0x40);
	}
};

typedef CSWeaponInfo& (__thiscall* GetCSWpnDataFn)(void*);

class CBaseViewModel {
public:
	inline int GetModelIndex() {

		return *(int*)((DWORD)this + offsetz.DT_BaseViewModel.m_nModelIndex);
	}

	inline void SetModelIndex(int nModelIndex) {
		*(int*)((DWORD)this + offsetz.DT_BaseViewModel.m_nModelIndex) = nModelIndex;

	}

	inline DWORD GetOwner() {

		return *(PDWORD)((DWORD)this + offsetz.DT_BaseViewModel.m_hOwner);
	}

	inline DWORD GetWeapon() {

		return *(PDWORD)((DWORD)this + offsetz.DT_BaseViewModel.m_hWeapon);
	}
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

class CBaseCombatWeapon : public CBaseAttributableItem
{
public:
	float m_flPostponeFireReadyTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_WeaponCSBase.m_flPostponeFireReadyTime);
	}

	HANDLE m_hWeaponWorldModel()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_hWeaponWorldModel);
	}
	int ammo()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iClip1);
	}
	int ammo2()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iReserve);
	}
	int* AccountID()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_iAccountID);
	}
	float GetNextPrimaryAttack()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_flNextPrimaryAttack);
	}

	/*int GetOwnerHandle()
	{
	return *(int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_hOwner);
	}*/

	Vector GetOrigin() // GetVecOrigin
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecOrigin);
	}


	int* ViewModelIndex()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iViewModelIndex);
	}

	int* WorldModelIndex()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iWorldModelIndex);
	}

	int* ModelIndex()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseViewModel.m_nModelIndex);
	}



	AttributeContainer* m_AttributeManager()
	{
		return (AttributeContainer*)((uintptr_t)this + 0x2D70);
	}




	float GetInaccuracy() {
		if (!this) return 0;
		typedef float(__thiscall* tFunc)(void*);
		return call_vfunc<tFunc>(this, 471)(this);
	}


	float GetSpread() {
		if (!this) return 0;
		typedef float(__thiscall* tFunc)(void*);
		return call_vfunc<tFunc>(this, 440)(this);
	}


	void UpdateAccuracyPenalty(CBaseCombatWeapon *pWeapon) {
		DWORD dwUpdateVMT = (*reinterpret_cast< PDWORD_PTR* >(pWeapon))[472];
		return ((void(__thiscall*)(CBaseCombatWeapon*)) dwUpdateVMT)(pWeapon);
	}


	CSWeaponInfo* GetCSWpnData()
	{
		if (!this)
			return nullptr;
		typedef CSWeaponInfo*(__thiscall* tGetCSWpnData)(void*);
		return call_vfunc<tGetCSWpnData>(this, 448)(this);
	}

	bool IsInReload()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_bInReload);
	}

	void Release()
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID);
		return call_vfunc<OriginalFn>(pNetworkable, 1)(pNetworkable);
	}
	void SetDestroyedOnRecreateEntities(void)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID);
		return call_vfunc<OriginalFn>(pNetworkable, 13)(pNetworkable);
	}

	bool IsKnife()
	{
		int iWeaponID = this->GetItemDefinitionIndex();
		return (iWeaponID == (int)ItemDefinitionIndexx::WEAPON_KNIFE_T || iWeaponID == (int)ItemDefinitionIndexx::WEAPON_KNIFE
			|| iWeaponID == 59 || iWeaponID == 41
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}

	char* GetGunName()
	{
		int WeaponId = this->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
		if (this->IsKnife())
			return "Knife";
		switch (WeaponId)
		{
		case (int)ItemDefinitionIndexx::WEAPON_AK47:
			return "AK-47";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_AUG:
			return "AUG";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_REVOLVER:
			return "R8 Revolver";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_AWP:
			return "AWP";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_BIZON:
			return "Bizon";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_C4:
			return "Bomb";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_CZ75A:
			return "CZ75";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_DEAGLE:
			return "Desert Eagle";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_DECOY:
			return "Decoy";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_ELITE:
			return "Dual Elites";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FAMAS:
			return "Famas";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FIVESEVEN:
			return "FiveSeveN";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FLASHBANG:
			return "Flash Bang";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_G3SG1:
			return "G3SG1";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_GALILAR:
			return "Galil";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_GLOCK:
			return "Glock";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_INCGRENADE:
			return "Incendiary";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MOLOTOV:
			return "Molotov";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SSG08:
			return "Scout";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_HEGRENADE:
			return "Grenade";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M249:
			return "M249";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M4A1:
			return "M4A1";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MAC10:
			return "MAC-10";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MAG7:
			return "MAG-7";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MP7:
			return "MP7";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MP9:
			return "MP9";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_NOVA:
			return "Nova";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_NEGEV:
			return "Negev";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_P250:
			return "P250";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_P90:
			return "P90";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SAWEDOFF:
			return "SawedOff";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SCAR20:
			return "SCAR-20";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SMOKEGRENADE:
			return "Smoke";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SG556:
			return "SG556";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_TEC9:
			return "TEC9";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_HKP2000:
			return "P2000";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_USP_SILENCER:
			return "USP-S";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_UMP45:
			return "UMP-45";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_XM1014:
			return "XM1014";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_TASER:
			return "Zeus";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M4A1_SILENCER:
			return "M4A1-S";
		}
		return "";
	}

	char* GetGunIcon()
	{
		int WeaponId = this->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
		if (this->IsKnife())
			return "]";
		switch (WeaponId)
		{
		case (int)ItemDefinitionIndexx::WEAPON_AK47:
			return "W";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_AUG:
			return "U";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_REVOLVER:
			return "J";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_AWP:
			return "Z";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_BIZON:
			return "M";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_C4:
			return "o";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_CZ75A:
			return "I";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_DEAGLE:
			return "A";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_DECOY:
			return "m";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_ELITE:
			return "B";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FAMAS:
			return "R";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FIVESEVEN:
			return "C";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FLASHBANG:
			return "i";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_G3SG1:
			return "X";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_GALILAR:
			return "Q";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_GLOCK:
			return "D";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_INCGRENADE:
			return "n";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MOLOTOV:
			return "m";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SSG08:
			return "a";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_HEGRENADE:
			return "j";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M249:
			return "g";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M4A1:
			return "T";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MAC10:
			return "K";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MAG7:
			return "d";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MP7:
			return "N";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MP9:
			return "O";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_NOVA:
			return "e";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_NEGEV:
			return "f";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_P250:
			return "F";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_P90:
			return "P";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SAWEDOFF:
			return "c";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SCAR20:
			return "Y";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SMOKEGRENADE:
			return "k";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SG556:
			return "V";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_TEC9:
			return "H";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_HKP2000:
			return "E";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_USP_SILENCER:
			return "G";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_UMP45:
			return "L";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_XM1014:
			return "b";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_TASER:
			return "h";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M4A1_SILENCER:
			return "S";
		}
		return "";
	}
};
class CGloves : public CBaseCombatWeapon {
public:

	void PreDataUpdate(int updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}

	void SetGloveModelIndex(int modelIndex)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		call_vfunc<OriginalFn>(this, 75)(this, modelIndex);
	}
};



class CCSBomb
{
public:

	HANDLE GetOwnerHandle()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_hOwner);
	}

	float GetC4BlowTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_PlantedC4.m_flC4Blow);
	}

	float GetC4DefuseCountDown()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_PlantedC4.m_flDefuseCountDown);
	}

	int GetBombDefuser()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlantedC4.m_hBombDefuser);
	}

	bool IsBombDefused()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_PlantedC4.m_bBombDefused);
	}
};


class CCSGrenade
{
public:

	float GetThrowTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseCSGrenade.m_fThrowTime);
	}
};

class CLocalPlayerExclusive
{
public:

	Vector GetViewPunchAngle()
	{
		return *(Vector*)((uintptr_t)this + 0x64);
	}
	Vector GetAimPunchAngle()
	{
		return *(Vector*)((uintptr_t)this + 0x70);
	}
	Vector GetAimPunchAngleVel()
	{
		return *(Vector*)((uintptr_t)this + 0x7C);
	}
};

class CollisionProperty
{
public:

	Vector GetMins()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecMins);
	}
	Vector GetMaxs()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecMaxs);
	}

	unsigned char GetSolidType()
	{
		return *(unsigned char*)((uintptr_t)this + offsetz.DT_BaseEntity.m_nSolidType);
	}

	unsigned short GetSolidFlags()
	{
		return *(unsigned short*)((uintptr_t)this + offsetz.DT_BaseEntity.m_usSolidFlags);
	}

	int GetSurroundsType()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseEntity.m_nSurroundType);
	}


	bool IsSolid()
	{
		return (GetSolidType() != SOLID_NONE) && ((GetSolidFlags() & FSOLID_NOT_SOLID) == 0);
	}
};

class IClientRenderable
{
public:
	//virtual void*					GetIClientUnknown() = 0;
	virtual Vector const&			GetRenderOrigin(void) = 0;
	virtual Vector const&			GetRenderAngles(void) = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual bool					IsTransparent(void) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void					GetShadowHandle() const = 0;
	virtual void*					RenderHandle() = 0;
	virtual const model_t*				GetModel() const = 0;
	virtual int						DrawModel(int flags) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBlend() = 0;



	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{

		typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4*, int, int, float);
		return call_vfunc< oSetupBones>(this, 13)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	}


};


class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/) = 0;
	virtual void			OnPreDataChanged(DataUpdateType_t updateType) = 0;
	virtual void			OnDataChanged(DataUpdateType_t updateType) = 0;
	virtual void			PreDataUpdate(DataUpdateType_t updateType) = 0;
	virtual void			PostDataUpdate(DataUpdateType_t updateType) = 0;
	virtual void			unknown();
	virtual bool			IsDormant(void) = 0;
	virtual int				GetIndex(void) const = 0;
	virtual void			ReceiveMessage(int classID /*, bf_read &msg*/) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};
class ICollideable
{
public:
	virtual void pad0();
	virtual const Vector& OBBMins() const;
	virtual const Vector& OBBMaxs() const;
};

class IClientUnknown
{
public:
	ICollideable* GetCollideable()
	{
		return (ICollideable*)((DWORD)this + 0x31C);
	}
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual C_BaseEntity*		GetIClientEntity() = 0;
	virtual C_BaseEntity*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};


class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};



enum class ObserverMode_t : int
{
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM = 1,
	OBS_MODE_FREEZECAM = 2,
	OBS_MODE_FIXED = 3,
	OBS_MODE_IN_EYE = 4,
	OBS_MODE_CHASE = 5,
	OBS_MODE_ROAMING = 6
};

enum EntityShape_t : int
{
	SHAPE_CIRCLE,
	SHAPE_SQUARE,
	SHAPE_TRIANGLE,
	SHAPE_TRIANGLE_UPSIDEDOWN
};

class animation_layer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038



class __declspec (novtable)C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
public:
	virtual void			Release(void) = 0;
	virtual void			blahblahpad(void) = 0;
	virtual Vector&	GetAbsOrigin(void) const = 0;
	virtual const Vector&	GetAbsAngles(void) const = 0;

	CUtlVector<animation_layer> GetAnimationOverlay() {
		return (CUtlVector<animation_layer>)((DWORD)this + 0x2980);
	}

	CLocalPlayerExclusive* localPlayerExclusive()
	{
		return (CLocalPlayerExclusive*)((uintptr_t)this + 0x2FBC);
	}
	CollisionProperty* collisionProperty()
	{
		return (CollisionProperty*)((uintptr_t)this + 0x474);
	}

	float GetSimulationTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseEntity.m_flSimulationTime);
	}

	float GetAnimTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseEntity.m_flAnimTime);
	}

	HANDLE m_hViewModel()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BasePlayer.m_hViewModel);
	}

	void SetAngle2(Vector wantedang) {
		typedef void(__thiscall* oSetAngle)(void*, const Vector &);
		static oSetAngle _SetAngle = (oSetAngle)((uintptr_t)U::pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		_SetAngle(this, wantedang);
	}
	int m_iShotsFired()
	{
		static int meme = NetVarManager->GetOffset("DT_CSPlayer", "m_iShotsFired");
		return *(int*)((uintptr_t)this + 0xA370); // meme offseti
	}
	float GetNextAttack()
	{
		return *(float*)((DWORD)this + 0x3218);
	}
	Vector GetAbsOrigin2()
	{
		return *(Vector*)((DWORD)this + 0x84 + 0x1C);
	}
	int GetHitboxSet()
	{
		return *(int*)((DWORD)this + offsetz.DT_BaseAnimating.m_nHitboxSet);
	}

	bool hasDefuser()
	{
		return *(bool*)((DWORD)this + offsetz.DT_CSPlayer.m_bHasDefuser);
	}
	bool m_bHasHeavyArmor()
	{
		return *(bool*)((DWORD)this + offsetz.DT_CSPlayer.m_bHasHeavyArmor);
	}
	int GetFlags()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_fFlags);
	}

	Vector GetOrigin()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecOrigin);
	}



	int GetOwnerHandle()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseEntity.m_hOwner);
	}

	Vector GetRotation()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_angRotation);
	}

	int GetTeamNum()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xF4); // hardcode
	}

	int GetMaxHealth()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_iMaxHealth);
	}

	int GetHealth()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_iHealth);
	}

	unsigned char GetLifeState()
	{
		return *(unsigned char*)((uintptr_t)this + offsetz.DT_BasePlayer.m_lifeState);
	}

	char* GetCallout()
	{
		return (char*)((uintptr_t)this + offsetz.DT_BasePlayer.m_szLastPlaceName);
	}

	HANDLE GetActiveWeaponHandle()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BaseCombatCharacter.m_hActiveWeapon);
	}

	CBaseHandle* m_hMyWeapons()
	{
		return (CBaseHandle*)((uintptr_t)this + offsetz.DT_BaseCombatCharacter.m_hMyWeapons);
	}

	int GetTickBase()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_nTickBase);
	}

	Vector GetViewOffset()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BasePlayer.m_vecViewOffset);
	}

	Vector GetVelocity()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BasePlayer.m_vecVelocity);
	}

	bool HasGunGameImmunity()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bGunGameImmunity);
	}

	bool IsDefusing()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bIsDefusing);
	}

	int ArmorValue()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayer.m_ArmorValue);
	}

	int iAccount()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayer.m_iAccount);
	}

	char* GetArmorName()
	{
		if (ArmorValue() > 0)
		{
			if (HasHelmet())
				return "Zirh + Kask";
			else
				return "Zirh";
		}
		else
			return "";
	}


	bool HasHelmet()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bHasHelmet);
	}

	bool IsScoped()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bIsScoped);
	}

	float GetFlashDuration() {
		return *(float*)((DWORD)this + offsetz.DT_CSPlayer.m_flFlashDuration);
	}

	bool IsFlashed()
	{
		return GetFlashDuration() > 0;
	}

	int GetMoney()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayer.m_iAccount);
	}

	HANDLE GetObserverTargetHandle()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BasePlayer.m_hObserverTarget);
	}






	void* GetObserverTarget()
	{
		return (void*)((uintptr_t)this + offsetz.DT_BasePlayer.m_hObserverTarget);
	}

	ObserverMode_t* GetObserverMode()
	{
		return (ObserverMode_t*)((uintptr_t)this + offsetz.DT_BasePlayer.m_iObserverMode);
	}

	CBaseHandle* GetWearables()
	{
		if (!this) return nullptr;
		return reinterpret_cast<CBaseHandle*>(this + offsetz.DT_BaseCombatCharacter.m_hMyWearables);
	}
	int GetMoveType() {
		return *(int*)((DWORD)this + offsetz.DT_BaseEntity.m_MoveType);
	}

	float GetLowerBodyYaw() {
		return *(float*)((DWORD)this + offsetz.DT_CSPlayer.m_flLowerBodyYawTarget);
	}

	QAngle* GetEyeAngles()
	{
		return (QAngle*)((uintptr_t)this + offsetz.DT_CSPlayer.m_angEyeAngles[0]);
	}

	IClientRenderable* GetRenderable()
	{
		return reinterpret_cast<IClientRenderable*>((DWORD)this + 0x70);
	}

	int draw_model(int flags, uint8_t alpha) {
		using fn = int(__thiscall*)(void*, int, uint8_t);
		return call_vfunc< fn >(GetRenderable(), 9)(GetRenderable(), flags, alpha);
	}

	bool IsAlive()
	{
		return (GetLifeState() == LIFE_ALIVE && GetHealth() > 0);
	}
	bool IsPlayer()
	{
		typedef bool(__thiscall *IsPlayer_t)(PVOID);
		return ((IsPlayer_t)(*(PDWORD)(*(PDWORD)(this) + 0x180)))(this);
	}
	Vector GetBonePos(int i)
	{
		matrix3x4 boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, static_cast<float>(GetTickCount64())))
		{
			return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	}

	Vector GetHeadPos()
	{
		return this->GetBonePos(8);
	}
	Vector GetEyePosition() {
		Vector v_origin = *(Vector*)((DWORD)this + offsetz.DT_BaseEntity.m_vecOrigin);
		Vector v_view = *(Vector*)((DWORD)this + offsetz.DT_BasePlayer.m_vecViewOffset);
		return(v_origin + v_view);
	}
};



class CPlayerResource {};

class C_CSPlayerResource : public CPlayerResource
{
public:
	int GetPing(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iPing + index * 4);
	}

	int GetKills(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iKills + index * 4);
	}

	int GetAssists(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iAssists + index * 4);
	}

	int GetDeaths(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iDeaths + index * 4);
	}

	bool GetConnected(int index)
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_PlayerResource.m_bConnected + index);
	}

	TeamID GetTeam(int index)
	{
		return *(TeamID*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iTeam + index * 4);
	}

	int GetPendingTeam(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iPendingTeam + index * 4);
	}

	bool GetAlive(int index)
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_PlayerResource.m_bAlive + index);
	}

	int GetHealth(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iHealth + index * 4);
	}

	int GetPlayerC4()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iPlayerC4);
	}

	int GetMVPs(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iMVPs + index * 4);
	}

	int GetArmor(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iArmor + index * 4);
	}

	int GetScore(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iScore + index * 4);
	}

	int* GetCompetitiveRanking(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iCompetitiveRanking + index * 4);
	}

	int* GetCompetitiveWins(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iCompetitiveWins + index * 4);
	}

	int GetCompTeammateColor(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iCompTeammateColor + index * 4);
	}

	const char* GetClan(int index)
	{
		return (const char*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_szClan + index * 16);
	}

	int* GetActiveCoinRank(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nActiveCoinRank + index * 4);
	}

	int* GetMusicID(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nMusicID + index * 4);
	}

	int* GetPersonaDataPublicCommendsLeader(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsLeader + index * 4);
	}

	int* GetPersonaDataPublicCommendsTeacher(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsTeacher + index * 4);
	}

	int* GetPersonaDataPublicCommendsFriendly(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsFriendly + index * 4);
	}
};

class C_CSGameRules
{
public:
	bool IsValveDS()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bIsValveDS);
	}

	bool IsBombDropped()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bBombDropped);
	}

	bool IsBombPlanted()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bBombPlanted);
	}

	bool IsFreezeTime()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bFreezePeriod);
	}
};

class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};

class IGameConsole : public IBaseInterface
{
public:
	// activates the console, makes it visible and brings it to the foreground
	virtual void Activate() = 0;

	virtual void Initialize() = 0;

	// hides the console
	virtual void Hide() = 0;

	// clears the console
	virtual void Clear() = 0;

	// return true if the console has focus
	virtual bool IsConsoleVisible() = 0;

	virtual void SetParent(int parent) = 0;
};

