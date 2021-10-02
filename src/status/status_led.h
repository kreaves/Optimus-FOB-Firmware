///
/// @file
///
/// @copydoc status_led.cpp
///

#if !defined(STATUS_LED_H)
#define STATUS_LED_H

//
// Includes
//
#include <stdint.h>

//
// Defines
//

/// @brief
/// Button LEDs
///
/// @details
/// This type is to be used to indicate which status LED is being referenced.
typedef enum {
    /// @brief
    /// Green status LED
    STATUS_LED_GRN,

    /// @brief
    /// Yellow status LED
    STATUS_LED_YEL,

    /// @brief
    /// Placeholder to indicate the maximum possible values of the enumeration
    STATUS_LED_MAX
} status_leds_t;

/// @brief
/// LED modes
///
/// @details
/// This type is to be used to indicate what state a led is in.
typedef enum {
    /// @brief
    /// Indicates that the LED is on all the time
    STATUS_LED_MODE_ON,

    /// @brief
    /// Indicates that the LED is blinking
    STATUS_LED_MODE_BLINKING,

    ///@brief
    /// Indicates that the LED is off all the time
    STATUS_LED_MODE_OFF,

    /// @brief
    /// Placeholder to indicate the maximum possible values of the enumeration
    STATUS_LED_MODE_MAX
} status_led_modes_t;

//
// Function Prototypes
//

/// @brief
/// Status LED config
///
/// @details
/// This function sets up the hardware required to control the status LED.
void status_led_cfg(void);

/// @brief
/// Perform status LED tasks
///
/// @details
/// This function will perform various actions based on the state of the
/// particular LED.
void status_led_action(void);

/// @brief
/// Set button led mode
///
/// @details
/// This function sets what mode the LED will be put into. If either of the two
/// input parameters are invalid, then nothing happens.
///
/// @param [in] led
/// This indicates which LED is to be affected by this function call. It needs
/// to be a member of the @ref status_leds_t type.
///
/// @param [in] led_mode
/// This indicates what mode the LED will be put into. It needs to be of the
/// @ref status_led_modes_t type.
void status_led_set_mode(
    const status_leds_t led,
    const status_led_modes_t led_mode);

#endif // STATUS_LED_H
