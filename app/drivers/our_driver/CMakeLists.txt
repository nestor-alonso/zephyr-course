zephyr_include_directories(.)

if (CONFIG_OUR_DRIVER)
	target_sources(app PRIVATE our_driver.c)
        target_sources(app PRIVATE our_driver_shell.c)
endif()
