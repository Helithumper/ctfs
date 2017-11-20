#include <stdio.h>
#include <stdlib.h>
#include "pwnable_harness.h"

void win(void) {
	char flag[64];
	
	FILE* fp = fopen("flag.txt", "r");
	if(!fp) {
		puts("error, contact admin");
		exit(0);
	}
	
	fgets(flag, sizeof(flag), fp);
	fclose(fp);
	puts(flag);
}

void handle_connection(int sock) {
	int admin = 0;
	char buf[32];
	
	scanf("%s", buf);
	
	if(admin) {
		win();
	}
	else {
		puts("nope!");
	}
}


int main(int argc, char** argv) {
	/* Defaults: Run on port 9000 for 30 seconds as user "ctf_bof1" in a chroot */
	server_options opts = {
		.user = "ctf_bof1",
		.chrooted = true,
		.port = 9000,
		.time_limit_seconds = 30
	};
	
	return server_main(argc, argv, opts, &handle_connection);
}
