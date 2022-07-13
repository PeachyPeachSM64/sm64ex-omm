// -------------------------------------------------------------------- //
// LiveSplit Auto-Splitter script for Odyssey Mario's Moveset speedruns //
// By PeachyPeach                                                       //
// -------------------------------------------------------------------- //
// - Starts when the player selects a file;                             //
// - Splits each time a certain amount of stars is collected;           //
// - Stops when the Grand Star is grabbed;                              //
// - Resets when the game returns to the main menu.                     //
// -------------------------------------------------------------------- //
// - Start LiveSplit, load your splits, and save them as a file named   //
//   "splits.lss" next to the "sm64.us.f3dex2e.exe" executable.         //
// - Then, start the game, the auto-splits will be generated from       //
//   the provided "splits.lss" file.                                    //
// -------------------------------------------------------------------- //
// - You need OMM v6.1.0 or later to make it work.                      //
// - Make sure the executable you play is named "sm64.us.f3dex2e.exe".  //
// - For star splits, you must indicate the amount of stars needed in   //
//   brackets or parentheses like this: "[20]" or "(20)".               //
// - If a split has "Bowser" in its name but no star amount, it is      //
//   interpreted as a Bowser key split or a Grand Star split.           //
// - If the final split has no star amount, it is interpreted as        //
//   a Grand Star split (i.e. after defeating the last Bowser).         //
// -------------------------------------------------------------------- //

state("sm64.us.f3dex2e") {
}

init {
	refreshRate = 30;
	vars.splitNum = 0;
	vars.splitPtr = 0;
	byte[] buffer = memory.ReadBytes(modules.First().BaseAddress, modules.First().ModuleMemorySize);
	for (int i = 0; i != -1;) {
		i = Array.IndexOf(buffer, (byte) 0x4F, i + 1);
		if (buffer[i + 0x1] == 0x4D &&
			buffer[i + 0x2] == 0x4D &&
			buffer[i + 0x3] == 0x41 &&
			buffer[i + 0x4] == 0x55 &&
			buffer[i + 0x5] == 0x54 &&
			buffer[i + 0x6] == 0x4F &&
			buffer[i + 0x7] == 0x53 &&
			buffer[i + 0x8] == 0x50 &&
			buffer[i + 0x9] == 0x4C &&
			buffer[i + 0xA] == 0x49 &&
			buffer[i + 0xB] == 0x54) {
			vars.splitPtr = i + 0xC;
			break;
		}
	}
}

start {
	if (memory.ReadValue<int>((IntPtr) ((long) modules.First().BaseAddress + (long) vars.splitPtr)) == -2) {
		vars.splitNum = 0;
		return true;
	}
	return false;
}

split {
	int curSplitNum = memory.ReadValue<int>((IntPtr) ((long) modules.First().BaseAddress + (long) vars.splitPtr));
	if (curSplitNum > vars.splitNum) {
		vars.splitNum = curSplitNum;
		return true;
	}
	return false;
}

reset {
	if (memory.ReadValue<int>((IntPtr) ((long) modules.First().BaseAddress + (long) vars.splitPtr)) == -1) {
		vars.splitNum = 0;
		return true;
	}
	return false;
}
