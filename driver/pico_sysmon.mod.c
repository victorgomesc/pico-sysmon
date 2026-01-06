#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0xefd5d5d8, "cdev_init" },
	{ 0x4c1dbbd9, "cdev_add" },
	{ 0xea5ac1d9, "class_create" },
	{ 0xf98f93a7, "device_create" },
	{ 0xaba46e12, "usb_register_driver" },
	{ 0x6fdeeff0, "device_destroy" },
	{ 0x14fcde53, "class_destroy" },
	{ 0x0c72f9ad, "cdev_del" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0xef4e4365, "usb_deregister" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x76a26ca1, "usb_bulk_msg" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xe4de56b4, "__ubsan_handle_load_invalid_value" },
	{ 0xa9289d30, "usb_get_dev" },
	{ 0xc7ffe1aa, "si_meminfo" },
	{ 0x058c185a, "jiffies" },
	{ 0xe199f25f, "jiffies_to_msecs" },
	{ 0xdeb05e81, "thermal_zone_get_zone_by_name" },
	{ 0x6e550cfe, "thermal_zone_get_temp" },
	{ 0x318e45ff, "filp_open" },
	{ 0x4cd313ad, "kernel_read" },
	{ 0xdb9a5310, "filp_close" },
	{ 0xd09b06f5, "kstrtoint" },
	{ 0x7ef085e1, "avenrun" },
	{ 0x40a621c5, "scnprintf" },
	{ 0x992ecee6, "kernel_cpustat" },
	{ 0xf296206e, "nr_cpu_ids" },
	{ 0xb5c51982, "__cpu_online_mask" },
	{ 0x86632fd6, "_find_next_bit" },
	{ 0x5ae9ee26, "__per_cpu_offset" },
	{ 0x32460570, "pci_get_device" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0xf64ac983, "__copy_overflow" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xf46d5bf3, "mutex_lock" },
	{ 0x5d5b6583, "usb_put_dev" },
	{ 0xf46d5bf3, "mutex_unlock" },
	{ 0xe8213e80, "_printk" },
	{ 0xc1e6c71e, "__mutex_init" },
	{ 0xba157484, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x9f222e1e,
	0xefd5d5d8,
	0x4c1dbbd9,
	0xea5ac1d9,
	0xf98f93a7,
	0xaba46e12,
	0x6fdeeff0,
	0x14fcde53,
	0x0c72f9ad,
	0x0bc5fb0d,
	0xef4e4365,
	0xa61fd7aa,
	0x092a35a2,
	0x76a26ca1,
	0xd272d446,
	0xe4de56b4,
	0xa9289d30,
	0xc7ffe1aa,
	0x058c185a,
	0xe199f25f,
	0xdeb05e81,
	0x6e550cfe,
	0x318e45ff,
	0x4cd313ad,
	0xdb9a5310,
	0xd09b06f5,
	0x7ef085e1,
	0x40a621c5,
	0x992ecee6,
	0xf296206e,
	0xb5c51982,
	0x86632fd6,
	0x5ae9ee26,
	0x32460570,
	0x092a35a2,
	0xf64ac983,
	0x90a48d82,
	0xd272d446,
	0xd272d446,
	0xf46d5bf3,
	0x5d5b6583,
	0xf46d5bf3,
	0xe8213e80,
	0xc1e6c71e,
	0xba157484,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"cdev_add\0"
	"class_create\0"
	"device_create\0"
	"usb_register_driver\0"
	"device_destroy\0"
	"class_destroy\0"
	"cdev_del\0"
	"unregister_chrdev_region\0"
	"usb_deregister\0"
	"__check_object_size\0"
	"_copy_from_user\0"
	"usb_bulk_msg\0"
	"__stack_chk_fail\0"
	"__ubsan_handle_load_invalid_value\0"
	"usb_get_dev\0"
	"si_meminfo\0"
	"jiffies\0"
	"jiffies_to_msecs\0"
	"thermal_zone_get_zone_by_name\0"
	"thermal_zone_get_temp\0"
	"filp_open\0"
	"kernel_read\0"
	"filp_close\0"
	"kstrtoint\0"
	"avenrun\0"
	"scnprintf\0"
	"kernel_cpustat\0"
	"nr_cpu_ids\0"
	"__cpu_online_mask\0"
	"_find_next_bit\0"
	"__per_cpu_offset\0"
	"pci_get_device\0"
	"_copy_to_user\0"
	"__copy_overflow\0"
	"__ubsan_handle_out_of_bounds\0"
	"__fentry__\0"
	"__x86_return_thunk\0"
	"mutex_lock\0"
	"usb_put_dev\0"
	"mutex_unlock\0"
	"_printk\0"
	"__mutex_init\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:vCAFEp5002d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "359AFBCBEA12118AC74BCB2");
