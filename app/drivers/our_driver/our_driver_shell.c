#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <errno.h>

#include "our_driver.h"

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

        const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        int ret = sensor_sample_fetch(dev);

        if (ret < 0) {
                shell_error(sh, "sensor_sample_fetch failed: %d", ret);
                return ret;
        }

        shell_print(sh, "Sample fetched.");
        return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

        const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        struct sensor_value val;
        int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

        if (ret < 0) {
                shell_error(sh, "sensor_channel_get failed: %d", ret);
                return ret;
        }

        shell_print(sh, "Channel value: %d.%06d", val.val1, val.val2);
        return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

        const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

        shell_print(sh, "Device name: %s", dev->name);
        shell_print(sh, "Ready: %s", device_is_ready(dev) ? "yes" : "no");
        return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
        int err = 0;
        long value = shell_strtol(argv[1], 10, &err);

        if (err) {
                shell_error(sh, "Invalid argument: '%s' is not a valid number", argv[1]);
                return -EINVAL;
        }

        const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        int ret = our_driver_set_counter(dev, (int)value);

        if (ret < 0) {
                shell_error(sh, "Value out of range: %ld", value);
                return ret;
        }

        shell_print(sh, "Counter set to %ld", value);
        return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
        SHELL_CMD(fetch, NULL, "Fetch a sample from the sensor.", cmd_sensor_fetch),
        SHELL_CMD(read,  NULL, "Read the last fetched channel value.", cmd_sensor_read),
        SHELL_CMD(info,  NULL, "Show device name and ready state.", cmd_sensor_info),
        SHELL_CMD_ARG(set, NULL, "Set the counter value. Usage: sensor set <value>",
                      cmd_sensor_set, 2, 0),
        SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor driver test commands", NULL);
