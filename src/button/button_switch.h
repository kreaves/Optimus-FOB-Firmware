///
/// @file
///
/// @copydoc button_switch.cpp

#if !defined(BUTTON_SWITCH_H)
#define BUTTON_SWITCH_H

//
// Shared Includes
//

//
// Shared Definitions
//

//
// Shared Structures & Enumerations
//

//
// Shared Typedefs
//

/// @brief
/// Button switches
///
/// @details
/// This type is to be used to indicate which button switch is being
/// referenced.
typedef enum {
    /// @brief
    /// Button 1 SWITCH
    BUTTON_1_SWITCH,

    /// @brief
    /// Button 2 SWITCH
    BUTTON_2_SWITCH,

    ///@brief
    /// Button 3 SWITCH
    BUTTON_3_SWITCH,

    /// @brief
    /// Placeholder to indicate the maximum possible values of the enumeration
    BUTTON_SWITCH_MAX
} button_switches_t;

/// @brief
/// Switch state
///
/// @details
/// This enumeration defines the possible states that the switch may be in.
typedef enum
{
    /// @brief
    /// The switch is open
    BUTTON_SWITCH_STATE_OPEN,
    /// @brief
    /// The switch is closed
    BUTTON_SWITCH_STATE_CLOSED,
    /// @brief
    /// Max possible switch states
    BUTTON_SWITCH_STATE_MAX,
} button_switch_state_type;

//
// Shared Method Definitions
//

/// @brief
/// Configure the HW for switch module
///
/// @details
/// This function sets up the hardware that will be use to monitor the switch.
void button_switch_cfg(void);

/// @brief
/// Immediate switch state
///
/// @details
/// This function returns the current switch state with no regard for
/// de-bounce.
///
/// @param [in] which_switch
/// This indicates which switch is being referred to. It needs to be a member
/// of the @ref button_switches_t type.
///
/// @return
/// This is the current state of the switch. It is of the
/// @ref button_switch_state_type type.
button_switch_state_type button_switch_state_now(
    button_switches_t which_switch);

/// @brief
/// Switch loop function
///
/// @details
/// This function handles the de-bouncing of the switch.
void button_switch_action(void);

/// @brief
/// Switch state
///
/// @details
/// This function returns the current switch state taking into account, the
/// de-bounce.
///
/// @param [in] which_switch
/// This indicates which switch is being referred to. It needs to be a member
/// of the @ref button_switches_t type.
///
/// @return
///
button_switch_state_type button_switch_state_get(
    button_switches_t which_switch);

//
// Shared Global Variables
//

#endif // BUTTON_SWITCH_H