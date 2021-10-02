///
/// @file
///
/// @details
/// This file contains the the code used to setup and perform the actions
/// required to perform a heartbeat function.

//
// Includes
//
#include "button_led.h"
#include "Arduino.h"

#include <stdint.h>

//
// Defines
//

/// @brief
/// Button 1 led pin
///
/// @details
/// This defines the pin that will be used for the button 1's led. This line
/// will be set based on the state that the led is put into.
#define BUTTON_1_LED_PIN                    2

/// @brief
/// Button 2 led pin
///
/// @details
/// This defines the pin that will be used for the button 2's led. This line
/// will be set based on the state that the led is put into.
#define BUTTON_2_LED_PIN                    4

/// @brief
/// Button 3 led pin
///
/// @details
/// This defines the pin that will be used for the button 3's led. This line
/// will be set based on the state that the led is put into.
#define BUTTON_3_LED_PIN                    6

/// @brief
/// Heartbeat toggle delay
///
/// @details
/// This defines the duration that must be exceeded prior to the heartbeat LED
/// changing state.
#define BUTTON_BLINK_DELAY_MSECS            750

/// @brief
/// LED on pin state
///
/// @details
/// This defines what the pin level needs to be to ensure that a particular
/// status LED is on.
#define BUTTON_LED_ON_PIN_SETTING           LOW

/// @brief
/// LED off pin state
///
/// @details
/// This defines what the pin level needs to be to ensure that a particular
/// status LED is off.
#define BUTTON_LED_OFF_PIN_SETTING          HIGH

//
// Typedefs
//

/// @brief
/// LED states
///
/// @details
/// This type is to be used to indicate what state a led is in.
typedef enum {
    /// @brief
    /// Indicates that the LED is on
    LED_STATE_ON,

    /// @brief
    /// Indicates that the LED is off
    LED_STATE_OFF,

    /// @brief
    /// Placeholder to indicate the maximum possible values of the enumeration
    LED_STATE_MAX
} led_states_t;

/// @brief
/// LED status
///
/// @details
/// This type is to be used to indicate the mode and current state of a
/// particular LED.
typedef struct
{
    /// @details
    /// This indicates which pin is associated with the LED.
    const uint8_t led_pin;

    /// @details
    /// This variable indicates if the LED is on or off.
    led_states_t led_state;

    /// @details
    /// This variable indicates what mode the LED is in.
    led_modes_t led_mode;

    /// @details
    /// This variable indicates what mode the LED is going to be.
    led_modes_t led_nxt_mode;

    /// @details
    /// This keeps track of the next time that something needs to be done to
    /// the led while blinking.
    uint64_t next_toggle_time_msecs;
} led_status_t;

//
// Local Global Variables
//

led_status_t button_led_status[BUTTON_LED_MAX] =
{
    {
        .led_pin = (uint8_t)BUTTON_1_LED_PIN,
        .led_state = LED_STATE_OFF,
        .led_mode = LED_MODE_OFF,
        .led_nxt_mode = LED_MODE_OFF,
        .next_toggle_time_msecs = (uint64_t)0u
    },
        {
        .led_pin = (uint8_t)BUTTON_2_LED_PIN,
        .led_state = LED_STATE_OFF,
        .led_mode = LED_MODE_OFF,
        .led_nxt_mode = LED_MODE_OFF,
        .next_toggle_time_msecs = (uint64_t)0u
    },
    {
        .led_pin = (uint8_t)BUTTON_3_LED_PIN,
        .led_state = LED_STATE_OFF,
        .led_mode = LED_MODE_OFF,
        .led_nxt_mode = LED_MODE_OFF,
        .next_toggle_time_msecs = (uint64_t)0u
    }
};

static void button_led_blink(led_status_t * const p_led_status);
static void button_led_on(led_status_t * const p_led_status);
static void button_led_off(led_status_t * const p_led_status);

//
// Code
//
void button_led_cfg(void)
{
    for (
        int idx = 0;
        (int)BUTTON_LED_MAX > idx;
        ++idx)
    {
        pinMode(button_led_status[idx].led_pin, OUTPUT);
        button_led_off(&button_led_status[idx]);
    }
}

void button_led_action(void)
{
    const unsigned long msecs = millis();

    for (
        int idx = 0;
        (int)BUTTON_LED_MAX > idx;
        ++idx)
    {
        if (button_led_status[idx].led_mode ==
                                        button_led_status[idx].led_nxt_mode)
        {
            if (LED_MODE_BLINKING == button_led_status[idx].led_mode)
            {
                button_led_blink(&button_led_status[idx]);
            }
        }
        else
        {
            if (LED_MODE_BLINKING == button_led_status[idx].led_nxt_mode)
            {
                button_led_status[idx].led_nxt_mode = LED_MODE_BLINKING;
                button_led_status[idx].led_mode = LED_MODE_BLINKING;
                button_led_blink(&button_led_status[idx]);
            }
            else if (LED_MODE_ON == button_led_status[idx].led_nxt_mode)
            {
                // LED on
                button_led_status[idx].led_nxt_mode = LED_MODE_ON;
                button_led_status[idx].led_mode = LED_MODE_ON;
                button_led_on(&button_led_status[idx]);
            }
            else
            {
                // LED off
                button_led_status[idx].led_nxt_mode = LED_MODE_OFF;
                button_led_status[idx].led_mode = LED_MODE_OFF;
                button_led_off(&button_led_status[idx]);
            }
        }
    }
}

void button_led_set_mode(const button_leds_t led, const led_modes_t led_mode)
{
    if ((BUTTON_LED_MAX > led) &&
        (LED_MODE_MAX > led_mode))
    {
        button_led_status[led].led_nxt_mode = led_mode;
    }
}

static void button_led_blink(led_status_t * const p_led_status)
{
    const uint64_t msecs = (uint64_t)millis();

    if (p_led_status->next_toggle_time_msecs <= msecs)
    {
        if (LED_STATE_OFF == p_led_status->led_state)
        {
            // Turn on LED
            button_led_on(p_led_status);
        }
        else
        {
            // Turn off LED
            button_led_off(p_led_status);
        }

        p_led_status->next_toggle_time_msecs =
                                    msecs + (uint64_t)BUTTON_BLINK_DELAY_MSECS;
    }
}

static void button_led_on(led_status_t * const p_led_status)
{
    digitalWrite(p_led_status->led_pin, (PinStatus)BUTTON_LED_ON_PIN_SETTING);
    p_led_status->led_state = LED_STATE_ON;
}

static void button_led_off(led_status_t * const p_led_status)
{
    digitalWrite(p_led_status->led_pin, (PinStatus)BUTTON_LED_OFF_PIN_SETTING);
    p_led_status->led_state = LED_STATE_OFF;
}