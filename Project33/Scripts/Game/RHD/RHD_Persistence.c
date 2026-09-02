// ============================================================================
// RHD / PROJECT33 - PERSISTENCE BOUNDARY
// ============================================================================
// Project33 owns the profile shape; Enfusion Persistence/Database owns the
// durable storage transaction. The adapter stays fail-safe until the mission
// author wires the exact installed persistence APIs/resources.
// ============================================================================
class RHD_PlayerSnapshot
{
	ref RHD_VirtualPlayerState m_State;
	int m_iMoney;
	string m_sPersistentId;
};

class RHD_PersistenceAdapter
{
	bool SavePlayerSnapshot(string persistentId, RHD_PlayerSnapshot snapshot) { return false; }
	bool LoadPlayerSnapshot(string persistentId, RHD_PlayerSnapshot snapshot) { return false; }
	bool DeletePlayerSnapshot(string persistentId) { return false; }
	bool HasPlayerSnapshot(string persistentId) { return false; }
};

class RHD_PersistenceService
{
	protected static ref RHD_PersistenceService s_Instance;
	protected ref RHD_PersistenceAdapter m_Adapter;

	static RHD_PersistenceService GetInstance()
	{
		if (!s_Instance) s_Instance = new RHD_PersistenceService();
		return s_Instance;
	}

	void RHD_PersistenceService() { m_Adapter = new RHD_PersistenceAdapter(); }
	RHD_PersistenceAdapter GetAdapter() { return m_Adapter; }

	RHD_PlayerSnapshot Capture(string persistentId, RHD_VirtualPlayerState state, int money)
	{
		if (!state || persistentId.IsEmpty()) return null;
		RHD_PlayerSnapshot snapshot = new RHD_PlayerSnapshot();
		snapshot.m_sPersistentId = persistentId;
		snapshot.m_iMoney = money;
		snapshot.m_State = state;
		return snapshot;
	}

	bool Save(string persistentId, RHD_VirtualPlayerState state, int money)
	{
		RHD_PlayerSnapshot snapshot = Capture(persistentId, state, money);
		return snapshot && m_Adapter && m_Adapter.SavePlayerSnapshot(persistentId, snapshot);
	}

	bool Load(string persistentId, RHD_PlayerSnapshot snapshot)
	{
		return !persistentId.IsEmpty() && snapshot && m_Adapter && m_Adapter.LoadPlayerSnapshot(persistentId, snapshot);
	}

	bool Delete(string persistentId)
	{
		return !persistentId.IsEmpty() && m_Adapter && m_Adapter.DeletePlayerSnapshot(persistentId);
	}
};
