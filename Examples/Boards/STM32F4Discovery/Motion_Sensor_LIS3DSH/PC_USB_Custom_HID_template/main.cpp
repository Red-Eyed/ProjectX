#include <usb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <byteswap.h>
#include <ncurses.h>

#define ID_VENDOR 0x483
#define ID_PID 0x5750

#define CONFIG 1
#define INTERFACE 0

#define EP_IN 0x81
#define EP_OUT 0x01

#define CUSTOM_HID_EPIN_SIZE 60

usb_dev_handle *open_dev(void);

int main(){
  usb_dev_handle *dev = NULL;

  usb_init();
  usb_find_busses();
  usb_find_devices();

  if (!(dev = open_dev())){
    printf("error opening device: \n%s\n", usb_strerror());
    return -1;
  }
  else{
    printf("success: device %04X:%04X opened\n", ID_VENDOR, ID_PID);
  }

  if(usb_detach_kernel_driver_np(dev, INTERFACE)){
    printf("error detaching kernel. %s\n", usb_strerror());
  }
  else{
    printf("success: kernel detached\n");
  }

  if (usb_set_configuration(dev, CONFIG) < 0){
    printf("error setting config #%d: %s\n", CONFIG, usb_strerror());
    usb_close(dev);
    return -1;
  }
  else{
    printf("success: set configuration #%d\n", CONFIG);
  }

  if (usb_claim_interface(dev, 0) < 0){
    printf("error claiming interface #%d:\n%s\n", INTERFACE, usb_strerror());
    usb_close(dev);
    return -1;
  }
  else{
    printf("success: claim_interface #%d\n", INTERFACE);
  }
  int ret=0;

  initscr();
  char USB_data[CUSTOM_HID_EPIN_SIZE];
  memset(USB_data, 0, CUSTOM_HID_EPIN_SIZE);

  int USB_data_len = 0;
  while(1){
    USB_data_len = strlen(USB_data);
    for(int i = 0; i < USB_data_len; ++i)
      printf("\b");

    ret = usb_bulk_read(dev, EP_OUT, USB_data, CUSTOM_HID_EPIN_SIZE, 0);
    if (ret < 0){
      printf("error reading:\n%s\n", usb_strerror());
      break;
    }
    else{
      printw("%s", USB_data);
      refresh();
      clear();
    }
  }

  printf("\n");

  usb_close(dev);

  return 0;
}

usb_dev_handle* open_dev(void){
  struct usb_bus *bus;
  struct usb_device *dev_local;

  for (bus = usb_get_busses(); bus; bus = bus->next){
    for (dev_local = bus->devices; dev_local; dev_local = dev_local->next){
      if (dev_local->descriptor.idVendor == ID_VENDOR
          && dev_local->descriptor.idProduct == ID_PID){
        return usb_open(dev_local);
      }
    }
  }
  return NULL;
}
