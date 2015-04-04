#include "network_tool.h"
#include <sstream>

NetworkTool * NetworkTool::s_instance = NULL;

int NetworkTool::login(std::string user_name, UserData * user_data) {
	Json::Value value;
	Json::Reader reader;
	char buff[5000];
	if (socket_fd == -1) {
		printf("please connect server!\n");
		return -1;
	}
	
	if (user_data == NULL) {
		printf("user_data point is NULL");
		disconnect_server();
		return -1;
	}

	if(user_name.length() > 19) {
		disconnect_server();
		return -1;
	}
	
	std::string str;
	str = "{\"type\":\"login\" , \"userName\":\"" + user_name + "\"}";
	int len = str.length();	
	if(write(socket_fd, &len, 4) == -1) {
		printf("write len error!\n");
		disconnect_server();
		return -1;
	}
	if(write(socket_fd, str.c_str(), len) == -1) {
		printf("write buff error!\n");
		disconnect_server();
		return -1;
	}

	int n = read(socket_fd, &len, 4);
	if(n <= 0) {
		disconnect_server();
		return -1;
	}
	n = read(socket_fd, buff, len);
	if(n <= 0) {
		disconnect_server();
		return -1;
	}

	if (!reader.parse(buff, value)) {
		printf("parse json error!\n");
		disconnect_server();
		return -1;
	}

	user_data->user_name = value["content"]["userName"].asString();
	user_data->potency = value["content"]["potency"].asInt();
	user_data->player[0].strength = value["content"]["player1"]["strength"].asInt();
	user_data->player[0].speed = value["content"]["player1"]["speed"].asInt();
	user_data->player[0].accuracy = value["content"]["player1"]["accuracy"].asInt();
	user_data->player[1].strength = value["content"]["player2"]["strength"].asInt();
	user_data->player[1].speed = value["content"]["player2"]["speed"].asInt();
	user_data->player[1].accuracy = value["content"]["player2"]["accuracy"].asInt();
	
	user_data->player[2].strength = value["content"]["player3"]["strength"].asInt();
	user_data->player[2].speed = value["content"]["player3"]["speed"].asInt();
	user_data->player[2].accuracy = value["content"]["player3"]["accuracy"].asInt();

	user_data->player[3].strength = value["content"]["player4"]["strength"].asInt();
	user_data->player[3].speed = value["content"]["player4"]["speed"].asInt();
	user_data->player[3].accuracy = value["content"]["player4"]["accuracy"].asInt();

	return 0;
}

int NetworkTool::start_game_req(std::string user_name) {
	Json::Value value;
	Json::Reader reader;
	value["type"] = "start_game";
	value["userName"] = user_name;
	char buff[5000];
	strncpy(buff, value.toStyledString().c_str(), 4900);
	int len = strlen(buff);
	if(write(socket_fd, &len, 4) == -1) {
		printf("write len error!\n");
		disconnect_server();
		return -1;
	}
	if(write(socket_fd, buff, len) == -1) {
		printf("write buff error!\n");
		disconnect_server();
		return -1;
	}
	return 0;
}

int NetworkTool::start_game_resp(std::string *position, UserData* user_data) {
	char buff[5000];
	Json::Value value;
	Json::Reader reader;
	int len;
	int ret = read(socket_fd, &len, 4);
	if(ret <= 0) {	
		printf("write buff error!\n");
		disconnect_server();
	}
	ret = read(socket_fd, buff, len);
	if(ret <= 0) {	
		printf("write buff error!\n");
		disconnect_server();
	}
	
	if (!reader.parse(buff, value)) {
		printf("parse json error!\n");
		disconnect_server();
		return -1;
	}
	
	if (value["code"].asInt() == 200 && value["status"].asString() == "successful matches") {
		*position = value["position"].asString();
		user_data->user_name = value["compeData"]["userName"].asString();
		user_data->potency = value["compeData"]["potency"].asInt();
		user_data->player[0].strength = value["compeData"]["player1"]["strength"].asInt();
		user_data->player[0].speed = value["compeData"]["player1"]["speed"].asInt();
		user_data->player[0].accuracy = value["compeData"]["player1"]["accuracy"].asInt();

		user_data->player[1].strength = value["compeData"]["player2"]["strength"].asInt();
		user_data->player[1].speed = value["compeData"]["player2"]["speed"].asInt();
		user_data->player[1].accuracy = value["compeData"]["player2"]["accuracy"].asInt();
	
		user_data->player[2].strength = value["compeData"]["player3"]["strength"].asInt();
		user_data->player[2].speed = value["compeData"]["player3"]["speed"].asInt();
		user_data->player[2].accuracy = value["compeData"]["player3"]["accuracy"].asInt();

		user_data->player[3].strength = value["compeData"]["player4"]["strength"].asInt();
		user_data->player[3].speed = value["compeData"]["player4"]["speed"].asInt();
		user_data->player[3].accuracy = value["compeData"]["player4"]["accuracy"].asInt();

		return 0;
	} else {
		printf("error!");
		disconnect_server();
		return -1;
	}
}

int NetworkTool::get_frame_sync_data(FrameSyncData *sync_data) {
	Json::Value value;
	Json::Reader reader;
	int buff[2];
	int len = read(socket_fd, (char *)sync_data->buff, 8); 
	if(len <= 0) {
		printf("read control error!\n");
		disconnect_server();
		return -1;
	}
	sync_data->to_real();
	return 0;
}

int NetworkTool::push_frame_uplo_data(FrameUploData * uplo_data) {
	Json::Value value;
	Json::Reader reader;
	
	uplo_data->to_binary();
	if(write(socket_fd, (char *)uplo_data->buff, 8) <= 0) {
		printf("write control error!\n");
		disconnect_server();
		return -1;
	}

	return 0;
}

int NetworkTool::get_user_data(std::string user_name, UserData * user_data) {
	Json::Value value;
	if (user_data == NULL) {
		printf("user_data point is NULL");
		disconnect_server();
		return -1;
	}
	value["type"] = "get_user_data";
	value["userName"] = user_name;
	char buff[5000];
	strncpy(buff, value.toStyledString().c_str(), 4900);
	int len = strlen(buff);
	if(write(socket_fd, &len, 4) <= 0) {
		printf("write len error!\n");
		disconnect_server();
		return -1;
	}
	if(write(socket_fd, buff, len) <= 0) {
		printf("write buff error!\n");
		return -1;
	}
	int n = read(socket_fd, &len, 4);
	if(n <= 0) {
		printf("read len error!\n");
		return -1;
	}
	n = read(socket_fd, buff, len);
	if(n <= 0) {
		printf("read buff error!\n");
		return -1;
	}
	Json::Reader reader;
	if (!reader.parse(buff, value)) {
		printf("parse json error!\n");
		return -1;
	}
	user_data->user_name = value["content"]["userName"].asString();
	user_data->potency = value["content"]["potency"].asInt();
	user_data->player[0].strength = value["content"]["player1"]["strength"].asInt();
	user_data->player[0].speed = value["content"]["player1"]["speed"].asInt();
	user_data->player[0].accuracy = value["content"]["player1"]["accuracy"].asInt();

	user_data->player[1].strength = value["content"]["player2"]["strength"].asInt();
	user_data->player[1].speed = value["content"]["player2"]["speed"].asInt();
	user_data->player[1].accuracy = value["content"]["player2"]["accuracy"].asInt();
	
	user_data->player[2].strength = value["content"]["player3"]["strength"].asInt();
	user_data->player[2].speed = value["content"]["player3"]["speed"].asInt();
	user_data->player[2].accuracy = value["content"]["player3"]["accuracy"].asInt();

	user_data->player[3].strength = value["content"]["player4"]["strength"].asInt();
	user_data->player[3].speed = value["content"]["player4"]["speed"].asInt();
	user_data->player[3].accuracy = value["content"]["player4"]["accuracy"].asInt();
	return 0;
}
int NetworkTool::push_user_data(UserData * user_data) {
	Json::Value value;
	value["type"] = "push_user_data";
	value["userName"] = user_data->user_name;
	value["potency"] = user_data->potency;

	Json::Value content;

	for(int i = 0; i < 4; i++) {
		Json::Value player;
		player["strength"] = user_data->player[i].strength;
		player["speed"] = user_data->player[i].speed;
		player["accuracy"] = user_data->player[i].accuracy;
		std::stringstream ss;
		ss << (i+1);
		std::string name;
		ss >> name;
		name = "player" +  name;
		content[name] = player;
	}
	value["content"] = content;	
	char buff[5000];
	strncpy(buff, value.toStyledString().c_str(), 4900);
	int len = strlen(buff);
	if(write(socket_fd, &len, 4) <= 0) {
		printf("write len error!\n");
		disconnect_server();
		return -1;
	}
	if(write(socket_fd, buff, len) <= 0) {
		printf("write buff error!\n");
		disconnect_server();
		return -1;
	}

	
	int ret = read(socket_fd, &len, 4);
	if(ret <= 0) {
		disconnect_server();
		return -1;
	}
	ret = read(socket_fd, buff, len);
	if(ret <= 0) {
		disconnect_server();
		return -1;
	}
	Json::Reader reader;
	if (!reader.parse(buff, value)) {
		printf("parse json error!\n");
		disconnect_server();
		return -1;
	}
	if (value["code"] == 200 && value["status"] == "OK") {
		return 0;
	} else {
		disconnect_server();
		return -1;
	}
}
