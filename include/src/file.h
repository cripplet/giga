#ifndef _FILE_H
#define _FILE_H

#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "src/block.h"
#include "src/client.h"
#include "src/config.h"
#include "src/global.h"
#include "src/info.h"

/**
 * File::File is the file abstraction for the library
 */
namespace giga {
	class Client;
	class Block;
	class BlockInfo;
	class File : public std::enable_shared_from_this<File> {
		public:
			/**
			 * these functions MAY be called by the user
			 *
			 * all API function calls here are guaranteed to be atomic
			 */
			File(std::string filename, std::string mode);
			File(std::string filename, std::string mode, const std::shared_ptr<Config>& config);
			std::shared_ptr<Client> open();
			void close(const std::shared_ptr<Client>& client);
			int get_n_clients();

			/**
			 * these functions MUST NOT be called by the user, and are called internally by the client wrapper functions,
			 *	or internally
			 */
			~File();
			std::shared_ptr<Client> get_client_list();
			giga_size get_client_pos(const std::shared_ptr<Client>& client);
			void seek(const std::shared_ptr<Client>& client, giga_size offset);
			giga_size read(const std::shared_ptr<Client>& client, const std::shared_ptr<std::string>& buffer, giga_size n_bytes);
			giga_size write(const std::shared_ptr<Client>& client, const std::shared_ptr<std::string>& buffer, bool is_insert);
			giga_size erase(const std::shared_ptr<Client>& client, size_t len);
			void save(const std::shared_ptr<Client>& client);

		private:
			std::atomic<int> n_clients;

			// unique ID tracker
			std::atomic<int> n_opens;

			size_t max_block_size;

			std::string filename;
			std::string mode;

			// linked list of open clients
			std::shared_ptr<Client> head_client;
			std::mutex client_list_lock;

			// data is represented by a linked list -- gives location of the first block of data
			std::shared_ptr<Block> head_block;

			void pause();
			void unpause();

			void allocate(const std::shared_ptr<Block>& block);

			// list of blocks whose data is loaded
			std::map<giga_size, std::shared_ptr<BlockInfo>> cache;
			std::mutex cache_lock;
			// one lock per cache entry -- static array of size n_cache_entries
			std::vector<std::shared_ptr<std::mutex>> cache_entry_locks;
			size_t n_cache_entries;

			void acquire_block(const std::shared_ptr<Client>& client, giga_size n_bytes);
	};

}

#endif
