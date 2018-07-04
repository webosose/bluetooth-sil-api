#!/bin/bash

# Test script for testing SILs with sil-tester (and mock SIL by default).
# To run on a desktop build, copy to BUILD folder.

# Device IO capabilities used for pairing tests:
#
# NoInputNoOutput  BLUETOOTH_PAIRING_IO_CAPABILITY_NO_INPUT_NO_OUTPUT,
# DisplayOnly      BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_ONLY,
# DisplayYesNo     BLUETOOTH_PAIRING_IO_CAPABILITY_DISPLAY_YES_NO,
# KeyboardOnly     BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_ONLY,
# KeyboardDisplay  BLUETOOTH_PAIRING_IO_CAPABILITY_KEYBOARD_DISPLAY,
#
# We need to test with KeyboardOnly and DisplayYesNo to cover the four possible responses for all
# incoming and outgoing tests: displayPasskeyPin, enterPasskeyPin, confirmPasskey, and AutoConfirm

# Enable one of the following to restrict tests to a specific of available pairing (or other) tests:
#TESTS=""
#TESTS="-p /SIL/Adapter"
TESTS="-p /SIL/Adapter/Properties"
#TESTS="-p /SIL/Adapter/Discovery"
#TESTS="-p /SIL/Adapter/Pairing/Passkey"
#TESTS="-p /SIL/Adapter/Pairing/Passkey/Outgoing"
#TESTS="-p /SIL/Adapter/Pairing/Passkey/Incoming"
#TESTS="-p /SIL/Adapter/Pairing/Pin"
#TESTS="-p /SIL/Adapter/Pairing/Pin/Outgoing"
#TESTS="-p /SIL/Adapter/Pairing/Pin/Incoming"
#TESTS="-p /SIL/Adapter/Pairing/Device/Outgoing"

# Allow (narrower) test path to be specified on the command line:
[ -n "${1}" ] && TESTS="-p ${1}"

# SIL can be set in environment (or command line) to override default
# e.g. "SIL=/path/to/mock.so ./bluetooth-sil-tester ..."
[ -n "${SIL}" ] || SIL="../../bluetooth-sil-mock/BUILD/mock.so"
[ ! -e "${SIL}" ] && echo "ERROR: Can't find SIL at '${SIL}'" && exit 1

# Figure out where sil-tester is (current dir, BUILD dir, or in PATH)
[ -x src/bluetooth-sil-tester ] && TESTER="src/bluetooth-sil-tester"
[ -x ../BUILD/src/bluetooth-sil-tester ] && TESTER="../BUILD/src/bluetooth-sil-tester"
[ -z "${TESTER}" ] && [ -n "`which bluetooth-sil-tester`" ] && TESTER="bluetooth-sil-tester"
[ -z "${TESTER}" ] && echo "ERROR: Can't find bluetooth-sil-tester" && exit 1

# Test DisplayYesNo and KeyboardOnly
for DEVICE in DisplayYesNo KeyboardOnly ; do
	echo
	echo Testing webOS $(basename ${SIL}) Bluetooth SIL as a ${DEVICE} device.
	echo
	echo "The following tests will be executed:"
	${TESTER} -l ${TESTS} ${SIL} ${DEVICE} 2>/dev/null
	echo
	set -x
	${TESTER} ${TESTS} ${SIL} ${DEVICE}
	set +x
done
