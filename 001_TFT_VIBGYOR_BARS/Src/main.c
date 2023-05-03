

#include "stdint.h"


#include "bsp_lcd.h"

#define RGB888(r,g,b)  (((r) << 16) | ((g) << 8) | (b))

#define VIOLET   	RGB888(148,0,211)
#define INDIGO   	RGB888(75,0,130)
#define BLUE   		RGB888(0,0,255)
#define GREEN   	RGB888(0,255,0)
#define YELLOW   	RGB888(255,255,0)
#define ORANGE   	RGB888(255,127,0)
#define RED   		RGB888(255,0,0)
#define WHITE   	RGB888(255,255,255)
#define BLACK		RGB888(0,0,0)

void RCC_Config(void);
void LTDC_Pin_Init(void);
void LTC_Init(void);
void LTDC_Layer_Init(LTDC_Layer_TypeDef *pLayer);
int main(void)
{
	RCC_Config();
	BSP_LCD_Init();
	BSP_LCD_Set_Orientation(LANDSCAPE);
	LTDC_Pin_Init();
	LTC_Init();
	LTDC_Layer_Init(LTDC_Layer1);
    bsp_lcd_set_fb_background_color(BLACK);
	bsp_lcd_fill_rect(VIOLET, 0, 319, 35*0, 35);
	bsp_lcd_fill_rect(INDIGO, 0, 319, 35*1, 35);
	bsp_lcd_fill_rect(BLUE,   0, 319, 35*2, 35);
	bsp_lcd_fill_rect(GREEN,  0, 319, 35*3, 35);
	bsp_lcd_fill_rect(YELLOW, 0, 319, 35*4, 35);
	bsp_lcd_fill_rect(ORANGE, 0, 319, 35*5, 35);
	bsp_lcd_fill_rect(RED, 	  0, 319, 35*6, 30);

	for(;;);
}
void LTDC_Pin_Init(void)
{
	//enable the peripheral clock for GPIO ports involved in LTDC interface
	REG_SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOAEN_Pos);
	REG_SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOBEN_Pos);
	REG_SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOCEN_Pos);
	REG_SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIODEN_Pos);
	REG_SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOGEN_Pos);
	REG_SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOFEN_Pos);

	for(int i = 0 ; i < total_ltdc_pins ;i++)
	{
		REG_SET_VAL(ltdc_io_ports[i]->MODER,2U,0x3U,(ltdc_pins[i] * 2U));
		REG_CLR_BIT(ltdc_io_ports[i]->OTYPER,ltdc_pins[i]);
		REG_SET_VAL(ltdc_io_ports[i]->OSPEEDR,2U,0x3U,(ltdc_pins[i] * 2U));
		if(ltdc_pins[i] < 8)
			REG_SET_VAL(ltdc_io_ports[i]->AFR[0],14U,0xFU,(ltdc_pins[i] * 4U));
		else
			REG_SET_VAL(ltdc_io_ports[i]->AFR[1],14U,0xFU,((ltdc_pins[i] % 8) * 4U));
	}

}

void RCC_Config(void)
{
   RCC_TypeDef *pRCC = RCC;
   FLASH_TypeDef *pFlash = FLASH;
   PWR_TypeDef *pPWR = PWR;


	//Program flash wait states
	REG_SET_VAL(pFlash->ACR,0x5U,0xFU,FLASH_ACR_LATENCY_Pos);

	//2. Over drive settings
	REG_SET_BIT(pRCC->APB1ENR,RCC_APB1ENR_PWREN_Pos); 	/*Enable clock for PWR register access*/
	REG_SET_VAL(pPWR->CR,0x3U,0x3U,PWR_CR_VOS_Pos); 		/*VOS = 0b11*/
	REG_SET_BIT(pPWR->CR,PWR_CR_ODEN_Pos);       		/* Activate over drive mode */
	while(! REG_READ_BIT(pPWR->CSR,PWR_CSR_ODRDY_Pos)); /* wait for overdrive ready*/
	REG_SET_BIT(pPWR->CR,PWR_CR_ODSWEN_Pos);     		/* Over drive switch enable*/

   // SETTING UP MAIN PLL as 180 MHZ


   REG_SET_VAL(pRCC->PLLCFGR,0x8U,0x3FU,RCC_PLLCFGR_PLLM_Pos);   //PLL M = 8

   REG_SET_VAL(pRCC->PLLCFGR,180U,0x1FFU,RCC_PLLCFGR_PLLN_Pos);  // PLL N = 180

   REG_SET_VAL(pRCC->PLLCFGR,0x00U,0x3U,RCC_PLLCFGR_PLLP_Pos);  // PLL P = 2

   // LCD DOTCLOCK SETTINGS
   REG_SET_VAL(pRCC->PLLSAICFGR,50U,0x1FFU,RCC_PLLSAICFGR_PLLSAIN_Pos);  // PLL SAIN  = 50

   REG_SET_VAL(pRCC->PLLSAICFGR,0x2U,0x7U,RCC_PLLSAICFGR_PLLSAIR_Pos);  // PLL SAIR  = 2

   REG_SET_VAL(pRCC->DCKCFGR,0x2U,0x3U,RCC_DCKCFGR_PLLSAIDIVR_Pos);  /* DIV 8*/// LCD CLOCK 6.25 MHZ

   REG_SET_BIT(pRCC->CR,RCC_CR_PLLSAION_Pos);  // RUN PLLSAI ON
   while(!REG_READ_BIT(pRCC->CR,RCC_CR_PLLSAIRDY_Pos));

   // SETUP AHB and ABPx Clocks
   REG_SET_VAL(pRCC->CFGR,0x0U,0xFU,RCC_CFGR_HPRE_Pos);  // AHB CLOCK NOT DIVIDED
   REG_SET_VAL(pRCC->CFGR,0x5U,0x7U,RCC_CFGR_PPRE1_Pos);  // APB1 CLOCK SET TO 45 MHZ
   REG_SET_VAL(pRCC->CFGR,0x4U,0x7U,RCC_CFGR_PPRE2_Pos);  // APB2 CLOCK SET TO 90 MHZ

   REG_SET_BIT(pRCC->CR,RCC_CR_PLLON_Pos); // TURN ON  PLL ON

   while(!REG_READ_BIT(pRCC->CR,RCC_CR_PLLRDY_Pos)); // WAIT UNTIL PLLCLK GETS READY

   REG_SET_VAL(pRCC->CFGR,0x2U,0x3U,RCC_CFGR_SW_Pos); // SWITCH PLLCLCK AS SYSCLK

   while(!(REG_READ_VAL(pRCC->CFGR,0x3U,RCC_CFGR_SWS_Pos) == 0x2U));

}
void LTC_Init(void)
{

	LTDC_TypeDef *pLTDC = LTDC;

	REG_SET_BIT(RCC->APB2ENR,RCC_APB2ENR_LTDCEN_Pos);

	//Configure horizontal synchronization timings
	REG_SET_VAL(pLTDC->SSCR,(BSP_LCD_HSW-1),0xFFFU,LTDC_SSCR_HSW_Pos);
	REG_SET_VAL(pLTDC->BPCR,(BSP_LCD_HSW+BSP_LCD_HBP-1),0xFFFU,LTDC_BPCR_AHBP_Pos);
	REG_SET_VAL(pLTDC->AWCR,(BSP_LCD_HSW+BSP_LCD_HBP+BSP_LCD_ACTIVE_WIDTH-1),0xFFFU,LTDC_AWCR_AAW_Pos);
	uint32_t total_width = BSP_LCD_HSW+BSP_LCD_HBP+BSP_LCD_ACTIVE_WIDTH+BSP_LCD_HFP-1;
	REG_SET_VAL(pLTDC->TWCR,total_width,0xFFFU,LTDC_TWCR_TOTALW_Pos);

	//configure the vertical synchronization timings
	REG_SET_VAL(pLTDC->SSCR,(BSP_LCD_VSW-1),0x7FFU,LTDC_SSCR_VSH_Pos);
	REG_SET_VAL(pLTDC->BPCR,(BSP_LCD_VSW+BSP_LCD_VBP-1),0x7FFU,LTDC_BPCR_AVBP_Pos);
	REG_SET_VAL(pLTDC->AWCR,(BSP_LCD_VSW+BSP_LCD_VBP+BSP_LCD_HEIGHT-1),0x7FFU,LTDC_AWCR_AAH_Pos);
	uint32_t total_height = BSP_LCD_VSW+BSP_LCD_VBP+BSP_LCD_HEIGHT+BSP_LCD_VFP-1;
	REG_SET_VAL(pLTDC->TWCR,total_height,0x7FFU,LTDC_TWCR_TOTALH_Pos);

	//Configure the background color(RED)
	REG_SET_VAL(pLTDC->BCCR,0xFFU,0xFFU,LTDC_BCCR_BCRED_Pos);


	//enable the LTDC peripheral
	REG_SET_BIT(pLTDC->GCR,LTDC_GCR_LTDCEN_Pos);
}
void LTDC_Layer_Init(LTDC_Layer_TypeDef *pLayer)
{
	LTDC_TypeDef *pLTDC = LTDC;
	uint32_t tmp = 0;

	//1. configure the pixel format of the layer's framebuffer
	REG_SET_VAL(pLayer->PFCR,0x2U,0x7U,LTDC_LxPFCR_PF_Pos);

	//2. configure the constant alpha and blending factors
	REG_SET_VAL(pLayer->CACR,255U,0xFFU,LTDC_LxCACR_CONSTA_Pos);
	tmp = 0;
	REG_SET_VAL(tmp,0x4U,0x7U,LTDC_LxBFCR_BF1_Pos);
	REG_SET_VAL(tmp,0x5U,0x7U,LTDC_LxBFCR_BF2_Pos);
	REG_WRITE(pLayer->BFCR,tmp);


	//3. Configure layer position (Windowing)
	uint32_t AHBP =  REG_READ_VAL(pLTDC->BPCR,0xFFFU,LTDC_BPCR_AHBP_Pos);
	uint32_t WHSTART = AHBP+BSP_LTDC_LAYER_H_START +1;
	REG_SET_VAL(tmp,WHSTART,0xFFFU,LTDC_LxWHPCR_WHSTPOS_Pos);

	uint32_t WHSTOP = AHBP+BSP_LTDC_LAYER_H_START+BSP_LTDC_LAYER_WIDTH+1;
	uint32_t AAW =   REG_READ_VAL(pLTDC->AWCR,0xFFFU,LTDC_AWCR_AAW_Pos);
	WHSTOP = (WHSTOP > AAW)?AAW:WHSTOP;
	REG_SET_VAL(tmp,WHSTOP,0xFFFU,LTDC_LxWHPCR_WHSPPOS_Pos);

	REG_WRITE(pLayer->WHPCR,tmp);

	tmp = 0;
	uint32_t AVBP = REG_READ_VAL(pLTDC->BPCR,0x7FFU,LTDC_BPCR_AVBP_Pos);
	uint32_t WVSTART = AVBP+BSP_LTDC_LAYER_V_START+1;
	REG_SET_VAL(tmp,WVSTART,0x7FFU,LTDC_LxWVPCR_WVSTPOS_Pos);

	uint32_t AAH = REG_READ_VAL(pLTDC->AWCR,0x7FFU,LTDC_AWCR_AAH_Pos);
	uint32_t WVSTOP = AVBP+BSP_LTDC_LAYER_V_START+BSP_LTDC_LAYER_HEIGHT+1;
	WVSTOP = (WVSTOP > AAH)?AAH:WVSTOP;
	REG_SET_VAL(tmp,WVSTOP,0x7FFU,LTDC_LxWVPCR_WVSPPOS_Pos);

	REG_WRITE(pLayer->WVPCR,tmp);

	//4. Configure Frame buffer address
	REG_WRITE(pLayer->CFBAR,bsp_lcd_get_fb_address());

	//5. Configure the default color of the layer (optional)
	REG_WRITE(pLayer->DCCR,YELLOW);

	//6 . Configure pitch, line length and total lines of the frame buffer
	tmp = 0;
	uint32_t pitch =  BSP_LTDC_LAYER_WIDTH * 2;
	uint32_t line_len = pitch + 3;
	REG_SET_VAL(tmp,pitch,0x1FFFU,LTDC_LxCFBLR_CFBP_Pos);
	REG_SET_VAL(tmp,line_len,0x1FFFU,LTDC_LxCFBLR_CFBLL_Pos);
	REG_WRITE(pLayer->CFBLR,tmp);

	REG_SET_VAL(pLayer->CFBLNR,BSP_LTDC_LAYER_HEIGHT,0x7FFU,LTDC_LxCFBLNR_CFBLNBR_Pos);

	//7. Activate immediate reload
	REG_SET_BIT(LTDC->SRCR,LTDC_SRCR_IMR);

	//8. Enable the layer
	REG_SET_BIT(pLayer->CR,LTDC_LxCR_LEN_Pos);

}

