# 25-NUEDC

// 非[]字符
		if(point_rx_data_buffer[point_rx_data_index] != '[' && point_rx_data_buffer[point_rx_data_index] != ']' && point_rx_data_index != 0)
			point_rx_data_index++;	// 索引递增
		// 检测第一个坐标字符串开始（[）
        if (point_rx_data_buffer[point_rx_data_index] == '[' && point_rx_data_index == 0){
			point_rx_data_index++;	// 索引递增
		}
		// 检测第二个坐标字符串开始（[）
		else if (point_rx_data_buffer[point_rx_data_index] == '[' && point_rx_data_index != 0){
			// 前有 ，
			if(point_rx_data_buffer[point_rx_data_index-1] == ','){
				point_rx_data_index++;	// 索引递增
				point_index = 1;
			}
			// 前无 ，
			else if(point_rx_data_buffer[point_rx_data_index-1] != ','){
				// 重置
				point_index = 0;
				point_rx_data_index = 0;
				point_rx_data_buffer[point_rx_data_index] = '[';
				point_rx_data_index++;
			}
		}
		// 检测第一个坐标字符串结束（]）
		if (point_rx_data_buffer[point_rx_data_index] == ']' && point_rx_data_index != 0 && point_index == 0){
			point_rx_data_index++;	// 索引递增
		}
		// 检测第二个坐标字符串结束（]）
		else if (point_rx_data_buffer[point_rx_data_index] == ']' && point_rx_data_index != 0 && point_index == 1){
//			HAL_UART_Transmit(&huart1, point_rx_data_buffer, sizeof(point_rx_data_buffer), 10);	// 串口回显
			// 解析
			sscanf(point_rx_data_buffer, "[%d,%d],[%d,%d]", &pointRT[0], &pointRT[1], &pointTAR[0], &pointTAR[1]);
			
			// 重置
			point_index = 0;
			point_rx_data_index = 0;
			memset(point_rx_data_buffer, 0, sizeof(point_rx_data_buffer));
		}