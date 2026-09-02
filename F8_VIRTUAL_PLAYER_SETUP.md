# GRS F8 Virtual Player Menu

## Overview

F8 opens the player's virtual management interface. It is independent of the F6 ATAK menu and F7 admin menu and does not require an ATAK item.

The system is divided into four persistent concepts:

1. **Virtual Inventory**
2. **Virtual Garage**
3. **Jobs**
4. **Property**

The script layer provides bounded state, UI/controller contracts and server integration hooks. Actual persistence and world spawning must be implemented by the authoritative mission/server layer.

## Virtual Inventory

### Base-game inventory

Players can store up to **120 base-game item entries** from their normal inventory.

The stored entry should contain a server-approved item/resource identifier, display name and quantity. The server must verify that the player actually owns the item before removing it from the physical inventory and placing it in virtual storage.

The operation should be transactional:

`physical inventory -> server validation -> virtual storage commit -> physical removal`

If the virtual storage commit fails, the physical item must not be lost.

### Virtual-only inventory

The system reserves **435 virtual item entries**. These are menu-only objects and should not be spawned as physical entities unless a specific mission integration says otherwise.

Examples suitable for this system include:

- Quest packages
- Civilian delivery documents
- Contracts
- Keys/licenses represented as data
- Property deeds
- Special delivery goods
- Economy tokens
- Mission-specific virtual resources

Virtual items should be identified by stable mission-owned IDs rather than fake engine prefab resources.

## Virtual Garage

The garage stores purchased vehicles independently of the player's current physical vehicle.

The mission should build its vehicle catalogue from the vehicles actually available in the active mission. Do **not** hard-code resource GUIDs from another project.

Recommended flow:

`Mission vehicle catalogue -> F8 Garage -> purchase -> server validates money -> persistent garage entry -> Spawn`

When spawning a stored vehicle, the server must:

1. Verify ownership.
2. Verify the vehicle is actually present in the mission catalogue.
3. Find a safe spawn location.
4. Prevent spawning inside another vehicle/player/building.
5. Create the authoritative entity.
6. Mark the garage entry as active/deployed if the mission wants one-active-instance behavior.

## Jobs

Jobs are generated from civilian AI that exists in the active mission.

### Simple delivery

A generated job can be:

`Civilian A -> player receives item -> Civilian B -> player delivers item -> reward`

The server should store:

- Job ID
- Source civilian ID
- Destination civilian ID
- Virtual item ID
- Reward
- Current stage
- Expiration, if used

### Multi-stage conversation

A more advanced job can be:

`Civilian A -> talk -> Civilian B -> talk -> Civilian C -> receive delivery instruction -> deliver virtual item -> reward`

The civilian actors should be selected from mission-owned civilian AI that can safely participate in the interaction. Do not create arbitrary AI from a client-provided resource path.

Dialogue is stateful. A player should not be able to skip stages simply by selecting the final NPC in the UI.

### Job anti-exploit rules

The server should verify:

- The player owns the active job.
- The correct civilian is being interacted with.
- The player is within an interaction distance.
- The required virtual item is present.
- The job stage is correct.
- The reward has not already been paid.

## Property

Players can own up to **10 properties**.

### Build radius

The maximum property build radius is **500 ft / 152.4 m**.

The property purchase price scales with the selected build radius and is capped at **$15,000,000 at 500 ft**.

The controller uses a radius-based calculation where 500 ft maps to the $15,000,000 ceiling. A production mission can replace the formula with a land-value curve if desired, while retaining the hard maximum.

### Property placement

A production server implementation must validate:

- Map bounds
- Terrain suitability
- Existing structures
- Protected mission areas
- No-build zones
- Overlap/collision
- Road/strategic restrictions if desired
- Ownership

### 500 ft expansion rule

The requested cluster behavior is represented as a server placement rule:

- A standalone property can use up to its purchased radius.
- A property may have a build range extending toward an owned neighboring property, up to **500 ft**.
- Owned properties forming a cluster can share a build area up to the **500 ft** maximum or until the expansion encounters another building/structure that the mission treats as a boundary.

The authoritative mission implementation must calculate the actual union of owned property zones and collision/building boundaries. The client must never decide this by itself.

## Property storage

Every storage object built at a property adds:

**+100 virtual storage capacity**

The total property storage capacity is capped at:

**15,000 virtual storage**

That means a maximum of 150 storage-object increments if every object contributes the full 100 capacity.

Storage is virtual. It does not require 15,000 physical inventory entities.

## Property construction categories

The property build menu should expose mission-whitelisted objects in these categories:

### Storage
- Storage container
- Warehouse storage
- Locker
- Mission-approved virtual-storage object

### Arsenal
- Arsenal object
- Ammunition storage
- Equipment station

### Decorations
- Furniture
- Lighting
- Signs
- Flags
- Landscaping/decorative objects

### Fortifications
- Barriers
- Sandbags
- Fences
- Defensive walls
- Gates
- Mission-approved defensive structures

Every physical construction must be validated and spawned by the server from a whitelist.

## Recommended persistent data

For each player persist:

```text
Player
├── Money
├── VirtualInventory
│   ├── BaseItems[]
│   └── VirtualItems[]
├── Garage[]
├── Jobs[]
└── Properties[]
    ├── Property ID
    ├── Position
    ├── Build Radius
    ├── Purchase Price
    ├── Storage Capacity
    ├── Storage Objects[]
    └── Structures[]
```

The exact persistence mechanism should be selected for the target mission/server environment. Do not rely on local client memory for persistent ownership.

## Workbench setup

### 1. Input

Create an input action named exactly:

`GRS_Virtual_Player_Menu`

Bind it to **F8**.

The script listens for the action rather than directly reading the physical key.

### 2. Menu preset

Create/register the `GRS_Virtual_Player_Menu` preset in the project's menu configuration using Workbench. The preset must point to `GRS_VirtualPlayerMenuUI`.

Do not hand-author fake resource GUIDs.

### 3. Minimum layout widgets

The layout must contain:

- `GRS_Virtual_Close` — `ButtonWidget`
- `GRS_Virtual_Money` — `TextWidget`
- `GRS_Virtual_Content` — `TextWidget`

Recommended production layout:

```text
GRS_Virtual_Root
├── GRS_Virtual_Header
│   ├── GRS_Virtual_Title
│   ├── GRS_Virtual_Money
│   └── GRS_Virtual_Close
├── GRS_Virtual_Tabs
│   ├── Inventory
│   ├── Garage
│   ├── Jobs
│   └── Property
└── GRS_Virtual_Content
```

For production, replace the summary text with real list/grid widgets and buttons wired to the controller operations.

### 4. Inventory UI

Provide separate views for:

- Physical/base-game stored items
- Virtual-only items
- Quantity
- Store
- Retrieve
- Delete/consume virtual item where permitted

Do not allow the UI to directly manipulate the player's physical inventory.

### 5. Garage UI

Provide:

- Mission vehicle catalogue
- Purchase price
- Ownership state
- Stored/deployed state
- Spawn button
- Store/return button where supported

The catalogue should be generated from the mission's server-approved vehicle list.

### 6. Jobs UI

Provide:

- Available job
- Current objective
- Current civilian
- Destination civilian
- Required virtual item
- Reward
- Current stage

Civilian interactions should happen through the mission's interaction system, not by allowing the player to complete a job by clicking a fake UI button from anywhere on the map.

### 7. Property UI

Provide:

- Buy property
- Radius selector
- Calculated price
- Placement preview
- Ownership list
- Build mode
- Storage capacity
- Storage objects
- Arsenal
- Decorations
- Fortifications

The server must re-check the price and placement when the player confirms the purchase/build action.

## Important Reforger limitations

This system cannot safely promise unrestricted persistence, arbitrary prefab spawning, arbitrary vehicle access, or arbitrary civilian manipulation without mission integration.

The script foundation therefore intentionally avoids inventing:

- Resource GUIDs
- Vehicle prefab paths
- AI prefab paths
- Persistence APIs
- Undocumented player inventory APIs
- Undocumented building-placement APIs

Those values must come from the actual mission and Workbench resources.

## Security model

All valuable state follows:

`F8 UI -> server request -> ownership/permission validation -> transaction -> replicated state`

Never trust:

- Client purchase prices
- Client vehicle IDs
- Client property radius
- Client property ownership
- Client job completion
- Client inventory quantities
- Client storage capacity
- Client build coordinates

The server should calculate all prices, capacity, ownership and placement rules itself.
