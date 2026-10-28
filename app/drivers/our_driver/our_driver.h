/* our_driver public API: custom extensions to the sensor API
 * 2026-10-28 */

#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Allowed range for our_driver_set_counter(), shared with the shell
 * so both validate against the same limits. */
#define OUR_DRIVER_COUNTER_MIN 0
#define OUR_DRIVER_COUNTER_MAX 100

/* Custom extension to the standard sensor API.
 * Increments the internal toggle counter kept in our_driver_data
 * and returns the new count. */
int our_driver_bump_counter(const struct device *dev);

/* Sets the internal toggle counter to an explicit value.
 * Returns 0 on success, -EINVAL if value is out of the allowed range. */
int our_driver_set_counter(const struct device *dev, int value);

#ifdef __cplusplus
}
#endif

#endif
