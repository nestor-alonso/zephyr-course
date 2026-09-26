#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Custom extension to the standard sensor API.
 * Increments the internal toggle counter kept in our_driver_data
 * and returns the new count. */
int our_driver_bump_counter(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif
