static struct OmmDialogEntryRaw { s32 id; u32 soundBits; s32 linesPerBox; s32 leftOffset; s32 downOffset; const char *str; } sOmmDialogEntriesRaw[] = {

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
OMM_DIALOG_SPARKLY_STAR_NAMES, 0, 1, 95, 200,
"PRECISE CANNON SHOTS\n"
"SHINY GOLDEN COINS\n"
"TREASURE OF THE FLOODED BAY\n"
"SUSPICIOUS SLIDE SHORTCUT\n"
"BLUE COINS HUNT\n"
"CLIMBING IN THE CAVE\n"
"IMMINENT VOLCANIC ERUPTION\n"
"SHINING ABOVE THE PYRAMID\n"
"IN FRONT OF BOWSER'S SUB\n"
"A SNOWMAN ON THE BIG HEAD\n"
"ABOVE THE SKY TUNNEL\n"
"STAR OF THE DEADLY SLIDE\n"
"TEN TINY GOOMBAS\n"
"NO TIME TO WASTE\n"
"PERILOUS RIDE\n"
"ODYSSEY OF A GOOMBA\n"
"MUSHROOMS IN THE FIRE SEA\n"
"THE FINAL TASK\n"
"FREE FLIGHT FOR COINS\n"
"MASTERED AIR CONTROL\n"
"WALL-JUMP MADNESS\n"
"NOTHING BUT SLIDING\n"
"JUST MARIO OVER THE RAINBOW\n"
"FROZEN REDS IN COLD WATER\n"
"ABOVE PEACH'S CASTLE\n"
"INSIDE PEACH'S CASTLE\n"
"TOADS DRAINING THE MOAT\n"
"AT THE PINNACLE OF TIME\n"
"COURTYARD CHEST TRAP?\n"
"FIGHT FOR A PRINCESS\n"

"MARIO WINGLESS TO THE SKY\n"
"HEIST OF WHOMP'S VAULT\n"
"LOST IN THE DARK DEPTHS\n"
"APEX OF THE MOUNTAIN\n"
"TIMED BLUE COINS HUNT\n"
"FIREFIGHTING IN THE CAVE\n"
"VOLCANIC DISASTER\n"
"SEALED INSIDE THE PYRAMID\n"
"POLE-JUMPING ABOVE THE DOCKS\n"
"THE FOUR SNOWMEN\n"
"CHUCKYA MAGIC TRICK\n"
"BLIND JUMP INTO THE VOID\n"
"HUGE TOWER OF GOOMBAS\n"
"CLIMBING THE CLOCK\n"
"MUSHROOMS IN THE SKY\n"
"GOOMBA EMERGENCY\n"
"THROUGH THE FIRE AND FLAMES\n"
"THE ULTIMATE TASK\n"
"FAST FLIGHT FOR COINS\n"
"PERFECT AIR CONTROL\n"
"WALL-JUMP INSANITY\n"
"NEVER-ENDING SLIDE\n"
"ONLY MARIO OVER THE RAINBOW\n"
"FROZEN COINS IN COLD WATER\n"
"TOP OF PEACH'S CASTLE\n"
"AVOIDING BOWSER'S TRAP\n"
"MIPS THE YELLOW RABBIT\n"
"SECRET OF THE MIRROR ROOM\n"
"TOTALLY CLASSIC?\n"
"HERE WE GO AGAIN\n"

"TRUE CHAMPIONS RACE\n"
"SMMS - Green Woods\n"
"HARDER, DEEPER, COLDER\n"
"SM64 - Cool, Cool Mountain\n"
"SMSR - Cloudrail Station\n"
"74EE - Infernopit Cave\n"
"SMSR - Koopa Canyon\n"
"74EE - Overgrown Oasis\n"
"SMSR - Mad Musical Mess\n"
"74EE - Dried-out Dead Desert\n"
"74EE - Cliff of Pain\n"
"SM64 - Tall, Tall Mountain\n"
"SMSR - Bob-omb Battle Factory\n"
"SMSR - Starlight Runway\n"
"74EE - Crudelo Sphere\n"
"THE INVISIBLE STAR\n"
"SMSR - Bowser's Rainbow Rumble\n"
"74EE - Ztar Zanctuary\n"
"SMSR - Windy Wing Cap Well\n"
"SMMS - Sweet Sweet Rush\n"
"74EE - Toxic Terrace\n"
"THE ULTIMATE SLIDE\n"
"SMSR - Mushroom Mountain Town\n"
"74EE - Old Supply Slide\n"
"SOARING STAR\n"
"74EE - Lake of the Lords\n"
"SMSR - Star Road (the place)\n"
"SMSR - Zero Life Area\n"
"74EE - Triarc Bridge\n"
"SM64 - Final Boss\n"
},

{
OMM_DIALOG_SPARKLY_STAR_1, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 3),
"I'll give you a useful tip.\n"
"You should take a look at\n"
"your star counter.\n"

"Do you see an 8th star\n"
"icon? That's a Pink Gold\n"
"Star indicator.\n"

"It should be empty. That\n"
"means a Pink Gold Star\n"
"is hidden somewhere.\n"

"If it's filled, then you\n"
"already collected that\n"
"Pink Gold Star.\n"

"But if it disappears,\n"
"that means you broke\n"
"one of the rules.\n"

"In that case, the Star\n"
"will turn intangible,\n"
"or won't spawn at all.\n"

"But don't worry! Just\n"
"restart the level to\n"
"make it available again.\n"

"And since that's the very\n"
"first level of the game,\n"
"here's a hint to help you.\n"

"Five secrets in five rings\n"
"in the sky, but you cannot\n"
"use the Wing cap to fly.\n"

"If a secret is too high,\n"
"you should try to use\n"
"a Bob-omb buddy cannon."
OMM_DIALOG_SPARKLY_MULTI_END

"Nothing really new here.\n"
"Five secrets, but no wing\n"
"cap, and no cannon.\n"

"Fairly simple rules, but\n"
"not so easy to actually\n"
"get to the Crystal Star.\n"

"To give you an idea of the\n"
"difficulty, this is one of\n"
"the easiest Stars.\n"

"If you don't feel good\n"
"enough for this, give\n"
"up now or get better."
OMM_DIALOG_SPARKLY_MULTI_END

"For years Koopa the Quick\n"
"was misunderstood.\n"
"\n"

"He knows he can't win\n"
"against people that keep\n"
"jumping and rolling around.\n"

"But nobody listens to him,\n"
"everyone keeps mashing\n"
"the A button.\n"

"So, here's the actual\n"
"challenge, intended by\n"
"Koopa the Quick himself.\n"

"How about a race to the\n"
"mountaintop, while you\n"
"collect every single star?\n"

"Of course, no cheating\n"
"allowed.\n"
"\n"

"You can't collect anything\n"
"or talk to King Bob-omb\n"
"before the race starts.\n"

"And you can't end the\n"
"race before collecting\n"
"all stars.\n"

"This is a race for the\n"
"true champions, like\n"
"Koopa the Quick."
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_2, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"There are various ways\n"
"to make Pink Gold Stars\n"
"appear.\n"

"Most of them stand still\n"
"somewhere in the level,\n"
"waiting for Mario.\n"

"This type is the easiest\n"
"to find, but the hardest\n"
"to collect, as those Stars\n"

"are often located in the\n"
"least convenient places.\n"
"\n"

"You can spot them by\n"
"looking around you, or by\n"
"noticing something unusual.\n"

"The other type can spawn\n"
"only after Mario meets\n"
"certain conditions.\n"

"These ones are hard to\n"
"find, but quite easy to\n"
"reach once revealed.\n"

"Also, after revealing a\n"
"Pink Gold Star, most of\n"
"the restrictions don't\n"

"apply anymore, and, for\n"
"instance, Mario can use\n"
"caps again.\n"

"In this level, the Star\n"
"will show up once you've\n"
"collected 90 gold coins."
OMM_DIALOG_SPARKLY_MULTI_END

"You horrid little human!\n"
"How did you dare stealing\n"
"my precious gold coins?\n"

"We build your houses.\n"
"We put your walls up.\n"
"We pave your roads.\n"

"We earned this money\n"
"and that Sparkly Star\n"
"from our hard work!\n"

"You can't just pass by\n"
"and take these as if\n"
"they were yours!\n"

"It makes me so mad!\n"
"But I'm not going to\n"
"let you rob us twice.\n"

"This time, you'll need\n"
"100 golden coins to\n"
"make the Star appear.\n"

"I hid the Star inside\n"
"the fortress vault, so\n"
"nobody can reach it!\n"

"On top of that, I even\n"
"added a timer, to make\n"
"the vault inaccessible\n"

"after 1 minute and 50\n"
"seconds! Ha!\n"
"//// --- Whomp King"
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
"80 coins, because that\n"
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

"You keep going deeper.\n"
"Welcome to the lowest\n"
"point known to mankind.\n"

"Surprisingly, you can\n"
"clearly see around you.\n"
"\n"

"But there is something\n"
"worse than a pitch black\n"
"darkness.\n"

"Look. You're the only\n"
"living being. Because\n"
"of that deadly cold.\n"

"Fortunately, the Calm\n"
"Vibe prevents Peach from\n"
"being frozen to death.\n"

"But let it run out, and\n"
"you won't survive long..."
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_4, 0, OMM_DIALOG_SPARKLY_MULTI(0, 0, 0),
""
OMM_DIALOG_SPARKLY_MULTI_END

""
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
"15 seconds. Hurry up!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_6, 0, OMM_DIALOG_SPARKLY_MULTI(0, 4, 0),
""
OMM_DIALOG_SPARKLY_MULTI_END

"The flames of the cave\n"
"are bigger than usual...\n"
"Is that because of the\n"
"Crystal Star?\n"

"The fire is pretty wild,\n"
"someone should extinguish\n"
"it to avoid a disastrous\n"
"outcome...\n"

"3 minutes. There isn't too\n"
"much time left! Find the\n"
"17 flames, put them out,\n"
"and grab the Star!"
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_7, 0, OMM_DIALOG_SPARKLY_MULTI(2, 2, 0),
"The Pink Gold Star's been\n"
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
OMM_DIALOG_SPARKLY_STAR_8, 0, OMM_DIALOG_SPARKLY_MULTI(0, 3, 0),
""
OMM_DIALOG_SPARKLY_MULTI_END

"To get the Crystal Star\n"
"of the pyramid, you'll\n"
"have to break two seals.\n"

"The first condition is\n"
"to open the path leading\n"
"to the top.\n"

"The second condition is\n"
"to collect all 62 coins\n"
"scattered inside.\n"

"You start with almost no\n"
"time, but coins are here\n"
"to fill your hourglass."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_9, 0, OMM_DIALOG_SPARKLY_MULTI(0, 0, 0),
""
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_10, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Can you reach the top\n"
"of the big snowman...\n"
"as a snowman?\n"

"The Pink Gold Star power\n"
"gives you an extra bit\n"
"of determination...\n"

"...making snowmen jump\n"
"higher than usual."
OMM_DIALOG_SPARKLY_MULTI_END

"Prepare for trouble!\n"
"And make it quadruple!\n"
"\n"

"The snowman you brought\n"
"to the top enjoyed the\n"
"view so much that...\n"

"He wants to go back\n"
"again! But this time,\n"
"with his friends.\n"

"You'll probably need the\n"
"help of some spindrifts\n"
"to gain extra height."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_11, 0, OMM_DIALOG_SPARKLY_MULTI(0, 2, 0),
""
OMM_DIALOG_SPARKLY_MULTI_END

"You see the opening where\n"
"the red coins star is?\n"

"You should try to bring\n"
"a Chuckya in there."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_12, 0, OMM_DIALOG_SPARKLY_MULTI(0, 0, 0),
""
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_13, 0, OMM_DIALOG_SPARKLY_MULTI(4, 4, 0),
"Those little Goombas\n"
"aren't very strong, but\n"
"they're fast. Catching\n"
"them isn't that easy.\n"

"Rumors say that if you\n"
"manage to pile up ten of\n"
"them in one go, a Pink\n"
"Gold Star will appear."
OMM_DIALOG_SPARKLY_MULTI_END

"A Crystal Star is hidden\n"
"somewhere in the clouds,\n"
"but you can't see it from\n"
"the ground...\n"

"You must be at least as\n"
"tall as ten giant Goombas\n"
"to have a chance to catch\n"
"sight of it..."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_14, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"You're inside a clock.\n"
"What about playing\n"
"a timed game?\n"

"You see that yellow box?\n"
"There are 13 of them in\n"
"this stage.\n"

"Will you be fast enough\n"
"to open them all in less\n"
"than 60 seconds?"
OMM_DIALOG_SPARKLY_MULTI_END

"It's the same clock again.\n"
"And you're going to be in\n"
"for a rough time.\n"

"You don't have to break\n"
"some boxes. You're not\n"
"limited by a timer.\n"

"And the Crystal Star is\n"
"already here, waiting\n"
"for you at the top.\n"

"...\n"
"But you still have to\n"
"follow one simple rule.\n"

"Don't touch the floor\n"
"again after leaving it.\n"
"Super Mario Maker style."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_15, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Can you reach the top\n"
"of the mansion without\n"
"pressing the [A] button?"
OMM_DIALOG_SPARKLY_MULTI_END

"For the last level of the\n"
"game, you have to collect\n"
"all nine 1-up mushrooms.\n"

"But! You can only use\n"
"Cappy. You can't press\n"
"the [A], [B] and [Z] buttons."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_16, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 4),
"You are now a Goomba.\n"
"Until you've got all 8 red\n"
"coins, follow these rules:\n"

"Don't leave the blue area\n"
"or grab a coin as Mario,\n"
"or you'll have to restart."
OMM_DIALOG_SPARKLY_MULTI_END

"You are now a Goomba.\n"
"And this is really bad.\n"
"You didn't do your job.\n"

"Six impostors, disguised\n"
"as Goombas, stole 74 of\n"
"Bowser's coins.\n"

"Get the money back, and\n"
"catch the culprits before\n"
"Bowser learns about it.\n"

"You only have 1 minute\n"
"and 45 seconds before\n"
"you get roasted. Go!\n"

"(Remember: You can't\n"
"leave the blue area or\n"
"grab a coin as Mario.)"
OMM_DIALOG_SPARKLY_MULTI_END

"In a parallel universe,\n"
"there is a version of\n"
"BITS where everything\n"
"is invisible.\n"

"In that universe lies\n"
"a great power. This is\n"
"the power of a Nebula\n"
"Star.\n"

"To be able to summon it,\n"
"one must go through all\n"
"30 Star rings under\n"
"3 minutes.\n"

"A true champion doesn't\n"
"need to see the floor\n"
"he's on to move forward.\n"
"Be that champion."
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_17, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Let's crank the difficulty\n"
"up a bit. There are seven\n"
"1-up mushrooms here.\n"

"It's simple. Grab them all,\n"
"but only with captures.\n"
"Don't get them as Mario."
OMM_DIALOG_SPARKLY_MULTI_END

"Let's step up the pace.\n"
"Six secrets (and a star)\n"
"are waiting for you.\n"

"For the first four, cross\n"
"the fire sea and reach the\n"
"corners, then come back.\n"

"For the last two, climb\n"
"the pillars that support\n"
"the middle and top floors.\n"

"Survive these trials, and\n"
"the Crystal Star should\n"
"spawn at the end.\n"

"And don't try to take\n"
"shortcuts, it will end\n"
"badly for you..."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_18, 0, OMM_DIALOG_SPARKLY_MULTI(4, 4, 0),
"The end in near. Here's\n"
"your final task: All red\n"
"coins without a single\n"
"[A] button press."
OMM_DIALOG_SPARKLY_MULTI_END

"Can you feel it?\n"
"It's almost over.\n"
"Here's your final task:\n"
"Collect all red coins.\n"

"But that would be too\n"
"easy for a pro player\n"
"like you. So I decided\n"
"to add some constraints.\n"

"You can't capture or\n"
"collect anything else,\n"
"and you must not press\n"
"the [A], [B] or [Z] button."
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
"time to reach the star."
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

"Reach the star at the\n"
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
"Pink Gold Star is yours."
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

"Are you EXTREMELY good\n"
"at sliding? Don't make\n"
"me laugh!\n"

"50 coins, in 21 seconds,\n"
"without a single button\n"
"press.\n"

"Beat this time, even by\n"
"a millisecond, and the\n"
"Nebula Star is yours."
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_23, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"True test of skill.\n"
"Collect all 8 red coins\n"
"without the Wing cap."
OMM_DIALOG_SPARKLY_MULTI_END

"True test of skill.\n"
"All coins, no cannon,\n"
"no Wing cap."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_24, 0, OMM_DIALOG_SPARKLY_MULTI(3, 3, 0),
"Cold water drains Mario's\n"
"health quickly, so don't\n"
"stay in for too long.\n"

"Collect the 8 red coins\n"
"and survive until the\n"
"Pink Gold Star appear."
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

"Do you see me?\n"
"That sparkly object, far\n"
"above the castle tower?\n"
"I'm a Pink Gold Star.\n"

"You know the Princess\n"
"is missing, right?\n"
"Even the Grand Star\n"
"couldn't free her...\n"

"That's because Bowser\n"
"stole us from her,\n"
"and used us to make\n"
"mischief again...\n"

"But thanks to you, the\n"
"Pink Gold Star Block,\n"
"in which we were held\n"
"captive, is broken!\n"

"From now on, Pink Gold\n"
"Stars will appear in\n"
"every world, for a\n"
"total of 30 Stars.\n"

"But because we're not\n"
"totally free, we still\n"
"have to obey to some\n"
"of Bowser's rules.\n"

"That means we can't\n"
"appear at will, you'll\n"
"have to fulfill certain\n"
"conditions first.\n"

"You can rely only on\n"
"yourself. Cap power-ups\n"
"are forbidden, as well\n"
"as cheats.\n"

"But cannons and captures\n"
"are ok. Toads and other\n"
"beings can help you, so\n"
"feel free to use them.\n"

"Anywhere in the Castle,\n"
"you can see what Stars\n"
"you already collected\n"
"by pausing the game.\n"

"If you ever feel lost,\n"
"remember to look around\n"
"you with [C]^, especially\n"
"in large areas."
OMM_DIALOG_SPARKLY_MULTI_END

"Look up.\n"
"\n"
"\n"
"\n"

"Do you see me?\n"
"That sparkly object, far\n"
"above the castle tower?\n"
"I'm a Crystal Star.\n"

"But the Princess is\n"
"already safe, right?\n"
"So why is it there,\n"
"waiting for you?\n"

"That's because Bowser\n"
"found us, the Crystal\n"
"Stars, and used us to\n"
"make mischief... again.\n"

"But thanks to you, the\n"
"Crystal Star Block,\n"
"in which we were held\n"
"captive, is broken!\n"

"From now on, Crystal\n"
"Stars will appear in\n"
"every world, for a\n"
"total of 30 stars.\n"

"But because we're not\n"
"totally free, we still\n"
"have to obey to some\n"
"of Bowser's rules.\n"

"That means we can't\n"
"appear at will, you'll\n"
"have to fulfill certain\n"
"conditions first.\n"

"You can rely only on\n"
"yourself. No cannon,\n"
"no cap power-up, and\n"
"no cheat.\n"

"Captures are allowed.\n"
"Toads and other beings\n"
"can help you, so feel\n"
"free to use them.\n"

"Anywhere in the Castle,\n"
"you can see what stars\n"
"you already collected\n"
"by pausing the game.\n"

"If you ever feel lost,\n"
"remember to look around\n"
"you with [C]^, especially\n"
"in large areas.\n"

"Be careful. There is a\n"
"world between the Pink\n"
"Gold Stars and us, the\n"
"Crystal Stars.\n"

"This time, Bowser learnt\n"
"his lesson, and didn't\n"
"play around with his\n"
"brand new powers.\n"

"You'll have to do your\n"
"best to catch us all.\n"
"If you don't feel ready\n"
"enough, don't try this."
OMM_DIALOG_SPARKLY_MULTI_END

"Look up.\n"
"\n"
"\n"
"\n"

"Do you see me?\n"
"...\n"
"\n"
"No, you don't.\n"

"You're not going to\n"
"like this.\n"
"You shouldn't have\n"
"broken that block.\n"

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

"Us Nebula Stars built\n"
"these wicked worlds.\n"
"We drove crazy the fools\n"
"who tried to challenge us.\n"

"And now, you're about\n"
"to face something even\n"
"harder, something you\n"
"can't even imagine.\n"

"But with the Odyssey\n"
"Moveset and Peach,\n"
"it's gonna be a piece\n"
"of cake, right?...\n"

"You're wrong.\n"
"You're going to have\n"
"a VERY bad time.\n"
"\n"

"As usual, there are 30\n"
"Nebula Stars, scattered\n"
"across the same number\n"
"of levels.\n"

"But to be able to catch\n"
"all of us, you'll have\n"
"to travel through time\n"
"and space.\n"

"The Original world.\n"
"The Moon world.\n"
"The Extreme world.\n"
"The Star world.\n"

"Collect 29 of us from\n"
"these 4 worlds, then\n"
"come back here, if\n"
"you dare.\n"

"You can rely only on\n"
"yourself. No cannon,\n"
"no cap power-up, and\n"
"no cheat.\n"

"You have only 1 HP.\n"
"No second chance.\n"
"Get hit, and you're\n"
"good to start over.\n"

"Captures and Vibes are\n"
"allowed, but sometimes,\n"
"you won't be able to\n"
"use them.\n"

"Do you have the guts to\n"
"confront us, are you ready\n"
"to give up your sanity in\n"
"this ultimate challenge?"
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

"However, the star must be\n"
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

"By the way, there is no\n"
"Star shards inside the\n"
"castle, this time.\n"

"The Toads managed to hide\n"
"the Stars before Bowser\n"
"could break them.\n"

"So, to be able to get\n"
"the Stars, you must\n"
"find the Toads first.\n"

"Here, the first one is\n"
"very well hidden. You\n"
"can only see his cap.\n"

"The second one needs\n"
"some Classic moves\n"
"to be reached."
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_27, 0, OMM_DIALOG_SPARKLY_MULTI(4, 0, 0),
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

""
OMM_DIALOG_SPARKLY_MULTI_END

""
OMM_DIALOG_SPARKLY_MULTI_END
},

{
OMM_DIALOG_SPARKLY_STAR_28, 0, OMM_DIALOG_SPARKLY_MULTI(0, 3, 0),
""
OMM_DIALOG_SPARKLY_MULTI_END

"The Crystal Star is here,\n"
"hidden behind the mirror.\n"
"How can you reach it?...\n"

"You'll probably need help\n"
"from Toads to go through\n"
"that wall of glass.\n"

"The first one isn't easy\n"
"to find, you can hardly\n"
"spot his cap.\n"

"To reach the second one,\n"
"you need to go backwards\n"
"at high speed."
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

"Gathering enough Pink\n"
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
"The Pink Gold Stars...\n"
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

"With the Pink Gold Stars,\n"
"I was able to lock the\n"
"Princess away before you\n"
"could even save her!\n"

"I kept one for myself,\n"
"and cast a spell on the\n"
"other ones to make them\n"
"impossible to collect!\n"

"Hidden in every world,\n"
"there was no way you\n"
"could find them and\n"
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
"the last Pink Gold Star,\n"
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
"Pink Gold Stars weren't\n"
"powerful enough, so I had\n"
"to rely on something else...\n"

"Something more dangerous,\n"
"more threatening...\n"
"Something that could\n"
"annihilate everything!\n"

"With the Crystal Stars,\n"
"I created the most evil,\n"
"infuriating and mind\n"
"breaking challenge!\n"

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

"I kinda missed you,\n"
"you know. Nobody else\n"
"is able to catch me.\n"

"What about you chasing\n"
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
"Here, take your star."
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
