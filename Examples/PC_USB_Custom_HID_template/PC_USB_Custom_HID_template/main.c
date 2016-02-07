#include <stdio.h>
#include <libusb.h>

void printdev(libusb_device *dev); //prototype of the function

int main() {
    libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
    libusb_context *ctx = NULL; //a libusb session
    int r; //for return values
    ssize_t cnt; //holding number of devices in list
    r = libusb_init(&ctx); //initialize a library session
    if(r < 0) {
        printf("Init Error "); //there was an error
                return 1;
    }
    libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0) {
        printf("Get Device Error\n"); //there was an error
    }
    printf("Devices in list.\n"); //print total number of usb devices
        ssize_t i; //for iterating through the list
    for(i = 0; i < cnt; i++) {
                printdev(devs[i]); //print specs of this device
        }
        libusb_free_device_list(devs, 1); //free the list, unref the devices in it
        libusb_exit(ctx); //close the session
        return 0;
}

void printdev(libusb_device *dev) {
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) {
        printf("failed to get device descriptor\n");
        return;
    }
    printf("Number of possible configurations: %d\n", (int)desc.bNumConfigurations);
    printf("Device Class: %d\n", (int)desc.bDeviceClass);
    printf("VendorID: %d\n", desc.idVendor);
    printf("ProductID:%d\n" ,desc.idProduct);
    struct libusb_config_descriptor *config;
    libusb_get_config_descriptor(dev, 0, &config);
    printf("Interfaces: %d ||| \n", (int)config->bNumInterfaces);
    const struct libusb_interface *inter;
    const struct libusb_interface_descriptor *interdesc;
    const struct libusb_endpoint_descriptor *epdesc;
    for(int i=0; i<(int)config->bNumInterfaces; i++) {
        inter = &config->interface[i];
        printf("Number of alternate settings: %d | \n", inter->num_altsetting);
        for(int j=0; j<inter->num_altsetting; j++) {
            interdesc = &inter->altsetting[j];
            printf("Interface Number: %d | \n", (int)interdesc->bInterfaceNumber);
            printf("Number of endpoints: %d | \n", (int)interdesc->bNumEndpoints);
            for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
                epdesc = &interdesc->endpoint[k];
                printf("Descriptor Type: %d | \n", (int)epdesc->bDescriptorType);
                printf("EP Address: %d | \n", (int)epdesc->bEndpointAddress);
            }
        }
    }
    printf("\n");
    libusb_free_config_descriptor(config);
}

