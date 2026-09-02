class RHD_ATAKMenuUI : ChimeraMenuBase
{
	protected static const string BUTTON_CLOSE = "RHD_ATK_Close";
	protected static const string TEXT_MONEY = "RHD_ATK_Money";
	protected static const string TEXT_CONTENT = "RHD_ATK_Content";
	protected ref RHD_ATAKMenuController m_Controller;
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		m_Controller = RHD_ATAKMenuController.GetInstance();
		Widget root = GetRootWidget(); if (!root) return;
		SCR_ButtonTextComponent closeButton = SCR_ButtonTextComponent.GetButtonText(BUTTON_CLOSE, root); if (closeButton) closeButton.m_OnClicked.Insert(OnCloseClicked);
		RefreshStats();
		InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.AddActionListener("MenuBack", EActionTrigger.DOWN, CloseMenu);
	}
	override void OnMenuClose()
	{
		InputManager inputManager = GetGame().GetInputManager(); if (inputManager) inputManager.RemoveActionListener("MenuBack", EActionTrigger.DOWN, CloseMenu);
		super.OnMenuClose();
	}
	protected void OnCloseClicked() { CloseMenu(); }
	protected void CloseMenu(float value = 0, EActionTrigger reason = EActionTrigger.DOWN) { GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.RHD_ATAK_Menu); }
	void RefreshStats()
	{
		Widget root = GetRootWidget(); if (!root || !m_Controller) return;
		RHD_ATAKPlayerState state = m_Controller.GetPlayerState();
		TextWidget money = TextWidget.Cast(root.FindAnyWidget(TEXT_MONEY)); if (money) money.SetText("MONEY: $" + state.GetMoney().ToString() + " | CART: $" + m_Controller.GetShopCartTotal().ToString());
		TextWidget content = TextWidget.Cast(root.FindAnyWidget(TEXT_CONTENT)); if (!content) return;
		string shopText = "SHOP | ADD TO CART | REMOVE | CLEAR CART | CHECKOUT\n\n";
		for (int i = 0; i < m_Controller.GetShopItemCount(); i++)
		{
			string itemId = m_Controller.GetShopItem(i); shopText += itemId + "  $" + m_Controller.GetShopPrice(itemId).ToString() + "  [ICON:" + itemId + "]\n";
		}
		shopText += "\nSELL VIRTUAL ITEMS | SELL ALL BY ITEM TYPE\n\nCASINO | MAP | STATS | HANDLER";
		content.SetText(shopText);
	}
};
