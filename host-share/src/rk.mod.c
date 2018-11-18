#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x14522340, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0x7edc1537, "device_destroy" },
	{ 0xfa0d49c7, "__register_chrdev" },
	{ 0xc2f57cb9, "kobject_del" },
	{ 0x343a1a8, "__list_add" },
	{ 0xea147363, "printk" },
	{ 0xa9d45217, "class_unregister" },
	{ 0xb4390f9a, "mcount" },
	{ 0x521445b, "list_del" },
	{ 0x2d2cf7d, "device_create" },
	{ 0xe50e8a2d, "kobject_add" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x37a0cba, "kfree" },
	{ 0xe06bb002, "class_destroy" },
	{ 0xa2654165, "__class_create" },
	{ 0x3302b500, "copy_from_user" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D755EC7CFF933693B947DB1");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 10,
	.rhel_release = 754,
};
#ifdef RETPOLINE
	MODULE_INFO(retpoline, "Y");
#endif
