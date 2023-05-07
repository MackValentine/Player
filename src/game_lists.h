#include <vector>
#include <compiler.h>
#include <lcf/data.h>
#include "string_view.h"
#include <cstdint>
#include <string>

#include "output.h"

#ifndef EP_GAME_STRINGVAR_H
#define EP_GAME_STRINGVAR_H
class Game_Lists
{
public:
	using ListVar_t = std::vector< std::vector<int>>;
	using StringVar_t2 = std::vector<lcf::DBString>;

	static constexpr int kMaxWarnings = 10;

	Game_Lists();

	void SetData(ListVar_t s);

	void SetSaveData(StringVar_t2 s);

	const ListVar_t& GetData() const;

	std::vector<int> Get(int list_id) const;

	std::vector<int> Set(int list_id, std::vector<int> value);

	int Add(int list_id, int value);

	int GetElt(int list_id, int value);

	void Delete(int list_id, int elementID);

	void DeleteElt(int list_id, int elt);

	void Clear(int list_id);

	int GetSizeList(int list_id);

	int Pop(int list_id);

	int randomPop(int list_id);
	bool Contains(int list_id, int elt) const;


	StringView GetName(int list_id) const;

	bool IsValid(int list_id) const;

	int GetSize() const;

	void SetWarning(int w);

	std::vector<lcf::DBString> GetSaveData();

private:
	bool ShouldWarn(int first_id, int last_id) const;
	void WarnGet(int variable_id) const;

private:
	ListVar_t _stringvar;
	StringVar_t2 _stringvarDBS;

	int MaxSize = 200;

	void tokenize(std::string const& str, const char delim, std::vector<std::string>& out);

	mutable int _warnings = kMaxWarnings;
};

inline void Game_Lists::SetData(ListVar_t s) {
	_stringvar = std::move(s);
}


inline void Game_Lists::SetSaveData(StringVar_t2 s) {
	_stringvar.clear();

	_stringvarDBS = std::move(s);

	for (int i = 0; i < _stringvarDBS.size();  i++) {
		lcf::DBString ss = _stringvarDBS[i];

		std::string string = ss.data();
		const char delim = '/';

		std::vector<std::string> out;
		tokenize(string, delim, out);
		for (std::string s2 : out) {
			int v = std::stoi(s2);
			Add(i + 1, v);
		}
	}
}

inline const Game_Lists::ListVar_t& Game_Lists::GetData() const {
	return _stringvar;
}

inline int Game_Lists::GetSize() const {
	return static_cast<int>(MaxSize);
}

inline bool Game_Lists::IsValid(int variable_id) const {
	return variable_id > 0 && variable_id <= GetSize();
}

inline bool Game_Lists::ShouldWarn(int first_id, int last_id) const {
	return (first_id <= 0 || last_id > static_cast<int>(MaxSize)) && (_warnings > 0);
}

inline std::vector<int> Game_Lists::Get(int list_id) const {
	if (EP_UNLIKELY(ShouldWarn(list_id, list_id))) {
		WarnGet(list_id);
	}
	if (list_id <= 0 || list_id > static_cast<int>(_stringvar.size())) {
		std::vector<int> v;
		return v;
	}
	return _stringvar[list_id - 1];
}

inline void Game_Lists::SetWarning(int w) {
	_warnings = w;
}

#endif
