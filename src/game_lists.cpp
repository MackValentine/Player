#include "game_lists.h"
#include <output.h>
#include <lcf/reader_util.h>
#include <lcf/data.h>

Game_Lists::Game_Lists() {
	_stringvar.reserve(MaxSize);
}

void Game_Lists::WarnGet(int variable_id) const {
	Output::Debug("Invalid read sw[{}]!", variable_id);
	--_warnings;
}

int Game_Lists::GetSizeList(int list_id) {
	int r = 0;
	if (list_id - 1 < _stringvar.size()) {
		r = _stringvar[list_id - 1].size();
	}
	return r;
}

void Game_Lists::Clear(int list_id) {
	if (list_id - 1 < _stringvar.size()) {
		_stringvar[list_id - 1].clear();
	}
}

void Game_Lists::DeleteElt(int list_id, int elt) {
	if (list_id - 1 < _stringvar.size()) {
		int i = 0;
		for (int id : _stringvar[list_id - 1]) {
			if (elt == id) {
				_stringvar[list_id - 1].erase(_stringvar[list_id - 1].begin() + i);
				break;
			}
			i++;
		}
	}
}


void Game_Lists::Delete(int list_id, int elementID) {
	if (list_id - 1 < _stringvar.size()) {
		if (!_stringvar[list_id - 1].empty()) {
			_stringvar[list_id - 1].erase(_stringvar[list_id - 1].begin() + elementID);
		}
	}
}

int Game_Lists::Pop(int list_id) {
	int value = 0;
	if (list_id - 1 < _stringvar.size()) {
		if (!_stringvar[list_id - 1].empty()) {
			value = _stringvar[list_id - 1].front();
			_stringvar[list_id - 1].erase(_stringvar[list_id - 1].begin());
		}
	}
	return value;
}

int Game_Lists::randomPop(int list_id) {
	int value = 0;
	if (list_id - 1 < _stringvar.size()) {
		if (!_stringvar[list_id - 1].empty()) {
			int r = std::rand() % _stringvar[list_id - 1].size();
			value = _stringvar[list_id - 1][r];
			_stringvar[list_id - 1].erase(_stringvar[list_id - 1].begin() + r);
		}
	}
	return value;
}

int Game_Lists::GetElt(int list_id, int i) {
	int value = 0;
	if (list_id - 1 < _stringvar.size() && i < _stringvar[list_id - 1].size())
		value = _stringvar[list_id - 1][i];
	return value;
}

std::vector<int> Game_Lists::Set(int list_id, std::vector<int> values) {
	if (list_id > _stringvar.size()) {
		_stringvar.resize(list_id);
	}

	_stringvar[list_id - 1] = values;

	return _stringvar[list_id - 1];
}

int Game_Lists::Add(int variable_id, int value) {
	if (EP_UNLIKELY(ShouldWarn(variable_id, variable_id))) {
		Output::Debug("Invalid write sw[{}] = {}!", variable_id, value);
		--_warnings;
	}
	if (variable_id <= 0) {
		return 0;
	}
	auto& ss = _stringvar;
	if (variable_id > static_cast<int>(ss.size())) {
		ss.resize(variable_id);
	}
	ss[variable_id - 1].push_back(value);
	return value;
}

StringView Game_Lists::GetName(int _id) const {
	const auto* sw = lcf::ReaderUtil::GetElement(lcf::Data::switches, _id);

	if (!sw) {
		// No warning, is valid because the switch array resizes dynamic during runtime
		return {};
	}
	else {
		return sw->name;
	}
}

bool Game_Lists::Contains(int list_id, int elt) const {
	bool b = false;
	if (list_id - 1 < _stringvar.size())
		for (int id : _stringvar[list_id - 1]) {
			if (elt == id) {
				b = true;
				break;
			}
		}
	return b;
}

std::vector<lcf::DBString> Game_Lists::GetSaveData() {
	std::vector<lcf::DBString> v;
	for (std::vector<int> l : _stringvar) {
		std::string s;
		for (int i : l) {
			
			s = s + std::to_string(i) + "/";
		}
		v.push_back(lcf::DBString(s));
	}
	return v;
}

void Game_Lists::tokenize(std::string const& str, const char delim,
	std::vector<std::string>& out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}
