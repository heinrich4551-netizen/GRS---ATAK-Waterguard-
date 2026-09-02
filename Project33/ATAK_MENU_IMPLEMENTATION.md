# GRS ATAK F6 Menu

This branch adds the script-side foundation for an F6 ATAK menu.

## Current features

- F6 action listener named `GRS_ATAK_Menu`.
- Menu tabs: Shop, Casino, Map, Stats, Handler.
- Session economy state.
- $125 per enemy kill.
- $4,500 per enemy revive.
- $145 per base supply action.
- $1,786 precise mortar request charge.
- Shop price table and admin-only setter entry point.
- Casino payout helpers for blackjack, roulette, scratch cards and a daily-lotto cooldown state.
- Private map state model for pins, labels and layers.
- Handler request model for AI recruitment/loadout and support requests.

## Important integration work

The repository currently contains only the `Project33` game-project metadata and no existing gameplay/UI scripts to safely bind these systems to mission-specific inventory, AI, mortar, supply-crate or revive/base events. The new files therefore expose narrow integration hooks instead of inventing prefab/resource IDs or engine APIs.

### F6 binding

Create/bind the input action `GRS_ATAK_Menu` to keyboard F6 in the Workbench Input Configuration. The controller registers this action with `InputManager.AddActionListener`, which is the same listener pattern used by Reforger UI code.

### Server authority

Money, purchases, rewards, AI spawning, mortar calls and supply drops should be executed on the server. The menu must send requests to the server and only the server should call the economy mutators. Do not trust a client-provided reward, price, inventory item or target.

### Shop

`AdminSetShopItem(itemId, price)` is the admin-side entry point. A production integration should validate the caller's admin permission, persist the shop table for the session/server, validate the item resource, debit the buyer server-side, and then spawn/insert the purchased item into the player's inventory.

### Casino

The supplied helpers are deterministic payout calculators. The server must generate the actual random outcomes, debit the stake before resolving the game, and credit only server-calculated winnings. `MarkDailyLottoClaimed` should be set only after a successful daily-lotto transaction.

### Map

`GRS_ATAKMapState` is deliberately per-player. Do not replicate its edits to other clients if the map is intended to be private. The final UI should render the player's local state over the map widget.

### Handler

Use `GRS_ATAKHandlerRequest` to carry AI name/prefab/loadout and support targets. Recruitment must validate available squad slots and the selected loadout. Mortar requests must validate a legal target/weapon solution before charging or, preferably, reserve/debit the cost transactionally. Supply drops should spawn the requested marker and crate server-side and populate the crate from a server-side random enemy-equipment table.

### Rewards

Hook the existing mission's authoritative enemy-death, revive and base-supply events to:

- `RecordEnemyKill()`
- `RecordEnemyRevive()`
- `RecordBaseSupply()`

This prevents duplicate client-side reward claims.
