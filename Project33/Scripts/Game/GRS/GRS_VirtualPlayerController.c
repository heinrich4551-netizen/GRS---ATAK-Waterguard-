class GRS_VirtualPlayerController
{
	protected bool m_bOpen;
	protected ref GRS_VirtualPlayerState m_State;
	protected GRS_VirtualPlayerTab m_eTab = GRS_VirtualPlayerTab.INVENTORY;
	protected ref GRS_VirtualPlayerMissionAdapter m_Mission;

	void GRS_VirtualPlayerController()
	{
		m_State = new GRS_VirtualPlayerState();
		m_Mission = new GRS_VirtualPlayerMissionAdapter();
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

	void OnF8(float value, EActionTrigger reason) { Toggle(); }

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
	GRS_VirtualPlayerMissionAdapter GetMissionAdapter() { return m_Mission; }

	// The mission adapter is the only place that needs mission-specific logic.
	bool StoreBaseItem(string itemId, string displayName, int quantity)
	{
		if (itemId.IsEmpty() || quantity <= 0)
			return false;
		if (!m_Mission.RemoveBaseItemFromPlayer(itemId, quantity))
			return false;
		return m_State.AddBaseItem(itemId, displayName, quantity);
	}

	bool PurchaseVirtualItem(string itemId, string displayName, int price)
	{
		if (itemId.IsEmpty() || price < 0)
			return false;
		// The authoritative implementation must use the configured catalogue
		// price rather than trusting a price supplied by the UI.
		if (!m_Mission.ChargePlayer("LOCAL_PLAYER", price))
			return false;
		if (m_State.AddVirtualItem(itemId, displayName, 1))
			return true;
		m_Mission.CreditPlayer("LOCAL_PLAYER", price);
		return false;
	}

	bool PurchaseVehicle(string vehicleId, string displayName, int price)
	{
		if (vehicleId.IsEmpty() || price < 0)
			return false;
		if (!m_Mission.IsVehicleAvailableInMission(vehicleId))
			return false;
		if (!m_Mission.ChargePlayer("LOCAL_PLAYER", price))
			return false;
		if (m_State.AddGarageVehicle(vehicleId, displayName))
			return true;
		m_Mission.CreditPlayer("LOCAL_PLAYER", price);
		return false;
	}

	bool SpawnStoredVehicle(string vehicleId)
	{
		if (vehicleId.IsEmpty() || !m_Mission.IsVehicleAvailableInMission(vehicleId))
			return false;
		// The mission decides the safe spawn position.
		return m_Mission.SpawnGarageVehicle(vehicleId, vector.Zero);
	}

	// Generate a job from existing civilian AI. No civilian prefab is required.
	bool GenerateRandomJob(string jobId, string virtualItemId, int reward)
	{
		if (jobId.IsEmpty() || virtualItemId.IsEmpty() || reward < 0)
			return false;
		if (m_State.m_aJobs.Count() >= GRS_VirtualPlayerConfig.MAX_ACTIVE_JOBS)
			return false;

		string source = m_Mission.FindRandomCivilian("LOCAL_PLAYER");
		string destination = m_Mission.FindRandomCivilian("LOCAL_PLAYER");
		if (source.IsEmpty() || destination.IsEmpty() || source == destination)
			return false;
		if (!m_Mission.IsCivilianCharacter(source) || !m_Mission.IsCivilianCharacter(destination))
			return false;

		GRS_JobEntry job = new GRS_JobEntry();
		job.m_sJobId = jobId;
		job.m_sTitle = "Civilian Delivery";
		job.m_sDescription = "Talk to the first civilian, follow the lead, then deliver the virtual item to another civilian.";
		job.m_sSourceCivilian = source;
		job.m_sDestinationCivilian = destination;
		job.m_sVirtualItemId = virtualItemId;
		job.m_iReward = reward;
		job.m_iStage = GRS_JobStage.TALK_TO_SOURCE;
		job.m_bActive = true;
		m_State.m_aJobs.Insert(job);
		return true;
	}

	bool StartJob(string jobId) { return jobId.IsEmpty() ? false : ServerStartJob(jobId); }
	bool AdvanceJobDialogue(string jobId, string civilianId)
	{
		return jobId.IsEmpty() || civilianId.IsEmpty() ? false : ServerAdvanceJobDialogue(jobId, civilianId);
	}
	bool CompleteJobDelivery(string jobId, string civilianId, string virtualItemId)
	{
		return jobId.IsEmpty() || civilianId.IsEmpty() || virtualItemId.IsEmpty() ? false : ServerCompleteJobDelivery(jobId, civilianId, virtualItemId);
	}

	bool PurchaseProperty(string propertyId, string displayName, vector position, float buildRadiusMeters, int price)
	{
		if (m_State.m_aProperties.Count() >= GRS_VirtualPlayerConfig.MAX_PROPERTIES)
			return false;
		if (propertyId.IsEmpty() || buildRadiusMeters <= 0 || buildRadiusMeters > GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M)
			return false;
		int expectedPrice = CalculatePropertyPrice(buildRadiusMeters);
		if (price != expectedPrice)
			return false;
		if (!m_Mission.IsMapPositionBuildable(position, buildRadiusMeters))
			return false;
		if (m_Mission.DoesPositionOverlapAnotherPlayerProperty(position, buildRadiusMeters))
			return false;
		return ServerPurchaseProperty(propertyId, displayName, position, buildRadiusMeters, expectedPrice);
	}

	int CalculatePropertyPrice(float buildRadiusMeters)
	{
		if (buildRadiusMeters <= 0)
			return 0;
		if (buildRadiusMeters > GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M)
			buildRadiusMeters = GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M;
		int price = Math.Round(GRS_VirtualPlayerConfig.MAX_PROPERTY_PRICE * (buildRadiusMeters / GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M));
		return Math.Max(GRS_VirtualPlayerConfig.MIN_PROPERTY_PRICE, price);
	}

	bool AddPropertyStorage(int propertyIndex, string storageId)
	{
		if (propertyIndex < 0 || propertyIndex >= m_State.m_aProperties.Count() || storageId.IsEmpty())
			return false;
		if (!m_State.AddStorageObject(propertyIndex, storageId))
			return false;
		return m_Mission.SpawnPropertyObject(GRS_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE, storageId, m_State.m_aProperties[propertyIndex].m_vPosition);
	}

	bool BuildPropertyObject(int propertyIndex, string objectType, string objectId)
	{
		if (propertyIndex < 0 || propertyIndex >= m_State.m_aProperties.Count() || objectType.IsEmpty() || objectId.IsEmpty())
			return false;
		if (objectType == GRS_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE)
			return AddPropertyStorage(propertyIndex, objectId);
		if (!IsAllowedPropertyObjectType(objectType))
			return false;
		return m_Mission.SpawnPropertyObject(objectType, objectId, m_State.m_aProperties[propertyIndex].m_vPosition);
	}

	bool IsAllowedPropertyObjectType(string objectType)
	{
		return objectType == GRS_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE
			|| objectType == GRS_VirtualPlayerConfig.PROPERTY_OBJECT_ARSENAL
			|| objectType == GRS_VirtualPlayerConfig.PROPERTY_OBJECT_DECORATION
			|| objectType == GRS_VirtualPlayerConfig.PROPERTY_OBJECT_FORTIFICATION;
	}

	bool IsPropertyPlacementAllowed(vector position, float radiusMeters)
	{
		if (radiusMeters <= 0 || radiusMeters > GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M)
			return false;
		return m_Mission.IsMapPositionBuildable(position, radiusMeters)
			&& !m_Mission.DoesPositionOverlapAnotherPlayerProperty(position, radiusMeters);
	}

	protected bool ServerStoreBaseItem(string itemId, string displayName, int quantity) { return StoreBaseItemInternal(itemId, displayName, quantity); }
	protected bool StoreBaseItemInternal(string itemId, string displayName, int quantity) { return false; }
	protected bool ServerPurchaseVirtualItem(string itemId, string displayName, int price) { return false; }
	protected bool ServerPurchaseVehicle(string vehicleId, string displayName, int price) { return false; }
	protected bool ServerSpawnStoredVehicle(string vehicleId) { return m_Mission.SpawnGarageVehicle(vehicleId, vector.Zero); }
	protected bool ServerStartJob(string jobId) { return false; }
	protected bool ServerAdvanceJobDialogue(string jobId, string civilianId) { return false; }
	protected bool ServerCompleteJobDelivery(string jobId, string civilianId, string virtualItemId) { return false; }
	protected bool ServerPurchaseProperty(string propertyId, string displayName, vector position, float buildRadiusMeters, int price) { return false; }
};

enum GRS_VirtualPlayerTab
{
	INVENTORY,
	GARAGE,
	JOBS,
	PROPERTY
};
