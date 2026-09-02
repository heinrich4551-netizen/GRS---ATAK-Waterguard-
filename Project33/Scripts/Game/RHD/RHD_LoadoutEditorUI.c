// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR UI HELPER
// ============================================================================
// This helper is hosted inside the existing F8 virtual-player menu. It does
// not introduce a second menu preset, which keeps the Workbench setup small.
//
// Required optional widgets in the existing F8 layout:
// RHD_Loadout_Status        TextWidget
// RHD_Loadout_Profile       TextWidget
// RHD_Loadout_Slot          EditBoxWidget
// RHD_Loadout_Item          EditBoxWidget
// RHD_Loadout_Attachment    EditBoxWidget
// RHD_Loadout_Zeroing       EditBoxWidget
// RHD_Loadout_Content       TextWidget
// RHD_Loadout_Apply         ButtonWidget
// RHD_Loadout_Save          ButtonWidget
// RHD_Loadout_Refresh       ButtonWidget
// RHD_Loadout_Clear         ButtonWidget
// RHD_Loadout_AddAttach     ButtonWidget
// RHD_Loadout_RemoveAttach  ButtonWidget
// RHD_Loadout_SetZeroing    ButtonWidget
// RHD_Loadout_Holster       ButtonWidget
// RHD_Loadout_WCS            ButtonWidget
// ============================================================================
class RHD_LoadoutEditorUI
{
	protected ref RHD_LoadoutEditorController m_Controller;
	protected Widget m_Root;
	protected TextWidget m_wStatus;
	protected TextWidget m_wProfile;
	protected TextWidget m_wContent;
	protected EditBoxWidget m_wSlot;
	protected EditBoxWidget m_wItem;
	protected EditBoxWidget m_wAttachment;
	protected EditBoxWidget m_wZeroing;
	protected ref array<ButtonWidget> m_aButtons = {};
	protected string m_sSelectedSlot = "PRIMARY";

	void Initialize(Widget root)
	{
		m_Root = root;
		m_Controller = RHD_LoadoutEditorController.GetInstance();
		if (!m_Root || !m_Controller) return;
		m_wStatus = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Status"));
		m_wProfile = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Profile"));
		m_wContent = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Content"));
		m_wSlot = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Slot"));
		m_wItem = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Item"));
		m_wAttachment = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Attachment"));
		m_wZeroing = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Zeroing"));
		BindButton("RHD_Loadout_Apply", OnApply);
		BindButton("RHD_Loadout_Save", OnSave);
		BindButton("RHD_Loadout_Refresh", OnRefresh);
		BindButton("RHD_Loadout_Clear", OnClear);
		BindButton("RHD_Loadout_AddAttach", OnAddAttachment);
		BindButton("RHD_Loadout_RemoveAttach", OnRemoveAttachment);
		BindButton("RHD_Loadout_SetZeroing", OnSetZeroing);
		BindButton("RHD_Loadout_Holster", OnHolster);
		BindButton("RHD_Loadout_WCS", OnOpenWCS);
		Refresh();
	}

	protected void BindButton(string widgetName, func OnClick())
	{
		ButtonWidget button = ButtonWidget.Cast(m_Root.FindAnyWidget(widgetName));
		if (!button) return;
		SCR_ButtonTextComponent component = SCR_ButtonTextComponent.Cast(button.FindHandler(SCR_ButtonTextComponent));
		if (!component) return;
		component.m_OnClicked.Insert(OnClick);
		m_aButtons.Insert(button);
	}

	void Shutdown()
	{
		if (!m_Root) return;
		foreach (ButtonWidget button : m_aButtons)
		{
			if (!button) continue;
			SCR_ButtonTextComponent component = SCR_ButtonTextComponent.Cast(button.FindHandler(SCR_ButtonTextComponent));
			if (component) component.m_OnClicked.Remove(OnApply);
			if (component) component.m_OnClicked.Remove(OnSave);
			if (component) component.m_OnClicked.Remove(OnRefresh);
			if (component) component.m_OnClicked.Remove(OnClear);
			if (component) component.m_OnClicked.Remove(OnAddAttachment);
			if (component) component.m_OnClicked.Remove(OnRemoveAttachment);
			if (component) component.m_OnClicked.Remove(OnSetZeroing);
			if (component) component.m_OnClicked.Remove(OnHolster);
			if (component) component.m_OnClicked.Remove(OnOpenWCS);
		}
		m_aButtons.Clear();
	}

	protected string GetSlot()
	{
		if (m_wSlot && !m_wSlot.GetText().IsEmpty()) m_sSelectedSlot = m_wSlot.GetText();
		return m_sSelectedSlot;
	}

	protected string GetItem() { return m_wItem ? m_wItem.GetText() : ""; }
	protected string GetAttachment() { return m_wAttachment ? m_wAttachment.GetText() : ""; }

	protected void OnApply()
	{
		if (!m_Controller.Apply()) SetStatus("LOADOUT NOT APPLIED - wire WCS_LoadoutEditor adapter"); else SetStatus("LOADOUT APPLIED");
		Refresh();
	}
	protected void OnSave()
	{
		if (!m_Controller.Save()) SetStatus("PROFILE NOT SAVED - wire persistence/loadout adapter"); else SetStatus("PROFILE SAVED");
		Refresh();
	}
	protected void OnRefresh()
	{
		if (!m_Controller.RefreshCurrentLoadout()) SetStatus("USING EDITOR PREVIEW - current loadout adapter is not wired"); else SetStatus("CURRENT LOADOUT IMPORTED");
		Refresh();
	}
	protected void OnClear()
	{
		if (m_Controller.GetModel().ClearItem(GetSlot())) SetStatus("CLEARED " + GetSlot()); else SetStatus("INVALID SLOT");
		Refresh();
	}
	protected void OnAddAttachment()
	{
		if (m_Controller.AddAttachment(GetSlot(), GetAttachment())) SetStatus("ATTACHMENT ADDED"); else SetStatus("ATTACHMENT NOT AVAILABLE / ADAPTER REQUIRED");
		Refresh();
	}
	protected void OnRemoveAttachment()
	{
		if (m_Controller.RemoveAttachment(GetSlot(), GetAttachment())) SetStatus("ATTACHMENT REMOVED"); else SetStatus("ATTACHMENT NOT FOUND");
		Refresh();
	}
	protected void OnSetZeroing()
	{
		if (!m_wZeroing) return;
		int meters = m_wZeroing.GetText().ToInt();
		if (m_Controller.SetZeroing(GetSlot(), meters)) SetStatus("ZEROING SET: " + meters.ToString() + "m"); else SetStatus("ZEROING NOT AVAILABLE / ADAPTER REQUIRED");
		Refresh();
	}
	protected void OnHolster()
	{
		if (m_Controller.SetHolsterState("HOLSTERED")) SetStatus("HOLSTER STATE APPLIED"); else SetStatus("HOLSTER MOD ADAPTER REQUIRED");
		Refresh();
	}
	protected void OnOpenWCS()
	{
		if (m_Controller.GetMissionAdapter().OpenWCSLoadoutEditor()) SetStatus("WCS LOADOUT EDITOR OPENED"); else SetStatus("WCS LOADOUT EDITOR HANDOFF REQUIRES MISSION WIRING");
	}
	protected void SetStatus(string text) { if (m_wStatus) m_wStatus.SetText(text); }

	void Refresh()
	{
		if (!m_Controller) return;
		RHD_LoadoutProfile profile = m_Controller.GetModel().GetActiveProfile();
		if (m_wProfile) m_wProfile.SetText("PROFILE: " + (profile ? profile.m_sDisplayName : "NONE"));
		if (m_wSlot && m_wSlot.GetText().IsEmpty()) m_wSlot.SetText(m_sSelectedSlot);
		if (!m_wContent || !profile) return;
		string text = "LOADOUT EDITOR\n";
		text += "WCS / GRS / RHS / OPTICS / ATTACHMENTS / ZEROING / HOLSTER\n\n";
		foreach (RHD_LoadoutSlot slot : profile.m_aSlots)
		{
			if (!slot) continue;
			text += slot.m_sSlotId + " | " + slot.m_sDisplayName + " | " + (slot.m_sItemId.IsEmpty() ? "EMPTY" : slot.m_sItemId) + "\n";
			if (slot.m_iZeroing > 0) text += "  ZEROING: " + slot.m_iZeroing.ToString() + "m\n";
			foreach (string attachment : slot.m_aAttachments) text += "  ATTACHMENT: " + attachment + "\n";
		}
		text += "\nCATALOG: " + m_Controller.GetDependencySummary();
		m_wContent.SetText(text);
		if (m_wItem && !m_wItem.GetText().IsEmpty())
			m_Controller.GetModel().SetItem(GetSlot(), m_wItem.GetText());
	}
};
