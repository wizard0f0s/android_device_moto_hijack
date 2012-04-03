#include <reboot/reboot.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/reboot.h>

#ifdef RECOVERY_SHELL
#include "../../../bootable/recovery/libcrecovery/common.h"
#define system __system
#endif

#ifndef REBOOT_REASON_DEFAULT
#define REBOOT_REASON_DEFAULT NULL
#endif

#ifndef BOOTMODE_CONFIG_FILE
#define BOOTMODE_CONFIG_FILE "/data/.bootmenu"
#endif

#define DBG_LEVEL 0

int reboot_wrapper(const char* reason) {

	int ret;
	int reboot_with_reason = 0;
	int need_clear_reason = 0;
	FILE * config;

	if (reason == NULL || strlen(reason) == 0 ) {

		reason = REBOOT_REASON_DEFAULT;

		system("rm -f '" BOARD_BOOTMODE_CONFIG_FILE "'");

		#if (DBG_LEVEL)
		printf("reboot: rm -f " BOARD_BOOTMODE_CONFIG_FILE "\n");
		#endif

	} else {

		// pass the reason to the kernel when we reboot
		reboot_with_reason = 1;
                need_clear_reason = 1;

		system("mkdir -p /cache/recovery");

		config = fopen(BOARD_BOOTMODE_CONFIG_FILE,"w");
		if (config == NULL) {
			fprintf(stderr, "reboot: unable to create file " BOARD_BOOTMODE_CONFIG_FILE "\n");
			reboot(RB_AUTOBOOT);
			exit(1);
		}

		// called for all reboot reasons
		if ( 0 == strncmp(reason,"bootloader",10) || 0 == strncmp(reason,"bootmenu", 8) ) {

			// override bootloader reboot mode
			ret = fputs("bootmenu", config);

			#if (DBG_LEVEL)
			printf("reboot: %s->bootmenu " BOARD_BOOTMODE_CONFIG_FILE " (%d)\n", reason, ret);
			#endif

		} else if ( 0 == strncmp(reason,"shell",5) ) {

			// override bootloader reboot mode
			ret = fputs("shell", config);

			#if (DBG_LEVEL)
			printf("reboot: %s->shell " BOARD_BOOTMODE_CONFIG_FILE " (%d)\n", reason, ret);
			#endif

		} else {

			ret = fputs(reason, config);

			#if (DBG_LEVEL)
			printf("reboot: %s " BOARD_BOOTMODE_CONFIG_FILE " (%d)\n", reason, ret);
			#endif

		}

		fflush(config);
		fclose(config);

	}

	// although we have a reason, ignore it on reboot
	if (need_clear_reason) {
		reboot_with_reason = 0;
	}

	system("sync");

	#if (DBG_LEVEL)
	system("sleep 3");
	#endif

	if (reboot_with_reason)
		ret = __reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART2, (char *)reason);
	else
		ret = reboot(RB_AUTOBOOT);

	return ret;
}
