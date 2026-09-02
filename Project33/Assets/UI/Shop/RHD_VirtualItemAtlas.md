# RHD Shop Item Visuals

The PNG atlas contains one visual tile for every production item in this order, left-to-right then top-to-bottom, six columns per row:

APPLE
CANNABIS_PLANT
COCA_LEAF
CORN_COB
GRAPES
PEACHES
IRON_ORE
COPPER_ORE
GOLD_ORE
DIAMOND
OIL_SAND
IRON
COPPER
GOLD
OIL
CANNABIS_FLOWER
COCAINE
CANNED_CORN

Atlas: `RHD_VirtualItemAtlas.png`

Workbench can use the atlas as the shop visual source, with the tile coordinates above used when constructing the item-card UI. For final production UI, import the PNG through Workbench's texture/resource pipeline rather than referencing a guessed resource GUID in script.
