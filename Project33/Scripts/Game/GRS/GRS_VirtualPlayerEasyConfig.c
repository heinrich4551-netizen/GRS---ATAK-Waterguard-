class GRS_VirtualPlayerEasyConfig
{
	// ================================================================
	// BEGINNER CONFIGURATION
	// Change values here instead of editing the menu/controller scripts.
	// ================================================================

	// Starting virtual cash for a new player profile.
	static const int STARTING_VIRTUAL_MONEY = 0;

	// Prices are examples. Replace/add catalogue entries in Workbench.
	static const int DEFAULT_VIRTUAL_ITEM_PRICE = 250;
	static const int DEFAULT_JOB_REWARD = 750;
	static const int DEFAULT_STORAGE_OBJECT_PRICE = 500;
	static const int DEFAULT_ARSENAL_PRICE = 2500;
	static const int DEFAULT_DECORATION_PRICE = 100;
	static const int DEFAULT_FORTIFICATION_PRICE = 1000;

	// Building price is calculated automatically from build radius.
	// 500 ft is always capped at $15,000,000.
	static const int PROPERTY_PRICE_AT_500FT = 15000000;

	// Job generation safety distance. The mission should use its civilian
	// interaction system to find actual nearby civilian characters.
	static const float JOB_MAX_INTERACTION_DISTANCE_M = 4.0;

	// Vehicle purchases are unlimited unless the mission chooses to add a cap.
	// This matches the requested "all vehicles available in the mission" model.
	static const bool GARAGE_ALLOW_MULTIPLE_OF_SAME_VEHICLE = true;

	// Set false if your mission does not want players to build property.
	static const bool ENABLE_PROPERTY_BUILDING = true;

	// These labels are also used by the UI and are intentionally plain English.
	static const string TAB_INVENTORY = "Virtual Inventory";
	static const string TAB_GARAGE = "Virtual Garage";
	static const string TAB_JOBS = "Jobs";
	static const string TAB_PROPERTY = "Property";
};
