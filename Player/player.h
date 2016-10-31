#ifndef PLAYER_H_
#define PLAYER_H_

extern uint32_t clock_rate;

// Background PWM playback functions.
void pwm_setup(void);
void pwm_play(const unsigned char *pcm, int size, int sample_rate);
void pwm_wait(void);
extern bool pwm_playing; // Mutate at your own peril.

#endif /* PLAYER_H_ */
