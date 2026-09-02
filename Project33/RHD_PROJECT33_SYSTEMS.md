# Project33 Systems

Project33 is intended to be the gameplay and persistence layer around the supplied Reforger mod stack, not a replacement for those mods.

## Contracts

`RHD_Contracts.c` defines reusable contract types and level requirements. `RHD_ContractRuntime.c` creates contract instances only when the mission can provide two distinct existing civilian characters through `RHD_VirtualPlayerMissionAdapter.c`.

Supported flow:

`AVAILABLE -> ACCEPTED -> SOURCE_CONTACT -> DELIVERY -> COMPLETED`

The runtime can also record a failure and apply a configurable reputation penalty. Completion consumes the required virtual item, awards the configured money reward, grants XP, and grants faction reputation.

No contract-specific civilian NPCs are spawned.

## Progression and reputation

`RHD_PlayerProgression.c` tracks:

- level and XP
- completed/failed contracts
- total contract earnings
- civilian reputation
- military reputation
- criminal reputation
- contractor reputation

`RHD_ProgressionConfig.c` contains the balancing knobs and level unlocks so a non-coder can tune the progression curve without changing gameplay code.

## Persistence

`RHD_Persistence.c` defines the Project33 snapshot shape and transaction boundary. `RHD_Project33PersistenceAdapter.c` is the mission-owned integration point for the supplied Enfusion Persistence Framework and Enfusion Database Framework.

The adapter is intentionally fail-safe until the exact installed persistence APIs are wired. No undocumented calls are fabricated.

## F8 integration

The existing F8 virtual-player screen now surfaces the player's progression/reputation summary and available contracts. Contract actions can be attached to mission UI widgets later without changing the underlying state model.

## Recommended next wiring

Workbench work should be limited to the mission adapter and, if desired, additional F8 buttons for `Accept`, `Contact`, `Deliver`, and `Save` operations. The underlying contract/progression state does not require new map actors.
