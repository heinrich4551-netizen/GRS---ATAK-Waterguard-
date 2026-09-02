class GRS_ATAKMenuModel
{
	static const string TAB_SHOP = "Shop";
	static const string TAB_CASINO = "Casino";
	static const string TAB_MAP = "Map";
	static const string TAB_STATS = "Stats";
	static const string TAB_HANDLER = "Handler";

	static const string CASINO_BLACKJACK = "Blackjack";
	static const string CASINO_ROULETTE = "Roulette";
	static const string CASINO_SCRATCH = "Scratch Cards";
	static const string CASINO_LOTTO = "Daily Lotto";

	static const string HANDLER_RECRUIT = "Recruit AI";
	static const string HANDLER_MORTAR = "Mortar Fire";
	static const string HANDLER_SUPPLY = "Supply Drop";

	static array<string> GetTabs()
	{
		return {TAB_SHOP, TAB_CASINO, TAB_MAP, TAB_STATS, TAB_HANDLER};
	}

	static array<string> GetCasinoGames()
	{
		return {CASINO_BLACKJACK, CASINO_ROULETTE, CASINO_SCRATCH, CASINO_LOTTO};
	}

	static array<string> GetHandlerActions()
	{
		return {HANDLER_RECRUIT, HANDLER_MORTAR, HANDLER_SUPPLY};
	}
};
