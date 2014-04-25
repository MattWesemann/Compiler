#include "namespace.h"

using namespace std;

Namespace::Namespace(){
}

// currently loops through array seeing if correct chars already exist and then uses those
// if not add to end
// this is NOT optimal given the following case
// bytes = "nameixyz"
// try to add "names"
// result is "nameixyznames" but should be "namesixyz" or "ixyznames"
Name Namespace::add(string name){
	size_t index = 0;
	for (size_t i = 0; i < bytes.size(); ++i){
		if (name[index] == bytes[i]){
			index++;
			if (index == name.length()){
				return Name(i - index + 1, index);
			}
		}
		else
			index = 0;
	}

	bytes.insert(bytes.end(), name.begin(), name.end());
	return Name(bytes.size() - name.length(), name.length());
}

string Namespace::get(size_t offset, size_t len){
	if (offset + len > bytes.size())
		throw new exception("invalid string");

	return string(&bytes[offset], len);
}

Namespace* Namespace::singleton = nullptr;
Namespace* Namespace::instance(){
	if (singleton == nullptr)
		singleton = new Namespace();
	return singleton;
}

void Namespace::clear(){
	bytes.clear();
}