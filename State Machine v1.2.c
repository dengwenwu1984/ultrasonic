/*
 * State Machine v1.2c
 *
 *  Created on: 01 Aug 2013
 *      Author: Ant Scranney
 */

#include <stdio.h>
#include <stdlib.h>

/*****
* State definitions, states are the routine the program jumps to, these are prefixed with an 'S' for easy identification
*****/
enum states { S_OFF, S_ON, S_PROCESS, S_MAX };

/*****
* Event definitions, events trigger a transition and are prefixed with an 'E' for easy identification
*****/
enum events { E_OFF, E_ON, E_START, E_MAX};

/*****
* Function prototypes
*****/
enum states StateMachine(enum events event, enum states Current_State);
int ReadKeyInput(void);

void State_Enter_OFF(void);   void State_Enter_ON(void);   void State_Enter_PROCESS(void);
void State_InState_OFF(void); void State_InState_ON(void); void State_InState_PROCESS(void);
void State_Exit_OFF(void);    void State_Exit_ON(void);    void State_Exit_PROCESS(void);

typedef void (* const voidFunc)(void);

voidFunc UponEnter[S_MAX] =          {State_Enter_OFF,   State_Enter_ON,   State_Enter_PROCESS};
voidFunc ActionWhileInState[S_MAX] = {State_InState_OFF, State_InState_ON, State_InState_PROCESS};
voidFunc UponExit[S_MAX] =           {State_Exit_OFF,    State_Exit_ON,    State_Exit_PROCESS};

/*****
* Main function starts here
*****/
int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);	// Lines required as Eclipse has buffer issues when used with Windows and the scanf function
	setvbuf(stderr, NULL, _IONBF, 0);	// Lines required as Eclipse has buffer issues when used with Windows and the scanf function

	// This line and the way the StateMachine function is called inside the while loop
	//avoids using Current_State as a global variable which improves the code
	enum states Current_State = S_OFF;

    while ( Current_State != S_MAX )
    {
		// Function call for the state machine, which then calls the read keys function and waits for the value returned
        Current_State = StateMachine( ReadKeyInput(), Current_State );
    }

    return 0;
}

/*****
* Read Keys function definition containing the code to execute
*****/
int ReadKeyInput()
{
    char input;
    scanf("%c", &input);
    switch (input)
    {
        case '1':
            return E_OFF;
            break;
        case '2':
            return E_ON;
            break;
        case '3':
            return E_START;
            break;
    }
    return E_MAX; //
}

/*****
* State Machine function definition containing the code to execute
*****/
enum states StateMachine(enum events event, enum states Current_State)
{
    int Next_State = Current_State;

    switch ( Current_State )
    {
    case S_OFF:
           switch (event )
           {
               // A transition to the next state will occur here
               case E_ON:
               	Next_State = S_ON;
                   break;
               default:		// Default case placed here to avoid Eclipse warnings as Eclipse expects
            	   break;	//to handle all enumerated values
           }
           break;
       case S_ON:
           switch (event )
           {
               // A transition to the next state will occur here
               case E_OFF:
               	Next_State = S_OFF;
                   break;
               case E_START:
               	Next_State = S_PROCESS;
                   break;
               default:		// Default case placed here to avoid Eclipse warnings as Eclipse expects
            	   break;	//to handle all enumerated values
           }
           break;
       case S_PROCESS:
           switch (event )
           {
               // A transition to the next state will occur here
           case E_OFF:
           	Next_State = S_OFF;
               break;
           default:		// Default case placed here to avoid Eclipse warnings as Eclipse expects
        	   break;	//to handle all enumerated values
           }
           break;
           // The program should never arrive here
   		default:
           break;

    }

    if (Next_State != Current_State)
    {
        // Function call for Upon Exit function, it can be omitted but allows extra functionality
        UponExit[Current_State]();
        // Function call for Upon Enter function, it can be omitted but allows extra functionality
        if (Next_State != S_MAX) UponEnter[Next_State]();
    }
    else // ActionWhileInState is only be called when NOT doing a transition
     {
        if ( event != E_MAX) ActionWhileInState[Current_State]();
    }
    return Next_State;
}







/*****
* Enter function definitions containing the code to execute
*****/
void State_Enter_OFF(void)
{
	puts("This is the UponEnter function running for the S_OFF state");
}

void State_Enter_ON(void)
{
	puts("This is the UponEnter function running for the S_ON state");
}

void State_Enter_PROCESS(void)
{
	puts("This is the UponEnter function running for the S_PROCESS state");
}



/*****
* Action While function definitions containing the code to execute
*****/
void State_InState_OFF(void)
{
	puts("Current state S_OFF - WhileInState function");
}

void State_InState_ON(void)
{
	puts("Current state S_ON - WhileInState function");
}

void State_InState_PROCESS(void)
{
	puts("Current state S_PROCESS - WhileInState function");
}



/*****
* Exit function definitions containing the code to execute
*****/
void State_Exit_OFF(void)
{
	puts("This is the UponExit function running for the S_OFF state");
}

void State_Exit_ON(void)
{
	puts("This is the UponExit function running for the S_ON state");
}

void State_Exit_PROCESS(void)
{
	puts("This is the UponExit function running for the S_PROCESS state");
}




/* void hEmpty(void)
{
  // This function is empty and will be called for each event that I don't want to write a handler for.
} */

