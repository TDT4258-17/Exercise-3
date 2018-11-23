// #include <stdint.h>

// GPIO

#define GPIO_PA_BASE 0x40006000
#define GPIO_PB_BASE 0x40006024
#define GPIO_PC_BASE 0x40006048

#define GPIO_PA_CTRL     ((volatile unsigned int*)(GPIO_PA_BASE + 0x00))
#define GPIO_PA_MODEL    ((volatile unsigned int*)(GPIO_PA_BASE + 0x04))
#define GPIO_PA_MODEH    ((volatile unsigned int*)(GPIO_PA_BASE + 0x08))
#define GPIO_PA_DOUT     ((volatile unsigned int*)(GPIO_PA_BASE + 0x0c))
#define GPIO_PA_DOUTSET  ((volatile unsigned int*)(GPIO_PA_BASE + 0x10))
#define GPIO_PA_DOUTCLR  ((volatile unsigned int*)(GPIO_PA_BASE + 0x14))
#define GPIO_PA_DOUTTGL  ((volatile unsigned int*)(GPIO_PA_BASE + 0x18))
#define GPIO_PA_DIN      ((volatile unsigned int*)(GPIO_PA_BASE + 0x1c))
#define GPIO_PA_PINLOCKN ((volatile unsigned int*)(GPIO_PA_BASE + 0x20))

#define GPIO_PB_CTRL     ((volatile unsigned int*)(GPIO_PB_BASE + 0x00))
#define GPIO_PB_MODEL    ((volatile unsigned int*)(GPIO_PB_BASE + 0x04))
#define GPIO_PB_MODEH    ((volatile unsigned int*)(GPIO_PB_BASE + 0x08))
#define GPIO_PB_DOUT     ((volatile unsigned int*)(GPIO_PB_BASE + 0x0c))
#define GPIO_PB_DOUTSET  ((volatile unsigned int*)(GPIO_PB_BASE + 0x10))
#define GPIO_PB_DOUTCLR  ((volatile unsigned int*)(GPIO_PB_BASE + 0x14))
#define GPIO_PB_DOUTTGL  ((volatile unsigned int*)(GPIO_PB_BASE + 0x18))
#define GPIO_PB_DIN      ((volatile unsigned int*)(GPIO_PB_BASE + 0x1c))
#define GPIO_PB_PINLOCKN ((volatile unsigned int*)(GPIO_PB_BASE + 0x20))

#define GPIO_PC_CTRL     ((volatile unsigned int*)(GPIO_PC_BASE + 0x00))
#define GPIO_PC_MODEL    ((volatile unsigned int*)(GPIO_PC_BASE + 0x04))
#define GPIO_PC_MODEH    ((volatile unsigned int*)(GPIO_PC_BASE + 0x08))
#define GPIO_PC_DOUT     ((volatile unsigned int*)(GPIO_PC_BASE + 0x0c))
#define GPIO_PC_DOUTSET  ((volatile unsigned int*)(GPIO_PC_BASE + 0x10))
#define GPIO_PC_DOUTCLR  ((volatile unsigned int*)(GPIO_PC_BASE + 0x14))
#define GPIO_PC_DOUTTGL  ((volatile unsigned int*)(GPIO_PC_BASE + 0x18))
#define GPIO_PC_DIN      ((volatile unsigned int*)(GPIO_PC_BASE + 0x1c))
#define GPIO_PC_PINLOCKN ((volatile unsigned int*)(GPIO_PC_BASE + 0x20))

#define GPIO_EXTIPSELL ((volatile unsigned int*)(GPIO_PA_BASE + 0x100))
#define GPIO_EXTIPSELH ((volatile unsigned int*)(GPIO_PA_BASE + 0x104))
#define GPIO_EXTIRISE  ((volatile unsigned int*)(GPIO_PA_BASE + 0x108))
#define GPIO_EXTIFALL  ((volatile unsigned int*)(GPIO_PA_BASE + 0x10c))
#define GPIO_IEN       ((volatile unsigned int*)(GPIO_PA_BASE + 0x110))
#define GPIO_IFC       ((volatile unsigned int*)(GPIO_PA_BASE + 0x11c))

// CMU

#define CMU_BASE2 0x400c8000

#define CMU_HFPERCLKDIV  ((volatile unsigned int*)(CMU_BASE2 + 0x008))
#define CMU_HFCORECLKEN0 ((volatile unsigned int*)(CMU_BASE2 + 0x040))
#define CMU_HFPERCLKEN0  ((volatile unsigned int*)(CMU_BASE2 + 0x044))
#define CMU_CMD          ((volatile unsigned int*)(CMU_BASE2 + 0x024))

#define CMU2_HFPERCLKEN0_DAC0   (1 << 17)
#define CMU2_HFPERCLKEN0_PRS    (1 << 15)
#define CMU2_HFPERCLKEN0_GPIO   (1 << 13)
#define CMU2_HFPERCLKEN0_TIMER1 (1 << 6)

#define CMU_HFCORECLKEN0_DMA (1 << 0)

// TIMER1

#define TIMER1_BASE 0x40010400

#define TIMER1_CMD ((volatile unsigned int*)(TIMER1_BASE + 0x04))
#define TIMER1_IEN ((volatile unsigned int*)(TIMER1_BASE + 0x0c))
#define TIMER1_IFC ((volatile unsigned int*)(TIMER1_BASE + 0x18))
#define TIMER1_TOP ((volatile unsigned int*)(TIMER1_BASE + 0x1c))
#define TIMER1_CNT ((volatile unsigned int*)(TIMER1_BASE + 0x24))

// NVIC

#define ISER0 ((volatile unsigned int*)0xe000e100)
#define ISER1 ((volatile unsigned int*)0xe000e104)
#define ICER0 ((volatile unsigned int*)0xe000e180)
#define ICER1 ((volatile unsigned int*)0xe000e184)
#define ISPR0 ((volatile unsigned int*)0xe000e200)
#define ISPR1 ((volatile unsigned int*)0xe000e204)
#define ICPR0 ((volatile unsigned int*)0xe000e280)
#define ICPR1 ((volatile unsigned int*)0xe000e284)
#define IABR0 ((volatile unsigned int*)0xe000e300)
#define IABR1 ((volatile unsigned int*)0xe000e304)

// IPR

#define IPR_BASE  0xe000e400

#define IPR0 ((volatile unsigned int*)(IPR_BASE + 0x00))
#define IPR1 ((volatile unsigned int*)(IPR_BASE + 0x04))
#define IPR2 ((volatile unsigned int*)(IPR_BASE + 0x08))
#define IPR3 ((volatile unsigned int*)(IPR_BASE + 0x0c))

// EMU

#define EMU_BASE2 0x400c6000

#define EMU_CTRL ((volatile unsigned int*)(EMU_BASE2 + 0x000))

// DAC0

#define DAC0_BASE2 0x40004000

#define DAC0_CTRL     ((volatile unsigned int*)(DAC0_BASE2 + 0x000))
#define DAC0_CH0CTRL  ((volatile unsigned int*)(DAC0_BASE2 + 0x008))
#define DAC0_CH1CTRL  ((volatile unsigned int*)(DAC0_BASE2 + 0x00c))
#define DAC0_IEN      ((volatile unsigned int*)(DAC0_BASE2 + 0x010))
#define DAC0_IF       ((volatile unsigned int*)(DAC0_BASE2 + 0x014))
#define DAC0_IFS      ((volatile unsigned int*)(DAC0_BASE2 + 0x018))
#define DAC0_IFC      ((volatile unsigned int*)(DAC0_BASE2 + 0x01c))
#define DAC0_CH0DATA  ((volatile unsigned int*)(DAC0_BASE2 + 0x020))
#define DAC0_CH1DATA  ((volatile unsigned int*)(DAC0_BASE2 + 0x024))
#define DAC0_COMBDATA ((volatile unsigned int*)(DAC0_BASE2 + 0x028))

// DMA

#define DMA_BASE 0x400c2000

#define DMA_STATUS      ((volatile unsigned int*)(DMA_BASE + 0x0000))
#define DMA_CONFIG      ((volatile unsigned int*)(DMA_BASE + 0x0004))
#define DMA_CTRLBASE    ((volatile unsigned int*)(DMA_BASE + 0x0008))
#define DMA_ALTCTRLBASE ((volatile unsigned int*)(DMA_BASE + 0x000c))
#define DMA_CHUSEBURSTS ((volatile unsigned int*)(DMA_BASE + 0x0018))
#define DMA_CHUSEBURSTC ((volatile unsigned int*)(DMA_BASE + 0x001c))
#define DMA_REQMASKC    ((volatile unsigned int*)(DMA_BASE + 0x0024))
#define DMA_CHENS       ((volatile unsigned int*)(DMA_BASE + 0x0028))
#define DMA_CHALTC      ((volatile unsigned int*)(DMA_BASE + 0x0034))
#define DMA_IFC         ((volatile unsigned int*)(DMA_BASE + 0x1008))
#define DMA_IEN         ((volatile unsigned int*)(DMA_BASE + 0x100c))
#define DMA_CH0_CTRL    ((volatile unsigned int*)(DMA_BASE + 0x1100))

// PRS

#define PRS_BASE 0x400cc000

#define PRS_CH0_CTRL ((volatile unsigned int*)(PRS_BASE + 0x010))

// System Control Block

#define SCR          ((volatile unsigned int*)0xe000ed10)
#define SYSTICK_CTRL ((volatile unsigned int*)0xe000e010)
#define SYSTICK_LOAD ((volatile unsigned int*)0xe000e014)

