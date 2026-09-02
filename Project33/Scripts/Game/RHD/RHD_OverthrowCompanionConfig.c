// ============================================================================
// RHD / PROJECT33 - OVERTHROW COMPANION CONFIGURATION
// ============================================================================
// Keep all player-facing tuning here. Mission-specific APIs belong in the
// mission adapter, not in this file.
// ============================================================================
class RHD_OverthrowCompanionConfig
{
	// Feature switches.
	static const bool ENABLE_CONTRACT_BOARD = true;
	static const bool ENABLE_REPUTATION = true;
	static const bool ENABLE_LEGAL_ILLEGAL_MARKET = true;
	static const bool ENABLE_PLAYER_TRADING = true;
	static const bool ENABLE_VEHICLE_OWNERSHIP = true;
	static const bool ENABLE_DYNAMIC_MARKETS = true;
	static const bool ENABLE_TERRITORY_INTELLIGENCE = true;
	static const bool ENABLE_PROPERTY_BENEFITS = true;
	static const bool ENABLE_LOGISTICS_CONTRACTS = true;
	static const bool ENABLE_LICENSES = true;
	static const bool ENABLE_INSURANCE = true;
	static const bool ENABLE_LOANS = true;
	static const bool ENABLE_BLACK_MARKET_DEALERS = true;
	static const bool ENABLE_PLAYER_STATISTICS = true;
	static const bool ENABLE_ATAK_NOTIFICATIONS = true;

	// Contract tuning.
	static const int MAX_ACTIVE_CONTRACTS = 8;
	static const int DEFAULT_CONTRACT_REWARD = 750;
	static const int LOGISTICS_CONTRACT_BONUS = 500;

	// Reputation tuning.
	static const int STARTING_REPUTATION = 0;
	static const int MIN_REPUTATION = -100;
	static const int MAX_REPUTATION = 100;
	static const int LEGAL_REP_DELTA = 1;
	static const int ILLEGAL_REP_DELTA = -1;

	// Market tuning. 100 = normal price.
	static const int MARKET_BASE_MULTIPLIER = 100;
	static const int MARKET_MIN_MULTIPLIER = 60;
	static const int MARKET_MAX_MULTIPLIER = 180;
	static const int MARKET_STEP = 10;

	// Trade / ownership limits.
	static const int MAX_OPEN_TRADE_OFFERS = 10;
	static const int MAX_OWNED_VEHICLES = 30;
	static const int MAX_LICENSES = 12;
	static const int MAX_ACTIVE_LOANS = 4;
	static const int MAX_INSURED_VEHICLES = 20;

	// Finance.
	static const int LOAN_MIN_PRINCIPAL = 1000;
	static const int LOAN_MAX_PRINCIPAL = 1000000;
	static const int LOAN_INTEREST_PERCENT = 12;
	static const int INSURANCE_BASE_PREMIUM = 250;
	static const int INSURANCE_CLAIM_FEE = 500;

	// Default black-market risk premium.
	static const int BLACK_MARKET_PRICE_PREMIUM_PERCENT = 35;
};
