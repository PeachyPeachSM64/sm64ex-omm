#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS
#if WINDOWS_BUILD
#include <windows.h>
#endif

struct PCCLIOptions gCLIOpts = {0};

static void parse_arg_str(sys_path_t arg, const char *arg_name, const char *arg_value) {
    u32 arg_len = strlen(arg_value);
    if (arg_len >= sizeof(sys_path_t)) {
        sys_fatal("parse_arg_str: Supplied value for `%s` is too long.", arg_name);
    }
    str_cpy(arg, arg_len + 1, arg_value);
}

void parse_cli_opts(int argc, char *argv[]) {
    OMM_DO_ONCE {
        omm_log("Parsing command line arguments\n");
        for (int i = 1; i < argc; ++i) {
            const char *arg_name = argv[i];
            if (strcmp(arg_name, "--skip-intro") == 0) {
                gCLIOpts.skip_intro = 1;
                omm_printf("`skip_intro` set to TRUE\n");
                continue;
            }
            if (strcmp(arg_name, "--fullscreen") == 0) {
                gCLIOpts.fullscreen = 1;
                omm_printf("`fullscreen` set to TRUE\n");
                continue;
            }
            if (strcmp(arg_name, "--windowed") == 0) {
                gCLIOpts.fullscreen = 2;
                omm_printf("`windowed` set to TRUE\n");
                continue;
            }
            if (strcmp(arg_name, "--gamedir") == 0 && (i + 1) < argc) {
                parse_arg_str(gCLIOpts.gamedir, arg_name, argv[++i]);
                omm_printf("`gamedir` set to \"%s\"\n",, gCLIOpts.gamedir);
                continue;
            }
            if (strcmp(arg_name, "--savepath") == 0 && (i + 1) < argc) {
                parse_arg_str(gCLIOpts.savepath, arg_name, argv[++i]);
                omm_printf("`savepath` set to \"%s\"\n",, gCLIOpts.savepath);
                continue;
            }
        }
    }
}

void set_cli_opts(void) {
}

void launch_game(const char *exe) {
    char command_line[3 * sizeof(sys_path_t) + 0x100];
    snprintf(command_line, sizeof(command_line), "\"%s\"%s%s%s%s%s%s%s%s%s",
        exe,
        gCLIOpts.skip_intro      ? " --skip-intro"   : "",
        gCLIOpts.fullscreen == 1 ? " --fullscreen"   : "",
        gCLIOpts.fullscreen == 2 ? " --windowed"     : "",
        *gCLIOpts.gamedir        ? " --gamedir \""   : "",
        *gCLIOpts.gamedir        ? gCLIOpts.gamedir  : "",
        *gCLIOpts.gamedir        ? "\""              : "",
        *gCLIOpts.savepath       ? " --savepath \""  : "",
        *gCLIOpts.savepath       ? gCLIOpts.savepath : "",
        *gCLIOpts.savepath       ? "\""              : ""
    );
#if WINDOWS_BUILD
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    CreateProcess(NULL, command_line, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
#else
    system(command_line);
#endif
}
