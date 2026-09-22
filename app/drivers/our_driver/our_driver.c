#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios);


static int sample_fetch_my_imp(const struct device *dev, enum sensor_channel chan)
{
        gpio_pin_set_dt(&led, 1);   // LED on
	LOG_INF("Hello from sample fetch, channel %d", chan);
	LOG_INF("LED on");
        return 0;
}

static int channel_get_my_imp(const struct device *dev,
				enum sensor_channel chan,
				struct sensor_value *val){
	gpio_pin_set_dt(&led, 0);  // LED off
	LOG_INF("Hello from channel Get, channel %d", chan);
	LOG_INF("LED off");
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

        if (!gpio_is_ready_dt(&led)) {
                LOG_ERR("LED GPIO not ready");
                return -ENODEV;
        }

        ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
                LOG_ERR("Failed to configure LED GPIO: %d", ret);
                return ret;
        }

	LOG_INF("Device initialized!");

	return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_nalonso);
