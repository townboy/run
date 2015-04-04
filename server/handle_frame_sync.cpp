#include "handle_frame_sync.h"
#include "data_def.h"
namespace server{
int HandleFrameSync::handle(int t_connfd1, std::string user_name1, int t_connfd2, std::string user_name2, std::queue<int> *task_queue, pthread_mutex_t *task_queue_mutex) {
	
	connfd1 = t_connfd1;
	connfd2 = t_connfd2;
	Json::Value response;
	response["code"] = 200;
	response["status"] = "successful matches";
	response["position"] = "left";
	Json::Value compeData = get_user_data(user_name2);
	response["compeData"] = compeData;
	strcpy(buff, response.toStyledString().c_str());
	int len = strlen(buff);
	LOG_DEBUG << buff << std::endl;
	if (write(connfd1, &len, 4)  <= 0) {
		LOG_ERROR << "write len error!" << std::endl;
		close(connfd1);
		close(connfd2);
		return -1;
	}

	if (write(connfd1, buff, len) <= 0) {
		LOG_ERROR << "write buff error!" << std::endl;
		close(connfd1);
		close(connfd2);
		return -1;
	}

	response["code"] = 200;
	response["status"] = "successful matches";
	response["position"] = "right";
	compeData = get_user_data(user_name1);
	response["compeData"] = compeData;
	strcpy(buff, response.toStyledString().c_str());
	len = strlen(buff);
	
	LOG_DEBUG << buff << std::endl;
	if (write(connfd2, &len, 4) <= 0) {
		LOG_ERROR << "write len error!" << std::endl;
		close(connfd1);
		close(connfd2);
		return -1;
	}

	if (write(connfd2, buff, len) <= 0) {
		LOG_ERROR << "write buff error!" << std::endl;
		close(connfd2);
		close(connfd1);
		return -1;
	}

	int tmp = start_frame_sync();
	if(tmp == 0) {
		pthread_mutex_lock(task_queue_mutex);
		task_queue->push(connfd1);
		task_queue->push(connfd2);
		pthread_mutex_unlock(task_queue_mutex);
	} else if(tmp == -1) {
		LOG_ERROR << "frame sync  error!" << std::endl;
		close(connfd2);
		close(connfd1);
		return -1;
	}
	return 0;
}

Json::Value HandleFrameSync::get_user_data(std::string user_name) {
	if(user_name.length() > 19) {
		LOG_ERROR << "name too long" << std::endl;
		return -1;
	}
	MYSQL *mysql_conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_conn = mysql_init(NULL);
	if (!mysql_real_connect(mysql_conn, "localhost", "root", "123456", "game_database", 0, NULL, 0)) {
		LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
		return -1;
	}

	std::string query_string  = "select * from user where userName = \"" + user_name + "\";";
	LOG_DEBUG << query_string << std::endl;
	if (mysql_query(mysql_conn, query_string.c_str())) {	
		LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
		return -1;
	}

	res = mysql_use_result(mysql_conn);
	row = mysql_fetch_row(res);
	
	Json::Value content;	
	content["userName"] = user_name;
	content["potency"] = atoi(row[1]);

	std::string play_id[4];
	play_id[0] = row[2];
	play_id[1] = row[3];
	play_id[2] = row[4];
	play_id[3] = row[5];
	mysql_free_result(res);

	for (int i = 0; i < 4; i++) {
		query_string = "select * from footballer where playId = " + play_id[i] + ";";
		LOG_DEBUG << query_string << std::endl;
		if (mysql_query(mysql_conn, query_string.c_str())) {	
			LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
			return -1;
		}

		MYSQL_RES *footballer_res = mysql_use_result(mysql_conn);
		MYSQL_ROW footballer_row = mysql_fetch_row(footballer_res);
		std::string strength = footballer_row[1];
		std::string speed = footballer_row[2];
		std::string accuracy = footballer_row[3];
		mysql_free_result(footballer_res);

		Json::Value player;
		player["strength"] = atoi(strength.c_str());
		player["speed"] = atoi(speed.c_str());
		player["accuracy"] = atoi(accuracy.c_str());

		std::string player_name;
		if(i ==0) {
			player_name = "player1";
		} else if (i == 1) {
			player_name = "player2";
		} else if (i == 2) {
			player_name = "player3";
		} else {
			player_name = "player4";
		}
		content[player_name] = player;
	}
		
	mysql_close(mysql_conn);
	return content;
}
int HandleFrameSync::start_frame_sync() {	
	FrameSyncData sync_data;
	sync_data.status = 'r';
	for(int i = 0; i < 8; i	++) {
		sync_data.control[i] = 8;
	}
	sync_data.a_shoot = sync_data.a_pass = 0;
	sync_data.b_shoot = sync_data.b_pass = 0;
	sync_data.a_tar_x = sync_data.a_tar_y = 0;
	sync_data.b_tar_x = sync_data.b_tar_y = 0;
	while(1) {
		sync_data.to_binary();
		if (write(connfd1, sync_data.buff, 8) <= 0) {
			LOG_ERROR << "write control error!" << std::endl;
			return -1;
		}

		for (int i = 0; i < 4; i ++) {
			int tmp = sync_data.control[i];
			sync_data.control[i] = sync_data.control[i + 4];
			sync_data.control[i + 4] = tmp;
		}
		int tmp;
		tmp = sync_data.a_shoot;
		sync_data.a_shoot = sync_data.b_shoot;
		sync_data.b_shoot = tmp;

		tmp = sync_data.b_pass;
		sync_data.b_pass = sync_data.a_pass;
		sync_data.a_pass = tmp;

		tmp = sync_data.a_tar_x;
		sync_data.a_tar_x = sync_data.b_tar_x;
		sync_data.b_tar_x = tmp;

		tmp = sync_data.a_tar_y;
		sync_data.a_tar_y = sync_data.b_tar_y;
		sync_data.b_tar_y = tmp;

		sync_data.to_binary();
		if (write(connfd2, sync_data.buff, 8) <= 0) {
			LOG_ERROR << "write control error!" << std::endl;
			return -1;
		}

		if (sync_data.status == 'e') {
			return 0;
		}
		
		FrameUploData uplo_data;

		int n;
		n = read(connfd1, uplo_data.buff, 8);
		if (n <= 0) {
			LOG_ERROR << "read control error" << std::endl;
			return -1;
		}
		uplo_data.to_real();

		if (sync_data.status == 'r') {
			sync_data.status = uplo_data.status;
		}
		for(int i = 0; i < 4; i++) {
			sync_data.control[i] = uplo_data.control[i];
		}
		sync_data.a_shoot = uplo_data.a_shoot;
		sync_data.a_pass = uplo_data.a_pass;
		sync_data.a_tar_x = uplo_data.tar_x;
		sync_data.a_tar_y = uplo_data.tar_y;

		n = read(connfd2, uplo_data.buff, 8);
		if (n <= 0) {
			LOG_ERROR << "read control error" << std::endl;
			return -1;
		}

		uplo_data.to_real();
		if (sync_data.status == 'r') {
			sync_data.status = uplo_data.status;
		}

		for(int i = 0; i < 4; i++) {
			sync_data.control[i + 4] = uplo_data.control[i];
		}
		sync_data.b_shoot = uplo_data.a_shoot;
		sync_data.b_pass = uplo_data.a_pass;
		sync_data.b_tar_x = uplo_data.tar_x;
		sync_data.b_tar_y = uplo_data.tar_y;
		
	}
	return 0;
}
}
