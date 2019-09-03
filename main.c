#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

static bool time_out;
static time_t input_last;
#define TMOUT_SEC	(10)

/**
 * @brief 未入力時のイベント
 * 			- rl_set_keyboard_input_timeout(int);にて設定可能
 * 			- デフォルト0.1sec
 * @return int 
 */
static int my_rl_event_hook( void )
{
	int t = time(NULL) - input_last;
	if( t >= TMOUT_SEC ) {
		time_out = true;
		rl_done = 1;
	}
	return 0;
}
/**
 * @brief 入力イベント
 * 
 * @param fp 
 * @return int 
 */
int my_rl_getc_function( FILE *fp )
{
	int c = rl_getc(fp);
	input_last = time(NULL);
	return c;
}

void main(void)
{
	char *input;
	
	time_out = false;
	
	rl_getc_function = my_rl_getc_function;
	rl_event_hook = my_rl_event_hook;
	rl_set_keyboard_input_timeout(10 * 1000);	// usec

	input_last = time(NULL);
	while(true)
	{
		input = readline("readline:");
		if( input == NULL ) break;
		if( time_out ) break;
		puts(input);
		add_history(input);
		if(input) free(input);
	}
	if(input) free(input);
	puts("end.");
}
