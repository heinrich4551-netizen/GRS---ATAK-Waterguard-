// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR CATEGORY CATALOG
// ============================================================================
// The editor exposes categories rather than inventing a second item database.
// EnumerateItems() in the mission adapter supplies the actual resources from
// the installed WCS/GRS/RHS ecosystem.
// ============================================================================
class RHD_LoadoutEditorCategory
{
	string m_sId;
	string m_sDisplayName;
	string m_sDescription;
	string m_sDependency;
	string m_sPrimarySlot;

	void RHD_LoadoutEditorCategory(string id, string displayName, string description, string dependency, string primarySlot)
	{
		m_sId = id;
		m_sDisplayName = displayName;
		m_sDescription = description;
		m_sDependency = dependency;
		m_sPrimarySlot = primarySlot;
	}
};

class RHD_LoadoutEditorCatalog
{
	static void Build(out array<RHD_LoadoutEditorCategory> categories)
	{
		categories = {};
		categories.Insert(new RHD_LoadoutEditorCategory("WEAPON", "Weapons", "Primary, secondary, launchers and weapon platforms.", "WCS_Weapons / RHS", "PRIMARY"));
		categories.Insert(new RHD_LoadoutEditorCategory("OPTIC", "Optics & Sights", "Rifle optics, red dots and magnified sights.", "WCS_Scopes / Rayzis Optics", "PRIMARY_OPTIC"));
		categories.Insert(new RHD_LoadoutEditorCategory("ATTACHMENT", "Weapon Attachments", "Muzzle, rail, grip, light and laser attachments.", "WCS_Attachments / RIS", "PRIMARY_MUZZLE"));
		categories.Insert(new RHD_LoadoutEditorCategory("MAGAZINE", "Magazines & Ammunition", "Weapon magazines and ammunition carried for active weapons.", "WCS_Weapons / RHS", "PRIMARY_MAG"));
		categories.Insert(new RHD_LoadoutEditorCategory("THROWABLE", "Throwables", "Grenades and other throwable equipment.", "WCS / RHS", "THROWABLES"));
		categories.Insert(new RHD_LoadoutEditorCategory("EXPLOSIVE", "Explosives", "Explosives, charges and demolition equipment.", "WCS / RHS", "EXPLOSIVES"));
		categories.Insert(new RHD_LoadoutEditorCategory("MEDICAL", "Medical", "Medical and treatment supplies carried by the player.", "Overthrow / mission catalog", "MEDICAL"));
		categories.Insert(new RHD_LoadoutEditorCategory("TOOL", "Tools", "Mission tools and field equipment.", "Mission catalog", "TOOLS"));
		categories.Insert(new RHD_LoadoutEditorCategory("HELMET", "Headgear", "Helmets and head protection.", "WCS_Clothing / GRS Apparel / RHS", "HELMET"));
		categories.Insert(new RHD_LoadoutEditorCategory("CLOTHING", "Clothing", "Uniforms, gloves and boots.", "WCS_Clothing / GRS Apparel / RHS", "UNIFORM"));
		categories.Insert(new RHD_LoadoutEditorCategory("VEST", "Vests", "Protective vests and carriers.", "GRS Modular Vests & Rigs / WCS / RHS", "VEST"));
		categories.Insert(new RHD_LoadoutEditorCategory("RIG", "Rigs & Chest Systems", "Modular rigs and configurable chest systems.", "GRS Modular Vests & Rigs", "RIG"));
		categories.Insert(new RHD_LoadoutEditorCategory("BELT", "Belts", "Belts and belt-mounted equipment.", "GRS Belts & Bags & Droplegs", "BELT"));
		categories.Insert(new RHD_LoadoutEditorCategory("DROPLEG", "Droplegs", "Left/right drop-leg equipment.", "GRS Belts & Bags & Droplegs", "DROPLEG_LEFT"));
		categories.Insert(new RHD_LoadoutEditorCategory("BAG", "Backpacks & Bags", "Backpacks and modular bags.", "AAO GRS Bag / GRS Belts & Bags", "BAG"));
		categories.Insert(new RHD_LoadoutEditorCategory("COMMS", "Comms & Headsets", "Radio/headset and communications equipment.", "WCS / GRS / mission catalog", "EAR"));
		categories.Insert(new RHD_LoadoutEditorCategory("ACCESSORY", "Accessories", "Face, eye, utility and small equipment.", "WCS / GRS / RHS", "ACCESSORY"));
		categories.Insert(new RHD_LoadoutEditorCategory("PATCH", "Patches & Armbands", "Patches, identifiers and armband equipment.", "GRS Patches / mission catalog", "PATCH"));
		categories.Insert(new RHD_LoadoutEditorCategory("HOLSTER", "Holster", "Holster state and supported holster equipment.", "Weapon Holstering", "HOLSTER"));
	}

	static string GetCategoryDisplayName(string category)
	{
		array<RHD_LoadoutEditorCategory> categories;
		Build(categories);
		foreach (RHD_LoadoutEditorCategory entry : categories)
			if (entry && entry.m_sId == category) return entry.m_sDisplayName;
		return category;
	}
};
