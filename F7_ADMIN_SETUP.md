# RHD F7 Admin Menu

F7 is the server/admin menu. It is fail-closed until the mission supplies real server-side authorization.

## Input
Create/bind:

`RHD_Admin_Menu`

Bind to **F7**.

## Menu
Register `RHD_Admin_Menu` in the mod Chimera menu configuration and connect it to `RHD_AdminMenuUI`.

Required widgets:
- `RHD_Admin_Close`
- `RHD_Admin_Status`
- `RHD_Admin_Content`

## Admin controller
The runtime class is `RHD_AdminController`. `IsAuthorizedAdmin()` returns false by default. A mission integration must override it with the real server permission system.

Do not authorize from client UI state, nickname alone, local booleans or an inventory item.

## Available command groups
Players, Moderation, Teleport, Gameplay, Spawn, World, Economy, Server and Diagnostics.

Economy includes grant/remove money and shop-price administration. Shop price changes must use the shared `RHD_Shop` service.

## Mission integration
Override only the server hooks supported by the actual mission and Reforger version. Keep destructive actions server-authoritative and validate targets/resources on the server.

## F6 relationship
- **F6** = RHD ATAK/economy menu
- **F7** = RHD administrator menu
- **F8** = RHD virtual player menu

The script prefix is now **RHD**. Existing physical `.c` filenames/directories remain named `GRS` until they are renamed together with Workbench resource references; this avoids breaking existing resource paths during the code-prefix migration.
