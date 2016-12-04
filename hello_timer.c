#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h> 
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nick Mandrik");
MODULE_VERSION("0.01");

static struct timer_list my_timer;
static struct kobject *mytimer_kobject;
static int delay = 1;

    
static void print_hello(unsigned long data)
{
	printk(KERN_INFO "Hello, timer!\n");
	mod_timer(&my_timer, jiffies + delay * HZ);
}

static ssize_t mytimer_read(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", delay);
}

static ssize_t mytimer_write(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d", &delay);
	return count;
}

static struct kobj_attribute delay_attribute =__ATTR(delay, 0664, mytimer_read, mytimer_write);

static int mytimer_init(void)
{
	int error = 0;	
	mytimer_kobject = kobject_create_and_add("mytimer", kernel_kobj);
	if (!mytimer_kobject) {
		return -ENOMEM;
	}
	error = sysfs_create_file(mytimer_kobject, &delay_attribute.attr);
	if (error) {
		printk(KERN_INFO "failed to create the delay file in /sys/kernel/mytimer \n");
		return error;
	}
	init_timer_on_stack(&my_timer);
	my_timer.expires = jiffies + delay * HZ;
	my_timer.data = 0;
	my_timer.function = print_hello;
	add_timer(&my_timer);
	return 0;
}

static void mytimer_exit(void)
{	
	kobject_put(mytimer_kobject);
	del_timer(&my_timer);
}

module_init(mytimer_init);
module_exit(mytimer_exit);
