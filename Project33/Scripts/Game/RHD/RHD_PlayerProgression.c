// ============================================================================
// RHD / PROJECT33 - PLAYER PROGRESSION + REPUTATION
// ============================================================================
class RHD_PlayerProgression
{
	int m_iLevel = 1;
	int m_iXP = 0;
	int m_iContractsCompleted = 0;
	int m_iContractsFailed = 0;
	int m_iTotalEarnings = 0;
	int m_iReputationCivilian = 0;
	int m_iReputationMilitary = 0;
	int m_iReputationCriminal = 0;
	int m_iReputationContractor = 0;

	int GetLevel() { return m_iLevel; }
	int GetXP() { return m_iXP; }
	int GetReputationCivilian() { return m_iReputationCivilian; }
	int GetReputationMilitary() { return m_iReputationMilitary; }
	int GetReputationCriminal() { return m_iReputationCriminal; }
	int GetReputationContractor() { return m_iReputationContractor; }

	int AddXP(int amount)
	{
		if (amount <= 0) return 0;
		if (m_iLevel >= RHD_ProgressionConfig.MAX_LEVEL) return 0;
		int before = m_iXP;
		m_iXP += amount;
		while (m_iLevel < RHD_ProgressionConfig.MAX_LEVEL && m_iXP >= RHD_ProgressionConfig.GetXPForNextLevel(m_iLevel))
		{
			m_iXP -= RHD_ProgressionConfig.GetXPForNextLevel(m_iLevel);
			m_iLevel += 1;
		}
		return m_iXP - before;
	}

	void AddReputation(string faction, int amount)
	{
		if (faction == "CIVILIAN") m_iReputationCivilian = Math.Clamp(m_iReputationCivilian + amount, RHD_ProgressionConfig.MIN_REPUTATION, RHD_ProgressionConfig.MAX_REPUTATION);
		else if (faction == "MILITARY") m_iReputationMilitary = Math.Clamp(m_iReputationMilitary + amount, RHD_ProgressionConfig.MIN_REPUTATION, RHD_ProgressionConfig.MAX_REPUTATION);
		else if (faction == "CRIMINAL") m_iReputationCriminal = Math.Clamp(m_iReputationCriminal + amount, RHD_ProgressionConfig.MIN_REPUTATION, RHD_ProgressionConfig.MAX_REPUTATION);
		else if (faction == "CONTRACTOR") m_iReputationContractor = Math.Clamp(m_iReputationContractor + amount, RHD_ProgressionConfig.MIN_REPUTATION, RHD_ProgressionConfig.MAX_REPUTATION);
	}

	void RecordContractCompleted(int reward)
	{
		m_iContractsCompleted += 1;
		if (reward > 0) m_iTotalEarnings += reward;
	}

	void RecordContractFailed() { m_iContractsFailed += 1; }

	string GetSummary()
	{
		return "LVL " + m_iLevel.ToString() + " | XP " + m_iXP.ToString() + "/" + RHD_ProgressionConfig.GetXPForNextLevel(m_iLevel).ToString() +
			" | REP C:" + m_iReputationCivilian.ToString() + " M:" + m_iReputationMilitary.ToString() +
			" CR:" + m_iReputationCriminal.ToString() + " K:" + m_iReputationContractor.ToString();
	}
};
