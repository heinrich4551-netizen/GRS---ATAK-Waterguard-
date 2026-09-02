class GRS_ATAKConfig
{
	// Gameplay economy requested for GRS - ATAK / Waterguard.
	static const int KILL_REWARD = 125;
	static const int REVIVE_REWARD = 4500;
	static const int BASE_SUPPLY_REWARD = 145;
	static const int MORTAR_COST = 1786;
	static const int SUPPLY_DROP_COST = 1000;
	static const int DAILY_LOTTO_COOLDOWN_SECONDS = 86400;
	static const int SCRATCH_COST = 250;
	static const int BLACKJACK_ENTRY_COST = 500;
	static const int ROULETTE_ENTRY_COST = 250;

	// Replace these with the actual prefab/resource names used by the mission.
	static const string MORTAR_PROJECTILE_PREFAB = "";
	static const string SUPPLY_CRATE_PREFAB = "";
	static const string RED_SMOKE_PREFAB = "";

	static const ref array<string> RANDOM_ENEMY_EQUIPMENT = {
		"",
		"",
		"",
		""
	};
};
