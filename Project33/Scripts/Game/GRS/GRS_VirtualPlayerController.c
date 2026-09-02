class GRS_VirtualPlayerController
{
	protected bool m_bOpen;
	protected ref GRS_VirtualPlayerState m_State;
	protected GRS_VirtualPlayerTab m_eTab = GRS_VirtualPlayerTab.INVENTORY;

	void GRS_VirtualPlayerController()
	{
		m_State = new GRS_VirtualPlayerState();
	}

	void Initialize()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.AddActionListener("GRS_Virtual_Player_Menu", EActionTrigger.DOWN, OnF8);
	}

	void Shutdown()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.RemoveActionListener("GRS_Virtual_Player_Menu", EActionTrigger.DOWN, OnF8);
	}

	void OnF8(float value, EActionTrigger reason)
	{
		Toggle();
	}

	void Toggle()
	{
		m_bOpen = !m_bOpen;
		if (m_bOpen)
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.GRS_Virtual_Player_Menu);
		else
			GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.GRS_Virtual_Player_Menu);
	}

	bool IsOpen() { return m_bOpen; }
	GRS_VirtualPlayerState GetState() { return m_State; }
	GRS_VirtualPlayerTab GetTab() { return m_eTab; }
	void SetTab(GRS_VirtualPlayerTab tab) { m_eTab = tab; }

	// -------------------------------------------------------------------------
	// Server-owned operation hooks. The menu is only a client presentation layer;
	// inventory, garage, property ownership, jobs and purchases must be persisted
	// and validated by the authoritative mission/server implementation.
	// -------------------------------------------------------------------------
	bool StoreBaseItem(string itemId, string displayName, int quantity)
	{
		return ServerStoreBaseItem(itemId, displayName, quantity);
	}

	bool PurchaseVirtualItem(string itemId, string displayName, int price)
	{
		if (price < 0)
			return false;
		return ServerPurchaseVirtualItem(itemId, displayName, price);
	}

	bool PurchaseVehicle(string vehicleId, string displayName, int price)
	{
		if (price < 0)
			return false;
		return ServerPurchaseVehicle(vehicleId, displayName, price);
	}

	bool SpawnStoredVehicle(string vehicleId)
	{
		return ServerSpawnStoredVehicle(vehicleId);
	}

	bool StartJob(string jobId)
	{
		return ServerStartJob(jobId);
	}

	bool AdvanceJobDialogue(string jobId, string civilianId)
	{
		return ServerAdvanceJobDialogue(jobId, civilianId);
	}

	bool CompleteJobDelivery(string jobId, string civilianId, string virtualItemId)
	{
		return ServerCompleteJobDelivery(jobId, civilianId, virtualItemId);
	}

	bool PurchaseProperty(string propertyId, string displayName, vector position, float buildRadiusMeters, int price)
	{
		if (m_State.m_aProperties.Count() >= GRS_VirtualPlayerConfig.MAX_PROPERTIES)
			return false;
		if (buildRadiusMeters <= 0 || buildRadiusMeters > GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M)
			return false;
		if (price < GRS_VirtualPlayerConfig.MIN_PROPERTY_PRICE || price > GRS_VirtualPlayerConfig.MAX_PROPERTY_PRICE)
			return false;
		if (!IsPropertyPlacementAllowed(position, buildRadiusMeters))
			return false;
		return ServerPurchaseProperty(propertyId, displayName, position, buildRadiusMeters, price);
	}

	int CalculatePropertyPrice(float buildRadiusMeters)
	{
		if (buildRadiusMeters <= 0)
			return 0;
		if (buildRadiusMeters > GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M)
			buildRadiusMeters = GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M;

		// Radius-based pricing: the maximum radius is exactly $15,000,000.
		return Math.Round(GRS_VirtualPlayerConfig.MAX_PROPERTY_PRICE * (buildRadiusMeters / GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M));
	}

	bool AddPropertyStorage(int propertyIndex, string storageId)
	{
		if (propertyIndex < 0 || propertyIndex >= m_State.m_aProperties.Count())
			return false;
		return ServerBuildPropertyObject(propertyIndex, "Storage", storageId);
	}

	bool BuildPropertyObject(int propertyIndex, string objectType, string objectId)
	{
		if (propertyIndex < 0 || propertyIndex >= m_State.m_aProperties.Count())
			return false;
		if (objectType.IsEmpty() || objectId.IsEmpty())
			return false;
		return ServerBuildPropertyObject(propertyIndex, objectType, objectId);
	}

	bool IsPropertyPlacementAllowed(vector position, float radiusMeters)
	{
		// The authoritative implementation must test map bounds, terrain, existing
		// buildings and overlap. The base client controller does not have authority
		// to approve placement.
		return radiusMeters > 0 && radiusMeters <= GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M;
	}

	protected bool ServerStoreBaseItem(string itemId, string displayName, int quantity) { return false; }
	protected bool ServerPurchaseVirtualItem(string itemId, string displayName, int price) { return false; }
	protected bool ServerPurchaseVehicle(string vehicleId, string displayName, int price) { return false; }
	protected bool ServerSpawnStoredVehicle(string vehicleId) { return false; }
	protected bool ServerStartJob(string jobId) { return false; }
	protected bool ServerAdvanceJobDialogue(string jobId, string civilianId) { return false; }
	protected bool ServerCompleteJobDelivery(string jobId, string civilianId, string virtualItemId) { return false; }
	protected bool ServerPurchaseProperty(string propertyId, string displayName, vector position, float buildRadiusMeters, int price) { return false; }
	protected bool ServerBuildPropertyObject(int propertyIndex, string objectType, string objectId) { return false; }
};

enum GRS_VirtualPlayerTab
{
	INVENTORY,
	GARAGE,
	JOBS,
	PROPERTY
};
