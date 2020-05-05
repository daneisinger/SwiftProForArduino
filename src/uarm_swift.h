#ifndef _UARM_SWIFT_H_
#define _UARM_SWIFT_H_


#include "uarm_common.h"
#include "uarm_grove_manage.h"

#if defined(UARM_MINI)
	#define DEVICE_NAME						"SwiftPro mini"
#elif	defined(UARM_2500)
	#define DEVICE_NAME						"uArm3Plus"
#else
	#define DEVICE_NAME						"SwiftPro"
#endif

#define HARDWARE_VERSION			hardware_version
#define SOFTWARE_VERSION			"V4.5.1"
#define API_VERSION						"V4.0.4"
#define BLE_UUID							bt_mac_addr


void uarm_swift_init(void);
void uarm_swift_tick_run(void);


#endif
