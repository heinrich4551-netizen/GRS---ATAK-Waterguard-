modded class SCR_BaseGameMode
{
	protected ref GRS_ATAKMenuController m_GRS_ATAKMenu;
	protected ref GRS_AdminController m_GRS_AdminMenu;

	override void OnGameStart()
	{
		super.OnGameStart();

		// F6 is a standalone menu. It never checks for an ATAK inventory item.
		m_GRS_ATAKMenu = new GRS_ATAKMenuController();
		m_GRS_ATAKMenu.Initialize();

		// F7 uses the same controller instance as the admin UI service.
		m_GRS_AdminMenu = GRS_AdminMenuService.GetInstance();
		m_GRS_AdminMenu.Initialize();
	}

	override void OnGameEnd()
	{
		if (m_GRS_ATAKMenu)
			m_GRS_ATAKMenu.Shutdown();
		m_GRS_ATAKMenu = null;

		if (m_GRS_AdminMenu)
			m_GRS_AdminMenu.Shutdown();
		m_GRS_AdminMenu = null;

		super.OnGameEnd();
	}
};
