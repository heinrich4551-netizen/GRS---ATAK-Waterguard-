// GRS virtual production rules. Authoritative inventory changes belong on the server.
class GRS_VirtualProduction
{
	static bool CanProcess(GRS_VirtualPlayerState state, string recipeId)
	{
		if (!state || recipeId.IsEmpty())
			return false;
		GRS_ProductionRecipe recipe = FindRecipe(recipeId);
		if (!recipe)
			return false;
		return HasVirtualItem(state, recipe.m_sInputId, recipe.m_iInputQuantity);
	}

	static bool Process(GRS_VirtualPlayerState state, string recipeId, int outputQuantity)
	{
		if (!CanProcess(state, recipeId) || outputQuantity <= 0)
			return false;
		GRS_ProductionRecipe recipe = FindRecipe(recipeId);
		if (outputQuantity < recipe.m_iOutputMin || outputQuantity > recipe.m_iOutputMax)
			return false;
		if (!state.RemoveVirtualItem(recipe.m_sInputId, recipe.m_iInputQuantity))
			return false;
		return state.AddVirtualItem(recipe.m_sOutputId, GetItemName(recipe.m_sOutputId), outputQuantity);
	}

	static bool HasVirtualItem(GRS_VirtualPlayerState state, string itemId, int quantity)
	{
		if (!state || itemId.IsEmpty() || quantity <= 0)
			return false;
		foreach (GRS_VirtualInventoryEntry entry : state.m_aVirtualInventory)
		{
			if (entry && entry.m_sItemId == itemId && entry.m_bVirtual && entry.m_iQuantity >= quantity)
				return true;
		}
		return false;
	}

	static GRS_ProductionRecipe FindRecipe(string recipeId)
	{
		array<GRS_ProductionRecipe> recipes = {};
		GRS_VirtualProductionConfig.GetRecipes(recipes);
		foreach (GRS_ProductionRecipe recipe : recipes)
			if (recipe && recipe.m_sId == recipeId)
				return recipe;
		return null;
	}

	static string GetItemName(string itemId)
	{
		array<GRS_ProductionItem> items = {};
		GRS_VirtualProductionConfig.GetItems(items);
		foreach (GRS_ProductionItem item : items)
			if (item && item.m_sId == itemId)
				return item.m_sName;
		return itemId;
	}
};
