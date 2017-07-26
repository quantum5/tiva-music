#include <stdint.h>
#include <stdbool.h>
#include "player.h"

static const note song_notes[] = {{587, 675}, {523, 225}, {466, 450}, {523, 900}, {440, 450}, {392, 900}, {523, 450}, {440, 450}, {349, 900}, {587, 675}, {523, 225}, {466, 450}, {349, 1350}, {293, 450}, {466, 1800}, {0, 450}, {587, 675}, {523, 225}, {466, 450}, {523, 900}, {440, 450}, {392, 900}, {523, 450}, {440, 450}, {349, 900}, {587, 675}, {523, 225}, {466, 450}, {349, 1350}, {293, 450}, {466, 1800}, {0, 450}, {466, 450}, {440, 450}, {392, 450}, {349, 450}, {311, 450}, {293, 450}, {293, 450}, {392, 2700}, {392, 450}, {349, 450}, {311, 450}, {311, 450}, {293, 450}, {261, 450}, {293, 2700}, {466, 450}, {440, 450}, {392, 450}, {349, 450}, {311, 450}, {293, 450}, {293, 450}, {392, 1350}, {392, 450}, {440, 450}, {466, 1350}, {587, 1350}, {523, 2700}, {587, 675}, {523, 225}, {466, 450}, {523, 900}, {440, 450}, {392, 900}, {523, 450}, {440, 450}, {349, 900}, {587, 675}, {523, 225}, {466, 450}, {349, 1350}, {293, 450}, {466, 900}, {0, 900}, {0, 450}, {587, 675}, {523, 225}, {466, 450}, {523, 900}, {440, 450}, {392, 900}, {523, 450}, {440, 450}, {349, 900}, {587, 675}, {523, 225}, {466, 450}, {349, 1350}, {293, 450}, {466, 1800}, {0, 450}, {466, 450}, {440, 450}, {392, 450}, {349, 450}, {311, 450}, {293, 450}, {293, 450}, {392, 1800}, {392, 450}, {349, 450}, {311, 450}, {311, 450}, {293, 450}, {261, 450}, {293, 2700}, {466, 450}, {440, 450}, {392, 450}, {349, 450}, {311, 450}, {293, 450}, {293, 450}, {392, 1350}, {392, 450}, {440, 450}, {466, 1350}, {587, 1350}, {523, 2700}, {698, 675}, {622, 225}, {554, 450}, {622, 900}, {523, 450}, {466, 900}, {622, 450}, {523, 450}, {415, 900}, {698, 675}, {622, 225}, {554, 450}, {415, 1350}, {349, 450}, {554, 2700}, {698, 675}, {622, 225}, {554, 450}, {622, 900}, {523, 450}, {466, 900}, {622, 450}, {523, 450}, {415, 900}, {698, 675}, {622, 225}, {554, 450}, {415, 1350}, {349, 450}, {554, 1800}, {554, 450}, {554, 450}, {415, 450}, {349, 450}, {554, 450}, {523, 450}, {466, 450}, {493, 450}, {369, 450}, {311, 450}, {493, 450}, {440, 450}, {415, 450}, {440, 450}, {329, 450}, {277, 450}, {415, 450}, {392, 450}, {369, 450}, {392, 450}, {293, 450}, {246, 450}, {392, 450}, {369, 450}, {329, 450}};
static const lyric_line song_lyrics[48] = {{0, "Bravo, Christine"}, {4, "das war"}, {6, "fantastisch,"}, {9, "So muss Musik"}, {13, "klingen!"}, {16, "Ach, ich w\x14sst'"}, {19, "gern, was du"}, {22, "geheim h\x12ltst,"}, {25, "wer lehrt dich"}, {28, "jetzt singen?"}, {32, "Vater versprach"}, {36, "einen Engel,"}, {40, "Einst kam im"}, {43, "Traum er zu mir."}, {47, "Ich f\x14hl' es"}, {50, "jetzt kanns"}, {52, "nicht glauben:"}, {55, "Er ist wirklich"}, {59, "hier!"}, {60, "Er ruft mich"}, {63, "leis' bei meinem"}, {67, "Namen, bleibt"}, {70, "bis er mich"}, {73, "geh'n l\x12sst."}, {77, "\x11""berall sp\x14r"}, {81, "ich seine N\x12he,"}, {86, "Ihn, der sich"}, {89, "nie seh'n l\x12sst."}, {93, "Du kommst mir"}, {96, "vor wie ein"}, {99, "Schatten,"}, {101, "F\x14r mich bist du"}, {105, "ganz weit fort."}, {108, "Ich h\x13r' dich"}, {111, "singen im"}, {114, "Dunkeln,"}, {116, "Und versteh"}, {119, "kein Wort."}, {121, "Engel der Muse,"}, {126, "f\x14hr und"}, {128, "leit mich,"}, {130, "Dann wird mein"}, {133, "Weg klar sein."}, {136, "Engel der Muse,"}, {141, "mir nur"}, {143, "zeig dich,"}, {145, "Lass diesen"}, {148, "Traum wahr sein!"}};

const sw_song angelofmusic = {
    song_notes, ARRAY_SIZE(song_notes),
    song_lyrics, ARRAY_SIZE(song_lyrics),
};
