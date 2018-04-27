#include <stdio.h>
#include "platform.h"
#include "xintc.h"
#include "xparameters.h"
#include "xio.h"
#include "xil_exception.h"
#include "vga_periph_mem.h"

XIntc Intc;

void interrupt_handler(void * baseaddr_p) {
	xil_printf("\n\rtimer timeout.");
	clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
	draw_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 8);
}
int main()
{
	 init_platform();
	 XStatus Status;
	 Xuint32 value1 = 480U, value2 = 0x1;

	 xil_printf("Interrupt example\n\r");
	 //Set Terminal count for my_timer
	 XIo_Out32(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x7*0x4, value1);
	 // Run my_timer
	 XIo_Out32(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x8*0x4, value2);

	 //initialize interrupt controller
	 Status = XIntc_Initialize (&Intc, XPAR_INTC_0_DEVICE_ID);
	 if (Status != XST_SUCCESS) xil_printf ("\r\nInterrupt controller initialization failure");
	 else xil_printf("\r\nInterrupt controller initialized");

	 // Connect my_timer_interrupt_handler
	 Status = XIntc_Connect (&Intc, XPAR_AXI_INTC_0_DEVICE_ID,
	 (XInterruptHandler) interrupt_handler,(void *)0);
	 if (Status != XST_SUCCESS) xil_printf ("\r\nRegistering MY_TIMER Interrupt Failed");
	 else xil_printf("\r\nMY_TIMER Interrupt registered");
	 //start the interrupt controller in real mode
	 Status = XIntc_Start(&Intc, XIN_REAL_MODE);
	 //enable interrupt controller
	 XIntc_Enable (&Intc, XPAR_AXI_INTC_0_DEVICE_ID);
	 microblaze_enable_interrupts();
	 VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0);// direct mode   0
	 VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3);// display_mode  1
	 VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x1);// show frame      2

	 VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0x1);// font size       3

	 VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0xFFFFFF);// foreground 4
	 VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xFF0000);// frame color      6

	 while (1){
		 /*value3 = XIo_In32(XPAR_MY_TIMER_0_BASEADDR + 0x0);
		 value1 = XIo_In32(XPAR_MY_TIMER_0_BASEADDR + 0x4);
		 value2 = XIo_In32(XPAR_MY_TIMER_0_BASEADDR + 0x8);
		 xil_printf("\n\rvalue1 = %x, value2 = %x, value3 = %x.", value1, value2, value3);*/


	 }
	 cleanup_platform();
	 return 0;
}
