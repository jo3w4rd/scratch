#pragma once
#include <stdint.h>
//#include "../types.h"

#pragma pack(1)

/*
 Command                          XU#    Data Size      Description
------------------------------------------------------------------------------------------------------------------------
 LEAP_XU_STROBE_WIDTH          |   1  |  <u32_t> n  |  SET: Sets the strobe width generated by the sensors
                               |      |             |         n = 0 - max(uint32_t) : Strobe width in uS
                               |      |             |  GET: Returns the programmed strobe width in uS
                               |      |             |
 LEAP_XU_LED_POSITIONS         |   2  |  <u8_t> * sizeof(LEAP_LED_POSITION)
                               |      |             |  SET: Sets the LED Positional Data in Flash Memory
                               |      |             |  GET: Returns the LED Positional Data from Flash Memory
                               |      |             |
 LEAP_XU_STROBE_INTERVAL       |   3  |  <u8_t> n   |  SET: Sets Frame Interval
                               |      |             |         n = 0 : All bright frames
                               |      |             |         n = 1 : All dark frames
                               |      |             |         n = 2 : Robust mode(alternating bright and dark frames)
                               |      |             |         n = 3->127 : Normal mode(1 dark frame + n bright frames)
                               |      |             |  GET: Returns current Frame Interval setting
                               |      |             |
 LEAP_XU_LED_SET               |   4  |  <u32_t> n  |  SET: Sets the color on the RGB LED on the Dragonfly mother board
                               |      |             |         where n is a 24-bit RGB value, each component from 0-0x1F
                               |      |             |         n = 0 : Turns the RGB LED Off
                               |      |             |         n = 0x001F0000 : Sets RGB color to Red
                               |      |             |         n = 0x00001F00 : Sets RGB color to Green
                               |      |             |         n = 0x0000001F : Sets RGB color to Blue
                               |      |             |
 LEAP_XU_ESC_RESET_STATUS      |   5  |  <u32_t> n  |  SET: Resets the ESC board to default
                               |      |             |         n is not used and can be set to any positive value
                               |      |             |
 LEAP_XU_ESC_LED_CHARGE        |   6  |  <u32_t> n  |  SET: Enables and Disables the LED Charge circuit
                               |      |             |         n = 0 : Disables the LED Charge circuit
                               |      |             |         n = 1 : Enables the LED Charge circuit
                               |      |             |  GET: Returns the state of LED Charge
                               |      |             |
 LEAP_XU_ESC_GPIO_OVERRIDE     |   7  |  <u32_t> n  |  SET: Sets GPIO override
                               |      |             |         n = 0 : No override
                               |      |             |         n = 1 : Override by setting output Low
                               |      |             |         n = 2 : Override by setting output High
                               |      |             |         n = 3 : Toggle LEDs at ~1Hz
                               |      |             |         n = 4 : Run Script
                               |      |             |  GET: Returns the value of GPIO override
                               |      |             |
 LEAP_XU_ESC_PULSE_DELAY       |   8  |  <u32_t> n  |  SET: Sets the value for the Pulse Width Delay
                               |      |             |  GET: Returns the programmed value for the Pulse Width Delay
                               |      |             |         n = 1->0xFFFF : Pulse Delay (0xFF on pre-Rigel HW)
                               |      |             |  Units given in femtoseconds by LEAP_SC_DEVCAPS.sc_str_res_fS
                               |      |             |
 LEAP_XU_ESC_PULSE_WIDTH       |   9  |  <u32_t> n  |  SET: Sets the value for the Pulse Width
                               |      |             |  GET: Returns the programmed value for the Pulse Width
                               |      |             |         n = 1->0xFFFF : Pulse width (0xFF on older SC firmware)
                               |      |             |  Units given in femtoseconds by LEAP_SC_DEVCAPS.sc_str_res_fS
                               |      |             |
 LEAP_XU_RESOLUTION_CONFIGS    |  10  |  <u8_t> * sizeof(LEAP_DEFAULT_RESOLUTION_ARRAY)
                               |      |             |  SET: Sets the Default Resolution in Flash Memory
                               |      |             |  GET: Returns the Default Resolution from Flash Memory
                               |      |             |
 LEAP_XU_FRAME_COUNT           |  11  |  <u32_t> n  |  SET: Resets the frame count to zero
                               |      |             |         n is not used and can be set to any positive value
                               |      |             |  GET: Returns the most recent frame count
                               |      |             |
 LEAP_XU_ESC_SHIFT_RESET_START |  12  |  <u32_t> n  |  SET: Sets the Shift Register Address
                               |      |             |       Be aware that the Shift Register always starts at address 0
                               |      |             |         n = 0->15
                               |      |             |  GET: Returns the value of the Shift Register Address
                               |      |             |
 LEAP_XU_ESC_SHIFT_BIT_DATA    |  13  |  <u32_t> n  |  SET: Write Shift Register Data to the Address set above(first write)
                               |      |             |       Address increments after each write
                               |      |             |       Bit execution is from LSB to MSB starting at address 0
                               |      |             |         n = 0->255
                               |      |             |  GET: Reads the register data at the Address set above(first write)
                               |      |             |       Address increments after each read
                               |      |             |
 LEAP_XU_ESC_SHIFT_BIT_LENGTH  |  14  |  <u32_t> n  |  SET: Sets the number of bits to be shifted out
                               |      |             |       Resets the Shift Register Starting Address to 0
                               |      |             |         n = 0 : Shift Register Disabled
                               |      |             |         n = 1->128 : Number of bits to be shifted out by the shift register
                               |      |             |  GET: Resets the shfit bit count
                               |      |             |
 LEAP_XU_HARDWARE_CLOCK        |  15  |  <u32_t>    |  GET: Returns the presend hardware counter value
                               |      |             |       Returned value is in uS
                               |      |             |
 LEAP_XU_DEVCAPS               |  16  |  <u8_t> * sizeof(LEAP_DEVCAPS)
                               |      |             |  GET: Returns the Device Capabilities
                               |      |             |       The values are :
                               |      |             |         *  RESERVED (32 bit value)
                               |      |             |         *  Device Capability Flags (32 bit value)
                               |      |             |         *  Firmware Revision (32 bit value)
                               |      |             |         *  Leap Controller Type (8 bit enumberated value)
                               |      |             |         *  Leap Sensor Type (8 bit enumberated value)
                               |      |             |
 LEAP_XU_EMBLINE_BEHAVIOR      |  17  |  <u8_t> * sizeof(LEAP_EMBLINE_FORMAT_LASTLINE)
                               |      |             |  GET: Embedded line support (LEAP_EMBLINE_FORMAT_LASTLINE)
                               |      |             |
 LEAP_XU_HARDWARE_FRAME_INTERVAL  18  |  <u32_t> n  |  SET: Sets the Frame Interval in 0.1uS units
                               |      |             |         n = 0 -> 0xFFFF. 0x823 ~= 480fps
                               |      |             |         0x208D ~= 120fps, 0x411A ~= 60FPS
                               |      |             |         0xFFFF ~= 15fps
                               |      |             |  GET: Returns the Frame Interval in 0.1uS units
                               |      |             |
 LEAP_XU_HARDWARE_FRAME_SHIFT  |  19  |  <u32_t>    |  SET: Sets the Frame Phase Shift in 1uS units
                               |      |             |         n = 0 -> 0xFFFF
                               |      |             |  GET: Returns the Frame Phase Shift in 1uS units
                               |      |             |
 LEAP_XU_DEVCONFIG             |  20  |  <u8_t> * sizeof(LEAP_DEVCONFIG)
                               |      |             |  SET: Saves the Device Configuration values in flash memory
                               |      |             |       The values are :
                               |      |             |         *  update timestamp(32 bit value)
                               |      |             |         *  ledPower_uW(32 bit value)
                               |      |             |         *  tankCap_nF(32 bit value)
                               |      |             |         *  minimum led pulse width(32 bit value)
                               |      |             |         *  maximum led pulse width(32 bit value)
                               |      |             |         *  baseline_10um (32 bit value)
                               |      |             |         *  lens type (LEAP_LENS_ID) (32 bit value)
                               |      |             |         *  number of LEDs (32 bit value)
                               |      |             |  GET: Returns saved Device Configuration values from flash memory
                               |      |             |
 LEAP_XU_SC_DEVCAPS            |  21  |  <u8_t> * sizeof(LEAP_SC_DEVCAPS)
                               |      |             |  GET: Returns the values for the Strobe Controller Configuration
                               |      |             |       The values are :
                               |      |             |         *  System ID(16 bit value)
                               |      |             |         *  Firmware Revision(16 bit value)
                               |      |             |         *  Strobe Pulse Width Resolution(32 bit value)
                               |      |             |         *  AVR Low Fuse Setting(8 bit value)
                               |      |             |         *  AVR High Fuse Setting(8 bit value)
                               |      |             |         *  AVR Extended Fuse Setting(8 bit value)
                               |      |             |
 LEAP_XU_DEBUG_RESERVED        |  22  |  <u8_t> * sizeof(g_err)
                               |      |             |  SET:  Transfers 16 bytes that can be used for debugging
                               |      |             |  GET:  Transfers 16 bytes that can be used for debugging
                               |      |             |
 LEAP_XU_TEST_PATTERN_MODE     |  23  |  <u32_t> n  |  SET: Turns On or Off the Solid Vertical Bars Test Pattern
                               |      |             |         n = 0 : Turns Test Pattern Off
                               |      |             |         n = 1 : Turns Test Pattern On
                               |      |             |  GET: Returns last set state.  Default is 0.
                               |      |             |
 LEAP_XU_ACCELEROMETER         |  24  |  <u8_t> * sizeof(ACCELEROMETER_DATA)
                               |      |             |  SET: Initialization for the Accelerometer
                               |      |             |  GET: Returns the x, y, z axis values in ug, and status
                               |      |             |
 LEAP_XU_CALIBRATION_DATA      |  25  |  <u8_t> * CALIBRATION_DATA_SIZE
                               |      |             |  SET: Used to program the 156 bytes of calibration data
                               |      |             |  GET: Returns the 156 bytes of calibration data
                               |      |             |
 LEAP_XU_SET_UPGRADE_TARGET    |  26  |  <u32_t>    |  SET: Used to set the upgrade target
                               |      |             |  GET: Returns the upgrade target
                               |      |             |
 LEAP_XU_STROBE_CONTROL        |  27  |  <u8_t> n   |  SET: Used to Program the Sensor Strobe GPIO Output
                               |      |             |         n = 0 : Sets the Strobe GPIO to Input
                               |      |             |                 (Disables Strobe Output)
                               |      |             |         n = 1 : Sets the Strobe GPIO to Output
                               |      |             |                 (Enables Strobe Output)
                               |      |             |  GET: Returns the state of the Strobe GPIO
                               |      |             |
 LEAP_XU_OTP_CONTROLLER        |  28  |  sizeof(LEAP_OTP_CONTROLLER)
                               |      |             |  SET/GET: Controller One Time Programmable Memory Access
                               |      |             |
 LEAP_XU_OTP_SENSOR            |  29  |  sizeof(LEAP_OTP_SENSOR)
                               |      |             |  SET/GET: Sensor One Time Programmable Memory Access
                               |      |             |
 LEAP_XU_EXPOSURE_CONTROL      |  30  |  <u16_t> n  |  SET: Sets the Sensor Exposure in microseconds
                               |      |             |  GET: Returns the Sensor Exposure in microiseconds
                               |      |             |
 LEAP_XU_FRAME_ADJUST          |  31  |  sizeof(LEAP_FRAME_ADJUST)
                               |      |             |  SET: Sets the Horizontal And Vertical Sensor Position in pixels
                               |      |             |  GET: Returns the Horizontal And Vertical Sensor Position in pixels
                               |      |             |
                               |      |             |  LEAP_FRAME_ADJUST::flags
                               |      |             |        LEAP_FRAME_ADJUST_F_X_AXIS : Set X Axis to x_axis
                               |      |             |        LEAP_FRAME_ADJUST_F_Y_AXIS : Set Y Axis to y_axis
                               |      |             |  LEAP_FRAME_ADJUST::x_axis : New value for X Axis
                               |      |             |  LEAP_FRAME_ADJUST::y_axis : New value for Y Axis
                               |      |             |
------------------------------------------------------------------------------------------------------------------------
*/

// Definition of the strobe GUID in standard GUID layout
#define LEAP_XU_GUID {0x9ADA33BC, 0x8B9B, 0x4B7F, {0xA9, 0xD1, 0xF3, 0xBC, 0x8C, 0x63, 0x90, 0x29}}

// String representation of the Leap XU GUID:
#define LEAP_XU_GUID_STR "{9ADA33BC-8B9B-4B7F-A9D1-F3BC8C639029}"
#define LEAP_XU_GUID_WSTR L"{9ADA33BC-8B9B-4B7F-A9D1-F3BC8C639029}"

// Firmware will allow OTP Read/Write only if OTP_ENABLE matches this magic number "0xB055"
#define LEAP_XU_OTP_ENABLE_MAGIC 0xB055

// Max number of LEDs
#define LEAP_MAX_LEDS 20

#define LEAP_DEFAULT_RESOLUTION_SLOTS 10

typedef struct _LEAP_ESC_PROPS {
  uint32_t Delay;
  uint32_t Width;
} LEAP_ESC_PROPS;

typedef struct _LEAP_ESC_SHIFT_REG {
  // The length of the shift register, in bits
  // If this value is zero, the shift register is disabled, and the LEDs will be
  // engaged every time the strobe pin is pulled high.
  uint8_t Length;

  // The shift register proper
  uint8_t Register[16];
} LEAP_ESC_SHIFT_REG;

typedef enum _LEAP_EMBLINE_FORMAT_TYPE {
  // Invalid format type
  LEAP_EMBLINE_FORMAT_TYPE_INVALID = 0,

  // Legacy embedded line type, used in Cypress firmware
  LEAP_EMBLINE_FORMAT_TYPE_LEGACY = 1,

  // Embedded line is the last scan line in the frame
  LEAP_EMBLINE_FORMAT_TYPE_LASTLINE = 2
} LEAP_EMBLINE_FORMAT_TYPE;

typedef enum _LEAP_CONTROLLER_ID {
  CONTROLLER_UNKNOWN = 0,
  CONTROLLER_OV580 = 1
} LEAP_CONTROLLER_ID;

typedef enum _LEAP_SENSOR_ID {
  SENSOR_UNKNOWN = 0x00,
  SENSOR_OV7251 = 0x01,
  SENSOR_OV4682 = 0x02,
  SENSOR_OV9281 = 0x03,
  SENSOR_APTINA_MT9V024 = 0x04,
  SENSOR_APTINA_AR0134 = 0x05
} LEAP_SENSOR_ID;

typedef enum _LEAP_LENS_ID {
  LENS_UNKNOWN = 0x00,
  LENS_OLD = 0x01,
  LENS_SUNNY_2033A = 0x02,
  LENS_SUNNY_2D05A = 0x03,
  LENS_SUNNY_2D07A = 0x04,
  LENS_AOET_CSP = 0x05,
  LENS_LONG = 0x06,
  LENS_M12_4MM = 0x07,
  LENS_M12_6MM = 0x08,
  LENS_NEWMAX_898B7 = 0x09,
  LENS_KINKO_03942 = 0x0A,
  LENS_SUNEX_DSL217 = 0x0B,
  LENS_SUNEX_DSL218 = 0x0C,
  LENS_SUNEX_DSL219 = 0x0D,
  LENS_SUNEX_DSL180 = 0x0E,
  LENS_LCE_LV003 = 0x0F
} LEAP_LENS_ID;

typedef struct _LEAP_EMBLINE_FORMAT_LEGACY {
  // Embedded line format type, defined to be LEAP_EMBLINE_FORMAT_TYPE_LEGACY
  uint8_t type;
} LEAP_EMBLINE_FORMAT_LEGACY;

typedef struct _LEAP_EMBLINE_FORMAT_LASTLINE {
  // Embedded line format type, defined to be LEAP_EMBLINE_FORMAT_TYPE_LASTLINE
  uint8_t type;
  uint8_t reserved;

  // Offset of the various supported fields, given as distances from the start of
  // the last scan line.  A value of 0xFF indicates that the field is not present.
  uint16_t cycleLabel;
  uint16_t frameLabel;
  uint16_t exposure;
  uint16_t gain;
  uint16_t vcenter;
  uint16_t vzoom;
  uint16_t frameCount;
  uint16_t hwClock;
  uint16_t accelerometer;
} LEAP_EMBLINE_FORMAT_LASTLINE;

// Set if property knocking has been removed from this device
#define LEAP_DEVCAPS_F_PROP_KNOCKING_REMOVED      0x00000001

// Set if the serial number may be modified
#define LEAP_DEVCAPS_F_SN_MUTABLE                 0x00000002

// Set if the device is presently running in super speed (USB 3.0) mode
#define LEAP_DEVCAPS_SUPER_SPEED                  0x00000004

typedef struct _LEAP_DEVCAPS {
  uint32_t reserved;
  uint32_t flags;
  uint32_t firmware_rev;
  uint8_t controller_id; // Contains uint8_t version of LEAP_CONTROLLER enum
  uint8_t sensor_id; // Contains uint8_t version of LEAP_SENSOR enum

  // The device's serial number.  This number must be exactly equal to the serial
  // number reported in the USB device descriptor.  This string is null-terminated,
  // unless the string length is 64 characters, in which case the null termiantor
  // is omitted.
  char serial[64];
} LEAP_DEVCAPS;

typedef struct _secp256r1_signature {
  uint8_t r[32];
  uint8_t s[32];
} secp256r1_signature;

typedef struct _LEAP_DEVCONFIG {
  // timestamp of when the dev config was last updated
  // timestamp is in minutes since the unix epoch
  uint32_t update_timestamp;

  // Power of LEDs in microwatts
  uint32_t ledPower_uW;

  // Tank capacitance in nanofarads
  uint32_t tankCap_nF;

  // Min Pulse width of LEDs in Microseconds
  uint32_t ledPulseWidthMin_uS;

  // Max Pulse width of LEDs in Microseconds
  uint32_t ledPulseWidthMax_uS;

  // The baseline of board in 10um increments
  uint32_t baseline_10um;

  // The type of lens the device is using (see LEAP_LENS_ID)
  uint32_t lensType;

  // The number of LEDs
  uint32_t ledCount;

  // The device's digital signature.  If this field is all zeroes, a signature
  // is not present on the device.
  secp256r1_signature deviceSignature;
} LEAP_DEVCONFIG;

typedef struct _LEAP_U16_VEC3 {
  uint16_t x;
  uint16_t y;
  uint16_t z;
} LEAP_U16_VEC3;

typedef struct _LEAP_LED_POSITIONS {
  // Positions are in hundreths of a millimeter
  LEAP_U16_VEC3 ledPositions[LEAP_MAX_LEDS];
} LEAP_LED_POSITIONS;

typedef struct _LEAP_SC_DEVCAPS {
  // Strobe Controller Information
  uint16_t sc_system_id;
  uint16_t sc_firmware_rev;

  // Strobe controller resolution, in nanoseconds
  uint32_t sc_str_res_nS;

  union {
    uint32_t sc_fuse_bits;
    struct {
      uint8_t sc_fuse_low;
      uint8_t sc_fuse_high;
      uint8_t sc_fuse_extended;
      uint8_t sc_fuse_reserved;
    };
  };

  // The strobe controller resolution, in femtoseconds.
  // This field is meant to supercede sc_str_res_nS, and should be used instead of sc_str_res_nS
  // if it is nonzero.
  uint64_t sc_str_res_fS;
} LEAP_SC_DEVCAPS;

typedef struct _LEAP_HARDWARE_FRAME_INTERVAL {
  // The duration of a single frame, in 100ns increments
  uint32_t frameInterval;
} LEAP_HARDWARE_FRAME_INTERVAL;

typedef struct _LEAP_DEBUG_STRUCTURE {
  uint8_t reserved[16];
} LEAP_DEBUG_STRUCTURE;

typedef struct _LEAP_OTP_SENSOR_LEGACY {
  // To enable OTP (for get/set), the enable needs to match LEAP_XU_OTP_ENABLE_MAGIC
  uint32_t enable;
  uint8_t data[8];
} LEAP_OTP_SENSOR_LEGACY;

typedef struct _LEAP_OTP_CONTROLLER {
  // To enable OTP (for get/set), the enable needs to match LEAP_XU_OTP_ENABLE_MAGIC
  uint32_t enable;
  uint32_t sensor_leap_otp_length;
  uint8_t sensor_leap_otp[32];
} LEAP_OTP_CONTROLLER;

typedef struct _LEAP_OTP_SENSOR {
  // To enable OTP (for get/set), the enable needs to match LEAP_XU_OTP_ENABLE_MAGIC
  uint32_t enable;
  uint32_t sensor_leap_otp_length;
  uint8_t sensor_leap_otp[32];
  uint32_t sensor_omnivision_otp_length;
  uint8_t sensor_omnivision_otp[32];
} LEAP_OTP_SENSOR;

typedef struct _LEAP_SET_UPGRADE_TARGET {
  // Specifies the target version number of the next firmware update that the host
  // will be performing on the device.  Based on this version number, the device may
  // optionally choose to silently reject the host's request to upgrade firmware.
  // This is done to prevent a host from mistakenly downgrading the device's firmware
  // to an earlier version.
  //
  // If this value is set to 0xFFFF, the device MUST accept the firmware version
  // supplied by the host, regardless of whether it is a downgrade.
  uint16_t bcdTarget;

  // Reserved for future use.  Required because the firmware does not support
  // extension units smaller than 32 bits.
  uint16_t reserved;
} LEAP_SET_UPGRADE_TARGET;
//STATIC_ASSERT(sizeof(LEAP_SET_UPGRADE_TARGET) == 4, upgTgtExtent, "Upgrade target XU field must be exactly four bytes long");

typedef enum _LEAP_RESOLUTION {
  // USB 2.0 resolutions:
  DEFAULT = 0,
  ROBUST_MODE = 1,
  CALIBRATION = 2,
  HIGH_PERFORMANCE = 3,

  // USB 3.0 resolutions:
  DEFAULT_SS = 5,
  ROBUST_MODE_SS = 6,
  CALIBRATION_SS = 7,
  HIGH_PERFORMANCE_SS = 8,
} LEAP_RESOLUTION;

typedef struct _LEAP_DEFAULT_RESOLUTION {
  uint16_t x_axis;
  uint16_t y_axis;
  uint16_t fps;
  uint8_t  x_binning;
  uint8_t  y_binning;
} LEAP_DEFAULT_RESOLUTION;

typedef enum _LEAP_FRAME_ADJUST_FLAGS {
  LEAP_FRAME_ADJUST_F_Y_AXIS = 0x00000001,  // Set if adjusting Y axis
  LEAP_FRAME_ADJUST_F_X_AXIS = 0x00000002   // Set if adjusting X axis
} LEAP_FRAME_ADJUST_FLAGS;

typedef struct _LEAP_FRAME_ADJUST {
  uint32_t flags;
  uint32_t x_axis;
  uint32_t y_axis;
} LEAP_FRAME_ADJUST;

typedef LEAP_DEFAULT_RESOLUTION LEAP_DEFAULT_RESOLUTION_ARRAY[LEAP_DEFAULT_RESOLUTION_SLOTS];

// Leap Motion extension unit:
LFW_DECLARE_ENUM(LEAP_XU, unsigned int) {
  // OV580 extension unit definitions:
  LEAP_XU_UNDEFINED = 0x00,
  LEAP_XU_STROBE_WIDTH = 0x01,
  LEAP_XU_LED_POSITIONS = 0x02,
  LEAP_XU_STROBE_INTERVAL = 0x03,

  // Sets the LED on the mainboard
  LEAP_XU_LED_SET = 0x04,

  // Resets the device
  LEAP_XU_ESC_RESET_STATUS = 0x05,

  // Enables or disables the charge circuit on the LED board
  LEAP_XU_ESC_LED_CHARGE = 0x06,

  // Forces the LEDs to the ON state regardless of the strobe interrupt pin status
  LEAP_XU_ESC_GPIO_OVERRIDE = 0x07,

  LEAP_XU_ESC_PULSE_DELAY = 0x08,
  LEAP_XU_ESC_PULSE_WIDTH = 0x09,

  // Default configuration resolutions
  LEAP_XU_RESOLUTION_CONFIGS = 0x0A,

  LEAP_XU_FRAME_COUNT = 0x0B,
  LEAP_XU_ESC_SHIFT_RESET_START = 0x0C,
  LEAP_XU_ESC_SHIFT_BIT_DATA = 0x0D,
  LEAP_XU_ESC_SHIFT_BIT_LENGTH = 0x0E,
  LEAP_XU_HARDWARE_CLOCK = 0x0F,

  // A LEAP_DEVCAPS structure.
  LEAP_XU_DEVCAPS = 0x10,

  // One of the LEAP_EMBLINE_FORMAT_XXX structures.  The specific structure is defined
  // by the value of the first byte, and taken from LEAP_EMBLINE_FORMAT_TYPE
  LEAP_XU_EMBLINE_BEHAVIOR = 0x11,

  // A LEAP_HARDWARE_FRAME_INTERVAL structure
  // Obtains or assigns the device's frame interval
  LEAP_XU_HARDWARE_FRAME_INTERVAL = 0x12,

  // A LEAP_HARDWARE_FRAME_SHIFT structure
  // Causes an adjustment to a single frame
  LEAP_XU_HARDWARE_FRAME_SHIFT = 0x13,

  // A LEAP_DEVCONFIG structure.
  LEAP_XU_DEVCONFIG = 0x14,

  // A LEAP_SC_DEVCAPS structure.
  LEAP_XU_SC_DEVCAPS = 0x15,

  // A general-purpose debug block.  Variable length, format is completely defined
  // by the device.  This is intended to be used exclusively in debug cases.  Production
  // devices do not support this extension unit
  // Nominally, LEAP_DEBUG_STRUCTURE is used to contain the data defined by this XU
  LEAP_XU_DEBUG_RESERVED = 0x16,

  // Enables or disables the test pattern mode
  LEAP_XU_TEST_PATTERN_MODE = 0x17,

  // Enable access to the Accelerometer
  LEAP_XU_ACCELEROMETER = 0x18,

  // Load calibration data to flash
  LEAP_XU_CALIBRATION_DATA = 0x19,

  // Specifies the intended firmware version update target
  LEAP_XU_SET_UPGRADE_TARGET = 0x1A,

  // Gate strobe on/off
  LEAP_XU_STROBE_CONTROL = 0x1B,

  // Obtains or assigns the controller's OTP
  LEAP_XU_OTP_CONTROLLER = 0x1C,

  // Obtains or assigns the sensor's OTP
  LEAP_XU_OTP_SENSOR = 0x1D,

  // Exposure control
  LEAP_XU_EXPOSURE_CONTROL = 0x1E,

  // Y Axis Frame position adjustment
  LEAP_XU_FRAME_ADJUST = 0x1F
};

static const char* const LEAP_SC_NAMES [] = {
  "LEAP_XU_STROBE_WIDTH",
  "LEAP_XU_LED_POSITIONS",
  "LEAP_XU_STROBE_INTERVAL",
  "LEAP_XU_LED_SET",
  "LEAP_XU_ESC_RESET_STATUS",
  "LEAP_XU_ESC_LED_CHARGE",
  "LEAP_XU_ESC_GPIO_OVERRIDE",
  "LEAP_XU_ESC_PULSE_DELAY",
  "LEAP_XU_ESC_PULSE_WIDTH",
  "LEAP_XU_RESOLUTION_CONFIGS",
  "LEAP_XU_FRAME_COUNT",
  "LEAP_XU_ESC_SHIFT_RESET_START",
  "LEAP_XU_ESC_SHIFT_BIT_DATA",
  "LEAP_XU_ESC_SHIFT_BIT_LENGTH",
  "LEAP_XU_HARDWARE_CLOCK",
  "LEAP_XU_DEVCAPS",
  "LEAP_XU_EMBLINE_BEHAVIOR",
  "LEAP_XU_HARDWARE_FRAME_INTERVAL",
  "LEAP_XU_HARDWARE_FRAME_SHIFT",
  "LEAP_XU_DEVCONFIG",
  "LEAP_XU_SC_DEVCAPS",
  "LEAP_XU_DEBUG_RESERVED",
  "LEAP_XU_TEST_PATTERN_MODE",
  "LEAP_XU_ACCELEROMETER",
  "LEAP_XU_CALIBRATION_DATA",
  "LEAP_XU_SET_UPGRADE_TARGET",
  "LEAP_XU_STROBE_CONTROL",
  "LEAP_XU_OTP_CONTROLLER",
  "LEAP_XU_OTP_SENSOR",
  "LEAP_XU_EXPOSURE_CONTROL",
  "LEAP_XU_FRAME_ADJUST"
};

#pragma pack()