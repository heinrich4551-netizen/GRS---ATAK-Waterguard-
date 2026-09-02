class GRS_AdminMenuUI : ChimeraMenuBase
{
	protected ButtonWidget m_wClose;
	protected TextWidget m_wStatus;
	protected TextWidget m_wContent;
	protected ref GRS_AdminController m_Admin;

	protected override void OnMenuOpen()
	{
		super.OnMenuOpen();
		m_Admin = GRS_AdminMenuService.GetInstance();
		if (m_Admin)
			m_Admin.RefreshAuthorization();

		m_wClose = ButtonWidget.Cast(GetRootWidget().FindWidget("GRS_Admin_Close"));
		m_wStatus = TextWidget.Cast(GetRootWidget().FindWidget("GRS_Admin_Status"));
		m_wContent = TextWidget.Cast(GetRootWidget().FindWidget("GRS_Admin_Content"));

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
		GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.GRS_Admin_Menu);
	}

	void Refresh()
	{
		if (!m_Admin)
			return;

		if (m_wStatus)
		{
			if (m_Admin.IsAuthorized())
				m_wStatus.SetText("ADMIN AUTHORIZED | SERVER TOOLS ENABLED");
			else
				m_wStatus.SetText("ADMIN AUTHORIZATION REQUIRED");
		}

		if (m_wContent)
		{
			if (m_Admin.IsAuthorized())
				m_wContent.SetText("PLAYERS | MODERATION | TELEPORT | GAMEPLAY | SPAWN | WORLD | ECONOMY | SERVER | DIAGNOSTICS\n\n" + m_Admin.GetTabSummary());
			else
				m_wContent.SetText("F7 ADMIN MENU\n\nThis menu is fail-closed. Connect the mission's server-side admin permission system before enabling administrative actions.");
		}
	}
};

// A single local controller instance is shared by the F7 menu and the game mode.
class GRS_AdminMenuService
{
	protected static ref GRS_AdminController s_Instance;

	static GRS_AdminController GetInstance()
	{
		if (!s_Instance)
			s_Instance = new GRS_AdminController();
		return s_Instance;
	}
};
