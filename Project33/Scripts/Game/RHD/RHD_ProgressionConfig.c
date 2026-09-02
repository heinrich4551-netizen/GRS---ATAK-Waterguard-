// ============================================================================
// RHD / PROJECT33 - PROGRESSION CONFIG
// ============================================================================
// Easy server-balancing knobs. Keep progression independent from the equipment,
// stamina, weight, admin and economy implementations owned by other mods.
// ============================================================================
class RHD_ProgressionConfig
{
	static const int XP_PER_LEVEL_BASE = 1000;
	static const int XP_PER_LEVEL_STEP = 500;
	static const int MAX_LEVEL = 100;
	static const int MIN_REPUTATION = -1000;
	static const int MAX_REPUTATION = 1000;
	static const int CONTRACT_XP_BASE = 250;
	static const int CONTRACT_REPUTATION_BASE = 10;
	static const int CONTRACT_FAILURE_REPUTATION_PENALTY = 5;
	static const bool ENABLE_PROGRESS_UNLOCKS = true;
	static const int LEVEL_FOR_ADVANCED_CONTRACTS = 5;
	static const int LEVEL_FOR_PREMIUM_CONTRACTS = 15;
	static const int LEVEL_FOR_PROPERTY_UPGRADES = 10;
	static const int LEVEL_FOR_LOGISTICS = 8;

	static int GetXPForNextLevel(int currentLevel)
	{
		if (currentLevel < 1) currentLevel = 1;
		return XP_PER_LEVEL_BASE + ((currentLevel - 1) * XP_PER_LEVEL_STEP);
	}
};

class RHD_ProgressionUnlocks
{
	static bool HasAdvancedContracts(int level) { return !RHD_ProgressionConfig.ENABLE_PROGRESS_UNLOCKS || level >= RHD_ProgressionConfig.LEVEL_FOR_ADVANCED_CONTRACTS; }
	static bool HasPremiumContracts(int level) { return !RHD_ProgressionConfig.ENABLE_PROGRESS_UNLOCKS || level >= RHD_ProgressionConfig.LEVEL_FOR_PREMIUM_CONTRACTS; }
	static bool HasPropertyUpgrades(int level) { return !RHD_ProgressionConfig.ENABLE_PROGRESS_UNLOCKS || level >= RHD_ProgressionConfig.LEVEL_FOR_PROPERTY_UPGRADES; }
	static bool HasLogistics(int level) { return !RHD_ProgressionConfig.ENABLE_PROGRESS_UNLOCKS || level >= RHD_ProgressionConfig.LEVEL_FOR_LOGISTICS; }
};
