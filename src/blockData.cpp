#include "blockData.h"

//generateBlock
std::random_device rd;
std::mt19937 mt(rd());

void blockData::generateBlock() {
	int count = 0;

	if (!allow_duplicate) {
		while (count++ <= std::max(FAIL_COUNT, (int)created_list.size())) {
			init();
			makeBlock();

			std::string key = getIdentify();

			if (created_list.find(key) == created_list.end()) {
				created_list.insert(key);
				break;
			}
		}
	}
	else {
		init();
		makeBlock();
	}

	if (count > FAIL_COUNT) {
		init();
		is_generated = false;
		status.setStatus(statusLevel::Error, "[ blockData ] No more unique block configurations can be generated. Please generate new block data object.");
	}
	else if (count > 1) {
		status.addStatusMessage("(duplication : " + std::to_string(count) + ")");
	}
}

void blockData::makeBlock() {
    int dr[4] = { -1, 0, 1, 0 };
	int dc[4] = { 0, 1, 0, -1 };
    int cur_count = 0;

    std::uniform_int_distribution<int> dis(block_count_pair.first, block_count_pair.second);
    block_count = dis(mt);

	setStartPoint();
	int start_r = start_point.first;
	int start_c = start_point.second;
	cubic_data[start_r][start_c][1] = 1;
    height_data[start_r][start_c] = 1;

    std::vector<Tuple> created;
    
    cur_count++;
    created.push_back(std::make_tuple(start_r,start_c,1));
	measureSize(std::make_tuple(start_r, start_c, 1));

    while(cur_count < block_count) {
        double weight_sum = 0;
        std::vector<Tuple> adj;
        bool selected[MAX_SIZE][MAX_SIZE][MAX_SIZE] = {0};
        std::vector<std::pair<Tuple,double>> weight_list;

        for(Tuple cur : created) {
            for(int i=0; i<5; i++) {
                Tuple next;
                if(i < 4)
                    next = std::make_tuple(get<0>(cur) + dr[i], get<1>(cur) + dc[i], get<2>(cur));
                else if(i == 4)
                    next = std::make_tuple(get<0>(cur), get<1>(cur), get<2>(cur) + 1);

				if(!checkCreatable(get<0>(next), get<1>(next), get<2>(next)))
					continue;
                if(cubic_data[get<0>(next)][get<1>(next)][get<2>(next)])
                    continue;
                if(selected[get<0>(next)][get<1>(next)][get<2>(next)])
                    continue;
                adj.push_back(next);
                selected[get<0>(next)][get<1>(next)][get<2>(next)] = 1;
            }
        }

        for(Tuple t : adj) {
            weight_sum += getWeight(get<0>(t), get<1>(t), get<2>(t));
            weight_list.push_back({t, weight_sum});
        }

        std::uniform_real_distribution<double> dis_weight(0, weight_sum);
        double cur_weight = dis_weight(mt);

        if(weight_list.empty() || weight_sum <= EPSILON) {
			status.setStatus(statusLevel::Warning, "[ blockData ] Cannot generate block anymore. Block generated");
			is_generated = true;
            return;
        }

        for(std::pair<Tuple,double> p : weight_list) {
			if (cur_weight > p.second)
				continue;

            //block creation
            Tuple cur = p.first;

            created.push_back(cur);
            cur_count++;

            cubic_data[get<0>(cur)][get<1>(cur)][get<2>(cur)] = 1;
            height_data[get<0>(cur)][get<1>(cur)] = std::max(height_data[get<0>(cur)][get<1>(cur)], get<2>(cur));
            measureSize(cur);
            break;
        }
    }

	is_generated = true;
	status.setStatus(statusLevel::Info, "[ blockData ] Block generated.");
}

void blockData::setStartPoint() {
	std::vector<std::pair<Pair, double>> weight_list;
	double weight_sum = 0;

	for (int i = 0; i < max_r; i++) {	
		weight_sum += weight_field[i][0][1];
		weight_list.push_back({ { i, 0 }, weight_sum });
	}
	for (int j = 1; j < max_c; j++) {
		weight_sum += weight_field[0][j][1];
		weight_list.push_back({ { 0, j }, weight_sum });
	}

	std::uniform_real_distribution<double> dis_weight(0, weight_sum);
	double rand_weight = dis_weight(mt);

	for (std::pair<Pair, double> p : weight_list) {
		if (rand_weight > p.second)
			continue;
		start_point = p.first;
		break;
	}
}

//setWeight
bool blockData::checkCreatable(int r, int c, int h) {
    if(r >= max_r || r < 0)
        return false;
    if(c >= max_c || c < 0)
        return false;
    if(h > max_h)
        return false;
    
    if(h > 1 && cubic_data[r][c][h-1] == false)
        return false;

    if(!checkObscure(r, c, h))
        return false;
    
    return true;
}

bool blockData::checkObscure(int r, int c, int h) {
    int check_r, check_c, check_h;

    if(r-1 >= 0 && c+1 < max_c) {
        check_r = r-1;
        check_c = c;
        check_h = height_data[check_r][check_c];

        if(h > check_h && (height_data[r-1][c+1] > check_h || height_data[r][c+1] > check_h))
            return false;
    }
    if(r-1 >= 0 && c-1 >= 0) {
        check_r = r-1;
        check_c = c-1;
        check_h = height_data[check_r][check_c];

        if(h > check_h && (height_data[r-1][c] > check_h || height_data[r][c-1] > check_h))
            return false;
    }
    if(r+1 < max_r && c-1 >= 0) {
        check_r = r;
        check_c = c-1;
        check_h = height_data[check_r][check_c];

        if(h > check_h && (height_data[r+1][c] > check_h || height_data[r+1][c-1] > check_h))
            return false;
    }

    return true;
}

double blockData::getWeight(int r, int c, int h) {
    if(!checkCreatable(r,c,h))
        return 0;
    double mul = 1.0;
    double possibility = weight_field[r][c][h];

    possibility *= mul;
    return possibility;
}

void blockData::setWeight() {
    for(int r = 0; r < max_r; r++) {
        for(int c = 0; c < max_c; c++) {
            double dist = r + c;
            for(int h=0; h <= max_h; h++)
                weight_field[r][c][h] = DEFAULT_WEIGHT * exp(-DENSITY_COEFF * dist * density_var);
        }
    }
}

//setStatus
void blockData::init() {
    std::fill(&cubic_data[0][0][0], &cubic_data[0][0][0] + MAX_SIZE*MAX_SIZE*(MAX_SIZE + 1), 0);
    std::fill(&height_data[0][0], &height_data[0][0] + MAX_SIZE*MAX_SIZE, 0);

    biggest_r = 0;
    biggest_c = 0;
    biggest_h = 0;
    smallest_r = MAX_SIZE;
    smallest_c = MAX_SIZE;
    size_r = 0;
    size_c = 0;
    size_h = 0;
}

void blockData::measureSize(Tuple added) {
    biggest_r = std::max(get<0>(added), biggest_r);
    biggest_c = std::max(get<1>(added), biggest_c);
    biggest_h = std::max(get<2>(added), biggest_h);
    smallest_r = std::min(get<0>(added), smallest_r);
    smallest_c = std::min(get<1>(added), smallest_c);

    size_r = biggest_r - smallest_r + 1;
    size_c = biggest_c - smallest_c + 1;
    size_h = biggest_h;
}

std::string blockData::getIdentify() {
	std::string s = "";
	char hex[] = "0123456789ABCDEF";

	s += hex[size_r];
	s += hex[size_c];
	s += '_';

	for (int i = smallest_r; i <= biggest_r; i++) {
		for (int j = smallest_c; j <= biggest_c; j++) {
			char c = hex[height_data[i][j]];
			s += c;
		}
	}
	return s;
}
