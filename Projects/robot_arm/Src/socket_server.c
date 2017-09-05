#include "socket_server.h"

void socket_server_thread(void const *argument)
{
	// Define params
	int server_socket;
	struct sockaddr_in server_addr;
	socket_server_on = 1;



	log_msg(USER, "TCP Server thread started.\n");

	// Create server socket
	server_socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket < 0) {
		log_msg(ERROR, "Cannot create socket.\n");
		osThreadTerminate(NULL);
	}

	// Create sockaddr_in structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_len = sizeof(server_addr);

	// Bind socket to port
	int bind_result = lwip_bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
	if (bind_result < 0) {
		log_msg(ERROR, "Cannot bind socket.\n");
		lwip_close(server_socket);
		osThreadTerminate(NULL);
	}

	// Listen for incoming connections
	if (lwip_listen(server_socket, SERVER_QUEUE_SIZE) != 0 ) {
		log_msg(ERROR, "Server listen failed.\n");
		lwip_close(server_socket);
		osThreadTerminate(NULL);
	} else {
		log_msg(DEBUG, "TCP server is listening.\n");
	}

	// Accept incoming connection -> client socket
	while (socket_server_on) {

		// Define client params
		int client_socket;
		struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
	    char buffer[SERVER_BUFF_LEN];
	    int received_bytes;

	    // Let other threads to get some processor time too
	    osDelay(10);

	    // Accept connection
	    client_socket = lwip_accept(server_socket, (struct sockaddr*) &client_addr, (socklen_t*) &client_addr_len);

	    log_msg(DEBUG, "TCP server: connection accepted.\n");

	    // Turn off UDP client
	    udp_client_on = 0;


	    // Change draw button color to green
	    buttons[2].btn_color1 = LCD_COLOR_GREEN;


	    // Keep receiving messages
	    if ((client_socket > 0) && socket_server_on) {
			do {
				draw_command_t command;

				// Receive a command
				memset(buffer, 0, SERVER_BUFF_LEN);
				received_bytes = lwip_recv(client_socket, buffer, SERVER_BUFF_LEN, 0);

				if (received_bytes == 0) {
					osDelay(5);
				} else {

					char tmp[100];
					sprintf(tmp, "r:%d, b0:%d, b1:%d, b2:%d, b3:%d, b4:%d, b5:%d, b6:%d, b7:%d\n", received_bytes, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
					log_msg(DEBUG, tmp);


					// Deserialize buffer into command structure
					deserialize(buffer, &command);



					sprintf(tmp, "t:%d, b:%d, x:%d, y:%d, z:%d\n", command.command_type, command.button_value, command.x, command.y, command.z);
					log_msg(DEBUG, tmp);

					// Interpret command
					if (command.command_type == 0) {

						// Button sent, do action

					} else {

						// Coordinates sent, set position without speed control if dist < 2 cm else with speed control
						// Check if thread is running. If not, launch it. If other thread needed, kill original.


					}

					// After command ran, send acknowledge byte
					uint8_t sent_bytes = lwip_send(client_socket, "0", 1, 0);
					log_msg(DEBUG, "TCP ack sent.\n");
					}

			} while ((received_bytes > 0) && socket_server_on);

		// Close client connection
		lwip_close(client_socket);

		// Change button color to orange
		buttons[2].btn_color1 = LCD_COLOR_ORANGE;

		// Send buffer content to another thread via mail queue
//		mail_t* mail;
//		mail = (mail_t*) osMailAlloc(mail_q_id, osWaitForever);
//		strcpy(mail->text, buffer);
//		osMailPut(mail_q_id, mail);
	    }
	}

	// Close server socket
	while(1) {
		lwip_close(server_socket);
		log_msg(USER, "TCP server socket thread terminating.\n");
		osThreadTerminate(NULL);
	}
}

void deserialize(char* buffer, draw_command_t* command)
{
	command->command_type = buffer[0];
	command->button_value = buffer[1];
	command->x = buffer[2] << 8 | buffer[3];
	command->y = buffer[4] << 8 | buffer[5];
	command->z = buffer[6] << 8 | buffer[7];

	return;
}
