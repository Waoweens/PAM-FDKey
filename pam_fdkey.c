#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>

#define INIT_PROMPT "Insert USB device containing key and press enter."
#define RETRY_PROMPT "Key not found. Retry? [(Y)es/(N)o/(P)assword]"

extern int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    
    return PAM_AUTH_ERR;
}


extern int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc,
						  const char **argv) {
	return PAM_SUCCESS;
}

// get mounted flash drives


#ifdef PAM_STATIC
struct pam_module _pam_fdkey_modstruct = {
	"pam_fdkey", pam_sm_authenticate, pam_sm_setcred, NULL, NULL, NULL, NULL,
};
#endif