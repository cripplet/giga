#include <sstream>
#include <string>

#include "libs/exceptionpp/exception.h"

#include "src/client.h"

giga::Client::Client(cachepp::identifier id, const std::shared_ptr<File>& file, std::string mode) : id(id), file(file), is_closed(0) {
	this->l = std::shared_ptr<std::recursive_mutex> (new std::recursive_mutex);
	this->set_mode(mode);
}

void giga::Client::set_mode(std::string mode) {
	this->mode = 0;
	if(mode.find('r') != std::string::npos) {
		this->mode |= giga::Client::read_only;
	}
	if(mode.find('w') != std::string::npos) {
		this->mode |= giga::Client::write_only;
	}
}

std::string giga::Client::get_mode() {
	std::stringstream buf;
	if(this->mode & giga::Client::read_only) {
		buf << 'r';
	}
	if(this->mode & giga::Client::write_only) {
		buf << 'w';
	}
	return(buf.str());
}

cachepp::identifier giga::Client::get_identifier() {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	return(this->id);
}

size_t giga::Client::get_pos() {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	if(this->get_is_closed()) { throw(exceptionpp::InvalidOperation("giga::Client::get_pos", "invalid operation on a closed client")); }
	return(this->file->s(this->shared_from_this(), 0, false));
}

bool giga::Client::get_is_closed() {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	return(this->is_closed);
}

void giga::Client::open() {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	this->set_is_closed(false);
}

void giga::Client::close() {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	this->set_is_closed(true);
}

std::string giga::Client::read(size_t len) {
	std::lock_guard<std::recursive_mutex> l(*this->l);

	if(!(this->mode & giga::Client::read_only)) {
		throw(exceptionpp::InvalidOperation("giga::Client::read", "permission denied"));
	}

	if(this->file) {
		return(this->file->r(this->shared_from_this(), len));
	}

	return("");
}

size_t giga::Client::write(std::string buffer, bool is_insert) {
	std::lock_guard<std::recursive_mutex> l(*this->l);

	if(!(this->mode & giga::Client::write_only)) {
		throw(exceptionpp::InvalidOperation("giga::Client::write", "permission denied"));
	}

	if(this->file) {
		if(is_insert) {
			return(this->file->i(this->shared_from_this(), buffer));
		} else {
			return(this->file->w(this->shared_from_this(), buffer));
		}
	}

	return(0);
}

size_t giga::Client::erase(size_t len) {
	std::lock_guard<std::recursive_mutex> l(*this->l);

	if(!(this->mode & giga::Client::write_only)) {
		throw(exceptionpp::InvalidOperation("giga::Client::erase", "permission denied"));
	}

	if(this->file) {
		return(this->file->d(this->shared_from_this(), len));
	}

	return(0);
}

size_t giga::Client::seek(size_t len, bool is_forward) {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	if(this->file) {
		return(this->file->s(this->shared_from_this(), len, is_forward));
	}
	return(0);
}

void giga::Client::save() {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	if(this->file) {
		return(this->file->save());
	}
	return;
}

void giga::Client::set_is_closed(bool is_closed) {
	std::lock_guard<std::recursive_mutex> l(*this->l);
	if(this->get_is_closed() == is_closed) { return; }
	if(this->file != NULL) {
		if(is_closed) {
			this->file->open(this->shared_from_this());
		} else {
			this->file->close(this->shared_from_this());
		}
	}
	this->is_closed = is_closed;
}

giga::ClientData::ClientData(cachepp::identifier id) : id(id) {}

void giga::ClientData::set_file_offset(size_t file_offset) { this->file_offset = file_offset; }
void giga::ClientData::set_page(std::list<std::shared_ptr<giga::Page>>::iterator page) { this->page = page; }
void giga::ClientData::set_page_offset(size_t page_offset) { this->page_offset = page_offset; }

size_t giga::ClientData::get_file_offset() { return(this->file_offset); }
std::list<std::shared_ptr<giga::Page>>::iterator giga::ClientData::get_page() { return(this->page); }
size_t giga::ClientData::get_page_offset() { return(this->page_offset); }
