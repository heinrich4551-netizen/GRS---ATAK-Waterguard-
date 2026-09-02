class GRS_ATAKPlayerState
{
	protected int m_iMoney;
	protected int m_iSessionKills;
	protected int m_iSessionRevives;
	protected int m_iSessionSupplies;
	protected int m_iSessionSpent;
	protected int m_iDailyLottoDay = -1;

	void GRS_ATAKPlayerState(int startingMoney = 0)
	{
		m_iMoney = Math.Max(0, startingMoney);
	}

	int GetMoney() { return m_iMoney; }
	int GetKills() { return m_iSessionKills; }
	int GetRevives() { return m_iSessionRevives; }
	int GetSupplies() { return m_iSessionSupplies; }
	int GetSpent() { return m_iSessionSpent; }

	bool CanAfford(int amount)
	{
		return amount >= 0 && m_iMoney >= amount;
	}

	bool TrySpend(int amount)
	{
		if (!CanAfford(amount))
			return false;

		m_iMoney -= amount;
		m_iSessionSpent += amount;
		return true;
	}

	void AddMoney(int amount)
	{
		if (amount > 0)
			m_iMoney += amount;
	}

	void RecordKill()
	{
		m_iSessionKills++;
		AddMoney(GRS_ATAKConfig.KILL_REWARD);
	}

	void RecordRevive()
	{
		m_iSessionRevives++;
		AddMoney(GRS_ATAKConfig.REVIVE_REWARD);
	}

	void RecordSupply()
	{
		m_iSessionSupplies++;
		AddMoney(GRS_ATAKConfig.BASE_SUPPLY_REWARD);
	}

	bool HasClaimedDailyLotto(int day)
	{
		return m_iDailyLottoDay == day;
	}

	void MarkDailyLottoClaimed(int day)
	{
		m_iDailyLottoDay = day;
	}
};
