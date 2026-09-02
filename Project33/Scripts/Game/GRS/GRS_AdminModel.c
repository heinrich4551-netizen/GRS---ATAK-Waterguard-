enum RHD_AdminTab
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

class RHD_AdminModel
{
	static string GetTabName(RHD_AdminTab tab)
	{
		switch (tab)
		{
			case RHD_AdminTab.PLAYERS: return "Players";
			case RHD_AdminTab.MODERATION: return "Moderation";
			case RHD_AdminTab.TELEPORT: return "Teleport";
			case RHD_AdminTab.GAMEPLAY: return "Gameplay";
			case RHD_AdminTab.SPAWN: return "Spawn";
			case RHD_AdminTab.WORLD: return "World";
			case RHD_AdminTab.ECONOMY: return "Economy";
			case RHD_AdminTab.SERVER: return "Server";
			case RHD_AdminTab.DIAGNOSTICS: return "Diagnostics";
		}
		return "Admin";
	}
	static string GetTabSummary(RHD_AdminTab tab)
	{
		switch (tab)
		{
			case RHD_AdminTab.PLAYERS: return "Player list, follow/spectate, heal, revive, kill and teleport targets.";
			case RHD_AdminTab.MODERATION: return "Warn, mute, kick and ban request hooks. Server authority required.";
			case RHD_AdminTab.TELEPORT: return "Move yourself or a selected player using mission-safe server hooks.";
			case RHD_AdminTab.GAMEPLAY: return "God mode, invulnerability, stamina, ammo and vehicle-control hooks.";
			case RHD_AdminTab.SPAWN: return "Spawn configured vehicles, AI, supplies and mission assets.";
			case RHD_AdminTab.WORLD: return "Time, weather, fog, visibility and map-state controls.";
			case RHD_AdminTab.ECONOMY: return "Grant/remove money, shop administration and economy diagnostics.";
			case RHD_AdminTab.SERVER: return "Server notices, mission flow, save/restart and maintenance hooks.";
			case RHD_AdminTab.DIAGNOSTICS: return "Authority state, script status, entity counts and admin action log.";
		}
		return "";
	}
};
