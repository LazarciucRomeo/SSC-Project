#include "PmodBT2.h"
#include "PmodMAXSONAR.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "stdio.h"
#include "string.h"

// Required definitions for sending & receiving data over host board's UART port
#ifdef __MICROBLAZE__
#include "xuartlite.h"
typedef XUartLite SysUart;
#define CLK_FREQ XPAR_CPU_M_AXI_DP_FREQ_HZ
#define SysUart_Send            XUartLite_Send
#define SysUart_Recv            XUartLite_Recv
#define SYS_UART_DEVICE_ID      XPAR_AXI_UARTLITE_0_DEVICE_ID
#define BT2_UART_AXI_CLOCK_FREQ XPAR_CPU_M_AXI_DP_FREQ_HZ
#define PMOD_MAXSONAR_BASEADDR XPAR_PMODMAXSONAR_0_AXI_LITE_GPIO_BASEADDR
#endif

#define ACCEPTED_MESSAGE "Accepted request"
#define DENIED_MESSAGE "Invalid message"
#define USER_ROMEO "Romeo"

PmodBT2 pmodBtDevice;
PmodMAXSONAR maxSonarDevice;
SysUart myUart;

void DemoRun();
void SysUartInit();
void EnableCaches();
void DisableCaches();
void CleanUp();
void MaxSonar_Init();
void PmodBt_Init();
char* ReadPmod();
void SignalBt2();

int main() {
	MaxSonar_Init();
	PmodBt_Init();
	DemoRun();
	CleanUp();
	return XST_SUCCESS;
}

void MaxSonar_Init(){
	EnableCaches();
	MAXSONAR_begin(&maxSonarDevice, PMOD_MAXSONAR_BASEADDR, CLK_FREQ);
}

void PmodBt_Init(){
	EnableCaches();
	SysUartInit();
	BT2_Begin (
		&pmodBtDevice,
		XPAR_PMODBT2_0_AXI_LITE_GPIO_BASEADDR,
		XPAR_PMODBT2_0_AXI_LITE_UART_BASEADDR,
		BT2_UART_AXI_CLOCK_FREQ,
		115200
	);
}

/*
 * function that appends to the end of the string
 * given as the first parameter
 * another character
 * and terminates the string accordingly (with return character)
 */
void append(char* dest, unsigned char* source) {
	size_t len = strlen(dest);
	dest[len] = *source;
	dest[len+1] = '\0';
}

/*
 * function that reads information from the BT2 device
 * and if the information is the expected string defined earlier
 * in this case "Romeo" it will return a message of success
 * in other cases, an error message will be returned
 * the return value is a string
 */
char* ReadPmod(){
	u8 buffer[1];
	char bt2_message[100] = "";
	int n;

    n = BT2_RecvData(&pmodBtDevice, buffer, 1);

    //something received from the device
    if(n != 0){

		//not a letter (symbol or number)
		if((int)buffer < 65 || (int)buffer > 122)
			return DENIED_MESSAGE;

		append(bt2_message, buffer);

		if(strcmp(bt2_message, USER_ROMEO))
		   //expected message for stopping the system
		   return ACCEPTED_MESSAGE;
		else if (sizeof(bt2_message) > sizeof(USER_ROMEO))
		   //message was incorrect
		   return DENIED_MESSAGE;
    }

    return "";
}

/*
 * function that sends information to the bt2 device
 * according to what the device has sent previously
 * and the return value of the ReadPmod function
 */
void SignalBt2(){
	char* response = ReadPmod();

	//signal the response to the bt2 device
    BT2_SendData(&pmodBtDevice, response, sizeof(response));
}


/*
 * main function containing the main loop for the devices
 */
void DemoRun() {
   u8 message[20]; //message to be sent to BT2
   u32 dist; //distance from the MAXSONAR device

   //main loop
   while (1) {
	   //get distance from MAXSONAR
	   dist = MAXSONAR_getDistance(&maxSonarDevice);
	   xil_printf("distance (in) = %3d\r", dist);

	   //send distance to BT2
	   for(int i = 0; i < 4; i++)
	   {
	     message[i] = dist & 0xff000000;
	     dist <<= 8;
	     BT2_SendData(&pmodBtDevice, message, 8);
	   }

       usleep(1000000);

   }
 }

// Initialize the system UART device
void SysUartInit() {
#ifdef __MICROBLAZE__
   // AXI Uartlite for MicroBlaze
   XUartLite_Initialize(&myUart, SYS_UART_DEVICE_ID);
#endif
}

void EnableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheEnable();
#endif
#endif
}

void DisableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheDisable();
#endif
#endif
}

void CleanUp(){
	DisableCaches();
}
