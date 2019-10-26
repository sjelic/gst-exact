#include<map>
#include<unordered_map>
typedef unsigned int _index;
typedef float  sreal;
typedef double dreal;
#include"heap.h"


// specific data structures
template <class Element, class PriorityType>
class MinPriorityQueue
{
protected:

	typename std::unordered_map<Element,_index>* I;
	Element* J;
	MinBinaryHeap<PriorityType>* priorities;

public:

	MinPriorityQueue() {}
	MinPriorityQueue(_index n);
	MinPriorityQueue(Element* E, PriorityType* prior, _index n);


	void changekey(Element elem, PriorityType nV);
	void remove(Element elem);
	void push(Element elem, PriorityType pr);
	void pop();

	bool empty() const;
	_index size() const;
	Element top() const;
	~MinPriorityQueue();
};

// implementation
template <class Element, class PriorityType>
MinPriorityQueue<Element,PriorityType>::MinPriorityQueue(Element* E, PriorityType* prior, _index n) : I(new std::unordered_map<Element,_index>()),priorities(new MinBinaryHeap<PriorityType>(prior,n)), J(E)
{
	for(int i=0; i<n; i++){
      *I[E[i]] = i;
	}
}

template <class Element, class PriorityType>
MinPriorityQueue<Element,PriorityType>::MinPriorityQueue(_index n) : I(new std::unordered_map<Element,_index>()),J(new Element[n]), priorities(new MinBinaryHeap<PriorityType>(n))
{
    for(int i=0; i<n; i++)J[i] = NULL;

}

template <class Element, class PriorityType>
MinPriorityQueue<Element,PriorityType>::~MinPriorityQueue()
{
	delete I;
	delete J;
	delete priorities;
}

template <class Element, class PriorityType>
void MinPriorityQueue<Element,PriorityType>::push(Element elem, PriorityType pr)
{
    _index ind = this->priorities->push(pr);
    this->J[ind] = elem;
    *I[elem] = ind;
    J[ind] = elem;
    return;
}

template <class Element, class PriorityType>
void MinPriorityQueue<Element,PriorityType>::pop()
{
    _index ind = this->priorities->pop();
    this->I->erase(J[ind]);
    J[ind] = NULL;
    return;
}

template <class Element, class PriorityType>
void MinPriorityQueue<Element,PriorityType>::remove(Element elem)
{
    typename std::unordered_map<Element,_index>::iterator it = this->I->find(elem);
    if(it == this->I->end()) return;
    _index ind  = it->second();
    this->priorities->remove(ind);
    this->I->erase(it);
    J[ind] = NULL;
    return;
}

template <class Element, class PriorityType>
void MinPriorityQueue<Element,PriorityType>::changekey(Element elem, PriorityType nV)
{
    typename std::unordered_map<Element,_index>::iterator it = this->I->find(elem);
    if(it == this->I->end()) return;
    _index ind  = it->second();
    this->priorities->changekey(ind,nV);
    return;
}

template <class Element, class PriorityType>
bool MinPriorityQueue<Element,PriorityType>::empty() const
{
    return this->priorities->empty();
}

template <class Element, class PriorityType>
_index MinPriorityQueue<Element,PriorityType>::size() const
{
    return this->priorities->size();
}

template <class Element, class PriorityType>
Element MinPriorityQueue<Element,PriorityType>::top() const
{
    _index t = this->priorities->top();
    if(t >= this->priorities->size()) return NULL;
    return this->J[t];
}
