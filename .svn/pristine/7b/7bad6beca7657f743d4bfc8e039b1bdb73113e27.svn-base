/*
 * Ecosat2_messages.h
 *
 *			Application layer message definitions for all subsystems please follow the template [SUBSYSTEM]_[descriptive name]_ID
 *			These values can overlap between different systems but we have a large enough message identifier space to avoid this
 *			(high and low values denote upper and lower bytes respectively)
 *
 *			TIP! holding the ALT key will turn most code editors (AVR studio, sublime etc) into column mode allowing you to select
 *				the column of number if you need to shift them up or down after adding or removing a message
 *
 * Created: 12/05/2014 3:04:07 PM
 *  Author: Cass
 */
#include "compiler.h"
/****************************************************************************************
				General (GEN)
****************************************************************************************/
#define GEN_UPDATE_TIME_ID					(U32)0			//Sends updated RTC time value from OBC (obtained from GPS module) |data: [time] (this will be more than 1 byte)
#define GEN_REQUEST_TIME_ID					(U32)1
#define GEN_BAD_SEQUENCE_ID					(U32)2

/****************************************************************************************
				Power System (PWR)
****************************************************************************************/
#define PWR_EMERGENCY_ALERT_ID			    (U32)3			//Sends Emergency alert message to OBC in over/under volt/temp etc situations |  data: [time, value, type] (time will be more than 1 byte)
#define PWR_STATUS_B0P0_ID				    (U32)4			//Sends battery board 0 page 0 telemetry | data: [temp1_high, temp1_low, temp2_high, temp2_low,  temp3_high, temp3_low, temp4_high, temp4_low]
#define PWR_STATUS_B0P1_ID				    (U32)5			//Sends battery board 0 page 1 telemetry | data: [voltage1_high, voltage1_low, voltage2_high, voltage2_low,  voltage3_high, voltage3_low, voltage4_high, voltage4_low]
#define PWR_STATUS_B1P0_ID				    (U32)6			//Sends battery board 1 page 0 telemetry | data: [temp1_high, temp1_low, temp2_high, temp2_low,  temp3_high, temp3_low, temp4_high, temp4_low]
#define PWR_STATUS_B1P1_ID				    (U32)7			//Sends battery board 1 page 1 telemetry | data: [voltage1_high, voltage1_low, voltage2_high, voltage2_low,  voltage3_high, voltage3_low, voltage4_high, voltage4_low]
#define PWR_STATUS_REGP0_ID				    (U32)8			//Sends regulation board telemetry (room left for expansion) | data: [temp1_high, temp1_low, temp2_high, temp2_low, temp3_high, temp3_low]
#define PWR_STATUS_SOLARP0_ID			    (U32)9			//Sends solar cell page 0 telemetry | data: [x-_high, x-_low, x_high, x_low,  y-_high, y-_low, y_high, y_low]
#define PWR_STATUS_SOLARP1_ID			    (U32)10			//Sends solar cell page 1 telemetry | data: [zenith_high, zenith_low]
#define PWR_WARNING_EVENT_ID			    (U32)11			//Sends Warning message to OBC |  data: [time, value, type]
#define PWR_BATTERY_ARRAY_STATUS_ID		    (U32)12			//Sends the current control states of the battery arrays (0 for disconnected, 1 for connected) | data: [b0a0_status, b0a1_status, b0a2_status, b0a3_status, b1a0_status, b1a1_status, b1a2_status, b1a3_status]
#define PWR_BATTERY_ARRAY_CTRL_ID		    (U32)13			//OBC->PWR message to control states of the battery arrays (b[n]a[i] -> board n array i, 0 for disconnect, 1 for connect) | data: [b0a0, b0a1, b0a2, b0a3, b1a0, b1a1, b1a2, b1a3]
#define PWR_REQUEST_STATUS_ID			    (U32)14			//OBC->PWR message to request status messages | data: [B0P0, B0P1, B1P0, B1P1, REGP0, SOLARP0. SOLARP1, EVENT]
#define PWR_DUTY_CYCLE_CTRL_ID			    (U32)15			//Message left for future expansion, do something to control systems power consumptions up or down
#define PWR_OVRTEMP_ALERT_ID				(U32)16			//Duplicate of functionality in bus connector sent when temperature sensor raises an Int or CT interrupt

/****************************************************************************************
				Attitude Control System (ACS)
****************************************************************************************/

//RX message IDs------------------------------------------
#define ACS_STATUS_REQUEST_ID				(U32)17		//message to request ACS_HB_FAULT_ID message
#	define TEMP								0
#	define FAULT							1

#define ACS_AXIS_CTRL_REQUEST_ID			(U32)18
#	define X_CONTROL						0
#	define Y_CONTROL						1
#	define Z_CONTROL						2
#	define X_DURATION_MS					3
#	define Y_DURATION_MS					4
#	define Z_DURATION_MS					5

#define ACS_HBRIDGE_ENABLE_REQUEST_ID		(U32)19
#	define X1								0
#	define X2								1
#	define Y1								2
#	define Y2								3
#	define Z1								4
#	define Z2								5

#define ACS_DEGAUSS_REQUEST_ID				(U32)20		//sends command to run degaussing procedures on x,y, or/and z axis magnetorquers | data: [AXES]
#	define X1								0
#	define X2								1
#	define Y1								2
#	define Y2								3
#	define Z1								4
#	define Z2								5

//TX message IDs------------------------------------------
#define ACS_STATUS_FAULT_RESPONSE_ID		(U32)21
#	define X1_FAULT							0
#	define X2_FAULT							1
#	define Y1_FAULT							2
#	define Y2_FAULT							3
#	define Z1_FAULT							4
#	define Z2_FAULT							5

#define ACS_STATUS_TEMP_RESPONSE_ID			(U32)22
#	define TEMP_LOW							0
#	define TEMP_HIGH						1

#define ACS_AXIS_CTRL_RESPONSE_ID			(U32)18
#	define HB								0	//The response contains the number of the H-bridge that carried out the command, and is sent upon completion.

#define ACS_HBRIDGE_ENABLE_RESPONSE_ID		(U32)23
#	define X1								0
#	define X2								1
#	define Y1								2
#	define Y2								3
#	define Z1								4
#	define Z2								5

#define ACS_OVRTEMP_ALERT_ID				(U32)25
#	define TEMP_LOW							0
#	define TEMP_HIGH						1

#define ACS_FAULT_ALERT_ID					(U32)26
#	define X1_FAULT							0
#	define X2_FAULT							1
#	define Y1_FAULT							2
#	define Y2_FAULT							3
#	define Z1_FAULT							4
#	define Z2_FAULT							5

#define ACS_DEGAUSS_RESPONSE_ID				(U32)20
#	define X1								0
#	define X2								1
#	define Y1								2
#	define Y2								3
#	define Z1								4
#	define Z2								5


//#define ACS_DUTY_CYCLE_CTRL_ID			(U32)27		//Message left for future expansion, do something to control systems power consumptions up or down

/****************************************************************************************
				Payload system (PAYLOAD)
****************************************************************************************/
//RX message IDs------------------------------------------
#define PAYLOAD_STATUS_REQUEST_ID			(U32)25			//request status messages (1 for request, 0 for not) | data [temp, gyro, accel, mag]

#define PAYLOAD_TEMP_REQUEST_ID				(U32)26			//current Temperature values	| data: [temp0_high, temp0_low, temp1_high, temp1_low, temp2_high, temp02_low, temp3_high, temp3_low,]
#	define REQUEST							0
#		define TEMP0_MASK					(1<<0)
#		define TEMP1_MASK					(1<<1)
#		define TEMP2_MASK					(1<<2)
#		define TEMP3_MASK					(1<<3)

#define PAYLOAD_CAMERA_INITIAL_ID			(U32)25	
#	define COLOUR_TYPE				0
#	define RAW_RES					1
	
#define PAYLOAD_CAMERA_GET_PICTURE_ID		(U32)26
#	define PICTURE_TYPE				0

#define PAYLOAD_CAMERA_SNAPSHOT_ID			(U32)27
#	define SNAPSHOT_TYPE				0
#	define SKIP_FRAME_COUNTER			1
	
#define PAYLOAD_CAMERA_SET_PACKAGE_SIZE_ID	(U32)28
#	define PACKAGE_SIZE_LOW			0 //
#	define PACKAGE_SIZE_HIGH			1 //must not be odd or a multiple of 16
	
#define PAYLOAD_CAMERA_SET_BAUD_ID			(U32)29
#	define FIRST_DIVIDER				0
#	define SECOND_DIVIDER				1
	
#define PAYLOAD_CAMERA_RESET_ID				(U32)30
#	define RESET_TYPE					0

#define PAYLOAD_CAMERA_DATA_ID				(U32)31
#	define DATA_TYPE					0
#	define LENGTH0					1
#	define LENGTH1					2
#	define LENGTH2					3

#define PAYLOAD_CAMERA_SYNC_ID				(U32)32

#define PAYLOAD_CAMERA_LIGHT_ID				(U32)33
#	define LIGHT_FREQUENCY_TYPE		0
	
#define PAYLOAD_RESULTS_REQUEST_ID			(U32)34//sent as a data transfer in the bus protocol#define PAYLOAD_UPDATE_ATTITUDE_ID			

#define PAYLOAD_EXPERIMENT_ID				(U32)35	//initiate experiment

			

//TX message IDs------------------------------------------
#define PAYLOAD_STATUS_RESPONSE_ID			(U32)36
#	define TEMP0_STATUS				0
#	define TEMP1_STATUS				1
#	define TEMP2_STATUS				2
#	define TEMP3_STATUS				3
#		define TLOW_MASK					(1<<4)
#		define THIGH_MASK					(1<<5)
#		define TCRIT_MASK					(1<<6)
#		define RDY_MASK						(1<<7)
#	define GYRO_STATUS				4
#	define MAG_STATUS					5
#	define ACCEL_STATUS				6

#define PAYLOAD_TEMP_RESPONSE_ID			(U32)37
#	define TEMP0_LOW					0
#	define TEMP0_HIGH					1
#	define TEMP1_LOW					2
#	define TEMP1_HIGH					3
#	define TEMP2_LOW					4
#	define TEMP2_HIGH					5
#	define TEMP3_LOW					6
#	define TEMP3_HIGH					7

#define PAYLOAD_RESULTS_RESPONSE_ID			(U32)38		//sent as a data transfer in the bus protocol#define PAYLOAD_UPDATE_ATTITUDE_ID
#define PAYLOAD_GYRO_DATA_RESPONSE_ID		(U32)39		//current IMU gyroscope values | data: [x_high, x_low, y_high, y_low, z_high, z_low]
#define PAYLOAD_MAG_DATA_RESPONSE_ID		(U32)41		//current IMU magnetometer values| data: [x_high, x_low, y_high, y_low, z_high, z_low]
#define PAYLOAD_ACCEL_DATA_RESPONSE_ID		(U32)42		//current IMU accelerometer values | data: [x_high, x_low, y_high, y_low, z_high, z_low]
			
#define PAYLOAD_OVRTEMP_ALERT_ID			(U32)43		//Over temperature or under temperature alert preventing and experiment from being run | data: [temp0, temp1, temp2, temp3]
