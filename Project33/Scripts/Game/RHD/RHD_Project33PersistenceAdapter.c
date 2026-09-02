// ============================================================================
// RHD / PROJECT33 - PERSISTENCE INTEGRATION ADAPTER
// ============================================================================
// Mission author wires the exact Enfusion Persistence Framework / Database
// calls here. Project33 intentionally does not guess undocumented APIs.
// ============================================================================
class RHD_Project33PersistenceAdapter : RHD_PersistenceAdapter
{
	bool GetPlayerPersistentId(out string persistentId)
	{
		persistentId = "";
		return false;
	}

	bool SaveCurrentPlayer(RHD_VirtualPlayerState state, int money)
	{
		string persistentId;
		if (!state || !GetPlayerPersistentId(persistentId)) return false;
		RHD_PersistenceService persistence = RHD_PersistenceService.GetInstance();
		return persistence && persistence.Save(persistentId, state, money);
	}
};
