
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "usbdescriptor.h"

uint8_t receiveBuf[OUT_PACKETSIZE];
#define IN_MULT 4
uint8_t transferBuf[IN_PACKETSIZE*IN_MULT];

USBDriver* usbp = &USBD1;
uint8_t initUSB=0;
uint8_t usbStatus = 0;

/*
 * data Transmitted Callback
 */
void dataTransmitted(USBDriver *usbp, usbep_t ep){
    (void) usbp;
    (void) ep;
    //Toggle a status LED (toggles up to 1000x per second)
    palTogglePad(GPIOD, GPIOD_LED3);

    // exit on USB reset
    if(!usbStatus) return;

    // Since this is a benchmarking example, the next transfer is emitted immediately
    usbTransmit(usbp, EP_IN, transferBuf, sizeof(transferBuf));
}

/**
 * @brief   IN EP1 state.
 */
static USBInEndpointState ep1instate;

/**
 * @brief   EP1 initialization structure (IN only).
 */
static const USBEndpointConfig ep1config = {
  USB_EP_MODE_TYPE_BULK,    //Type and mode of the endpoint
  NULL,                     //Setup packet notification callback (NULL for non-control EPs)
  dataTransmitted,          //IN endpoint notification callback
  NULL,                     //OUT endpoint notification callback
  IN_PACKETSIZE,            //IN endpoint maximum packet size
  0x0000,                   //OUT endpoint maximum packet size
  &ep1instate,              //USBEndpointState associated to the IN endpoint
  NULL,                     //USBEndpointState associated to the OUTendpoint
  IN_MULT,
  NULL                      //Pointer to a buffer for setup packets (NULL for non-control EPs)
};

/*
 * data Received Callback
 * It toggles an LED based on the first received character.
 */
void dataReceived(USBDriver *usbp, usbep_t ep){
    USBOutEndpointState *osp = usbp->epc[ep]->out_state;
    (void) usbp;
    (void) ep;
    // exit on USB reset
    if(!usbStatus) return;

    if(osp->rxcnt){
        switch(receiveBuf[0]){
            case '1':
                palTogglePad(GPIOD, GPIOD_LED3);
                break;
            case '2':
                palTogglePad(GPIOD, GPIOD_LED4);
                break;
            case '3':
                palTogglePad(GPIOD, GPIOD_LED5);
                break;
            case '4':
                palTogglePad(GPIOD, GPIOD_LED6);
                break;

        }
    }

    /*
     * Initiate next receive
     */
    usbReceive(usbp, EP_OUT, receiveBuf, 64);
}

/**
 * @brief   OUT EP2 state.
 */
USBOutEndpointState ep2outstate;

/**
 * @brief   EP2 initialization structure (OUT only).
 */
static const USBEndpointConfig ep2config = {
  USB_EP_MODE_TYPE_BULK,
  NULL,
  NULL,
  dataReceived,
  0x0000,
  OUT_PACKETSIZE,
  NULL,
  &ep2outstate,
  1,
  NULL
};


/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver *usbp, usbevent_t event) {
  (void) usbp;
  switch (event) {
  case USB_EVENT_RESET:
    //with usbStatus==0 no new transfers will be initiated
    usbStatus = 0;
    palTogglePad(GPIOD, GPIOD_LED3);
    return;
  case USB_EVENT_ADDRESS:
    return;
  case USB_EVENT_CONFIGURED:

    /* Enables the endpoints specified into the configuration.
       Note, this callback is invoked from an ISR so I-Class functions
       must be used.*/
    chSysLockFromISR();
    usbInitEndpointI(usbp, 1, &ep1config);
    usbInitEndpointI(usbp, 2, &ep2config);
    chSysUnlockFromISR();
    //allow the main thread to init the transfers
    initUSB =1;
    return;
  case USB_EVENT_SUSPEND:
    return;
  case USB_EVENT_WAKEUP:
    return;
  case USB_EVENT_STALLED:
    return;
  }
  palTogglePad(GPIOD, GPIOD_LED5);
  return;
}

/*
 * Returns the USB descriptors defined in usbdescriptor.h
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
    if (dindex < 4){
      return &vcom_strings[dindex];
      }
      else{
          return &vcom_strings[4];
      }
  }
  palTogglePad(GPIOD, GPIOD_LED4);
  return NULL;
}

/*
 * Requests hook callback.
 * This hook allows to be notified of standard requests or to
 *          handle non standard requests.
 * This implementation does nothing and passes all requests to
 *     the upper layers
 */
bool requestsHook(USBDriver *usbp) {
    (void) usbp;
    return FALSE;
}

/*
 * USBconfig
 */
const USBConfig config = {
    usb_event,
    get_descriptor,
    requestsHook,
    NULL
  };

uint8_t t = 9;

int main(void) {
  halInit();
  chSysInit();

  palSetPad(GPIOD, GPIOD_LED3);
  palSetPad(GPIOD, GPIOD_LED4);
  palSetPad(GPIOD, GPIOD_LED5);
  palSetPad(GPIOD, GPIOD_LED6);

  //Start and Connect USB

  usbStart(usbp, &config);

  /* main loop
   * initiates the first transfers
   * all concurent transfers are initiated from their callbacks
   */
  while (TRUE) {
    chThdSleepMilliseconds(1000);
    palTogglePad(GPIOD, GPIOD_LED6);

    /*
     * Starts first receiving transaction
     * all further transactions are initiated by the dataReceived callback
     */

//    usbReceive(usbp, EP_OUT, receiveBuf, 64);
//    usbStartTransmitI(usbp, EP_IN, &t, 1);
    usbTransmit(usbp, EP_IN, &t, 1);

    /*
     * Starts first transfer
     * all further transactions are initiated by the dataTransmitted callback
     */
//    usbStartTransmitI(usbp, EP_IN);
    initUSB=0;
  }
}
