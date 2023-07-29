#define TRS_KEY 6148
#define 获取信息 'f'

#include "fixFork.h"
#include "func.h"
#include "ini.h"
#include "key.h"

#ifndef VM_RESERVED
#define VM_RESERVED (VM_DONTEXPAND | VM_DONTDUMP)
#endif

#define 大小 映射区->大小

struct mm_struct *pb_mm = NULL;
struct task_struct *pb_task = NULL;
struct pid *pb_pid = NULL;

// static struct mm_struct *noget_task_mm(struct task_struct *task)
// {
    // struct mm_struct *mm;

    // task_lock(task);
    // mm = task->mm;
    // if (mm)
    // {
        // if (task->flags & PF_KTHREAD)
            // mm = NULL;
    // }
    // task_unlock(task);
    // return mm;
// }

// static struct mm_struct *get_task_mm2(struct task_struct *task)
// {
    // struct mm_struct *mm;
    // mm = get_task_mm(task);
    // mmput(mm);
    // return mm;
// }

static ssize_t 读操作(struct file *file, char __user *buf, size_t 线程标号, loff_t *ppos)
{
    int 结果;
    uint32_t crc;
    // int * TData;
    // struct vm_area_struct *vma = NULL;

    pb_mm = NULL;
    pb_task = NULL;
    pb_pid = NULL;

    switch (用户指令)
    {

    case 指定进程:

        映射区->返回信息 = 失败;

        crc = get_str_crc32(映射区->sbm);

		if ((crc ^ 映射区->key) != 原码)
		{
			if(映射区->key != 0x2023666)
			{
				映射区->返回信息='e';
				return -1;
			}
		}

        pb_pid = find_get_pid(目标pid);
        if (!pb_pid)
        {
            映射区->返回信息 = 失败;
            return -1;
        }
        put_pid(pb_pid);

        
        pb_task = get_pid_task(pb_pid, PIDTYPE_PID);
        if (!pb_task)
        {
            映射区->返回信息 = 失败;
            return -1;
        }
        put_task_struct(pb_task);


        pb_mm = get_task_mm(pb_task);
        if (!pb_mm)
        {
            映射区->返回信息 = 失败;
            return -1;
        }
        mmput(pb_mm);


        
        
        down_read(&pb_mm->mmap_sem);
        tag_pgd = pb_mm->pgd;
        up_read(&pb_mm->mmap_sem);
        
        

        
        映射区->返回信息 = 成功;
        
        
      
        break;

    case 转化内存:
        // vma = find_vma(pb_mm, (目标地址 ^ TRS_KEY));
        // if (!vma)
        // {
            // 映射区->返回信息 = 失败;
            // return -1;
        // }

        结果 = 虚拟地址转物理地址(写入内容, 目标地址 ^ TRS_KEY, tag_pgd);

        if (!结果)
        {
            映射区->返回信息 = 失败;
            return -1;
        }
        映射区->返回信息 = 成功;
        break;

    case 读取内存:

        结果 = 读取物理内存2(写入内容, 目标地址, 大小);

        if (!结果)
        {
            映射区->返回信息 = 失败;
            return -1;
        }
        映射区->返回信息 = 成功;

        break;

        // case 获取基址:
        // 结果=GetModuleBase(&(映射区->内容);
        // 判断(结果);
        // break;

        // case 获取cmdline地址:
        // 结果=GetModuleBase(&(映射区->内容);
        // 判断(结果);
        // break;
    }
    return 0;
}

static ssize_t 写操作(struct file *filp, const char __user *buf, size_t 线程标号, loff_t *ppos)
{
    // int 结果;
    写入物理内存(映射区->内容 , 映射区->地址, 大小);
    // 判断(结果);
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

int 映射内存2(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);
    vma->vm_flags |= VM_IO;
    vma->vm_flags |= VM_RESERVED;
    if (remap_pfn_range(vma, vma->vm_start, virt_to_phys(映射区) >> PAGE_SHIFT, size, vma->vm_page_prot))
        return -EAGAIN;
    strcpy(映射区->内容, "内存读写驱动 1.75(单线程版) \n仅供学习使用\n");
    return 0;
}

static struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .open = 打开操作,
    .release = 关闭操作,
    .read = 读操作,
    .write = 写操作,
    .mmap = 映射内存2,
};

struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_FILENAME,
    .fops = &dev_fops,
};

int __init misc_dev_init(void)
{
    int ret;
    printk("[+] 初始化驱动");
    映射区 = (DAT *)kmalloc(PAGE_SIZE, GFP_KERNEL);
    SetPageReserved(virt_to_page(映射区));
    ret = misc_register(&misc);
    return ret;
}

void __exit misc_dev_exit(void)
{
    printk("[+] 卸载驱动");
    ClearPageReserved(virt_to_page(映射区));

    kfree(映射区);

    misc_deregister(&misc);
}

module_init(misc_dev_init);
module_exit(misc_dev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux default module");
