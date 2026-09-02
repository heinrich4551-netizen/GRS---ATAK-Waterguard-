// ============================================================================
// GRS VIRTUAL PLAYER - MISSION ADAPTER
// ============================================================================
// BEGINNER RULE: put mission-specific connections here. Do not edit the F8
// controller just to change vehicles, inventory or civilian-job behavior.
//
// JOBS use EXISTING civilian AI from the running mission. The adapter must
// return IDs for civilians that are already spawned. The job system never
// creates a replacement civilian just to satisfy a job.
// ============================================================================

class GRS_VirtualPlayerMissionAdapter
{
	// PLAYER / BASE INVENTORY
	bool RemoveBaseItemFromPlayer(string itemId, int quantity) { return false; }
	bool GiveBaseItemToPlayer(string itemId, int quantity) { return false; }

	// ECONOMY
	bool ChargePlayer(string playerId, int amount) { return false; }
	bool CreditPlayer(string playerId, int amount) { return false; }

	// VEHICLES
	// These checks are against the current mission, not an arbitrary client ID.
	bool IsVehicleAvailableInMission(string vehicleId) { return false; }
	bool SpawnGarageVehicle(string vehicleId, vector position) { return false; }

	// EXISTING CIVILIAN AI - JOBS
	// Return true only for an actual civilian character currently spawned in
	// the mission. Never spawn a new civilian for a job.
	bool IsCivilianCharacter(string civilianId) { return false; }

	// Used for dialogue and delivery validation.
	bool IsPlayerNearCivilian(string playerId, string civilianId, float distanceMeters) { return false; }

	// IMPORTANT: return the network-safe ID of an EXISTING civilian AI.
	// The job manager calls this whenever it needs a random civilian.
	string FindRandomCivilian(string playerId) { return ""; }

	// PROPERTY / BUILDING
	bool IsMapPositionBuildable(vector position, float radiusMeters) { return false; }
	bool DoesPositionOverlapAnotherPlayerProperty(vector position, float radiusMeters) { return true; }
	bool IsPropertyInPlayerCluster(string playerId, vector position) { return false; }
	bool SpawnPropertyObject(string objectType, string objectId, vector position) { return false; }
};
