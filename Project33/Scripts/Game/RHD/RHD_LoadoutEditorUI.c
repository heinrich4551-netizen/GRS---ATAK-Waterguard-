// ============================================================================
// RHD / PROJECT33 - LOADOUT EDITOR UI HELPER
// ============================================================================
// Hosted inside the existing F8 virtual-player menu; no second menu preset.
// Optional widgets in the existing F8 layout:
//
// RHD_Loadout_Status, RHD_Loadout_Profile, RHD_Loadout_Content   TextWidget
// RHD_Loadout_Slot, RHD_Loadout_Item, RHD_Loadout_Attachment,
// RHD_Loadout_Zeroing                                                EditBoxWidget
// RHD_Loadout_Apply, RHD_Loadout_Save, RHD_Loadout_Refresh,
// RHD_Loadout_Clear, RHD_Loadout_AddAttach, RHD_Loadout_RemoveAttach,
// RHD_Loadout_SetZeroing, RHD_Loadout_Holster, RHD_Loadout_WCS    ButtonWidget
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
		BindApply();
		BindSave();
		BindRefresh();
		BindClear();
		BindAddAttachment();
		BindRemoveAttachment();
		BindSetZeroing();
		BindHolster();
		BindOpenWCS();
		Refresh();
	}

	protected void BindButton(string widgetName, void callback())
	{
		ButtonWidget button = ButtonWidget.Cast(m_Root.FindAnyWidget(widgetName));
		if (!button) return;
		SCR_ButtonTextComponent component = SCR_ButtonTextComponent.Cast(button.FindHandler(SCR_ButtonTextComponent));
		if (!component) return;
		component.m_OnClicked.Insert(callback);
		m_aButtons.Insert(button);
	}
	protected void BindApply() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Apply")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnApply); m_aButtons.Insert(b); } } }
	protected void BindSave() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Save")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnSave); m_aButtons.Insert(b); } } }
	protected void BindRefresh() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Refresh")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnRefresh); m_aButtons.Insert(b); } } }
	protected void BindClear() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Clear")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnClear); m_aButtons.Insert(b); } } }
	protected void BindAddAttachment() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_AddAttach")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnAddAttachment); m_aButtons.Insert(b); } } }
	protected void BindRemoveAttachment() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_RemoveAttach")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnRemoveAttachment); m_aButtons.Insert(b); } } }
	protected void BindSetZeroing() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_SetZeroing")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnSetZeroing); m_aButtons.Insert(b); } } }
	protected void BindHolster() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Holster")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnHolster); m_aButtons.Insert(b); } } }
	protected void BindOpenWCS() { ButtonWidget b = ButtonWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_WCS")); if (b) { SCR_ButtonTextComponent c = SCR_ButtonTextComponent.Cast(b.FindHandler(SCR_ButtonTextComponent)); if (c) { c.m_OnClicked.Insert(OnOpenWCS); m_aButtons.Insert(b); } } }

	void Shutdown()
	{
		foreach (ButtonWidget button : m_aButtons)
		{
			if (!button) continue;
			SCR_ButtonTextComponent component = SCR_ButtonTextComponent.Cast(button.FindHandler(SCR_ButtonTextComponent));
			if (!component) continue;
			component.m_OnClicked.Remove(OnApply);
			component.m_OnClicked.Remove(OnSave);
			component.m_OnClicked.Remove(OnRefresh);
			component.m_OnClicked.Remove(OnClear);
			component.m_OnClicked.Remove(OnAddAttachment);
			component.m_OnClicked.Remove(OnRemoveAttachment);
			component.m_OnClicked.Remove(OnSetZeroing);
			component.m_OnClicked.Remove(OnHolster);
			component.m_OnClicked.Remove(OnOpenWCS);
		}
		m_aButtons.Clear();
	}

	protected string GetSlot()
	{
		if (m_wSlot && !m_wSlot.GetText().IsEmpty()) m_sSelectedSlot = m_wSlot.GetText();
		return m_sSelectedSlot;
	}
	protected string GetAttachment() { return m_wAttachment ? m_wAttachment.GetText() : ""; }
	protected void SetStatus(string text) { if (m_wStatus) m_wStatus.SetText(text); }

	protected void OnApply() { if (!m_Controller.Apply()) SetStatus("LOADOUT NOT APPLIED - WCS/missions adapter not wired"); else SetStatus("LOADOUT APPLIED"); Refresh(); }
	protected void OnSave() { if (!m_Controller.Save()) SetStatus("PROFILE NOT SAVED - persistence adapter not wired"); else SetStatus("PROFILE SAVED"); Refresh(); }
	protected void OnRefresh() { if (!m_Controller.RefreshCurrentLoadout()) SetStatus("EDITOR PREVIEW - current loadout import not wired"); else SetStatus("CURRENT LOADOUT IMPORTED"); Refresh(); }
	protected void OnClear() { if (m_Controller.GetModel().ClearItem(GetSlot())) SetStatus("CLEARED " + GetSlot()); else SetStatus("INVALID SLOT"); Refresh(); }
	protected void OnAddAttachment() { if (m_Controller.AddAttachment(GetSlot(), GetAttachment())) SetStatus("ATTACHMENT ADDED"); else SetStatus("ATTACHMENT NOT AVAILABLE / ADAPTER REQUIRED"); Refresh(); }
	protected void OnRemoveAttachment() { if (m_Controller.RemoveAttachment(GetSlot(), GetAttachment())) SetStatus("ATTACHMENT REMOVED"); else SetStatus("ATTACHMENT NOT FOUND"); Refresh(); }
	protected void OnSetZeroing() { if (!m_wZeroing) return; int meters = m_wZeroing.GetText().ToInt(); if (m_Controller.SetZeroing(GetSlot(), meters)) SetStatus("ZEROING SET: " + meters.ToString() + "m"); else SetStatus("ZEROING NOT AVAILABLE / ADAPTER REQUIRED"); Refresh(); }
	protected void OnHolster() { if (m_Controller.SetHolsterState("HOLSTERED")) SetStatus("HOLSTER STATE APPLIED"); else SetStatus("HOLSTER ADAPTER REQUIRED"); Refresh(); }
	protected void OnOpenWCS() { if (m_Controller.GetMissionAdapter().OpenWCSLoadoutEditor()) SetStatus("WCS LOADOUT EDITOR OPENED"); else SetStatus("WCS HANDOFF REQUIRES MISSION WIRING"); }

	void Refresh()
	{
		if (!m_Controller) return;
		RHD_LoadoutProfile profile = m_Controller.GetModel().GetActiveProfile();
		if (m_wProfile) m_wProfile.SetText("PROFILE: " + (profile ? profile.m_sDisplayName : "NONE"));
		if (m_wSlot && m_wSlot.GetText().IsEmpty()) m_wSlot.SetText(m_sSelectedSlot);
		if (!m_wContent || !profile) return;
		string text = "LOADOUT EDITOR\nWCS / GRS / RHS / OPTICS / ATTACHMENTS / ZEROING / HOLSTER\n\n";
		foreach (RHD_LoadoutSlot slot : profile.m_aSlots)
		{
			if (!slot) continue;
			text += slot.m_sSlotId + " | " + slot.m_sDisplayName + " | " + (slot.m_sItemId.IsEmpty() ? "EMPTY" : slot.m_sItemId) + "\n";
			if (slot.m_iZeroing > 0) text += "  ZEROING: " + slot.m_iZeroing.ToString() + "m\n";
			foreach (string attachment : slot.m_aAttachments) text += "  ATTACHMENT: " + attachment + "\n";
		}
		text += "\nCATALOG: " + m_Controller.GetDependencySummary();
		m_wContent.SetText(text);
		if (m_wItem && !m_wItem.GetText().IsEmpty()) m_Controller.GetModel().SetItem(GetSlot(), m_wItem.GetText());
	}
};
