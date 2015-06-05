/*
 * Ecosat2_messages.c
 *
 * Created: 10/09/2014
 *  Author: Brad Maibach
 *
 *	This file contains one function with cases for programming all responses to CAN Bus messages.
 *	This should be used by all subsystems for the programming of actions to be taken upon receiving a particular message.
 *
 *	
 */ 

//_____ I N C L U D E S ___________________________________________________________________________
#include "Ecosat2_messages.h"

//_____ D E F I N I T I O N S _____________________________________________________________________

ecosat2_messages(can_format receive_command)
{
	U32 receive_command_header = (receive_command.id & message_command_data_mask) >> message_command_data_offset;
	switch(receive_command_header){

                /****************************************************************************************
				General (GEN)
				****************************************************************************************/
                case GEN_UPDATE_TIME_ID:
				break;
				case GEN_REQUEST_TIME_ID:	
                break;
				case GEN_BAD_SEQUENCE_ID:
                break;
				/****************************************************************************************
				Power System (PWR)
				****************************************************************************************/
                case PWR_EMERGENCY_ALERT_ID:						//Sends Emergency alert message to OBC in over/under volt/temp etc situations |  data: [time, value, type] (time will be more than 1 byte)
				break;
				case PWR_STATUS_B0P0_ID:				    		//Sends battery board 0 page 0 telemetry | data: [temp1_high, temp1_low, temp2_high, temp2_low,  temp3_high, temp3_low, temp4_high, temp4_low]
				break;
				case PWR_STATUS_B0P1_ID:				   			//Sends battery board 0 page 1 telemetry | data: [voltage1_high, voltage1_low, voltage2_high, voltage2_low,  voltage3_high, voltage3_low, voltage4_high, voltage4_low]
				break;
				case PWR_STATUS_B1P0_ID:				   			//Sends battery board 1 page 0 telemetry | data: [temp1_high, temp1_low, temp2_high, temp2_low,  temp3_high, temp3_low, temp4_high, temp4_low]
                break;
				case PWR_STATUS_B1P1_ID:							//Sends battery board 1 page 1 telemetry | data: [voltage1_high, voltage1_low, voltage2_high, voltage2_low,  voltage3_high, voltage3_low, voltage4_high, voltage4_low]
                break;
				case PWR_STATUS_REGP0_ID:				   		//Sends regulation board telemetry (room left for expansion) | data: [temp1_high, temp1_low, temp2_high, temp2_low, temp3_high, temp3_low]
                break;
				case PWR_STATUS_SOLARP0_ID:			   			//Sends solar cell page 0 telemetry | data: [x-_high, x-_low, x_high, x_low,  y-_high, y-_low, y_high, y_low]
                break;
				case PWR_STATUS_SOLARP1_ID:			    		//Sends solar cell page 1 telemetry | data: [zenith_high, zenith_low]
                break;
				case PWR_WARNING_EVENT_ID:			   			//Sends Warning message to OBC |  data: [time, value, type]
                break;
				case PWR_BATTERY_ARRAY_STATUS_ID:		  		//Sends the current control states of the battery arrays (0 for disconnected, 1 for connected) | data: [b0a0_status, b0a1_status, b0a2_status, b0a3_status, b1a0_status, b1a1_status, b1a2_status, b1a3_status]
                break;
				case PWR_BATTERY_ARRAY_CTRL_ID:		    		//OBC->PWR message to control states of the battery arrays (b[n]a[i] -> board n array i, 0 for disconnect, 1 for connect) | data: [b0a0, b0a1, b0a2, b0a3, b1a0, b1a1, b1a2, b1a3]
                break;
				case PWR_REQUEST_STATUS_ID:			    		//OBC->PWR message to request status messages | data: [B0P0, B0P1, B1P0, B1P1, REGP0, SOLARP0. SOLARP1, EVENT]
                break;
				case PWR_DUTY_CYCLE_CTRL_ID:			    		//Message left for future expansion, do something to control systems power consumptions up or down
                break;
				case PWR_OVRTEMP_ALERT_ID:						//Duplicate of functionality in bus connector sent when temperature sensor raises an Int or CT interrupt
                break;
				
				/****************************************************************************************
				Attitude Control System (ACS)
				****************************************************************************************/

				
				/****************************************************************************************
				Payload system (PAYLOAD)
				****************************************************************************************/
                case PAYLOAD_OVRTEMP_ALERT_ID:					//Over temperature or under temperature alert preventing and experiment from being run | data: [temp0, temp1, temp2, temp3] 
                break;
				case PAYLOAD_STATUS_GYRO_ID:						//current IMU gyroscope values | data: [x_high, x_low, y_high, y_low, z_high, z_low]
                break;
				case PAYLOAD_STATUS_MAG_ID:						//current IMU magnetometer values| data: [x_high, x_low, y_high, y_low, z_high, z_low]
                break;
				case PAYLOAD_STATUS_ACCEL_ID:					//current IMU accelerometer values | data: [x_high, x_low, y_high, y_low, z_high, z_low]
                break;
				case PAYLOAD_STATUS_TEMP_ID:						//current Temperature values	| data: [temp0_high, temp0_low, temp1_high, temp1_low, temp2_high, temp02_low, temp3_high, temp3_low,]
                break;
				case PAYLOAD_REQUEST_STATUS_ID:					//request status messages (1 for request, 0 for not) | data [temp, gyro, accel, mag]
                break;
				case PAYLOAD_CAMERA_INITIAL_ID:		
                break;
				case PAYLOAD_CAMERA_GET_PICTURE_ID:		
                break;
				case PAYLOAD_CAMERA_SNAPSHOT_ID:			
                break;
				case PAYLOAD_CAMERA_SET_PACKAGE_SIZE_ID:	
                break;
				case PAYLOAD_CAMERA_SET_BAUD_ID:			
                break;
				case PAYLOAD_CAMERA_RESET_ID:				
                break;
				case PAYLOAD_CAMERA_DATA_ID:				
                break;
				case PAYLOAD_CAMERA_SYNC_ID:				
                break;
				case PAYLOAD_CAMERA_ACK_ID:				
                break;
				case PAYLOAD_CAMERA_NAK_ID:				
                break;
				case PAYLOAD_CAMERA_LIGHT_ID:			
                break;
				case PAYLOAD_EXPERIMENT_ID:						//initiate experiment
                break;
				case PAYLOAD_REQUEST_RESULTS_ID:				//sent as a data transfer in the bus protocol
                break;
				case PAYLOAD_UPDATE_ATTITUDE_ID:			
                break;
				case PAYLOAD_CAMERA_TAKE_RAW_ID:
				break;								
	}
}

//_____ M A C R O S _______________________________________________________________________________

//_____ I M P L E M E N T A T I O N S _____________________________________________________________
/*
#################################################
		Initializations
#################################################
*/
//-----------------------------------------------

/*
#################################################
		Tasks
#################################################
*/

/*
#################################################
		Other Functions
#################################################
*/

/*
#################################################
		Interrupt Service Routines
#################################################
*/
