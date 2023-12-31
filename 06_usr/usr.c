#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

dev_t dev_num;
static int major = 0;
static int minor = 0;

struct cdev cdev_test;

struct class *class;
struct device *device;

static int cdev_test_open(struct inode *inode, struct file *file)
{
    printk("open\n");

    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    char kbuf[1024] = "this is cdev_test_read";
    if(copy_to_user(buf, kbuf, strlen(kbuf)) != 0)
    {
        printk("copy_to_user error\n");
        return -1;
    }

    printk("cdev_test_read\n");
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    char kbuf[1024] = {0};
    if(copy_from_user(kbuf, buf, size) != 0)
    {
        printk("copy_from_user error\n");
        return -1;
    }

    printk("kbuf is %s\n", kbuf);

    printk("write\n");
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file)
{
    printk("release\n");

    return 0;
}

struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE,
    .open = cdev_test_open,
    .read = cdev_test_read,
    .write = cdev_test_write,
    .release = cdev_test_release
};

static int modulecdev_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");
    if(ret < 0)
    {
        printk("alloc_chrdev_region failed.\n");
        return -1;
    }
    else
    {
        printk("alloc_chrdev_region success.\n");
    }

    major = MAJOR(dev_num);
    minor = MINOR(dev_num);
    printk("major is %d\n", major);
    printk("minor is %d\n", minor);

    cdev_test.owner = THIS_MODULE;
    cdev_init(&cdev_test, &cdev_test_ops);
    cdev_add(&cdev_test, dev_num, 1);

    class = class_create(THIS_MODULE, "test");
    device = device_create(class, NULL, dev_num, NULL, "test");

    return 0;
}

static void modulecdev_exit(void)
{
    unregister_chrdev_region(dev_num, 1);
    cdev_del(&cdev_test);

    device_destroy(class, dev_num);
    class_destroy(class);

    printk("bye bye\n");
}

module_init(modulecdev_init);
module_exit(modulecdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hhr");
MODULE_VERSION("v1.0");