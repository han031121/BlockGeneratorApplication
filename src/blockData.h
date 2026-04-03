#pragma once

#include "statusManager.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_set>
#include <random>
#include <cmath>
#include <numbers>
#include <string>
#include <sstream>
#include <iomanip>

#define MAX_SIZE 13
#define EPSILON 1e-12
#define DEFAULT_WEIGHT 100.0
#define DENSITY_COEFF 0.02
#define FAIL_COUNT 3000

typedef std::tuple<int,int,int> Tuple;
typedef std::pair<int, int> Pair;

using std::get;

class blockData {

private:
    //condition
    Pair block_count_pair; //max : max_r * max_c * max_h
    int block_count; //current block_count : randomly decided in block_count_pair
    int max_r, max_c, max_h; //max : MAX_SIZE
    double density_var = 0;
	Pair start_point;
	bool allow_duplicate;

    //status
    int biggest_r = 0, biggest_c = 0, biggest_h = 0;
    int smallest_r = MAX_SIZE, smallest_c = MAX_SIZE;
    int size_r = 0, size_c = 0, size_h = 0;
	std::unordered_set<std::string> created_list;
	bool is_generated = false;
        
    //weight
    double weight_field[MAX_SIZE][MAX_SIZE][MAX_SIZE+1] = {0};
        
    //data
    bool cubic_data[MAX_SIZE][MAX_SIZE][MAX_SIZE+1] = {0};
    int height_data[MAX_SIZE][MAX_SIZE] = {0};

	//block generate
	void makeBlock();
	void setStartPoint();

    //setting weight
    bool checkCreatable(int r, int c, int h); //check invisibility and max size limit
    bool checkObscure(int r, int c, int h);
    double getWeight(int r, int c, int h); //get modified weight
    void setWeight(); //calculate initial weight

    //set status
    void init(); //initialize data
    void measureSize(Tuple t); //measure current size

public:
	statusManager status;

    blockData(int _bc1, int _bc2, int _max_r, int _max_c, int _max_h, double _den = 25, bool _dup = 0)
        : max_r(_max_r), max_c(_max_c), max_h(_max_h), density_var(_den), allow_duplicate(_dup) {
            block_count_pair = {std::min(_bc1, _bc2), std::max(_bc1, _bc2)};
                
			if (block_count_pair.first > max_r * max_c * max_h || block_count_pair.second > max_r * max_c * max_h || block_count_pair.first < 1 || block_count_pair.second < 1) {
				status.setStatus(statusLevel::Warning, "[ blockData ] : Maximum block count is invalid. The value has been adjusted");

				if (block_count_pair.first > max_r * max_c * max_h)
					block_count_pair.first = max_r * max_c * max_h;
				if (block_count_pair.second > max_r * max_c * max_h)
					block_count_pair.second = max_r * max_c * max_h;
				if (block_count_pair.first < 1)
					block_count_pair.first = 1;
				if (block_count_pair.second < 1)
					block_count_pair.second = 1;
			}
			if (_max_r > MAX_SIZE || _max_c > MAX_SIZE || _max_h > MAX_SIZE || _max_r < 1 || _max_c < 1 || _max_h < 1) {
				status.setStatus(statusLevel::Error, "[ blockData ] : Max size value is invalid. Please initialize block data.");
				max_r = 1; max_c = 1; max_h = 1;
			}

			status.setStatus(statusLevel::Info, "[ blockData ] : BlockData generated.");
            setWeight();
        }

    void generateBlock(); //make new Block data

    bool getData(int r, int c, int h) const { return cubic_data[r][c][h]; }
	int getHeightData(int r, int c) const { return height_data[r][c]; }
	int getMaxRow() const { return max_r; }
	int getMaxCol() const { return max_c; }
	int getMaxHeight() const { return max_h; }
	int getSizeRow() const { return size_r; }
	int getSizeCol() const { return size_c; }
	int getSizeHeight() const { return size_h; }
	std::tuple<float, float, float> const getCenter() {
		return std::make_tuple(
			(float)(biggest_r + smallest_r) / 2,
			(float)(biggest_c + smallest_c) / 2,
			(float)(biggest_h - 1) / 2);
	}
	int getBlockCount() const { return block_count; }
	bool isGenerated() const { return is_generated; }

    //utility
	std::string getIdentify(); //get identity of current block pattern
};
