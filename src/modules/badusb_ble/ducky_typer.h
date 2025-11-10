#ifndef __DUCKY_TYPER_H
#define __DUCKY_TYPER_H
#include <Arduino.h>
#include <SD.h>
#include <USB.h>
#include <globals.h>

#ifdef USB_as_HID
#include <USBHIDKeyboard.h>
#else
// #include <CH9329_Keyboard.h>  // Commented out - library not available
// Placeholder for CH9329 compatibility when USB_as_HID is not defined
#endif
// #include <BleKeyboard.h>  // Temporarily commented out due to compilation issues

// extern void* hid_usb;  // Commented out - HIDInterface not available
// extern void* hid_ble;  // Commented out - HIDInterface not available
extern uint8_t _Ask_for_restart;
// Start badUSB or badBLE ducky runner (simplified)
void ducky_setup(void *hid, bool ble = false);

// Setup the keyboard for badUSB or badBLE (simplified)
void ducky_startKb(void *hid, bool ble);

// Parses a file to run in the badUSB (simplified)
void key_input(FS fs, String bad_script, void *hid);

// Sends a simple command through USB
void key_input_from_string(String text);

// Use device as a keyboard (USB or BLE) (simplified)
void ducky_keyboard(void *hid, bool ble = false);

// Send media commands through BLE or USB HID (simplified)
void MediaCommands(void *hid, bool ble = false);

#endif
