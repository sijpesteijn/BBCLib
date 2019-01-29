/*
 * pwm.h
 *
 *  Created on: Jun 7, 2015
 *      Author: gijs
 */

#ifndef PWM_H_
#define PWM_H_

#define PWM_PATH "/sys/devices/ocp.3/"
typedef struct {
	char *name;
} pwm_properties;


extern int pwm_open(pwm_properties *pwm);
extern int pwm_set_run(pwm_properties *pwm, int run);
extern int pwm_set_period(pwm_properties *pwm, int period);
extern int pwm_set_duty(pwm_properties *pwm, int duty);

#endif /* PWM_H_ */
