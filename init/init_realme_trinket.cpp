/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>
#include <string>
#include <fstream>

#include <android-base/logging.h>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

#include "vendor_init.h"

using ::android::base::SetProperty;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_multifp(char const buildfp[], char const systemfp[],
	char const bootimagefp[], char const vendorfp[], char const value[])
{
	property_override(buildfp, value);
	property_override(systemfp, value);
	property_override(bootimagefp, value);
	property_override(vendorfp, value);
}

void load_RMX1911() {
    property_override("ro.product.model", "Realme 5");
    property_override("ro.build.product", "RMX1911");
    property_override("ro.product.device", "RMX1911");
    property_override("ro.build.description", "redfin-user 11 RQ2A.210305.006 7119741 release-keys");
}

void load_RMX1925() {
    property_override("ro.product.model", "Realme 5s");
    property_override("ro.build.product", "RMX1925");
    property_override("ro.product.device", "RMX1925");
    property_override("ro.build.description", "coral-user 11 RP1A.201005.004 6782484 release-keys");
}

void load_RMX1927() {
    property_override("ro.product.model", "Realme 5 NFC");
    property_override("ro.build.product", "RMX1927");
    property_override("ro.product.device", "RMX1927");
    property_override("persist.sys.nfc.disPowerSave", "1");
    property_override("persist.sys.nfc.default", "on");
    property_override("persist.sys.nfc.aid_overflow", "true");
    property_override("ro.product.cuptsm", "OPPO|ESE|01|02");
    property_override("persist.sys.nfc.antenna_area", "bottom");
    property_override("ro.build.description", "redfin-user 11 RQ2A.210305.006 7119741 release-keys");
}

void load_RMX2030() {
    property_override("ro.product.model", "Realme 5i");
    property_override("ro.build.product", "RMX2030");
    property_override("ro.product.device", "RMX2030");
    property_override("ro.build.description", "redfin-user 11 RQ2A.210305.006 7119741 release-keys");
}

void load_dalvikvm_properties()
{
    struct sysinfo sys;

    sysinfo(&sys);
    if (sys.totalram < 3072ull * 1024 * 1024) {
        // 4GB RAM
        SetProperty("dalvik.vm.heapstartsize", "8m");
        SetProperty("dalvik.vm.heaptargetutilization", "0.6");
        SetProperty("dalvik.vm.heapmaxfree", "16m");
    } else {
        // 3GB RAM
        SetProperty("dalvik.vm.heapstartsize", "8m");
        SetProperty("dalvik.vm.heaptargetutilization", "0.75");
        SetProperty("dalvik.vm.heapmaxfree", "8m");
    }

    SetProperty("dalvik.vm.heapgrowthlimit", "192m");
    SetProperty("dalvik.vm.heapsize", "512m");
    SetProperty("dalvik.vm.heapminfree", "512k");
}

void vendor_load_properties() {
    std::ifstream infile("/proc/oppoVersion/prjVersion");
    std::string prjName;
    getline(infile, prjName);
    std::ifstream fin;
    std::string buf;
    fin.open("/proc/cmdline");
    while (std::getline(fin, buf, ' '))
      if (buf.find("board_id") != std::string::npos)
          break;
    fin.close();

    if (prjName == "19631") {
        if (buf.find("S86125AA1") != std::string::npos) {
            load_RMX1925();
        } else {
            load_RMX1911();
        }
    } else if (prjName == "19632") {
        load_RMX1927();
    } else if (prjName == "19743") {
        load_RMX2030();
    } else {
        LOG(ERROR) << __func__ << ": unexcepted prjVersion!";
    }

    // fingerprint
    property_override("ro.oem_unlock_supported", "0");
    property_override_multifp("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.bootimage.build.fingerprint",
	    "ro.vendor.build.fingerprint", " google/redfin/redfin:11/RQ2A.210305.006/7119741:user/release-keys");
    property_override("ro.control_privapp_permissions", "log");
    property_override("ro.apex.updatable", "true");
    load_dalvikvm_properties();
}

