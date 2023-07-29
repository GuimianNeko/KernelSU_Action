#ifndef _FIX_FUNCS
#define _FIX_FUNCS

// #define OLD_VER_4_9

#ifndef OLD_VER_4_9

// #include <linux/delayacct.h>
#include <linux/elf.h>
#include <linux/export.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/hugetlb.h>
#include <linux/init.h>
#include <linux/kallsyms.h>
#include <linux/kernel_stat.h>
#include <linux/ksm.h>
#include <linux/memcontrol.h>
#include <linux/memremap.h>
#include <linux/migrate.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmu_notifier.h>
#include <linux/pagemap.h>
#include <linux/pfn_t.h>
#include <linux/rmap.h>
#include <linux/sched/coredump.h>
#include <linux/sched/mm.h>
#include <linux/sched/numa_balancing.h>
#include <linux/sched/task.h>
#include <linux/string.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/writeback.h>
// #include <linux/dma-debug.h>
#include <linux/dax.h>
#include <linux/debugfs.h>
#include <linux/oom.h>
#include <linux/userfaultfd_k.h>

#include <trace/events/kmem.h>

#include <asm/io.h>
#include <asm/mmu_context.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/tlb.h>
#include <asm/tlbflush.h>
#include <linux/uaccess.h>

#else
#include <linux/dax.h>
#include <linux/debugfs.h>
#include <linux/delayacct.h>
#include <linux/elf.h>
#include <linux/export.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/hugetlb.h>
#include <linux/init.h>
#include <linux/kallsyms.h>
#include <linux/kernel_stat.h>
#include <linux/ksm.h>
#include <linux/memcontrol.h>
#include <linux/migrate.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmu_notifier.h>
#include <linux/pagemap.h>
#include <linux/pfn_t.h>
#include <linux/rmap.h>
#include <linux/string.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/userfaultfd_k.h>
#include <linux/writeback.h>

#include <asm/io.h>
#include <asm/mmu_context.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/tlb.h>
#include <asm/tlbflush.h>
#include <asm/uaccess.h>
#endif

#endif /* _FIX_FUNCS */