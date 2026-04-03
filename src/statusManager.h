#pragma once

#include <string>

enum class statusLevel {
	Info,
	Warning,
	Error
};

struct statusMessage {
	statusLevel level;
	std::string msg;
};

class statusManager {
private:
	statusMessage status = { statusLevel::Info, "" };
	bool status_updated = false;

public:
	void setStatus(statusLevel level, std::string msg) {
		status_updated = true;
		status = { level, msg };
	}

	void addStatusMessage(std::string msg) {
		status_updated = true;
		status.msg += " " + msg;
	}

	statusMessage getStatus() {
		status_updated = false;
		return status;
	}

	bool isUpdated() { return status_updated; }
};
