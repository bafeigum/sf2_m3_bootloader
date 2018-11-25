#include "drivers/mss_gpio/mss_gpio.h"
#include "drivers/mss_uart/mss_uart.h"

#include "CMSIS/system_m2sxxx.h"


#define Microsemi_logo \
"\n\r\n\r \
**     ** *******  ****** *****    ****    ***** ****** **     ** *******\n\r \
* *   * *    *    *       *    *  *    *  *      *      * *   * *    *   \n\r \
*  * *  *    *    *       *****   *    *   ****  ****** *  * *  *    *   \n\r \
*   *   *    *    *       *    *  *    *       * *      *   *   *    *   \n\r \
*       * *******  ****** *    *   ****   *****  ****** *       * *******\n\r"

/*==============================================================================
  Private functions.
 */
static void delay(void);



#define UHSA_ESCAPE '['
#define UHSA_ESCAPE_END ']'


void alt_putchar(char ch) {
	char buf[2];
	buf[0] = ch;
	buf[1] = 0;
	MSS_UART_polled_tx_string( &g_mss_uart0, buf);

}

/*
MSS_UART_get_rx
(
    mss_uart_instance_t * this_uart,
    uint8_t * rx_buff,
    size_t buff_size
)
*/

char alt_getchar() {
	char buf[2];
	buf[0] = 0;
	buf[1] = 0;
	MSS_UART_get_rx( &g_mss_uart0, buf, 1 );
	return buf[0];

}

void print_hex_char(int val) {
	char tempchar;
	tempchar = (val & 0x0f) + '0'; // 0..9 => 0x30..0x39
	if (tempchar>'9') {
		tempchar+= 39;
	}
	alt_putchar(tempchar);
}

void print_hex_16(int val) {
	print_hex_char(val >> 12);
	print_hex_char(val >> 8);
	print_hex_char(val >> 4);
	print_hex_char(val);
}


/*==============================================================================
 * main() function.
 */

int main()
{
	const uint8_t greeting[] = "\n\rRISC-V SoftCPU Contest 2018 bootloader for bit_riscv\n\rCredit to Antti Lukats\n\rUsing Cortex-M3 assisted boot concept\n\rRev: 0.0.2\n\r";


	const int* AddressFabricRAM = 0x50000000;
	volatile int* lsram;
	lsram = AddressFabricRAM;

	const int* Address_NVM_RV32I = 0x60018000;
	volatile int* nvm_rv32i;
	nvm_rv32i = Address_NVM_RV32I;

	const int* Address_NVM_APP = 0x60010000;
	volatile int* nvm_app;
	nvm_app = Address_NVM_APP;

	const int* Address_SRAM_APP = 0x20000000;
	volatile int* sram_app;
	sram_app = Address_SRAM_APP;


	// UHSA variables
	char c = 0;				// First char received
	char c0;
	unsigned int b;		// c converted to hex nibble
	char in_escape = 0;
	char is_nibble = 0;
	char done = 0;
	unsigned int channel = 0;
	unsigned char temp;

	int i;


    /*
     * Initialize MSS GPIOs.
     */
    MSS_GPIO_init();
    // Reset output!
    MSS_GPIO_config( MSS_GPIO_0 , MSS_GPIO_OUTPUT_MODE );

    MSS_GPIO_set_outputs(0); // keep RISC-V in reset

    /* Initialize and configure UART0. */
    MSS_UART_init
     (
     &g_mss_uart0,
     MSS_UART_115200_BAUD,
     MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
     );


    //alt_putchar(0x1B);  alt_putchar('H');
    //alt_putchar(0x1B);  alt_putchar('J');


    /* Send the Microsemi Logo over the UART_0 */
    MSS_UART_polled_tx_string( &g_mss_uart0, (const uint8_t *)Microsemi_logo);
    /* Send greeting message over the UART_0 */
    MSS_UART_polled_tx( &g_mss_uart0, greeting, sizeof(greeting) );


    /*
     *
     */


//    for (mem = AddressToTest; mem < AddressToTestEnd; mem++)
    lsram = AddressFabricRAM;
    nvm_rv32i = Address_NVM_RV32I;
    sram_app = Address_SRAM_APP;
    unsigned int w;

    // Copy to LSRAM in FPGA Fabric
    for (i=0; i < 512; i++)
    {
    	w = *nvm_rv32i; nvm_rv32i++;

    	//*sram_app = w & 0xFFFF;    	sram_app++;
    	*lsram    = w & 0xFFFF;    	lsram++;
		//print_hex_16(w); alt_putchar(10);alt_putchar(13);

    	//*sram_app = w >> 16;    	sram_app++;
    	*lsram    = w >> 16;    	lsram++;
		//print_hex_16(w >> 16); alt_putchar(10);alt_putchar(13);
    }

	MSS_UART_polled_tx_string( &g_mss_uart0, "Fabric LSRAM loaded\n\r\0");

    // Copy to eSRAM in FPGA Fabric 32K filled to 64K region
    for (i=0; i < 8192; i++)
    {
    	w = *nvm_app; nvm_app++;

    	*sram_app = w;    	sram_app++;
		//print_hex_16(w); alt_putchar(10);alt_putchar(13);

    	//*sram_app = w >> 16;    	sram_app++;
		//print_hex_16(w >> 16); alt_putchar(10);alt_putchar(13);
		//print_hex_16(i); alt_putchar(10);alt_putchar(13);
    }

	MSS_UART_polled_tx_string( &g_mss_uart0, "eNVM copy to eSRAM done\n\r\0");

	// Todo copy RISC-V code to eSRAM if desired so



    for (i=0; i < 25; i++)
    {
		delay();
		alt_putchar('.');
    }
    MSS_UART_polled_tx_string( &g_mss_uart0, " -> RISC-V SoftCPU is running\n\r\0");


	// We should sleep here a bit to let uart string to come out from TXD line...
    MSS_GPIO_set_outputs(1); // release RISC-V in reset

	//while (1) {	}

    int x;
/*
    *lsram = 0x55;

    if (*lsram != 0x55) {
        MSS_UART_polled_tx_string( &g_mss_uart0, "LSRAM Failed!\0");

    } else {
        MSS_UART_polled_tx_string( &g_mss_uart0, "LSRAM OK!\0");
        print_hex_char(*lsram);

    }
*/


    /* Event loop never exits. */
    while (1) {
    	__WFI;
    };

}

/*==============================================================================
  Delay between displays of the watchdog counter value.
 */
static void delay(void)
{
    volatile uint32_t delay_count = SystemCoreClock / 128u;

    while(delay_count > 0u)
    {
        --delay_count;
    }
}
