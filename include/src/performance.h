#ifndef _GIGA_PERFORMANCE_H
#define _GIGA_PERFORMANCE_H

#include <atomic>
#include <deque>
#include <memory>

#include "src/client.h"
#include "src/file.h"

namespace giga {
	class Result {
		public:
			Result();
			void push_back(std::string tag, size_t n_transactions, double total_runtime, size_t total_data, size_t read, size_t write, size_t insert, size_t erase, size_t file_size, size_t cache_size, size_t init_size, size_t max_size, double miss_rate, size_t n_clients);

			size_t get_size();

			std::string get_tag(size_t index);
			double get_latency(size_t index);
			double get_throughput(size_t index);
			size_t get_file_size(size_t index);
			double get_data_size(size_t index);
			size_t get_cache_size(size_t index);
			size_t get_init_size(size_t index);
			size_t get_max_size(size_t index);
			size_t get_n_clients(size_t index);
			double get_read(size_t index);
			double get_write(size_t index);
			double get_insert(size_t index);
			double get_erase(size_t index);
			double get_miss_rate(size_t index);

			std::string pop_front(bool is_tsv, bool include_header);
			std::string to_string(bool is_tsv = false);
			static bool is_dup(char l, char r);

		private:
			size_t size;
			size_t i_count;

			std::deque<std::string> tag;
			std::deque<size_t> index;
			std::deque<size_t> n_transactions;
			std::deque<double> total_runtime;
			std::deque<size_t> total_data;
			std::deque<size_t> file_size;
			std::deque<size_t> cache_size;
			std::deque<size_t> init_size;
			std::deque<size_t> max_size;
			std::deque<size_t> n_clients;
			std::deque<size_t> read;
			std::deque<size_t> write;
			std::deque<size_t> insert;
			std::deque<size_t> erase;
			std::deque<double> miss_rate;
	};

	class Performance {
		public:
			Performance();

			void set_file(std::shared_ptr<giga::File> file);

			void run(std::string tag, std::vector<size_t> access_pattern, std::vector<uint8_t> type, std::vector<size_t> data_size, size_t n_clients, size_t n_attempts);
			std::shared_ptr<Result> get_result();

			const static uint8_t R = 0;
			const static uint8_t W = 1;
			const static uint8_t I = 2;
			const static uint8_t E = 3;

			static size_t parse_line(char *line);
			static size_t get_usage();

		private:
			std::weak_ptr<File> file;
			std::shared_ptr<Result> result;

			void aux_run(const std::shared_ptr<std::atomic<double>>& runtime, const std::shared_ptr<std::atomic<size_t>>& data, const std::shared_ptr<Client>& client, std::vector<size_t> access_pattern, std::vector<uint8_t> type, std::vector<size_t> data_size);
	};
}

std::ostream& operator<< (std::ostream& os, giga::Result& obj);

#endif
