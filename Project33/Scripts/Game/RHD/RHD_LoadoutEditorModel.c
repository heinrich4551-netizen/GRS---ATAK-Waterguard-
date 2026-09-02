// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR MODEL
// ============================================================================
// Arsenal-style loadout data model. It stores the desired loadout and lets the
// mission adapter apply it using real WCS/GRS/RHS inventory resources.
// ============================================================================
class RHD_LoadoutContainerEntry
{
	string m_sContainerId;
	string m_sItemId;
	string m_sDisplayName;
	string m_sCategory;
	int m_iQuantity;
	int m_iCapacity;
};

class RHD_LoadoutSlot
{
	string m_sSlotId;
	string m_sDisplayName;
	string m_sCategory;
	string m_sItemId;
	ref array<string> m_aAttachments = {};
	int m_iZeroing;
	int m_iQuantity;
	bool m_bEnabled;
};

class RHD_LoadoutProfile
{
	string m_sProfileId;
	string m_sDisplayName;
	ref array<RHD_LoadoutSlot> m_aSlots = {};
	ref array<RHD_LoadoutContainerEntry> m_aContainers = {};
	ref array<RHD_LoadoutContainerEntry> m_aLooseItems = {};
};

class RHD_LoadoutEditorModel
{
	protected ref array<RHD_LoadoutProfile> m_aProfiles = {};
	protected ref RHD_LoadoutProfile m_ActiveProfile;

	void RHD_LoadoutEditorModel()
	{
		CreateDefaultProfile(RHD_LoadoutEditorConfig.PROFILE_DEFAULT, "Default");
		CreateDefaultProfile(RHD_LoadoutEditorConfig.PROFILE_MEDIC, "Medic");
		CreateDefaultProfile(RHD_LoadoutEditorConfig.PROFILE_RIFLEMAN, "Rifleman");
		CreateDefaultProfile(RHD_LoadoutEditorConfig.PROFILE_MACHINEGUNNER, "Machinegunner");
		CreateDefaultProfile(RHD_LoadoutEditorConfig.PROFILE_MARKSMAN, "Marksman");
		CreateDefaultProfile(RHD_LoadoutEditorConfig.PROFILE_AA_AT, "AA / AT");
		CreateDefaultProfile(RHD_LoadoutEditorConfig.PROFILE_CUSTOM, "Custom");
		m_ActiveProfile = m_aProfiles[0];
	}

	protected void CreateDefaultProfile(string profileId, string displayName)
	{
		RHD_LoadoutProfile profile = new RHD_LoadoutProfile();
		profile.m_sProfileId = profileId;
		profile.m_sDisplayName = displayName;
		CreateSlots(profile);
		m_aProfiles.Insert(profile);
	}

	protected void CreateSlots(RHD_LoadoutProfile profile)
	{
		AddSlot(profile, "PRIMARY", "Primary Weapon", "WEAPON");
		AddSlot(profile, "PRIMARY_OPTIC", "Primary Optic", "OPTIC");
		AddSlot(profile, "PRIMARY_MUZZLE", "Primary Muzzle", "ATTACHMENT");
		AddSlot(profile, "PRIMARY_RAIL", "Primary Rail / Grip", "ATTACHMENT");
		AddSlot(profile, "PRIMARY_LIGHT", "Primary Light / Laser", "ATTACHMENT");
		AddSlot(profile, "PRIMARY_UNDERBARREL", "Primary Underbarrel", "ATTACHMENT");
		AddSlot(profile, "PRIMARY_MAG", "Primary Magazine", "MAGAZINE");
		AddSlot(profile, "PRIMARY_MAG_EXTRA", "Spare Primary Magazines", "MAGAZINE");
		AddSlot(profile, "SECONDARY", "Secondary Weapon", "WEAPON");
		AddSlot(profile, "SECONDARY_OPTIC", "Secondary Optic", "OPTIC");
		AddSlot(profile, "SECONDARY_MUZZLE", "Secondary Muzzle", "ATTACHMENT");
		AddSlot(profile, "SECONDARY_MAG", "Secondary Magazine", "MAGAZINE");
		AddSlot(profile, "LAUNCHER", "Launcher", "WEAPON");
		AddSlot(profile, "LAUNCHER_OPTIC", "Launcher Optic", "OPTIC");
		AddSlot(profile, "LAUNCHER_AMMO", "Launcher Ammunition", "MAGAZINE");
		AddSlot(profile, "HELMET", "Helmet", "HELMET");
		AddSlot(profile, "UNIFORM", "Uniform", "CLOTHING");
		AddSlot(profile, "VEST", "Vest", "VEST");
		AddSlot(profile, "RIG", "Rig / Chest System", "RIG");
		AddSlot(profile, "BELT", "Belt", "BELT");
		AddSlot(profile, "DROPLEG_LEFT", "Left Dropleg", "DROPLEG");
		AddSlot(profile, "DROPLEG_RIGHT", "Right Dropleg", "DROPLEG");
		AddSlot(profile, "BAG", "Backpack / Bag", "BAG");
		AddSlot(profile, "EAR", "Ear / Comms", "COMMS");
		AddSlot(profile, "FACE", "Face / Eye Protection", "ACCESSORY");
		AddSlot(profile, "GLOVES", "Gloves", "CLOTHING");
		AddSlot(profile, "BOOTS", "Boots", "CLOTHING");
		AddSlot(profile, "HOLSTER", "Holster State", "HOLSTER");
		AddSlot(profile, "PATCH", "Patch / Armband", "PATCH");
	}

	protected void AddSlot(RHD_LoadoutProfile profile, string slotId, string displayName, string category)
	{
		if (!profile || profile.m_aSlots.Count() >= RHD_LoadoutEditorConfig.MAX_LOADOUT_SLOTS) return;
		RHD_LoadoutSlot slot = new RHD_LoadoutSlot();
		slot.m_sSlotId = slotId;
		slot.m_sDisplayName = displayName;
		slot.m_sCategory = category;
		slot.m_bEnabled = true;
		profile.m_aSlots.Insert(slot);
	}

	int GetProfileCount() { return m_aProfiles.Count(); }
	RHD_LoadoutProfile GetProfile(int index) { return index >= 0 && index < m_aProfiles.Count() ? m_aProfiles[index] : null; }
	RHD_LoadoutProfile GetActiveProfile() { return m_ActiveProfile; }

	bool SelectProfile(string profileId)
	{
		foreach (RHD_LoadoutProfile profile : m_aProfiles)
			if (profile && profile.m_sProfileId == profileId) { m_ActiveProfile = profile; return true; }
		return false;
	}

	RHD_LoadoutSlot FindSlot(string slotId)
	{
		if (!m_ActiveProfile || slotId.IsEmpty()) return null;
		foreach (RHD_LoadoutSlot slot : m_ActiveProfile.m_aSlots)
			if (slot && slot.m_sSlotId == slotId) return slot;
		return null;
	}

	bool SetItem(string slotId, string itemId)
	{
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot || itemId.IsEmpty()) return false;
		slot.m_sItemId = itemId;
		if (slot.m_iQuantity <= 0) slot.m_iQuantity = 1;
		return true;
	}

	bool SetQuantity(string slotId, int quantity)
	{
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot || quantity <= 0) return false;
		int maximum = RHD_LoadoutEditorConfig.MAX_CARRIED_ITEM_STACK;
		if (slot.m_sCategory == "MAGAZINE") maximum = RHD_LoadoutEditorConfig.MAX_MAGAZINE_STACK;
		slot.m_iQuantity = Math.Min(maximum, quantity);
		return true;
	}

	bool ClearItem(string slotId)
	{
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot) return false;
		slot.m_sItemId = "";
		slot.m_aAttachments.Clear();
		slot.m_iZeroing = 0;
		slot.m_iQuantity = 0;
		return true;
	}

	bool AddAttachment(string slotId, string attachmentId)
	{
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot || attachmentId.IsEmpty() || slot.m_aAttachments.Count() >= RHD_LoadoutEditorConfig.MAX_ATTACHMENTS_PER_SLOT) return false;
		foreach (string existing : slot.m_aAttachments)
			if (existing == attachmentId) return true;
		slot.m_aAttachments.Insert(attachmentId);
		return true;
	}

	bool RemoveAttachment(string slotId, string attachmentId)
	{
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot || attachmentId.IsEmpty()) return false;
		for (int i = 0; i < slot.m_aAttachments.Count(); i++)
			if (slot.m_aAttachments[i] == attachmentId) { slot.m_aAttachments.Remove(i); return true; }
		return false;
	}

	bool AddContainerItem(string containerId, string itemId, string displayName, string category, int quantity, int capacity)
	{
		if (!m_ActiveProfile || containerId.IsEmpty() || itemId.IsEmpty() || quantity <= 0) return false;
		foreach (RHD_LoadoutContainerEntry existing : m_ActiveProfile.m_aContainers)
		{
			if (!existing || existing.m_sContainerId != containerId || existing.m_sItemId != itemId) continue;
			existing.m_iQuantity = Math.Min(existing.m_iCapacity, existing.m_iQuantity + quantity);
			return true;
		}
		if (m_ActiveProfile.m_aContainers.Count() >= RHD_LoadoutEditorConfig.MAX_CONTAINER_ENTRIES) return false;
		RHD_LoadoutContainerEntry entry = new RHD_LoadoutContainerEntry();
		entry.m_sContainerId = containerId;
		entry.m_sItemId = itemId;
		entry.m_sDisplayName = displayName;
		entry.m_sCategory = category;
		entry.m_iCapacity = Math.Max(quantity, capacity);
		entry.m_iQuantity = Math.Min(entry.m_iCapacity, quantity);
		m_ActiveProfile.m_aContainers.Insert(entry);
		return true;
	}

	bool RemoveContainerItem(string containerId, string itemId, int quantity)
	{
		if (!m_ActiveProfile || containerId.IsEmpty() || itemId.IsEmpty() || quantity <= 0) return false;
		foreach (RHD_LoadoutContainerEntry entry : m_ActiveProfile.m_aContainers)
		{
			if (!entry || entry.m_sContainerId != containerId || entry.m_sItemId != itemId || entry.m_iQuantity < quantity) continue;
			entry.m_iQuantity -= quantity;
			if (entry.m_iQuantity <= 0) m_ActiveProfile.m_aContainers.RemoveItem(entry);
			return true;
		}
		return false;
	}

	bool AddLooseItem(string itemId, string displayName, string category, int quantity)
	{
		if (!m_ActiveProfile || itemId.IsEmpty() || quantity <= 0) return false;
		int maximum = RHD_LoadoutEditorConfig.MAX_CARRIED_ITEM_STACK;
		if (category == "MAGAZINE") maximum = RHD_LoadoutEditorConfig.MAX_MAGAZINE_STACK;
		else if (category == "THROWABLE") maximum = RHD_LoadoutEditorConfig.MAX_THROWABLE_STACK;
		else if (category == "EXPLOSIVE") maximum = RHD_LoadoutEditorConfig.MAX_EXPLOSIVE_STACK;
		else if (category == "MEDICAL") maximum = RHD_LoadoutEditorConfig.MAX_MEDICAL_STACK;
		else if (category == "TOOL") maximum = RHD_LoadoutEditorConfig.MAX_TOOL_STACK;
		foreach (RHD_LoadoutContainerEntry existing : m_ActiveProfile.m_aLooseItems)
		{
			if (!existing || existing.m_sItemId != itemId || existing.m_sCategory != category) continue;
			existing.m_iQuantity = Math.Min(maximum, existing.m_iQuantity + quantity);
			return true;
		}
		if (m_ActiveProfile.m_aLooseItems.Count() >= RHD_LoadoutEditorConfig.MAX_CONTAINER_ENTRIES) return false;
		RHD_LoadoutContainerEntry entry = new RHD_LoadoutContainerEntry();
		entry.m_sContainerId = "LOOSE";
		entry.m_sItemId = itemId;
		entry.m_sDisplayName = displayName;
		entry.m_sCategory = category;
		entry.m_iCapacity = maximum;
		entry.m_iQuantity = Math.Min(maximum, quantity);
		m_ActiveProfile.m_aLooseItems.Insert(entry);
		return true;
	}

	bool RemoveLooseItem(string itemId, string category, int quantity)
	{
		if (!m_ActiveProfile || itemId.IsEmpty() || quantity <= 0) return false;
		foreach (RHD_LoadoutContainerEntry entry : m_ActiveProfile.m_aLooseItems)
		{
			if (!entry || entry.m_sItemId != itemId || entry.m_sCategory != category || entry.m_iQuantity < quantity) continue;
			entry.m_iQuantity -= quantity;
			if (entry.m_iQuantity <= 0) m_ActiveProfile.m_aLooseItems.RemoveItem(entry);
			return true;
		}
		return false;
	}

	bool SetZeroing(string slotId, int meters)
	{
		if (!RHD_LoadoutEditorConfig.ENABLE_ZEROING) return false;
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot || meters < RHD_LoadoutEditorConfig.ZEROING_MIN_METERS || meters > RHD_LoadoutEditorConfig.ZEROING_MAX_METERS) return false;
		slot.m_iZeroing = meters;
		return true;
	}

	bool SetHolsterState(string state)
	{
		if (!RHD_LoadoutEditorConfig.ENABLE_HOLSTER) return false;
		return SetItem("HOLSTER", state);
	}

	int CountEquippedItems()
	{
		if (!m_ActiveProfile) return 0;
		int count = 0;
		foreach (RHD_LoadoutSlot slot : m_ActiveProfile.m_aSlots)
			if (slot && !slot.m_sItemId.IsEmpty()) count += 1;
		foreach (RHD_LoadoutContainerEntry entry : m_ActiveProfile.m_aContainers)
			if (entry && entry.m_iQuantity > 0) count += entry.m_iQuantity;
		foreach (RHD_LoadoutContainerEntry loose : m_ActiveProfile.m_aLooseItems)
			if (loose && loose.m_iQuantity > 0) count += loose.m_iQuantity;
		return count;
	}

	bool HasAnyItem() { return CountEquippedItems() > 0; }
};
