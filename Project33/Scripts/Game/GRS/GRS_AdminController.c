class RHD_AdminActionRequest
{
	string m_sAction;
	string m_sTarget;
	string m_sValue;
	vector m_vPosition;
	void RHD_AdminActionRequest(string action, string target = "", string value = "") { m_sAction = action; m_sTarget = target; m_sValue = value; m_vPosition = "0 0 0"; }
};

class RHD_AdminController
{
	protected bool m_bOpen;
	protected bool m_bAuthorized;
	protected ref RHD_AdminModel m_Model;
	protected ref array<string> m_aActionLog = {};
	protected int m_iSelectedPlayer = -1;
	protected RHD_AdminTab m_eTab = RHD_AdminTab.PLAYERS;
	void RHD_AdminController() { m_Model = new RHD_AdminModel(); }
	void Initialize() { InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.AddActionListener("RHD_Admin_Menu", EActionTrigger.DOWN, OnF7); }
	void Shutdown() { InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.RemoveActionListener("RHD_Admin_Menu", EActionTrigger.DOWN, OnF7); }
	void OnF7(float value, EActionTrigger reason) { RefreshAuthorization(); if (!m_bAuthorized) return; Toggle(); }
	void RefreshAuthorization() { m_bAuthorized = IsAuthorizedAdmin(); }
	bool IsAuthorizedAdmin() { return false; }
	void Toggle() { m_bOpen = !m_bOpen; if (m_bOpen) GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.RHD_Admin_Menu); else GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.RHD_Admin_Menu); }
	bool IsOpen() { return m_bOpen; }
	bool IsAuthorized() { return m_bAuthorized; }
	RHD_AdminTab GetTab() { return m_eTab; }
	void SetTab(RHD_AdminTab tab) { m_eTab = tab; }
	void SetSelectedPlayer(int playerId) { m_iSelectedPlayer = playerId; }
	int GetSelectedPlayer() { return m_iSelectedPlayer; }
	string GetTabSummary() { return RHD_AdminModel.GetTabSummary(m_eTab); }
	int GetActionLogCount() { return m_aActionLog.Count(); }
	string GetActionLog(int index) { if (index < 0 || index >= m_aActionLog.Count()) return ""; return m_aActionLog[index]; }
	bool Execute(RHD_AdminActionRequest request)
	{
		RefreshAuthorization();
		if (!m_bAuthorized || !request || request.m_sAction.IsEmpty() || !ValidateRequest(request)) return false;
		bool success = DispatchServerAction(request);
		if (success) LogAction(request);
		return success;
	}
	protected bool ValidateRequest(RHD_AdminActionRequest request)
	{
		if (request.m_sAction.IsEmpty()) return false;
		if (request.m_sAction == "GrantMoney" || request.m_sAction == "RemoveMoney")
		{
			int amount = request.m_sValue.ToInt();
			if (amount < 0 || amount > RHD_AdminConfig.MAX_MONEY_GRANT) return false;
		}
		if (request.m_sAction == "Heal")
		{
			int amount = request.m_sValue.ToInt();
			if (amount < 0 || amount > RHD_AdminConfig.MAX_HEAL_AMOUNT) return false;
		}
		return true;
	}
	protected bool DispatchServerAction(RHD_AdminActionRequest request)
	{
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
	protected void LogAction(RHD_AdminActionRequest request)
	{
		m_aActionLog.Insert(request.m_sAction + " target=" + request.m_sTarget + " value=" + request.m_sValue);
		while (m_aActionLog.Count() > RHD_AdminConfig.MAX_ACTION_LOG_ENTRIES) m_aActionLog.Remove(0);
	}
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
	protected bool ServerSetShopPrice(string itemId, int price)
	{
		RHD_ATAKMenuController shop = RHD_ATAKMenuController.GetInstance();
		if (!shop) return false;
		return shop.AdminSetShopItem(itemId, price, true);
	}
	protected bool ServerEndMission() { return false; }
	protected bool ServerRestartMission() { return false; }
	protected bool ServerSave() { return false; }
	protected bool ServerClearAI() { return false; }
	protected bool ServerClearVehicles() { return false; }
	protected bool ServerSetDamage(string target, bool enabled) { return false; }
};
