modded class SCR_BaseGameMode
{
	protected ref RHD_ATAKMenuController m_RHD_ATAKMenu;
	protected ref RHD_AdminController m_RHD_AdminMenu;
	protected ref RHD_VirtualPlayerController m_RHD_VirtualPlayerMenu;

	override void OnGameStart()
	{
		super.OnGameStart();
		m_RHD_ATAKMenu = new RHD_ATAKMenuController();
		m_RHD_ATAKMenu.Initialize();
		m_RHD_AdminMenu = RHD_AdminMenuService.GetInstance();
		m_RHD_AdminMenu.Initialize();
		m_RHD_VirtualPlayerMenu = RHD_VirtualPlayerMenuService.GetInstance();
		m_RHD_VirtualPlayerMenu.Initialize();
	}

	override void OnGameEnd()
	{
		if (m_RHD_ATAKMenu)
			m_RHD_ATAKMenu.Shutdown();
		m_RHD_ATAKMenu = null;
		if (m_RHD_AdminMenu)
			m_RHD_AdminMenu.Shutdown();
		m_RHD_AdminMenu = null;
		if (m_RHD_VirtualPlayerMenu)
			m_RHD_VirtualPlayerMenu.Shutdown();
		m_RHD_VirtualPlayerMenu = null;
		super.OnGameEnd();
	}
};
