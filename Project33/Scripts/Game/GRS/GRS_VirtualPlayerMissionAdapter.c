// ============================================================================
// RHD VIRTUAL PLAYER - MISSION ADAPTER
// ============================================================================
// Put mission-specific connections here. Jobs use EXISTING civilian AI only.
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
