///
/// @file
///
/// @details
/// This file contains the the code used to setup and perform the actions
/// required to perform a heartbeat function.

//
// Includes
//
#include "status_led.h"
#include "Arduino.h"

#include <stdint.h>

//
// Defines
//

/// @brief
/// Status LED green pin
///
/// @details
/// This defines the pin that will be used for the status led's green diode.
/// This line will be set based on the state that the led is put into.
#define STATUS_LED_GRN_PIN                  11

/// @brief
/// Status LED yellow pin
///
/// @details
/// This defines the pin that will be used for the status led's yellow diode.
/// This line will be set based on the state that the led is put into.
#define STATUS_LED_YEL_PIN                  10

/// @brief
/// Heartbeat toggle delay
///
/// @details
/// This defines the duration that must be exceeded prior to the heartbeat LED
/// changing state.
#define STATUS_LED_BLINK_DELAY_MSECS        750

/// @brief
/// LED on pin state
///
/// @details
/// This defines what the pin level needs to be to ensure that a particular
/// status LED is on.
#define STATUS_LED_ON_PIN_SETTING           HIGH

/// @brief
/// LED off pin state
///
/// @details
/// This defines what the pin level needs to be to ensure that a particular
/// status LED is off.
#define STATUS_LED_OFF_PIN_SETTING          LOW

//
// Typedefs
//

/// @brief
/// Status LED states
///
/// @details
/// This type is to be used to indicate what state a status led is in.
typedef enum {
    /// @brief
    /// Indicates that the particular status LED is on
    STATUS_LED_STATE_ON,

    /// @brief
    /// Indicates that the particular status LED is off
    STATUS_LED_STATE_OFF,

    /// @brief
    /// Placeholder to indicate the maximum possible values of the enumeration
    STATUS_LED_STATE_MAX
} status_led_states_t;

/// @brief
/// LED structure
///
/// @details
/// This structure outlines what is needed to be known about a particular LED.
typedef struct
{
    /// @details
    /// This indicates which pin is associated with the LED.
    pin_size_t led_pin;

    /// @details
    /// This variable indicates if the LED is on or off.
    status_led_states_t led_state;
} led_t;

/// @brief
/// Status LED structure
///
/// @details
/// This structure defines what is needed to be know abut an LED to implement
/// the desired functionality.
typedef struct
{
    /// @details
    /// The information about a particular status LED.
    led_t led;

    /// @details
    /// This variable indicates what mode a particular status LED is in.
    status_led_modes_t led_mode;

    /// @details
    /// This variable indicates what mode a particular status LED is going to
    /// be in.
    status_led_modes_t led_nxt_mode;

    /// @details
    /// This keeps track of the next time that something needs to be done to
    /// a particular led while blinking.
    uint64_t next_toggle_time_msecs;
} status_led_t;


//
// Local Global Variables
//

status_led_t status_leds[STATUS_LED_MAX] =
{
    {
        .led =
        {
            .led_pin = (pin_size_t)STATUS_LED_GRN_PIN,
            .led_state = STATUS_LED_STATE_OFF
        },
        .led_mode = STATUS_LED_MODE_OFF,
        .led_nxt_mode = STATUS_LED_MODE_OFF,
        .next_toggle_time_msecs = (uint64_t)0u
    },
    {
        .led =
        {
            .led_pin = (pin_size_t)STATUS_LED_YEL_PIN,
            .led_state = STATUS_LED_STATE_OFF
        },
        .led_mode = STATUS_LED_MODE_OFF,
        .led_nxt_mode = STATUS_LED_MODE_OFF,
        .next_toggle_time_msecs = (uint64_t)0u
    }
};

static void status_led_on(led_t * const p_led);
static void status_led_off(led_t * const p_led);
static void status_led_blink(status_led_t * const p_led_status);

//
// Code
//
void status_led_cfg(void)
{
    for (
        int idx = 0;
        (int)STATUS_LED_MAX > idx;
        ++idx)
    {
        pinMode(status_leds[idx].led.led_pin, OUTPUT);
        status_led_off(&status_leds[idx].led);
    }
}

void status_led_action(void)
{
    const unsigned long msecs = millis();

    for (
        int idx = 0;
        (int)STATUS_LED_MAX > idx;
        ++idx)
    {
        if (status_leds[idx].led_mode ==
                                        status_leds[idx].led_nxt_mode)
        {
            if (STATUS_LED_MODE_BLINKING == status_leds[idx].led_mode)
            {
                status_led_blink(&status_leds[idx]);
            }
        }
        else
        {
            if (STATUS_LED_MODE_BLINKING == status_leds[idx].led_nxt_mode)
            {
                status_leds[idx].led_nxt_mode = STATUS_LED_MODE_BLINKING;
                status_leds[idx].led_mode = STATUS_LED_MODE_BLINKING;
                status_led_blink(&status_leds[idx]);
            }
            else if (STATUS_LED_MODE_ON == status_leds[idx].led_nxt_mode)
            {
                // LED on
                status_leds[idx].led_nxt_mode = STATUS_LED_MODE_ON;
                status_leds[idx].led_mode = STATUS_LED_MODE_ON;
                status_led_on(&status_leds[idx].led);
            }
            else
            {
                // LED off
                status_leds[idx].led_nxt_mode = STATUS_LED_MODE_OFF;
                status_leds[idx].led_mode = STATUS_LED_MODE_OFF;
                status_led_off(&status_leds[idx].led);
            }
        }
    }
}

void status_led_set_mode(
    const status_leds_t led,
    const status_led_modes_t led_mode)
{
    if ((STATUS_LED_MAX > led) &&
        (STATUS_LED_MODE_MAX > led_mode))
    {
        status_leds[led].led_nxt_mode = led_mode;
    }
}

static void status_led_blink(status_led_t * const p_led_status)
{
    const uint64_t msecs = (uint64_t)millis();

    if (p_led_status->next_toggle_time_msecs <= msecs)
    {
        if (STATUS_LED_STATE_OFF == p_led_status->led.led_state)
        {
            // Turn on LED
            status_led_on(&p_led_status->led);
        }
        else
        {
            // Turn off LED
            status_led_off(&p_led_status->led);
        }

        p_led_status->next_toggle_time_msecs =
                                msecs + (uint64_t)STATUS_LED_BLINK_DELAY_MSECS;
    }
}

static void status_led_on(led_t * const p_led)
{
    digitalWrite(p_led->led_pin, (PinStatus)STATUS_LED_ON_PIN_SETTING);
    p_led->led_state = STATUS_LED_STATE_ON;
}

static void status_led_off(led_t * const p_led)
{
    digitalWrite(p_led->led_pin, (PinStatus)STATUS_LED_OFF_PIN_SETTING);
    p_led->led_state = STATUS_LED_STATE_OFF;
}
