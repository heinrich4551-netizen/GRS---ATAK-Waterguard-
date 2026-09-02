class RHD_VirtualPlayerMenuUI : ChimeraMenuBase
{
	protected ButtonWidget m_wClose;
	protected TextWidget m_wMoney;
	protected TextWidget m_wContent;
	protected ref RHD_VirtualPlayerController m_Controller;

	protected override void OnMenuOpen()
	{
		super.OnMenuOpen();
		m_Controller = RHD_VirtualPlayerMenuService.GetInstance();
		Widget root = GetRootWidget();
		if (!root) return;
		m_wClose = ButtonWidget.Cast(root.FindAnyWidget("RHD_Virtual_Close"));
		m_wMoney = TextWidget.Cast(root.FindAnyWidget("RHD_Virtual_Money"));
		m_wContent = TextWidget.Cast(root.FindAnyWidget("RHD_Virtual_Content"));
		if (m_wClose)
		{
			SCR_ButtonTextComponent closeButton = SCR_ButtonTextComponent.Cast(m_wClose.FindHandler(SCR_ButtonTextComponent));
			if (closeButton) closeButton.m_OnClicked.Insert(OnCloseClicked);
		}
		Refresh();
	}

	protected override void OnMenuClose()
	{
		if (m_wClose)
		{
			SCR_ButtonTextComponent closeButton = SCR_ButtonTextComponent.Cast(m_wClose.FindHandler(SCR_ButtonTextComponent));
			if (closeButton) closeButton.m_OnClicked.Remove(OnCloseClicked);
		}
		super.OnMenuClose();
	}

	protected void OnCloseClicked() { GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.RHD_Virtual_Player_Menu); }

	void Refresh()
	{
		if (!m_Controller) return;
		RHD_VirtualPlayerState state = m_Controller.GetState();
		RHD_ATAKPlayerState economy = m_Controller.GetSharedEconomy();
		if (!state) return;
		RHD_ContractRuntime.RefreshAvailable(state, m_Controller.GetMissionAdapter());
		if (m_wMoney) m_wMoney.SetText("MONEY: $" + (economy ? economy.GetMoney().ToString() : "0") + " | CART: $" + m_Controller.GetShopCartTotal().ToString());
		if (m_wContent)
		{
			m_wContent.SetText(
				"PROFILE: " + (state.m_Progression ? state.m_Progression.GetSummary() : "UNAVAILABLE") +
				"\nINVENTORY: " + state.m_aBaseInventory.Count().ToString() + "/" + RHD_VirtualPlayerConfig.MAX_BASE_INVENTORY_SLOTS.ToString() +
				"\nVIRTUAL ITEMS: " + state.m_aVirtualInventory.Count().ToString() + "/" + RHD_VirtualPlayerConfig.MAX_VIRTUAL_ITEM_TYPES.ToString() +
				"\nGARAGE: " + state.m_aGarage.Count().ToString() +
				"\nPROPERTY: " + state.m_aProperties.Count().ToString() + "/" + RHD_VirtualPlayerConfig.MAX_PROPERTIES.ToString() +
				"\nSHOP: Add To Cart | Remove From Cart | Clear Cart | CHECKOUT" +
				"\nSELL: Select item quantity or SELL ALL for one item type" +
				"\nPRODUCTION: Refine Ore | Process Farming Inputs" +
				"\n\n" + RHD_ContractRuntime.GetSummary(state));
		}
	}
};
class RHD_VirtualPlayerMenuService
{
	protected static ref RHD_VirtualPlayerController s_Instance;
	static RHD_VirtualPlayerController GetInstance()
	{
		if (!s_Instance) s_Instance = new RHD_VirtualPlayerController();
		return s_Instance;
	}
};
