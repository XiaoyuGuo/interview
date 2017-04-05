#include "ini_parser.h"
#include <fstream>

namespace qh
{

	INIParser::INIParser()
	{
		std::map<std::string, std::string> key_value;
		dictionary.insert(std::make_pair("default", key_value));
	}

	INIParser::~INIParser()
	{

	}

	/**

	* @brief     Get value from section default with key

	* @param     const std::string& key

	* @param     bool* found

	* @TC        O(log(num(default)))

	* @MC         O(1)

	*/
	const std::string INIParser::Get(const std::string& key, bool* found)
	{
		return Get("default", key, found);
	}

	/**

	* @brief     Get value from section with key

	* @param     const std::string& section

	* @param     const std::string& key

	* @param     bool* found

	* @TC        O(log(num(default)))

	* @MC        O(1)

	*/
	const std::string INIParser::Get(const std::string& section, const std::string& key, bool* found)
	{
		if (found != NULL) *found = false;
		std::map<std::string, std::map<std::string, std::string> >::iterator section_iter;
		std::map<std::string, std::string>::iterator key_value_iter;
		std::map<std::string, std::string> map;
		section_iter = dictionary.find(section);
		if (section_iter != dictionary.end())
		{
			key_value_iter = section_iter->second.find(key);
			if (key_value_iter != section_iter->second.end())
			{
				if (found != NULL) *found = true;
				return key_value_iter->second;
			}
		}
		return "";
	}

	/**

	* @brief     Parse ini_data from file

	* @param     const std::string& in_file_path

	* @param     const std::string& line_seperator

	* @TC        O(n * m)      n = num(lines)   m = len(line)

	* @MC        O(n)          n = sizeof(line)

	* No memory leak

	* Handle closed

	*/
	bool INIParser::Parse(const std::string& in_file_path, const std::string& line_seperator)
	{
		std::ifstream in_file(in_file_path);
		std::string line;

		// Clear the dictionary
		dictionary.clear();
		std::string section = "default";
		std::map<std::string, std::string> key_value;
		dictionary.insert(std::make_pair(section, key_value));

		while (std::getline(in_file, line))
		{
			section = ParseLine(section, line, line_seperator);
		}

		in_file.close();
		return true;
	}

	/**

	* @brief     Parse ini_data in memory

	* @param     const char*  ini_data

	* @param     size_t ini_data_len

	* @param     const std::string& line_seperator = "\n"

	* @param     const std::string& key_value_seperator = "="

	* @TC        O(n ^ 2)   n = len(ini_data)

	* @MC        O(n)       n = sizeof(ini_data)

	* No memory leak

	* Handle closed

	*/
	bool INIParser::Parse(const char*  ini_data,
		                              size_t ini_data_len, 
		                              const std::string& line_seperator = "\n",
		                              const std::string& key_value_seperator = "=")
	{
		// Clear the dictionary
		dictionary.clear();
		std::string ini_data_s(ini_data);
		std::string section = "default";
		std::map<std::string, std::string> key_value;
		dictionary.insert(std::make_pair(section, key_value));

		// ParseLine
		int begin = 0;
		int start = 0;
		begin = ini_data_s.find(line_seperator, begin);
		while (begin != -1)
		{
			std::string a = ini_data_s.substr(start, begin - start);
			section = ParseLine(section, ini_data_s.substr(start, begin - start), key_value_seperator);
			start = begin + line_seperator.size();
			begin = begin + line_seperator.size();
			begin = ini_data_s.find(line_seperator, begin);
		}
		ParseLine(section, ini_data_s.substr(start), key_value_seperator);
		return true;
	}

	/**

	* @brief     Parse one line into dictionary

	* @param     const std::string& section

	* @param     const std::string& line

	* @param     const std::string& key_value_seperator

	* @TC        O(n)  n = len(line)

	* @MC        O(n)  n = len(line)

	* No memory leak

	* Handle closed

	*/
	const std::string INIParser::ParseLine(const std::string& section, const std::string& line, const std::string& key_value_seperator)
	{
		size_t split;
		std::string no_space_line = Trim(line);
		std::string new_section = section;

		if (no_space_line[0] == '[' && no_space_line[no_space_line.size() - 1])
		{
			// Is section
			new_section = no_space_line.substr(1, no_space_line.size() - 2);
			std::map<std::string, std::map<std::string, std::string> >::iterator section_iter;
			std::map<std::string, std::string>::iterator key_value_iter;

			section_iter = dictionary.find(new_section);
			if (section_iter == dictionary.end())
			{
				std::map<std::string, std::string> key_value;
				dictionary.insert(std::make_pair(new_section, key_value));
			}
		}
		else if (no_space_line.length() != 0)
		{
			split = no_space_line.find(key_value_seperator);
			if (static_cast<int>(split) != -1)
			{
				// Is key
				std::string key = no_space_line.substr(0, split);
				std::string value = no_space_line.substr(split + key_value_seperator.size());
				if (key.length() != 0) dictionary.at(section).insert(std::make_pair(key, value));
			}
			
		}

		return new_section;
	}

	/**

	* @brief    Trim input line

	* @param    std::string line

	* @TC       O(n)  n = len(line)

	* @MC       O(1)

	* No memory leak

	* Handle closed

	*/
	const std::string INIParser::Trim(std::string line)
	{
		int begin = 0;
		begin = line.find(" ", begin);
		while (begin != -1)
		{
			line.replace(begin, 1, "");
			begin = line.find(" ", begin);
		}

		begin = 0;
		begin = line.find("\t", begin);
		while (begin != -1)
		{
			line.replace(begin, 1, "");
			begin = line.find("\t", begin);
		}

		return line;
	}

}