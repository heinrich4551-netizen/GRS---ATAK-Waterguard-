# GRS ATAK / Waterguard F6 Menu

## Reforger-safe implementation status

The branch now contains the Enforce Script side of the feature plus the custom menu preset and menu class. The repository still does **not** contain mission-specific gameplay resources, an existing custom `chimeraMenus.conf`, inventory integration, AI squad integration, mortar support implementation, or supply-crate/smoke prefabs. Those cannot be safely fabricated: Reforger resource references are project assets and engine APIs must match the mission's actual components.

The scripts therefore use narrow integration hooks and keep the parts that can be implemented generically inside normal Reforger scripting limits.

## Menu

Files:

- `Scripts/Game/GRS/GRS_ATAKMenuPreset.c` registers `GRS_ATAK_Menu` with `ChimeraMenuPreset`.
- `Scripts/Game/GRS/GRS_ATAKMenuUI.c` is the `ChimeraMenuBase` UI controller.
- `Scripts/Game/GRS/GRS_ATAKMenuController.c` handles the F6 action and feature/service state.
- `Scripts/Game/GRS/GRS_ATAKMenuModel.c` contains the tab/action names.

### Workbench-only resource setup

Two editor resources are intentionally not generated as hand-written fake files:

1. Create `UI/Layouts/GRS/GRS_ATAK_Menu.layout` in Resource Manager. Give the root/child widgets the names expected by `GRS_ATAKMenuUI.c`, at minimum:
   - `GRS_ATK_Close`
   - `GRS_ATK_Money`
   - `GRS_ATK_Content`
2. Override the base game's `Configs/System/chimeraMenus.conf` in the mod and add an entry named exactly `GRS_ATAK_Menu` pointing to the new layout and class `GRS_ATAKMenuUI`.

Reforger's documented custom-menu workflow is to add a `modded enum ChimeraMenuPreset : ScriptMenuPresetEnum` entry and then add the matching object-name entry to the mod's `chimeraMenus.conf`. The enum and script are present here; the project-specific layout/config resource must be authored by Workbench so it receives valid resource metadata.

### F6 action

Create/bind the input action `GRS_ATAK_Menu` to keyboard F6 in the mission/mod input configuration. The script registers the action with `InputManager.AddActionListener` using the same listener pattern used by Reforger UI code.

## Economy

Requested rewards/costs are centralized in `GRS_ATAKConfig.c`:

- Enemy kill: **$125**
- Enemy revive: **$4,500**
- Base supply: **$145**
- Precise mortar request: **$1,786**
- Supply drop: **$1,000**
- Blackjack minimum: **$500**
- Roulette minimum: **$250**
- Scratch card: **$250**
- Daily lotto: **$100**

`GRS_ATAKPlayerState` owns money and current-session counters. Spending supports rollback if an integration operation fails.

## Server authority requirement

All shared/economic operations must ultimately run on the authoritative server. Do not let a client submit a payout, reward amount, shop price, inventory resource, or casino outcome.

The current controller exposes the game rules and integration hooks, but the repository has no player/economy network component to attach to the mission's player entities. That is the remaining mission integration step; adding guessed RPC/component wiring would be less safe than leaving the explicit seam.

## Shop

`AdminSetShopItem(itemId, price, callerIsAdmin)` requires a server-side permission result. The real integration must additionally validate the item resource, debit the buyer on the server, insert/spawn the item through the mission's inventory API, and roll back the debit if insertion fails.

## Casino

Blackjack, roulette and scratch cards now generate their outcomes inside the service rather than accepting an outcome from a client. Daily lotto accepts a server-provided day key so the mission can use its authoritative date/session system. The server should own the ticket/result transaction.

## Map

`GRS_ATAKMapState` is client-local by design. Pins, labels and layers are not replicated. The layout/UI should draw these values over the player's map. This satisfies the requirement that only the user sees their edits.

## Handler

`GRS_ATAKHandlerRequest` carries the selected AI prefab/name/loadout and support target. The controller caps recruited AI at four by default. The real mission integration must validate squad capacity and loadout resources before spawning.

- Mortar: validate target and available fire-support solution, then charge **$1,786** transactionally.
- Supply drop: spawn the crate and red-smoke marker server-side, populate it from a server-owned enemy-equipment table, and charge **$1,000** transactionally.

No fake prefab IDs are included.

## Rewards integration

Connect the authoritative mission events to:

- `RecordEnemyKill()`
- `RecordEnemyRevive()`
- `RecordBaseSupply()`

Pass the correct player state/controller for the player responsible for the event. Do not invoke these from client UI code.

## Compatibility review

The added code uses normal Enforce Script classes under `Project33/Scripts/Game/GRS`, `SCR_BaseGameMode` lifecycle hooks, `InputManager` action listeners, `ChimeraMenuBase`, `ChimeraMenuPreset`, standard containers, and `Math.RandomInt`. It does not add native code, external libraries, filesystem/network access, arbitrary executable code, or invented engine classes.

The original project metadata files remain unchanged. This is intentional: `addon.gproj` already defines the game-project identity/dependencies, while the missing gameplay/UI assets must be created through Workbench rather than by guessing serialized resource data.
