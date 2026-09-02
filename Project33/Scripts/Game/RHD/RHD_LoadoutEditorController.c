// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR CONTROLLER
// ============================================================================
// Deep arsenal-style editor controller. Project33 owns the UI/model; the real
// inventory/loadout transaction remains delegated to the mission adapter.
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
		if (!ValidateActiveProfile()) return false;
		return m_Mission.ApplyLoadout(m_Model.GetActiveProfile());
	}

	bool Save()
	{
		if (!m_Model || !m_Model.GetActiveProfile() || !ValidateActiveProfile()) return false;
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
		RHD_LoadoutSlot slot = m_Model.FindSlot(slotId);
		if (!slot || itemId.IsEmpty()) return false;
		return SetCategoryItem(slot.m_sCategory, slotId, itemId);
	}

	bool SetCategoryItem(string category, string slotId, string itemId)
	{
		if (category.IsEmpty() || slotId.IsEmpty() || itemId.IsEmpty()) return false;
		if (!m_Mission.IsItemAllowed(category, itemId)) return false;
		return m_Model.SetItem(slotId, itemId);
	}

	bool SetQuantity(string slotId, int quantity) { return m_Model && m_Model.SetQuantity(slotId, quantity); }

	bool AddAttachment(string slotId, string attachmentId)
	{
		RHD_LoadoutSlot slot = m_Model.FindSlot(slotId);
		if (!slot || slot.m_sItemId.IsEmpty() || attachmentId.IsEmpty() || !m_Mission.CanAttach(slot.m_sItemId, attachmentId)) return false;
		return m_Model.AddAttachment(slotId, attachmentId);
	}

	bool RemoveAttachment(string slotId, string attachmentId) { return m_Model.RemoveAttachment(slotId, attachmentId); }

	bool AddLooseItem(string itemId, string displayName, string category, int quantity)
	{
		if (!RHD_LoadoutEditorConfig.ENABLE_LOADOUT_EDITOR || itemId.IsEmpty() || category.IsEmpty() || quantity <= 0) return false;
		if (!m_Mission.IsItemAllowed(category, itemId)) return false;
		return m_Model.AddLooseItem(itemId, displayName, category, quantity);
	}

	bool AddContainerItem(string containerId, string itemId, string displayName, string category, int quantity, int capacity)
	{
		if (!RHD_LoadoutEditorConfig.ENABLE_LOADOUT_EDITOR || containerId.IsEmpty() || itemId.IsEmpty() || category.IsEmpty() || quantity <= 0) return false;
		if (!m_Mission.IsItemAllowed(category, itemId)) return false;
		return m_Model.AddContainerItem(containerId, itemId, displayName, category, quantity, capacity);
	}

	bool RemoveContainerItem(string containerId, string itemId, int quantity) { return m_Model.RemoveContainerItem(containerId, itemId, quantity); }

	bool SetZeroing(string slotId, int meters)
	{
		RHD_LoadoutSlot slot = m_Model.FindSlot(slotId);
		if (!slot || slot.m_sItemId.IsEmpty()) return false;
		if (!m_Mission.SetWeaponZeroing(slot.m_sItemId, meters)) return false;
		return m_Model.SetZeroing(slotId, meters);
	}

	bool SetHolsterState(string state)
	{
		if (!m_Mission.SetHolsterState(state)) return false;
		return m_Model.SetHolsterState(state);
	}

	bool GetCatalog(string category, out array<string> itemIds) { return m_Mission.EnumerateItems(category, itemIds); }

	bool ValidateActiveProfile()
	{
		if (!m_Model || !m_Model.GetActiveProfile()) return false;
		return ValidateProfile(m_Model.GetActiveProfile());
	}

	bool ValidateProfile(RHD_LoadoutProfile profile)
	{
		if (!profile) return false;
		foreach (RHD_LoadoutSlot slot : profile.m_aSlots)
		{
			if (!slot || !slot.m_bEnabled || slot.m_sItemId.IsEmpty()) continue;
			if (slot.m_sCategory == "HOLSTER") continue;
			if (!m_Mission.IsItemAllowed(slot.m_sCategory, slot.m_sItemId)) return false;
			if (slot.m_iQuantity < 0) return false;
			foreach (string attachment : slot.m_aAttachments)
				if (!attachment.IsEmpty() && !m_Mission.CanAttach(slot.m_sItemId, attachment)) return false;
		}
		foreach (RHD_LoadoutContainerEntry entry : profile.m_aContainers)
		{
			if (!entry || entry.m_sItemId.IsEmpty() || entry.m_iQuantity <= 0 || entry.m_iCapacity < entry.m_iQuantity) return false;
			if (!m_Mission.IsItemAllowed(entry.m_sCategory, entry.m_sItemId)) return false;
		}
		foreach (RHD_LoadoutContainerEntry loose : profile.m_aLooseItems)
		{
			if (!loose || loose.m_sItemId.IsEmpty() || loose.m_iQuantity <= 0) return false;
			if (!m_Mission.IsItemAllowed(loose.m_sCategory, loose.m_sItemId)) return false;
		}
		return true;
	}

	string GetActiveProfileSummary()
	{
		RHD_LoadoutProfile profile = m_Model ? m_Model.GetActiveProfile() : null;
		if (!profile) return "NO ACTIVE PROFILE";
		string result = "PROFILE: " + profile.m_sDisplayName + " | EQUIPPED: " + m_Model.CountEquippedItems().ToString();
		result += " | VALID: " + (ValidateProfile(profile) ? "YES" : "NO");
		return result;
	}

	string GetCategorySummary()
	{
		array<RHD_LoadoutEditorCategory> categories;
		RHD_LoadoutEditorCatalog.Build(categories);
		string result = "";
		foreach (RHD_LoadoutEditorCategory category : categories)
		{
			if (!category) continue;
			if (!result.IsEmpty()) result += "\n";
			result += category.m_sId + " | " + category.m_sDisplayName + " | " + category.m_sDependency;
		}
		return result;
	}

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
