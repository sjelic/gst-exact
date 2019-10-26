
#include<iostream>
#include<limits>
#include<queue>
#include<cmath>
using namespace std;


typedef unsigned int _index;
typedef float  sreal;
typedef double dreal;


//typedef index unsigned int;

template <class KeyType>
class BinaryHeap
{
protected:
    KeyType* A; // polje s hrpom
    _index*  P;
    _index*  Q;
    queue<_index> F;
    _index hS;
    _index MhS;

    inline _index left(_index i) const;
    inline _index right(_index i) const;
    inline _index parent(_index i) const;

public:
    BinaryHeap(); // create empty heap
    BinaryHeap(_index n); //create empty heap of maximal size n
    template<class KT> BinaryHeap(BinaryHeap<KT>& H); //  copy heap


    virtual void heapify(_index i) = 0;

	void changekey(_index i, KeyType nV);

	virtual _index push(KeyType newK) = 0;
	_index pop();
	void remove(_index i);
	virtual _index top() const = 0;

    void buildheap();

    bool empty() const;
    _index size() const;
    template<class U> friend ostream& operator<<(ostream& buffer, const BinaryHeap<U>& hrpa);

    ~BinaryHeap();
private:
    virtual void decreasekey(_index i, KeyType nV) = 0;
	virtual void increasekey(_index i, KeyType nV) = 0;


};

template <class KeyType>
BinaryHeap<KeyType>::BinaryHeap():  A(NULL), P(NULL), Q(NULL), hS(0), MhS(0){};

template <class KeyType>
BinaryHeap<KeyType>::BinaryHeap(_index n): A(new KeyType[n]), P(new _index[n]), Q(new _index[n]), hS(0), MhS(n)
{
    for(_index i=0; i<n; i++){ P[i]=n; Q[i]=n; F.push(i);}
};

template <class KeyType>
BinaryHeap<KeyType>::~BinaryHeap()
{
    if(MhS > 0)
    {
        delete[] A;
        delete[] P;
        delete[] Q;
    }
}

template <class KeyType>
inline _index BinaryHeap<KeyType>::left(_index i) const
{
    _index lc = (i<<1)+1;
	if (lc >= this->hS)  return i;
	return lc;
}

template <class KeyType>
inline _index BinaryHeap<KeyType>::right(_index i) const
{
    _index rc = (i+1)<<1;
	if (rc >= this->hS) return i;
	return rc;
}


template <class KeyType>
inline _index BinaryHeap<KeyType>::parent(_index i) const
{
    return (i-1)>>1;
}

template <class KeyType>
void BinaryHeap<KeyType>::changekey(_index i, KeyType nV) // vratiti se...
{

    i = P[i];
    if (i >= this->hS) return;
	if (nV < A[i]){ decreasekey(i, nV); return; }
	if (nV > A[i]){ increasekey(i, nV); return; }
	return;
}

template <class KeyType>
void BinaryHeap<KeyType>::remove(_index i)
{
    _index pos = P[i];
	if (pos >= hS) return;
	A[pos] = A[hS - 1];

	Q[pos] = Q[hS - 1];
	P[Q[hS - 1]] = pos;
	P[i] = std::numeric_limits<_index>::max();

	F.push(i);


	hS--;
	heapify(pos);
	return;
}

template <class KeyType>
_index BinaryHeap<KeyType>::pop()
{
	if (hS == 0)  return std::numeric_limits<_index>::max();
	_index removed = Q[0] ;
	A[0] = A[hS - 1];

    F.push(removed);
	P[Q[hS - 1]] = 0;
	P[removed] = std::numeric_limits<_index>::max();
	Q[0] = Q[hS - 1];


	hS--;
	heapify(0);
	return removed;
}

template <class KeyType>
void BinaryHeap<KeyType>::buildheap()
{
    hS = MhS;
    for(int i = (MhS>>1) - 1; i>=0; i--) heapify(i);
    return;

}

template <class KeyType>
bool BinaryHeap<KeyType>::empty() const
{
    return hS == 0;
}

template <class KeyType>
_index BinaryHeap<KeyType>::size() const
{
    return hS;
}

/*template <class KeyType>
ostream& operator<<(ostream& buffer, const BinaryHeap<KeyType>& hrpa)
{
    if(hrpa.empty()){ return buffer; }
    else
    {
        for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.A[i]<<" ";
        buffer<<endl;
        for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.P[i]<<" ";
        buffer<<endl;
        for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.Q[i]<<" ";
        buffer<<endl;
    }
    return buffer;
}*/

template <class KeyType>
class MinBinaryHeap : public BinaryHeap<KeyType>
{

public:
    MinBinaryHeap(): BinaryHeap<KeyType>::BinaryHeap(){}; // create empty heap
    MinBinaryHeap(_index n) : BinaryHeap<KeyType>::BinaryHeap(n){}; //create empty heap of maximal size n
    MinBinaryHeap(KeyType* A, _index n); // create heap from the array
    template<class KT> MinBinaryHeap(MinBinaryHeap<KT>& H); //  copy heap


    void heapify(_index i);
	_index push(KeyType newK);
	_index pushi(KeyType newK, _index slot);
	_index top() const;
	KeyType topk() const;

    //bool empty() const;
    template<class U> friend ostream& operator<<(ostream& buffer, const MinBinaryHeap<U>& hrpa);
    /*template<class U>
T* Matrix<T>::operator[](int i)*/
/*{
   return (&M[i*n]);
}*/
    ~MinBinaryHeap(){};
private:
    void decreasekey(_index i, KeyType nV);
	void increasekey(_index i, KeyType nV);
};


template <class KeyType>
MinBinaryHeap<KeyType>::MinBinaryHeap(KeyType* A, _index n)
{
    this->MhS = n;
    this->hS = n;
    this->A = new KeyType[n];
    this->P = new _index[n];
    this->Q = new _index[n];
    for(_index i=0; i<n; i++)
    {
            this->A[i]=A[i];
            this->P[i]=i;
            this->Q[i]=i;
    }
    this->buildheap();
}

//template <class KeyType>
//MinBinaryHeap<KeyType>::~MinBinaryHeap() { /*BinaryHeap<KeyType>::~BinaryHeap();*/ }

template <class KeyType>
void MinBinaryHeap<KeyType>::heapify(_index i)
{
    _index smallest, l, r;
	while (true)
	{
		l = this->left(i);
		r = this->right(i);
		if (this->A[l] < this->A[i]) { smallest = l; }
		else { smallest = i; }
		if (this->A[r] < this->A[smallest]) smallest = r;
		if (smallest == i) return;
		// exchange elements in container
		swap(this->A[i],this->A[smallest]);
		swap(this->Q[i],this->Q[smallest]);
		swap(this->P[this->Q[i]],this->P[this->Q[smallest]]);
		i = smallest;
	}
	return;
}

template <class KeyType>
void MinBinaryHeap<KeyType>::decreasekey(_index i, KeyType nV)
{
    if(this->A[i] <= nV) return;
	this->A[i] = nV;
	while (i>0 && this->A[i]< this->A[this->parent(i)])
	{
		swap(this->A[i], this->A[this->parent(i)]);
		swap(this->Q[i],this->Q[this->parent(i)]);
		swap(this->P[this->Q[i]],this->P[this->Q[this->parent(i)]]);
		i = this->parent(i);
	}
	return;
}

template <class KeyType>
void MinBinaryHeap<KeyType>::increasekey(_index i, KeyType nV)
{
    if(this->A[i] >= nV) return;
	this->A[i] = nV;
	heapify(i);
	return;
}

template <class KeyType>
_index MinBinaryHeap<KeyType>::push(KeyType newK)
{
	if(this->hS >= this->MhS) return this->hS;
	(this->hS)++;
	KeyType temp = newK;
	//this->A[this->hS - 1] = std::numeric_limits<KeyType>::infinity();
	this->A[this->hS - 1] = std::numeric_limits<KeyType>::max();
	_index slot = this->F.front();
	this->F.pop();
	this->P[slot] = this->hS - 1;
	this->Q[this->hS - 1] = slot;
	decreasekey( this->hS - 1, temp);
	return slot;
}


template <class KeyType>
_index MinBinaryHeap<KeyType>::pushi(KeyType newK, _index slot) // lose ako se koristi kombinirano
{
	if(this->hS >= this->MhS) return this->hS;
	if (slot >= this->MhS) return std::numeric_limits<_index>::max();
	KeyType temp = newK;
	if(this->P[slot] < this->MhS)
    {
        this->decreasekey(this->P[slot],newK);
        return slot;
    }
    else (this->hS)++;
	//this->A[this->hS - 1] = std::numeric_limits<KeyType>::infinity();
	this->A[this->hS - 1] = std::numeric_limits<KeyType>::max();

	/*while(this->P[slot] < this->MhS) // trazenje slota
    {
        slot = this->F.front();
        this->F.pop();
    }*/


    // ako element/tj. slot postoji, daj mu novi prioritet i u tom slucaju nemoj mijenjati velicinu, inace ubaci novi element
	this->P[slot] = this->hS - 1;
	this->Q[this->hS - 1] = slot;
	decreasekey( this->hS - 1, temp);
	return slot;
}



template <class KeyType>
_index MinBinaryHeap<KeyType>::top() const
{
	if (this->hS > 0) return this->Q[0];
	return std::numeric_limits<_index>::max();
}

template <class KeyType>
KeyType MinBinaryHeap<KeyType>::topk() const
{
	if (this->hS > 0) return this->A[0];
	return std::numeric_limits<_index>::max();
}

template <class KeyType>
ostream& operator<<(ostream& buffer, const MinBinaryHeap<KeyType>& hrpa)
{
    if(hrpa.empty()){ return buffer; }
    else
    {
        for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.A[i]<<" ";
        //buffer<<endl;
        /*for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.P[i]<<" ";
        buffer<<endl;
        for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.Q[i]<<" ";
        buffer<<endl;*/
    }
    return buffer;
}


template <class KeyType>
class MaxBinaryHeap : public BinaryHeap<KeyType>
{
public:
    MaxBinaryHeap(): BinaryHeap<KeyType>::BinaryHeap(){}; // create empty heap
    MaxBinaryHeap(_index n) : BinaryHeap<KeyType>::BinaryHeap(n){}; //create empty heap of maximal size n
    MaxBinaryHeap(KeyType* A, _index n); // create heap from the array
    template<class KT> MaxBinaryHeap(MaxBinaryHeap<KT>& H); //  copy heap


    void heapify(_index i);
	_index push(KeyType newK);
	_index top() const;

    //bool empty() const;
    template<class U> friend ostream& operator<<(ostream& buffer, const MaxBinaryHeap<U>& hrpa);

    ~MaxBinaryHeap(){};
private:
    void decreasekey(_index i, KeyType nV);
	void increasekey(_index i, KeyType nV);
};

template <class KeyType>
MaxBinaryHeap<KeyType>::MaxBinaryHeap(KeyType* A, _index n)
{
    this->MhS = n;
    this->hS = n;
    this->A = new KeyType[n];
    this->P = new _index[n];
    this->Q = new _index[n];
    for(_index i=0; i<n; i++)
    {
            this->A[i]=A[i];
            this->P[i]=i;
            this->Q[i]=i;
    }
    this->buildheap();
}

//template <class KeyType>
//MaxBinaryHeap<KeyType>::~MaxBinaryHeap() { BinaryHeap<KeyType>::~BinaryHeap(); }

template <class KeyType>
void MaxBinaryHeap<KeyType>::heapify(_index i)
{
    _index largest, l, r;
	while (true)
	{
		l = this->left(i);
		r = this->right(i);
		if (this->A[l] > this->A[i]) { largest = l; }
		else { largest = i; }
		if (this->A[r] > this->A[largest]) largest = r;
		if (largest == i) return;
		// exchange elements in container
		swap(this->A[i],this->A[largest]);
		swap(this->Q[i],this->Q[largest]);
		swap(this->P[this->Q[i]],this->P[this->Q[largest]]);
		i = largest;
	}
	return;
}

template <class KeyType>
void MaxBinaryHeap<KeyType>::increasekey(_index i, KeyType nV)
{
	this->A[i] = nV;
	while (i>0 && this->A[i] > this->A[this->parent(i)])
	{
		swap(this->A[i], this->A[this->parent(i)]);
		swap(this->Q[i],this->Q[this->parent(i)]);
		swap(this->P[this->Q[i]],this->P[this->Q[this->parent(i)]]);
		i = this->parent(i);
	}
	return;
}



template <class KeyType>
void MaxBinaryHeap<KeyType>::decreasekey(_index i, KeyType nV)
{
    this->A[i] = nV;
	heapify(i);
	return;
}

template <class KeyType>
_index MaxBinaryHeap<KeyType>::push(KeyType newK)
{
	if(this->hS >= this->MhS) return this->hS;
	(this->hS)++;
	KeyType temp = newK;
	this->A[this->hS - 1] = std::numeric_limits<KeyType>::min();
	_index slot = this->F.front();
	this->F.pop();
	this->P[slot] = this->hS - 1;
	this->Q[this->hS - 1] = slot;
	increasekey( this->hS - 1, temp);
	return slot;
}


template <class KeyType>
_index MaxBinaryHeap<KeyType>::top() const
{
	if (this->hS > 0) return this->Q[0];
	return std::numeric_limits<_index>::min();
}

template <class KeyType>
ostream& operator<<(ostream& buffer, const MaxBinaryHeap<KeyType>& hrpa)
{
    if(hrpa.empty()){ return buffer; }
    else
    {
        for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.A[i]<<" ";
        //buffer<<endl;
        /*for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.P[i]<<" ";
        buffer<<endl;
        for(_index i=0; i<hrpa.hS; i++) buffer<<hrpa.Q[i]<<" ";
        buffer<<endl;*/
    }
    return buffer;
}







/*
int main()
{
    string naredba;
    while(1)
    {
        cin>>naredba;
        if(!naredba.compare("BUILD"))
        {
            int size;
            cin>>size;
            Heap<float> hrpa(size);
            cout<<"Hrpa: "<<hrpa<<endl;
            continue;
        }
        if(!naredba.compare("SORT"))
        {
            int size;
            cin>>size;
            float* polje = new float[size];
            for(int i=0; i<size; i++) cin>>polje[i];
            cout<<endl;
            heapsort<float>(polje,size);
            for(int i=0; i<size; i++) cout<<polje[i]<<" ";
            cout<<endl;
            delete[] polje;
            continue;
        }
        if(!naredba.compare("exit"))
        {
            cout<<"Izlaz"<<endl;
            break;
        }
        cout<<"Naredba nije prepoznata:\n BUILD - izfradnja hrpe \n SORT - sortiranje pomocu HEAPSORT algoritma\n exit - izlaz\n\n";
    }
    return 0;
}*/
