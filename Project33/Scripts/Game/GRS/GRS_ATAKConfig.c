class GRS_ATAKConfig
{
	// GRS ATAK / Waterguard economy.
	static const int KILL_REWARD = 125;
	static const int REVIVE_REWARD = 4500;
	static const int BASE_SUPPLY_REWARD = 145;

	static const int MORTAR_COST = 1786;
	static const int SUPPLY_DROP_COST = 1000;

	static const int BLACKJACK_MIN_STAKE = 500;
	static const int ROULETTE_MIN_STAKE = 250;
	static const int SCRATCH_CARD_COST = 250;
	static const int DAILY_LOTTO_COST = 100;

	// Daily lotto is one claim per real-world day/session day key.
	static const int DAILY_LOTTO_COOLDOWN_SECONDS = 86400;

	// Integration values are deliberately empty until the mission supplies real
	// resource IDs. Never put guessed prefab IDs here.
	static const string SUPPLY_CRATE_PREFAB = "";
	static const string RED_SMOKE_PREFAB = "";

	// Maximum number of custom shop entries kept in the session table.
	static const int MAX_SHOP_ITEMS = 128;

	// Handler safety limits. These keep requests bounded by normal Reforger gameplay.
	static const int MAX_RECRUITED_AI = 4;
};
