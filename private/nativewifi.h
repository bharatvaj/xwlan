#ifndef __XWLAN_NATIVE_WIFI
#define __XWLAN_NATIVE_WIFI __FILE__

#include <xwlan.h>

#include <windows.h>

typedef struct xwlan_context_native {
	DWORD major_version;
	DWORD minor_version;
	DWORD client_version;
	DWORD negotiated_version;
	HANDLE client_handle;
	DWORD last_ret_code;
	xwlan_error_status last_error;
} xwlan_context_native;

#endif
