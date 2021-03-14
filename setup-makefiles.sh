#!/bin/bash
#
# Copyright (C) 2018-2019 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEVICE=realme_trinket
VENDOR=realme

INITIAL_COPYRIGHT_YEAR=2019

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

OCTAVI_ROOT="${MY_DIR}/../../.."

HELPER="${OCTAVI_ROOT}/vendor/octavi/build/tools/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
    echo "Unable to find helper script at ${HELPER}"
    exit 1
fi
source "${HELPER}"

# Initialize the helper
setup_vendor "${DEVICE}" "${VENDOR}" "${OCTAVI_ROOT}"

# Copyright headers and guards
write_headers

# The standard blobs
write_makefiles "${MY_DIR}/proprietary-files.txt" true

# Finish
write_footers
