#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x945918ef, "module_layout" },
	{ 0x70684b04, "cdev_del" },
	{ 0x50db23de, "class_destroy" },
	{ 0xd79e45c6, "device_destroy" },
	{ 0x92513a97, "cdev_add" },
	{ 0x35a75fe6, "cdev_init" },
	{ 0x83a5b739, "device_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xe56ec7cc, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "18BFC06C1C82135FA1C23A0");
