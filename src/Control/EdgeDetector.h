#ifndef EDGEDETECTOR_H_
#define EDGEDETECTOR_H_

class EdgeDetector {
private:
	bool init = false;
	bool pState{};
	bool risingEdge{};

public:
	EdgeDetector(bool risingEdge=true)
	: risingEdge(risingEdge){
		pState = !risingEdge;
	}

	bool update(bool value){
		bool out = false;
		if(value != pState && ((risingEdge && value)||(!risingEdge && !value))){
			out = true;
		}
		pState = value;
		return out;
	}
};


#endif
