class GRS_ATAKMenuController
{
	protected bool m_bOpen;
	protected ref GRS_ATAKPlayerState m_PlayerState;
	protected ref array<string> m_aShopItems = {};
	protected ref map<string, int> m_mShopPrices = new map<string, int>();

	void GRS_ATAKMenuController()
	{
		m_PlayerState = new GRS_ATAKPlayerState();
	}

	void Initialize()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.AddActionListener("GRS_ATAK_Menu", EActionTrigger.PRESSED, OnF6);
	}

	void Shutdown()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.RemoveActionListener("GRS_ATAK_Menu", EActionTrigger.PRESSED, OnF6);
	}

	void OnF6(float value, EActionTrigger reason)
	{
		Toggle();
	}

	void Toggle()
	{
		m_bOpen = !m_bOpen;
		// Bind this state to the RplWidget/ScriptedWidget menu in the UI layer.
	}

	bool IsOpen() { return m_bOpen; }
	GRS_ATAKPlayerState GetPlayerState() { return m_PlayerState; }

	// These methods are intentionally server-authoritative entry points. The UI should
	// request them through RPC rather than mutating player money or inventory locally.
	bool PurchaseShopItem(string itemId)
	{
		if (!m_mShopPrices.Contains(itemId))
			return false;
		int price = m_mShopPrices.Get(itemId);
		if (!m_PlayerState.TrySpend(price))
			return false;
		return SpawnPurchasedItemForPlayer(itemId);
	}

	bool AdminSetShopItem(string itemId, int price)
	{
		if (price < 0 || itemId.IsEmpty())
			return false;
		m_mShopPrices.Set(itemId, price);
		if (!m_aShopItems.Contains(itemId))
			m_aShopItems.Insert(itemId);
		return true;
	}

	bool RequestMortar(vector target)
	{
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.MORTAR_COST))
			return false;
		return CallPreciseMortar(target);
	}

	bool RequestSupplyDrop(vector target)
	{
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.SUPPLY_DROP_COST))
			return false;
		return SpawnSupplyDropWithRedSmoke(target);
	}

	void RecordEnemyKill() { m_PlayerState.RecordKill(); }
	void RecordEnemyRevive() { m_PlayerState.RecordRevive(); }
	void RecordBaseSupply() { m_PlayerState.RecordSupply(); }

	// Integration hooks. Implement these against the mission's inventory/fire-support
	// systems once the mission prefabs/components are present.
	protected bool SpawnPurchasedItemForPlayer(string itemId) { return !itemId.IsEmpty(); }
	protected bool CallPreciseMortar(vector target) { return !target.IsZero(); }
	protected bool SpawnSupplyDropWithRedSmoke(vector target) { return !target.IsZero(); }
};
