// ============================================================================
// RHD / PROJECT33 - DEEP ARSENAL-STYLE LOADOUT EDITOR UI
// ============================================================================
// Hosted inside the existing F8 virtual-player menu. The layout is intentionally
// made from optional widgets so one Workbench menu can support shallow or deep
// presentations without changing the code.
//
// Text widgets:
// RHD_Loadout_Status, RHD_Loadout_Profile, RHD_Loadout_Content,
// RHD_Loadout_Categories, RHD_Loadout_Dependency, RHD_Loadout_Summary
//
// Edit boxes:
// RHD_Loadout_Category, RHD_Loadout_Slot, RHD_Loadout_Item,
// RHD_Loadout_Attachment, RHD_Loadout_Quantity, RHD_Loadout_Container,
// RHD_Loadout_Zeroing
//
// Buttons:
// RHD_Loadout_Apply, RHD_Loadout_Save, RHD_Loadout_Refresh, RHD_Loadout_Clear,
// RHD_Loadout_SetItem, RHD_Loadout_AddItem, RHD_Loadout_RemoveItem,
// RHD_Loadout_AddAttach, RHD_Loadout_RemoveAttach, RHD_Loadout_SetZeroing,
// RHD_Loadout_Holster, RHD_Loadout_Validate, RHD_Loadout_WCS
// ============================================================================
class RHD_LoadoutEditorUI
{
	protected ref RHD_LoadoutEditorController m_Controller;
	protected Widget m_Root;
	protected TextWidget m_wStatus;
	protected TextWidget m_wProfile;
	protected TextWidget m_wContent;
	protected TextWidget m_wCategories;
	protected TextWidget m_wDependency;
	protected TextWidget m_wSummary;
	protected EditBoxWidget m_wCategory;
	protected EditBoxWidget m_wSlot;
	protected EditBoxWidget m_wItem;
	protected EditBoxWidget m_wAttachment;
	protected EditBoxWidget m_wQuantity;
	protected EditBoxWidget m_wContainer;
	protected EditBoxWidget m_wZeroing;
	protected ref array<ButtonWidget> m_aButtons = {};
	protected string m_sSelectedSlot = "PRIMARY";
	protected string m_sSelectedCategory = "WEAPON";

	void Initialize(Widget root)
	{
		m_Root = root;
		m_Controller = RHD_LoadoutEditorController.GetInstance();
		if (!m_Root || !m_Controller) return;
		m_wStatus = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Status"));
		m_wProfile = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Profile"));
		m_wContent = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Content"));
		m_wCategories = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Categories"));
		m_wDependency = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Dependency"));
		m_wSummary = TextWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Summary"));
		m_wCategory = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Category"));
		m_wSlot = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Slot"));
		m_wItem = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Item"));
		m_wAttachment = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Attachment"));
		m_wQuantity = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Quantity"));
		m_wContainer = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Container"));
		m_wZeroing = EditBoxWidget.Cast(m_Root.FindAnyWidget("RHD_Loadout_Zeroing"));
		BindApply();
		BindSave();
		BindRefresh();
		BindClear();
		BindButton("RHD_Loadout_SetItem", OnSetItem);
		BindButton("RHD_Loadout_AddItem", OnAddItem);
		BindButton("RHD_Loadout_RemoveItem", OnRemoveItem);
		BindButton("RHD_Loadout_AddAttach", OnAddAttachment);
		BindButton("RHD_Loadout_RemoveAttach", OnRemoveAttachment);
		BindButton("RHD_Loadout_SetZeroing", OnSetZeroing);
		BindButton("RHD_Loadout_Holster", OnHolster);
		BindButton("RHD_Loadout_Validate", OnValidate);
		BindButton("RHD_Loadout_WCS", OnOpenWCS);
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
	protected void BindApply() { BindButton("RHD_Loadout_Apply", OnApply); }
	protected void BindSave() { BindButton("RHD_Loadout_Save", OnSave); }
	protected void BindRefresh() { BindButton("RHD_Loadout_Refresh", OnRefresh); }
	protected void BindClear() { BindButton("RHD_Loadout_Clear", OnClear); }

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
			component.m_OnClicked.Remove(OnSetItem);
			component.m_OnClicked.Remove(OnAddItem);
			component.m_OnClicked.Remove(OnRemoveItem);
			component.m_OnClicked.Remove(OnAddAttachment);
			component.m_OnClicked.Remove(OnRemoveAttachment);
			component.m_OnClicked.Remove(OnSetZeroing);
			component.m_OnClicked.Remove(OnHolster);
			component.m_OnClicked.Remove(OnValidate);
			component.m_OnClicked.Remove(OnOpenWCS);
		}
		m_aButtons.Clear();
	}

	protected string GetCategory()
	{
		if (m_wCategory && !m_wCategory.GetText().IsEmpty()) m_sSelectedCategory = m_wCategory.GetText();
		return m_sSelectedCategory;
	}
	protected string GetSlot()
	{
		if (m_wSlot && !m_wSlot.GetText().IsEmpty()) m_sSelectedSlot = m_wSlot.GetText();
		return m_sSelectedSlot;
	}
	protected string GetItem() { return m_wItem ? m_wItem.GetText() : ""; }
	protected string GetAttachment() { return m_wAttachment ? m_wAttachment.GetText() : ""; }
	protected string GetContainer() { return m_wContainer ? m_wContainer.GetText() : "LOOSE"; }
	protected int GetQuantity() { return m_wQuantity ? m_wQuantity.GetText().ToInt() : 1; }
	protected void SetStatus(string text) { if (m_wStatus) m_wStatus.SetText(text); }

	protected void OnApply()
	{
		if (!m_Controller.Apply()) SetStatus("LOADOUT NOT APPLIED - validate or wire mission adapter"); else SetStatus("LOADOUT APPLIED");
		Refresh();
	}
	protected void OnSave()
	{
		if (!m_Controller.Save()) SetStatus("PROFILE NOT SAVED - validate or wire persistence adapter"); else SetStatus("PROFILE SAVED");
		Refresh();
	}
	protected void OnRefresh()
	{
		if (!m_Controller.RefreshCurrentLoadout()) SetStatus("EDITOR PREVIEW - current loadout import not wired"); else SetStatus("CURRENT LOADOUT IMPORTED");
		Refresh();
	}
	protected void OnClear()
	{
		if (m_Controller.GetModel().ClearItem(GetSlot())) SetStatus("CLEARED " + GetSlot()); else SetStatus("INVALID SLOT");
		Refresh();
	}
	protected void OnSetItem()
	{
		if (m_Controller.SetCategoryItem(GetCategory(), GetSlot(), GetItem())) SetStatus("ITEM SET"); else SetStatus("ITEM REJECTED - not in mission catalog / wrong category");
		Refresh();
	}
	protected void OnAddItem()
	{
		int quantity = GetQuantity();
		if (quantity <= 0) quantity = 1;
		if (m_Controller.AddLooseItem(GetItem(), GetItem(), GetCategory(), quantity)) SetStatus("ITEM ADDED: " + quantity.ToString()); else SetStatus("ITEM NOT AVAILABLE");
		Refresh();
	}
	protected void OnRemoveItem()
	{
		if (m_Controller.GetModel().AddLooseItem("", "", "", 0)) SetStatus("NO ACTION");
		SetStatus("REMOVE USES THE MISSION INVENTORY HANDLER AFTER CATALOG WIRING");
	}
	protected void OnAddAttachment()
	{
		if (m_Controller.AddAttachment(GetSlot(), GetAttachment())) SetStatus("ATTACHMENT ADDED"); else SetStatus("ATTACHMENT NOT COMPATIBLE");
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
		if (m_Controller.SetZeroing(GetSlot(), meters)) SetStatus("ZEROING SET: " + meters.ToString() + "m"); else SetStatus("ZEROING REJECTED");
		Refresh();
	}
	protected void OnHolster()
	{
		if (m_Controller.SetHolsterState("HOLSTERED")) SetStatus("HOLSTER STATE APPLIED"); else SetStatus("HOLSTER ADAPTER REQUIRED");
		Refresh();
	}
	protected void OnValidate() { SetStatus(m_Controller.ValidateActiveProfile() ? "LOADOUT VALID" : "LOADOUT INVALID"); Refresh(); }
	protected void OnOpenWCS()
	{
		if (m_Controller.GetMissionAdapter().OpenWCSLoadoutEditor()) SetStatus("WCS LOADOUT EDITOR OPENED"); else SetStatus("WCS HANDOFF REQUIRES MISSION WIRING");
	}

	void Refresh()
	{
		if (!m_Controller) return;
		RHD_LoadoutProfile profile = m_Controller.GetModel().GetActiveProfile();
		if (!profile) return;
		if (m_wProfile) m_wProfile.SetText("PROFILE: " + profile.m_sDisplayName);
		if (m_wCategory && m_wCategory.GetText().IsEmpty()) m_wCategory.SetText(m_sSelectedCategory);
		if (m_wSlot && m_wSlot.GetText().IsEmpty()) m_wSlot.SetText(m_sSelectedSlot);
		if (m_wQuantity && m_wQuantity.GetText().IsEmpty()) m_wQuantity.SetText("1");
		if (m_wDependency) m_wDependency.SetText("SUPPORTED: " + m_Controller.GetDependencySummary());
		if (m_wCategories) m_wCategories.SetText(m_Controller.GetCategorySummary());
		if (m_wSummary) m_wSummary.SetText(m_Controller.GetActiveProfileSummary());
		if (!m_wContent) return;
		string text = "ARSENAL LOADOUT\n";
		text += "WEAPONS -> OPTICS -> ATTACHMENTS -> MAGAZINES -> EQUIPMENT -> CONTAINERS\n\n";
		foreach (RHD_LoadoutSlot slot : profile.m_aSlots)
		{
			if (!slot) continue;
			text += slot.m_sSlotId + " | " + slot.m_sDisplayName + " | " + (slot.m_sItemId.IsEmpty() ? "EMPTY" : slot.m_sItemId);
			if (slot.m_iQuantity > 1) text += " x" + slot.m_iQuantity.ToString();
			text += "\n";
			if (slot.m_iZeroing > 0) text += "  ZEROING: " + slot.m_iZeroing.ToString() + "m\n";
			foreach (string attachment : slot.m_aAttachments) text += "  ATTACHMENT: " + attachment + "\n";
		}
		if (profile.m_aContainers.Count() > 0)
		{
			text += "\nCONTAINERS\n";
			foreach (RHD_LoadoutContainerEntry container : profile.m_aContainers)
				if (container) text += "  " + container.m_sContainerId + " -> " + container.m_sItemId + " x" + container.m_iQuantity.ToString() + "\n";
		}
		if (profile.m_aLooseItems.Count() > 0)
		{
			text += "\nLOOSE CARRIED\n";
			foreach (RHD_LoadoutContainerEntry loose : profile.m_aLooseItems)
				if (loose) text += "  " + loose.m_sCategory + " -> " + loose.m_sItemId + " x" + loose.m_iQuantity.ToString() + "\n";
		}
		m_wContent.SetText(text);
	}
};
