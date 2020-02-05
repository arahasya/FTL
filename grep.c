/* Pi-hole: A black hole for Internet advertisements
*  (c) 2017 Pi-hole, LLC (https://pi-hole.net)
*  Network-wide ad blocking via your own hardware.
*
*  FTL Engine
*  grep-like routines
*
*  This file is copyright under the latest version of the EUPL.
*  Please see LICENSE file for your rights under this license. */

#include "FTL.h"

char ** wildcarddomains = NULL;
unsigned char blockingstatus = 2;

int countlines(const char* fname)
{
	FILE *fp;
	int ch = 0, lines = 0, chars = 0;

	if((fp = fopen(fname, "r")) == NULL) {
		return -1;
	}

	while ((ch = fgetc(fp)) != EOF)
	{
		chars++;
		if (ch=='\n')
		{
			// Add one to the lines counter
			++lines;
			// Reset chars counter
			chars = 0;
		}
	}

	// Add one more line if there were characters at the
	// last line of the file even without a final "\n"
	if(chars > 0)
		++lines;

	// Close the file
	fclose(fp);

	return lines;
}

int countlineswith(const char* str, const char* fname)
{
	FILE *fp;
	int found = 0;
	char *buffer = NULL;
	size_t size = 0;

	if((fp = fopen(fname, "r")) == NULL) {
		return -1;
	}

	// Search through file
	// getline reads a string from the specified file up to either a
	// newline character or EOF
	while(getline(&buffer, &size, fp) != -1)
	{
		// Strip potential newline character at the end of line we just read
		if(buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';

		// Search for exact match
		if(strcmp(buffer, str) == 0)
		{
			found++;
			continue;
		}

		// If line starts with *, search for partial match of
		// needle "buffer+1" in haystack "str"
		if(buffer[0] == '*')
		{
			char * buf = strstr(str, buffer+1);
			// The  strstr() function finds the first occurrence of
			// the substring buffer+1 in the string str.
			// These functions return a pointer to the beginning of
			// the located substring, or NULL if the substring is not
			// found. Hence, we compare the length of the substring to
			// the wildcard entry to rule out the possiblity that
			// there is anything behind the wildcard. This avoids that given
			// "*example.com" "example.com.xxxxx" would also match.
			if(buf != NULL && strlen(buf) == strlen(buffer+1))
				found++;
		}
	}

	// Free allocated memory
	if(buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}

	// Close the file
	fclose(fp);

	return found;
}

void check_blocking_status(void)
{
	char* blocking = read_setupVarsconf("BLOCKING_ENABLED");
	const char* message;

	if(blocking == NULL || getSetupVarsBool(blocking))
	{
		// Parameter either not present in setupVars.conf
		// or explicitly set to true
		blockingstatus = BLOCKING_ENABLED;
		message = "enabled";
		clearSetupVarsArray();
	}
	else
	{
		// Disabled
		blockingstatus = BLOCKING_DISABLED;
		message = "disabled";
	}

	logg("Blocking status is %s", message);
}
