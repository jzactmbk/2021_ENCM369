/*!*********************************************************************************************************************
@file user_app.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1Run(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags;                             /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserAppInitialize(void)

@brief
Initializes the application's variables.

Should only be called once in main init section.

Requires:
- RA0-7 setup as digital output

Promises:
- For RA7 to be high
- For timer0 to be enabled in async. 16-bit mode with a prescaler of 1:16 for the (Fosc/4) source.

*/
void UserAppInitialize(void)
{
    //LED initialization
    LATA = 0x80; //Setting RA7 latch to digital high, and RA0-6 latches low
    
    //Enabling timer0 in 16-bit mode with a post-scaler value of 1:1
    T0CON0 = 0x90;
    
    //Setting timer0 to asynchronous. mode with (Fosc/4) as the source with a pre-scaler of 1:16
    T0CON1 = 0x54;
   
} /* end UserAppInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:+
- Delay/system tick of 4us between calls

Promises:
- To create a triangle wave @ 976.56Hz on DAC1
 
*/
void UserAppRun(void)
{
    static u8 u8SlopeState = 0;
    
    if (u8SlopeState == 0)
    {
        if(DAC1DATL == 0xFE)
        {
            u8SlopeState = 1;
        }
        DAC1DATL +=1;
    }
    else 
    {
        if(DAC1DATL == 0x01)
        {
            u8SlopeState = 0;
        }
        DAC1DATL -=1;
    }
    
} /* end UserAppRun */

/*!--------------------------------------------------------------------------------------------------------------------
@fn void TimeXus(u16 u16TimeXus)

@brief Sets Timer0 to count u16Microseconds_

Requires:
- Timer0 configured such that each timer tick is 1 microsecond
- u16 u16TimeXus is the value in microseconds to time from 1 to 65535

Promises:
- Pre-loads TMR0H:L to clock out desired period
- TMR0IF cleared
- Timer0 enabled

*/

void TimeXus(u16 u16TimerTime)
{
    T0CON0 &= 0x7F; //Disabling timer0
    
    TMR0H = (0xFFFF - u16TimerTime) >> 8; /*Right shifting upper 8 bits of u16TimerTime 
                               to fit in 8-bit TMR0H*/ 
    
    //Pre-loading TMR0L/H based on passed u16TimerTime
    TMR0L = (0xFFFF - u16TimerTime) & 0x00FF; /*Taking only the lower 8 bits of u16TimerTime
                                               for TMR0L*/  
    
    PIR3 &= 0x7F; //Clearing the TMR0IF flag
    T0CON0 |= 0x80; //Enabling timer0
    
    
}  /* end TimeXus () */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
