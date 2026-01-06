#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/cpumask.h>
#include <linux/kernel_stat.h>
#include <linux/pci.h>
#include <linux/fb.h>


#include <linux/mm.h>          // si_meminfo
#include <linux/sysinfo.h>
#include <linux/jiffies.h>
#include <linux/sched/loadavg.h>

#define DRIVER_NAME "pico_sysmon"

// USB VID/PID do dispositivo (Pico 2 com firmware)
#define USB_VID 0xCAFE
#define USB_PID 0x5002

// Endpoints do firmware Vendor (bulk). Ajuste se mudar no descritor.
#define EP_OUT 0x01
#define EP_IN  0x81
#define MAX_PKT 64

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor");
MODULE_DESCRIPTION("Pico SysMon: char device always available + USB activation when device connected");
MODULE_VERSION("1.1");

struct pico_sysmon {
	// Char device (sempre criado)
	dev_t devt;
	struct cdev cdev;
	struct class *cls;
	struct device *devnode;

	// Estado USB (ativado quando o Pico conecta)
	struct usb_device *udev;
	bool usb_online;

	// Sincronização e buffers
	struct mutex lock;
	u8 tx[MAX_PKT];
	u8 rx[MAX_PKT];

	// Estado para modo simulado
	char last_cmd[128];
};

static struct pico_sysmon g;

struct cpu_prev_stat {
	u64 total;
	u64 idle;
};

static struct cpu_prev_stat prev_cpu[NR_CPUS];
static bool cpu_prev_valid;


// ============ Helpers (coleta de métricas) ============

static void append_cpu_usage_percent(char *buf, size_t buf_sz, int *pos)
{
	int cpu;

	for_each_online_cpu(cpu) {
		struct kernel_cpustat kstat;
		u64 idle, total, busy;
		u64 delta_total, delta_idle;
		u32 usage = 0;

		kstat = kcpustat_cpu(cpu);

		idle = kstat.cpustat[CPUTIME_IDLE];
		total =
			kstat.cpustat[CPUTIME_USER] +
			kstat.cpustat[CPUTIME_SYSTEM] +
			kstat.cpustat[CPUTIME_IRQ] +
			kstat.cpustat[CPUTIME_SOFTIRQ] +
			kstat.cpustat[CPUTIME_NICE] +
			idle;

		if (cpu_prev_valid) {
			delta_total = total - prev_cpu[cpu].total;
			delta_idle  = idle  - prev_cpu[cpu].idle;

			if (delta_total > 0) {
				busy = delta_total - delta_idle;
				usage = (busy * 100) / delta_total;
			}
		}

		prev_cpu[cpu].total = total;
		prev_cpu[cpu].idle  = idle;

		if (*pos < buf_sz) {
			*pos += scnprintf(buf + *pos, buf_sz - *pos,
				"CPU%-2d : %3u %%\n", cpu, usage);
		}
	}

	cpu_prev_valid = true;
}

static void append_gpu_info(char *buf, size_t buf_sz, int *pos)
{
	struct pci_dev *pdev = NULL;

	for_each_pci_dev(pdev) {
		// Classe VGA ou Display Controller
		if ((pdev->class >> 8) == PCI_CLASS_DISPLAY_VGA ||
		    (pdev->class >> 8) == PCI_CLASS_DISPLAY_OTHER) {

			*pos += scnprintf(buf + *pos, buf_sz - *pos,
				"GPU        : %s\n"
				"Vendor ID  : 0x%04x\n"
				"Device ID  : 0x%04x\n",
				pci_name(pdev),
				pdev->vendor,
				pdev->device
			);
			return;
		}
	}

	*pos += scnprintf(buf + *pos, buf_sz - *pos,
		"GPU        : Not detected\n");
}


static int build_stats_report(char *out, size_t out_sz)
{
	struct sysinfo info;
	unsigned long uptime_s;
	unsigned long total_mb, free_mb, used_mb;
	unsigned int mem_percent;
	int pos = 0;

	si_meminfo(&info);

	uptime_s = jiffies_to_msecs(jiffies) / 1000;
	total_mb = (info.totalram * info.mem_unit) / (1024 * 1024);
	free_mb  = (info.freeram  * info.mem_unit) / (1024 * 1024);
	used_mb  = total_mb - free_mb;
	mem_percent = (used_mb * 100) / total_mb;

	pos += scnprintf(out + pos, out_sz - pos,
		"====================================\n"
		"           PICO SYSMON\n"
		"====================================\n"
		"Mode        : %s\n"
		"Uptime      : %lu s\n"
		"Load Avg    : %lu %lu %lu\n"
		"\n"
		"MEMORY\n"
		"------------------------------------\n"
		"RAM Usage   : %lu / %lu MB (%u %%)\n"
		"\n"
		"CPU USAGE (per core)\n"
		"------------------------------------\n",
		g.usb_online ? "USB" : "SIMULATED",
		uptime_s,
		avenrun[0] >> FSHIFT,
		avenrun[1] >> FSHIFT,
		avenrun[2] >> FSHIFT,
		used_mb, total_mb, mem_percent
	);

	append_cpu_usage_percent(out, out_sz, &pos);

	pos += scnprintf(out + pos, out_sz - pos,
		"\nGPU INFO\n"
		"------------------------------------\n");
	append_gpu_info(out, out_sz, &pos);

	pos += scnprintf(out + pos, out_sz - pos,
		"\nLast Command : %s\n"
		"====================================\n",
		g.last_cmd[0] ? g.last_cmd : "(none)"
	);

	return pos;
}



// ============ File ops (/dev/pico_sysmon) ============

static int sysmon_open(struct inode *inode, struct file *file)
{
	// Sempre disponível
	return 0;
}

static ssize_t sysmon_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
	char kbuf[512];
	int n;

	if (*off > 0)
		return 0; // EOF

	mutex_lock(&g.lock);
	n = build_stats_report(kbuf, sizeof(kbuf));
	mutex_unlock(&g.lock);

	if (n < 0)
		return -EINVAL;

	if (len < (size_t)n)
		n = (int)len;

	if (copy_to_user(buf, kbuf, n))
		return -EFAULT;

	*off += n;
	return n;
}

static ssize_t sysmon_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
	size_t copy_len;
	int actual = 0;
	int ret;

	if (len == 0)
		return 0;

	// Salva last_cmd (modo simulado e também para log no modo USB)
	mutex_lock(&g.lock);
	memset(g.last_cmd, 0, sizeof(g.last_cmd));
	copy_len = (len >= sizeof(g.last_cmd)) ? (sizeof(g.last_cmd) - 1) : len;

	if (copy_from_user(g.last_cmd, buf, copy_len)) {
		mutex_unlock(&g.lock);
		return -EFAULT;
	}
	mutex_unlock(&g.lock);

	// Se USB não está online, write "funciona" em modo simulado
	if (!g.usb_online || !g.udev)
		return (ssize_t)len;

	// Se USB online, manda para o Pico via bulk OUT (melhor prova de write real)
	if (len > MAX_PKT)
		len = MAX_PKT;

	if (copy_from_user(g.tx, buf, len))
		return -EFAULT;

	mutex_lock(&g.lock);
	ret = usb_bulk_msg(g.udev,
	                   usb_sndbulkpipe(g.udev, EP_OUT),
	                   g.tx, (int)len, &actual,
	                   2000);
	mutex_unlock(&g.lock);

	if (ret)
		return ret;

	return actual;
}

static const struct file_operations sysmon_fops = {
	.owner = THIS_MODULE,
	.open  = sysmon_open,
	.read  = sysmon_read,
	.write = sysmon_write,
};

// ============ USB driver (ativa/desativa modo USB) ============

static struct usb_device_id id_table[] = {
	{ USB_DEVICE(USB_VID, USB_PID) },
	{}
};
MODULE_DEVICE_TABLE(usb, id_table);

static int pico_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(intf);

	mutex_lock(&g.lock);
	if (g.udev) {
		mutex_unlock(&g.lock);
		pr_err(DRIVER_NAME ": another device already attached\n");
		return -EBUSY;
	}

	g.udev = usb_get_dev(udev);
	g.usb_online = true;
	mutex_unlock(&g.lock);

	pr_info(DRIVER_NAME ": USB device connected (VID=%04x PID=%04x). Mode=USB\n", USB_VID, USB_PID);
	return 0;
}

static void pico_disconnect(struct usb_interface *intf)
{
	mutex_lock(&g.lock);
	if (g.udev) {
		usb_put_dev(g.udev);
		g.udev = NULL;
	}
	g.usb_online = false;
	mutex_unlock(&g.lock);

	pr_info(DRIVER_NAME ": USB device disconnected. Mode=SIMULATED\n");
}

static struct usb_driver pico_usb_driver = {
	.name = DRIVER_NAME,
	.id_table = id_table,
	.probe = pico_probe,
	.disconnect = pico_disconnect,
};

// ============ Module init/exit (cria /dev sempre) ============

static int __init sysmon_init(void)
{
	int ret;

	memset(&g, 0, sizeof(g));
	mutex_init(&g.lock);
	snprintf(g.last_cmd, sizeof(g.last_cmd), "%s", "(none)");
	g.usb_online = false;
	g.udev = NULL;

	// 1) Criar char device sempre
	ret = alloc_chrdev_region(&g.devt, 0, 1, "pico_sysmon");
	if (ret) return ret;

	cdev_init(&g.cdev, &sysmon_fops);
	ret = cdev_add(&g.cdev, g.devt, 1);
	if (ret) goto err_unreg;

	g.cls = class_create("pico_sysmon");
	if (IS_ERR(g.cls)) {
		ret = PTR_ERR(g.cls);
		goto err_cdev;
	}

	g.devnode = device_create(g.cls, NULL, g.devt, NULL, "pico_sysmon");
	if (IS_ERR(g.devnode)) {
		ret = PTR_ERR(g.devnode);
		goto err_class;
	}

	// 2) Registrar USB driver (para quando o Pico conectar)
	ret = usb_register(&pico_usb_driver);
	if (ret) goto err_dev;

	pr_info(DRIVER_NAME ": loaded. /dev/pico_sysmon ready (SIMULATED until USB device connects)\n");
	return 0;

err_dev:
	device_destroy(g.cls, g.devt);
err_class:
	class_destroy(g.cls);
err_cdev:
	cdev_del(&g.cdev);
err_unreg:
	unregister_chrdev_region(g.devt, 1);
	return ret;
}

static void __exit sysmon_exit(void)
{
	usb_deregister(&pico_usb_driver);

	mutex_lock(&g.lock);
	if (g.udev) {
		usb_put_dev(g.udev);
		g.udev = NULL;
	}
	g.usb_online = false;
	mutex_unlock(&g.lock);

	if (g.devnode) device_destroy(g.cls, g.devt);
	if (g.cls) class_destroy(g.cls);
	cdev_del(&g.cdev);
	unregister_chrdev_region(g.devt, 1);

	pr_info(DRIVER_NAME ": unloaded\n");
}

module_init(sysmon_init);
module_exit(sysmon_exit);
