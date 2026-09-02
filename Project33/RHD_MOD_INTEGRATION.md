# Project33 / RHD mod integration

Project33 is designed to sit on top of the supplied Reforger stack rather than replacing it.

## Supported stack

The exact supported stack and versions are recorded in `Project33/RHD_MOD_STACK.md`.

Bacon Loadout Editor is **not** a dependency. Loadout-editor integration is now planned around **WCS_LoadoutEditor 8.0.3** (`61D57616CAFBB23D`).

## Ownership boundaries

| System | Preferred owner | Project33 behavior |
|---|---|---|
| ATAK foundation | GRS - ATAK Plugin Core / GRS - ATAK DEV / SWAG_ATAK | Uses its own RHD F6 companion layer; does not replace ATAK internals. |
| Map markers / drawings | Anarchy Map Markers & Drawings / Anarchy Markers x GRS-ATAK DEV / PlayerMapMarkersByCryneX | Provides map-intel integration points without duplicating marker systems. |
| Player equipment | WCS_LoadoutEditor / GRS bags-bag belts / apparel / modular vests / Weapon Holstering | Leaves loadout and equipment behavior to the source mods. |
| Inventory editing | WCS_LoadoutEditor / Edit Entity Inventory / Overthrow | Connected through `RHD_VirtualPlayerMissionAdapter.c`; no guessed third-party API calls. |
| Economy / persistence | Overthrow, Enfusion Persistence Framework, Enfusion Database Framework | RHD keeps the companion profile model isolated; persistence is an adapter point. |
| Admin authority | AdminCommandsByCryneX / Game Master Enhanced | RHD admin actions remain fail-closed until mission authority is connected. |
| Vehicles / weapons | RHS, WCS, Air Assets, drones, scopes/attachments | RHD references mission-provided vehicle IDs instead of inventing prefab GUIDs. |
| AI jobs | Existing mission civilians | No job-specific civilian spawning. |
| Stamina / weight / holstering | InfiniteStaminaByCryneX / NoWeightByCryneX / Weapon Holstering | RHD does not duplicate or override these systems. |

## Minimal Workbench work

1. Open `Project33` in Workbench and let the resource database rebuild.
2. Verify the exact supported dependency mods are installed/enabled.
3. Connect the real mission inventory/loadout, civilian, vehicle, property and persistence calls in `RHD_VirtualPlayerMissionAdapter.c`.
4. Assign the generated shop atlas to the actual ImageWidget/material used by the shop layout.
5. Wire the Anarchy marker/drawing integration to the companion contract and territory-intelligence events.
6. Test F6/F7/F8 in the target Overthrow mission.

The adapter intentionally returns safe failure by default. This is preferable to shipping guessed APIs that could break another mod after an update.
