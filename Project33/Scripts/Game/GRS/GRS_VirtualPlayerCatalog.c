// ============================================================================
// RHD VIRTUAL PLAYER CATALOG
// ============================================================================
// Beginner configuration: define menu-only items, mission vehicles and
// property objects here. Jobs use civilians already spawned by the mission.
// ============================================================================

class RHD_VirtualItemDefinition
{
	string m_sId;
	string m_sName;
	int m_iPrice;
};
class RHD_VehicleDefinition
{
	string m_sId;
	string m_sName;
	int m_iPrice;
};
class RHD_PropertyObjectDefinition
{
	string m_sId;
	string m_sName;
	string m_sType;
	int m_iPrice;
};
class RHD_VirtualPlayerCatalog
{
	static void GetVirtualItems(out array<RHD_VirtualItemDefinition> items)
	{
		items = {};
		RHD_VirtualItemDefinition item = new RHD_VirtualItemDefinition();
		item.m_sId = "RHD_VIRTUAL_PACKAGE";
		item.m_sName = "Virtual Package";
		item.m_iPrice = 250;
		items.Insert(item);
	}
	static void GetVehicles(out array<RHD_VehicleDefinition> vehicles)
	{
		vehicles = {};
		// Add only vehicle prefab/resource IDs that are available in the mission.
	}
	static void GetPropertyObjects(out array<RHD_PropertyObjectDefinition> objects)
	{
		objects = {};
		RHD_PropertyObjectDefinition storage = new RHD_PropertyObjectDefinition();
		storage.m_sId = "STORAGE_DEFAULT";
		storage.m_sName = "Virtual Storage";
		storage.m_sType = RHD_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE;
		storage.m_iPrice = 1000;
		objects.Insert(storage);
	}
};
