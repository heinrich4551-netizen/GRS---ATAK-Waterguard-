class GRS_AdminActionRequest
{
	string m_sAction;
	string m_sTarget;
	string m_sValue;
	vector m_vPosition;

	void GRS_AdminActionRequest(string action, string target = "", string value = "")
	{
		m_sAction = action;
		m_sTarget = target;
		m_sValue = value;
		m_vPosition = "0 0 0";
	}
};

class GRS_AdminController
{
	protected bool m_bOpen;
	protected bool m_bAuthorized;
	protected ref GRS_AdminModel m_Model;
	protected ref array<string> m_aActionLog = {};
	protected int m_iSelectedPlayer = -1;
	protected GRS_AdminTab m_eTab = GRS_AdminTab.PLAYERS;

	void GRS_AdminController()
	{
		m_Model = new GRS_AdminModel();
	}

	void Initialize()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.AddActionListener("GRS_Admin_Menu", EActionTrigger.DOWN, OnF7);
	}

	void Shutdown()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.RemoveActionListener("GRS_Admin_Menu", EActionTrigger.DOWN, OnF7);
	}

	void OnF7(float value, EActionTrigger reason)
	{
		// Authorization is checked every time the menu is requested. This prevents
		// a stale local UI state from becoming an authorization bypass.
		RefreshAuthorization();
		if (!m_bAuthorized)
			return;
		Toggle();
	}

	void RefreshAuthorization()
	{
		m_bAuthorized = IsAuthorizedAdmin();
	}

	bool IsAuthorizedAdmin()
	{
		// Fail closed. Override this method in the mission's server-authority layer
		// and validate the caller against the actual Reforger/server permission source.
		return false;
	}

	void Toggle()
	{
		m_bOpen = !m_bOpen;
		if (m_bOpen)
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.GRS_Admin_Menu);
		else
			GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.GRS_Admin_Menu);
	}

	bool IsOpen() { return m_bOpen; }
	bool IsAuthorized() { return m_bAuthorized; }
	GRS_AdminTab GetTab() { return m_eTab; }
	void SetTab(GRS_AdminTab tab) { m_eTab = tab; }
	void SetSelectedPlayer(int playerId) { m_iSelectedPlayer = playerId; }
	int GetSelectedPlayer() { return m_iSelectedPlayer; }

	string GetTabSummary()
	{
		return GRS_AdminModel.GetTabSummary(m_eTab);
	}

	int GetActionLogCount() { return m_aActionLog.Count(); }
	string GetActionLog(int index)
	{
		if (index < 0 || index >= m_aActionLog.Count())
			return "";
		return m_aActionLog[index];
	}

	bool Execute(GRS_AdminActionRequest request)
	{
		RefreshAuthorization();
		if (!m_bAuthorized || !request || request.m_sAction.IsEmpty())
			return false;

		if (!ValidateRequest(request))
			return false;

		bool success = DispatchServerAction(request);
		if (success)
			LogAction(request);
		return success;
	}

	protected bool ValidateRequest(GRS_AdminActionRequest request)
	{
		if (request.m_sAction.IsEmpty())
			return false;

		if (request.m_sAction == "GrantMoney" || request.m_sAction == "RemoveMoney")
		{
			int amount = request.m_sValue.ToInt();
			if (amount < 0 || amount > GRS_AdminConfig.MAX_MONEY_GRANT)
				return false;
		}

		if (request.m_sAction == "Heal")
		{
			int amount = request.m_sValue.ToInt();
			if (amount < 0 || amount > GRS_AdminConfig.MAX_HEAL_AMOUNT)
				return false;
		}

		if (request.m_sAction == "TeleportSelf" || request.m_sAction == "TeleportTarget" || request.m_sAction == "Spawn")
		{
			// Position validation is intentionally conservative. The mission hook must
			// perform terrain, safe-zone and line-of-sight checks appropriate to its map.
			if (request.m_vPosition == "0 0 0")
				return false;
		}

		return true;
	}

	protected bool DispatchServerAction(GRS_AdminActionRequest request)
	{
		// Every action below is a server-authoritative integration point. The base mod
		// never guesses undocumented engine APIs, player identifiers, prefab GUIDs or
		// moderation backends. Missions override these hooks with their real systems.
		switch (request.m_sAction)
		{
			case "Heal": return ServerHeal(request.m_sTarget, request.m_sValue.ToInt());
			case "Revive": return ServerRevive(request.m_sTarget);
			case "Kill": return ServerKill(request.m_sTarget);
			case "TeleportSelf": return ServerTeleportSelf(request.m_vPosition);
			case "TeleportTarget": return ServerTeleportTarget(request.m_sTarget, request.m_vPosition);
			case "TeleportToTarget": return ServerTeleportSelfToTarget(request.m_sTarget);
			case "BringTarget": return ServerBringTarget(request.m_sTarget);
			case "Warn": return ServerWarn(request.m_sTarget, request.m_sValue);
			case "Mute": return ServerMute(request.m_sTarget, request.m_sValue.ToInt());
			case "Kick": return ServerKick(request.m_sTarget, request.m_sValue);
			case "Ban": return ServerBan(request.m_sTarget, request.m_sValue);
			case "Unban": return ServerUnban(request.m_sTarget);
			case "Freeze": return ServerFreeze(request.m_sTarget, request.m_sValue.ToInt() != 0);
			case "GodMode": return ServerGodMode(request.m_sTarget, request.m_sValue.ToInt() != 0);
			case "RefillAmmo": return ServerRefillAmmo(request.m_sTarget);
			case "RepairVehicle": return ServerRepairVehicle(request.m_sTarget);
			case "FlipVehicle": return ServerFlipVehicle(request.m_sTarget);
			case "Spawn": return ServerSpawn(request.m_sValue, request.m_vPosition);
			case "DeleteTarget": return ServerDeleteTarget(request.m_sTarget);
			case "SetTime": return ServerSetTime(request.m_sValue);
			case "SetWeather": return ServerSetWeather(request.m_sValue);
			case "SetFog": return ServerSetFog(request.m_sValue);
			case "Announce": return ServerAnnounce(request.m_sValue);
			case "GrantMoney": return ServerGrantMoney(request.m_sTarget, request.m_sValue.ToInt());
			case "RemoveMoney": return ServerRemoveMoney(request.m_sTarget, request.m_sValue.ToInt());
			case "SetShopPrice": return ServerSetShopPrice(request.m_sTarget, request.m_sValue.ToInt());
			case "EndMission": return ServerEndMission();
			case "RestartMission": return ServerRestartMission();
			case "SaveServer": return ServerSave();
			case "ClearAI": return ServerClearAI();
			case "ClearVehicles": return ServerClearVehicles();
			case "SetDamage": return ServerSetDamage(request.m_sTarget, request.m_sValue.ToInt() != 0);
		}
		return false;
	}

	protected void LogAction(GRS_AdminActionRequest request)
	{
		m_aActionLog.Insert(request.m_sAction + " target=" + request.m_sTarget + " value=" + request.m_sValue);
		while (m_aActionLog.Count() > GRS_AdminConfig.MAX_ACTION_LOG_ENTRIES)
			m_aActionLog.Remove(0);
	}

	// -------------------------------------------------------------------------
	// Mission integration hooks. Override these in the actual mission/server
	// implementation. Base implementations fail safely rather than pretending
	// that a client-side call changed authoritative game state.
	// -------------------------------------------------------------------------
	protected bool ServerHeal(string target, int amount) { return false; }
	protected bool ServerRevive(string target) { return false; }
	protected bool ServerKill(string target) { return false; }
	protected bool ServerTeleportSelf(vector position) { return false; }
	protected bool ServerTeleportTarget(string target, vector position) { return false; }
	protected bool ServerTeleportSelfToTarget(string target) { return false; }
	protected bool ServerBringTarget(string target) { return false; }
	protected bool ServerWarn(string target, string message) { return false; }
	protected bool ServerMute(string target, int seconds) { return false; }
	protected bool ServerKick(string target, string reason) { return false; }
	protected bool ServerBan(string target, string reason) { return false; }
	protected bool ServerUnban(string target) { return false; }
	protected bool ServerFreeze(string target, bool enabled) { return false; }
	protected bool ServerGodMode(string target, bool enabled) { return false; }
	protected bool ServerRefillAmmo(string target) { return false; }
	protected bool ServerRepairVehicle(string target) { return false; }
	protected bool ServerFlipVehicle(string target) { return false; }
	protected bool ServerSpawn(string prefabId, vector position) { return false; }
	protected bool ServerDeleteTarget(string target) { return false; }
	protected bool ServerSetTime(string value) { return false; }
	protected bool ServerSetWeather(string value) { return false; }
	protected bool ServerSetFog(string value) { return false; }
	protected bool ServerAnnounce(string message) { return false; }
	protected bool ServerGrantMoney(string target, int amount) { return false; }
	protected bool ServerRemoveMoney(string target, int amount) { return false; }
	protected bool ServerSetShopPrice(string itemId, int price) { return false; }
	protected bool ServerEndMission() { return false; }
	protected bool ServerRestartMission() { return false; }
	protected bool ServerSave() { return false; }
	protected bool ServerClearAI() { return false; }
	protected bool ServerClearVehicles() { return false; }
	protected bool ServerSetDamage(string target, bool enabled) { return false; }
};
