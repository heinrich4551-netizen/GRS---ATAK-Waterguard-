class GRS_ATAKMenuController
{
	protected static ref GRS_ATAKMenuController s_Instance;
	protected bool m_bOpen;
	protected ref GRS_ATAKPlayerState m_PlayerState;
	protected ref GRS_ATAKMapState m_MapState;
	protected ref array<string> m_aShopItems = {};
	protected ref map<string, int> m_mShopPrices = new map<string, int>();
	protected int m_iRecruitedAI;

	void GRS_ATAKMenuController()
	{
		s_Instance = this;
		m_PlayerState = new GRS_ATAKPlayerState();
		m_MapState = new GRS_ATAKMapState();
		LoadDefaultShop();
	}

	static GRS_ATAKMenuController GetInstance() { return s_Instance; }

	void Initialize()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.AddActionListener("GRS_ATAK_Menu", EActionTrigger.DOWN, OnF6);
	}

	void Shutdown()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.RemoveActionListener("GRS_ATAK_Menu", EActionTrigger.DOWN, OnF6);
		if (s_Instance == this)
			s_Instance = null;
	}

	void OnF6(float value, EActionTrigger reason) { Toggle(); }
	void Toggle()
	{
		m_bOpen = !m_bOpen;
		if (m_bOpen)
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.GRS_ATAK_Menu);
		else
			GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.GRS_ATAK_Menu);
	}

	bool IsOpen() { return m_bOpen; }
	GRS_ATAKPlayerState GetPlayerState() { return m_PlayerState; }
	GRS_ATAKMapState GetMapState() { return m_MapState; }

	protected void LoadDefaultShop()
	{
		int price;
		string itemId;
		array<string> itemIds = {"APPLE","CANNABIS_PLANT","COCA_LEAF","CORN_COB","GRAPES","PEACHES","IRON_ORE","COPPER_ORE","GOLD_ORE","DIAMOND","OIL_SAND","IRON","COPPER","GOLD","OIL","CANNABIS_FLOWER","COCAINE","CANNED_CORN"};
		foreach (itemId : itemIds)
		{
			if (GRS_Shop.GetSellPrice(itemId, price))
				AdminSetShopItem(itemId, price, true);
		}
	}

	bool AdminSetShopItem(string itemId, int price, bool callerIsAdmin)
	{
		if (!callerIsAdmin || itemId.IsEmpty() || price < 0)
			return false;
		if (!m_mShopPrices.Contains(itemId) && m_aShopItems.Count() >= GRS_ATAKConfig.MAX_SHOP_ITEMS)
			return false;
		m_mShopPrices.Set(itemId, price);
		if (!m_aShopItems.Contains(itemId))
			m_aShopItems.Insert(itemId);
		return true;
	}

	int GetShopItemCount() { return m_aShopItems.Count(); }
	string GetShopItem(int index)
	{
		if (index < 0 || index >= m_aShopItems.Count()) return "";
		return m_aShopItems[index];
	}
	int GetShopPrice(string itemId)
	{
		if (!m_mShopPrices.Contains(itemId)) return -1;
		return m_mShopPrices.Get(itemId);
	}

	bool PurchaseShopItem(string itemId)
	{
		if (!m_mShopPrices.Contains(itemId)) return false;
		int price = m_mShopPrices.Get(itemId);
		if (!m_PlayerState.TrySpend(price)) return false;
		if (SpawnPurchasedItemForPlayer(itemId)) return true;
		m_PlayerState.Refund(price);
		return false;
	}

	bool SellVirtualItem(string itemId, int quantity)
	{
		if (!GRS_VirtualPlayerEasyConfig.ALLOW_SHOP_SELLING || quantity <= 0)
			return false;
		int unitPrice;
		if (!GRS_Shop.GetSellPrice(itemId, unitPrice))
			return false;
		m_PlayerState.AddMoney(unitPrice * quantity);
		return true;
	}

	int PlayBlackjack(int stake)
	{
		if (!GRS_ATAKFeatures.IsValidBet(stake, GRS_ATAKConfig.BLACKJACK_MIN_STAKE)) return 0;
		if (!m_PlayerState.TrySpend(stake)) return 0;
		int roll = Math.RandomInt(0, 100);
		int payout = GRS_ATAKFeatures.BlackjackPayout(roll < 48, roll < 5, stake);
		m_PlayerState.AddMoney(payout);
		return payout;
	}

	int PlayRoulette(int stake, int selectedNumber)
	{
		if (!GRS_ATAKFeatures.IsValidBet(stake, GRS_ATAKConfig.ROULETTE_MIN_STAKE) || selectedNumber < 0 || selectedNumber > 36) return 0;
		if (!m_PlayerState.TrySpend(stake)) return 0;
		int payout = GRS_ATAKFeatures.RoulettePayout(Math.RandomInt(0, 37), selectedNumber, stake);
		m_PlayerState.AddMoney(payout);
		return payout;
	}

	int BuyScratchCard()
	{
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.SCRATCH_CARD_COST)) return 0;
		int payout = GRS_ATAKFeatures.ScratchPayout(Math.RandomInt(0, 100), GRS_ATAKConfig.SCRATCH_CARD_COST);
		m_PlayerState.AddMoney(payout);
		return payout;
	}

	bool ClaimDailyLotto(int dayKey, int playerTicket)
	{
		if (m_PlayerState.HasClaimedDailyLotto(dayKey) || !m_PlayerState.TrySpend(GRS_ATAKConfig.DAILY_LOTTO_COST)) return false;
		m_PlayerState.MarkDailyLottoClaimed(dayKey);
		if (playerTicket == Math.RandomInt(0, 1000000)) m_PlayerState.AddMoney(100000);
		return true;
	}

	bool RecruitAI(GRS_ATAKHandlerRequest request)
	{
		if (m_iRecruitedAI >= GRS_ATAKConfig.MAX_RECRUITED_AI || request.m_sPrefab.IsEmpty() || !SpawnHandlerAI(request)) return false;
		m_iRecruitedAI++;
		return true;
	}
	bool RequestMortar(vector target)
	{
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.MORTAR_COST) || !CallPreciseMortar(target)) { m_PlayerState.Refund(GRS_ATAKConfig.MORTAR_COST); return false; }
		return true;
	}
	bool RequestSupplyDrop(vector target)
	{
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.SUPPLY_DROP_COST) || !SpawnSupplyDropWithRedSmoke(target)) { m_PlayerState.Refund(GRS_ATAKConfig.SUPPLY_DROP_COST); return false; }
		return true;
	}
	void RecordEnemyKill() { m_PlayerState.RecordKill(); }
	void RecordEnemyRevive() { m_PlayerState.RecordRevive(); }
	void RecordBaseSupply() { m_PlayerState.RecordSupply(); }

	protected bool SpawnPurchasedItemForPlayer(string itemId) { return !itemId.IsEmpty(); }
	protected bool SpawnHandlerAI(GRS_ATAKHandlerRequest request) { return !request.m_sPrefab.IsEmpty(); }
	protected bool CallPreciseMortar(vector target) { return true; }
	protected bool SpawnSupplyDropWithRedSmoke(vector target) { return true; }
};
