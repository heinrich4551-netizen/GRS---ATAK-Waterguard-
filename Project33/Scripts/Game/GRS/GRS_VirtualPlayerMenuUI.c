class GRS_VirtualPlayerMenuUI : ChimeraMenuBase
{
	protected ButtonWidget m_wClose;
	protected TextWidget m_wMoney;
	protected TextWidget m_wContent;
	protected ref GRS_VirtualPlayerController m_Controller;

	protected override void OnMenuOpen()
	{
		super.OnMenuOpen();
		m_Controller = GRS_VirtualPlayerMenuService.GetInstance();
		m_wClose = ButtonWidget.Cast(GetRootWidget().FindWidget("GRS_Virtual_Close"));
		m_wMoney = TextWidget.Cast(GetRootWidget().FindWidget("GRS_Virtual_Money"));
		m_wContent = TextWidget.Cast(GetRootWidget().FindWidget("GRS_Virtual_Content"));
		if (m_wClose)
		{
			SCR_ButtonTextComponent closeButton = SCR_ButtonTextComponent.Cast(m_wClose.FindHandler(SCR_ButtonTextComponent));
			if (closeButton)
				closeButton.m_OnClicked.Insert(OnCloseClicked);
		}
		Refresh();
	}

	protected override void OnMenuClose()
	{
		if (m_wClose)
		{
			SCR_ButtonTextComponent closeButton = SCR_ButtonTextComponent.Cast(m_wClose.FindHandler(SCR_ButtonTextComponent));
			if (closeButton)
				closeButton.m_OnClicked.Remove(OnCloseClicked);
		}
		super.OnMenuClose();
	}

	protected void OnCloseClicked()
	{
		GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.GRS_Virtual_Player_Menu);
	}

	void Refresh()
	{
		if (!m_Controller)
			return;
		if (m_wMoney)
			m_wMoney.SetText("Virtual Player");
		if (m_wContent)
		{
			GRS_VirtualPlayerState state = m_Controller.GetState();
			m_wContent.SetText("VIRTUAL INVENTORY: " + state.m_aBaseInventory.Count().ToString() + "/" + GRS_VirtualPlayerConfig.MAX_BASE_INVENTORY_SLOTS.ToString() +
				"\nVIRTUAL ITEMS: " + state.m_aVirtualInventory.Count().ToString() + "/" + GRS_VirtualPlayerConfig.MAX_VIRTUAL_ITEM_TYPES.ToString() +
				"\nGARAGE: " + state.m_aGarage.Count().ToString() +
				"\nPROPERTY: " + state.m_aProperties.Count().ToString() + "/" + GRS_VirtualPlayerConfig.MAX_PROPERTIES.ToString() +
				"\n\nUse the Workbench tab buttons to expose actions.");
		}
	}
};

class GRS_VirtualPlayerMenuService
{
	protected static ref GRS_VirtualPlayerController s_Instance;

	static GRS_VirtualPlayerController GetInstance()
	{
		if (!s_Instance)
			s_Instance = new GRS_VirtualPlayerController();
		return s_Instance;
	}
};
