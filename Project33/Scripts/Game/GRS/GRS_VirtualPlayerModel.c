class GRS_VirtualInventoryEntry
{
	string m_sItemId;
	string m_sDisplayName;
	int m_iQuantity;
	bool m_bVirtual;
};

class GRS_GarageVehicleEntry
{
	string m_sVehicleId;
	string m_sDisplayName;
	bool m_bStored;
};

class GRS_PropertyStorageEntry
{
	string m_sStorageId;
	int m_iCapacity;
};

class GRS_PropertyEntry
{
	string m_sPropertyId;
	string m_sDisplayName;
	vector m_vPosition;
	float m_fBuildRadiusMeters;
	int m_iPurchasePrice;
	int m_iStorageCapacity;
	ref array<GRS_PropertyStorageEntry> m_aStorage = {};
	ref array<string> m_aStructures = {};
	bool m_bOwned;
};

class GRS_JobEntry
{
	string m_sJobId;
	string m_sTitle;
	string m_sDescription;
	string m_sSourceCivilian;
	string m_sDestinationCivilian;
	string m_sVirtualItemId;
	int m_iReward;
	bool m_bActive;
};

class GRS_VirtualPlayerState
{
	ref array<GRS_VirtualInventoryEntry> m_aBaseInventory = {};
	ref array<GRS_VirtualInventoryEntry> m_aVirtualInventory = {};
	ref array<GRS_GarageVehicleEntry> m_aGarage = {};
	ref array<GRS_PropertyEntry> m_aProperties = {};
	ref array<GRS_JobEntry> m_aJobs = {};

	bool AddBaseItem(string itemId, string displayName, int quantity)
	{
		if (quantity <= 0 || m_aBaseInventory.Count() >= GRS_VirtualPlayerConfig.MAX_BASE_INVENTORY_ITEMS)
			return false;
		GRS_VirtualInventoryEntry entry = new GRS_VirtualInventoryEntry();
		entry.m_sItemId = itemId;
		entry.m_sDisplayName = displayName;
		entry.m_iQuantity = quantity;
		entry.m_bVirtual = false;
		m_aBaseInventory.Insert(entry);
		return true;
	}

	bool AddVirtualItem(string itemId, string displayName, int quantity)
	{
		if (quantity <= 0 || m_aVirtualInventory.Count() >= GRS_VirtualPlayerConfig.MAX_VIRTUAL_ITEMS)
			return false;
		GRS_VirtualInventoryEntry entry = new GRS_VirtualInventoryEntry();
		entry.m_sItemId = itemId;
		entry.m_sDisplayName = displayName;
		entry.m_iQuantity = quantity;
		entry.m_bVirtual = true;
		m_aVirtualInventory.Insert(entry);
		return true;
	}

	bool AddGarageVehicle(string vehicleId, string displayName)
	{
		if (m_aGarage.Count() >= GRS_VirtualPlayerConfig.MAX_GARAGE_VEHICLES)
			return false;
		GRS_GarageVehicleEntry entry = new GRS_GarageVehicleEntry();
		entry.m_sVehicleId = vehicleId;
		entry.m_sDisplayName = displayName;
		entry.m_bStored = true;
		m_aGarage.Insert(entry);
		return true;
	}

	bool AddProperty(string propertyId, string displayName, vector position, float radiusMeters, int price)
	{
		if (m_aProperties.Count() >= GRS_VirtualPlayerConfig.MAX_PROPERTIES)
			return false;
		if (radiusMeters <= 0 || radiusMeters > GRS_VirtualPlayerConfig.MAX_BUILD_RADIUS_M)
			return false;
		if (price < GRS_VirtualPlayerConfig.MIN_PROPERTY_PRICE || price > GRS_VirtualPlayerConfig.MAX_PROPERTY_PRICE)
			return false;

		GRS_PropertyEntry property = new GRS_PropertyEntry();
		property.m_sPropertyId = propertyId;
		property.m_sDisplayName = displayName;
		property.m_vPosition = position;
		property.m_fBuildRadiusMeters = radiusMeters;
		property.m_iPurchasePrice = price;
		property.m_iStorageCapacity = 0;
		property.m_bOwned = true;
		m_aProperties.Insert(property);
		return true;
	}

	bool AddStorageObject(int propertyIndex, string storageId)
	{
		if (propertyIndex < 0 || propertyIndex >= m_aProperties.Count())
			return false;
		GRS_PropertyEntry property = m_aProperties[propertyIndex];
		if (property.m_iStorageCapacity + GRS_VirtualPlayerConfig.STORAGE_PER_OBJECT > GRS_VirtualPlayerConfig.MAX_PROPERTY_STORAGE)
			return false;

		GRS_PropertyStorageEntry storage = new GRS_PropertyStorageEntry();
		storage.m_sStorageId = storageId;
		storage.m_iCapacity = GRS_VirtualPlayerConfig.STORAGE_PER_OBJECT;
		property.m_aStorage.Insert(storage);
		property.m_iStorageCapacity += storage.m_iCapacity;
		return true;
	}
};
