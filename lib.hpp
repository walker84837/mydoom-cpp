#include <algorithm> // for std::transform, std::replace_if, std::equal
#include <cctype>    // for std::isalpha, std::tolower
#include <ctime>     // for std::strftime
#include <chrono>    // for std::chrono::system_clock
#include <iomanip>   // for std::setw, std::setfill
#include <random>
#include <regex>
#include <windows.h>

std::string rot13(std::string in) {
	std::transform(in.begin(), in.end(), in.begin(), [](char c) {
		if (std::isalpha(c)) {
			char base = std::isupper(c) ? 'A' : 'a';
			return ((c - base + 13) % 26) + base;
		} else {
			return c;
		}
	});
	return in;
}

std::string mk_smtpdate(std::chrono::system_clock::time_point in_tp = std::chrono::system_clock::now())
{
	std::time_t t = std::chrono::system_clock::to_time_t(in_tp);
	std::tm tm_local = std::tm();
	localtime_s(&tm_local, &t);

	char buf[64];
	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %z", &tm_local);
	// Replace the UTC offset with "+/-HHMM" format
	std::string utc_offset = (tm_local.tm_gmtoff >= 0 ? "+" : "-") + std::to_string(abs(tm_local.tm_gmtoff) / 3600) + std::to_string((abs(tm_local.tm_gmtoff) % 3600) / 60);

	return std::string(buf) + " " + utc_offset;
}

std::default_random_engine engine;
std::uniform_int_distribution<WORD> rand16_dist(0, 0xFFFF);
std::uniform_int_distribution<DWORD> rand32_dist(0, 0xFFFFFFFF);

void xrand_init() {
	engine.seed(std::random_device()());
}

WORD xrand16() {
	return rand16_dist(engine);
}

DWORD xrand32() {
	return rand32_dist(engine);
}

std::string::size_type xstrstr(const std::string& str, const std::string& pat) {
	auto pos = str.find(pat);
	return (pos != std::string::npos) ? static_caststd::string::size_type(pos) : 0;
}

std::string::size_type xstrrchr(const std::string& str, char ch) {
	auto pos = str.rfind(ch);
	return (pos != std::string::npos) ? static_caststd::string::size_type(pos) : 0;
}

std::string::iterator xstrchr(std::string& str, char ch) {
	return std::find(str.begin(), str.end(), ch);
}

int xsystem(std::string cmd, bool wait) {
	int result = std::system(cmd.c_str());
	return (wait && result != -1) ? result : 0;
}

struct ci_equal {
	bool operator()(const char& c1, const char& c2) const {
		return std::tolower(c1) == std::tolower(c2);
	}
};

bool is_html_escape(const char& c) {
	return c == '&';
}

std::string html_replace(std::string str) {
	static const std::regex regex_escape("&#[0-9]+;");
	static const std::string hex_prefix("&#x");
	static const std::string hex_suffix(";");

	std::string result;
	std::regex_replace(std::back_inserter(result), str.begin(), str.end(), regex_escape, [&](std::smatch m) {
		std::string match_str = m.str();
		if (match_str.rfind(hex_prefix, 0) == 0 &&
			match_str.find(hex_suffix) == match_str.length() - hex_suffix.length()) {
			// Parse hex escape
			int charcode = std::stoi(match_str.substr(hex_prefix.length(), match_str.length() - 1), nullptr, 16);
			return std::string(1, static_cast<char>(charcode));
		} else {
			// Parse decimal escape
			int charcode = std::stoi(match_str.substr(2, match_str.length() - 3));
			return std::string(1, static_cast<char>(charcode));
		}
	});
	return result;
}

bool xmemcmpi(const char* p, const char* q, int len) {
	return std::equal(p, p + len, q, q + len, ci_equal());
}

int xstrncmp(const char* first, const char* last, size_t count) {
	return std::strncmp(first, last, count);
}