# F8 RHD Virtual Player Menu - Easy Setup

F8 opens **Virtual Inventory, Virtual Garage, Jobs, Property and Shop**.

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

Recommended tabs: Inventory, Garage, Jobs, Property, Shop.

## Virtual Inventory
- Up to 120 base-game inventory slots.
- Up to 435 virtual item types.
- Base-game items are removed from the real inventory before virtual storage succeeds.
- Virtual production items are menu-only.

## Shop
The shared shop is `RHD_Shop` and its default prices are in `RHD_ShopConfig.c`.

Every production item has a shop price and a visual identity. The visual atlas is:

`Assets/UI/Shop/RHD_VirtualItemAtlas.png`

The shop supports:
- Add item to cart
- Increase/decrease cart quantity
- Remove cart entries
- Clear cart
- Checkout the complete cart as one transaction
- Sell a selected quantity of a virtual item
- **SELL ALL** of one item type

F7's `SetShopPrice` changes the shared price table used by F6 and F8.

## Production items
### Farming
Apple, Cannabis Plant, Coca Leaf, Corn Cob, Grapes, Peaches.

### Mining
Iron Ore, Copper Ore, Gold Ore, Diamond, Oil Sand.

### Refining
1 Iron Ore -> 1 Iron
1 Copper Ore -> 1 Copper
1 Gold Ore -> 1 Gold
1 Oil Sand -> 1 Oil

Diamond has no refining recipe.

### Processing
1 Cannabis Plant -> random 100-435 Cannabis Flower
200 Coca Leaf -> 1 Cocaine
2 Corn Cob -> 1 Canned Corn

The recipe limits are in `Scripts/Game/RHD/RHD_VirtualProductionConfig.c`.

## Virtual Garage
The mission supplies the vehicle catalogue. Every vehicle made available by the mission can be exposed here; there is no arbitrary 50-vehicle cap.

The mission adapter must verify vehicle IDs and perform safe server-side spawning.

## Jobs
Jobs use **existing civilian AI already spawned in the mission**. F8 never spawns replacement civilians just to satisfy a job.

The intended chain is: source civilian -> contact civilian -> virtual delivery item -> destination civilian -> server-verified reward.

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

`Scripts/Game/RHD/RHD_VirtualPlayerMissionAdapter.c`

Connect real mission systems for:
- base-game inventory remove/give
- authoritative economy charge/credit
- mission vehicle validation/spawn
- existing civilian AI detection, selection and distance checks
- buildable/overlap validation
- property object spawning

Safe defaults reject unconnected operations. Do not add invented prefab GUIDs or undocumented engine APIs.

## Easy configuration
Normal limits/settings remain in:
- `Scripts/Game/RHD/RHD_VirtualPlayerConfig.c`
- `Scripts/Game/RHD/RHD_VirtualPlayerEasyConfig.c`
- `Scripts/Game/RHD/RHD_ShopConfig.c`
- `Scripts/Game/RHD/RHD_VirtualProductionConfig.c`

## Multiplayer
Purchases, inventory transfers, shop checkout, selling, jobs, garage spawning, property ownership and construction must be server-authoritative. The UI is only a request surface.

## Workbench note
The repository now uses **RHD filenames and the RHD script directory**. The PNG atlas is a normal repository asset, but the final Reforger UI texture/material/layout registration still needs to be imported/assigned through Workbench so the resource metadata is valid for the target project.
