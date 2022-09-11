#include <objbase.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include <wlanapi.h>
#include <wtypes.h>

#include <private/nativewifi.h>

#include <clog/clog.h>

#define XWLAN_CTX_NATIVE(CTX) (xwlan_context_native*)CTX->native_context

/*
 * Updates error code from xwlan_context_native to wifi_context
 */
#define XWLAN_CTX_UPDATE_ERROR_CODE(CTX_NATIVE)                                \
	switch (CTX_NATIVE->last_ret_code) {                                       \
	case ERROR_NOT_ENOUGH_MEMORY:                                              \
		CTX_NATIVE->last_error = XWLAN_ERROR_NOT_ENOUGH_MEMORY;                \
		break;                                                                 \
	/* case RPC_STATUS: */                                                     \
	/*didn't handle ERROR_INVALID_PARAMETER explicitly*/                       \
	case ERROR_REMOTE_SESSION_LIMIT_EXCEEDED:                                  \
		CTX_NATIVE->last_error = XWLAN_ERROR_NOT_ENOUGH_MEMORY;                \
		break;                                                                 \
	case ERROR_SUCCESS:                                                        \
		CTX_NATIVE->last_error = XWLAN_OK;                                     \
		break;                                                                 \
	}

xwlan_context_native*
xwlan_create_context_native(const xwlan_context* xwlan_ctx) {
	xwlan_context_native* ctx_native =
	        (xwlan_context_native*)malloc(sizeof(xwlan_context_native));
	ctx_native->major_version = WLAN_API_VERSION_MAJOR(2);
	ctx_native->minor_version = WLAN_API_VERSION_MINOR(0);
	ctx_native->client_version = WLAN_API_MAKE_VERSION(
	        ctx_native->major_version, ctx_native->minor_version);
	/* ctx_native->client_handle = NULL; */
	ctx_native->last_ret_code = WlanOpenHandle(ctx_native->client_version,
	                                           NULL,
	                                           &ctx_native->negotiated_version,
	                                           &ctx_native->client_handle);
	XWLAN_CTX_UPDATE_ERROR_CODE(ctx_native);
	return ctx_native;
}

xwlan_error_status
xwlan_destroy_context_native(const xwlan_context* xwlan_ctx) {
	xwlan_context_native* ctx_native = XWLAN_CTX_NATIVE(xwlan_ctx);
	if (ctx_native->client_handle != NULL) {
		DWORD ret_code = WlanCloseHandle(ctx_native->client_handle, NULL);
		if (ret_code == ERROR_INVALID_HANDLE) {
			// if we have an invalid handle which is not NULL
			// it's probably a corrupted memory
			return XWLAN_ERROR_UNEXPECTED;
		}
	}
	free(ctx_native);
}

xwlan_error_status
xwlan_enumerate_interfaces(xwlan_context* xwlan_ctx,
                           xwlan_interface*** xwlan_interfaces,
                           size_t* xwlan_interfaces_len) {
	xwlan_context_native* ctx_native = XWLAN_CTX_NATIVE(xwlan_ctx);
	PWLAN_INTERFACE_INFO_LIST p_if_list = NULL;
	PWLAN_INTERFACE_INFO p_if_info = NULL;
	WCHAR GuidString[40] = {0};
	int iRet = 0;
	int i = 0;
	ctx_native->last_ret_code =
	        WlanEnumInterfaces(ctx_native->client_handle, NULL, &p_if_list);
	XWLAN_CTX_UPDATE_ERROR_CODE(ctx_native);
	*xwlan_interfaces_len = p_if_list->dwNumberOfItems;
	*xwlan_interfaces =
	        malloc(sizeof(xwlan_interface*) * (*xwlan_interfaces_len));
	/* clog_w(__XWLAN_NATIVE_WIFI, "Current Index: %lu\n", p_if_list->dwIndex); */
	for (i = 0; i < *xwlan_interfaces_len; i++) {
		p_if_info = (WLAN_INTERFACE_INFO*)&p_if_list->InterfaceInfo[i];
		xwlan_interface* xwlan_if =
		        (xwlan_interface*)malloc(sizeof(xwlan_interface));
		xwlan_if->index = i;
#ifdef __cplusplus
		iRet = StringFromGUID2(
		        p_if_info->InterfaceGuid, (LPOLESTR)&GuidString, 39);
#else
		iRet = StringFromGUID2(
		        &p_if_info->InterfaceGuid, (LPOLESTR)&GuidString, 39);
#endif
		if (iRet == 0) {
			clog_w(__XWLAN_NATIVE_WIFI, "StringFromGUID2 failed");
		} else {
			/* xwlan_if->guid = GuidString; */
			xwlan_if->guid_len = wcslen(GuidString);
		}
		xwlan_if->name_len = wcslen(p_if_info->strInterfaceDescription);
		xwlan_if->name = malloc(sizeof(char) * xwlan_if->name_len);
		wcstombs(xwlan_if->name,
		         p_if_info->strInterfaceDescription,
		         xwlan_if->name_len);
		xwlan_interface_state interface_state;
		switch (p_if_info->isState) {
		case wlan_interface_state_not_ready:
			interface_state = XWLAN_NOT_READY;
			break;
		case wlan_interface_state_connected:
			interface_state = XWLAN_CONNECTED;
			break;
		case wlan_interface_state_ad_hoc_network_formed:
			interface_state = XWLAN_AD_HOC_NETWORK_FORMED;
			break;
		case wlan_interface_state_disconnecting:
			interface_state = XWLAN_DISCONNECTING;
			break;
		case wlan_interface_state_disconnected:
			interface_state = XWLAN_DISCONNECTED;
			break;
		case wlan_interface_state_associating:
			interface_state = XWLAN_ASSOCIATING;
			break;
		case wlan_interface_state_discovering:
			interface_state = XWLAN_DISCOVERING;
			break;
		case wlan_interface_state_authenticating:
			interface_state = XWLAN_AUTHENTICATING;
			break;
		default:
			interface_state = XWLAN_UNKNOWN;
			break;
		}
		xwlan_if->state = interface_state;
		(*xwlan_interfaces)[i] = xwlan_if;
	}

	if (p_if_list != NULL) {
		WlanFreeMemory(p_if_list);
		p_if_list = NULL;
	}
	return xwlan_ctx->last_error;
}
