// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR CONFIGURATION
// ============================================================================
// Easy-to-edit feature switches. Project33 delegates actual equipment changes
// to WCS_LoadoutEditor and the supplied equipment/content mods through the
// mission adapter. No third-party API is guessed here.
// ============================================================================
class RHD_LoadoutEditorConfig
{
	static const bool ENABLE_LOADOUT_EDITOR = true;
	static const bool ENABLE_WEAPONS = true;
	static const bool ENABLE_OPTICS = true;
	static const bool ENABLE_ATTACHMENTS = true;
	static const bool ENABLE_MAGAZINES = true;
	static const bool ENABLE_THROWABLES = true;
	static const bool ENABLE_EXPLOSIVES = true;
	static const bool ENABLE_MEDICAL = true;
	static const bool ENABLE_TOOLS = true;
	static const bool ENABLE_CLOTHING = true;
	static const bool ENABLE_VESTS = true;
	static const bool ENABLE_RIGS = true;
	static const bool ENABLE_BELTS = true;
	static const bool ENABLE_BAGS = true;
	static const bool ENABLE_DROPLEGS = true;
	static const bool ENABLE_HOLSTER = true;
	static const bool ENABLE_ZEROING = true;
	static const bool ENABLE_ACCESSORIES = true;
	static const bool ENABLE_PATCHES = true;
	static const bool ENABLE_COMMS = true;
	static const bool ENABLE_WCS = true;
	static const bool ENABLE_GRS = true;
	static const bool ENABLE_RHS = true;

	static const int MAX_LOADOUT_SLOTS = 48;
	static const int MAX_LOADOUT_PROFILES = 10;
	static const int MAX_ATTACHMENTS_PER_SLOT = 12;
	static const int MAX_CARRIED_ITEM_STACK = 999;
	static const int MAX_MAGAZINE_STACK = 20;
	static const int MAX_THROWABLE_STACK = 12;
	static const int MAX_EXPLOSIVE_STACK = 8;
	static const int MAX_MEDICAL_STACK = 20;
	static const int MAX_TOOL_STACK = 10;
	static const int MAX_CONTAINER_ENTRIES = 80;
	static const int MAX_CONTAINERS = 8;
	static const int MAX_PROFILE_NAME_LENGTH = 24;

	static const int ZEROING_MIN_METERS = 0;
	static const int ZEROING_MAX_METERS = 1500;
	static const int ZEROING_STEP_METERS = 50;

	static const string PROFILE_DEFAULT = "DEFAULT";
	static const string PROFILE_MEDIC = "MEDIC";
	static const string PROFILE_RIFLEMAN = "RIFLEMAN";
	static const string PROFILE_MACHINEGUNNER = "MACHINEGUNNER";
	static const string PROFILE_MARKSMAN = "MARKSMAN";
	static const string PROFILE_AA_AT = "AA_AT";
	static const string PROFILE_CUSTOM = "CUSTOM";
};

class RHD_LoadoutDependencyCapability
{
	static const string WCS_LOADOUT_EDITOR = "WCS_LoadoutEditor";
	static const string WCS_WEAPONS = "WCS_Weapons";
	static const string WCS_ATTACHMENTS = "WCS_Attachments";
	static const string WCS_SCOPES = "WCS_Scopes";
	static const string WCS_CLOTHING = "WCS_Clothing";
	static const string GRS_APPAREL = "GRS_Apparel";
	static const string GRS_VESTS = "GRS_Modular_Vests_Rigs";
	static const string GRS_BAGS = "GRS_Bags_Belts_Droplegs";
	static const string GRS_PATCHES = "GRS_Patches";
	static const string RHS = "RHS";
	static const string RAYZI_OPTICS = "Rayzis_Optics";
	static const string RIS_LASERS = "RIS_Laser_Attachments";
	static const string ZEROING = "Advanced_Zeroing";
	static const string HOLSTER = "Weapon_Holstering";
};
