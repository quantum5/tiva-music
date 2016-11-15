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

extern bool pwm_playing; // Mutate at your own peril.

// Low level PWM API, unstable interface.
extern const unsigned char *pwm_sample;
extern const unsigned char *pwm_sample_end;

// PWM finish callback, return 1 to continue playback.
typedef bool (*pwm_finish_fn)(void);
void pwm_finish_register(pwm_finish_fn callback);

// USB access functions.
bool usb_ms_init(void);

// Menu structures.
struct menu_item;
typedef struct menu_item (*get_child_menu_func)(struct menu_item *self, int *length);
typedef void (*menu_item_suicide_func)(struct menu_item *self);
typedef void (*menu_item_play_func)(struct menu_item *data);
typedef struct menu_item {
	const char *name;
	void *data;
	get_child_menu_func get_children;
	menu_item_suicide_func release;
} menu_item;

// More sane alternatives to Gene Apperson.
#define OrbitOledSetRC(row, column) OrbitOledSetCursor(column, row)
#define OrbitOledGetRC(row, column) OrbitOledGetCursor(column, row)

#endif /* PLAYER_H_ */
