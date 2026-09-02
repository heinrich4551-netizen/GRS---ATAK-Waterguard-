# RHD ATAK / Waterguard F6 Menu

The F6 menu is the player ATAK/economy surface. The runtime script prefix is **RHD**.

## Menu
- `RHD_ATAKMenuPreset.c` registers `RHD_ATAK_Menu`.
- `RHD_ATAKMenuUI` is the menu UI controller.
- `RHD_ATAKMenuController` owns F6 feature/economy state.
- `RHD_ATAKMenuModel` contains tab/action names.

## Workbench
Create/register the matching Chimera menu resource using `RHD_ATAK_Menu` and `RHD_ATAKMenuUI`.

Required widgets:
- `RHD_ATK_Close`
- `RHD_ATK_Money`
- `RHD_ATK_Content`

Create/bind the input action `RHD_ATAK_Menu` to F6.

## Economy
`RHD_ATAKConfig` contains the ATAK rewards/costs and shop-entry limit. `RHD_ATAKPlayerState` owns the current session money/statistics.

The shared shop service is `RHD_Shop`, with prices in `RHD_ShopConfig`. F6, F7 and F8 should use that service rather than separate price tables.

## Security
Economic transactions and gameplay actions must be authoritative on the server. Do not trust client-supplied prices, outcomes, resource IDs or reward amounts.

## Handler
`RHD_ATAKHandlerRequest` carries the mission-selected AI/support request. Real prefab/loadout integration belongs to the mission; do not fabricate resource IDs.

## Map
`RHD_ATAKMapState` is client-local. Map pins/labels/layers are not replicated by this class.

## Workbench resource note
The repository does not fabricate serialized Workbench UI resources or undocumented mission APIs. Existing physical filenames/directories may still contain `GRS` so resource paths are not invalidated during the code-prefix migration. Rename those resources in Workbench together with their references during final asset cleanup.
