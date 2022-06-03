#include <xwlan.h>

#include <stddef.h>

#include <private/xwlanprivate.h>

#include <clog/clog.h>

void xwlan_log(uint8_t log_level) {
	if (log_level) {
		clog_enable();
	} else {
		clog_disable();
	}
}

xwlan_error_status xwlan_check_context(xwlan_context* ctx) {
	if (ctx == NULL) {
		return XWLAN_ERROR_CONTEXT_NULL;
	}
	return ctx->last_error;
}

xwlan_context* xwlan_create_context() {
	xwlan_context* xwlan_ctx = (xwlan_context*)malloc(sizeof(xwlan_context));
	xwlan_ctx->last_error = XWLAN_OK;
	if (xwlan_ctx == NULL) {
		return xwlan_ctx;
	}
	xwlan_ctx->native_context = xwlan_create_context_native(xwlan_ctx);
	xwlan_ctx->last_error = xwlan_ctx->native_context->last_error;
	if (xwlan_ctx->last_error != XWLAN_OK) {
		clog_i(__XWLAN_NATIVE_WIFI,
		       "xwlan_create_context_native failed with error: %p",
		       xwlan_ctx->last_error);
	}
	return xwlan_ctx;
}

void xwlan_destroy_context(xwlan_context* xwlan_ctx) {
	xwlan_error_status err = xwlan_destroy_context_native(xwlan_ctx);
	if (err != XWLAN_OK) {
		// native context destroy failed, check log for reasons
	}
	free(xwlan_ctx);
}

xwlan_permissions xwlan_get_permission(xwlan_context* xwlan_ctx,
                                       xwlan_permissions required_permissions) {
	return XWLAN_PERMISSION_NONE;
}

uint8_t xwlan_check_permission(xwlan_context* xwlan_ctx,
                               xwlan_permissions permissions,
                               xwlan_permissions check_what) {
	return 1;
}
