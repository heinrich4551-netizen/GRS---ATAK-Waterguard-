# GRS F7 Admin Menu

## Purpose

The mod now contains an F7 administrator menu foundation designed around Arma Reforger's authoritative multiplayer model. F7 is separate from the F6 ATAK/economy menu and does not require an ATAK item.

The menu is intentionally **fail-closed**. Opening the UI and executing an administrative action are both gated by `GRS_AdminController.IsAuthorizedAdmin()`. The base implementation returns `false`. A mission/server integration must override that method and validate the actual caller using the server's supported permission source.

Do not replace the authorization check with a client-controlled boolean, nickname, or UI state.

## Admin tools exposed by the controller

### Players
- Heal by amount
- Revive
- Kill
- Teleport yourself to a selected target
- Bring a target to the admin
- Teleport a target to coordinates
- Freeze/unfreeze
- Enable/disable target god mode
- Enable/disable target damage
- Refill target ammunition
- Repair target vehicle
- Flip target vehicle

### Moderation
- Warn
- Temporary mute
- Kick
- Ban
- Unban

Kick/ban/mute are request hooks. The mod does not pretend to implement a persistent moderation database or a server-platform ban service where no documented mission API is available.

### Teleport / world interaction
- Teleport admin to a position
- Teleport target to a position
- Teleport admin to target
- Bring target to admin
- Spawn a configured prefab at a position
- Delete a selected target/entity

The mission integration must validate terrain, safe zones, map bounds, collision and line-of-sight as appropriate.

### Gameplay / vehicle administration
- Heal
- Revive
- Kill
- Freeze
- God mode
- Damage enable/disable
- Refill ammunition
- Repair vehicle
- Flip vehicle
- Delete target

### World
- Set time
- Set weather
- Set fog

These are hooks because scenarios can use different world-state systems and replication policies.

### Spawn
- Spawn configured mission prefabs
- Spawn AI/vehicles/supplies through mission-owned prefab whitelists
- Delete selected entities

Do not accept arbitrary client-supplied resource IDs without a server-side whitelist.

### Economy
- Grant money
- Remove money
- Change shop prices
- Review the local admin action log

Economy changes must be performed on the authoritative server-side economy state, not by changing a client copy.

### Server
- Broadcast/announce a message
- End mission request
- Restart mission request
- Save request
- Clear AI request
- Clear vehicle request

Whether a particular server operation is available depends on the active game mode and supported engine/mission API. The hooks intentionally return `false` until implemented by the mission.

### Diagnostics
- Authorization state
- Current admin tab
- Selected player ID
- Bounded local action log
- Failure-safe action dispatch

## Workbench setup

### 1. Let Workbench import the scripts

Open the project in Arma Reforger Workbench and allow the resource database to finish indexing. The new files are under:

`Project33/Scripts/Game/GRS/`

New files:

- `GRS_AdminConfig.c`
- `GRS_AdminController.c`
- `GRS_AdminModel.c`
- `GRS_AdminMenuPreset.c`
- `GRS_AdminMenuUI.c`

`GRS_ATAKGameMode.c` now initializes both F6 and F7 controllers.

### 2. Create the F7 input action in Workbench

Create an input action named exactly:

`GRS_Admin_Menu`

Bind it to **F7** for keyboard/PC. Do not rely on the physical key in script; the script listens for the action name so the binding remains configurable.

The controller uses the standard Reforger input listener pattern:

`AddActionListener("GRS_Admin_Menu", EActionTrigger.DOWN, OnF7)`

### 3. Create the admin menu layout in Workbench

Create a layout resource for the `GRS_AdminMenuUI` class. Do not hand-write GUIDs or fake resource IDs.

At minimum the root layout must contain these named widgets:

- `GRS_Admin_Close` — `ButtonWidget`
- `GRS_Admin_Status` — `TextWidget`
- `GRS_Admin_Content` — `TextWidget`

Recommended production layout:

```text
GRS_Admin_Root
├── GRS_Admin_Header
│   ├── GRS_Admin_Title
│   ├── GRS_Admin_Status
│   └── GRS_Admin_Close
├── GRS_Admin_Tabs
│   ├── Players
│   ├── Moderation
│   ├── Teleport
│   ├── Gameplay
│   ├── Spawn
│   ├── World
│   ├── Economy
│   ├── Server
│   └── Diagnostics
└── GRS_Admin_Content
```

Buttons should call the controller's `Execute()` method with a `GRS_AdminActionRequest`. Keep target selection and text entry in the UI layer; never treat the UI itself as authoritative.

### 4. Register the menu preset

Add the `GRS_Admin_Menu` preset to the mod's `chimeraMenus.conf` using Workbench's menu configuration workflow.

Associate the preset with:

`GRS_AdminMenuUI`

and the layout resource created in step 3.

Do not copy a GUID from another mod. Workbench must create/register the resource metadata for this project.

### 5. Add the script to the game project

The existing `addon.gproj` already includes the project's script tree. After saving the Workbench resources, confirm that the script resource is indexed without errors.

### 6. Add real admin authorization

The base implementation is intentionally:

```c
bool IsAuthorizedAdmin()
{
    return false;
}
```

Create a mission/server-side `modded class GRS_AdminController` implementation that uses the server's actual authorization mechanism.

Good authorization sources include a server-owned permission manager, an existing mission admin list, or another server-authoritative permission system already used by the mission.

Do **not** authorize based on:

- A client checkbox
- A value sent by the UI
- A player nickname alone
- A local-only boolean
- An ATAK inventory item
- Whether F6 is open

### 7. Implement the action hooks in the mission

Override only the hooks your mission can safely implement. For example:

```c
modded class GRS_AdminController
{
    override bool IsAuthorizedAdmin()
    {
        // Query the mission's real server-side admin permission source.
        return false;
    }

    override protected bool ServerHeal(string target, int amount)
    {
        // Resolve the target using the mission's player/entity manager,
        // validate the target, then perform the authoritative health change.
        return false;
    }
};
```

Use the same pattern for teleport, spawning, moderation, economy and world-state actions.

### 8. Keep dangerous operations server-authoritative

For every destructive action:

`UI -> request -> server authorization -> server validation -> server action -> replicated result`

Never use:

`UI -> client changes entity -> hope replication accepts it`

The controller's base hooks return `false` so a missing mission integration cannot silently turn an admin button into a fake local action.

### 9. Whitelist spawnable assets

Create a mission-owned list of allowed prefab/resource identifiers. The UI should select from that list rather than accepting arbitrary resource paths from a player.

This protects against accidental spawning of invalid assets and prevents the admin UI from becoming an unrestricted resource loader.

### 10. Test the menu

Test in this order:

1. Start a local Workbench game with the mod enabled.
2. Verify normal players cannot open F7 because authorization is fail-closed.
3. Enable the mission's real admin permission for your test account.
4. Press F7.
5. Verify the admin menu opens.
6. Verify the status reads `ADMIN AUTHORIZED | SERVER TOOLS ENABLED`.
7. Test a harmless announcement/action first.
8. Test player targeting with a second client.
9. Verify the action is performed by the server and replicated to the other client.
10. Test an unauthorized client attempting the same action.
11. Confirm the unauthorized action is rejected server-side.
12. Test reconnecting the admin and confirm authorization is recalculated rather than restored from stale UI state.

## What this menu does not claim to bypass

Arma Reforger's server/platform administration features remain authoritative. A gameplay mod cannot safely manufacture platform-level permissions, persistent bans, server process control, or undocumented engine functionality.

The F7 menu therefore provides the admin command surface and safe integration points. The actual mission/server implementation must connect those hooks to APIs that are available in the target Reforger version.

## F6 relationship

F6 and F7 are independent:

- **F6** = player ATAK/economy menu
- **F7** = administrator menu

F6 does not require an ATAK item, and F7 does not require an ATAK item.
