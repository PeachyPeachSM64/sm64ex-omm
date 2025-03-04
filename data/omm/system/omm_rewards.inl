#define OMM_TEXT_REWARD_(x, name, cond, text) \
OMM_TEXT_(REWARD_##x##_NAME, name) \
OMM_TEXT_(REWARD_##x##_COND, cond) \
OMM_TEXT_(REWARD_##x##_TEXT, text)

OMM_TEXT_REWARD_(INSTANT_CAPS,
"Instant Caps",

"Collect all stars of a course.",

"Collecting all stars of a course allows Mario to\n"
"wear any unlocked cap in that course.\n"
"\n"
"[L] + [D]^ : Wing cap\n"
"[L] + [D]> : Metal cap\n"
"[L] + [D]< : Vanish cap\n"
"[L] + [D]| : Normal cap"
)

OMM_TEXT_REWARD_(UNLIMITED_CAPS,
"Unlimited Caps",

"Collect all stars.",

"Collecting all stars of a game allows Mario to wear\n"
"any unlocked cap anywhere, including the Castle\n"
"and Bowser fights.\n"
"\n"
"[L] + [D]^ : Wing cap\n"
"[L] + [D]> : Metal cap\n"
"[L] + [D]< : Vanish cap\n"
"[L] + [D]| : Normal cap"
)

OMM_TEXT_REWARD_(SUMMON_YOSHI,
"Summon Yoshi",

"Register all 48 captures.",

"Registering all captures allows Mario to summon\n"
"a capturable Yoshi in any course by pressing [L] + [Y]."
)

OMM_TEXT_REWARD_(SPARKLY_STARS,
"Sparkly Stars",

"In Super Mario 64, defeat Bowser with all 120 stars.",

"Defeating Bowser with all 120 stars revealed\n"
"another ending...\n"
"\n"
"Break one of the Sparkly Stars block in Castle\n"
"Grounds and collect all Sparkly Stars to rescue\n"
"Peach once and for all."
)

OMM_TEXT_REWARD_(SPARKLY_SPARKLES,
"Sparkly Sparkles",

"Collect a Sparkly Grand Star.",

"Completing a Sparkly Stars mode allows Mario to\n"
"emit sparkles.\n"
"\n"
"To enable or disable sparkles:\n"
"Options > Odyssey Mario's Moveset > Sparkly Stars >\n"
"Completion Reward."
)

OMM_TEXT_REWARD_(PLAYABLE_PEACH,
"Playable Peach",

"Complete the Sparkly Stars Normal Mode:\n"
"Pink-Gold Stars.",

"Collecting all Pink-Gold Stars unlocked Peach as\n"
"a playable character, with a unique moveset!\n"
"Float and glide with [A], attack with Perry with [B],\n"
"and activate powerful Vibes with [Y] + [D] buttons.\n"
"\n"
"To select Peach:\n"
"Options > Odyssey Mario's Moveset > Character."
)

OMM_TEXT_REWARD_(PERRY_CHARGE,
"Perry Charge",

"Complete the Sparkly Stars Hard Mode:\n"
"Crystal Stars.",

"Collecting all Crystal Stars allows Peach to charge\n"
"and release a powerful shockwave by holding [B].\n"
"\n"
"To enable or disable the charge:\n"
"Options > Odyssey Mario's Moveset > Sparkly Stars >\n"
"Perry Charge."
)

OMM_TEXT_REWARD_(_REDACTED_,
"PLACEHOLDER",

"Complete the Sparkly Stars Lunatic Mode:\n"
"Nebula Stars.",

"PLACEHOLDER"
)
