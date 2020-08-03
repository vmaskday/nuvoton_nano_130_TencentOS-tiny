# nuvoton_nano_130_TencentOS-tiny
nuvoton nano130ke3bn for TencentOS tiny

移植新唐nano130ke3bn到腾讯的TencentOS tiny.
1. 系统调度使用的是SysTick.
2. 系统的时钟配置用的是新唐的工具配置之后手动修改的，主系统频率设置的是48Mhz
3. 配置了简单的tickless模式，支持 TOS_LOW_POWER_MODE_SLEEP .基于SysTick实现的
