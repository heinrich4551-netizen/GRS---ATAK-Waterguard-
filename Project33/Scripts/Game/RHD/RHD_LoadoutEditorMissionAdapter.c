// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR MISSION ADAPTER
// ============================================================================
// This is the authoritative integration boundary for the loadout editor.
// The editor can expose deep controls without cloning WCS_LoadoutEditor,
// Reforger inventory, or the supplied gear/content implementations.
//
// Intended ownership:
// WCS_LoadoutEditor : arsenal/loadout workflow and saved WCS slots
// WCS_Weapons       : WCS weapons and magazines
// WCS_Attachments   : WCS grips, muzzles, rails and other attachments
// WCS_Scopes        : WCS optics/scopes
// WCS_Clothing      : WCS clothing/assets
// GRS Apparel       : apparel
// GRS Modular Vests : vests and rigs
// GRS Belts/Bags    : belts, bags and droplegs
// GRS Patches       : patches/identifiers
// RHS packs         : RHS weapons, clothing and content
// Rayzis Optics     : optics
// RIS Laser         : laser/light attachments
// Weapon Holstering : holster behavior
// Advanced Zeroing  : weapon zeroing behavior
// NoWeight/InfiniteStamina: behavior only, never duplicated here
//
// No prefab GUIDs or undocumented third-party API calls are fabricated here.
// ============================================================================
class RHD_LoadoutEditorMissionAdapter
{
	bool GetCurrentLoadout(RHD_LoadoutProfile profile) { return false; }
	bool ApplyLoadout(RHD_LoadoutProfile profile) { return false; }
	bool SaveLoadoutProfile(string profileId, RHD_LoadoutProfile profile) { return false; }
	bool LoadLoadoutProfile(string profileId, RHD_LoadoutProfile profile) { return false; }
	bool DeleteLoadoutProfile(string profileId) { return false; }

	// Dynamic catalog from the actual mission/arsenal.
	bool EnumerateItems(string category, out array<string> itemIds) { itemIds = {}; return false; }
	bool EnumerateAttachments(string weaponItemId, out array<string> attachmentIds) { attachmentIds = {}; return false; }
	bool EnumerateCompatibleMagazines(string weaponItemId, out array<string> magazineIds) { magazineIds = {}; return false; }
	bool EnumerateCompatibleOptics(string weaponItemId, out array<string> opticIds) { opticIds = {}; return false; }
	bool IsItemAllowed(string category, string itemId) { return false; }
	bool CanAttach(string weaponItemId, string attachmentId) { return false; }
	bool GetItemDisplayName(string itemId, out string displayName) { displayName = itemId; return false; }

	// Optional runtime behavior integrations.
	bool SetWeaponZeroing(string weaponItemId, int meters) { return false; }
	bool GetWeaponZeroing(string weaponItemId, out int meters) { meters = 0; return false; }
	bool SetHolsterState(string state) { return false; }
	bool GetHolsterState(out string state) { state = ""; return false; }
	bool OpenWCSLoadoutEditor() { return false; }

	// Inventory/container support for deep editor customization.
	bool AddInventoryItem(string itemId, int quantity, string targetContainerId) { return false; }
	bool RemoveInventoryItem(string itemId, int quantity, string sourceContainerId) { return false; }
	bool MoveInventoryItem(string itemId, int quantity, string sourceContainerId, string targetContainerId) { return false; }
	bool EnumeratePlayerContainers(out array<string> containerIds) { containerIds = {}; return false; }
	bool GetContainerCapacity(string containerId, out int used, out int capacity) { used = 0; capacity = 0; return false; }

	// Dependency availability is reported separately so the UI can explain why
	// a category is unavailable instead of silently failing.
	bool HasWCSLoadoutEditor() { return false; }
	bool HasWCSWeapons() { return false; }
	bool HasWCSAttachments() { return false; }
	bool HasWCSScopes() { return false; }
	bool HasWCSClothing() { return false; }
	bool HasGRSApparel() { return false; }
	bool HasGRSVestsRigs() { return false; }
	bool HasGRSBagsBeltsDroplegs() { return false; }
	bool HasGRSPatches() { return false; }
	bool HasRHS() { return false; }
	bool HasRayziOptics() { return false; }
	bool HasRISLasers() { return false; }
	bool HasHolstering() { return false; }
	bool HasZeroing() { return false; }
};
