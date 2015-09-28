#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x8efb7d0c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc50732fc, __VMLINUX_SYMBOL_STR(settdf) },
	{ 0x3d26547f, __VMLINUX_SYMBOL_STR(find_vpid) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xf2b1ac8e, __VMLINUX_SYMBOL_STR(__getnstimeofday) },
	{ 0x3fa26a85, __VMLINUX_SYMBOL_STR(pid_task) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x61e80937, __VMLINUX_SYMBOL_STR(kill_pgrp) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4B2D8E6C7FBAAEE25D87AE5");
