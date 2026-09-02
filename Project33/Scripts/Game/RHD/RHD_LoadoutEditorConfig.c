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
	static const bool ENABLE_CLOTHING = true;
	static const bool ENABLE_VESTS = true;
	static const bool ENABLE_RIGS = true;
	static const bool ENABLE_BELTS = true;
	static const bool ENABLE_BAGS = true;
	static const bool ENABLE_DROPLEGS = true;
	static const bool ENABLE_HOLSTER = true;
	static const bool ENABLE_ZEROING = true;
	static const bool ENABLE_ACCESSORIES = true;
	static const bool ENABLE_WCS = true;
	static const bool ENABLE_GRS = true;
	static const bool ENABLE_RHS = true;

	static const int MAX_LOADOUT_SLOTS = 24;
	static const int MAX_LOADOUT_PROFILES = 10;
	static const int MAX_ATTACHMENTS_PER_SLOT = 12;

	static const string PROFILE_DEFAULT = "DEFAULT";
	static const string PROFILE_MEDIC = "MEDIC";
	static const string PROFILE_RIFLEMAN = "RIFLEMAN";
	static const string PROFILE_MACHINEGUNNER = "MACHINEGUNNER";
	static const string PROFILE_MARKSMAN = "MARKSMAN";
	static const string PROFILE_AA_AT = "AA_AT";
	static const string PROFILE_CUSTOM = "CUSTOM";
};

// Dependency capability names are descriptive IDs used by the UI/adapter.
// Weight and stamina mods are behavior systems, not loadout slots, so the
// editor leaves those mechanics to the source mods.
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
	static const string RHS = "RHS";
	static const string RAYZI_OPTICS = "Rayzis_Optics";
	static const string RIS_LASERS = "RIS_Laser_Attachments";
	static const string ZEROING = "Advanced_Zeroing";
	static const string HOLSTER = "Weapon_Holstering";
};
