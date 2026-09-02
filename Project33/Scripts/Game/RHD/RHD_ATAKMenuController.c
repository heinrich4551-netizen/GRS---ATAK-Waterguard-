class RHD_ATAKMenuController
{
	protected static ref RHD_ATAKMenuController s_Instance;
	protected bool m_bOpen;
	protected ref RHD_ATAKPlayerState m_PlayerState;
	protected ref RHD_ATAKMapState m_MapState;
	protected ref RHD_ShopCart m_ShopCart;
	protected ref array<string> m_aShopItems = {};
	protected int m_iRecruitedAI;

	void RHD_ATAKMenuController()
	{
		s_Instance = this;
		m_PlayerState = new RHD_ATAKPlayerState();
		m_MapState = new RHD_ATAKMapState();
		m_ShopCart = new RHD_ShopCart();
		LoadDefaultShop();
	}
	static RHD_ATAKMenuController GetInstance() { return s_Instance; }
	void Initialize() { InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.AddActionListener("RHD_ATAK_Menu", EActionTrigger.DOWN, OnF6); }
	void Shutdown() { InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.RemoveActionListener("RHD_ATAK_Menu", EActionTrigger.DOWN, OnF6); if (s_Instance == this) s_Instance = null; }
	void OnF6(float value, EActionTrigger reason) { Toggle(); }
	void Toggle() { m_bOpen = !m_bOpen; if (m_bOpen) GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.RHD_ATAK_Menu); else GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.RHD_ATAK_Menu); }
	bool IsOpen() { return m_bOpen; }
	RHD_ATAKPlayerState GetPlayerState() { return m_PlayerState; }
	RHD_ATAKMapState GetMapState() { return m_MapState; }

	protected void LoadDefaultShop()
	{
		m_aShopItems.Clear();
		array<string> itemIds = {"APPLE","CANNABIS_PLANT","COCA_LEAF","CORN_COB","GRAPES","PEACHES","IRON_ORE","COPPER_ORE","GOLD_ORE","DIAMOND","OIL_SAND","IRON","COPPER","GOLD","OIL","CANNABIS_FLOWER","COCAINE","CANNED_CORN"};
		foreach (string itemId : itemIds) m_aShopItems.Insert(itemId);
	}
	bool AdminSetShopItem(string itemId, int price, bool callerIsAdmin)
	{
		if (!callerIsAdmin || itemId.IsEmpty() || price < 0) return false;
		return RHD_ShopAdmin.SetPrice(itemId, price);
	}
	int GetShopItemCount() { return m_aShopItems.Count(); }
	string GetShopItem(int index) { if (index < 0 || index >= m_aShopItems.Count()) return ""; return m_aShopItems[index]; }
	int GetShopPrice(string itemId) { int price; if (!RHD_Shop.GetSellPrice(itemId, price)) return -1; return price; }

	bool AddShopCartItem(string itemId, int quantity) { return RHD_ShopCartAllowed(itemId) && m_ShopCart.Add(itemId, quantity); }
	bool RemoveShopCartItem(string itemId, int quantity) { return m_ShopCart.Remove(itemId, quantity); }
	void ClearShopCart() { m_ShopCart.Clear(); }
	RHD_ShopCart GetShopCart() { return m_ShopCart; }
	int GetShopCartTotal() { return m_ShopCart.GetTotal(); }
	bool CheckoutShopCart()
	{
		if (!RHD_VirtualPlayerEasyConfig.ALLOW_SHOP_BUYING || m_ShopCart.GetCount() <= 0) return false;
		int total = m_ShopCart.GetTotal();
		if (total <= 0 || !m_PlayerState.TrySpend(total)) return false;
		foreach (RHD_ShopCartEntry entry : m_ShopCart.m_aEntries)
		{
			if (!entry) continue;
			if (!RHD_VirtualProduction.IsKnownVirtualItem(entry.m_sItemId) || !m_VirtualInventoryAdd(entry.m_sItemId, entry.m_iQuantity))
			{
				m_PlayerState.Refund(total);
				return false;
			}
		}
		m_ShopCart.Clear();
		return true;
	}
	bool SellVirtualItem(string itemId, int quantity)
	{
		if (!RHD_VirtualPlayerEasyConfig.ALLOW_SHOP_SELLING || quantity <= 0) return false;
		if (m_VirtualInventoryQuantity(itemId) < quantity) return false;
		int total = RHD_Shop.CalculateSale(itemId, quantity);
		if (total <= 0 || !m_VirtualInventoryRemove(itemId, quantity)) return false;
		m_PlayerState.AddMoney(total);
		return true;
	}
	bool SellAllVirtualItem(string itemId)
	{
		int quantity = m_VirtualInventoryQuantity(itemId);
		return quantity > 0 && SellVirtualItem(itemId, quantity);
	}
	protected bool RHD_ShopCartAllowed(string itemId) { int price; return RHD_Shop.GetSellPrice(itemId, price) && RHD_VirtualProduction.IsKnownVirtualItem(itemId); }
	protected bool m_VirtualInventoryAdd(string itemId, int quantity)
	{
		RHD_VirtualPlayerController virtualPlayer = RHD_VirtualPlayerMenuService.GetInstance();
		return virtualPlayer && virtualPlayer.GetState().AddVirtualItem(itemId, RHD_VirtualProduction.GetDisplayName(itemId), quantity);
	}
	protected int m_VirtualInventoryQuantity(string itemId)
	{
		RHD_VirtualPlayerController virtualPlayer = RHD_VirtualPlayerMenuService.GetInstance();
		return virtualPlayer ? virtualPlayer.GetState().GetVirtualItemQuantity(itemId) : 0;
	}
	protected bool m_VirtualInventoryRemove(string itemId, int quantity)
	{
		RHD_VirtualPlayerController virtualPlayer = RHD_VirtualPlayerMenuService.GetInstance();
		return virtualPlayer && virtualPlayer.GetState().RemoveVirtualItem(itemId, quantity);
	}

	int PlayBlackjack(int stake) { if (!RHD_ATAKFeatures.IsValidBet(stake, RHD_ATAKConfig.BLACKJACK_MIN_STAKE) || !m_PlayerState.TrySpend(stake)) return 0; int roll = Math.RandomInt(0, 100); int payout = RHD_ATAKFeatures.BlackjackPayout(roll < 48, roll < 5, stake); m_PlayerState.AddMoney(payout); return payout; }
	int PlayRoulette(int stake, int selectedNumber) { if (!RHD_ATAKFeatures.IsValidBet(stake, RHD_ATAKConfig.ROULETTE_MIN_STAKE) || selectedNumber < 0 || selectedNumber > 36 || !m_PlayerState.TrySpend(stake)) return 0; int payout = RHD_ATAKFeatures.RoulettePayout(Math.RandomInt(0, 37), selectedNumber, stake); m_PlayerState.AddMoney(payout); return payout; }
	int BuyScratchCard() { if (!m_PlayerState.TrySpend(RHD_ATAKConfig.SCRATCH_CARD_COST)) return 0; int payout = RHD_ATAKFeatures.ScratchPayout(Math.RandomInt(0, 100), RHD_ATAKConfig.SCRATCH_CARD_COST); m_PlayerState.AddMoney(payout); return payout; }
	bool ClaimDailyLotto(int dayKey, int playerTicket) { if (m_PlayerState.HasClaimedDailyLotto(dayKey) || !m_PlayerState.TrySpend(RHD_ATAKConfig.DAILY_LOTTO_COST)) return false; m_PlayerState.MarkDailyLottoClaimed(dayKey); if (playerTicket == Math.RandomInt(0, 1000000)) m_PlayerState.AddMoney(100000); return true; }
	bool RecruitAI(RHD_ATAKHandlerRequest request) { if (m_iRecruitedAI >= RHD_ATAKConfig.MAX_RECRUITED_AI || request.m_sPrefab.IsEmpty() || !SpawnHandlerAI(request)) return false; m_iRecruitedAI++; return true; }
	bool RequestMortar(vector target) { if (!m_PlayerState.TrySpend(RHD_ATAKConfig.MORTAR_COST)) return false; if (CallPreciseMortar(target)) return true; m_PlayerState.Refund(RHD_ATAKConfig.MORTAR_COST); return false; }
	bool RequestSupplyDrop(vector target) { if (!m_PlayerState.TrySpend(RHD_ATAKConfig.SUPPLY_DROP_COST)) return false; if (SpawnSupplyDropWithRedSmoke(target)) return true; m_PlayerState.Refund(RHD_ATAKConfig.SUPPLY_DROP_COST); return false; }
	void RecordEnemyKill() { m_PlayerState.RecordKill(); }
	void RecordEnemyRevive() { m_PlayerState.RecordRevive(); }
	void RecordBaseSupply() { m_PlayerState.RecordSupply(); }
	protected bool SpawnPurchasedItemForPlayer(string itemId) { return !itemId.IsEmpty(); }
	protected bool SpawnHandlerAI(RHD_ATAKHandlerRequest request) { return !request.m_sPrefab.IsEmpty(); }
	protected bool CallPreciseMortar(vector target) { return true; }
	protected bool SpawnSupplyDropWithRedSmoke(vector target) { return true; }
};

class RHD_ShopAdmin
{
	static bool SetPrice(string itemId, int price)
	{
		return false;
	}
};
