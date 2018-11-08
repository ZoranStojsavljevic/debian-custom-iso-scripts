root@dltv83:~# in_realroot 
(REALROOT)root@dltv83:/# rm /lib/modules/4.18.16/kernel/drivers/gpu/drm/i915/i915.ko 
rm: remove regular file '/lib/modules/4.18.16/kernel/drivers/gpu/drm/i915/i915.ko'? y
(REALROOT)root@dltv83:/# depmod -a
(REALROOT)root@dltv83:/# exit
exit
root@dltv83:~# reboot
Shared connection to mde closed.
