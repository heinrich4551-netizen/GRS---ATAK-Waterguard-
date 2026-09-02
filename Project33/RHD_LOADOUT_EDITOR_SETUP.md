# Project33 Deep Loadout Editor

Project33 now provides a deeper, arsenal-style loadout editor hosted inside the existing F8 virtual-player menu. The design is intentionally closer to a serious arsenal/loadout workflow while staying inside normal Arma Reforger equipment and inventory concepts.

## Design goal

The editor does **not** replace WCS_LoadoutEditor. WCS_LoadoutEditor remains the source-of-truth owner for its arsenal/loadout workflow; Project33 supplies a unified front end around the supplied mod stack and hands real equipment transactions to the mission adapter.

The WCS documentation confirms that the WCS editor works with arsenal crates and saved loadout slots, including faction-specific slot configuration. Project33 therefore exposes the same kinds of player-facing concepts without inventing a parallel equipment database.

## Deep customization supported by the model

### Weapon bench

Each weapon can be treated as a configurable platform with:

- Primary weapon
- Secondary weapon
- Launcher
- Optic/sight
- Muzzle device
- Rail/grip attachment
- Light/laser
- Underbarrel attachment
- Compatible magazine
- Spare magazines
- Weapon-specific zeroing

Actual compatibility is delegated to the mission adapter, so an invalid optic, magazine or attachment is rejected instead of being forced into the loadout.

### Clothing and protection

The model exposes:

- Helmet
- Uniform
- Gloves
- Boots
- Vest
- Modular rig/chest system
- Belt
- Left/right droplegs
- Backpack/bag
- Ear/comms
- Face/eye protection
- Patches/armbands
- Holster state

This maps directly onto the supplied WCS, GRS, RHS and Weapon Holstering ecosystem instead of creating duplicate equipment behavior.

### Carried equipment

The editor also supports two inventory concepts:

1. **Container contents** — items assigned to a named bag/vest/rig/container.
2. **Loose/carried items** — magazines, throwables, explosives, medical supplies and tools carried outside a specific named container.

Quantities are bounded by the editor config so the UI stays within sensible Reforger-style inventory limits.

### Dependency-aware categories

The category catalog exposes:

`WEAPON`
`OPTIC`
`ATTACHMENT`
`MAGAZINE`
`THROWABLE`
`EXPLOSIVE`
`MEDICAL`
`TOOL`
`HELMET`
`CLOTHING`
`VEST`
`RIG`
`BELT`
`DROPLEG`
`BAG`
`COMMS`
`ACCESSORY`
`PATCH`
`HOLSTER`

The actual item list for each category comes from `RHD_LoadoutEditorMissionAdapter.c`.

## Supplied dependencies used by the editor

| Capability | Owner |
|---|---|
| Loadout/arsenal workflow | WCS_LoadoutEditor |
| WCS weapons | WCS_Weapons |
| WCS attachments | WCS_Attachments |
| WCS scopes | WCS_Scopes |
| WCS clothing/assets | WCS_Clothing + WCS_Clothing_Assets |
| GRS apparel | GRS - Apparel |
| Vests / rigs | GRS - Modular Vests & Rigs |
| Belts / bags / droplegs | GRS - Belts & Bags & Droplegs + AAO GRS Bag |
| Patches | GRS - Patches |
| RHS equipment/content | RHS Content Packs + RHS Status Quo |
| Additional optics | Rayzis Optics |
| Lasers / lights | RIS Laser Attachments |
| Holster behavior | Weapon Holstering |
| Zeroing behavior | Advanced Zeroing System |
| Weight behavior | NoWeightByCryneX |
| Stamina behavior | InfiniteStaminaByCryneX |

Weight and stamina are behavior systems. The editor never tries to bypass or replace them.

## User-facing workflow

A full UI presentation should be organized as:

**LOADOUTS** → saved profiles / faction slots

**WEAPONS** → weapon selection and weapon-specific configuration

**OPTICS** → compatible sights for selected weapon

**ATTACHMENTS** → compatible muzzle / rail / light / laser / grip options

**AMMO** → compatible magazines and quantities

**EQUIPMENT** → helmet / uniform / vest / rig / belt / dropleg / bag / accessories

**CARRIED** → medical / tools / throwables / explosives / extra magazines

**VERIFY** → compatibility and capacity validation

**APPLY** → authoritative mission transaction

**SAVE** → WCS/persistence transaction

This is deliberately a richer workflow than a flat list of item IDs.

## Minimal Workbench work

Keep the existing `RHD_Virtual_Player_Menu` preset and add these optional widgets under its root.

### Text widgets

`RHD_Loadout_Status`

`RHD_Loadout_Profile`

`RHD_Loadout_Content`

`RHD_Loadout_Categories`

`RHD_Loadout_Dependency`

`RHD_Loadout_Summary`

### Edit boxes

`RHD_Loadout_Category`

`RHD_Loadout_Slot`

`RHD_Loadout_Item`

`RHD_Loadout_Attachment`

`RHD_Loadout_Quantity`

`RHD_Loadout_Container`

`RHD_Loadout_Zeroing`

### Buttons

`RHD_Loadout_Apply`

`RHD_Loadout_Save`

`RHD_Loadout_Refresh`

`RHD_Loadout_Clear`

`RHD_Loadout_SetItem`

`RHD_Loadout_AddItem`

`RHD_Loadout_RemoveItem`

`RHD_Loadout_AddAttach`

`RHD_Loadout_RemoveAttach`

`RHD_Loadout_SetZeroing`

`RHD_Loadout_Holster`

`RHD_Loadout_Validate`

`RHD_Loadout_WCS`

The UI code only uses widgets that exist, so a compact initial layout can be expanded later without rewriting the controller.

## Real mission integration

`RHD_LoadoutEditorMissionAdapter.c` is intentionally fail-safe. It now exposes dedicated boundaries for:

- current loadout import
- WCS saved profiles
- category enumeration
- compatible attachments
- compatible magazines
- compatible optics
- item display names
- inventory add/remove/move
- player container enumeration
- container capacity
- zeroing
- holster state

The exact installed APIs must be wired there. No undocumented WCS/GRS/RHS calls or prefab GUIDs are guessed.

This matters because WCS Loadout Editor uses real Reforger arsenal/loadout resources and saved faction slots; Project33 should integrate with those resources rather than creating fake substitute equipment.
