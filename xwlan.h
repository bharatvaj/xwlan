#ifndef __XWLAN_H
#define __XWLAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Forward declaration, this is for internal use */
typedef struct xwlan_context_native xwlan_context_native;

enum return_codes {
	/* Cannot allocate memory */
	XWLAN_NO_MEMORY = 1
};

typedef enum xwlan_interface_state {
	XWLAN_NOT_READY,
	XWLAN_CONNECTED,
	XWLAN_AD_HOC_NETWORK_FORMED,
	XWLAN_DISCONNECTING,
	XWLAN_DISCONNECTED,
	XWLAN_ASSOCIATING,
	XWLAN_DISCOVERING,
	XWLAN_AUTHENTICATING,
	XWLAN_UNKNOWN
} xwlan_interface_state;

typedef struct xwlan_interface {
	size_t index;
	char* name;
	size_t name_len;
	char* guid;
	size_t guid_len;
	xwlan_interface_state state;
} xwlan_interface;

/* Bit shifted values that correspond to different permissions */
typedef enum xwlan_permissions {
	XWLAN_PERMISSION_ALL,
	XWLAN_PERMISSION_NONE,
	XWLAN_PERMISSION_INTERFACE_NAME
} xwlan_permissions;

/* Learn from CURL, for better API */
typedef enum xwlan_error_status {
	XWLAN_OK,
	XWLAN_ERROR,
	XWLAN_ERROR_CONTEXT_NULL,
	XWLAN_ERROR_UNEXPECTED,
	XWLAN_ERROR_NOT_ENOUGH_MEMORY,
} xwlan_error_status;

typedef struct xwlan_context {
	xwlan_permissions granted_permissions;
	xwlan_permissions denied_permissions;
	xwlan_error_status last_error;
	/* Internal use only */
	struct xwlan_context_native* native_context;
} xwlan_context;

/*
 * log_level
 * 0 will disable logging
 * 1 will enable logging
 */
void xwlan_log_level(uint8_t log_level);

/*
 * Returns non-null `xwlan_context*` if successful
 * and returns NULL on failure. NULL probably means there is no memory,
 * you should panic and exit your program.
 */
xwlan_context* xwlan_create_context();

void xwlan_destroy_context(xwlan_context* xwlan_ctx);

xwlan_error_status xwlan_check_context(xwlan_context* xwlan_ctx);

/*
 * Checks whether the user has sufficient permission
 * for the given permission list.
 * `xwlan_ctx` should not be NULL
 * `required_permissions` should be bit stuffed with
 * permission you require
 * Returns permissions that were denied, if denied_permssion == XWLAN_PERMISSION_NONE,
 * all permssion were granted.
 * check manually for every permission before using an interface.
 * if denied_permssion != XWLAN_PERMISSION_INTERFACE_NAME
 *  you can query the interfaces.
 */
xwlan_permissions xwlan_get_permission(xwlan_context* xwlan_ctx,
                                       xwlan_permissions required_permissions);

uint8_t xwlan_check_permission(xwlan_context* xwlan_ctx,
                               xwlan_permissions permissions,
                               xwlan_permissions check_what);

/**
 * Returns a list of interfaces to enumerat
 */
xwlan_error_status
xwlan_enumerate_interfaces(xwlan_context* xwlan_ctx,
                           xwlan_interface*** xwlan_interfaces,
                           size_t* xwlan_interfaces_len);

#ifdef __cplusplus
}
#endif

#endif // __XWLAN_H
