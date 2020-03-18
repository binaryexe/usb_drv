/* To demonstrate USB features and used Anil Pugalia article
 * 
 */
/* include all header file may need*/
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/kernel.h>

static struct usb_device *device;
/* definition of connect and disconnect*/
static void bx_usbms_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "\nUSB Mass-storage device disconnected\n");
    printk(KERN_INFO "Pen i/f %d now disconnected\n",
            interface->cur_altsetting->desc.bInterfaceNumber);
}

/* structure for usb related operation */
static struct usb_device_id bx_usbms_table [] =
{
    {USB_DEVICE(0x3f0, 0xfe07)},
    {} /* terminating entry*/
};

MODULE_DEVICE_TABLE(usb, bx_usbms_table);
/* init funtion */
static int bx_usbms_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_host_interface *if_desc;
    struct usb_endpoint_descriptor *ep_desc;
   int i;
    printk(KERN_INFO "\n USB Mass Storage Device (%04X:%04X) plugged in\n", id->idVendor,
                                id->idProduct);
    /* get descriptor details into local structure*/
    if_desc = interface->cur_altsetting;
    printk(KERN_INFO "USBMS i/f %d now probed: (%04X:%04X)\n",
            if_desc->desc.bInterfaceNumber,
            id->idVendor, id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            if_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            if_desc->desc.bInterfaceClass);
    /* get inter details */
    /* Get endpoint details */
    for (i = 0; i < if_desc->desc.bNumEndpoints; i++)
    {
        ep_desc = &if_desc->endpoint[i].desc;

        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, ep_desc->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, ep_desc->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, ep_desc->wMaxPacketSize,
                ep_desc->wMaxPacketSize);
    }

    device = interface_to_usbdev(interface);
    /* send back usb_device details to kernel */
    return 0;
}

static struct usb_driver usb_ms =
{
    .name = "bx usb mass storage drv",
    .id_table = bx_usbms_table,
    .probe = bx_usbms_probe,
    .disconnect = bx_usbms_disconnect
};

static int __init bx_usbd_init(void)
{
    printk(KERN_INFO "\nBX_USBD Init Called\n");
    usb_register(&usb_ms);
    return 0;
}

/* exit function */
static void __exit bx_usbd_exit(void)
{
    printk(KERN_INFO "\nBX_USBD Exit Called\n");
    usb_deregister(&usb_ms);
}

/* associate init and exit to module_init and module_exit */
module_init(bx_usbd_init);
module_exit(bx_usbd_exit);

/* gpl related stuff */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("exebinary@gmail.com");
MODULE_DESCRIPTION(" For Self Learning LDD");



