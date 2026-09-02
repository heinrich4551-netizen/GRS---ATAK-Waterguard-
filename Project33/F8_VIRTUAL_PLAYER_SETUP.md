# F8 RHD Virtual Player Menu - Easy Setup

F8 opens **Virtual Inventory, Virtual Garage, Jobs and Property**.

## Input
Create/bind this action in Workbench:

`RHD_Virtual_Player_Menu`

Bind it to **F8**.

## Menu
Register `RHD_Virtual_Player_Menu` in the mod's Chimera menu configuration and connect it to `RHD_VirtualPlayerMenuUI`.

Required widgets:
- `RHD_Virtual_Close` - ButtonWidget
- `RHD_Virtual_Money` - TextWidget
- `RHD_Virtual_Content` - TextWidget

Recommended tabs: Inventory, Garage, Jobs, Property.

## Virtual Inventory
- Up to 120 base-game inventory slots.
- Up to 435 virtual item types.
- Base-game items are removed from the real inventory before virtual storage succeeds.
- Virtual items are menu-only and are not spawned as physical items.

## Virtual Garage
The mission supplies the vehicle catalogue. Every vehicle made available by the mission can be exposed in the garage; there is no arbitrary 50-vehicle cap.

The mission adapter must verify vehicle IDs and perform safe server-side spawning.

## Jobs
Jobs use **existing civilian AI already spawned in the mission**. F8 must never spawn replacement civilians just to satisfy a job.

The intended chain is: civilian -> civilian contact -> virtual delivery item -> destination civilian -> server-verified reward.

## Property
- Maximum 10 properties per player.
- Build radius up to 500 ft / 152.4 m.
- 500 ft radius costs $15,000,000.
- Each Storage object adds 100 virtual storage.
- Property storage caps at 15,000.
- Construction categories: Storage, Arsenal, Decoration and Fortification.
- Placement must be validated against the map and other player properties by the mission integration.

## Mission adapter
Mission-specific connections belong in:

`Scripts/Game/GRS/GRS_VirtualPlayerMissionAdapter.c`

The class is now named `RHD_VirtualPlayerMissionAdapter`. Connect real mission systems for:
- base-game inventory remove/give
- authoritative economy charge/credit
- mission vehicle validation/spawn
- existing civilian AI detection, selection and distance checks
- buildable/overlap validation
- property object spawning

Safe defaults reject unconnected operations. Do not add invented prefab GUIDs or undocumented engine APIs.

## Easy configuration
Normal limits/settings remain in:
- `Scripts/Game/GRS/GRS_VirtualPlayerConfig.c`
- `Scripts/Game/GRS/GRS_VirtualPlayerEasyConfig.c`

The runtime class prefix is **RHD**. Existing filenames/directories are retained for now so the change does not invalidate Workbench resource paths; rename the physical files/resources in Workbench together with any resource references when doing the final asset cleanup.

## Multiplayer
Purchases, inventory transfers, jobs, garage spawning, property ownership and construction must be server-authoritative. The UI is only a request surface.
