class GRS_ATAKFeatures
{
	// Stateless game rules used by the menu/server controller.
	// All calls that affect shared game state should execute on the server.

	static int CalculateKillReward() { return GRS_ATAKConfig.KILL_REWARD; }
	static int CalculateReviveReward() { return GRS_ATAKConfig.REVIVE_REWARD; }
	static int CalculateBaseSupplyReward() { return GRS_ATAKConfig.BASE_SUPPLY_REWARD; }
	static int GetMortarCost() { return GRS_ATAKConfig.MORTAR_COST; }

	static bool IsValidCasinoBet(int amount, int minimum)
	{
		return amount >= minimum;
	}

	static int BlackjackPayout(bool playerWon, bool blackjack, int stake)
	{
		if (stake <= 0)
			return 0;
		if (blackjack)
			return stake + Math.Round(stake * 1.5);
		if (playerWon)
			return stake * 2;
		return 0;
	}

	static int RoulettePayout(int winningNumber, int selectedNumber, int stake)
	{
		if (stake <= 0 || selectedNumber < 0 || selectedNumber > 36)
			return 0;
		return winningNumber == selectedNumber ? stake * 36 : 0;
	}

	static int ScratchPayout(int roll, int stake)
	{
		if (stake <= 0)
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

class GRS_ATAKHandlerRequest
{
	string m_sPrefab;
	string m_sName;
	string m_sPrimaryWeapon;
	string m_sSecondaryWeapon;
	string m_sLoadout;
	vector m_vTarget;

	void GRS_ATAKHandlerRequest(string name = "")
	{
		m_sName = name;
	}
};

class GRS_ATAKMapState
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
};
