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

	// This boolean must come from the server's real admin/permission system.
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
		if (!SpawnPurchasedItemForPlayer(itemId))
			return false;
		if (!m_PlayerState.TrySpend(price))
			return false;
		return true;
	}

	bool PlayBlackjack(int stake, bool playerWon, bool blackjack)
	{
		if (!GRS_ATAKFeatures.IsValidBet(stake, GRS_ATAKConfig.BLACKJACK_MIN_STAKE))
			return false;
		if (!m_PlayerState.TrySpend(stake))
			return false;
		m_PlayerState.AddMoney(GRS_ATAKFeatures.BlackjackPayout(playerWon, blackjack, stake));
		return true;
	}

	bool PlayRoulette(int stake, int selectedNumber, int winningNumber)
	{
		if (!GRS_ATAKFeatures.IsValidBet(stake, GRS_ATAKConfig.ROULETTE_MIN_STAKE))
			return false;
		if (!m_PlayerState.TrySpend(stake))
			return false;
		m_PlayerState.AddMoney(GRS_ATAKFeatures.RoulettePayout(winningNumber, selectedNumber, stake));
		return true;
	}

	bool BuyScratchCard(int roll)
	{
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.SCRATCH_CARD_COST))
			return false;
		m_PlayerState.AddMoney(GRS_ATAKFeatures.ScratchPayout(roll, GRS_ATAKConfig.SCRATCH_CARD_COST));
		return true;
	}

	bool ClaimDailyLotto(int dayKey, int winningTicket, int playerTicket, int jackpot)
	{
		if (m_PlayerState.HasClaimedDailyLotto(dayKey))
			return false;
		if (!m_PlayerState.TrySpend(GRS_ATAKConfig.DAILY_LOTTO_COST))
			return false;

		m_PlayerState.MarkDailyLottoClaimed(dayKey);
		if (winningTicket == playerTicket && jackpot > 0)
			m_PlayerState.AddMoney(jackpot);
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

	// Mission integration hooks. Keep these as the only places that touch mission
	// inventory, AI, fire-support, crate and smoke implementations.
	protected bool SpawnPurchasedItemForPlayer(string itemId) { return !itemId.IsEmpty(); }
	protected bool SpawnHandlerAI(GRS_ATAKHandlerRequest request) { return !request.m_sPrefab.IsEmpty(); }
	protected bool CallPreciseMortar(vector target) { return true; }
	protected bool SpawnSupplyDropWithRedSmoke(vector target) { return true; }
};
