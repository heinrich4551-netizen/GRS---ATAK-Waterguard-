modded class SCR_BaseGameMode
{
	protected ref GRS_ATAKMenuController m_GRS_ATAKMenu;

	override void OnGameStart()
	{
		super.OnGameStart();

		m_GRS_ATAKMenu = new GRS_ATAKMenuController();
		m_GRS_ATAKMenu.Initialize();
	}

	override void OnGameEnd()
	{
		if (m_GRS_ATAKMenu)
			m_GRS_ATAKMenu.Shutdown();
		m_GRS_ATAKMenu = null;

		super.OnGameEnd();
	}
};
