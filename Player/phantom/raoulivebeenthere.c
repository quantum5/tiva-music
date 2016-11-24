#include <stdint.h>
#include <stdbool.h>
#include "player.h"

static const note song_notes[] = {{493, 750}, {554, 750}, {440, 750}, {392, 750}, {0, 250}, {493, 250}, {493, 250}, {554, 250}, {554, 250}, {554, 250}, {440, 500}, {440, 250}, {392, 750}, {0, 250}, {392, 250}, {392, 250}, {440, 250}, {440, 250}, {440, 250}, {349, 250}, {349, 250}, {349, 250}, {311, 250}, {311, 250}, {311, 250}, {277, 700}, {277, 1400}, {246, 700}, {246, 1400}, {554, 750}, {622, 750}, {523, 750}, {466, 750}, {0, 250}, {554, 250}, {554, 250}, {622, 250}, {622, 250}, {622, 250}, {523, 500}, {523, 250}, {466, 500}, {0, 250}, {466, 250}, {466, 250}, {523, 250}, {523, 250}, {523, 250}, {415, 250}, {415, 250}, {415, 250}, {415, 250}, {369, 250}, {415, 250}, {466, 250}, {466, 250}, {466, 250}, {523, 250}, {523, 250}, {523, 250}, {415, 250}, {415, 250}, {415, 250}, {369, 250}, {369, 250}, {369, 250}, {329, 1000}, {329, 1500}, {0, 500}, {293, 1000}, {293, 2000}, {0, 1500}, {587, 750}, {587, 750}, {622, 1125}, {554, 375}, {493, 375}, {466, 375}, {415, 375}, {369, 375}, {415, 375}, {415, 750}, {369, 750}, {329, 750}, {415, 375}, {493, 375}, {659, 750}, {622, 562}, {554, 187}, {554, 375}, {493, 375}, {493, 562}, {466, 187}, {466, 1500}, {0, 750}, {622, 500}, {622, 500}, {622, 1000}, {554, 500}, {523, 500}, {466, 500}, {415, 500}, {466, 500}, {415, 500}, {830, 4000}, {0, 500}, {523, 500}, {523, 500}, {523, 1000}, {523, 500}, {523, 500}, {523, 500}, {466, 500}, {523, 500}, {554, 500}, {523, 2000}, {0, 2000}, {0, 2000}, {0, 500}, {493, 750}, {587, 750}, {659, 750}, {659, 1500}, {369, 750}, {392, 750}, {440, 750}, {493, 750}, {392, 750}, {369, 750}, {329, 3000}, {0, 750}, {493, 750}, {587, 750}, {659, 750}, {659, 1500}, {369, 750}, {392, 750}, {440, 750}, {493, 750}, {392, 750}, {369, 750}, {392, 4500}};

const sw_song raoulivebeenthere = {
    song_notes, ARRAY_SIZE(song_notes),
    0, 0,
};
