#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>

#define TEST_FILE "/tmp/pam_fdkey_test"
#define TEST_STRING "unlock:key"

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    FILE *file;
    char line[256];
    const char *prompt = "Create authentication file and press enter.";
    char *response = NULL;

    // Display custom prompt
    if (pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &response, "%s", prompt) != PAM_SUCCESS || response == NULL) {
        // Error or user just pressed enter
        return PAM_IGNORE;
    }

    // Free response after use
    if (response != NULL) {
        free(response);
    }

    // Open the file and check for the string
    file = fopen(TEST_FILE, "r");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, TEST_STRING) != NULL) {
                fclose(file);
                return PAM_SUCCESS;
            }
        }
        fclose(file);
    }

    // The file is not found or does not contain the string
    pam_info(pamh, "Authentication file not found.");
    return PAM_IGNORE;
}


extern int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc,
						  const char **argv) {
	return PAM_SUCCESS;
}

#ifdef PAM_STATIC
struct pam_module _pam_fdkey_modstruct = {
	"pam_fdkey", pam_sm_authenticate, pam_sm_setcred, NULL, NULL, NULL, NULL,
};
#endif