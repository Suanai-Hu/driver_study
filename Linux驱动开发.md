### 一个最简单的Linux驱动必须包含以下几个部分
1. 头文件 必须包含<linux/module.h><linux/init.h>
2. 驱动加载函数<br>
	当加载驱动时，加载函数会被内核自动执行
3. 驱动写在函数<br>
	同2
4. 许可声明
5. 模块参数（可选）
6. 作者和版本信息（可选）

### 编写一个简单的驱动程序
1. `helloworld.c`:

		#include <linux/module.h>
		#include <linux/init.h>
		
		static int helloworld_init(void)
		{
		    printk("helloworld!!!\n");
		
		    return 0;
		}
		
		static void helloworld_exit(void)
		{
		    printk("helloworld bye!!!\n");
		}
		
		module_init(helloworld_init);
		module_exit(helloworld_exit);
		
		MODULE_LICENSE("GPL");
		MODULE_AUTHOR("hhr");
		MODULE_VERSION("v1.0");

2. `Makefile`

		obj-m += helloworld.o									// 把目标文件.o作为模块进行编译
		KDIR:=/home/linux-kernel								// 绝对路径指定内核源码
		PWD?=$(shell pwd)										// 获取Makefile文件苏在路径
		all:
			make -C $(KIDR) M=$(PWD) modules					// 编译到KDIR目录，使用PWD路径下的源码和Makefile文件编译驱动
		clean:
			rm -f *.o *.ko *.mod.o *.mod.c *.symvers *.order	// 清除编译文件

3. 将内核源码编译通过

4. 设置环境变量
	- **以下方式设置环境变量是临时设置，在那个目录下设置，就只对这个目录下的文件起作用，到其他目录下就无效**
	- **交叉编译工具需要按照实际路径填写**

			export ARCH=arm64
			export CROSS_COMPILE=/home/hhr/ws/RK3568/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

5. 模块加载命令
	1. insmod命令
		- 功能：载入Linux内核模块
		- 语法：`insmod 模块名`
		- 举例：`insmod helloworld.ko`
	2. modprobe命令
		- 功能：加载Linux模块，同时这个模块所依赖的模块也被加载
		- 语法：`modprobe 模块名`
		- 举例：`modprobe helloworld.ko`<p>

6. 模块卸载命令
	1. rmmod命令
		- 功能：卸载已载入Linux的内核模块
		- 语法：`rmmod 模块名`
		- 举例：`rmmod helloworld.ko`<p>

7. 查看模块信息
	1. lsmod
		- 列出已经加载的内核模块
		- 也可以使用`cat /proc/modules`来查看模块是否加载成功
	2. modinfo
		- 功能：查看内核模块信息
		- 语法：`modinfo 模块名`
		- 举例：`modinfo helloworld.ko`

### 驱动模块传参引入
1. 在系统编程中绝大多数的情况需要main函数作为程序的入口，来进行参数的传递

		int main(int argc, char **argv[])
		{
			/* 代码 */
		}
其中：
	- `argc`英文全称 `argument count`，代表形参，表示传入参数的个数
	- `argv`英文全称 `argument value`，代表传入参数的值<p>

2. 驱动传参的意义
	- 优势：
		- 通过驱动传参，可以让驱动更灵活，兼容性更强
		- 可以通过驱动传参，设置安全校验，防止驱动盗用 
	- 不足：
		- 驱动代码更复杂
		- 驱动占用资源更多  <p>

3. 驱动可以传递的参数类型<br>
	C语言中常用的数据类型内核大部分都支持驱动传参，这里将内核支持的参数类型分为三类：
	- 基本类型：char、bool、int、long、short、byte、ushort、unit
	- 数组：array
	- 字符串：string<p>

4. 如何给驱动传递参数<br>
	驱动支持的三种参数类型分别对应函数
	- <font color=blue>`module_param` </font>
	- <font color=blue>`module_param_array` </font>
	- <font color=blue>`module_param_string`</font>

	这三个在函数在内核源码<font color=blue>`inlcude/linux/moduleparam.h`</font> 中有定义:
	1. <font color=blue>`module_param`</font> 函数<br>
		函数功能：传递基本类型参数给驱动<br>
		函数原型：<font color=blue>`module_param(name, type, perm);`</font><br>
		函数参数：
		- name：传递给驱动代码中的变量的名字
		- type：参数类型
		- perm：参数的读写权限<p>

	2. <font color=blue>`module_param_array`</font> 函数<br>
		函数功能：传递数组类型参数给驱动<br>
		函数原型：<font color=blue>`module_param_array(name, type, nump, perm);`</font><br>
		函数参数：
		- name：传递给驱动代码中的变量的名字
		- type：参数类型
		- nump：数组的长度
		- perm：参数的读写权限<p>

	3. <font color=blue>`module_param_string`</font> 函数<br>
		函数功能：传递数组类型参数给驱动<br>
		函数原型：<font color=blue>`module_param_string(name, type, len, perm);`</font><br>
		函数参数：
		- name：传递给驱动代码中的变量的名字
		- type：参数类型
		- len ：字符串的大小
		- perm：参数的读写权限<p>
	
	4. <font color=blue>`MODULE_PARAM_DESC`</font> 函数<br>
		函数功能：描述模块的参数的信息，在 <font color=blue>`include/linux/moduleparam.h`</font> 中定义<br>
		函数原型：<font color=blue>`MODULE_PARAM_DESC(_param, desc);`</font><br>
		函数参数：
		- _param：要描述的参数的参数名称
		- desc：描述信息<p>

5. 参数的读写权限<br>
	读写权限在 <font color=blue>`include/linux/stat.h`</font> 和 <font color=blue>`include/uapi/linux/stat.h`</font> 下有定义，一般使用 <font color=blue>`S_IRUGO`</font>，也可以使用数字表示，如 `444` 表示 `S_IRUGO`

	1. `include/linux/stat.h`

			#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
			#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
			#define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)
			#define S_IWUGO		(S_IWUSR|S_IWGRP|S_IWOTH)
			#define S_IXUGO		(S_IXUSR|S_IXGRP|S_IXOTH)

	2. `include/uapi/linux/stat.h`

			#define S_IRWXU 00700
			#define S_IRUSR 00400
			#define S_IWUSR 00200
			#define S_IXUSR 00100
			
			#define S_IRWXG 00070
			#define S_IRGRP 00040
			#define S_IWGRP 00020
			#define S_IXGRP 00010
			
			#define S_IRWXO 00007
			#define S_IROTH 00004
			#define S_IWOTH 00002
			#define S_IXOTH 00001

### 内核符号表导出
1. 内核符号表引入<br>
	驱动程序可以编译成内核模块，每个模块之间是相互独立的，也就是说模块间无法互相访问。
	所谓的符号就是内核中的函数名、全局变量名等，符号表就是记录这些符号的文件。

2. 内核符号表导出<br>
	模块可以使用宏 <font color=blue>`EXPORT_SYSBOL`</font> 和 <font color=blue>`EXPOLE_SYMBOL_GPL`</font> 导出符号岛内核符号表中<br>
	例如：

		EXPORT_SYSBOL(符号名);
		EXPOLE_SYMBOL_GPL(符号名);  	// 只用于包含GPL许可的模块

	导出去的符号可以被其他符号引用，使用前只需提前声明即可。
	
### 申请字符设备号<br>
1. 什么是设备号<br>
	- Linux规定每个字符设备或者块设备都需要一个专属的设备号。
	- 设备号由主设备号和次设备号组成。
	- 主设备号用来表示某一类驱动，次设备号表示这类驱动下的各个设备<br>
<br>

2. 设备号类型<br>
Linux中使用 <font color=blue>`dev_t`</font> 的数据类型表示设别号。 <font color=blue>`dev_t`</font>定义在<font color=blue>`include/linux/types.h`</font> 中。可以看出 <font color=blue>`dev_t`</font> 是u32类型，所以设备号是一个32位的数据类型。其中，高12位是主设备号，低20位是次设备号。

		typedef u32 __kernel_dev_t;
	
		typedef __kernel_fd_set		fd_set;
		typedef __kernel_dev_t		dev_t;
		typedef __kernel_ino_t		ino_t;
		typedef __kernel_mode_t		mode_t;

3. 设备号操作宏<br>
在文件 <font color=blue>`include/linux/kdev_t.h`</font> 中提供了几个设备号操作的宏定义。

		#define MINORBITS	20										// 次设备号位数
		#define MINORMASK	((1U << MINORBITS) - 1)					// 用于计算次设备号
		
		#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))	// 从dev_t中获取主设备号 本质是将dev_t右移20位
		#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))	// 从dev_t中获取次设备号 本质是取低20位
		#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))		//将主设备号ma和次设备号mi组成dev_t类型的设备号

4. 设备号的分配<br>
编写字符设备驱动代码时，可以静态分配设备号或者动态分配设备号。<br>
静态分配指自定一个设备号。由于有些设备号可能已经被使用，可以采用 <font color=blue>`cat /proc/devices`</font> 查看当前系统中已经使用的设备号。<br>
动态分配就是系统自动分配。<br>
内核中，提供了动态和静态分配设备号的函数，在 <font color=blue>`include/linux/fs.h`</font> 中。<br>
		
		静态分配： register_chrdev_region
		动态分配： alloc_chrdev_region

	1. <font color=blue>`register_chrdev_region`</font>
		1. 函数原型：<font color=blue>`int register_chrdev_region(dev_t, unsigned, const char *)`</font>
		2. 函数参数：
			- 参数1：设备号的起始值，类型是dev_t
			- 参数2：设备号的数量，表示在主设备号相同的情况下，有几个次设备号
			- 参数3：设备的名称
		3. 函数返回值：成功返回0，失败返回小于0<br>
<br>

	2. <font color=blue>`alloc_chrdev_region`</font>
		1. 函数原型：<font color=blue>`int register_chrdev_region(dev_t *, unsigned, unsigned, const char *)`</font>
		2. 函数参数：
			- 参数1：保存自动申请的设备号
			- 参数2：次设备号起始地址，一般为0
			- 参数3：设备号的数量，表示在主设备号相同的情况下，有几个次设备号
			- 参数4：设备的名称
		3. 函数返回值：成功返回0，失败返回小于0<br>
<br>
	
	3. <font color=blue>`unregister_chrdev_region`</font>
		1. 函数原型：<font color=blue>`int register_chrdev_region(dev_t, unsigned)`</font>
		2. 函数功能：设备号释放，注销字符设备后需要释放设备号
		3. 函数参数：
			- 参数1：要释放的设备号
			- 参数2：释放的设备号数量

### 注册字符类设备
1. cdev结构体<br>
Linux中，使用cdev结构体描述一个字符设备，cdev结构体定义 <font color=blue>`include/linux/cdev.h`</font> 

		struct cdev {
			struct kobject kobj;
			struct module *owner;					// 所属模块
			const struct file_operations *ops;		// 文件操作结构体
			struct list_head list;
			dev_t dev;								// 设备号
			unsigned int count;
		} __randomize_layout;

2. <font color=blue>`cdev_init`</font> 函数: <font color=blue>`linux-3.19.3/fs/char_dev.c`</font><br>
<font color=blue>`cdev_init`</font>函数用于初始化cdev结构体，建立cdev和file_operations之间的联系。函数原型如下：

		/**
		 * cdev_init() - initialize a cdev structure
		 * @cdev: the structure to initialize
		 * @fops: the file_operations for this device
		 *
		 * Initializes @cdev, remembering @fops, making it ready to add to the
		 * system with cdev_add().
		 */
		void cdev_init(struct cdev *cdev, const struct file_operations *fops)
		{
			memset(cdev, 0, sizeof *cdev);
			INIT_LIST_HEAD(&cdev->list);
			kobject_init(&cdev->kobj, &ktype_cdev_default);
			cdev->ops = fops;
		}

3. <font color=blue>`cdev_add`</font> 函数<br>
向系统添加一个cdev结构体，也就是添加一个字符设备。

		/**
		 * cdev_add() - add a char device to the system
		 * @p: the cdev structure for the device
		 * @dev: the first device number for which this device is responsible
		 * @count: the number of consecutive minor numbers corresponding to this
		 *         device
		 *
		 * cdev_add() adds the device represented by @p to the system, making it
		 * live immediately.  A negative error code is returned on failure.
		 */
		int cdev_add(struct cdev *p, dev_t dev, unsigned count)
		{
			int error;
		
			p->dev = dev;
			p->count = count;
		
			if (WARN_ON(dev == WHITEOUT_DEV)) {
				error = -EBUSY;
				goto err;
			}
		
			error = kobj_map(cdev_map, dev, count, NULL,
					 exact_match, exact_lock, p);
			if (error)
				goto err;
		
			kobject_get(p->kobj.parent);
		
			return 0;
		
		err:
			kfree_const(p->kobj.name);
			p->kobj.name = NULL;
			return error;
		}

### file_operatopns结构体
1. <font color=blue>`file_operatopns`</font>结构体的作用<br>
Linux有一个很重要的概念叫一切皆文件，也就是Linux中的设备就像普通的文件一样。<br>
访问一个设备就好像是在访问一个文件。在应用程序中我们可以使用open，read，write，close，ioctl这个几个系统调用来操作驱动。<br>
当我们在应用程序中调用open函数的时候，最终会去执行驱动中的open函数。所以file operations将系统调用 和驱动程序连接起来了<br>

2. <font color=blue>`file_operatopns`</font>结构体<br>
<font color=blue>`file_operatopns`</font> 结构体定义在 <font color=blue>include/linux/fs.h`</font> 文件当中，这个结构非常的庞大。

		struct file_operations {
			struct module *owner;															// 拥有该结构体的模块指针，一般设置为THIS_MODULE
			loff_t (*llseek) (struct file *, loff_t, int);									//用于修改文件当前的读写位置
			ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);				// 用于读取设备文件
			ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);		//用于向文件设备写数据
			ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
			ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
			int (*iterate) (struct file *, struct dir_context *);
			int (*iterate_shared) (struct file *, struct dir_context *);
			__poll_t (*poll) (struct file *, struct poll_table_struct *);					// 轮询函数，用于查询设备是否可被非阻塞的立刻读写
			long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);			// 提供设备相关的控制命令
			long (*compat_ioctl) (struct file *, unsigned int, unsigned long);				// 与unlock_ioctl功能一样
			int (*mmap) (struct file *, struct vm_area_struct *);   // 用于将设备内存映射到用户空间中去，应用程序可以直接访问无需再内核和应用间进行内存的赋值
			unsigned long mmap_supported_flags;
			int (*open) (struct inode *, struct file *);			// 用于打开设备文件
			int (*flush) (struct file *, fl_owner_t id);
			int (*release) (struct inode *, struct file *);			// 用于释放设备文件
			int (*fsync) (struct file *, loff_t, loff_t, int datasync);
			int (*fasync) (int, struct file *, int);
			int (*lock) (struct file *, int, struct file_lock *);
			ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
			unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
			int (*check_flags)(int);
			int (*flock) (struct file *, int, struct file_lock *);
			ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
			ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
			int (*setlease)(struct file *, long, struct file_lock **, void **);
			long (*fallocate)(struct file *file, int mode, loff_t offset,
					  loff_t len);
			void (*show_fdinfo)(struct seq_file *m, struct file *f);
		#ifndef CONFIG_MMU
			unsigned (*mmap_capabilities)(struct file *);
		#endif
			ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
					loff_t, size_t, unsigned int);
			int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
					u64);
			int (*dedupe_file_range)(struct file *, loff_t, struct file *, loff_t,
					u64);
			int (*fadvise)(struct file *, loff_t, loff_t, int);
		
			ANDROID_KABI_RESERVE(1);
			ANDROID_KABI_RESERVE(2);
			ANDROID_KABI_RESERVE(3);
			ANDROID_KABI_RESERVE(4);
		} __randomize_layout;

### 设备节点的概念
1. 什么是设备节点<br>
本着Linux中一切皆文件的思想。每个设备在Linux系统中都有一个对应的“设备文件”代表他们<br>
应用程序通过操作这个“设备文件”，便可以操作对应的硬件。如下代码所示:
		
		fd=open("/dev/hello",O_RDWR);
这个“设备文件”就是设备节点。 所以Linux设备节点是应用程序和驱动程序沟通的一个桥梁。设备节点被创建在dev目录下。

		crw-rw---- 1 root tty 7, 5 9月   6 22:13 /dev/vcs5
上面的vcs5就是设备节点，位于/dev 下，C表示它是一个字符设备，7是主设备号，5表示次设备号。Linux可以通过主设备号来找到他对应的file operations结构体，通过次设备号找到这个设备是同类设备中的第几个，这样就确定了是哪个驱动程序。

2. Linux下创建节点的方法
	1. 手动创建
		- 可以通过命令 mknod 创建设备节点
		- 格式：mknod 设备节点名称 设备类型(字符设备用c，块设备用b) 主设备号 次设备号
		- 举例：mknod /dev/test c 236 0

	2. 在注册设备时自动创建
		- 通过mdev机制实现设备节点的自动创建和删除

	3. udev机制
		- Linux中可以通过udev来实现设备节点的创建与删除，udev是一个用户程序，可以根据系统中的状态来创建或者删除节点，比如当驱动程序成功加载到Linux时会自动在/dev目录下创建对应的设备节点，当驱动程序卸载的时候，会自动删除设备节点。
		- 在嵌入式Linux中我们使用的是mdev，mdev是udev的简化版本。在使用busybox构建根文件系统时，busybox会自动创建mdev。

	4. <font color=blue>`class_create`</font>函数
		- class_create函数定义在<font color=blue>`include/linux/devide.h`</font>。是一个宏定义，这个函数会在<font color=blue>`/sys/class`</font>下创建文件。
		
				/* This is a #define to keep the compiler from merging different
 				* instances of the __key variable */
				#define class_create(owner, name)		\
				({						\
					static struct lock_class_key __key;	\
					__class_create(owner, name, &__key);	\
				})

				extern struct class * __must_check __class_create(struct module *owner,
						  const char *name,
						  struct lock_class_key *key);

		- <font color=blue>`class_create`</font>一共有两个参数，owner一般为THIS_MODULE；name是类的名字。

	5. <font color=blue>`device_create`</font>函数
		- 使用<font color=blue>`class_create`</font>函数创建好类后，需要使用<font color=blue>`device_create`</font>函数在该类下面创建一个设备，定义在<font color=blue>`include/linux/devide.h`</font>。

				struct device *device_create(struct class *cls, struct device *parent,
			     		dev_t devt, void *drvdata,
			     		const char *fmt, ...);

		- 第一参数class表示这个设备创建在哪个类下面，第二个参数parent是父设备，一般设置成NULL，也就是没有父设备。第三个参数dev_t是设备号。第四个参数是设备可能会用到的数据，一般设置成空，第五个参数是设备节点的名字

	6. <font color=blue>`devide_destroy`</font>函数
		- 使用<font color=blue>`devide_destroy`</font>函数可以删除创建的设备，参数class是要删除设备所处的类，dev_t是要删除的设备号。

	7. <font color=blue>`class_create`</font>函数
		- 使用<font color=blue>`class_create`</font>函数可以删除创建的类，参数class是要删除设备所处的类。

### 内核空间与用户空间
1. 内核空间与用户空间
	1. Linux系统将可访问的内存空间分为了俩部分，一部分是内核空间，一部分是用户空间。操作系统和驱动程序运行在内核空间(内核态)，应用程序运行在用户空间(用户态)。
	2. 内核空间中的代码控制了硬件资源，用户空间中的代码只能通过内核暴露的系统调用接口来使用系统中的硬件资源。这个样的设计可以保证操作系统自身的安全性和稳定性。
	3. 从另一方面来说，内核空间的代码更偏向于系统管理，而用户空间中的代码更偏重业务逻辑实现。两者的分工不同。<br>
<br>

2. 用户空间和内核空间的数据交换
	- 内核空间和用户空间的内存是不能互相访问的。但是很多业务程序都需要和内核交互数据,比如应用程序使用read函数从驱动中读取数据，使用write函数向驱动中写数据。这就要需要借助copy_from_user和copy_to_user这两个函数完成数据传输。分别是将用户空间的数据拷贝到内核空间以及将内核空间的数据拷贝到用户空间。这俩个函数定义在了linux/include/asm-arm/uaccess.h文件下。
<br>

3. <font color=blue>`copy_to_user`</font>函数
	- 函数原型：`unsigned long copy_to_user(void _user *to, const void *from, unsigned long n)`;
	- 作用：把内核空间的数据复制到用户空间函数
	- 参数：*to用户空间的指针。 *from内核空间的指针，n是从内核空间向用户空间拷贝的字节数
	- 函数返回值: 成功返回0<br>
<br>

4. <font color=blue>`copy_to_user`</font>函数
	- 函数原型: `unsigned long copy_from_user(void * to, const void user * from, unsigned long n)`
	- 作用：把用户空间的数据复制到内核空间函数
	- 参数：*to内核空间的指针。 *from用户空间的指针，n是数据的长度
	- 函数返回值:成功返回0

### 使用文件私有数据
1. 文件私有数据是什么
	1. 文件私有数据的概念在Linux驱动中有广泛的应用，文件私有数据就是将私有数据<font color=blue>`private_data`</font>指向设备结构体。然后在read、write等函数中通过<font color=blue>`private_data`</font>访问设备结构体。

### 杂项设备驱动
1. 描述
	1. 在Linux中，把无法归类的五花八门的设备定义成杂项设备。相对与字符设备来说,杂项设备主设备固定为10，而字符设备不管是动态分配还是静态分配设备号，都会消耗1个主设备号，比较浪费主设备号。杂项设备会自己调用class create()和device create ()来自动创建设备节点。所以可以把杂项设备看成是字符设备的一种。但是比我们平常写的字符设备降低了难度并节约了主设备号。

	2. 杂项设备使用结构体<font color=blue>`miscdevice`</font>描述，定义在<font color=blue>`include/linux/miscdevice.h`</font>
			
			struct miscdevice  {
				int minor;								// 次设备号
				const char *name;
				const struct file_operations *fops;		// 系统调用open、write、read等函数的桥梁
				struct list_head list;
				struct device *parent;
				struct device *this_device;
				const struct attribute_group **groups;
				const char *nodename;
				umode_t mode;
			};
	其中次设备号minor一般使用宏<font color=blue>`MISC_DYNAMIC_MINOR`</font>，表示自动分配次设备号，杂项设备主要依赖次设备号来管理不同的杂项设备。

	3. 杂项设备的注册与卸载
		1. <font color=blue>`misc_register`</font>
			- 函数原型：<font color=blue>`int misc_register(struct miscdevice *misc)`</font>
			- 参数：杂项设备的结构体指针
			- 返回值：成功返回0，失败返回负数
		2. <font color=blue>`misc_deregister`</font>
			- 函数原型：<font color=blue>`int misc_deregister(struct miscdevice *misc)`</font>
			- 参数：杂项设备的结构体指针
			- 返回值：成功返回0，失败返回负数

### Linux驱动错误处理
1. 内核中保留了地址0xfffffffffff000 ~ 0xffffffffffffffff(64位系统)用来记录错误码，这段地址与Linux错误码是一一对应的，内核基本错误码保存在errn-base.h文件中。
2. 内核中的函数常常返回指针，如果内核中返回一个指针，会出现三种情况：合法指针、NULL指针、非法指针。
	1. 使用<font color=blue>`IS_ERR`</font>函数去检查函数的返回值，如果地址落在0xfffffffffff000 ~ 0xffffffffffffffff范围内，表示该函数执行失败，<font color=blue>`IS_ERR`</font>为1，同时该函数返回的错误地址对应一个Linux错误号。
	2. 如果想知道这个地址是那个错误码，就用<font color=blue>`PTR_ERR`</font>函数来转化，其中，<font color=blue>`IS_ERR`</font>和<font color=blue>`PTR_ERR`</font>函数定义在errno.h中。
