# F8 Virtual Player Menu - Easy Setup

## What F8 provides

F8 opens four player systems: **Virtual Inventory, Virtual Garage, Jobs and Property**.

The limits are already configured: 120 base-game inventory slots, 435 virtual item types, 10 properties, 100 storage per storage object and 15,000 storage per property. A 500 ft property radius costs $15,000,000.

## 1. Open the project

1. Open `Project33` in Arma Reforger Workbench.
2. Let Workbench finish importing/indexing the scripts.
3. Do not rename the `GRS_` script files.

## 2. Add the F8 input

In **Game > Input > Actions**, create an action named exactly:

`GRS_Virtual_Player_Menu`

Bind it to **F8** for keyboard/PC. If the mission already owns F8, choose another key and change the binding only; do not rename the action.

## 3. Create the menu layout

Create one UI layout for `GRS_VirtualPlayerMenuUI` and register it with the existing Chimera menu configuration.

The root widget must contain these named widgets:

- `GRS_Virtual_Close` - ButtonWidget
- `GRS_Virtual_Money` - TextWidget
- `GRS_Virtual_Content` - TextWidget

For a beginner-friendly layout, make four tab buttons named Inventory, Garage, Jobs and Property, then place a scrollable content panel under them. The script layer already owns the feature rules; the layout only needs to present buttons and lists.

## 4. Make the four tabs useful

### Inventory
- **Store** removes a selected base-game item from the player's real inventory and places it in the 120-slot virtual inventory.
- **Take** removes it from virtual storage and gives it back to the player.
- Virtual items are separate from base-game items and can be purchased from the menu.

### Garage
- The mission supplies the vehicle catalogue. Every vehicle that the mission makes available can be exposed here.
- Purchase stores ownership.
- Spawn retrieves the selected stored vehicle at a safe server-selected location.
- There is no arbitrary 50-vehicle cap.

### Jobs
Jobs use this sequence:

1. Random civilian gives the player a task.
2. Player talks to the required civilian.
3. That civilian can send the player to another civilian.
4. Player receives a virtual delivery item.
5. Player delivers the item to the destination civilian.
6. Server verifies the complete chain and pays the reward.

Never mark a job complete from a client-only button press.

### Property
- Player can own up to 10 buildings.
- Build radius is selectable up to 500 ft / 152.4 m.
- Price scales with radius and is capped at $15,000,000.
- Each Storage object adds 100 virtual storage.
- Property storage caps at 15,000.
- Property objects are divided into Storage, Arsenal, Decoration and Fortification.
- A player's connected building cluster can use the 500 ft build range, but another player's building must still block/limit placement according to the mission's overlap rule.

## 5. Mission-specific connection

The file `Scripts/Game/GRS/GRS_VirtualPlayerMissionAdapter.c` is the single place intended for mission-specific connections. The core mod deliberately does **not** invent prefab GUIDs or undocumented inventory/AI APIs.

Connect these operations to the mission's existing systems:

- remove/give a base-game inventory item
- charge/credit the player's authoritative money
- verify a vehicle is available in the current mission
- spawn a purchased vehicle
- identify civilian AI and test interaction distance
- choose a random civilian
- validate map/building placement
- spawn Storage, Arsenal, Decoration and Fortification prefabs

The safe defaults reject these operations until they are connected. This prevents a client from creating money, items, vehicles or property by itself.

## 6. Do not put GUIDs in the scripts

Use Workbench to select the real prefabs/resources used by your mission. Keep those resource references in the mission's normal configuration/resources. Do not copy made-up GUIDs into the F8 scripts.

## 7. Easy settings

For normal changes, edit only:

`Scripts/Game/GRS/GRS_VirtualPlayerConfig.c`

and

`Scripts/Game/GRS/GRS_VirtualPlayerEasyConfig.c`

Both files are intentionally commented for non-programmers.

## 8. Multiplayer/security

All purchases, inventory transfers, job completion, vehicle spawning, property purchases and construction must be server-authoritative. The client menu is a request interface, not the source of truth.

## 9. Test checklist

Test in this order:

1. Press F8 and verify the menu opens/closes.
2. Verify all four tabs appear.
3. Test storing and retrieving one real inventory item.
4. Test buying one virtual item.
5. Test buying and spawning one mission vehicle.
6. Start a job and verify each civilian step is required.
7. Buy a small property, then test Storage/Arsenal/Decoration/Fortification placement.
8. Add Storage objects until 15,000 capacity and verify the next one is rejected.
9. Buy 10 properties and verify property 11 is rejected.
10. Test from a second client and confirm clients cannot change another player's money, inventory, garage or property.
11. Reconnect and verify persistence if your mission persistence system is enabled.

### Important
The F8 framework, limits, validation model and UI contract are in the mod. Actual interaction with a specific mission's inventory components, civilian AI, vehicle prefab catalogue and construction prefabs is necessarily mission-specific; Workbench must supply those real resources rather than guessed engine identifiers.
