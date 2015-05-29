/**
 * @file	httpUtil.c
 * @brief	HTTP Server Utilities	
 * @version 1.0
 * @date	2014/07/15
 * @par Revision
 *			2014/07/15 - 1.0 Release
 * @author	
 * \n\n @par Copyright (C) 1998 - 2014 WIZnet. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "W7500x.h"

#define Do4  262 //C octave4
#define Do4s 277 //C#
#define Re4  294 //D
#define Re4s 311//D#
#define Mi4  330 //E
#define Fa4  349 //F
#define Fa4s 370 //F#
#define So4  392 //G
#define So4s 415 //G#
#define La4  440 //A
#define La4s 466 //A#
#define Ti4  494 //B
#define Do5  523 //C octave5
#define Do5s 554 //C#
#define Re5  587 //D
#define Re5s 622//D#
#define Mi5  659 //E
#define Fa5  699 //F
#define Fa5s 740 //F#
#define So5  784 //G
#define So5s 831 //G#
#define La5  880 //A
#define La5s 932 //A#
#define Ti5  988 //B

int tones[] = {Mi4, Re4, Do4, Re4, Mi4, Mi4, Mi4, 0, Re4, Re4, Re4, 0, Mi4, So4, So4, 0, 
               Mi4, Re4, Do4, Re4, Mi4, Mi4, Mi4, 0, Re4, Re4, Mi4, Re4, Do4, 0, 0, 0};
int tones2[] = {Do4, So4, Mi4, So4, Do4, So4, Mi4, So4, Do4, So4, Mi4, So4, Do4, So4, Mi4, So4, 
               Do4, So4, Mi4, So4, Do4, So4, Mi4, So4, Do4, So4, Mi4, So4, Do4, So4, Mi4, So4};

int mario1[] = {Mi5,Mi5,0,Mi5,0,Do5,Mi5,0,So5,0,0,0,So4,0,0,0,Do5,0,0,So4,0,0,Mi4,0,Mi4,La4,0,Ti4,0,La4s,La4,0};
int mario2[] = {Fa4s,Fa4,0,Fa4,0,Fa4,Fa4,0,So4,0,0,0,0,0,0,0,Mi4,0,0,Mi4,0,0,Do4,0,Do4,Do4,0,Re4,0,Do4s,Do4,0};
int mario3[] = {Ti4,Ti4,0,Ti4,0,Ti4,Ti4,0,Mi5,0,0,0,Mi5,0,0,0,Mi5,0,0,Do4,0,0,La4,0,La4,Re5,0,Mi5,0,Re5s,Re5,0};
    
int num = 32;
int mario_num = 32;

void delay_ms(__IO uint32_t nCount)
{
    volatile uint32_t delay = nCount * 2500; // approximate loops per ms at 24 MHz, Debug config
    for(; delay != 0; delay--)
        __NOP();
}
               
void tone(PWM_CHn_TypeDef* PWM_CHn, int mr);
void auto_tone(PWM_CHn_TypeDef* PWM_CHn, int mr);

uint8_t http_get_cgi_handler(uint8_t * uri_name, uint8_t * buf, uint32_t * file_len)
{
	uint8_t ret = HTTP_OK;
	uint16_t len = 0;
	
	if(predefined_get_cgi_processor(uri_name, buf, &len))
	{
		;
	}
	else if(strcmp((const char *)uri_name, "example.cgi") == 0)
	{
		// To do
		;
	}
	else
	{
		// CGI file not found
		ret = HTTP_FAILED;
	}

	if(ret)	*file_len = len;
	return ret;
}

uint8_t http_post_cgi_handler(uint8_t * uri_name, st_http_request * p_http_request, uint8_t * buf, uint32_t * file_len)
{
	uint8_t ret = HTTP_OK;
	uint16_t len = 0;
	uint8_t val = 0;
	
	if(predefined_set_cgi_processor(uri_name, p_http_request->URI, buf, &len))
	{
		;
	}
	else if(strcmp((const char *)uri_name, "example.cgi") == 0)
	{
		// To do
		val = 1;
		len = sprintf((char *)buf, "%d", val);
	}
	else
	{
		// CGI file not found
		ret = HTTP_FAILED;
	}

	if(ret)	*file_len = len;
	return ret;
}


uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;	// ret = 1 means 'uri_name' matched
	uint8_t cgibuf[14] = {0, };
	int8_t cgi_dio = -1;
	int8_t cgi_ain = -1;

	uint8_t i;

	if(strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		;//make_json_todo(buf, len);
	}
	else if(strcmp((const char *)uri_name, "get_netinfo.cgi") == 0)
	{
		make_json_netinfo(buf, len);
	}
	else
	{
		;
	}

	return ret;
}


uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;	// ret = '1' means 'uri_name' matched
	uint8_t val = 0;

    
	if(strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		;//val = todo(uri);
		//*len = sprintf((char *)buf, "%d", val);
	}
	// Digital I/O; dio_s, dio_d
	else if(strcmp((const char *)uri_name, "set_LED.cgi") == 0)
	{
		val = set_LED(uri);
		*len = sprintf((char *)buf, "%d", val);
	}	
   	else if(strcmp((const char *)uri_name, "set_AUTO.cgi") == 0)
	{
		val = set_AUTO(uri);
		*len = sprintf((char *)buf, "%d", val);
	}	
	else
	{
		ret = 0;
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Get CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Example of make_jason function */
//void make_json_dio(uint8_t * buf, uint16_t * len, uint8_t pin)
//{
//	uint8_t pin_state 	= Chip_GPIO_GetPinState(LPC_GPIO, dio_ports[pin], dio_pins[pin]);
//	uint8_t pin_dir 	= Chip_GPIO_GetPinDIR(LPC_GPIO, dio_ports[pin], dio_pins[pin]);

//	*len = sprintf((char *)buf, "DioCallback({\"dio_p\":\"%d\",\
//											\"dio_s\":\"%d\",\
//											\"dio_d\":\"%d\"\
//											});",
//											pin,					// Digital io pin number
//											pin_state,				// Digital io status
//											pin_dir					// Digital io directions
//											);
//}

void make_json_netinfo(uint8_t * buf, uint16_t * len)
{
	wiz_NetInfo netinfo;
	ctlnetwork(CN_GET_NETINFO, (void*) &netinfo);

	// DHCP: 1 - Static, 2 - DHCP
	*len = sprintf((char *)buf, "NetinfoCallback({\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\
											\"ip\":\"%d.%d.%d.%d\",\
											\"gw\":\"%d.%d.%d.%d\",\
											\"sn\":\"%d.%d.%d.%d\",\
											\"dns\":\"%d.%d.%d.%d\",\
											\"dhcp\":\"%d\"\
											});",
											netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
											netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
											netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
											netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
											netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
											netinfo.dhcp
											);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Set CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int8_t set_LED(uint8_t * uri)
{
	uint8_t * param;
	uint8_t pin = 0, val = 0;

		
	if((param = get_http_param_value((char *)uri, "pin"))) // GPIO; D0 ~ D15
	{
		pin = (uint8_t)ATOI(param, 10);
		
		if(pin > 15) return -1;
        
        if((param = get_http_param_value((char *)uri, "val"))) 
        {
            val = (uint8_t)ATOI(param,10);
        }

             
        if(pin==0)   tone(PWM_CH7, Do4);
        else if(pin==1) tone(PWM_CH7, Do4s);
        else if(pin==2) tone(PWM_CH7, Re4);
        else if(pin==3) tone(PWM_CH7, Re4s);
        else if(pin==4) tone(PWM_CH7, Mi4);
        else if(pin==5) tone(PWM_CH7, Fa4);
        else if(pin==6) tone(PWM_CH7, Fa4s);
        else if(pin==7) tone(PWM_CH7, So4);
        else if(pin==8) tone(PWM_CH7, So4s);
        else if(pin==9) tone(PWM_CH7, La4);
        else if(pin==10) tone(PWM_CH7, La4s);
        else if(pin==11) tone(PWM_CH7, Ti4);
        else if(pin==12) tone(PWM_CH7, Do5);
        else if(pin==13) tone(PWM_CH7, Do5s);
        else if(pin==14) tone(PWM_CH7, Re5);
        else if(pin==15) tone(PWM_CH7, Re5s);
        else if(pin==16) tone(PWM_CH7, Mi5);
        else if(pin==17) tone(PWM_CH7, Fa5);
        else if(pin==18) tone(PWM_CH7, Fa5s);
        else if(pin==19) tone(PWM_CH7, So5);
        else if(pin==20) tone(PWM_CH7, So5s);
        else if(pin==21) tone(PWM_CH7, La5);
        else if(pin==22) tone(PWM_CH7, La5s);
        else if(pin==23) tone(PWM_CH7, Ti5);	
	}

	return pin;
}

int8_t set_AUTO(uint8_t * uri)
{
	uint8_t * param;
	uint8_t pin = 0, val = 0; 
    int i=0;

		
	if((param = get_http_param_value((char *)uri, "pin"))) // GPIO; D0 ~ D15
	{
		pin = (uint8_t)ATOI(param, 10);
		
		if(pin > 15) return -1;
        
        if((param = get_http_param_value((char *)uri, "val"))) 
        {
            val = (uint8_t)ATOI(param,10);
        }

             
        if(pin==0)   
            {
                for(i=0; i<num; i++)
                {
                    /*tone*/
                    auto_tone(PWM_CH7, tones[i]);
                    auto_tone(PWM_CH6, tones2[i]);
                    delay_ms(100);
                }
                PWM_CHn_Stop(PWM_CH7);
                PWM_CHn_Stop(PWM_CH6);
            }
            
        else if(pin==1)
            {
                for(i=0; i<mario_num; i++)
                {
                    /*tone*/
                    auto_tone(PWM_CH7, mario1[i]);
                    auto_tone(PWM_CH6, mario2[i]);
                    auto_tone(PWM_CH5, mario3[i]);
                    delay_ms(100);
                }
                PWM_CHn_Stop(PWM_CH7);
                PWM_CHn_Stop(PWM_CH6);
                PWM_CHn_Stop(PWM_CH5);
            }
        else if(pin==2) tone(PWM_CH7, Re4);
        else if(pin==3) tone(PWM_CH7, Re4s);
        else if(pin==4) tone(PWM_CH7, Mi4);
        else if(pin==5) tone(PWM_CH7, Fa4);
        else if(pin==6) tone(PWM_CH7, Fa4s);
        else if(pin==7) tone(PWM_CH7, So4);
        else if(pin==8) tone(PWM_CH7, So4s);
        else if(pin==9) tone(PWM_CH7, La4);
        else if(pin==10) tone(PWM_CH7, La4s);
        else if(pin==11) tone(PWM_CH7, Ti4);
        else if(pin==12) tone(PWM_CH7, Do5);
        else if(pin==13) tone(PWM_CH7, Do5s);
        else if(pin==14) tone(PWM_CH7, Re5);
        else if(pin==15) tone(PWM_CH7, Re5s);
        else if(pin==16) tone(PWM_CH7, Mi5);
        else if(pin==17) tone(PWM_CH7, Fa5);
        else if(pin==18) tone(PWM_CH7, Fa5s);
        else if(pin==19) tone(PWM_CH7, So5);
        else if(pin==20) tone(PWM_CH7, So5s);
        else if(pin==21) tone(PWM_CH7, La5);
        else if(pin==22) tone(PWM_CH7, La5s);
        else if(pin==23) tone(PWM_CH7, Ti5);	
	}

	return pin;
}