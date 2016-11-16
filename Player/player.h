#ifndef PLAYER_H_
#define PLAYER_H_

extern uint32_t clock_rate;

// Tools.
// Shoot yourself if you don't pass arrays.
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof*(arr))

// Background PWM playback functions.
void pwm_setup(void);
void pwm_play(const unsigned char *pcm, int size, int sample_rate);
void pwm_swap_buffer(const unsigned char *pcm, int size);
void pwm_wait(void);
void pwm_stop(void);
extern volatile bool pwm_playing; // Mutate at your own peril.

// Low level PWM API, unstable interface.
extern const unsigned char *pwm_sample;
extern const unsigned char *pwm_sample_end;

// PWM finish callback, return 1 to continue playback.
typedef bool (*pwm_finish_fn)(void);
void pwm_finish_register(pwm_finish_fn callback);

// Background square playback functions.
typedef struct note {
	uint16_t freq, len;
} note;
void sw_play(const note *notes, int size);
void sw_wait(void);
void sw_stop(void);
extern volatile uint32_t note_num;
extern volatile uint32_t sw_elapsed;
extern volatile bool sw_playing; // Mutate at your own peril.

typedef void (*sw_next_note_fn)(uint32_t note_num, const note *note);
void sw_next_note_register(sw_next_note_fn callback);

// USB access functions.
bool usb_ms_init(void);

// Orbit booster pack.
void initialize_orbit_button(uint32_t base, uint8_t pin);
void initialize_tiva_button(uint32_t base, uint8_t pin);
#define initialize_orbit_BTN1() initialize_orbit_button(GPIO_PORTD_BASE, GPIO_PIN_2)
#define initialize_orbit_BTN2() initialize_orbit_button(GPIO_PORTE_BASE, GPIO_PIN_0)
#define initialize_tiva_SW1() initialize_tiva_button(GPIO_PORTF_BASE, GPIO_PIN_4)
#define initialize_tiva_SW2() initialize_tiva_button(GPIO_PORTF_BASE, GPIO_PIN_0)
#define read_orbit_BTN1() (ROM_GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) == GPIO_PIN_2)
#define read_orbit_BTN2() (ROM_GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0) == GPIO_PIN_0)
#define read_tiva_SW1() (ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) != GPIO_PIN_4)
#define read_tiva_SW2() (ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) != GPIO_PIN_0)

// Song data structures.
typedef struct lyric_line {
	int position;
	const char *line;
} lyric_line;

typedef struct sw_song {
	const struct note *notes;
	int notes_len;
	const struct lyric_line *lyrics;
	int lyrics_len;
} sw_song;

typedef struct pcm_fragment {
	const unsigned char *pcm;
	int size;
	int sample_rate;
} pcm_fragment;

// Menu structures.
#define MENU_TYPE_MASK		0x0000000F
#define MENU_TYPE_SW_SONG	0x00000001
#define MENU_TYPE_PCM_SONG	0x00000002
#define MENU_CHILD_IS_FUNC 	0x00010000

struct menu_item;
typedef struct menu_item (*get_child_menu_func)(struct menu_item *self, int *length);
typedef void (*menu_item_play_func)(struct menu_item *data);
typedef struct menu_item {
	const char *name;
	void *data;
	uint32_t modifiers;
	const struct menu_item *children;
	uint32_t child_count;
} menu_item;

int scroll_text(char *buffer, int size, const char *text, int len, int shift);
void show_menu(const menu_item *menu, int size, const char *title);

// Playing.
void play_sw_song(const sw_song *song, const char *title);
void play_pcm_fragment(const pcm_fragment *data, const char *title);

// More sane alternatives to Gene Apperson.
#define OrbitOledSetRC(row, column) OrbitOledSetCursor(column, row)
#define OrbitOledGetRC(row, column) OrbitOledGetCursor(column, row)
#define OrbitOledMoveRC(row, column) OrbitOledMoveTo(column, row)
#define OrbitOledFillRectRC(row, column) OrbitOledFillRect(column, row)

#endif /* PLAYER_H_ */
