// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR MODEL
// ============================================================================
class RHD_LoadoutSlot
{
	string m_sSlotId;
	string m_sDisplayName;
	string m_sCategory;
	string m_sItemId;
	ref array<string> m_aAttachments = {};
	int m_iZeroing;
	bool m_bEnabled;
};

class RHD_LoadoutProfile
{
	string m_sProfileId;
	string m_sDisplayName;
	ref array<RHD_LoadoutSlot> m_aSlots = {};
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
		AddSlot(profile, "PRIMARY_RAIL", "Primary Rail", "ATTACHMENT");
		AddSlot(profile, "PRIMARY_LIGHT", "Primary Light / Laser", "ATTACHMENT");
		AddSlot(profile, "SECONDARY", "Secondary Weapon", "WEAPON");
		AddSlot(profile, "SECONDARY_OPTIC", "Secondary Optic", "OPTIC");
		AddSlot(profile, "LAUNCHER", "Launcher", "WEAPON");
		AddSlot(profile, "LAUNCHER_OPTIC", "Launcher Optic", "OPTIC");
		AddSlot(profile, "HELMET", "Helmet", "CLOTHING");
		AddSlot(profile, "UNIFORM", "Uniform", "CLOTHING");
		AddSlot(profile, "VEST", "Vest", "VEST");
		AddSlot(profile, "RIG", "Rig", "RIG");
		AddSlot(profile, "BELT", "Belt", "BELT");
		AddSlot(profile, "DROPLEG_LEFT", "Left Dropleg", "DROPLEG");
		AddSlot(profile, "DROPLEG_RIGHT", "Right Dropleg", "DROPLEG");
		AddSlot(profile, "BAG", "Backpack / Bag", "BAG");
		AddSlot(profile, "EAR", "Ear / Comms", "ACCESSORY");
		AddSlot(profile, "FACE", "Face / Eye Protection", "ACCESSORY");
		AddSlot(profile, "GLOVES", "Gloves", "CLOTHING");
		AddSlot(profile, "BOOTS", "Boots", "CLOTHING");
		AddSlot(profile, "HOLSTER", "Holster State", "HOLSTER");
		AddSlot(profile, "UTILITY", "Utility", "ACCESSORY");
		AddSlot(profile, "PATCH", "Patch", "ACCESSORY");
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
		return true;
	}

	bool ClearItem(string slotId)
	{
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot) return false;
		slot.m_sItemId = "";
		slot.m_aAttachments.Clear();
		slot.m_iZeroing = 0;
		return true;
	}

	bool AddAttachment(string slotId, string attachmentId)
	{
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot || attachmentId.IsEmpty() || slot.m_aAttachments.Count() >= RHD_LoadoutEditorConfig.MAX_ATTACHMENTS_PER_SLOT) return false;
		foreach (string existing : slot.m_aAttachments) if (existing == attachmentId) return true;
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

	bool SetZeroing(string slotId, int meters)
	{
		if (!RHD_LoadoutEditorConfig.ENABLE_ZEROING) return false;
		RHD_LoadoutSlot slot = FindSlot(slotId);
		if (!slot || meters < 0) return false;
		slot.m_iZeroing = meters;
		return true;
	}

	bool SetHolsterState(string state)
	{
		if (!RHD_LoadoutEditorConfig.ENABLE_HOLSTER) return false;
		return SetItem("HOLSTER", state);
	}
};
