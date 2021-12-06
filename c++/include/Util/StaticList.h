template <typename T, int size>
class StaticList{

	public:
	StaticList():
	n_elements(0)
	{

	}

	void clear(){
		n_elements = 0;
	}
	int getn() const {
		return n_elements;
	}
	bool add(T element){
		if (n_elements < size){
			elements[n_elements] = element;
			++n_elements;
			return true;
		}
		return false;
	}

	const T* const getElements() const {
		return elements;
	} 

    const T* begin(){
        return elements;
    }

    const T* end(){
        return elements + n_elements;
    }

	private:
	T elements[size];
	int n_elements;
};