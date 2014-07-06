#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>
#include <string>

namespace giga {
	class NotImplemented : public std::exception {
		public:
			NotImplemented(std::string func_name);
			virtual const char *what() const throw();
		protected:
			std::string func_name;
	};

	/**
	 * general error class
	 */
	class InvalidOperation : public std::exception {
		public:
			InvalidOperation(std::string func_name, std::string msg);
			virtual const char *what() const throw();
		protected:
			std::string func_name;
			std::string msg;
	};

	class RuntimeError : public std::exception {
		public:
			RuntimeError(std::string func_name, std::string msg);
			virtual const char *what() const throw();
		protected:
			std::string func_name;
			std::string msg;
	};

	class FileNotFound : public std::exception {
		public:
			FileNotFound(std::string filename);
			virtual const char *what() const throw();
		protected:
			std::string filename;
	};

	class InvalidFileOperation : public std::exception {
		public:
			InvalidFileOperation(std::string filename, std::string mode, int error_type);
			virtual const char *what() const throw();
		protected:
			std::string filename;
			std::string mode;
			int error_type;
	};
}

#endif