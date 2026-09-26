#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include "our_driver.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

// From l6-task1
//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios);

// For l6-task2 I must change the state model to config/data structs, since
// the custom extension function needs a *mutable* per-device struct
// (our_driver_data) to modify. The plain static variable can't be reached
// generically via dev->data, and Task 2 explicitly requires the dynamic
// data struct pattern.

/* Immutable, from devicetree */
struct our_driver_config {
        struct gpio_dt_spec led;
};

/* Mutable runtime state */
struct our_driver_data {
        int toggle_count;
};


static int sample_fetch_my_imp(const struct device *dev, enum sensor_channel chan)
{
        const struct our_driver_config *cfg = dev->config;

        gpio_pin_set_dt(&cfg->led, 1);   // LED on
	LOG_INF("Hello from sample fetch, channel %d", chan);
	LOG_INF("LED on");
        return 0;
}

static int channel_get_my_imp(const struct device *dev,
				enum sensor_channel chan,
				struct sensor_value *val){

        const struct our_driver_config *cfg = dev->config;

        gpio_pin_set_dt(&cfg->led, 0);   // LED off

	LOG_INF("Hello from channel Get, channel %d", chan);
	LOG_INF("LED off");
	return 0;
}


/* Custom extension API: added by me to the standard sensor interface */
int our_driver_bump_counter(const struct device *dev)
{
        struct our_driver_data *data = dev->data;

        data->toggle_count++;
        LOG_INF("Toggle count now %d", data->toggle_count);
        return data->toggle_count;
}


/* Another custom extension, as the previous one only autoincrements, but can't set values*/
#define OUR_DRIVER_COUNTER_MIN 0
#define OUR_DRIVER_COUNTER_MAX 100

int our_driver_set_counter(const struct device *dev, int value)
{
        struct our_driver_data *data = dev->data;

        if (value < OUR_DRIVER_COUNTER_MIN || value > OUR_DRIVER_COUNTER_MAX) {
                LOG_ERR("Value %d out of range [%d, %d]",
                        value, OUR_DRIVER_COUNTER_MIN, OUR_DRIVER_COUNTER_MAX);
                return -EINVAL;
        }

        data->toggle_count = value;
        LOG_INF("Counter set to %d", data->toggle_count);
        return 0;
}


// Add any other API call to this dict

static DEVICE_API(sensor, api_nalonso) = {
	.sample_fetch = sample_fetch_my_imp,
	.channel_get = channel_get_my_imp,
};

// Init fn
static int init(const struct device* dev) {
        int ret;

        const struct our_driver_config *cfg = dev->config;
        struct our_driver_data *data = dev->data;

        if (!gpio_is_ready_dt(&cfg->led)) {
                LOG_ERR("LED GPIO not ready");
                return -ENODEV;
        }

        ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
                LOG_ERR("Failed to configure LED GPIO: %d", ret);
                return ret;
        }

        data->toggle_count = 0;

	LOG_INF("Device initialized!");

	return 0;
}


static const struct our_driver_config config0 = {
        .led = GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios),
};

static struct our_driver_data data0;

// From l6-task1
//DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_nalonso);

// For l6-task2
DEVICE_DT_INST_DEFINE(0, init, NULL, &data0, &config0, POST_KERNEL, 80, &api_nalonso);
