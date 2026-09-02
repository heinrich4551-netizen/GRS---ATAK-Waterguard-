enum GRS_AdminTab
{
	PLAYERS,
	MODERATION,
	TELEPORT,
	GAMEPLAY,
	SPAWN,
	WORLD,
	ECONOMY,
	SERVER,
	DIAGNOSTICS
};

class GRS_AdminModel
{
	static string GetTabName(GRS_AdminTab tab)
	{
		switch (tab)
		{
			case GRS_AdminTab.PLAYERS: return "Players";
			case GRS_AdminTab.MODERATION: return "Moderation";
			case GRS_AdminTab.TELEPORT: return "Teleport";
			case GRS_AdminTab.GAMEPLAY: return "Gameplay";
			case GRS_AdminTab.SPAWN: return "Spawn";
			case GRS_AdminTab.WORLD: return "World";
			case GRS_AdminTab.ECONOMY: return "Economy";
			case GRS_AdminTab.SERVER: return "Server";
			case GRS_AdminTab.DIAGNOSTICS: return "Diagnostics";
		}
		return "Admin";
	}

	static string GetTabSummary(GRS_AdminTab tab)
	{
		switch (tab)
		{
			case GRS_AdminTab.PLAYERS:
				return "Player list, follow/spectate, heal, revive, kill and teleport targets.";
			case GRS_AdminTab.MODERATION:
				return "Warn, mute, kick and ban request hooks. Server authority required.";
			case GRS_AdminTab.TELEPORT:
				return "Move yourself or a selected player using mission-safe server hooks.";
			case GRS_AdminTab.GAMEPLAY:
				return "God mode, invulnerability, stamina, ammo and vehicle-control hooks.";
			case GRS_AdminTab.SPAWN:
				return "Spawn configured vehicles, AI, supplies and mission assets.";
			case GRS_AdminTab.WORLD:
				return "Time, weather, fog, visibility and map-state controls.";
			case GRS_AdminTab.ECONOMY:
				return "Grant/remove money, shop administration and economy diagnostics.";
			case GRS_AdminTab.SERVER:
				return "Server notices, mission flow, save/restart and maintenance hooks.";
			case GRS_AdminTab.DIAGNOSTICS:
				return "Authority state, script status, entity counts and admin action log.";
		}
		return "";
	}
};
