class GRS_ATAKMenuController
{
	protected bool m_bOpen;
	protected ref GRS_ATAKPlayerState m_PlayerState;
	protected ref GRS_ATAKMapState m_MapState;
	protected ref array<string> m_aShopItems = {};
	protected ref map<string, int> m_mShopPrices = new map<string, int>();
	protected int m_iRecruitedAI;

	void GRS_ATAKMenuController()
	{
		m_PlayerState = new GRS_ATAKPlayerState();
		m_MapState = new GRS_ATAKMapState();
	}

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
	}

	void OnF6(float value, EActionTrigger reason)
	{
		Toggle();
	}

	void Toggle()
	{
		m_bOpen = !m_bOpen;
	}

	bool IsOpen() { return m_bOpen; }
	GRS_ATAKPlayerState GetPlayerState() { return m_PlayerState; }
	GRS_ATAKMapState GetMapState() { return m_MapState; }

	// callerIsAdmin must be obtained from the mission's server-side permission system.
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
		if (index < 0 || index >= m_aShopItems.Count())
			return "";
		return m_aShopItems[index];
	}

	int GetShopPrice(string itemId)
	{
		if (!m_mShopPrices.Contains(itemId))
			return -1;
		return m_mShopPrices.Get(itemId);
	}

	bool PurchaseShopItem(string itemId)
	{
		if (!m_mShopPrices.Contains(itemId))
			return false;

		int price = m_mShopPrices.Get(itemId);
		if (!m_PlayerState.TrySpend(price))
			return false;
		if (SpawnPurchasedItemForPlayer(itemId))
			return true;

		m_PlayerState.Refund(price);
		return false;
	}

	// Server-side casino entry points. Outcomes are generated here, never supplied by
	// an untrusted client. The returned value is the total payout credited to the player.
	int PlayBlackjack(int stake)
	{
		if (!GRS_ATAKFeatures.IsValidBet(stake, GRS_ATAKConfig.BLACKJACK_MIN_STAKE))
			return 0;
		if (!m_PlayerState.TrySpend(stake))
			return 0;

		int roll = Math.RandomInt(0, 100);
		bool blackjack = roll < 5;
		bool playerWon = roll < 48;
		int payout = GRS_ATAKFeatures.BlackjackPayout(playerWon, blackjack, stake);
		m_PlayerState.AddMoney(payout);
		return payout;
	}

	int PlayRoulette(int stake, int selectedNumber)
	{
		if (!GRS_ATAKFeatures.IsValidBet(stake, GRS_ATAKConfig.ROULETTE_MIN_STAKE))
			return 0;
		if (selectedNumber < 0 || selectedNumber > 36)
			return 0;
		if (!m_PlayerState.TrySpend(stake))
			return 0;

		int winningNumber = Math.RandomInt(0, 37);
		int payout = GRS_ATAKFeatures.RoulettePayout(winningNumber, selectedNumber, stake);
		m_PlayerState.AddMoney(payout);
		return payout;
	}

	int BuyScratchCard()
	{
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.SCRATCH_CARD_COST))
			return 0;

		int roll = Math.RandomInt(0, 100);
		int payout = GRS_ATAKFeatures.ScratchPayout(roll, GRS_ATAKConfig.SCRATCH_CARD_COST);
		m_PlayerState.AddMoney(payout);
		return payout;
	}

	bool ClaimDailyLotto(int dayKey, int playerTicket)
	{
		if (m_PlayerState.HasClaimedDailyLotto(dayKey))
			return false;
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.DAILY_LOTTO_COST))
			return false;

		m_PlayerState.MarkDailyLottoClaimed(dayKey);
		int winningTicket = Math.RandomInt(0, 1000000);
		if (playerTicket == winningTicket)
			m_PlayerState.AddMoney(100000);
		return true;
	}

	bool RecruitAI(GRS_ATAKHandlerRequest request)
	{
		if (m_iRecruitedAI >= GRS_ATAKConfig.MAX_RECRUITED_AI || request.m_sPrefab.IsEmpty())
			return false;
		if (!SpawnHandlerAI(request))
			return false;
		m_iRecruitedAI++;
		return true;
	}

	bool RequestMortar(vector target)
	{
		if (!CallPreciseMortar(target))
			return false;
		return m_PlayerState.TrySpend(GRS_ATAKConfig.MORTAR_COST);
	}

	bool RequestSupplyDrop(vector target)
	{
		if (!SpawnSupplyDropWithRedSmoke(target))
			return false;
		return m_PlayerState.TrySpend(GRS_ATAKConfig.SUPPLY_DROP_COST);
	}

	void RecordEnemyKill() { m_PlayerState.RecordKill(); }
	void RecordEnemyRevive() { m_PlayerState.RecordRevive(); }
	void RecordBaseSupply() { m_PlayerState.RecordSupply(); }

	// Mission integration hooks. Replace only these methods with the mission's real
	// inventory, AI, mortar and supply-drop implementations.
	protected bool SpawnPurchasedItemForPlayer(string itemId) { return !itemId.IsEmpty(); }
	protected bool SpawnHandlerAI(GRS_ATAKHandlerRequest request) { return !request.m_sPrefab.IsEmpty(); }
	protected bool CallPreciseMortar(vector target) { return true; }
	protected bool SpawnSupplyDropWithRedSmoke(vector target) { return true; }
};
