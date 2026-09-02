# Project33 / RHD Overthrow Companion Features

Project33 now has a single companion service layer covering the requested gameplay set.

## Phase 1

1. Contract board — contract data, publishing, accepting, completion and notifications.
2. Persistent money/inventory/reputation — per-player companion profiles and reputation/stat containers; persistence remains the mission adapter boundary.
3. Legal vs illegal market — configurable illegal item classification and black-market premium.
4. Player-to-player trading — trade offer records, acceptance and notifications; actual item/money transfer belongs in the authoritative mission/economy integration.
5. Vehicle ownership/garage — owned vehicle records, condition, storage, insurance and theft state.

## Phase 2

6. Dynamic town supply/demand — town demand and price multipliers with configurable bounds.
7. Reputation/faction standing — bounded per-faction reputation values.
8. Territory intelligence — town controller, threat, supply and contested/friendly flags.
9. Property benefits — companion benefit calculation hooks for storage/property progression.
10. Logistics missions — specialized contract publisher with a configurable reward bonus.

## Phase 3

11. Licenses — player license records and limits.
12. Insurance/loans — vehicle insurance records and simple fixed-interest loan records.
13. Black-market dealer network — dealer records, criminal reputation gates and configurable premium.
14. Player statistics — contracts, deliveries, trades, vehicles, finance and money statistics.
15. Advanced ATAK notifications — prioritized unread notifications per player profile.

## Easy tuning

`RHD_OverthrowCompanionConfig.c` contains all feature switches and the main balancing values. A non-coder can disable an entire feature without editing gameplay code.

## Runtime integration boundary

The service deliberately does not guess third-party APIs. The following still need the actual target mission/Workbench resources:

- Authoritative Overthrow money transactions.
- Enfusion Persistence Framework / Database Framework save and load calls.
- Real player inventory item transfer.
- Real civilian discovery and interaction.
- Real vehicle prefab/catalog and spawn locations.
- Real property placement/build resources.
- ATAK widget bindings and notification presentation.
- Atomic multiplayer trade authorization.

This separation keeps Project33 from breaking when a dependency changes its internal API.
