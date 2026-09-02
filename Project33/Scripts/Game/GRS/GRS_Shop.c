// Shared virtual shop service.
// F6, F7 and F8 should call this service instead of maintaining separate prices.
class GRS_ShopItem
{
	string m_sItemId;
	string m_sDisplayName;
	int m_iSellPrice;
};

class GRS_Shop
{
	static bool GetSellPrice(string itemId, out int price)
	{
		price = 0;
		if (itemId == "APPLE") price = GRS_ShopConfig.APPLE_SELL;
		else if (itemId == "CANNABIS_PLANT") price = GRS_ShopConfig.CANNABIS_PLANT_SELL;
		else if (itemId == "COCA_LEAF") price = GRS_ShopConfig.COCA_LEAF_SELL;
		else if (itemId == "CORN_COB") price = GRS_ShopConfig.CORN_COB_SELL;
		else if (itemId == "GRAPES") price = GRS_ShopConfig.GRAPES_SELL;
		else if (itemId == "PEACHES") price = GRS_ShopConfig.PEACHES_SELL;
		else if (itemId == "IRON_ORE") price = GRS_ShopConfig.IRON_ORE_SELL;
		else if (itemId == "COPPER_ORE") price = GRS_ShopConfig.COPPER_ORE_SELL;
		else if (itemId == "GOLD_ORE") price = GRS_ShopConfig.GOLD_ORE_SELL;
		else if (itemId == "DIAMOND") price = GRS_ShopConfig.DIAMOND_SELL;
		else if (itemId == "OIL_SAND") price = GRS_ShopConfig.OIL_SAND_SELL;
		else if (itemId == "IRON") price = GRS_ShopConfig.IRON_SELL;
		else if (itemId == "COPPER") price = GRS_ShopConfig.COPPER_SELL;
		else if (itemId == "GOLD") price = GRS_ShopConfig.GOLD_SELL;
		else if (itemId == "OIL") price = GRS_ShopConfig.OIL_SELL;
		else if (itemId == "CANNABIS_FLOWER") price = GRS_ShopConfig.CANNABIS_FLOWER_SELL;
		else if (itemId == "COCAINE") price = GRS_ShopConfig.COCAINE_SELL;
		else if (itemId == "CANNED_CORN") price = GRS_ShopConfig.CANNED_CORN_SELL;
		return price > 0;
	}

	static int CalculateSale(string itemId, int quantity)
	{
		int price;
		if (quantity <= 0 || !GetSellPrice(itemId, price))
			return 0;
		return price * quantity;
	}
};
