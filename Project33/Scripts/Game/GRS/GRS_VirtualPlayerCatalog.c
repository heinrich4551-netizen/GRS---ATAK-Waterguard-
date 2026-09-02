// ============================================================================
// GRS VIRTUAL PLAYER CATALOG
// ============================================================================
// BEGINNER CONFIGURATION:
// This file is where you define the things players can buy/use.
//
// IMPORTANT:
// - Vehicle IDs must be the prefab/resource identifiers used by your mission.
// - Civilian jobs DO NOT use IDs typed here. Jobs are generated from civilians
//   that actually exist in the running mission.
// - Virtual items are menu-only items. They are never spawned into the game.
//
// Keep this file simple: add entries by copying an existing entry and changing
// the text/value fields.
// ============================================================================

class GRS_VirtualItemDefinition
{
	string m_sId;
	string m_sName;
	int m_iPrice;
};

class GRS_VehicleDefinition
{
	string m_sId;
	string m_sName;
	int m_iPrice;
};

class GRS_PropertyObjectDefinition
{
	string m_sId;
	string m_sName;
	string m_sType;
	int m_iPrice;
};

class GRS_VirtualPlayerCatalog
{
	// ------------------------------------------------------------------------
	// VIRTUAL ITEMS
	// Add up to 435 menu-only item definitions.
	// ------------------------------------------------------------------------
	static void GetVirtualItems(out array<GRS_VirtualItemDefinition> items)
	{
		items = {};

		GRS_VirtualItemDefinition item = new GRS_VirtualItemDefinition();
		item.m_sId = "GRS_VIRTUAL_PACKAGE";
		item.m_sName = "Virtual Package";
		item.m_iPrice = 250;
		items.Insert(item);

		// COPY THE BLOCK ABOVE to add more virtual items.
	}

	// ------------------------------------------------------------------------
	// VEHICLES
	// These are configuration entries only. The mission adapter checks that
	// the vehicle really exists/is available before allowing a purchase.
	// ------------------------------------------------------------------------
	static void GetVehicles(out array<GRS_VehicleDefinition> vehicles)
	{
		vehicles = {};

		// EXAMPLE:
		// GRS_VehicleDefinition vehicle = new GRS_VehicleDefinition();
		// vehicle.m_sId = "YOUR_MISSION_VEHICLE_PREFAB_ID";
		// vehicle.m_sName = "My Mission Vehicle";
		// vehicle.m_iPrice = 25000;
		// vehicles.Insert(vehicle);
	}

	// ------------------------------------------------------------------------
	// PROPERTY BUILD OBJECTS
	// ------------------------------------------------------------------------
	static void GetPropertyObjects(out array<GRS_PropertyObjectDefinition> objects)
	{
		objects = {};

		GRS_PropertyObjectDefinition storage = new GRS_PropertyObjectDefinition();
		storage.m_sId = "STORAGE_DEFAULT";
		storage.m_sName = "Virtual Storage";
		storage.m_sType = GRS_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE;
		storage.m_iPrice = 1000;
		objects.Insert(storage);

		// Add your real arsenal/decor/fortification prefab IDs here when desired.
	}
};
