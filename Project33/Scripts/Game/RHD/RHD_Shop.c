// RHD SHARED SHOP
// All F6/F8 shop prices and cart math use this service.
class RHD_ShopCartEntry
{
	string m_sItemId;
	int m_iQuantity;
};

class RHD_Shop
{
	static bool GetSellPrice(string itemId, out int price)
	{
		price = 0;
		if (itemId == "APPLE") price = RHD_ShopConfig.APPLE_SELL;
		else if (itemId == "CANNABIS_PLANT") price = RHD_ShopConfig.CANNABIS_PLANT_SELL;
		else if (itemId == "COCA_LEAF") price = RHD_ShopConfig.COCA_LEAF_SELL;
		else if (itemId == "CORN_COB") price = RHD_ShopConfig.CORN_COB_SELL;
		else if (itemId == "GRAPES") price = RHD_ShopConfig.GRAPES_SELL;
		else if (itemId == "PEACHES") price = RHD_ShopConfig.PEACHES_SELL;
		else if (itemId == "IRON_ORE") price = RHD_ShopConfig.IRON_ORE_SELL;
		else if (itemId == "COPPER_ORE") price = RHD_ShopConfig.COPPER_ORE_SELL;
		else if (itemId == "GOLD_ORE") price = RHD_ShopConfig.GOLD_ORE_SELL;
		else if (itemId == "DIAMOND") price = RHD_ShopConfig.DIAMOND_SELL;
		else if (itemId == "OIL_SAND") price = RHD_ShopConfig.OIL_SAND_SELL;
		else if (itemId == "IRON") price = RHD_ShopConfig.IRON_SELL;
		else if (itemId == "COPPER") price = RHD_ShopConfig.COPPER_SELL;
		else if (itemId == "GOLD") price = RHD_ShopConfig.GOLD_SELL;
		else if (itemId == "OIL") price = RHD_ShopConfig.OIL_SELL;
		else if (itemId == "CANNABIS_FLOWER") price = RHD_ShopConfig.CANNABIS_FLOWER_SELL;
		else if (itemId == "COCAINE") price = RHD_ShopConfig.COCAINE_SELL;
		else if (itemId == "CANNED_CORN") price = RHD_ShopConfig.CANNED_CORN_SELL;
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
