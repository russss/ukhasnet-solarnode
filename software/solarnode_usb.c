#include "ch.h"
#include "hal.h"
#include "usb_cdc.h"
#include "shell.h"

#include "solarnode_usb.h"
#include "usbdescriptor.h"
/*
 * Don't ask me, I have no idea what is done here...
 * I think most things are either self explanatory, well documented or not to be touched
 */

/*
 * USB Driver structure.
 */
SerialUSBDriver SDU1;


/*===========================================================================*/
/* USB related stuff.                                                        */
/*===========================================================================*/




/*
 * Handles the GET_DESCRIPTOR callback. All required descriptors must be
 * handled here.
 */
static const USBDescriptor *get_descriptor(USBDriver *usbp,
                                           uint8_t dtype,
                                           uint8_t dindex,
                                           uint16_t lang) {

  (void)usbp;
  (void)lang;
  switch (dtype) {
  case USB_DESCRIPTOR_DEVICE:
    return &vcom_device_descriptor;
  case USB_DESCRIPTOR_CONFIGURATION:
    return &vcom_configuration_descriptor;
  case USB_DESCRIPTOR_STRING:
    if (dindex < 4)
      return &vcom_strings[dindex];
  }
  return NULL;
}

/**
 * @brief   IN EP1 state.
 */
static USBInEndpointState ep1instate;
static USBOutEndpointState ep1outstate;

/**
 * @brief   EP1 initialization structure (IN only).
 */
static const USBEndpointConfig ep1config = {
  USB_EP_MODE_TYPE_BULK,
  NULL,
  sduDataTransmitted,
  sduDataReceived,
  0x0040,
  0x0040,
  &ep1instate,
  &ep1outstate,
  1,
  NULL
};

/**
 * @brief   OUT EP2 state.
 */
static USBInEndpointState ep2instate;

/**
 * @brief   EP2 initialization structure (IN only).
 */
static const USBEndpointConfig ep2config = {
  USB_EP_MODE_TYPE_INTR,
  NULL,
  sduInterruptTransmitted,
  NULL,
  0x0010,
  0x0000,
  &ep2instate,
  NULL,
  1,
  NULL
};


/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver *usbp, usbevent_t event) {
    extern SerialUSBDriver SDU1;
    switch (event) {
    case USB_EVENT_RESET:
      return;
    case USB_EVENT_ADDRESS:
      return;
    case USB_EVENT_CONFIGURED:
      chSysLockFromISR();

      /* Enables the endpoints specified into the configuration.
         Note, this callback is invoked from an ISR so I-Class functions
         must be used.*/
      usbInitEndpointI(usbp, USBD1_DATA_REQUEST_EP, &ep1config);
      usbInitEndpointI(usbp, USBD1_INTERRUPT_REQUEST_EP, &ep2config);

      /* Resetting the state of the CDC subsystem.*/
      sduConfigureHookI(&SDU1);

      chSysUnlockFromISR();
      return;
    case USB_EVENT_SUSPEND:
      chSysLockFromISR();

      /* Disconnection event on suspend.*/
      sduDisconnectI(&SDU1);

      chSysUnlockFromISR();
      return;
    case USB_EVENT_WAKEUP:
      return;
    case USB_EVENT_STALLED:
      return;
    }
    return;
}

/*
* Handles the USB driver global events.
*/
static void sof_handler(USBDriver *usbp) {
    (void)usbp;

    osalSysLockFromISR();
    sduSOFHookI(&SDU1);
    osalSysUnlockFromISR();
}

/*
* USB driver configuration.
*/
const USBConfig usbcfg = {
    usb_event,
    get_descriptor,
    sduRequestsHook,
    sof_handler
};

/*
* Serial over USB driver configuration.
*/
const SerialUSBConfig serusbcfg = {
    &USBD1,
    USBD1_DATA_REQUEST_EP,
    USBD1_DATA_AVAILABLE_EP,
    USBD1_INTERRUPT_REQUEST_EP
};


void USBInit(void){
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);
}
