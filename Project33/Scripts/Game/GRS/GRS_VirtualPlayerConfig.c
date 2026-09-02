class GRS_VirtualPlayerConfig
{
	// ================================================================
	// EASY SETTINGS - change these numbers only if you want different limits.
	// ================================================================
	static const int MAX_BASE_INVENTORY_SLOTS = 120;
	static const int MAX_VIRTUAL_ITEM_TYPES = 435;
	static const int MAX_PROPERTIES = 10;
	static const int MAX_PROPERTY_STORAGE = 15000;
	static const int STORAGE_PER_OBJECT = 100;

	// 500 feet = 152.4 metres.
	static const float MAX_BUILD_RADIUS_FT = 500.0;
	static const float MAX_BUILD_RADIUS_M = 152.4;

	// A property with a 500 ft build radius costs exactly $15,000,000.
	static const int MAX_PROPERTY_PRICE = 15000000;
	static const int MIN_PROPERTY_PRICE = 1;

	// Buildings in the same player's cluster can share a 500 ft build range.
	static const float BUILDING_CLUSTER_RADIUS_M = 152.4;

	// One generated job is active at a time by default.
	static const int MAX_ACTIVE_JOBS = 1;

	static const int DEFAULT_VIRTUAL_ITEM_PRICE = 250;
	static const int DEFAULT_JOB_REWARD = 750;

	// Property construction categories.
	static const string PROPERTY_OBJECT_STORAGE = "Storage";
	static const string PROPERTY_OBJECT_ARSENAL = "Arsenal";
	static const string PROPERTY_OBJECT_DECORATION = "Decoration";
	static const string PROPERTY_OBJECT_FORTIFICATION = "Fortification";
};
