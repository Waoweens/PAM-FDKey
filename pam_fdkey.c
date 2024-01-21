#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <sys/syslog.h>

#define TEST_FILE "/tmp/pam_fdkey_test"
#define TEST_STRING "unlock:key"

extern int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	FILE *file;
	char line[256];

	file = fopen(TEST_FILE, "r");
	if (file == NULL) {
		pam_syslog(pamh, LOG_ERR, "Failed to open test file");
		return PAM_AUTH_ERR;
	}

	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, TEST_STRING) !=NULL) {
			fclose(file);
			return PAM_SUCCESS;
		}
	}

	fclose(file);
	return PAM_AUTH_ERR;
}

extern int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	return PAM_SUCCESS;
}

#ifdef PAM_STATIC
struct pam_module _pam_fdkey_modstruct = {
	"pam_fdkey",
	pam_sm_authenticate,
	pam_sm_setcred,
	NULL,
	NULL,
	NULL,
	NULL,
};
#endif