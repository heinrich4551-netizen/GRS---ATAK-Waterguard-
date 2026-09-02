class GRS_ATAKMenuUI : ChimeraMenuBase
{
	protected static const string BUTTON_CLOSE = "GRS_ATK_Close";
	protected static const string TEXT_MONEY = "GRS_ATK_Money";
	protected static const string TEXT_CONTENT = "GRS_ATK_Content";

	protected ref GRS_ATAKMapState m_MapState;

	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		m_MapState = new GRS_ATAKMapState();
		RefreshStats();

		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.AddActionListener("MenuBack", EActionTrigger.DOWN, CloseMenu);
	}

	override void OnMenuClose()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
			inputManager.RemoveActionListener("MenuBack", EActionTrigger.DOWN, CloseMenu);
		super.OnMenuClose();
	}

	protected void CloseMenu(float value = 0, EActionTrigger reason = EActionTrigger.DOWN)
	{
		GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.GRS_ATAK_Menu);
	}

	void RefreshStats()
	{
		Widget root = GetRootWidget();
		if (!root)
			return;

		TextWidget money = TextWidget.Cast(root.FindAnyWidget(TEXT_MONEY));
		if (money)
			money.SetText("$0");

		TextWidget content = TextWidget.Cast(root.FindAnyWidget(TEXT_CONTENT));
		if (content)
			content.SetText("Shop | Casino | Map | Stats | Handler");
	}
};
