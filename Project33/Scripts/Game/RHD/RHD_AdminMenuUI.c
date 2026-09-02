class RHD_AdminMenuUI : ChimeraMenuBase
{
	protected ButtonWidget m_wClose;
	protected TextWidget m_wStatus;
	protected TextWidget m_wContent;
	protected ref RHD_AdminController m_Admin;
	protected override void OnMenuOpen()
	{
		super.OnMenuOpen();
		m_Admin = RHD_AdminMenuService.GetInstance();
		if (m_Admin) m_Admin.RefreshAuthorization();
		m_wClose = ButtonWidget.Cast(GetRootWidget().FindWidget("RHD_Admin_Close"));
		m_wStatus = TextWidget.Cast(GetRootWidget().FindWidget("RHD_Admin_Status"));
		m_wContent = TextWidget.Cast(GetRootWidget().FindWidget("RHD_Admin_Content"));
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
	protected void OnCloseClicked() { GetGame().GetMenuManager().CloseMenuByPreset(ChimeraMenuPreset.RHD_Admin_Menu); }
	void Refresh()
	{
		if (!m_Admin) return;
		if (m_wStatus)
		{
			if (m_Admin.IsAuthorized()) m_wStatus.SetText("ADMIN AUTHORIZED | SERVER TOOLS ENABLED");
			else m_wStatus.SetText("ADMIN AUTHORIZATION REQUIRED");
		}
		if (m_wContent)
		{
			if (m_Admin.IsAuthorized()) m_wContent.SetText("PLAYERS | MODERATION | TELEPORT | GAMEPLAY | SPAWN | WORLD | ECONOMY | SERVER | DIAGNOSTICS\n\n" + m_Admin.GetTabSummary());
			else m_wContent.SetText("F7 ADMIN MENU\n\nThis menu is fail-closed. Connect the mission's server-side admin permission system before enabling administrative actions.");
		}
	}
};

class RHD_AdminMenuService
{
	protected static ref RHD_AdminController s_Instance;
	static RHD_AdminController GetInstance()
	{
		if (!s_Instance) s_Instance = new RHD_AdminController();
		return s_Instance;
	}
};
