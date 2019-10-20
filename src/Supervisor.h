#pragma once

#include <sys/types.h>
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <chrono>
#include <map>

#include "iosource/IOSource.h"
#include "Pipe.h"
#include "Flare.h"

namespace zeek {

class Supervisor : public iosource::IOSource {
public:

	static std::string RunStem(std::unique_ptr<bro::PipePair> pipe);

	struct Config {
		int num_workers = 1;
		std::vector<std::string> pcaps;
		std::string zeek_exe_path;
	};

	struct Node {
		std::string name;
		pid_t pid = 0;
		int exit_status = 0;
		int signal_number = 0;
		int revival_attempts = 0;
		int revival_delay = 1;
		std::chrono::time_point<std::chrono::steady_clock> spawn_time;
	};

	Supervisor(Config cfg, std::unique_ptr<bro::PipePair> stem_pipe, pid_t stem_pid);

	~Supervisor();

	pid_t StemPID() const
		{ return stem_pid; }

	void ObserveChildSignal();

	RecordVal* Status(const std::string& node_name);
	std::string Create(const RecordVal* node);
	bool Destroy(const std::string& node_name);
	bool Restart(const std::string& node_name);

private:

	// IOSource interface overrides:
	void GetFds(iosource::FD_Set* read, iosource::FD_Set* write,
	            iosource::FD_Set* except) override;

	double NextTimestamp(double* local_network_time) override;

	void Process() override;

	void HandleChildSignal();

	void ReapStem();

	const char* Tag() override
		{ return "zeek::Supervisor"; }

	Config config;
	pid_t stem_pid;
	std::unique_ptr<bro::PipePair> stem_pipe;
	bro::Flare signal_flare;
	std::map<std::string, Node> nodes;
	std::string msg_buffer;
};

extern Supervisor* supervisor;

} // namespace zeek
