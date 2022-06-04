#include <xwlan.h>

#include <stddef.h>
#include <stdio.h>

/*
 * Enumerate wlan interfaces
 */

int main(int argc, char* argv[]) {
	xwlan_log_level(1);
	xwlan_context* ctx = xwlan_create_context();
	xwlan_error_status err = xwlan_check_context(ctx);
	if (err != XWLAN_OK) {
		printf("Failed to create context\n");
		return err;
	}
	xwlan_permissions required_permissions = XWLAN_PERMISSION_ALL;
	xwlan_permissions denied_permissions =
	        xwlan_get_permission(ctx, required_permissions);
	if (xwlan_check_permission(
	            ctx, denied_permissions, XWLAN_PERMISSION_NONE) != 1) {
		printf("You don't have the permissions query wlan interface\n");
		return XWLAN_PERMISSION_INTERFACE_NAME;
	}
	xwlan_interface** wlan_ifs = NULL;
	size_t interfaces_len = -1;
	err = xwlan_enumerate_interfaces(ctx, &wlan_ifs, &interfaces_len);
	if (err != XWLAN_OK) {
		printf("Failed to enumerate devices\n");
		return err;
	}
	printf("Interfaces: %zd: \n", interfaces_len);
	for (size_t i = 0; i < interfaces_len; i++) {
		xwlan_interface* wlan_if = wlan_ifs[i];
		if (wlan_if == NULL) {
			printf("ERROR: One of the enumerated wlan interface is null, "
			       "disconnected?\n");
			continue;
		}
		printf("Interface[%zd]: \n", wlan_if->index);
		printf("Interface Name: %s\n", wlan_if->name);
		switch (wlan_if->state) {
		case XWLAN_NOT_READY:
			printf("Not ready");
			break;
		case XWLAN_CONNECTED:
			printf("Connected");
			break;
		case XWLAN_AD_HOC_NETWORK_FORMED:
			printf("First node in a ad hoc network");
			break;
		case XWLAN_DISCONNECTING:
			printf("Disconnecting");
			break;
		case XWLAN_DISCONNECTED:
			printf("Not connected");
			break;
		case XWLAN_ASSOCIATING:
			printf("Attempting to associate with a network");
			break;
		case XWLAN_DISCOVERING:
			printf("Auto configuration is discovering settings for the network");
			break;
		case XWLAN_AUTHENTICATING:
			printf("In process of authenticating");
			break;
		default:
			printf("Unknown state %ld", wlan_if->state);
			break;
		}
		printf("\n");
	}
	return 0;
}
