#include "BList.h"
#include <iostream>//remove before submit!
template <typename T, unsigned Size>
BList<T, Size>::BList() : stats_(sizeof(BNode), 0, Size, 0){
  // default constructor
  head_ = new BNode();
  stats_.NodeCount++;
  //tail_ = head_;
}                          
template <typename T, unsigned Size>
BList<T, Size>::BList(const BList &rhs){
  // copy constructor
  head_ = rhs.head_;
  stats_ = rhs.GetStats();
} 

template <typename T, unsigned Size>
BList<T, Size>::~BList(){
  // destructor
  
}                         

template <typename T, unsigned Size>
BList<T, Size>& BList<T, Size>::operator=(const BList &rhs){
  // assign operator
  return const_cast<BList&>(rhs);
}

// arrays will be unsorted, if calling either of these
template <typename T, unsigned Size>
void BList<T, Size>::push_back(const T& value){
  tail_->values[0] = value;
}

template <typename T, unsigned Size>
void BList<T, Size>::push_front(const T& value){
  State ret = FAIL;
  BNode* ptrNode = head_; //start at head node
  BNode* ptrPrevNode;
  BNode* ptrCurrNode;
  while(ret != SUCCESS){
    //if node got space, assign to head..
    //printf("count: %u\t size: %u \n", ptrNode->count, Size);
    if(ptrNode->count < static_cast<int>(Size)){
      //std::cout<<"BREAKPT\n";
      ptrNode->values[ptrNode->count] = value; // assign value to front, based on count
      ptrNode->count++; //update count
      stats_.ItemCount++;
      ret = SUCCESS;
      break;
    }
    else if(ptrNode->prev == NULL){
      //check prev node if it exists
      //create a new node and link it with previous
      ptrCurrNode = ptrNode;
      ptrNode = ptrNode->prev;
      //std::cout<<"BREAKPT2\n";
      ptrNode = new BNode();
      ptrNode->prev = ptrCurrNode;
      ptrCurrNode->prev = ptrNode;
      head_ = ptrNode;
      
    }
    else{
      //prev node exists, move pointer to next node
      //std::cout<<"BREAKPT3\n";
      ptrPrevNode = ptrNode;
      ptrNode = ptrNode->next;
    }
  }
  
}

template <typename T, unsigned Size>
void BList<T, Size>::insert(const T& value){
  if(head_->values[0] == value)
    return;
}

template <typename T, unsigned Size>
void BList<T, Size>::remove(int index){
  if(index)
    return;
}

template <typename T, unsigned Size>
void BList<T, Size>::remove_by_value(const T& value){
  if(head_->values[0] == value)
    return;
}

template <typename T, unsigned Size>
int BList<T, Size>::find(const T& value) const{
  // returns index, -1 if not found
  if(head_->values[0] == value)
    return -1;
  return -1;
}       

template <typename T, unsigned Size>
T& BList<T, Size>::operator[](int index){
  // for l-values
  //if(index > stats_.ItemCount)
    //return ERROR; //out of range

  int countNodes = stats_.NodeCount;
  int countIndex = index;
  BNode* ptrNode = head_;

  //loop through nodes
  while(countNodes){
    //loop through items in node
    int countItems = ptrNode->count;
    while(countItems){
      if(countIndex == 0){
        return ptrNode->values[countItems];
      }
      countIndex--;
      countItems--;
    }
    ptrNode = ptrNode->next; // go to next node
    countNodes--;
  }
  return ptrNode->values[0];
}            

template <typename T, unsigned Size>
const T& BList<T, Size>::operator[](int index) const{
  // for r-values
  if(index > stats_.ItemCount)
    return *ptrToItem; //out of range

  unsigned int countNodes = stats_.NodeCount;
  unsigned int countIndex = index;
  BNode* ptrNode = head_;
  T* ptrToItem = nullptr;

  //loop through nodes
  while(countNodes){
    //loop through items in node
    unsigned int countItems = static_cast<unsigned int>(ptrNode->count);
    while(countItems){
      if(countIndex == 0){
        *ptrToItem = ptrNode->values[countItems];
        return *ptrToItem;
      }
      countIndex--;
      countItems--;
    }
    ptrNode = ptrNode->next; // go to next node
    countNodes--;
  }
  return *ptrToItem;
} 

template <typename T, unsigned Size>
size_t BList<T, Size>::size() const{
  // total number of items (not nodes)
  return stats_.ItemCount;
}   

template <typename T, unsigned Size>
void BList<T, Size>::clear(){
  // delete all nodes
}          

// template <typename T, unsigned Size>
// size_t BList<T, Size>::nodesize(){
//   // so the allocator knows the size
//   return stats_.NodeSize;
// } 

// For debugging
// template <typename T, unsigned Size>
// const BList<T, Size>::BNode* BList<T, Size>::GetHead() const{
//   return head_;
// }
template <typename T, unsigned Size>
BListStats BList<T, Size>::GetStats() const{
  return stats_;
}


template <typename T, unsigned Size>
size_t BList<T, Size>::nodesize(void)
{
  return sizeof(BNode);
}

template <typename T, unsigned Size>
const typename BList<T, Size>::BNode* BList<T, Size>::GetHead() const
{
  return head_;
}
