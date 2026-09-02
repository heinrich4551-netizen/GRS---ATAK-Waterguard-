# Project33 / RHD mod integration

Project33 is designed to sit on top of the supplied Reforger stack rather than replacing it.

## Supported stack

The exact supported stack and versions are recorded in `Project33/RHD_MOD_STACK.md`.

Bacon Loadout Editor is **not** a dependency. Loadout-editor integration is now planned around **WCS_LoadoutEditor 8.0.3** (`61D57616CAFBB23D`). The public WCS documentation describes it as an enhanced loadout interface for arsenal crates, so Project33 treats it as the loadout/arsenal owner instead of reimplementing its internals.

## Ownership boundaries

| System | Preferred owner | Project33 behavior |
|---|---|---|
| ATAK foundation | GRS - ATAK Plugin Core / GRS - ATAK DEV / SWAG_ATAK | Uses its own RHD F6 companion layer; does not replace ATAK internals. |
| Map markers / drawings | Anarchy Map Markers & Drawings / Anarchy Markers x GRS-ATAK DEV / PlayerMapMarkersByCryneX | Provides map-intel integration points without duplicating marker systems. |
| Player equipment | WCS_LoadoutEditor / GRS Apparel / GRS Modular Vests & Rigs / GRS Belts & Bags & Droplegs / AAO GRS Bag / Weapon Holstering | Leaves loadout and equipment behavior to the source mods. |
| Inventory editing | WCS_LoadoutEditor / Edit Entity Inventory / Overthrow | Connected through `RHD_VirtualPlayerMissionAdapter.c`; no guessed third-party API calls. |
| Weapons / attachments / optics | WCS Weapons / WCS Attachments / WCS Scopes / RHS / Rayzis Optics / RIS Laser Attachments | Project33 exposes slots and delegates real item validation/application through `RHD_LoadoutEditorMissionAdapter.c`. |
| Zeroing | Advanced Zeroing System | Project33 exposes zeroing controls; actual zeroing is delegated to the dependency. |
| Economy / persistence | Overthrow, Enfusion Persistence Framework, Enfusion Database Framework | RHD keeps the companion profile model isolated; persistence is an adapter point. |
| Admin authority | AdminCommandsByCryneX / Game Master Enhanced | RHD admin actions remain fail-closed until mission authority is connected. |
| Vehicles / weapons | RHS, WCS, Air Assets, drones, scopes/attachments | RHD references mission-provided vehicle IDs instead of inventing prefab GUIDs. |
| AI jobs | Existing mission civilians | No job-specific civilian spawning. |
| Stamina / weight / holstering | InfiniteStaminaByCryneX / NoWeightByCryneX / Weapon Holstering | RHD does not duplicate or override these systems. |

## Loadout editor files

- `RHD_LoadoutEditorConfig.c` — easy feature switches and profile limits.
- `RHD_LoadoutEditorModel.c` — weapon/clothing/rig/bag/attachment/zeroing profile model.
- `RHD_LoadoutEditorController.c` — edit, save, load, apply and catalog operations.
- `RHD_LoadoutEditorMissionAdapter.c` — the only place that should contain real WCS/GRS/RHS/Overthrow item APIs.
- `RHD_LoadoutEditorUI.c` — UI helper hosted by the existing F8 menu.
- `RHD_LOADOUT_EDITOR_SETUP.md` — exact widget names for the Workbench layout.

## Minimal Workbench work

1. Open `Project33` in Workbench and let the resource database rebuild.
2. Verify the exact supported dependency mods are installed/enabled.
3. Add the documented loadout-editor widgets to the existing F8 layout.
4. Connect the real mission inventory/loadout APIs in `RHD_LoadoutEditorMissionAdapter.c` and `RHD_VirtualPlayerMissionAdapter.c`.
5. Wire the Anarchy marker/drawing integration to companion contract and territory-intelligence events.
6. Test F6/F7/F8 and the loadout editor in the target Overthrow mission.

The adapters intentionally return safe failure by default. This is preferable to shipping guessed APIs that could break another mod after an update.
