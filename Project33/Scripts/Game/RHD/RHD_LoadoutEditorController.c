// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR CONTROLLER
// ============================================================================
class RHD_LoadoutEditorController
{
	protected static ref RHD_LoadoutEditorController s_Instance;
	protected ref RHD_LoadoutEditorModel m_Model;
	protected ref RHD_LoadoutEditorMissionAdapter m_Mission;

	static RHD_LoadoutEditorController GetInstance()
	{
		if (!s_Instance) s_Instance = new RHD_LoadoutEditorController();
		return s_Instance;
	}

	void RHD_LoadoutEditorController()
	{
		m_Model = new RHD_LoadoutEditorModel();
		m_Mission = new RHD_LoadoutEditorMissionAdapter();
	}

	RHD_LoadoutEditorModel GetModel() { return m_Model; }
	RHD_LoadoutEditorMissionAdapter GetMissionAdapter() { return m_Mission; }

	bool RefreshCurrentLoadout()
	{
		if (!m_Model || !m_Model.GetActiveProfile()) return false;
		return m_Mission.GetCurrentLoadout(m_Model.GetActiveProfile());
	}

	bool Apply()
	{
		if (!RHD_LoadoutEditorConfig.ENABLE_LOADOUT_EDITOR || !m_Model || !m_Model.GetActiveProfile()) return false;
		return m_Mission.ApplyLoadout(m_Model.GetActiveProfile());
	}

	bool Save()
	{
		if (!m_Model || !m_Model.GetActiveProfile()) return false;
		return m_Mission.SaveLoadoutProfile(m_Model.GetActiveProfile().m_sProfileId, m_Model.GetActiveProfile());
	}

	bool Load(string profileId)
	{
		if (!m_Model || profileId.IsEmpty() || !m_Model.SelectProfile(profileId)) return false;
		return m_Mission.LoadLoadoutProfile(profileId, m_Model.GetActiveProfile());
	}

	bool Delete(string profileId)
	{
		if (profileId.IsEmpty() || profileId == RHD_LoadoutEditorConfig.PROFILE_DEFAULT) return false;
		return m_Mission.DeleteLoadoutProfile(profileId);
	}

	bool SetItem(string slotId, string itemId)
	{
		if (!m_Mission.IsItemAllowed("ITEM", itemId)) return false;
		return m_Model.SetItem(slotId, itemId);
	}

	bool SetCategoryItem(string category, string slotId, string itemId)
	{
		if (category.IsEmpty() || slotId.IsEmpty() || itemId.IsEmpty() || !m_Mission.IsItemAllowed(category, itemId)) return false;
		return m_Model.SetItem(slotId, itemId);
	}

	bool AddAttachment(string slotId, string attachmentId)
	{
		RHD_LoadoutSlot slot = m_Model.FindSlot(slotId);
		if (!slot || !m_Mission.CanAttach(slot.m_sItemId, attachmentId)) return false;
		return m_Model.AddAttachment(slotId, attachmentId);
	}

	bool RemoveAttachment(string slotId, string attachmentId) { return m_Model.RemoveAttachment(slotId, attachmentId); }
	bool SetZeroing(string slotId, int meters)
	{
		RHD_LoadoutSlot slot = m_Model.FindSlot(slotId);
		if (!slot || !m_Mission.SetWeaponZeroing(slot.m_sItemId, meters)) return false;
		return m_Model.SetZeroing(slotId, meters);
	}
	bool SetHolsterState(string state)
	{
		if (!m_Mission.SetHolsterState(state)) return false;
		return m_Model.SetHolsterState(state);
	}

	bool GetCatalog(string category, out array<string> itemIds) { return m_Mission.EnumerateItems(category, itemIds); }

	string GetDependencySummary()
	{
		string result = "WCS Loadout Editor";
		if (m_Mission.HasWCSWeapons()) result += " | WCS Weapons";
		if (m_Mission.HasWCSAttachments()) result += " | WCS Attachments";
		if (m_Mission.HasWCSScopes()) result += " | WCS Scopes";
		if (m_Mission.HasWCSClothing()) result += " | WCS Clothing";
		if (m_Mission.HasGRSApparel()) result += " | GRS Apparel";
		if (m_Mission.HasGRSVestsRigs()) result += " | GRS Vests/Rigs";
		if (m_Mission.HasGRSBagsBeltsDroplegs()) result += " | GRS Bags/Belts/Droplegs";
		if (m_Mission.HasRHS()) result += " | RHS";
		if (m_Mission.HasRayziOptics()) result += " | Rayzi Optics";
		if (m_Mission.HasRISLasers()) result += " | RIS Lasers";
		if (m_Mission.HasHolstering()) result += " | Holstering";
		if (m_Mission.HasZeroing()) result += " | Advanced Zeroing";
		return result;
	}
};
