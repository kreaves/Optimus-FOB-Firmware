///
/// @file
///
/// @copydoc button_led.cpp
///

#if !defined(BUTTON_LED_H)
#define BUTTON_LED_H

//
// Includes
//

//
// Defines
//

/// @brief
/// Button LEDs
///
/// @details
/// This type is to be used to indicate which button LED is being referenced.
typedef enum {
    /// @brief
    /// Button 1 LED
    BUTTON_1_LED,

    /// @brief
    /// Button 2 LED
    BUTTON_2_LED,

    ///@brief
    /// Button 3 LED
    BUTTON_3_LED,

    /// @brief
    /// Placeholder to indicate the maximum possible values of the enumeration
    BUTTON_LED_MAX
} button_leds_t;

/// @brief
/// LED modes
///
/// @details
/// This type is to be used to indicate what state a led is in.
typedef enum {
    /// @brief
    /// Indicates that the LED is on all the time
    LED_MODE_ON,

    /// @brief
    /// Indicates that the LED is blinking
    LED_MODE_BLINKING,

    ///@brief
    /// Indicates that the LED is off all the time
    LED_MODE_OFF,

    /// @brief
    /// Placeholder to indicate the maximum possible values of the enumeration
    LED_MODE_MAX
} led_modes_t;

//
// Function Prototypes
//

/// @brief
/// Button LED config
///
/// @details
/// This function sets up the hardware required to control the button LEDs.
void button_led_cfg(void);

/// @brief
/// Perform button LED tasks
///
/// @details
/// This function will perform various actions based on the state of the
/// particular LED.
void button_led_action(void);

/// @brief
/// Set button led mode
///
/// @details
/// This function sets what mode the LED will be put into. If either of the two
/// input parameters are invalid, then nothing happens.
///
/// @param [in] led
/// This indicates which LED is to be affected by this function call. It needs
/// to be a member of the @ref button_leds_t type.
///
/// @param [in] led_mode
/// This indicates what mode the LED will be put into. It needs to be of the
/// @ref led_modes_t type.
void button_led_set_mode(const button_leds_t led, const led_modes_t led_mode);

#endif // BUTTON_LED_H