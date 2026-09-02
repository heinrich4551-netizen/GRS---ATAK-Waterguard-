class RHD_VirtualProduction
{
	static bool IsKnownVirtualItem(string itemId)
	{
		return itemId == "APPLE" || itemId == "CANNABIS_PLANT" || itemId == "COCA_LEAF" || itemId == "CORN_COB" || itemId == "GRAPES" || itemId == "PEACHES"
			|| itemId == "IRON_ORE" || itemId == "COPPER_ORE" || itemId == "GOLD_ORE" || itemId == "DIAMOND" || itemId == "OIL_SAND"
			|| itemId == "IRON" || itemId == "COPPER" || itemId == "GOLD" || itemId == "OIL" || itemId == "CANNABIS_FLOWER" || itemId == "COCAINE" || itemId == "CANNED_CORN";
	}
	static string GetDisplayName(string itemId)
	{
		if (itemId == "APPLE") return "Apple";
		if (itemId == "CANNABIS_PLANT") return "Cannabis Plant";
		if (itemId == "COCA_LEAF") return "Coca Leaf";
		if (itemId == "CORN_COB") return "Corn Cob";
		if (itemId == "GRAPES") return "Grapes";
		if (itemId == "PEACHES") return "Peaches";
		if (itemId == "IRON_ORE") return "Iron Ore";
		if (itemId == "COPPER_ORE") return "Copper Ore";
		if (itemId == "GOLD_ORE") return "Gold Ore";
		if (itemId == "DIAMOND") return "Diamond";
		if (itemId == "OIL_SAND") return "Oil Sand";
		if (itemId == "IRON") return "Iron";
		if (itemId == "COPPER") return "Copper";
		if (itemId == "GOLD") return "Gold";
		if (itemId == "OIL") return "Oil";
		if (itemId == "CANNABIS_FLOWER") return "Cannabis Flower";
		if (itemId == "COCAINE") return "Cocaine";
		if (itemId == "CANNED_CORN") return "Canned Corn";
		return itemId;
	}
	static bool Refine(RHD_VirtualPlayerState state, string inputId)
	{
		if (!state || inputId.IsEmpty()) return false;
		string outputId = "";
		if (inputId == "IRON_ORE") outputId = "IRON";
		else if (inputId == "COPPER_ORE") outputId = "COPPER";
		else if (inputId == "GOLD_ORE") outputId = "GOLD";
		else if (inputId == "OIL_SAND") outputId = "OIL";
		else return false;
		if (!state.RemoveVirtualItem(inputId, 1)) return false;
		if (state.AddVirtualItem(outputId, GetDisplayName(outputId), 1)) return true;
		state.AddVirtualItem(inputId, GetDisplayName(inputId), 1);
		return false;
	}
	static bool Process(RHD_VirtualPlayerState state, string inputId)
	{
		if (!state || inputId.IsEmpty()) return false;
		int required = 0;
		string outputId = "";
		int outputQuantity = 1;
		if (inputId == "CANNABIS_PLANT") { required = 1; outputId = "CANNABIS_FLOWER"; outputQuantity = Math.RandomInt(RHD_VirtualProductionConfig.CANNABIS_FLOWER_MIN, RHD_VirtualProductionConfig.CANNABIS_FLOWER_MAX + 1); }
		else if (inputId == "COCA_LEAF") { required = RHD_VirtualProductionConfig.COCA_LEAF_FOR_COCAINE; outputId = "COCAINE"; }
		else if (inputId == "CORN_COB") { required = RHD_VirtualProductionConfig.CORN_COB_FOR_CANNED_CORN; outputId = "CANNED_CORN"; }
		else return false;
		if (state.GetVirtualItemQuantity(inputId) < required || !state.RemoveVirtualItem(inputId, required)) return false;
		if (state.AddVirtualItem(outputId, GetDisplayName(outputId), outputQuantity)) return true;
		state.AddVirtualItem(inputId, GetDisplayName(inputId), required);
		return false;
	}
};
