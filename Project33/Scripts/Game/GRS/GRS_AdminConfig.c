class GRS_AdminConfig
{
	// The admin menu is deliberately fail-closed. A mission/server integration must
	// authorize the caller before destructive/server-authoritative actions are allowed.
	static const int MAX_ACTION_LOG_ENTRIES = 256;
	static const int MAX_TARGETED_PLAYERS = 128;
	static const int MAX_SPAWN_REQUESTS_PER_ACTION = 16;

	// Safe defaults for admin convenience actions.
	static const float MAX_TELEPORT_DISTANCE = 10000.0;
	static const float MAX_SPAWN_DISTANCE = 250.0;
	static const int MAX_MONEY_GRANT = 1000000;
	static const int MAX_MONEY_REMOVE = 1000000;
	static const int MAX_HEAL_AMOUNT = 10000;

	// Time/weather changes are mission-owned because Reforger scenarios can use
	// different world-state managers and replication policies.
	static const bool ENABLE_WORLD_STATE_HOOKS = true;
};
