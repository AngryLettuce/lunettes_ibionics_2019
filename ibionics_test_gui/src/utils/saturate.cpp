void saturateValue(int &value, int min, int max){
	if (value < min){
		value = min;
		return;
	}
	else if(value > max) {
		value = max;
		return;
	}
}
