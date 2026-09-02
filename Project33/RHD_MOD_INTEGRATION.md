# Project33 / RHD mod integration

Project33 is designed to sit on top of the supplied Reforger stack rather than replacing it.

## Dependency loading

`Project33/addon.gproj` declares the supplied mod IDs plus the base Arma Reforger dependency. This makes the intended load order visible to Workshop/Workbench and prevents Project33 from silently assuming another addon is present.

## Ownership boundaries

| System | Preferred owner | Project33 behavior |
|---|---|---|
| ATAK foundation | GRS - ATAK Plugin Core / GRS - ATAK DEV / SWAG_ATAK | Uses its own RHD F6 menu layer; does not replace ATAK internals. |
| Player equipment | GRS bags/belts/apparel/modular vests, Weapon Holstering | Leaves equipment behavior to the source mods. |
| Inventory editing | Edit Entity Inventory / Bacon Loadout Editor / Overthrow | Connected through `RHD_VirtualPlayerMissionAdapter.c`; no guessed third-party API calls. |
| Economy / persistence | Overthrow, Enfusion Persistence Framework, Enfusion Database Framework | RHD keeps the gameplay model isolated; persistence is an adapter point. |
| Admin | AdminCommandsByCryneX / Game Master Enhanced | RHD admin actions remain fail-closed until mission authority is connected. |
| Vehicles / weapons | RHS, WCS, Air Assets, drones, scopes/attachments | RHD references mission-provided vehicle IDs instead of inventing prefab GUIDs. |
| AI jobs | Existing mission civilians | No job-specific civilian spawning. |
| Stamina / weight / holstering | The listed specialist mods | RHD does not duplicate or override these systems. |

## Minimal Workbench work

1. Open `Project33` in Workbench and let the resource database rebuild.
2. Verify the supplied dependency mods are installed/enabled.
3. Connect the real mission inventory, civilian, vehicle, property and persistence calls in `RHD_VirtualPlayerMissionAdapter.c`.
4. Assign the generated shop atlas to the actual ImageWidget/material used by the shop layout.
5. Test F6/F7/F8 in the target mission.

The adapter intentionally returns safe failure by default. This is preferable to shipping guessed APIs that could break another mod after an update.
