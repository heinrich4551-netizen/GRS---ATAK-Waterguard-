// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR MISSION ADAPTER
// ============================================================================
// Wire these methods to the installed WCS_LoadoutEditor and the mission's
// inventory/equipment components. The defaults are fail-safe.
//
// Intended ownership:
// WCS_LoadoutEditor : arsenal/loadout editing workflow
// WCS_Weapons       : WCS weapons
// WCS_Attachments   : WCS attachments
// WCS_Scopes        : WCS scopes/optics
// WCS_Clothing      : WCS clothing
// GRS Apparel       : apparel
// GRS Modular Vests : vests/rigs
// GRS Belts/Bags    : belts/bags/droplegs
// RHS packs        : RHS weapons/clothing/content
// Rayzis Optics     : optics
// RIS Laser        : lasers/lights
// Weapon Holstering : holster state/behavior
// Advanced Zeroing : zeroing behavior
// ============================================================================
class RHD_LoadoutEditorMissionAdapter
{
	bool GetCurrentLoadout(RHD_LoadoutProfile profile) { return false; }
	bool EnumerateItems(string category, out array<string> itemIds) { itemIds = {}; return false; }
	bool IsItemAllowed(string category, string itemId) { return false; }
	bool CanAttach(string weaponItemId, string attachmentId) { return false; }
	bool ApplyLoadout(RHD_LoadoutProfile profile) { return false; }
	bool SaveLoadoutProfile(string profileId, RHD_LoadoutProfile profile) { return false; }
	bool LoadLoadoutProfile(string profileId, RHD_LoadoutProfile profile) { return false; }
	bool DeleteLoadoutProfile(string profileId) { return false; }
	bool SetWeaponZeroing(string weaponItemId, int meters) { return false; }
	bool SetHolsterState(string state) { return false; }
	bool OpenWCSLoadoutEditor() { return false; }

	// Dependency availability is reported separately so the UI can explain why
	// an option is unavailable instead of silently failing.
	bool HasWCSLoadoutEditor() { return false; }
	bool HasWCSWeapons() { return false; }
	bool HasWCSAttachments() { return false; }
	bool HasWCSScopes() { return false; }
	bool HasWCSClothing() { return false; }
	bool HasGRSApparel() { return false; }
	bool HasGRSVestsRigs() { return false; }
	bool HasGRSBagsBeltsDroplegs() { return false; }
	bool HasRHS() { return false; }
	bool HasRayziOptics() { return false; }
	bool HasRISLasers() { return false; }
	bool HasHolstering() { return false; }
	bool HasZeroing() { return false; }
};
