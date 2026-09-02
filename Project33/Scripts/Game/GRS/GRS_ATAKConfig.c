class GRS_ATAKConfig
{
	// GRS ATAK / Waterguard economy.
	static const int KILL_REWARD = 125;
	static const int REVIVE_REWARD = 4500;
	static const int BASE_SUPPLY_REWARD = 145;

	static const int MORTAR_COST = 1786;
	static const int SUPPLY_DROP_COST = 3500;

	static const int BLACKJACK_MIN_STAKE = 500;
	static const int ROULETTE_MIN_STAKE = 50;
	static const int SCRATCH_CARD_COST = 50;
	static const int DAILY_LOTTO_COST = 100;

	// Maximum number of custom shop entries kept in the session table.
	static const int MAX_SHOP_ITEMS = 328;

	// Handler safety limit. The actual AI prefab/loadout comes from the mission integration.
	static const int MAX_RECRUITED_AI = 1;
};
