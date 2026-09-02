# Project33 / RHD supported mod stack

Project33 is configured as a companion layer for the following exact Workshop stack supplied by the mission author.

| Mod | Version | Mod ID |
|---|---:|---|
| GRS - ATAK Plugin Core | 1.0.4 | 69F76987DB9F2214 |
| Weapon Holstering | 1.0.4 | 596F00811028DCC9 |
| WCS_LoadoutEditor | 8.0.3 | 61D57616CAFBB23D |
| AAO GRS Bag | 1.0.0 | 6A2081E42198A619 |
| GRS - Belts & Bags & Droplegs | 1.0.58 | 65183608D6DA5728 |
| Advanced Zeroing System | 1.0.22 | 6738DB574AAA1CB4 |
| Enfusion Database Framework | 0.6.10 | 5D6EA74A94173EDF |
| Rayzi Utils | 1.0.12 | 6632F94B46173164 |
| Anarchy Map Markers & Drawings | 2.0.24 | 69A510CE600D1126 |
| NoWeightByCryneX | 0.0.5 | 6A24CBB55832D64E |
| GRS - Apparel | 1.0.64 | 65157D09F042428A |
| WCS_Scopes | 8.0.0 | 62A668F513428630 |
| AdminCommandsByCryneX | 0.0.1 | 6A42D2545A633DE7 |
| Overthrow - RHS Compatibility | 1.5.0 | 5D8CAE1570401332 |
| InfiniteStaminaByCryneX | 0.0.1 | 6A2C72525A4E46F8 |
| SquadWaypointsByCryneX | 0.0.4 | 6A330A4644A0C2EF |
| RHS - Content Pack 02 | 0.16.5150 | BADC0DEDABBEDA5E |
| M224A1 - Handheld 60mm Mortar | 1.0.9 | 69C6E7E5C3006233 |
| GRS - Essentials | 1.0.68 | 6515672F96701110 |
| WCS_Weapons | 8.0.0 | 65CF7AE8574E06D2 |
| RIS Laser Attachments | 1.8.0 | 5ABD0CB57F7E9EB1 |
| Game Master Enhanced | 1.3.8 | 5964E0B3BB7410CE |
| RHS - Status Quo | 0.16.5150 | 595F2BF2F44836FB |
| UnlimitedGMBudgetByCryneX | 0.0.1 | 6A2ACE0A596BB7B8 |
| AdvancedCompassByCryneX | 0.0.2 | 6A2C9DDA5D87E077 |
| NametagRangeExtenderBYCryneX | 0.0.1 | 6A2E8B165DFC0BC1 |
| Thomoskis Modernised FIA | 1.0.12 | 69D007B68A1E8C20 |
| Anarchy Markers x GRS-ATAK DEV | 1.0.18 | 69DA0119707F0354 |
| Overthrow | 1.5.2 | 59B657D731E2A11D |
| WCS_Clothing | 8.0.0 | 6152CB0BD0684837 |
| WCS_Clothing_Assets | 8.0.0 | 6602C1EC7E5A4A87 |
| LHMBC - Handheld FDC | 1.0.0 | 69FD03EDDBCEEB4A |
| GRS - Modular Vests & Rigs | 1.0.44 | 651834C8D77BF86B |
| WCS_Sounds | 8.0.0 | 631C3C1AEE9C90BC |
| PlayerMapMarkersByCryneX | 0.0.4 | 6A2AE44858DDA1AE |
| Enfusion Persistence Framework | 0.6.18 | 5D6EBC81EB1842EF |
| WCS_RU | 8.0.0 | 615818DA7C0343FD |
| Air Assets and Operations | 0.5.6 | 694EFBCC38E93D25 |
| RHS - Content Pack 01 | 0.16.5150 | 1337C0DE5DABBEEF |
| WCS_Attachments | 8.0.0 | 61C74A8B647617DA |
| Realistic Combat Drones | 2.3.7 | 65AD60E204191D37 |
| Vampire UAV & FPV drones | 0.20.1 | 65B007413A813417 |
| GRS - Dev Framework | 1.0.50 | 65DACC64CE785B6C |
| Rayzis Optics | 1.3.81 | 628F720BC527C143 |
| SWAG_ATAK | 1.0.2 | 666D155A1AC8FA3C |
| WCS_NATO | 8.0.0 | 615806DC6C57AF02 |
| GRS - ATAK DEV | 1.0.74 | 69397BEDF03981C8 |
| GRS - Patches | 1.0.25 | 657B064AE0E231DF |
| WCS_Weapon_Scripts | 8.0.0 | 68F006D910E7546F |

## Replacement note

Bacon Loadout Editor is intentionally removed. Project33 now treats WCS_LoadoutEditor as the loadout-editor dependency.

Project33 does not call undocumented WCS APIs. The mission adapter remains the single place where real runtime inventory/loadout hooks should be wired after the exact installed WCS implementation is inspected in Workbench/source.
