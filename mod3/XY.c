
#include "fixFork.h"
//#include "zj.h"
#include "func.h"
#include "ini.h"
//#include <linux/module.h>
#include <linux/module.h>
//#include <net/netfilter/nf_conntrack_core.h>
#include <asm-generic/delay.h>
//#include <linux/kallsyms.h>
//#include <linux/list.c>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/random.h>
static char node_name[32];
//static char nod_name;

#ifndef VM_RESERVED
#define VM_RESERVED (VM_DONTEXPAND | VM_DONTDUMP)
#endif

#define 大小 映射区->数据体[线程标号].大小

//内核函数原型，为导出函数，用于找到输入函数名所对应的函数入口地址
//#include <linux/kallsyms.h>
/* Lookup the address for this symbol. Returns 0 if not found. */
/*unsigned long kallsyms_lookup_name(const char *name)
{
	char namebuf[KSYM_NAME_LEN];
	unsigned long i;
	unsigned int off;

	for (i = 0, off = 0; i < kallsyms_num_syms; i++) {
		off = kallsyms_expand_symbol(off, namebuf, ARRAY_SIZE(namebuf));

		if (strcmp(namebuf, name) == 0)
			return kallsyms_sym_address(i);
	}
	return module_kallsyms_lookup_name(name);
}

EXPORT_SYMBOL_GPL(kallsyms_lookup_name);*/
static ssize_t 读操作(struct file *file, char __user *buf, size_t 线程标号, loff_t *ppos)
{
	int 结果;
	struct mm_struct *mm;
	struct task_struct *tag_task;
	struct pid *proc_pid_struct;

	switch (用户指令)
	{
	case 指定进程:
		proc_pid_struct = find_get_pid(目标pid);
		判断(proc_pid_struct);
		printk_debug("proc_pid_struct 0x%lx\n", proc_pid_struct);
		tag_task = pid_task(proc_pid_struct, PIDTYPE_PID);
		判断(tag_task);
		printk_debug("tag_task 0x%lx\n", tag_task);

		mm = get_task_mm(tag_task);
		判断(mm);
     

		down_read(&mm->mmap_sem);
		tag_pgd = mm->pgd;
		up_read(&mm->mmap_sem);
		mmput(mm);
		判断(tag_pgd);
		printk_debug("tag_pgd 0x%lx\n", tag_pgd);

		break;

	case 转化内存:
		结果 = 虚拟地址转物理地址(写入内容, 目标地址, tag_pgd);
		判断(结果);
		break;

	case 读取内存:
		结果 = 读取物理内存(写入内容, 目标地址, 大小);
		判断(结果);
		break;

		// case 获取基址:
		// 结果=GetModuleBase(&(映射区->数据体[线程标号].地址) , 映射区->数据体[线程标号].数据);
		// 判断(结果);
		// break;

		// case 获取cmdline地址:
		// 结果=GetModuleBase(&(映射区->数据体[线程标号].地址) , 映射区->数据体[线程标号].数据);
		// 判断(结果);
		// break;
	}
	return 0;
}

static ssize_t 写操作(struct file *filp, const char __user *buf, size_t 线程标号, loff_t *ppos)
{
	int 结果;
	结果 = 写入物理内存(映射区->数据体[线程标号].内容, 映射区->数据体[线程标号].地址, 大小);
	判断(结果);
	return 0;
}

static int 打开操作(struct inode *inode, struct file *file)
{
	printk_debug("打开驱动了");
	return 0;
}

static int 关闭操作(struct inode *inode, struct file *filp)
{
	printk_debug("关闭驱动了");
	return 0;
}
static int 映射内存(struct file *file, struct vm_area_struct *vma)
{
	unsigned long page;
	unsigned long start = (unsigned long)vma->vm_start;
	// unsigned long end =  (unsigned long)vma->vm_end;
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

	// 得到物理地址
	page = virt_to_phys(映射区);
	// 将用户空间的一个vma虚拟内存区映射到以page开始的一段连续物理页面上
	if (remap_pfn_range(vma, start, page >> PAGE_SHIFT, size,
						PAGE_SHARED)) // 第三个参数是页帧号，由物理地址右移PAGE_SHIFT得到
		return -EAGAIN;
	strcpy(映射区->数据体[0].内容, "曦月魔改驱动 原作者红牛哥 橘子公益电报@orangeXY");
	return 0;
}

/*struct timer_list timer;
// 模拟重体力劳动
static void timer_func(unsigned long data)
{
	udelay(1000);
	mod_timer(&timer, jiffies + 10);
}*/
static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open = 打开操作,
	.release = 关闭操作,
	.read = 读操作,
	.write = 写操作,
	.mmap = 映射内存,
};

struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
    .name= node_name,
	.fops = &dev_fops,
	};
static void __init hide_myself(void)
{
	//struct vmap_area *va, *vtmp;
	struct module_use *use, *tmp;
	//struct list_head *_vmap_area_list;
	//struct rb_root *_vmap_area_root;

/*_vmap_area_list = (struct list_head *)kallsyms_lookup_name("vmap_area_list");
	_vmap_area_root = (struct rb_root *)kallsyms_lookup_name("vmap_area_root");

	// 摘除vmalloc调用关系链，/proc/vmallocinfo中不可见
	list_for_each_entry_safe(va, vtmp, _vmap_area_list, list) {
		if ((unsigned long)THIS_MODULE > va->va_start && (unsigned long)THIS_MODULE < va->va_end) {
			list_del(&va->list);
			// rbtree中摘除，无法通过rbtree找到
			rb_erase(&va->rb_node, _vmap_area_root);
		}
	}*/

    list_del_init(&THIS_MODULE->list);
	kobject_del(&THIS_MODULE->mkobj.kobj);
	list_for_each_entry_safe(use, tmp, &THIS_MODULE->target_list, target_list) {
		list_del(&use->source_list);
		list_del(&use->target_list);
		sysfs_remove_link(use->target->holders_dir, THIS_MODULE->name);
		kfree(use);
	}
}

/*int __init misc_dev_init(void)
{
	return 0;
//	return ret;
}

void __exit misc_dev_exit(void)
{
	printk("[+] 卸载驱动");
	ClearPageReserved(virt_to_page(映射区));

	kfree(映射区);

	misc_deregister(&misc);
//   del_timer_sync(&timer);

}*/
// hidemyself.c

static int __init random_node_init(void)
{   char random_suffix[4];
    // 将随机数转换为字符串
    char random_str[sizeof(random_suffix) * 2 + 1];
    int i;
    int ret;
    get_random_bytes(random_suffix, sizeof(random_suffix));
    for (i = 0; i < sizeof(random_suffix); i++) {
        snprintf(random_str + i * 2, 3, "%02x", random_suffix[i]);
    }
    // 生成节点名称
    /*nod_name = kmalloc(strlen("random_node_") + sizeof(random_str), GFP_KERNEL);
    if (!nod_name) {
        printk(KERN_ERR "Failed to allocate memory\n");
        return -ENOMEM;
    }*/
    strcpy(node_name, "mhi_");
    strcat(node_name, random_str);
    // 输出节点名称
    
	printk("[+] 初始化驱动");
	映射区 = (struct 桥梁 *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	SetPageReserved(virt_to_page(映射区));
//	list_del_init(&THIS_MODULE);
  // kobject_del(&THIS_MODULE);
//  random_node_init();
	ret = misc_register(&misc);
	hide_myself();
/*	init_timer(&timer);
	timer.expires = jiffies + 20;
	timer.function = timer_func;
	add_timer(&timer);*/

	// 模拟依赖。我们需要在依赖关系中也隐藏掉该模块的行踪
//	printk("address:%p   this:%p\n", nf_conntrack_in, THIS_MODULE);
    printk(KERN_INFO "Random node name: %s\n", node_name);
    // 在此处添加模块的其他初始化代码
    return 0;
}
static void __exit random_node_exit(void)
{	printk("[+] 卸载驱动");
	ClearPageReserved(virt_to_page(映射区));

	kfree(映射区);

	misc_deregister(&misc);

    // 在此处添加模块的清理代码
    // 释放节点名称的内存
    kfree(node_name);
    printk(KERN_INFO "Random node module unloaded\n");
}
module_init(random_node_init);
module_exit(random_node_exit);
/*module_init(misc_dev_init);
module_exit(misc_dev_exit);*/
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux default module");
MODULE_INFO(intree, "Y");
MODULE_INFO(scmversion, "gf9d99a97a122");
MODULE_AUTHOR("Xi Yue designed魔改非原创");
