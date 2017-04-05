
#include "proxy_url_extractor.h"
#include <fstream>
#include <vector>
#include "tokener.h"

namespace qh
{

	namespace {

		template< class _StringVector,
			class StringType,
			class _DelimType>
			inline void StringSplit(
				const StringType& str,
				const _DelimType& delims,
				unsigned int maxSplits,
				_StringVector& ret)
		{
			unsigned int numSplits = 0;

			// Use STL methods
			size_t start, pos;
			start = 0;

			do
			{
				pos = str.find_first_of(delims, start);

				if (pos == start)
				{
					ret.push_back(StringType());
					start = pos + 1;
				}
				else if (pos == StringType::npos || (maxSplits && numSplits + 1 == maxSplits))
				{
					// Copy the rest of the string
					ret.push_back(StringType());
					*(ret.rbegin()) = StringType(str.data() + start, str.size() - start);
					break;
				}
				else
				{
					// Copy up to delimiter
					//ret.push_back( str.substr( start, pos - start ) );
					ret.push_back(StringType());
					*(ret.rbegin()) = StringType(str.data() + start, pos - start);
					start = pos + 1;
				}

				++numSplits;

			} while (pos != StringType::npos);
		}
	}

	ProxyURLExtractor::ProxyURLExtractor()
	{
	}

	bool ProxyURLExtractor::Initialize(const std::string& param_keys_path)
	{
		std::ifstream ifs;
		ifs.open(param_keys_path.data(), std::fstream::in);
		typedef std::vector<std::string> stringvector;
		stringvector keysvect;

		while (!ifs.eof()) {
			std::string line;
			getline(ifs, line);
			if (ifs.fail() && !ifs.eof()) {
				fprintf(stderr, "SubUrlExtractor::LoadParamKeysFile readfile_error=[%s] error!!", param_keys_path.data());
				ifs.close();
				return false;
			}
			if (line.empty()) continue;

			keysvect.clear();
			StringSplit(line, ",", static_cast<unsigned int>(-1), keysvect);
			assert(keysvect.size() >= 1);
			keys_set_.insert(keysvect.begin(), keysvect.end());
			keys_set_.erase("");
		}

		ifs.close();

		return true;
	}

	std::string ProxyURLExtractor::Extract(const std::string& raw_url)
	{
		std::string sub_url;
		ProxyURLExtractor::Extract(keys_set_, raw_url, sub_url);
		return sub_url;
	}

    /**

    * \brief Extract target value in address

    * \param const KeyItems& keys          Target keys

    * \param const std::string& raw_url    Target address

    * \param std::string& sub_url          Target value. Warning, can be changed and effect outside

    * Time Complexity   : O(log(num(keys)) * num(&) + len(raw_url))

    * Memory Complexity : O(3 * sizeof(raw_url))

    */
	void ProxyURLExtractor::Extract(const KeyItems& keys, const std::string& raw_url, std::string& sub_url)
	{
		Tokener token(raw_url);
		token.skipTo('?');
		token.next(); //skip one char : '?' 
		std::string key;
		std::string kv_pair;
		const char* prev_pos = NULL;
		s32 prev_readable;

		while (!token.isEnd()) {
			kv_pair = token.nextString('&');

			if (kv_pair.length() == 0 && !token.isEnd()) {
				if (prev_pos != token.getCurReadPos()) {
					prev_pos = token.getCurReadPos();
					prev_readable = token.getReadableSize();
				}
				else {
					kv_pair.assign(prev_pos, prev_readable);
					token.reset("", 0);
				}
			}

			int split = kv_pair.find_first_of('=', 0);
			key = kv_pair.substr(0, split);
			if (split != -1 && keys.find(key) != keys.end()) {
				sub_url = kv_pair.substr(split + 1, kv_pair.size() - split);
				break;
			}
		}
	}

	std::string ProxyURLExtractor::Extract(const KeyItems& keys, const std::string& raw_url)
	{
		std::string sub_url;
		ProxyURLExtractor::Extract(keys, raw_url, sub_url);
		return sub_url;
	}
}
