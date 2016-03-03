#ifndef RL_CONFIG
#define RL_CONFIG

// NOTE that this is read only once in the event loop
// This structure will be passed to nearly all of the games
// functions! Do not miss use
typedef struct _RL_Config {
    // this makes the structure read only if set
    char readonly;
    // include key bindings here?
    int width;
    int height;
    int gen;
    
} RL_Config;

RL_Config* RL_LoadConfig(const char *configFilePath);
void RL_FreeConfig(RL_Config *config);
#endif
