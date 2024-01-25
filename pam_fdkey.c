#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>
#include <dbus/dbus.h>
#include <pwd.h>
#include <sys/types.h>

#define INIT_PROMPT "Insert USB device containing key and press enter."
#define RETRY_PROMPT "Key not found. Retry? [(Y)es/(N)o/(P)assword]"

#define FALLBACK_MSG "Falling back to password authentication."

extern int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	// initialize D-Bus

	DBusConnection* conn;
	DBusError err;
	DBusMessage *msg, *reply;
	DBusMessageIter args;
	DBusPendingCall* pending;

	dbus_error_init(&err);

	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		pam_error(pamh, "D-Bus connection error (%s)", err.message);
		dbus_error_free(&err);

		pam_info(pamh, FALLBACK_MSG);
		return PAM_AUTH_ERR;
	}

	// get the username
	const char* username;
	int pam_user;

	pam_user = pam_get_user(pamh, &username, NULL);
	if (pam_user != PAM_SUCCESS) {
		pam_error(pamh, "Failed to get username: (%s)", pam_user);
		return PAM_USER_UNKNOWN;
	}

	// get the uid
	unsigned int uid;

	struct passwd* pwd = getpwnam(username);
	if (pwd == NULL) {
		pam_error(pamh, "Failed to get uid for username %s", username);
		return PAM_USER_UNKNOWN;
	}

	uid = pwd->pw_uid;

	pam_info(pamh, "uid: %d", uid);

	// call D-Bus method PubKeyAuth(uid)
	// in interface net.meowcorp.FDKey.Authenticate
	// in object net.meowcorp.FDKey.Authentication
	// in service net.meowcorp.FDKey

	char* service = "net.meowcorp.FDKey";
	char* object = "/net/meowcorp/FDKey/Authentication";
	char* interface = "net.meowcorp.FDKey.Authenticate";
	char* method = "PubKeyAuth";

	msg = dbus_message_new_method_call(service, object, interface, method);
	if (msg == NULL) {
		pam_error(pamh, "Failed to create D-Bus message");
		return PAM_AUTH_ERR;
	}

	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &uid)) {
		pam_error(pamh, "Failed to append uid to D-Bus message");
		return PAM_AUTH_ERR;
	}

	reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
	dbus_message_unref(msg);
	if (!reply) {
		pam_error(pamh, "D-Bus call failed (%s)", err.message);
		dbus_error_free(&err);
		return PAM_AUTH_ERR;
	}

	// default
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