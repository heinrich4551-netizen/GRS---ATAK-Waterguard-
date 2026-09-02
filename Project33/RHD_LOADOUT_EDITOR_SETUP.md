# Project33 Loadout Editor

Project33 now includes a loadout editor layer hosted inside the existing F8 virtual-player menu.

## What it covers

The editor model exposes slots for:

- Primary / secondary / launcher weapons
- Primary, secondary and launcher optics
- Weapon muzzle / rail / light-laser attachments
- Helmet, uniform, gloves and boots
- Vest and modular rig
- Belt, bags and left/right droplegs
- Ear/comms and face/eye accessories
- Holster state
- Weapon zeroing
- Loadout profiles: Default, Medic, Rifleman, Machinegunner, Marksman, AA/AT, Custom

The intended dependency ownership is:

| Capability | Owner |
|---|---|
| Loadout/arsenal workflow | WCS_LoadoutEditor |
| WCS weapons | WCS_Weapons |
| WCS attachments | WCS_Attachments |
| WCS scopes | WCS_Scopes |
| WCS clothing | WCS_Clothing + WCS_Clothing_Assets |
| GRS apparel | GRS - Apparel |
| Vests/rigs | GRS - Modular Vests & Rigs |
| Bags/belts/droplegs | GRS - Belts & Bags & Droplegs + AAO GRS Bag |
| RHS content | RHS Content Packs + RHS Status Quo |
| Optics | Rayzis Optics + WCS_Scopes |
| Lasers/lights | RIS Laser Attachments |
| Holster behavior | Weapon Holstering |
| Zeroing behavior | Advanced Zeroing System |
| Weight/stamina behavior | NoWeightByCryneX + InfiniteStaminaByCryneX |

Weight/stamina are behavior systems, so the editor does not attempt to replace them.

## Minimal Workbench wiring

Use the existing `RHD_Virtual_Player_Menu` layout and add the following widgets under the root. The script looks them up by name, so they may be arranged however the mission author prefers.

### Text widgets

`RHD_Loadout_Status`

`RHD_Loadout_Profile`

`RHD_Loadout_Content`

### Edit boxes

`RHD_Loadout_Slot` — slot ID such as `PRIMARY`, `VEST`, `BAG`, `HOLSTER`

`RHD_Loadout_Item` — item/resource ID supplied by the mission catalog

`RHD_Loadout_Attachment` — attachment/resource ID

`RHD_Loadout_Zeroing` — zeroing distance in meters

### Buttons

`RHD_Loadout_Apply`

`RHD_Loadout_Save`

`RHD_Loadout_Refresh`

`RHD_Loadout_Clear`

`RHD_Loadout_AddAttach`

`RHD_Loadout_RemoveAttach`

`RHD_Loadout_SetZeroing`

`RHD_Loadout_Holster`

`RHD_Loadout_WCS`

The existing F8 menu automatically instantiates `RHD_LoadoutEditorUI` and binds those controls when they exist.

## Real mission integration

`RHD_LoadoutEditorMissionAdapter.c` is intentionally fail-safe. Wire it to the actual installed WCS/GRS/RHS equipment APIs and mission inventory component there.

Do not hard-code guessed prefab GUIDs. The mission adapter must provide the real item catalog and authoritative Apply/Save/Load operations.

The WCS documentation identifies `61D57616CAFBB23D-WCS_LoadoutEditor` as the WCS Loadout Editor and describes it as an enhanced loadout interface for arsenal crates. Project33 therefore complements it instead of attempting to replace its internal arsenal implementation.
