#ifndef MAIL_DIR_LOGGER
#define MAIL_DIR_LOGGER

#include <iostream>

class MailDirLogger {
public:

	static std::string logger_path; /**< Path to logger file where disk mail oves are soterd*/

	/**
	 * Read file defined in logger_path and revert changes with mail position on file system.
	 */
	static void reset();

	/**
	 * Add log into file defined by logger_path.
	 * @param source From where is mail moved.
	 * @param destination To where is mail moved.
	 */
	static void add_log(std::string source, std::string destination);

};
#endif //MAIL_DIR_LOGGER