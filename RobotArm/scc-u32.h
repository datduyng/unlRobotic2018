#ifndef SCC_DRIVER_H   
#define SCC_DRIVER_H

struct as_t{
	uint16_t azimuth;
	uint16_t shoulder;
	uint16_t elbow;
	uint16_t wrist;
}

uint8_t asconversion( joint_t *js, as_t *as );

uint8_t js_constain (joint_t *js);

float fconstrain( float input, float min, float max);

uint8_t setConfig ( as_t *as );
#endif