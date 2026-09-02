// ============================================================================
// RHD VIRTUAL PLAYER - MISSION / MOD ADAPTER
// ============================================================================
// This is the only file intended to need mission-specific wiring.
// Keep third-party calls here so Project33 does not hard-code unknown APIs.
//
// Intended integrations from the supplied dependency set:
// - Inventory / loadout: Overthrow, WCS_LoadoutEditor, Edit Entity Inventory,
//   GRS - Essentials / Bag / Belts / Apparel / Modular Vests.
// - ATAK / map services: GRS - ATAK Plugin Core, GRS - ATAK DEV, SWAG_ATAK,
//   Anarchy Map Markers & Drawings, Anarchy Markers x GRS-ATAK DEV,
//   PlayerMapMarkersByCryneX, AdvancedCompassByCryneX.
// - Admin authority: AdminCommandsByCryneX and Game Master Enhanced.
// - Persistence: Enfusion Persistence Framework and Enfusion Database Framework.
// - Vehicle / weapon content: RHS packs, WCS content, Air Assets, drones and
//   the listed attachment/optic mods. Project33 stores IDs, not fake prefabs.
// - Weight / stamina / holstering / equipment behavior remains owned by the
//   respective dependency; Project33 does not replace those systems.
//
// Jobs deliberately use EXISTING civilian AI. No job-only civilians are spawned.
// Defaults below are fail-safe until the mission author wires real APIs/resources.
// ============================================================================
class RHD_VirtualPlayerMissionAdapter
{
	bool RemoveBaseItemFromPlayer(string itemId, int quantity) { return false; }
	bool GiveBaseItemToPlayer(string itemId, int quantity) { return false; }
	bool ChargePlayer(string playerId, int amount) { return false; }
	bool CreditPlayer(string playerId, int amount) { return false; }
	bool IsVehicleAvailableInMission(string vehicleId) { return false; }
	bool SpawnGarageVehicle(string vehicleId, vector position) { return false; }
	bool IsCivilianCharacter(string civilianId) { return false; }
	bool IsPlayerNearCivilian(string playerId, string civilianId, float distanceMeters) { return false; }
	string FindRandomCivilian(string playerId) { return ""; }
	bool IsMapPositionBuildable(vector position, float radiusMeters) { return false; }
	bool DoesPositionOverlapAnotherPlayerProperty(vector position, float radiusMeters) { return true; }
	bool IsPropertyInPlayerCluster(string playerId, vector position) { return false; }
	bool SpawnPropertyObject(string objectType, string objectId, vector position) { return false; }
};
