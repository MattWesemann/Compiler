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
				return Name(&bytes[i - index + 1], index);
			}
		}
		else
			index = 0;
	}

	bytes.insert(bytes.end(), name.begin(), name.end());
	return Name(&bytes[bytes.size() - name.length()], name.length());
}