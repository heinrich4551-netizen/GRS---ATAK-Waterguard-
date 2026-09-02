class RHD_ShopVisualTile
{
	string m_sItemId;
	int m_iColumn;
	int m_iRow;
};

class RHD_ShopVisualModel
{
	static int GetColumn(string itemId)
	{
		string id; int index;
		array<string> ids = {"APPLE","CANNABIS_PLANT","COCA_LEAF","CORN_COB","GRAPES","PEACHES","IRON_ORE","COPPER_ORE","GOLD_ORE","DIAMOND","OIL_SAND","IRON","COPPER","GOLD","OIL","CANNABIS_FLOWER","COCAINE","CANNED_CORN"};
		for (index = 0; index < ids.Count(); index++) if (ids[index] == itemId) return index % 6;
		return -1;
	}
	static int GetRow(string itemId)
	{
		array<string> ids = {"APPLE","CANNABIS_PLANT","COCA_LEAF","CORN_COB","GRAPES","PEACHES","IRON_ORE","COPPER_ORE","GOLD_ORE","DIAMOND","OIL_SAND","IRON","COPPER","GOLD","OIL","CANNABIS_FLOWER","COCAINE","CANNED_CORN"};
		for (int index = 0; index < ids.Count(); index++) if (ids[index] == itemId) return index / 6;
		return -1;
	}
	static string GetAtlasPath() { return "Assets/UI/Shop/RHD_VirtualItemAtlas.png"; }
	static string GetIconKey(string itemId) { return itemId; }
};
