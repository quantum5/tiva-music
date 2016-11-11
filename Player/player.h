#ifndef PLAYER_H_
#define PLAYER_H_

extern uint32_t clock_rate;

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

#endif /* PLAYER_H_ */
