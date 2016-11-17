#include <stdint.h>
#include <stdbool.h>
#include "player.h"

static const note song_notes[] = {{587, 2700}, {587, 225}, {554, 225}, {523, 225}, {493, 225}, {466, 1800}, {0, 1800}, {587, 2700}, {587, 225}, {554, 225}, {523, 225}, {493, 225}, {466, 1800}, {0, 1800}, {415, 225}, {466, 225}, {0, 450}, {466, 225}, {493, 225}, {523, 225}, {554, 225}, {587, 1800}, {0, 450}, {220, 450}, {293, 450}, {220, 450}, {261, 675}, {233, 225}, {233, 1350}, {196, 450}, {261, 675}, {196, 225}, {220, 1800}, {0, 450}, {220, 450}, {293, 450}, {220, 450}, {261, 675}, {233, 225}, {233, 1350}, {196, 450}, {261, 675}, {196, 225}, {220, 1800}, {0, 450}, {220, 450}, {293, 450}, {349, 450}, {440, 675}, {392, 225}, {392, 1350}, {392, 450}, {523, 675}, {392, 225}, {440, 2700}, {0, 450}, {440, 450}, {587, 1800}, {523, 225}, {466, 225}, {440, 225}, {392, 225}, {349, 225}, {329, 225}, {293, 225}, {277, 1800}, {233, 450}, {233, 675}, {220, 225}, {220, 2700}, {587, 225}, {554, 225}, {523, 225}, {493, 225}, {466, 1800}, {415, 225}, {466, 675}, {554, 225}, {523, 225}, {493, 225}, {466, 225}, {392, 2700}, {293, 450}, {392, 450}, {293, 450}, {349, 675}, {311, 225}, {311, 1350}, {261, 450}, {349, 675}, {261, 225}, {293, 1800}, {293, 450}, {392, 450}, {293, 450}, {349, 675}, {311, 225}, {311, 1350}, {261, 450}, {349, 675}, {261, 225}, {293, 1800}, {293, 450}, {392, 450}, {466, 450}, {587, 675}, {523, 225}, {523, 1350}, {523, 450}, {698, 675}, {523, 225}, {587, 2700}, {0, 450}, {587, 450}, {783, 1800}, {698, 225}, {622, 225}, {587, 225}, {523, 225}, {466, 225}, {440, 225}, {392, 225}, {369, 1800}, {311, 450}, {311, 675}, {293, 225}, {293, 2700}, {783, 225}, {739, 225}, {698, 225}, {659, 225}, {622, 1800}, {587, 450}, {440, 450}, {523, 900}, {493, 1800}, {0, 450}, {246, 450}, {329, 450}, {246, 450}, {293, 675}, {261, 225}, {261, 1350}, {220, 450}, {293, 675}, {220, 225}, {246, 1800}, {246, 450}, {329, 450}, {246, 450}, {293, 675}, {261, 225}, {261, 1350}, {440, 450}, {587, 675}, {440, 225}, {493, 1800}, {0, 450}, {246, 450}, {329, 450}, {392, 450}, {493, 675}, {440, 225}, {440, 1350}, {440, 450}, {587, 675}, {440, 225}, {493, 2700}, {0, 450}, {493, 450}, {659, 1800}, {587, 225}, {523, 225}, {493, 225}, {440, 225}, {392, 225}, {369, 225}, {329, 225}, {311, 1800}, {0, 450}, {261, 450}, {261, 675}, {493, 225}, {493, 1800}, {0, 900}, {349, 450}, {349, 1350}, {392, 450}, {392, 675}, {349, 225}, {349, 675}, {349, 225}, {349, 900}, {523, 1800}, {0, 900}, {830, 750}, {783, 187}, {830, 187}, {783, 187}, {698, 562}, {783, 300}, {698, 562}, {622, 300}, {698, 750}, {830, 1500}, {0, 375}, {932, 750}, {880, 187}, {932, 187}, {880, 187}, {783, 562}, {880, 300}, {783, 562}, {698, 300}, {783, 750}, {932, 1500}, {0, 375}, {932, 750}, {880, 187}, {932, 187}, {880, 187}, {783, 562}, {880, 300}, {783, 562}, {698, 300}, {783, 750}, {932, 1500}, {0, 375}, {1046, 750}, {987, 187}, {1046, 187}, {987, 187}, {880, 562}, {987, 300}, {880, 562}, {783, 300}, {880, 750}, {1046, 1500}, {0, 375}, {1046, 2250}, {0, 375}, {1046, 2250}, {0, 375}, {1046, 2250}, {0, 375}, {1318, 3000}};

const sw_song phantom = {
    song_notes, ARRAY_SIZE(song_notes),
    0, 0,
};
