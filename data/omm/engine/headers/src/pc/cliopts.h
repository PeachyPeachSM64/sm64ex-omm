#ifndef _CLIOPTS_H_
#define _CLIOPTS_H_

#include "pc/fs/fs.h"

struct PCCLIOptions {
    int skip_intro;
    int fullscreen;
    int poolsize; // unused
    sys_path_t configfile; // unused
    sys_path_t gamedir;
    sys_path_t savepath;
};

#define SkipIntro   skip_intro
#define FullScreen  fullscreen
#define PoolSize    poolsize
#define ConfigFile  configfile
#define GameDir     gamedir
#define SavePath    savepath

extern struct PCCLIOptions gCLIOpts;

void parse_cli_opts(int argc, char *argv[]);
void set_cli_opts(void);
void launch_game(const char *exe);

#endif // _CLIOPTS_H
