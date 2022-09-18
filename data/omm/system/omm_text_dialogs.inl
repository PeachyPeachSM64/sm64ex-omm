#ifdef VSCODE
#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#endif

static struct OmmDialogEntryRaw { s32 id; u32 soundBits; s32 linesPerBox; s32 leftOffset; s32 downOffset; const char *str; } sOmmDialogEntriesRaw[] = {

{
OMM_DIALOG_PEACHY_ROOM, 0, 3, 95, 200,
"Many hear about me, but\n"
"little gets to see me.\n"
"\n"

"To find me, gather\n"
"enough power and\n"
"return to this place..."
},

{
OMM_DIALOG_LEVEL_VARIANTS, 0, 3, 95, 200,
"Thanks to the power of\n"
"the 120 Stars, you can\n"
"access new levels.\n"

"Leave a water level to\n"
"explore the depths of\n"
"Jolly Roger Bay.\n"

"Exit a snow level to\n"
"freeze the water of\n"
"the Secret Aquarium.\n"

"Go out of a dark level\n"
"to enter a shadowy\n"
"Bowser In The Sky..."
},

{
OMM_DIALOG_BOWSER_1_INTRO, SOUND_OBJ_BOWSER_LAUGH | 0xFF00, 4, 30, 200,
"Tough luck, Mario...\n"
"and Cappy!\n"
"Your dear Princess isn't\n"
"here... Bwah ha ha ha!\n"

"What will you do now?\n"
"Go around me and try\n"
"to grab me by the tail?\n"
"Go ahead!\n"

"I'm not going to fall\n"
"for it again! You'll\n"
"have to do better than\n"
"that to defeat me!\n"

"Brace yourselves...\n"
"Here come my most\n"
"explosive attacks!\n"
"Gwa ah ah!"
},

{
OMM_DIALOG_BOWSER_1_HINT_1, 0, 4, 30, 200,
"Mario! It's me, Cappy!\n"
"Bowser is stronger than\n"
"before... Don't try to\n"
"attack him directly!\n"

"Wait for him to throw\n"
"one of his bombs...\n"
"I should be able to\n"
"capture it! Let's try!\n"

"To throw me upwards,\n"
"hold D^ and press X,\n"
"or press X and [A]."
},

{
OMM_DIALOG_BOWSER_1_HINT_2, 0, 3, 30, 200,
"Mario! It's me again!\n"
"Take a look at Bowser.\n"
"He seems angry...\n"

"Is that because we...\n"
"captured one of his\n"
"flaming bob-ombs?\n"

"That means that's his\n"
"weakness! As a bob-omb,\n"
"try to avoid his fire,\n"

"and approach him only\n"
"when he is the most\n"
"vulnerable.\n"

"Once you're close enough,\n"
"press [B] to make him\n"
"blast off!"
},

{
OMM_DIALOG_BOWSER_1_HINT_3, SOUND_OBJ2_BOWSER_ROAR | 0xFF00, 3, 30, 200,
"I'm done... I can't move\n"
"anymore! You, go away!\n"
"Leave me alone!\n"

"And don't try to grab\n"
"me by the tail!"
},

{
OMM_DIALOG_BOWSER_2_INTRO, SOUND_OBJ_BOWSER_LAUGH | 0xFF00, 4, 30, 200,
"Pestering me again, are\n"
"you, Mario and Cappy?\n"
"Can't you see that I'm\n"
"busy making mischief?\n"

"You got me once, so\n"
"don't expect mercy\n"
"from me, this time!\n"
"\n"

"I've been looking for\n"
"something to fry with\n"
"my fire breath!\n"
"\n"

"Will you have enough\n"
"guts to jump above it?\n"
"I'd like to see you try!\n"
"Ha ha!\n"

"Now, enough talking!\n"
"Be ready... to lose!\n"
"Bwah ha ha ha!"
},

{
OMM_DIALOG_BOWSER_3_INTRO, SOUND_OBJ_BOWSER_LAUGH | 0xFF00, 4, 30, 200,
"Mario! Cappy! You again!\n"
"Don't you ever give up?\n"
"Well... That's fine.\n"
"That's too bad for you.\n"

"Your friends are still\n"
"trapped within the walls.\n"
"And you will never see\n"
"your Princess again.\n"

"Your Stars are useless\n"
"against me! I'll show\n"
"you my true strength!\n"
"Here, and now!\n"

"Bow before your master,\n"
"the great Koopa King,\n"
"you wimps!\n"
"Bwah ha ha ha!"
},

{
OMM_DIALOG_BOWSER_3_DEFEAT, 0, 4, 30, 200,
"Mario! Cappy! You have\n"
"really beaten me this\n"
"time! Arrgh! I can't\n"
"stand losing to you!\n"

"My troops... worthless!\n"
"I gave them power, and\n"
"they couldn't even slow\n"
"you down!\n"

"Graaah! I can see peace\n"
"returning to the world!\n"
"I really hate that!\n"
"I can't watch...\n"

"But don't be so happy.\n"
"You had better luck.\n"
"Next time we meet, I'll\n"
"make you bite the dust!\n"

"Until then, keep that\n"
"cap of yours spinnin'!\n"
"Bwaa ha ha!"
},

{
OMM_DIALOG_SPARKLY_STAR_NAMES, 0, OMM_DIALOG_SPARKLY_MULTI(1, 1, 1),
"BOB-OMB BATTLEFIELD$PRECISE CANNON SHOTS\n"
"WHOMP'S FORTRESS$SHINY GOLDEN COINS\n"
"JOLLY ROGER BAY$TREASURE OF THE FLOODED BAY\n"
"COOL, COOL MOUNTAIN$SUSPICIOUS SLIDE SHORTCUT\n"
"BIG BOO'S HAUNT$BLUE COINS HUNT\n"
"HAZY MAZE CAVE$CLIMBING IN THE CAVE\n"
"LETHAL LAVA LAND$IMMINENT VOLCANIC ERUPTION\n"
"SHIFTING SAND LAND$SHINING ABOVE THE PYRAMID\n"
"DIRE, DIRE DOCKS$IN FRONT OF BOWSER'S SUB\n"
"SNOWMAN'S LAND$A SNOWMAN ON THE BIG HEAD\n"
"WET-DRY WORLD$CANNON SHOT TO THE SKY\n"
"TALL, TALL MOUNTAIN$STAR OF THE DEADLY SLIDE\n"
"TINY-HUGE ISLAND$TEN TINY GOOMBAS\n"
"TICK TOCK CLOCK$NO TIME TO WASTE\n"
"RAINBOW RIDE$SECRETS IN THE SKY\n"
"BOWSER IN THE DARK WORLD$ODYSSEY OF A GOOMBA\n"
"BOWSER IN THE FIRE SEA$MUSHROOMS IN THE FIRE SEA\n"
"BOWSER IN THE SKY$THE FINAL TASK\n"
"TOWER OF THE WING CAP$FREE FLIGHT FOR COINS\n"
"VANISH CAP UNDER THE MOAT$MASTERED AIR CONTROL\n"
"CAVERN OF THE METAL CAP$WALL-JUMP MADNESS\n"
"THE PRINCESS'S SECRET SLIDE$NOTHING BUT SLIDING\n"
"WING MARIO OVER THE RAINBOW$JUST MARIO OVER THE RAINBOW\n"
"THE SECRET AQUARIUM$FROZEN REDS IN COLD WATER\n"
"CASTLE GROUNDS$ABOVE PEACH'S CASTLE\n"
"CASTLE INSIDE$INSIDE PEACH'S CASTLE\n"
"CASTLE BASEMENT$TOADS DRAINING THE MOAT\n"
"CASTLE UPSTAIRS$AT THE PINNACLE OF TIME\n"
"CASTLE COURTYARD$COURTYARD CHEST TRAP?\n"
"PINK-GOLD BOWSER$PINK-GOLD BOWSER\n"
OMM_DIALOG_SPARKLY_MULTI_END

"BOB-OMB BATTLEFIELD$MARIO WINGLESS TO THE SKY\n"
"WHOMP'S FORTRESS$HEIST OF WHOMP'S VAULT\n"
"JOLLY ROGER BAY$LOST IN THE DARK DEPTHS\n"
"COOL, COOL MOUNTAIN$APEX OF THE MOUNTAIN\n"
"BIG BOO'S HAUNT$TIMED BLUE COINS HUNT\n"
"HAZY MAZE CAVE$FIREFIGHTING IN THE CAVE\n"
"LETHAL LAVA LAND$VOLCANIC DISASTER\n"
"SHIFTING SAND LAND$SEALED INSIDE THE PYRAMID\n"
"DIRE, DIRE DOCKS$POLE-JUMPING ABOVE THE DOCKS\n"
"SNOWMAN'S LAND$SHELL-RIDING EXPERIENCE\n"
"WET-DRY WORLD$CHUCKYA MAGIC TRICK\n"
"TALL, TALL MOUNTAIN$BLIND JUMP INTO THE VOID\n"
"TINY-HUGE ISLAND$HUGE TOWER OF GOOMBAS\n"
"TICK TOCK CLOCK$RACE AGAINST THE CLOCK\n"
"RAINBOW RIDE$MUSHROOMS IN THE SKY\n"
"BOWSER IN THE DARK WORLD$GOOMBA EMERGENCY\n"
"BOWSER IN THE FIRE SEA$THROUGH THE FIRE AND FLAMES\n"
"BOWSER IN THE SKY$THE GRAND FINALE\n"
"TOWER OF THE WING CAP$FAST FLIGHT FOR COINS\n"
"VANISH CAP UNDER THE MOAT$PERFECT AIR CONTROL\n"
"CAVERN OF THE METAL CAP$WALL-JUMP INSANITY\n"
"THE PRINCESS'S SECRET SLIDE$NEVER-ENDING SLIDE\n"
"WING MARIO OVER THE RAINBOW$ONLY MARIO OVER THE RAINBOW\n"
"THE SECRET AQUARIUM$FROZEN COINS IN COLD WATER\n"
"CASTLE GROUNDS$TOP OF PEACH'S CASTLE\n"
"CASTLE INSIDE$AVOIDING BOWSER'S TRAP\n"
"CASTLE BASEMENT$MIPS THE YELLOW RABBIT\n"
"CASTLE UPSTAIRS$SECRET OF THE MIRROR ROOM\n"
"CASTLE COURTYARD$TOTALLY CLASSIC?\n"
"CRYSTAL BOWSER$CRYSTAL BOWSER\n"
OMM_DIALOG_SPARKLY_MULTI_END

"\n"
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_1, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 3),
"Welcome to the Pink-Gold\n"
"Star Hunt! There is one\n"
"in each level of the game.\n"

"Usually, Pink-Gold Star\n"
"challenges follow specific\n"
"rules.\n"

"If you break these rules,\n"
"you won't be able to\n"
"collect the Star.\n"

"If that happens, just\n"
"restart the level to\n"
"make it available again.\n"

"Here's the challenge of\n"
"the battlefield's Pink-\n"
"Gold Star.\n"

"Five secrets in five rings\n"
"in the sky, but you cannot\n"
"use the Wing cap to fly.\n"

"If a secret is too high,\n"
"a Bob-omb buddy's cannon\n"
"could be your best ally."
OMM_DIALOG_SPARKLY_MULTI_END

"Welcome to the Crystal\n"
"Star Hunt! There is one\n"
"in each level of the game.\n"

"Usually, Crystal Star\n"
"challenges...\n"
"\n"

"You already know that,\n"
"right? Then, let's get\n"
"to the point.\n"

"Objective: Five secrets.\n"
"Rules: No cap power-up,\n"
"no cannon, no cheat.\n"

"Difficulty: Definitely\n"
"harder than Pink-Gold\n"
"Stars."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_2, 0, OMM_DIALOG_SPARKLY_MULTI(4, 4, 0),
"Hidden somewhere in the\n"
"fortress, the Star will\n"
"show up once 90 gold\n"
"coins are collected."
OMM_DIALOG_SPARKLY_MULTI_END

"Kept in the fortress\n"
"vault, the Star will\n"
"show up once 100 gold\n"
"coins are collected.\n"

"You'll have to be fast,\n"
"the vault stays open\n"
"only for 1 minute and\n"
"45 seconds!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_3, 0, OMM_DIALOG_SPARKLY_MULTI(4, 3, 3),
"Welcome to the depths\n"
"of Jolly Roger Bay.\n"
"You can no longer return\n"
"to the surface.\n"

"Try to collect at least\n"
"101 coins, because that\n"
"will be your only way\n"
"to not drown here."
OMM_DIALOG_SPARKLY_MULTI_END

"Welcome to the very bottom\n"
"of the bay. Here, you can\n"
"hardly see around you.\n"

"Don't be afraid of the\n"
"dark. The more you panic,\n"
"the faster you'll drown.\n"

"Collect as many coins as\n"
"you can, they'll eventually\n"
"bring the light back."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_4, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"To reach the Star,\n"
"climb the walls of ice\n"
"of the forbidden path."
OMM_DIALOG_SPARKLY_MULTI_END

"To reach the Star,\n"
"harness the power\n"
"of the Flower Head."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_5, 0, OMM_DIALOG_SPARKLY_MULTI(2, 2, 0),
"Mario is red\n"
"Coins are blue\n"

"Get 20 of them\n"
"from Mr. I and Boo"
OMM_DIALOG_SPARKLY_MULTI_END

"Collect all blue coins,\n"
"and only blue ones...\n"

"...under 2 minutes and\n"
"10 seconds. Hurry up!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_6, 0, OMM_DIALOG_SPARKLY_MULTI(2, 4, 0),
"The Star is sparkling\n"
"above the largest room.\n"

"Use your cap abilities\n"
"to get high enough."
OMM_DIALOG_SPARKLY_MULTI_END

"The cave is burning,\n"
"and the fire is wild!\n"
"Put the flames out\n"
"before it's too late!\n"

"But there's no water\n"
"nearby... Time to think\n"
"fast, you only have 2\n"
"minutes and a half left!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_7, 0, OMM_DIALOG_SPARKLY_MULTI(2, 2, 0),
"The Pink-Gold Star's been\n"
"shattered into 8 pieces.\n"

"Gathering them all will\n"
"restore the Star, but...\n"

"...watch out for the lava\n"
"under your feet."
OMM_DIALOG_SPARKLY_MULTI_END

"The Crystal Star has been\n"
"shattered into 8 pieces.\n"

"Gathering them all will\n"
"restore the Star, but..."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_8, 0, OMM_DIALOG_SPARKLY_MULTI(2, 3, 0),
"The Pink-Gold Star shines\n"
"way above the pyramid.\n"

"But you won't get anywhere\n"
"without something bouncy."
OMM_DIALOG_SPARKLY_MULTI_END

"Collect all 62 coins of\n"
"the pyramid, then head\n"
"to the top.\n"

"The hourglass is almost\n"
"empty, and it can't be\n"
"filled with sand..."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_9, 0, OMM_DIALOG_SPARKLY_MULTI(2, 2, 0),
"This Pink-Gold Star is\n"
"looking at Bowser's sub.\n"

"Reach it with a well-\n"
"timed wall jump."
OMM_DIALOG_SPARKLY_MULTI_END

"This Crystal Star lies\n"
"way above the docks.\n"

"Reach it with well-\n"
"timed pole jumps."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_10, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Can you reach the top\n"
"of the big snowman...\n"
"as a snowman?"
OMM_DIALOG_SPARKLY_MULTI_END

"Want something cooler?\n"
"What about a shell-ride\n"
"around Snowman's Land?\n"

"Collect at least 85\n"
"coins while riding\n"
"a Koopa shell.\n"

"There is no time limit,\n"
"so no need to hurry.\n"
"Just enjoy the view."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_11, 0, OMM_DIALOG_SPARKLY_MULTI(1, 2, 0),
"Ready...\n"

"Aim,\n"

"Fire!"
OMM_DIALOG_SPARKLY_MULTI_END

"Bring a Chuckya to where\n"
"the fifth star shines."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_12, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Where the path ends,\n"
"don't stop and jump\n"
"to reach new heights."
OMM_DIALOG_SPARKLY_MULTI_END

"Even if the path makes a\n"
"turn, keep going forward\n"
"and jump into the void."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_13, 0, OMM_DIALOG_SPARKLY_MULTI(4, 3, 0),
"Little Goombas are fast\n"
"for sure. If you manage\n"
"to catch all 10, a Pink-\n"
"Gold Star could appear."
OMM_DIALOG_SPARKLY_MULTI_END

"To catch sight of the Star,\n"
"you must be at least as\n"
"tall as ten giant Goombas."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_14, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"How about playing a\n"
"timed game when you're\n"
"stuck inside a clock?\n"

"Be fast enough to open\n"
"all 13 yellow boxes in\n"
"less than 60 seconds."
OMM_DIALOG_SPARKLY_MULTI_END

"It's the same clock again.\n"
"But this time, you don't\n"
"have to break some boxes.\n"

"The Star is at the very\n"
"top, but you only have\n"
"35 seconds to reach it.\n"

"Also, you cannot land\n"
"after leaving the floor.\n"
"Super Mario Maker style."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_15, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Climb the 6 poles from\n"
"bottom to top to make\n"
"your way to the Star."
OMM_DIALOG_SPARKLY_MULTI_END

"This level has nine 1-up\n"
"mushrooms, scattered\n"
"across the entire level.\n"

"You don't have too much\n"
"time. Give it your all to\n"
"get a hand on that Star!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_16, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 4),
"You are now a Goomba.\n"
"Gather all red coins to\n"
"summon a Pink-Gold Star.\n"

"Don't leave the blue area\n"
"or grab a coin as Mario."
OMM_DIALOG_SPARKLY_MULTI_END

"You are now a Goomba.\n"
"Six impostors stole\n"
"74 coins from Bowser.\n"

"Take the money back\n"
"and catch the culprits,\n"
"disguised as Goombas.\n"

"You only have 1 minute\n"
"and 40 seconds before\n"
"getting roasted. Go!\n"

"(Remember: You can't\n"
"leave the blue area or\n"
"grab a coin as Mario.)"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_17, 0, OMM_DIALOG_SPARKLY_MULTI(3, 4, 0),
"This level has 7 1-up\n"
"mushrooms. Some are\n"
"visible, some are not.\n"

"Find them all, but you\n"
"can't collect them as\n"
"Mario."
OMM_DIALOG_SPARKLY_MULTI_END

"Six secrets are spread\n"
"across the fire sea:\n"
"four on corners and\n"
"two between pillars.\n"

"During this challenge,\n"
"Mario cannot have more\n"
"than 3 Health (4 with\n"
"Classic Moveset).\n"

"Collect the secrets,\n"
"survive the trials,\n"
"and the Crystal Star\n"
"will appear to you.\n"

"Play fair. If you try\n"
"to take shortcuts, it\n"
"will end badly for you..."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_18, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"The end is near. Collect\n"
"at least 75 coins to\n"
"reveal the last Star.\n"

"You have only 1 minute,\n"
"but each coin collected\n"
"increases the timer."
OMM_DIALOG_SPARKLY_MULTI_END

"Time for a big firework!\n"
"Turn into a walking bomb\n"
"and blow up your enemies!\n"

"Defeat 15 of them as\n"
"a Bob-omb to reveal\n"
"the Crystal Star.\n"

"Don't leave your capture\n"
"until you're back at the\n"
"starting area."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_19, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"----- CAP RACE 1 -----\n"
"------ Wing Cap ------\n"
"----- 60 Seconds -----\n"

"Use both Cappy and the\n"
"Improved Wing cap to\n"
"collect every coin."
OMM_DIALOG_SPARKLY_MULTI_END

"----- CAP RACE 1 -----\n"
"------ Wing Cap ------\n"
"----- 35 Seconds -----\n"

"Use both Cappy and the\n"
"Improved Wing cap to\n"
"collect every coin.\n"

"Don't miss a single one,\n"
"because you'll need some\n"
"time to reach the Star."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_20, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"----- CAP RACE 2 -----\n"
"----- Vanish Cap ------\n"
"----- 50 Seconds -----\n"

"The Improved Vanish cap\n"
"reduces Mario's gravity\n"
"and terminal velocity.\n"

"Use this ability to reach\n"
"every corner of the map\n"
"and make the Star appear."
OMM_DIALOG_SPARKLY_MULTI_END

"----- CAP RACE 2 -----\n"
"----- Vanish Cap ------\n"
"----- 40 Seconds -----\n"

"The Improved Vanish cap\n"
"reduces Mario's gravity\n"
"and terminal velocity.\n"

"The timer is pretty tight,\n"
"so make sure to optimize\n"
"your movement."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_21, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"----- CAP RACE 3 -----\n"
"------ Metal Cap ------\n"
"----- 40 Seconds -----\n"

"Make your way through\n"
"the cavern by jumping\n"
"from a wall to another.\n"

"Reach the Star in front\n"
"of the waterfall without\n"
"touching the floor once."
OMM_DIALOG_SPARKLY_MULTI_END

"----- CAP RACE 3 -----\n"
"------ Metal Cap ------\n"
"----- 30 Seconds -----\n"

"Make your way through\n"
"the cavern by jumping\n"
"from a wall to another.\n"

"Reach the Star at the\n"
"top of the waterfall\n"
"without landing once."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_22, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 3),
"Are you good at sliding?\n"
"Let's find out!\n"
"\n"

"50 coins, in 24 seconds,\n"
"without a single button\n"
"press.\n"

"Beat this time, even by\n"
"a millisecond, and the\n"
"Pink-Gold Star is yours."
OMM_DIALOG_SPARKLY_MULTI_END

"Are you really good at\n"
"sliding? Let's find out!\n"
"\n"

"50 coins, in 22 seconds,\n"
"without a single button\n"
"press.\n"

"Beat this time, even by\n"
"a millisecond, and the\n"
"Crystal Star is yours."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_23, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"True test of skill.\n"
"Collect all 8 red coins\n"
"without the Wing cap."
OMM_DIALOG_SPARKLY_MULTI_END

"Truer test of skill.\n"
"All coins, no cannon,\n"
"no Wing cap."
OMM_DIALOG_SPARKLY_MULTI_END

"Truest test of skill.\n"
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_24, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Cold water drains Mario's\n"
"health quickly, so don't\n"
"stay in for too long.\n"

"Collect the 8 red coins\n"
"and survive until the\n"
"Pink-Gold Star appear."
OMM_DIALOG_SPARKLY_MULTI_END

"You need all yellow and\n"
"red coins to make the\n"
"Crystal Star spawn.\n"

"But the water is colder\n"
"than before... This time,\n"
"you can't use Cappy.\n"

"Don't waste any second,\n"
"if you don't want to get\n"
"frozen to death."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_25, 0, OMM_DIALOG_SPARKLY_MULTI(4, 4, 4),
"Look up.\n"
"\n"
"\n"
"\n"

"Do you see it?\n"
"That sparkly object, far\n"
"above the Castle tower?\n"
"It's a Pink-Gold Star.\n"

"You know the Princess\n"
"is missing, right?\n"
"Even the Grand Star\n"
"couldn't free her...\n"

"That's because Bowser\n"
"stole them from her,\n"
"and used them to make\n"
"mischief again...\n"

"But thanks to you, the\n"
"Pink-Gold Star Block is\n"
"broken, and its power\n"
"has been released!\n"

"From now on, Pink-Gold\n"
"Stars will appear in\n"
"every world, for a\n"
"total of 30 Stars.\n"

"But because Bowser's\n"
"magic is still there,\n"
"you can't collect\n"
"them directly...\n"

"You'll have to surpass\n"
"yourself to be able to\n"
"reach them or summon them\n"
"by completing challenges.\n"

"You can rely only on\n"
"yourself. Cap power-ups\n"
"are forbidden, as well\n"
"as cheats.\n"

"But cannons and captures\n"
"are ok. Toads and other\n"
"beings can help you, so\n"
"feel free to use them.\n"

"Anywhere in the Castle,\n"
"you can see which Stars\n"
"you already collected\n"
"by pausing the game."
OMM_DIALOG_SPARKLY_MULTI_END

"Look up.\n"
"\n"
"\n"
"\n"

"Do you see it?\n"
"That sparkly object, far\n"
"above the Castle tower?\n"
"It's a Crystal Star.\n"

"But the Princess is\n"
"already safe, right?\n"
"So why is it there,\n"
"waiting for you?\n"

"That's because Bowser\n"
"stole them from her,\n"
"and used them to make\n"
"mischief... again.\n"

"But thanks to you, the\n"
"Crystal Star Block is\n"
"broken, and its power\n"
"has been released!\n"

"From now on, Crystal\n"
"Stars will appear in\n"
"every world, for a\n"
"total of 30 Stars.\n"

"But because Bowser's\n"
"magic is still there,\n"
"you can't collect\n"
"them directly...\n"

"You'll have to surpass\n"
"yourself to be able to\n"
"reach them or summon them\n"
"by completing challenges.\n"

"You can rely only on\n"
"yourself. No cannon,\n"
"no cap power-up, and\n"
"no cheat.\n"

"Captures are allowed.\n"
"Toads and other beings\n"
"can help you, so feel\n"
"free to use them.\n"

"Anywhere in the Castle,\n"
"you can see which Stars\n"
"you already collected\n"
"by pausing the game.\n"

"Be careful.\n"
"The Crystal Stars are\n"
"on another level of\n"
"difficulty.\n"

"This time, Bowser learnt\n"
"his lesson, and didn't\n"
"play around with his\n"
"brand new powers.\n"

"You'll have to do your\n"
"best to catch them all.\n"
"If you don't feel ready\n"
"enough, don't try this."
OMM_DIALOG_SPARKLY_MULTI_END

"Look up.\n"
"\n"
"\n"
"\n"

"Do you see it?\n"
"...\n"
"\n"
"No, you don't.\n"

"You're not going to\n"
"like this.\n"
"You shouldn't have\n"
"broken that Block.\n"

"The Nebula Stars.\n"
"The most powerful force\n"
"known to the Mushroom\n"
"Kingdom, even beyond.\n"

"One must prove its\n"
"worthiness to be able\n"
"to harness their power.\n"
"\n"

"Kaizo Mario 64?\n"
"Super Mario 74 EE?\n"
"Night of Doom?\n"
"King Boo's Revenge?\n"

"They drove crazy the\n"
"fools who tried them.\n"
"But Nebula Stars are\n"
"an even worse evil.\n"

"You're about to face\n"
"something you can't\n"
"imagine, even in your\n"
"worst nightmares.\n"

"But with the Odyssey\n"
"Moveset and Peach,\n"
"it's gonna be a piece\n"
"of cake, right?...\n"

"You're wrong.\n"
"You're going to have\n"
"a VERY bad time.\n"
"\n"

"To begin with, there\n"
"are 90 Nebula Stars.\n"
"Yes. NINETY.\n"
"\n"

"Secondly, to be able\n"
"to catch all of them,\n"
"you'll have to travel\n"
"through time and space.\n"

"You can rely only on\n"
"yourself. No cannon,\n"
"no cap power-up, and\n"
"no cheat.\n"

"The Nebula Stars curse\n"
"reduces your health\n"
"points to 1. Get hit,\n"
"and it's over.\n"

"If by any chance you\n"
"manage to collect 89\n"
"Nebula Stars, come back\n"
"here, if you dare.\n"

"Do you have the guts to\n"
"try this madness, are you\n"
"ready to give up your\n"
"sanity in this challenge?"
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_26, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Read this carefully.\n"
"There are some rules\n"
"you have to remember.\n"

"\n"
"      1. NO CAPS\n"
"\n"

"Mario cannot use any cap\n"
"power-up to find Stars,\n"
"unless inside Cap courses.\n"

"However, the Star must be\n"
"revealed before the cap\n"
"power-up runs out.\n"

"\n"
"     2. CANNONS\n"
"\n"

"Make sure to talk to\n"
"Bob-omb buddies. Their\n"
"cannons will help you.\n"

"\n"
"     3. NO CHEAT\n"
"\n"

"Cheats are forbidden.\n"
"Do not try to cheat.\n"
"Never.\n"

"\n"
"    4. STAR SHARDS\n"
"\n"

"Sometimes, you'll see\n"
"sparkling objects here\n"
"and there.\n"

"Those are Star shards.\n"
"You must collect 8 of\n"
"them to restore the Star."
OMM_DIALOG_SPARKLY_MULTI_END

"Read this carefully.\n"
"There are some rules\n"
"you have to remember.\n"

"\n"
"      1. NO CAPS\n"
"\n"

"Mario cannot use any cap\n"
"power-up, unless inside\n"
"Cap courses.\n"

"However, the Star must be\n"
"found and collected before\n"
"the power-up runs out.\n"

"\n"
"    2. NO CANNON\n"
"\n"

"Mario cannot enter or\n"
"use Bob-omb buddies'\n"
"cannons.\n"

"\n"
"     3. NO VIBES\n"
"\n"

"If you're playing as\n"
"Peach, you can't use\n"
"her Vibe powers.\n"

"But that doesn't mean\n"
"Peach is useless. Even\n"
"without her superpowers,\n"

"she can count on Perry\n"
"to reach places you\n"
"couldn't before.\n"

"\n"
"     4. NO CHEAT\n"
"\n"

"Cheats are forbidden.\n"
"Do not try to cheat.\n"
"Never.\n"

"\n"
"       5. TOADS?\n"
"\n"

"This time, there is no\n"
"Star shard inside the\n"
"Castle.\n"

"The Toads managed to hide\n"
"the Stars before Bowser\n"
"could shatter them.\n"

"So, to be able to get\n"
"the Stars, you must\n"
"find the Toads first."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_27, 0, OMM_DIALOG_SPARKLY_MULTI(4, 4, 0),
"Another tip: Use Cappy\n"
"and its capture ability\n"
"as much as you can.\n"
"\n"

"Sometimes, objects are\n"
"not in their right place,\n"
"and moving them around\n"
"can be the solution.\n"

"Here in the basement,\n"
"Toads are looking for a\n"
"place to stand on, like\n"
"some kind of pedestal..."
OMM_DIALOG_SPARKLY_MULTI_END

"Mips the rabbit is back,\n"
"faster than ever! But...\n"
"what are those sparkles\n"
"that come out of its body?"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_28, 0, OMM_DIALOG_SPARKLY_MULTI(5, 3, 0),
"Last but not least:\n"
"some Stars can appear\n"
"only if you enter a\n"
"level after exiting a\n"
"specific one.\n"

"Did you read that sign\n"
"next to a painting on\n"
"the first floor?..."
OMM_DIALOG_SPARKLY_MULTI_END

"The Crystal Star is here,\n"
"hidden behind the mirror.\n"
"How can you reach it?...\n"

"You'll probably need help\n"
"from Toads to go through\n"
"that wall of glass."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_29, 0, OMM_DIALOG_SPARKLY_MULTI(3, 1, 0),
"Something was written\n"
"here, but some parts\n"
"have been erased...\n"

"S-p-- Ma--o --ys--y\n"
"--sh-o-- --ng-o- -8\n"
"--ur-y--d --es- T--p"
OMM_DIALOG_SPARKLY_MULTI_END

"Totally Odyssey."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_BOWSER_4_UNLOCKED, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"You suddenly feel a\n"
"strong energy coming\n"
"from Castle Grounds...\n"

"Gathering enough Pink-\n"
"Gold Stars opened a path\n"
"leading to a new place.\n"

"Maybe this is where the\n"
"Princess is waiting for\n"
"you to save her...\n"

"There is only one way\n"
"to know. Go, Mario!"
OMM_DIALOG_SPARKLY_MULTI_END

"A force stronger than\n"
"before is emanating\n"
"from Castle Grounds...\n"

"Gathering enough Crystal\n"
"Stars opened a path to\n"
"a dangerous place.\n"

"Nothing good is waiting\n"
"for you, this time...\n"
"Be careful, Mario!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_BOWSER_4_INTRO, SOUND_OBJ_BOWSER_LAUGH | 0xFF00, OMM_DIALOG_SPARKLY_MULTI(4, 4, 0),
"Mario!... and Cappy!\n"
"How did you get here?\n"
"The Pink-Gold Stars...\n"
"You found 29 of them?!\n"

"I can't stand this!\n"
"Do you know how hard\n"
"it was for me to lay\n"
"my hands on them?\n"

"After I lost to you, the\n"
"Grand Star was no longer\n"
"mine... So, I had to rely\n"
"on something else...\n"

"Something more powerful,\n"
"more terrifying...\n"
"Something that could\n"
"surpass the Power Stars!\n"

"With the Pink-Gold Stars,\n"
"I was able to lock the\n"
"Princess away before you\n"
"could even save her!\n"

"I kept one for myself,\n"
"and used my magic to\n"
"hide the other ones\n"
"in each world...\n"

"There was no way you\n"
"could find all the\n"
"Pink-Gold Stars and\n"
"use them against me!\n"

"My plan was flawless!\n"
"First the Princess,\n"
"and then the Mushroom\n"
"Kingdom!...\n"

"But you had to ruin\n"
"everything again!\n"
"Oh, I hate you!\n"
"You'll pay for this!\n"

"No more tricks, traps\n"
"or evil plans...\n"
"Just you and me, in a\n"
"duel of raw strength!\n"

"With the full power of\n"
"the last Pink-Gold Star,\n"
"I'll reduce you to ashes!\n"
"It's showtime!"
OMM_DIALOG_SPARKLY_MULTI_END

"Mario!... and Cappy!\n"
"Don't you ever give up?\n"
"The Crystal Stars...\n"
"You found 29 of them?!\n"

"No way! Do you know how\n"
"hard it was for me to\n"
"lay my hands on them?\n"
"No, you don't!\n"

"It turned out that the\n"
"Pink-Gold Stars weren't\n"
"powerful enough, so I had\n"
"to rely on something else...\n"

"Something more dangerous,\n"
"more threatening...\n"
"Something that could\n"
"annihilate everything!\n"

"With the Crystal Stars,\n"
"I created the most evil,\n"
"challenging and mind\n"
"breaking experience!\n"

"My plan was flawless!\n"
"The Crystal Stars were\n"
"supposed to defeat you,\n"
"once and for all!\n"

"But you had to ruin\n"
"everything... again!\n"
"Oh, I despise you...\n"
"You're driving me nuts!\n"

"I'm angrier than ever!\n"
"Nothing can stop me now!\n"
"Mario, Cappy... Taste the\n"
"full extent of my power!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_BOWSER_4_DEFEAT, SOUND_OBJ_BOWSER_DEFEATED, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Noooo! Not again!\n"
"Why does it have to\n"
"always end like this?\n"

"You win this time...\n"
"But you haven't heard\n"
"the last of me!\n"

"I'll be back, stronger\n"
"and more furious than\n"
"ever! You'll see!\n"

"Until then, don't get\n"
"stuck in a painting!\n"
"Gwah ha ha ha!"
OMM_DIALOG_SPARKLY_MULTI_END

"...Wasn't that enough?\n"
"How can you be so strong?\n"
"That's unfair!\n"

"I can't stand losing to\n"
"you, again and again...\n"
"I need some rest...\n"

"Take it as a win for now,\n"
"but don't brag about it\n"
"for too long...\n"

"I'll come back, sooner or\n"
"later... and you'll see!\n"
"Bwah ah ah ah ah!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_MIPS_1, 0, 3, 30, 200,
"Long time no see...\n"
"How long has it been\n"
"since we last met?\n"

"How about you chasing\n"
"me again, like the good\n"
"old times?\n"

"I have one of these\n"
"shiny things you want\n"
"so much...\n"

"But you'll have to do\n"
"better than that! I can\n"
"go faster! Can you?"
},

{
OMM_DIALOG_SPARKLY_MIPS_2, 0, 3, 30, 200,
"Not bad. Not bad at all.\n"
"But I think you can do\n"
"even better.\n"

"Ready for the last round?\n"
"Give it your all, I'm not\n"
"going to make it easy!"
},

{
OMM_DIALOG_SPARKLY_MIPS_3, 0, 3, 30, 200,
"Ok! Ok! I got it.\n"
"I was too slow.\n"
"I lose. You win.\n"

"But, hey, it was fun\n"
"playing with you.\n"
"Here, take your Star."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_INTRO, 0, 4, 95, 200,
"Hey you!\n"
"Stop right there!\n"
"You can't fool me,\n"
"I have eyes everywhere!"
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_0, 0, 4, 95, 200,
"You ain't a good cheater,\n"
"you got easily spotted.\n"
"Next time stay in order,\n"
"you won't be suspected."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_1, 0, 4, 95, 200,
"Cheating is not fair,\n"
"can you stop please?\n"
"I thought I was clear,\n"
"you cannot do this."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_2, 0, 4, 95, 200,
"No matter what you try,\n"
"I'm always ahead of you.\n"
"If the game you modify,\n"
"I'm going to punish you."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_3, 0, 4, 95, 200,
"What you do is not cool,\n"
"no matter the intention.\n"
"Cheats make you a fool,\n"
"who needs a correction."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_4, 0, 4, 95, 200,
"Do you think I won't see\n"
"a despicable cheater?\n"
"You tried to trick me,\n"
"shame on you, you faker."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_5, 0, 4, 95, 200,
"In the code, an anomaly,\n"
"I can detect, immediately.\n"
"Cheats are no exception,\n"
"they get all my attention."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_6, 0, 4, 95, 200,
"I sensed an irregularity\n"
"inside the game's memory.\n"
"Is cheating what you want?\n"
"I'm sorry, but you can't."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_7, 0, 4, 95, 200,
"What an odd behavior I see\n"
"that makes me wonder...\n"
"Is this game too glitchy?\n"
"No, nothing but a cheater."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_0, 0, 4, 95, 200,
"I'll let it go for now,\n"
"but don't do that again.\n"
"Get caught twice in a row,\n"
"and I will show you pain."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_1, 0, 4, 95, 200,
"I'll do nothing this time,\n"
"but think about it twice.\n"
"Keep crossing the line,\n"
"and I won't be that nice."
},

{
OMM_DIALOG_SPARKLY_ANTI_CHEAT_END_2, 0, 4, 95, 200,
"I can tolerate this breach,\n"
"but only once in a while.\n"
"Make me repeat my speech?\n"
"Say bye to your savefile."
},

};
