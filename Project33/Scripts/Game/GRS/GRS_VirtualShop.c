// ============================================================================
// GRS VIRTUAL SHOP
// ============================================================================
// All sell prices come from GRS_VirtualProductionConfig.GetItems().
// Keep the shop separate from production so prices are easy to rebalance.
// The authoritative economy adapter must credit the player only after the
// item quantity has been removed successfully.
// ============================================================================

class GRS_VirtualShop
{
	static int GetSellPrice(string itemId)
	{
		GRS_ProductionItem item = FindItem(itemId);
		if (!item)
			return 0;
		return item.m_iSellPrice;
	}

	static bool CanSell(GRS_VirtualPlayerState state, string itemId, int quantity)
	{
		if (!state || itemId.IsEmpty() || quantity <= 0)
			return false;
		return state.GetVirtualItemQuantity(itemId) >= quantity && GetSellPrice(itemId) > 0;
	}

	static int CalculateSaleValue(string itemId, int quantity)
	{
		if (quantity <= 0)
			return 0;
		int price = GetSellPrice(itemId);
		if (price <= 0)
			return 0;
		return price * quantity;
	}

	static bool Sell(GRS_VirtualPlayerState state, string itemId, int quantity)
	{
		if (!CanSell(state, itemId, quantity))
			return false;
		int value = CalculateSaleValue(itemId, quantity);
		if (!state.RemoveVirtualItem(itemId, quantity))
			return false;
		// The menu/server integration should credit 'value' after this point.
		return value > 0;
	}

	static GRS_ProductionItem FindItem(string itemId)
	{
		array<GRS_ProductionItem> items = {};
		GRS_VirtualProductionConfig.GetItems(items);
		foreach (GRS_ProductionItem item : items)
			if (item && item.m_sId == itemId)
				return item;
		return null;
	}
};
