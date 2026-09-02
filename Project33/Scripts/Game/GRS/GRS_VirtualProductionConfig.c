// ============================================================================
// GRS VIRTUAL PRODUCTION CONFIG
// ============================================================================
// Easy-to-edit production definitions. All farming, mining, refining and
// processing items live here so prices and recipes are not buried in code.
// ============================================================================

class GRS_ProductionItem
{
	string m_sId;
	string m_sName;
	string m_sCategory;
	int m_iSellPrice;
};

class GRS_ProductionRecipe
{
	string m_sId;
	string m_sName;
	string m_sInputId;
	int m_iInputQuantity;
	string m_sOutputId;
	int m_iOutputMin;
	int m_iOutputMax;
};

class GRS_VirtualProductionConfig
{
	// ---------------- FARMING ----------------
	static const string APPLES = "FARM_APPLES";
	static const string CANNABIS_PLANT = "FARM_CANNABIS_PLANT";
	static const string COCA_LEAF = "FARM_COCA_LEAF";
	static const string CORN_COB = "FARM_CORN_COB";
	static const string GRAPES = "FARM_GRAPES";
	static const string PEACHES = "FARM_PEACHES";

	// ---------------- MINING ----------------
	static const string IRON_ORE = "MINE_IRON_ORE";
	static const string COPPER_ORE = "MINE_COPPER_ORE";
	static const string GOLD_ORE = "MINE_GOLD_ORE";
	static const string DIAMOND = "MINE_DIAMOND";
	static const string OIL_SAND = "MINE_OIL_SAND";

	// ---------------- REFINED / PROCESSED ----------------
	static const string IRON = "REFINED_IRON";
	static const string COPPER = "REFINED_COPPER";
	static const string GOLD = "REFINED_GOLD";
	static const string OIL = "REFINED_OIL";
	static const string CANNABIS_FLOUR = "PROC_CANNABIS_FLOUR";
	static const string COCAINE = "PROC_COCAINE";
	static const string CANNED_CORN = "PROC_CANNED_CORN";

	// Cannabis plant output is randomly selected between these inclusive values
	// when the production service is connected to the authoritative RNG.
	static const int CANNABIS_FLOUR_MIN = 100;
	static const int CANNABIS_FLOUR_MAX = 435;

	static void GetItems(out array<GRS_ProductionItem> items)
	{
		items = {};
		AddItem(items, APPLES, "Apples", "Farming", 18);
		AddItem(items, CANNABIS_PLANT, "Cannabis Plant", "Farming", 35);
		AddItem(items, COCA_LEAF, "Coca Leaf", "Farming", 30);
		AddItem(items, CORN_COB, "Corn Cob", "Farming", 12);
		AddItem(items, GRAPES, "Grapes", "Farming", 22);
		AddItem(items, PEACHES, "Peaches", "Farming", 20);

		AddItem(items, IRON_ORE, "Iron Ore", "Mining", 35);
		AddItem(items, COPPER_ORE, "Copper Ore", "Mining", 45);
		AddItem(items, GOLD_ORE, "Gold Ore", "Mining", 150);
		AddItem(items, DIAMOND, "Diamond", "Mining", 1000);
		AddItem(items, OIL_SAND, "Oil Sand", "Mining", 60);

		AddItem(items, IRON, "Iron", "Refining", 100);
		AddItem(items, COPPER, "Copper", "Refining", 125);
		AddItem(items, GOLD, "Gold", "Refining", 450);
		AddItem(items, OIL, "Oil", "Refining", 200);

		AddItem(items, CANNABIS_FLOUR, "Cannabis Flour", "Processing", 75);
		AddItem(items, COCAINE, "Cocaine", "Processing", 500);
		AddItem(items, CANNED_CORN, "Can of Corn", "Processing", 30);
	}

	static void GetRecipes(out array<GRS_ProductionRecipe> recipes)
	{
		recipes = {};
		AddRecipe(recipes, "REFINE_IRON", "Iron Ore -> Iron", IRON_ORE, 1, IRON, 1, 1);
		AddRecipe(recipes, "REFINE_COPPER", "Copper Ore -> Copper", COPPER_ORE, 1, COPPER, 1, 1);
		AddRecipe(recipes, "REFINE_GOLD", "Gold Ore -> Gold", GOLD_ORE, 1, GOLD, 1, 1);
		AddRecipe(recipes, "REFINE_OIL", "Oil Sand -> Oil", OIL_SAND, 1, OIL, 1, 1);
		AddRecipe(recipes, "PROCESS_CANNABIS", "Cannabis Plant -> Cannabis Flour", CANNABIS_PLANT, 1, CANNABIS_FLOUR, CANNABIS_FLOUR_MIN, CANNABIS_FLOUR_MAX);
		AddRecipe(recipes, "PROCESS_COCA", "200 Coca Leaf -> 1 Cocaine", COCA_LEAF, 200, COCAINE, 1, 1);
		AddRecipe(recipes, "PROCESS_CORN", "2 Corn Cob -> 1 Can of Corn", CORN_COB, 2, CANNED_CORN, 1, 1);
	}

	protected static void AddItem(out array<GRS_ProductionItem> items, string id, string name, string category, int sellPrice)
	{
		GRS_ProductionItem item = new GRS_ProductionItem();
		item.m_sId = id;
		item.m_sName = name;
		item.m_sCategory = category;
		item.m_iSellPrice = sellPrice;
		items.Insert(item);
	}

	protected static void AddRecipe(out array<GRS_ProductionRecipe> recipes, string id, string name, string inputId, int inputQuantity, string outputId, int outputMin, int outputMax)
	{
		GRS_ProductionRecipe recipe = new GRS_ProductionRecipe();
		recipe.m_sId = id;
		recipe.m_sName = name;
		recipe.m_sInputId = inputId;
		recipe.m_iInputQuantity = inputQuantity;
		recipe.m_sOutputId = outputId;
		recipe.m_iOutputMin = outputMin;
		recipe.m_iOutputMax = outputMax;
		recipes.Insert(recipe);
	}
};
