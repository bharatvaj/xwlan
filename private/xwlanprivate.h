#ifndef __XWLAN_PRIVATE_H
#define __XWLAN_PRIVATE_H

#include <xwlan.h>
#ifdef __XWLAN_ENABLE_NATIVE_WIFI__
#include <private/nativewifi.h>
#endif

xwlan_context_native*
xwlan_create_context_native(const xwlan_context* xwlan_ctx);

/*
 * Cleans up wifi interfaces structs and frees up
 * xwlan_ctx->native_context
 * Returns an explicit `xwlan_error_status`
 */
xwlan_error_status xwlan_destroy_context_native(const xwlan_context* xwlan_ctx);

#endif
