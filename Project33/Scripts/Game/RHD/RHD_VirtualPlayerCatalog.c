class RHD_VirtualItemDefinition
{
	string m_sId;
	string m_sName;
	string m_sCategory;
	string m_sIcon;
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
	static void AddItem(out array<RHD_VirtualItemDefinition> items, string id, string name, string category, string icon, int price)
	{
		RHD_VirtualItemDefinition item = new RHD_VirtualItemDefinition();
		item.m_sId = id; item.m_sName = name; item.m_sCategory = category; item.m_sIcon = icon; item.m_iPrice = price;
		items.Insert(item);
	}
	static void GetVirtualItems(out array<RHD_VirtualItemDefinition> items)
	{
		items = {};
		AddItem(items, "APPLE", "Apple", "Farming", "APPLE", 25);
		AddItem(items, "CANNABIS_PLANT", "Cannabis Plant", "Farming", "CANNABIS_PLANT", 60);
		AddItem(items, "COCA_LEAF", "Coca Leaf", "Farming", "COCA_LEAF", 75);
		AddItem(items, "CORN_COB", "Corn Cob", "Farming", "CORN_COB", 20);
		AddItem(items, "GRAPES", "Grapes", "Farming", "GRAPES", 30);
		AddItem(items, "PEACHES", "Peaches", "Farming", "PEACHES", 30);
		AddItem(items, "IRON_ORE", "Iron Ore", "Mining", "IRON_ORE", 40);
		AddItem(items, "COPPER_ORE", "Copper Ore", "Mining", "COPPER_ORE", 55);
		AddItem(items, "GOLD_ORE", "Gold Ore", "Mining", "GOLD_ORE", 250);
		AddItem(items, "DIAMOND", "Diamond", "Mining", "DIAMOND", 1000);
		AddItem(items, "OIL_SAND", "Oil Sand", "Mining", "OIL_SAND", 80);
		AddItem(items, "IRON", "Iron", "Refining", "IRON", 100);
		AddItem(items, "COPPER", "Copper", "Refining", "COPPER", 125);
		AddItem(items, "GOLD", "Gold", "Refining", "GOLD", 500);
		AddItem(items, "OIL", "Oil", "Refining", "OIL", 175);
		AddItem(items, "CANNABIS_FLOWER", "Cannabis Flower", "Processing", "CANNABIS_FLOWER", 15);
		AddItem(items, "COCAINE", "Cocaine", "Processing", "COCAINE", 150);
		AddItem(items, "CANNED_CORN", "Canned Corn", "Processing", "CANNED_CORN", 75);
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
		storage.m_sId = "STORAGE_DEFAULT"; storage.m_sName = "Virtual Storage"; storage.m_sType = RHD_VirtualPlayerConfig.PROPERTY_OBJECT_STORAGE; storage.m_iPrice = 1000;
		objects.Insert(storage);
	}
};
