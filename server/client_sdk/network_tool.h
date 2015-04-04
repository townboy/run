#ifndef __NETWORK_TOOL_H__
#define __NETWORK_TOOL_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <arpa/inet.h>
#include <string>
#include "data_def.h"

class NetworkTool {
public:
	static NetworkTool* get_instance() {
		if (s_instance == NULL) {
			s_instance = new NetworkTool();
		}
		return s_instance;
	}

	static int destroy_instance() {
		if (s_instance != NULL) {
			delete s_instance;
		}
		return 0;
	}

	int connect_server() {
	
		struct sockaddr_in server_address;
		if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			printf("create socket error!\n");
			return -1;
		}

		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(8897);
		if(inet_pton(AF_INET, ip_addr.c_str(), &server_address.sin_addr) <= 0) {
			printf("inet_pton error!\n");
			return -1;
		}

		if(connect(socket_fd, (sockaddr *)&server_address, sizeof(server_address)) == -1) {
			printf("connect error!\n");
			return -1;
		
		}
		return 0;
	}

	int disconnect_server() {
		close(socket_fd);
		socket_fd = -1;
		return 0;
	}
	int login(std::string user_name, UserData * user_data);
	
	int get_frame_sync_data(FrameSyncData * sync_data);
	
	int push_frame_uplo_data(FrameUploData * uplo_data);

	int start_game_req(std::string user_name);

	int start_game_resp(std::string *position, UserData *compe_name);

	int push_user_data(UserData * user_data);

	int get_user_data(std::string user_name, UserData *user_data);

private:
	int socket_fd;
	std::string ip_addr;
	std::string ip_port;
	NetworkTool() {
		socket_fd = -1;
		//ip_addr = "192.168.3.221";
		ip_addr = "218.244.157.227";
	}
	~NetworkTool() {
	}

	static NetworkTool* s_instance;

};

#endif
