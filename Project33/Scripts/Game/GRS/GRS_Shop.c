// Shared virtual shop service.
// F6, F7 and F8 should call this service instead of maintaining separate prices.
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
