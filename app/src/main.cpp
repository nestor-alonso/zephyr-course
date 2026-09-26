#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/sensor.h> 
#include "our_driver.h" 


/* #define SLEEP_TIME_MS 1000 */

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)
#define APP_LED DT_ALIAS(app_led)

// From Module 3
//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
// From Module 4
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(APP_LED, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
	void test(){
		const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
		struct sensor_value val;

		sensor_sample_fetch(driver);
		k_msleep(500);
		int ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
		LOG_INF("Channel ret %d", ret);

                int count = our_driver_bump_counter(driver);
                LOG_INF("Bump counter returned %d", count);

	}
}


int main(void)
{
//    test();

/*    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
*/
    while (1) {
	test();
/*        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
*/	// From Module 3
        //k_msleep(CONFIG_LED_BLINK_SLEEP_TIME_MS);
        // From Module 4  
	k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
