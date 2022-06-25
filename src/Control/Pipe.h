#ifndef CONTROL_PIPE_H_
#define CONTROL_PIPE_H_

#include <vector>

template <typename T>
class Pipe {
private:
	std::vector<T> vals;

public:
	Pipe(){}
	~Pipe(){}

	void add(T message){
		vals.push_back(message);
	}

	T next(){
		if(vals.size()>0){
			T out = vals[0];
			vals.erase(vals.begin());
			return out;
		}
		return T();
	}

	void clear(){
		vals.clear();
	}

	bool isEmpty() {
		return vals.size()==0;
	}
};


#endif
