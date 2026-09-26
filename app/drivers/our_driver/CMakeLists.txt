zephyr_include_directories(.)

if (CONFIG_OUR_DRIVER)
	target_sources(app PRIVATE our_driver.c)
endif()
