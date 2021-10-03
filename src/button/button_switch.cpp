///
/// @file
///
/// @details
/// This file contains the the code used to setup and perform the actions
/// required to handle the input button switches.

//
// Includes
//
#include "button_switch.h"

#include <Arduino.h>

#include <stdint.h>

//
// Local Definitions
//

/// @brief
/// Button 1 switch pin
///
/// @details
/// This defines which pin is connected to button 1's switch.
#define BUTTON_1_SWITCH_PIN                 3

/// @brief
/// Button 2 switch pin
///
/// @details
/// This defines which pin is connected to button 2's switch.
#define BUTTON_2_SWITCH_PIN                 5

/// @brief
/// Button 3 switch pin
///
/// @details
/// This defines which pin is connected to button 3's switch.
#define BUTTON_3_SWITCH_PIN                 7

/// @brief
/// Switch de-bounce time
///
/// @details
/// This defines the amount of time in milli-seconds between the sampling of
/// the switch input pin.
#define SWITCH_SAMPLE_DELAY_MSECS           100

//
// Local Structures & Enumerations
//

//
// Local Typedefs
//

/// @brief
/// Switch structure
///
/// @details
/// This structure indicates what needs to be know about a particular switch.
typedef struct
{
    /// @details
    /// Indicates which pis is associates with the switch.
    uint8_t pin;

    /// @details
    /// Indicates the last checked state of the switch.
    button_switch_state_type state;
} button_switch_t;

/// @brief
///
///
/// @details
///
typedef struct
{
    /// @details
    /// Details the switch in question.
    button_switch_t the_switch;

    /// @details
    /// Used to handle debouncing of the switch.
    button_switch_state_type switch_debounce[2];

    /// @details
    /// Indicates the debounced state of the switch.
    button_switch_state_type switch_state;

    /// @details
    /// Used to indicate when to next check the switch's state.
    uint64_t nxt_sample_time_msecs;
} button_switch_debounce_t;

//
// Local Function Prototypes
//

//
// Local Global Variables
//

/// @brief
/// Buttons
///
/// @details
/// This variable holds the information about the button switches.
static button_switch_debounce_t button_switches[] =
{
    {
        .the_switch =
        {
            .pin = (uint8_t)BUTTON_1_SWITCH_PIN,
            .state = BUTTON_SWITCH_STATE_OPEN
        },
        .switch_debounce = { BUTTON_SWITCH_STATE_OPEN, BUTTON_SWITCH_STATE_OPEN },
        .switch_state = BUTTON_SWITCH_STATE_OPEN,
        .nxt_sample_time_msecs = (uint64_t)0u
    },
    {
        .the_switch =
        {
            .pin = (uint8_t)BUTTON_2_SWITCH_PIN,
            .state = BUTTON_SWITCH_STATE_OPEN
        },
        .switch_debounce = { BUTTON_SWITCH_STATE_OPEN, BUTTON_SWITCH_STATE_OPEN },
        .switch_state = BUTTON_SWITCH_STATE_OPEN,
        .nxt_sample_time_msecs = (uint64_t)0u
    },
    {
        .the_switch =
        {
            .pin = (uint8_t)BUTTON_3_SWITCH_PIN,
            .state = BUTTON_SWITCH_STATE_OPEN
        },
        .switch_debounce = { BUTTON_SWITCH_STATE_OPEN, BUTTON_SWITCH_STATE_OPEN },
        .switch_state = BUTTON_SWITCH_STATE_OPEN,
        .nxt_sample_time_msecs = (uint64_t)0u
    }
};

//
// Code
//
void button_switch_cfg(void)
{
    for (
        int idx = 0;
        (int)BUTTON_SWITCH_MAX > idx;
        ++idx)
    {
        pinMode(button_switches[idx].the_switch.pin, INPUT);
    }
}

button_switch_state_type button_switch_state_now(
    button_switches_t which_switch)
{
    button_switch_state_type rv = BUTTON_SWITCH_STATE_MAX;

    if (BUTTON_SWITCH_MAX > which_switch)
    {
        const int pin_val = digitalRead(
                                button_switches[which_switch].the_switch.pin);

        if (pin_val)
        {
            button_switches[which_switch].the_switch.state =
                                                    BUTTON_SWITCH_STATE_CLOSED;
        }
        else
        {
            button_switches[which_switch].the_switch.state =
                                                    BUTTON_SWITCH_STATE_OPEN;
        }

        rv = button_switches[which_switch].the_switch.state;
    }

    return rv;
}

void button_switch_action(void)
{
    for (
        int idx = 0;
        (int)BUTTON_SWITCH_MAX > idx;
        ++idx)
    {
        const uint64_t msecs = (uint64_t)millis();

        if (button_switches[idx].nxt_sample_time_msecs <= msecs)
        {
            // Perform the debounce
            button_switches[idx].switch_debounce[1] =
                                    button_switches[idx].switch_debounce[0];

            // Update the switch state
            button_switches[idx].switch_debounce[0] =
                            button_switch_state_now((button_switches_t )idx);

            // Update the switch state if needed
            if (button_switches[idx].switch_debounce[1] ==
                                    button_switches[idx].switch_debounce[0])
            {
                button_switches[idx].switch_state =
                                    button_switches[idx].switch_debounce[1];
            }

            // Set next sample time
            button_switches[idx].nxt_sample_time_msecs =
                                msecs + (uint64_t)SWITCH_SAMPLE_DELAY_MSECS;
        }
    }
}

button_switch_state_type button_switch_state_get(
    button_switches_t which_switch)
{
    button_switch_state_type rv = BUTTON_SWITCH_STATE_MAX;

    if (BUTTON_SWITCH_MAX > which_switch)
    {
        rv = button_switches[which_switch].switch_state;
    }

    return rv;
}