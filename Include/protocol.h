/*
 * IncFile1.h
 *
 * Created: 14/05/2013 11:41:20 AM
 *  Author: Cass
 *
 *	This file contains definition and structure types used by the protocol implemented on the Can
 *	Bus side for acknowledgments and data formating within the identifier field.
 *
 *		I have been calling this the bus side of the protocol, for system messages/application side see Ecosat2_messages.h
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

//Data Structures
typedef  struct{
	U32 id;
	U8 dlc;
	U8 ide;
	U8  packet_data[8];
} can_format;

#define can_recieve_format can_format //for legacy code from 499 project, keep for now

typedef struct{
	U8 station_timer_state;
	U8 station_ack_state;
	can_format station_message_1;
	//can_format station_message_2;
	xTimerHandle station_timer;
}reservation_station;

//Identifier field offsets
#define message_destination_id_offset			26
#define message_source_id_offset				23
#define message_command_offset					19
#define message_command_data_offset				5
#define message_sequence_num_offset				2
#define message_nack_offset						1
#define message_ack_offset						0

#define message_id_mask_offset					message_destination_id_offset
#define message_destination_source_difference	(message_destination_id_offset - message_source_id_offset)

//Identifier field masks
#define message_destination_id_mask ((U32)0x7 << message_destination_id_offset)
#define message_source_id_mask		((U32)0x7 << message_source_id_offset)
#define message_command_mask		((U32)0x7 << message_command_offset)
#define message_command_data_mask	((U32)0x3FFF << message_command_data_offset)
#define message_message_num_mask	((U32)0x7 << message_sequence_num_offset)
#define message_nack_mask			((U32)0x1 << message_nack_offset)
#define message_ack_mask			((U32)0x1 << message_ack_offset)

//System IDs
#define message_CPU_ID			(U32)0
#define message_ACDS_ID			(U32)1
#define message_comms_modem_ID	(U32)2
#define message_comms_rxtx_ID	(U32)3
#define message_power_ID		(U32)4
//addresses 5 unused currently
#define message_payload_ID		(U32)6
#define message_broadcast_ID	(U32)7

//Transmit timeout states
#define reservation_free		(U8)0
#define reservation_waiting		(U8)1
#define reservation_miss_two	(U8)2
#define reservation_miss_three	(U8)3
#define reservation_miss_four	(U8)4
#define reservation_miss_five	(U8)5
#define reservation_miss_six	(U8)6
#define reservation_miss_seven	(U8)7

#define reservation_default_max_number_of_trys reservation_miss_two

//Transmit acknowledgment states
#define StateFree				(U8)0
#define StateSent				(U8)1

//UART States
#define UART_WAITING			0
#define UART_RX_NORMAL			1
#define UART_RX_ISP				3






//Low level Commands and command data field values
#define message_cmd_send_data_packet							(U8)0
	#define message_cmd_send_data_packet_data_offset			0 + message_command_data_offset
	#define message_cmd_send_data_packet_data_mask				((U32)0x3FFF << message_cmd_send_data_packet_data_offset)

#define message_cmd_start_data_transfer							(U8)1
	#define message_cmd_start_data_transfer_page_offset			0 + message_command_data_offset
	#define message_cmd_start_data_transfer_check_sum_offset	6 + message_command_data_offset
	#define message_cmd_start_data_transfer_page_mask			((U32)0xFF << message_cmd_start_data_transfer_page_offset)
	#define message_cmd_start_data_transfer_check_sum_mask   	((U32)0x3F << message_cmd_start_data_transfer_check_sum_offset)
	
#define message_cmd_send_data_transfer							(U8)2	
	#define message_cmd_send_data_transfer_end_offset			0 + message_command_data_offset
	#define message_cmd_send_data_transfer_end_mask				((U32)0x1 << message_cmd_send_data_transfer_end_offset)
	
#define message_cmd_request_data_transfer						(U8)3
	#define message_cmd_request_data_transfer_page_offset		0 + message_command_data_offset
	#define message_cmd_request_data_transfer_page_mask			((U32)0xFF << message_cmd_request_data_transfer_page_offset)

#define message_cmd_sequence_reset								(U8)4

#define message_cmd_non_responsive_alert						(U8)5
	#define message_cmd_non_responsive_alert_subsystem_offset   0
	#define message_cmd_non_responsive_alert_subsystem_mask     ((U32)0x7 << message_cmd_non_responsive_alert_subsystem_offset)
	
#define message_cmd_bus_config									(U8)6
	#define message_cmd_bus_config_C_offset						0
	#define message_cmd_bus_config_S_offset						1
	#define message_cmd_bus_config_R_offset						2
	#define message_cmd_bus_config_C_mask						((U32)0x1 << message_cmd_bus_config_C_offset)
	#define message_cmd_bus_config_S_mask						((U32)0x1 << message_cmd_bus_config_S_offset)
	#define message_cmd_bus_config_R_mask						((U32)0x1 << message_cmd_bus_config_R_offset)
	
#define message_cmd_echo										(U8)7
	#define message_cmd_echo_svn_version_offset					0
	#define message_cmd_echo_request_offset						8
	#define message_cmd_echo_svn_version_mask					((U32)0xFF << message_cmd_echo_svn_version_offset)
	#define message_cmd_echo_request_mask						((U32)0x1 << message_cmd_echo_request_offset)
	
#define message_cmd_reset										(U8)8

#define message_cmd_select_device								(U8)9
	#define message_cmd_select_device_device_offset				0
	#define message_cmd_select_device_device_mask				((U32)0x3FFF << message_cmd_select_device_device_offset)
	
#define message_cmd_programming_start							(U8)10
	#define message_cmd_programming_start_mode_offset			0
	#define message_cmd_programming_start_mode_mask				((U32)0xFF << message_cmd_programming_start_mode_offset)

#define message_cmd_programming_data							(U8)11
	#define message_cmd_programming_data_offset_offset			0
	#define message_cmd_programming_data_offset_mask			((U32)0x1FFF << message_cmd_programming_data_offset_offset)
	
#define message_cmd_display_data								(U8)12
	#define message_cmd_display_data_mode_offset				1
	#define message_cmd_display_data_request_offset				0
	#define message_cmd_display_data_mode_mask					((U32)0xFF << message_cmd_display_data_mode_offset)
	#define message_cmd_display_data_request_mask				((U32)0x1 << message_cmd_display_data_request_offset)
	
#define message_cmd_start_application							(U8)13

#define message_cmd_set_mem_page								(U8)14
	#define message_cmd_set_mem_page_M_offset					1
	#define message_cmd_set_mem_page_P_offset					0
	#define message_cmd_set_mem_page_M_mask						((U32)0x1 << message_cmd_set_mem_page_M_offset)
	#define message_cmd_set_mem_page_P_mask						((U32)0x1 << message_cmd_set_mem_page_P_offset)

#define message_cmd_send_data_packet_extended					(U8)15
	#define message_cmd_send_data_packet_data_extended_offset	0
	#define message_cmd_send_data_packet_data_extended_mask		((U32)0x3FFF << message_cmd_send_data_packet_data_extended_offset)


#endif /* INCFILE1_H_ */