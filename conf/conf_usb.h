
#pragma once
#include <usb_protocol.h>
/* Use ASF's HID mouse device class (UDI HID MOUSE) */
#define  USB_DEVICE_VENDOR_ID             0x03EB  /* Atmel (leave or change) */
#define  USB_DEVICE_PRODUCT_ID            0x2402  /* Example PID for HID Mouse */
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         0
#define  USB_DEVICE_POWER                 100
#define  USB_DEVICE_ATTR                  (USB_CONFIG_ATTR_BUS_POWERED)

/* Strings */
#define  USB_DEVICE_MANUFACTURE_NAME      "CustomDucky"
#define  USB_DEVICE_PRODUCT_NAME          "Ducky Mouse"
#define  USB_DEVICE_SERIAL_NAME           "0001"

/* UDI (USB Device Interface) list */
#define UDI_HID_MOUSE_ENABLE_EXT()        true
#define UDI_HID_MOUSE_DISABLE_EXT()
#define UDI_HID_MOUSE_MOVE_EVENT(dx,dy,scroll)  /* not used here */
#define UDI_HID_MOUSE_BTN_EVENT(btn, state)     /* not used */

#define UDI_COMPOSITE_DESC_T     udi_hid_mouse_desc_t
#define UDI_COMPOSITE_DESCFS     udi_hid_mouse_desc
#define UDI_COMPOSITE_DESCHS     udi_hid_mouse_desc
#define UDI_COMPOSITE_API     &udi_api_hid_mouse

/* Include class header */
#include "udi_hid_mouse.h"
