modded class SCR_BaseGameMode
{
	protected ref GRS_ATAKMenuController m_GRS_ATAKMenu;
	protected ref GRS_AdminController m_GRS_AdminMenu;
	protected ref GRS_VirtualPlayerController m_GRS_VirtualPlayerMenu;

	override void OnGameStart()
	{
		super.OnGameStart();

		// F6 is standalone and never checks for an ATAK inventory item.
		m_GRS_ATAKMenu = new GRS_ATAKMenuController();
		m_GRS_ATAKMenu.Initialize();

		m_GRS_AdminMenu = GRS_AdminMenuService.GetInstance();
		m_GRS_AdminMenu.Initialize();

		m_GRS_VirtualPlayerMenu = GRS_VirtualPlayerMenuService.GetInstance();
		m_GRS_VirtualPlayerMenu.Initialize();
	}

	override void OnGameEnd()
	{
		if (m_GRS_ATAKMenu)
			m_GRS_ATAKMenu.Shutdown();
		m_GRS_ATAKMenu = null;

		if (m_GRS_AdminMenu)
			m_GRS_AdminMenu.Shutdown();
		m_GRS_AdminMenu = null;

		if (m_GRS_VirtualPlayerMenu)
			m_GRS_VirtualPlayerMenu.Shutdown();
		m_GRS_VirtualPlayerMenu = null;

		super.OnGameEnd();
	}
};
