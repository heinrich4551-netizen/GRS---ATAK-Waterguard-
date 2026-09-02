class RHD_ATAKFeatures
{
	static int GetKillReward() { return RHD_ATAKConfig.KILL_REWARD; }
	static int GetReviveReward() { return RHD_ATAKConfig.REVIVE_REWARD; }
	static int GetBaseSupplyReward() { return RHD_ATAKConfig.BASE_SUPPLY_REWARD; }
	static int GetMortarCost() { return RHD_ATAKConfig.MORTAR_COST; }

	static bool IsValidBet(int amount, int minimum)
	{
		return amount >= minimum;
	}

	static int BlackjackPayout(bool playerWon, bool blackjack, int stake)
	{
		if (stake <= 0)
			return 0;
		if (blackjack)
			return stake + ((stake * 3) / 2);
		if (playerWon)
			return stake * 2;
		return 0;
	}

	static int RoulettePayout(int winningNumber, int selectedNumber, int stake)
	{
		if (stake <= 0 || winningNumber < 0 || winningNumber > 36 || selectedNumber < 0 || selectedNumber > 36)
			return 0;
		if (winningNumber == selectedNumber)
			return stake * 36;
		return 0;
	}

	static int ScratchPayout(int roll, int stake)
	{
		if (stake <= 0 || roll < 0 || roll > 99)
			return 0;
		if (roll == 0)
			return stake * 10;
		if (roll < 10)
			return stake * 3;
		if (roll < 35)
			return stake;
		return 0;
	}
};

class RHD_ATAKHandlerRequest
{
	string m_sPrefab;
	string m_sName;
	string m_sPrimaryWeapon;
	string m_sSecondaryWeapon;
	string m_sLoadout;
	vector m_vTarget;

	void RHD_ATAKHandlerRequest(string name = "")
	{
		m_sName = name;
	}
};

class RHD_ATAKMapState
{
	protected ref array<vector> m_aPins = {};
	protected ref array<string> m_aLabels = {};
	protected ref array<int> m_aLayers = {};

	void AddPin(vector position, string label, int layer)
	{
		m_aPins.Insert(position);
		m_aLabels.Insert(label);
		m_aLayers.Insert(layer);
	}
	void Clear()
	{
		m_aPins.Clear();
		m_aLabels.Clear();
		m_aLayers.Clear();
	}
	int GetPinCount() { return m_aPins.Count(); }
	vector GetPin(int index)
	{
		if (index < 0 || index >= m_aPins.Count())
			return "0 0 0";
		return m_aPins[index];
	}
	string GetLabel(int index)
	{
		if (index < 0 || index >= m_aLabels.Count())
			return "";
		return m_aLabels[index];
	}
};
