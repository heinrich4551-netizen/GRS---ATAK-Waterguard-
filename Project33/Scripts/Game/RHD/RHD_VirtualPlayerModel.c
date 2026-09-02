class RHD_VirtualInventoryEntry
{
	string m_sItemId;
	string m_sDisplayName;
	int m_iQuantity;
	bool m_bVirtual;
};
class RHD_GarageVehicleEntry
{
	string m_sVehicleId;
	string m_sDisplayName;
	bool m_bStored;
};
class RHD_PropertyStorageEntry
{
	string m_sStorageId;
	int m_iCapacity;
};
class RHD_PropertyEntry
{
	string m_sPropertyId;
	string m_sDisplayName;
	vector m_vPosition;
	float m_fBuildRadiusMeters;
	int m_iPurchasePrice;
	int m_iStorageCapacity;
	ref array<RHD_PropertyStorageEntry> m_aStorage = {};
	ref array<string> m_aStructures = {};
	bool m_bOwned;
};
class RHD_JobEntry
{
	string m_sJobId;
	string m_sTitle;
	string m_sDescription;
	string m_sSourceCivilian;
	string m_sDestinationCivilian;
	string m_sVirtualItemId;
	int m_iReward;
	int m_iStage;
	bool m_bActive;
};
class RHD_VirtualPlayerState
{
	ref array<RHD_VirtualInventoryEntry> m_aBaseInventory = {};
	ref array<RHD_VirtualInventoryEntry> m_aVirtualInventory = {};
	ref array<RHD_GarageVehicleEntry> m_aGarage = {};
	ref array<RHD_PropertyEntry> m_aProperties = {};
	ref array<RHD_JobEntry> m_aJobs = {};
	int GetBaseInventorySlotsUsed() { return m_aBaseInventory.Count(); }
	bool AddBaseItem(string itemId, string displayName, int quantity)
	{
		if (itemId.IsEmpty() || quantity <= 0) return false;
		foreach (RHD_VirtualInventoryEntry existing : m_aBaseInventory)
			if (existing && existing.m_sItemId == itemId && !existing.m_bVirtual) { existing.m_iQuantity += quantity; return true; }
		if (GetBaseInventorySlotsUsed() >= RHD_VirtualPlayerConfig.MAX_BASE_INVENTORY_SLOTS) return false;
		RHD_VirtualInventoryEntry entry = new RHD_VirtualInventoryEntry();
		entry.m_sItemId = itemId; entry.m_sDisplayName = displayName; entry.m_iQuantity = quantity; entry.m_bVirtual = false;
		m_aBaseInventory.Insert(entry); return true;
	}
	bool AddVirtualItem(string itemId, string displayName, int quantity)
	{
		if (itemId.IsEmpty() || quantity <= 0) return false;
		foreach (RHD_VirtualInventoryEntry existing : m_aVirtualInventory)
			if (existing && existing.m_sItemId == itemId && existing.m_bVirtual) { existing.m_iQuantity += quantity; return true; }
		if (m_aVirtualInventory.Count() >= RHD_VirtualPlayerConfig.MAX_VIRTUAL_ITEM_TYPES) return false;
		RHD_VirtualInventoryEntry entry = new RHD_VirtualInventoryEntry();
		entry.m_sItemId = itemId; entry.m_sDisplayName = displayName; entry.m_iQuantity = quantity; entry.m_bVirtual = true;
		m_aVirtualInventory.Insert(entry); return true;
	}
	bool RemoveVirtualItem(string itemId, int quantity)
	{
		if (itemId.IsEmpty() || quantity <= 0) return false;
		foreach (RHD_VirtualInventoryEntry entry : m_aVirtualInventory)
		{
			if (!entry || entry.m_sItemId != itemId || !entry.m_bVirtual || entry.m_iQuantity < quantity) continue;
			entry.m_iQuantity -= quantity;
			if (entry.m_iQuantity <= 0) m_aVirtualInventory.RemoveItem(entry);
			return true;
		}
		return false;
	}
	int GetVirtualItemQuantity(string itemId)
	{
		foreach (RHD_VirtualInventoryEntry entry : m_aVirtualInventory)
			if (entry && entry.m_sItemId == itemId && entry.m_bVirtual) return entry.m_iQuantity;
		return 0;
	}
	bool AddGarageVehicle(string vehicleId, string displayName)
	{
		if (vehicleId.IsEmpty()) return false;
		RHD_GarageVehicleEntry entry = new RHD_GarageVehicleEntry();
		entry.m_sVehicleId = vehicleId; entry.m_sDisplayName = displayName; entry.m_bStored = true;
		m_aGarage.Insert(entry); return true;
	}
	bool AddProperty(string propertyId, string displayName, vector position, float radiusMeters, int price)
	{
		if (m_aProperties.Count() >= RHD_VirtualPlayerConfig.MAX_PROPERTIES) return false;
		if (propertyId.IsEmpty() || radiusMeters <= 0 || radiusMeters > RHD_VirtualPlayerConfig.MAX_BUILD_RADIUS_M) return false;
		if (price < RHD_VirtualPlayerConfig.MIN_PROPERTY_PRICE || price > RHD_VirtualPlayerConfig.MAX_PROPERTY_PRICE) return false;
		RHD_PropertyEntry property = new RHD_PropertyEntry();
		property.m_sPropertyId = propertyId; property.m_sDisplayName = displayName; property.m_vPosition = position;
		property.m_fBuildRadiusMeters = radiusMeters; property.m_iPurchasePrice = price; property.m_iStorageCapacity = 0; property.m_bOwned = true;
		m_aProperties.Insert(property); return true;
	}
	bool AddStorageObject(int propertyIndex, string storageId)
	{
		if (propertyIndex < 0 || propertyIndex >= m_aProperties.Count() || storageId.IsEmpty()) return false;
		RHD_PropertyEntry property = m_aProperties[propertyIndex];
		if (!property || property.m_iStorageCapacity + RHD_VirtualPlayerConfig.STORAGE_PER_OBJECT > RHD_VirtualPlayerConfig.MAX_PROPERTY_STORAGE) return false;
		RHD_PropertyStorageEntry storage = new RHD_PropertyStorageEntry();
		storage.m_sStorageId = storageId; storage.m_iCapacity = RHD_VirtualPlayerConfig.STORAGE_PER_OBJECT;
		property.m_aStorage.Insert(storage); property.m_iStorageCapacity += storage.m_iCapacity; return true;
	}
};
enum RHD_JobStage
{
	TALK_TO_SOURCE,
	TALK_TO_CONTACT,
	DELIVER_ITEM,
	COMPLETE
};
