// RHD SHARED SHOP
// One price table is shared by F6, F7 and F8.
class RHD_ShopCartEntry
{
	string m_sItemId;
	int m_iQuantity;
};

class RHD_Shop
{
	protected static ref map<string, int> s_mPrices = new map<string, int>();
	protected static bool s_bInitialized;
	static void Initialize()
	{
		if (s_bInitialized) return;
		s_bInitialized = true;
		AddDefault("APPLE", RHD_ShopConfig.APPLE_SELL);
		AddDefault("CANNABIS_PLANT", RHD_ShopConfig.CANNABIS_PLANT_SELL);
		AddDefault("COCA_LEAF", RHD_ShopConfig.COCA_LEAF_SELL);
		AddDefault("CORN_COB", RHD_ShopConfig.CORN_COB_SELL);
		AddDefault("GRAPES", RHD_ShopConfig.GRAPES_SELL);
		AddDefault("PEACHES", RHD_ShopConfig.PEACHES_SELL);
		AddDefault("IRON_ORE", RHD_ShopConfig.IRON_ORE_SELL);
		AddDefault("COPPER_ORE", RHD_ShopConfig.COPPER_ORE_SELL);
		AddDefault("GOLD_ORE", RHD_ShopConfig.GOLD_ORE_SELL);
		AddDefault("DIAMOND", RHD_ShopConfig.DIAMOND_SELL);
		AddDefault("OIL_SAND", RHD_ShopConfig.OIL_SAND_SELL);
		AddDefault("IRON", RHD_ShopConfig.IRON_SELL);
		AddDefault("COPPER", RHD_ShopConfig.COPPER_SELL);
		AddDefault("GOLD", RHD_ShopConfig.GOLD_SELL);
		AddDefault("OIL", RHD_ShopConfig.OIL_SELL);
		AddDefault("CANNABIS_FLOWER", RHD_ShopConfig.CANNABIS_FLOWER_SELL);
		AddDefault("COCAINE", RHD_ShopConfig.COCAINE_SELL);
		AddDefault("CANNED_CORN", RHD_ShopConfig.CANNED_CORN_SELL);
	}
	protected static void AddDefault(string itemId, int price) { s_mPrices.Set(itemId, price); }
	static bool SetSellPrice(string itemId, int price)
	{
		Initialize();
		if (itemId.IsEmpty() || price < 0 || !s_mPrices.Contains(itemId)) return false;
		s_mPrices.Set(itemId, price);
		return true;
	}
	static bool GetSellPrice(string itemId, out int price)
	{
		Initialize();
		price = 0;
		if (!s_mPrices.Contains(itemId)) return false;
		price = s_mPrices.Get(itemId);
		return price > 0;
	}
	static int CalculateSale(string itemId, int quantity)
	{
		int price;
		if (quantity <= 0 || !GetSellPrice(itemId, price)) return 0;
		return price * quantity;
	}
};

class RHD_ShopCart
{
	protected ref array<RHD_ShopCartEntry> m_aEntries = {};
	bool Add(string itemId, int quantity)
	{
		int unitPrice;
		if (itemId.IsEmpty() || quantity <= 0 || !RHD_Shop.GetSellPrice(itemId, unitPrice)) return false;
		foreach (RHD_ShopCartEntry entry : m_aEntries)
			if (entry && entry.m_sItemId == itemId) { entry.m_iQuantity += quantity; return true; }
		RHD_ShopCartEntry entry = new RHD_ShopCartEntry();
		entry.m_sItemId = itemId; entry.m_iQuantity = quantity;
		m_aEntries.Insert(entry);
		return true;
	}
	bool Remove(string itemId, int quantity)
	{
		if (itemId.IsEmpty() || quantity <= 0) return false;
		foreach (RHD_ShopCartEntry entry : m_aEntries)
		{
			if (!entry || entry.m_sItemId != itemId || entry.m_iQuantity < quantity) continue;
			entry.m_iQuantity -= quantity;
			if (entry.m_iQuantity <= 0) m_aEntries.RemoveItem(entry);
			return true;
		}
		return false;
	}
	void Clear() { m_aEntries.Clear(); }
	int GetCount() { return m_aEntries.Count(); }
	RHD_ShopCartEntry Get(int index) { if (index < 0 || index >= m_aEntries.Count()) return null; return m_aEntries[index]; }
	int GetTotal()
	{
		int total = 0;
		foreach (RHD_ShopCartEntry entry : m_aEntries)
			if (entry) total += RHD_Shop.CalculateSale(entry.m_sItemId, entry.m_iQuantity);
		return total;
	}
};
