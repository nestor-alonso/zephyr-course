#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int board_my_board_from_scratch_init(void)
{
	printf("Board initialized\n");
	return 0;
}

SYS_INIT(board_my_board_from_scratch_init, POST_KERNEL, 0);
