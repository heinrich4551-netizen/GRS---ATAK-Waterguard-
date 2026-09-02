class RHD_VirtualPlayerController
{
	protected bool m_bOpen;
	protected ref RHD_VirtualPlayerState m_State;
	protected RHD_VirtualPlayerTab m_eTab = RHD_VirtualPlayerTab.INVENTORY;
	protected ref RHD_VirtualPlayerMissionAdapter m_Mission;
	void RHD_VirtualPlayerController() { m_State = new RHD_VirtualPlayerState(); m_Mission = new RHD_VirtualPlayerMissionAdapter(); }
	void Initialize() { InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.AddActionListener("RHD_Virtual_Player_Menu", EActionTrigger.DOWN, OnF8); }
	void Shutdown() { InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.RemoveActionListener("RHD_Virtual_Player_Menu", EActionTrigger.DOWN, OnF8); }
	void OnF8(float value, EActionTrigger reason) { Toggle(); }
	void Toggle() { m_bOpen = !m_bOpen; if (m_bOpen) GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.RHD_Virtual_Player_Menu); else GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.RHD_Virtual_Player_Menu); }
	bool IsOpen() { return m_bOpen; }
	RHD_VirtualPlayerState GetState() { return m_State; }
	RHD_VirtualPlayerTab GetTab() { return m_eTab; }
	void SetTab(RHD_VirtualPlayerTab tab) { m_eTab = tab; }
	RHD_VirtualPlayerMissionAdapter GetMissionAdapter() { return m_Mission; }
	RHD_ATAKPlayerState GetSharedEconomy() { RHD_ATAKMenuController atak = RHD_ATAKMenuController.GetInstance(); return atak ? atak.GetPlayerState() : null; }

	bool StoreBaseItem(string itemId, string displayName, int quantity)
	{
		if (itemId.IsEmpty() || quantity <= 0 || !m_Mission.RemoveBaseItemFromPlayer(itemId, quantity)) return false;
		if (m_State.AddBaseItem(itemId, displayName, quantity)) return true;
		m_Mission.GiveBaseItemToPlayer(itemId, quantity); return false;
	}
	bool TakeBaseItem(string itemId, int quantity)
	{
		if (itemId.IsEmpty() || quantity <= 0 || m_State.GetBaseItemQuantity(itemId) < quantity) return false;
		if (!m_Mission.GiveBaseItemToPlayer(itemId, quantity)) return false;
		return m_State.RemoveBaseItem(itemId, quantity);
	}
	bool PurchaseVirtualItem(string itemId, string displayName, int requestedPrice)
	{
		int price;
		if (!RHD_VirtualPlayerEasyConfig.ALLOW_SHOP_BUYING || !RHD_Shop.GetSellPrice(itemId, price) || !RHD_VirtualProduction.IsKnownVirtualItem(itemId)) return false;
		RHD_ATAKPlayerState economy = GetSharedEconomy();
		if (!economy || !economy.TrySpend(price)) return false;
		if (m_State.AddVirtualItem(itemId, RHD_VirtualProduction.GetDisplayName(itemId), 1)) return true;
		economy.Refund(price); return false;
	}
	bool AddShopCartItem(string itemId, int quantity)
	{
		RHD_ATAKMenuController atak = RHD_ATAKMenuController.GetInstance();
		return atak && atak.AddShopCartItem(itemId, quantity);
	}
	bool RemoveShopCartItem(string itemId, int quantity) { RHD_ATAKMenuController atak = RHD_ATAKMenuController.GetInstance(); return atak && atak.RemoveShopCartItem(itemId, quantity); }
	void ClearShopCart() { RHD_ATAKMenuController atak = RHD_ATAKMenuController.GetInstance(); if (atak) atak.ClearShopCart(); }
	int GetShopCartTotal() { RHD_ATAKMenuController atak = RHD_ATAKMenuController.GetInstance(); return atak ? atak.GetShopCartTotal() : 0; }
	bool CheckoutShopCart() { RHD_ATAKMenuController atak = RHD_ATAKMenuController.GetInstance(); return atak && atak.CheckoutShopCart(); }
	bool SellVirtualItem(string itemId, int quantity)
	{
		if (!RHD_VirtualPlayerEasyConfig.ALLOW_SHOP_SELLING || quantity <= 0 || m_State.GetVirtualItemQuantity(itemId) < quantity) return false;
		int total = RHD_Shop.CalculateSale(itemId, quantity);
		RHD_ATAKPlayerState economy = GetSharedEconomy();
		if (total <= 0 || !economy || !m_State.RemoveVirtualItem(itemId, quantity)) return false;
		economy.AddMoney(total); return true;
	}
	bool SellAllVirtualItem(string itemId) { int quantity = m_State.GetVirtualItemQuantity(itemId); return quantity > 0 && SellVirtualItem(itemId, quantity); }
	bool RefineVirtualItem(string inputId) { return RHD_VirtualProduction.Refine(m_State, inputId); }
	bool ProcessVirtualItem(string inputId) { return RHD_VirtualProduction.Process(m_State, inputId); }

	bool PurchaseVehicle(string vehicleId, string displayName, int price)
	{
		if (vehicleId.IsEmpty() || !m_Mission.IsVehicleAvailableInMission(vehicleId)) return false;
		RHD_ATAKPlayerState economy = GetSharedEconomy();
		if (!economy || !economy.TrySpend(price)) return false;
		if (m_State.AddGarageVehicle(vehicleId, displayName)) return true;
		economy.Refund(price); return false;
	}
	bool SpawnStoredVehicle(string vehicleId) { return !vehicleId.IsEmpty() && m_Mission.IsVehicleAvailableInMission(vehicleId) && m_Mission.SpawnGarageVehicle(vehicleId, vector.Zero); }
	bool GenerateRandomJob(string jobId, string virtualItemId, int reward)
	{
		if (jobId.IsEmpty() || !RHD_VirtualProduction.IsKnownVirtualItem(virtualItemId) || reward < 0 || m_State.m_aJobs.Count() >= RHD_VirtualPlayerConfig.MAX_ACTIVE_JOBS) return false;
		string source = m_Mission.FindRandomCivilian("LOCAL_PLAYER"); string destination = m_Mission.FindRandomCivilian("LOCAL_PLAYER");
		if (source.IsEmpty() || destination.IsEmpty() || source == destination || !m_Mission.IsCivilianCharacter(source) || !m_Mission.IsCivilianCharacter(destination)) return false;
		RHD_JobEntry job = new RHD_JobEntry(); job.m_sJobId = jobId; job.m_sTitle = "Civilian Delivery"; job.m_sDescription = "Talk to the source civilian, follow the contact, then deliver the virtual item."; job.m_sSourceCivilian = source; job.m_sDestinationCivilian = destination; job.m_sVirtualItemId = virtualItemId; job.m_iReward = reward; job.m_iStage = RHD_JobStage.TALK_TO_SOURCE; job.m_bActive = true; m_State.m_aJobs.Insert(job); return true;
	}
	bool StartJob(string jobId) { return !jobId.IsEmpty() && ServerStartJob(jobId); }
	bool AdvanceJobDialogue(string jobId, string civilianId) { return !jobId.IsEmpty() && !civilianId.IsEmpty() && ServerAdvanceJobDialogue(jobId, civilianId); }
	bool CompleteJobDelivery(string jobId, string civilianId, string virtualItemId) { return !jobId.IsEmpty() && !civilianId.IsEmpty() && !virtualItemId.IsEmpty() && ServerCompleteJobDelivery(jobId, civilianId, virtualItemId); }
	bool PurchaseProperty(string propertyId, string displayName, vector position, float buildRadiusMeters, int price)
	{
		if (m_State.m_aProperties.Count() >= RHD_VirtualPlayerConfig.MAX_PROPERTIES || propertyId.IsEmpty() || buildRadiusMeters <= 0 || buildRadiusMeters > RHD_VirtualPlayerConfig.MAX_BUILD_RADIUS_M) return false;
		int expectedPrice = CalculatePropertyPrice(buildRadiusMeters); if (price != expectedPrice || !IsPropertyPlacementAllowed(position, buildRadiusMeters)) return false;
		return ServerPurchaseProperty(propertyId, displayName, position, buildRadiusMeters, expectedPrice);
	}
	int CalculatePropertyPrice(float buildRadiusMeters) { if (buildRadiusMeters <= 0) return 0; if (buildRadiusMeters > RHD_VirtualPlayerConfig.MAX_BUILD_RADIUS_M) buildRadiusMeters = RHD_VirtualPlayerConfig.MAX_BUILD_RADIUS_M; return Math.Max(RHD_VirtualPlayerConfig.MIN_PROPERTY_PRICE, Math.Round(RHD_VirtualPlayerConfig.MAX_PROPERTY_PRICE * (buildRadiusMeters / RHD_VirtualPlayerConfig.MAX_BUILD_RADIUS_M))); }
	bool AddPropertyStorage(int propertyIndex, string storageId) { return propertyIndex >= 0 && propertyIndex < m_State.m_aProperties.Count() && !storageId.IsEmpty() && ServerBuildPropertyObject(propertyIndex, RHD_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE, storageId); }
	bool BuildPropertyObject(int propertyIndex, string objectType, string objectId) { return propertyIndex >= 0 && propertyIndex < m_State.m_aProperties.Count() && !objectType.IsEmpty() && !objectId.IsEmpty() && IsAllowedPropertyObjectType(objectType) && ServerBuildPropertyObject(propertyIndex, objectType, objectId); }
	bool IsAllowedPropertyObjectType(string objectType) { return objectType == RHD_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE || objectType == RHD_VirtualPlayerConfig.PROPERTY_OBJECT_ARSENAL || objectType == RHD_VirtualPlayerConfig.PROPERTY_OBJECT_DECORATION || objectType == RHD_VirtualPlayerConfig.PROPERTY_OBJECT_FORTIFICATION; }
	bool IsPropertyPlacementAllowed(vector position, float radiusMeters) { return radiusMeters > 0 && radiusMeters <= RHD_VirtualPlayerConfig.MAX_BUILD_RADIUS_M && m_Mission.IsMapPositionBuildable(position, radiusMeters) && !m_Mission.DoesPositionOverlapAnotherPlayerProperty(position, radiusMeters); }
	protected bool ServerStartJob(string jobId) { return false; }
	protected bool ServerAdvanceJobDialogue(string jobId, string civilianId) { return false; }
	protected bool ServerCompleteJobDelivery(string jobId, string civilianId, string virtualItemId) { return false; }
	protected bool ServerPurchaseProperty(string propertyId, string displayName, vector position, float buildRadiusMeters, int price) { return false; }
	protected bool ServerBuildPropertyObject(int propertyIndex, string objectType, string objectId) { return false; }
};

enum RHD_VirtualPlayerTab { INVENTORY, GARAGE, JOBS, PROPERTY, SHOP };
