/*
 * core.c
 *
 *  Created on: May 31, 2015
 *      Author: gijs
 */

#include <syslog.h>
#include "core.h"

char* slotsFilePath;
#define MAX_LENGTH 1048576

char *substring(char *string, int position, int length) {
	char *pointer;
	int c;

	pointer = malloc(length + 1);

	if (pointer == NULL) {
		printf("Unable to allocate memory.\n");
		exit(1);
	}

	for (c = 0; c < length; c++) {
		*(pointer + c) = *(string + position - 1);
		string++;
	}

	*(pointer + c) = '\0';
	return pointer;
}

int copyFile(char *srcFile, char *destinationFile) {
	char ch;
	FILE *source, *target;

	source = fopen(srcFile, "r");

	if (source == NULL) {
		syslog(LOG_INFO, "could not open source file: %s", srcFile);
		return 1;
	}

	target = fopen(destinationFile, "w");

	if (target == NULL) {
		fclose(source);
		syslog(LOG_INFO, "could not create destination file: %s", destinationFile);
		return 1;
	}

	while ((ch = fgetc(source)) != EOF)
		fputc(ch, target);

	syslog(LOG_INFO, "File copied from %s to %s.", srcFile, destinationFile);

	fclose(source);
	fclose(target);

	return 0;

}

char* getTokenValue(char* token) {
	if (token == NULL)
		return "";
	if (strcmp(token, "(null)") == 0 && strcmp(token, "\n") == 0)
		return "";
	return token;
}

int get_device_tree_overlay_count() {
	syslog(LOG_INFO, "%s", "get device tree overlay count");
	FILE *fh = fopen("/sys/devices/bone_capemgr.9/slots", "r");

	if (fh == NULL) {
		printf("failed to openfile\n");
		exit(1);
	}
	int nrOfOverlays = 0;
	char buf[1024];
	while (fgets(buf, sizeof buf, fh) != NULL) {
		nrOfOverlays++;
	}
	return nrOfOverlays;
}

int get_device_tree_overlays(overlay** overlays) {
	syslog(LOG_INFO, "%s", "get device tree overlays");
	FILE *fh = fopen("/sys/devices/bone_capemgr.9/slots", "r");

	if (fh == NULL) {
		printf("failed to openfile\n");
		exit(1);
	}
	int nrOfOverlays = 0;
	char buf[1024];
	while (fgets(buf, sizeof buf, fh) != NULL) {
		char *id, *metadata, *token;
		overlay *ol = malloc(sizeof(overlay));
		id = substring(buf, 1, 2);
		ol->id = atoi(id);
		metadata = substring(buf, 14, strlen(buf));
		token = strtok(metadata, ",");
		ol->board_name = getTokenValue(token);
		token = strtok(NULL, ",");
		ol->version = getTokenValue(token);
		token = strtok(NULL, ",");
		ol->manufacturer = getTokenValue(token);
		token = strtok(NULL, ",");
		char* partNr = getTokenValue(token);
		int len = strlen(partNr);
		if (len > 0) {
			partNr[len-1] = '\0';
			ol->part_number = partNr;
		} else {
			ol->part_number = "";
		}
		overlays[nrOfOverlays++] = ol;
	}
	fclose(fh);
	return nrOfOverlays;
}

int device_tree_overlay_equal(overlay* ol1, overlay* ol2) {
	syslog(LOG_INFO, "device tree overlay equal. %s-%s %s-%s", ol1->part_number, ol1->version,
			ol2->part_number, ol2->version);
	int equals = 0;
	if (ol1 == NULL || ol2 == NULL) {
		syslog(LOG_INFO, "%s",
				"Can't compare. Either ol1 or ol2 or both are NULL");
		return 0;
	}

	if (strcmp(ol1->part_number, ol2->part_number) == 0
			&& strcmp(ol1->version, ol2->version) == 0)
		equals = 1;
	return equals;
}

int is_device_tree_overlay_loaded(overlay* ol) {
	syslog(LOG_INFO, "Is device tree overlay loaded: %s", ol->file_name);
	int len = get_device_tree_overlay_count();
	overlay** overlays = malloc(len * sizeof(overlay));
	get_device_tree_overlays(overlays);
	int i = 0;

	for (i = 0; i < len; i++) {
		if (device_tree_overlay_equal(ol, overlays[i]) == 1) {
			overlay *ol1;
			ol1 = overlays[i];
			syslog(LOG_INFO,
					"Overlay already loaded. version: %s,%s partnumber: %s,%s",
					ol->version, ol1->version, ol->part_number,
					ol1->part_number);
			return 1;
		}
	}
	return 0;
}

int load_device_tree_overlay(overlay* ol) {
	syslog(LOG_INFO, "Load device tree overlay: %s", ol->file_name);
	if (is_device_tree_overlay_loaded(ol) == 0) {
		syslog(LOG_INFO, "Loading device tree overlay: %s", ol->part_number);
		int status;
		pid_t pID = fork();
		if (pID == 0) {               // child
		   char cmd[100] = "echo ";
		   strcat(cmd, ol->file_name);
		   strcat(cmd, " > ");
		   strcat(cmd, slotsFilePath);
		   char *name[] = {
		        "/bin/bash",
		        "-c",
				cmd,
		        NULL
		    };
		   chdir("/lib/firmware/");
		   execvp(name[0], name);
		   usleep(50000);
		   exit(EXIT_SUCCESS);
		}
		wait(&status);

	} else {
		syslog(LOG_INFO, "Device tree overlay %s already loaded",
				ol->part_number);
	}
	return 0;
}

int unload_device_tree_overlay(int slot_nr) {
	pid_t pID = fork();
	if (pID == 0) {               // child
	   char cmd[100] = "echo ";
	   char nr[5];
	   sprintf(nr, "-%d", slot_nr);
	   strcat(cmd, nr);
	   strcat(cmd, " > ");
	   strcat(cmd, slotsFilePath);
	   char *name[] = {
	        "/bin/bash",
	        "-c",
			cmd,
	        NULL
	    };
	   chdir("/lib/firmware/");
	   execvp(name[0], name);
   }

	return 0;
}

int init_bbc_lib() {
	openlog("BBCLib", LOG_PID | LOG_CONS | LOG_NDELAY | LOG_NOWAIT, LOG_LOCAL0);
	syslog(LOG_INFO, "%s", "Starting BBCLib");
	return 0;
}

