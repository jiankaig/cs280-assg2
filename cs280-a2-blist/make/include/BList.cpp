#include "BList.h"
#include <iostream>//remove before submit!
// #define DEBUG_

template <typename T, unsigned Size>
BList<T, Size>::BList() : stats_(sizeof(BNode), 0, Size, 0){
  // default constructor
  try{
    head_ = new BNode();
    tail_ = head_;

  }
  catch (const std::exception& e){
    throw(BListException(BListException::E_NO_MEMORY, e.what()));
  }
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
  while (head_)
  {
    BNode* curNode = head_;
    head_ = head_->next; // get next
    delete curNode; // delete current
  }
  // set all to initial value
  head_ = tail_ = nullptr;
  stats_.NodeCount = stats_.ItemCount = 0;
  // unsorted = true;
}                         

template <typename T, unsigned Size>
BList<T, Size>& BList<T, Size>::operator=(const BList &rhs){
  // assign operator
  return const_cast<BList&>(rhs);
}

// arrays will be unsorted, if calling either of these
template <typename T, unsigned Size>
void BList<T, Size>::push_back(const T& value){
  State ret = FAIL;
  BNode* ptrNode = head_; //start at head node
  BNode* ptrPrevNode;
  int i=0;
  while(ret != SUCCESS){
    //  printf("%i ",i);
    //if node got space, assign to node..
    if(ptrNode->count < static_cast<int>(Size)){
      ptrNode->values[ptrNode->count] = value; // assign value to front, based on count
      // std::cout<<"BREAKPT -assign "<<value<<" at "<<&ptrNode->values[ptrNode->count]<<"\n";
      ptrNode->count++; //update count
      stats_.ItemCount++;
      ret = SUCCESS;
      break;
    }
    else if(ptrNode->next == NULL && ptrNode->count >= Size){
      //check next node if it exists & node is full
      //create a new node and link it with previous
      ptrPrevNode = ptrNode;
      ptrNode = ptrNode->prev;
      try{
        ptrNode = new BNode();
        ptrNode->prev = ptrPrevNode;
        ptrPrevNode->next = ptrNode;
        // std::cout<<"BREAKPT - new node created"<<head_<<" | "<<tail_<<"\n";
        tail_ = ptrNode;
        // std::cout<<"BREAKPT - new node created"<<head_<<" | "<<tail_<<"\n";
        stats_.NodeCount++;
      }
      catch (const std::exception& e){
        throw(BListException(BListException::E_NO_MEMORY, e.what()));
      }
      
      
    }
    else if(ptrNode->count < Size){
      insertAt(value, ptrNode, ptrNode->count);
    }
    else{
      //next node exists, move pointer to next node
      ptrNode = ptrNode->next;
    }
    i++;
  }
  
}

template <typename T, unsigned Size>
void BList<T, Size>::push_front(const T& value){
  State ret = FAIL;
  BNode* ptrNode = head_; //start at head node
  BNode* PrevNode;
  while(ret != SUCCESS){
    //if node got space, assign to head..
    if(ptrNode->count < static_cast<int>(Size)){
      //shift current values up by one position
      int position = ptrNode->count;
      for(int i=position-1; i>=0; i--){
        ptrNode->values[i+1] = ptrNode->values[i];
      }
      ptrNode->values[0] = value; //insert value at front of node
      ptrNode->count++; //update count
      stats_.ItemCount++;
      ret = SUCCESS;
      break;
    }
    else{
      //create new node and place in front of head node
      BNode* NewNode = nullptr;
      try{
        NewNode = new BNode();
        PrevNode = ptrNode;
        ptrNode = NewNode;
        PrevNode->prev = NewNode;
        NewNode->next = PrevNode;
        head_ = NewNode;
        stats_.NodeCount++;
      }
      catch (const std::exception& e){
        throw(BListException(BListException::E_NO_MEMORY, e.what()));
      }
    }
  }
  
}

template <typename T, unsigned Size>
void BList<T, Size>::insert(const T& value){
  #ifdef DEBUG_
    std::cout<<"inserting "<<value<<" ";
  #endif
  BNode* ptrNode = head_; //start at head node

  //find position to insert
  //search through every node
  while(ptrNode){
    for(unsigned int j=0; j< Size; j++){
      // base case: first item just push front
      if(stats_.ItemCount == 0){
        push_front(value); //basecase
        return;
      }
      // split node when value < item and node is full
      if(value < ptrNode->values[j] && ptrNode->count >= Size){
        SplitNode(ptrNode);
          #ifdef DEBUG_
            std::cout<<" ||| ";
          #endif
      }

      // insert(before item) when value less than item and not check unassigned data
      if(value < ptrNode->values[j] && j < ptrNode->count){
        // special case, where after split, node may be full
        if(Size == 1){
          #ifdef DEBUG_
            std::cout<<" || ";
          #endif
          insertAt(ptrNode->values[0], ptrNode->next, 0);
          ptrNode->values[0] = (T)0;
          ptrNode->count--;
          stats_.ItemCount--;
          insertAt(value, ptrNode, j);
          return;
        }
        else{
          #ifdef DEBUG_
            std::cout<<" | "<<ptrNode->values[j];
          #endif
          //insert value before, normal case
          insertAt(value, ptrNode, j);
          return;
        } 
      }

      // split when at last node, last item 
      if(ptrNode->next == nullptr && j+1 == ptrNode->count ){
        if(ptrNode->count >= Size){ //and node is full 
          SplitNode(ptrNode);
          if(Size>1)
            insertAt(value, ptrNode->next, ptrNode->next->count);
          else if(Size ==1)
            insertAt(value, ptrNode->next, 0);
          #ifdef DEBUG_
            std::cout<<" / ";
          #endif
          return;
        }
        else{
          // push_back(value);
          insertAt(value, ptrNode, ptrNode->count); //insert in front since value > item
          // std::cout<<" // ";
          return;
        }
      }

      if(j >= ptrNode->count && value < ptrNode->next->values[0]){
        //check next node[0], if smaller, insert here
        insertAt(value, ptrNode, j);
        #ifdef DEBUG_
          std::cout<<" /// ";
        #endif
        return;
      }
      else{
        //need to insert at next node..
      }
      
    }
          // std::cout<<" |D\n ";
    ptrNode = ptrNode->next;
  }
  //if no suitable position, just pushback
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
  BNode* ptr = head_;
  int ret_lsb = -1, ret_msb = 0;
  while(ptr){
    ret_lsb = SearchFor(value, ptr);
    if(ret_lsb >= 0)
      return (ret_lsb + ret_msb);
    ret_msb += ptr->count; 
    ptr = ptr->next;
  }
  return -1;
}       

template <typename T, unsigned Size>
T& BList<T, Size>::operator[](int index){
  // for l-values
  if(index > stats_.ItemCount)
    throw BListException(BListException::E_BAD_INDEX, "invalid index!");

  unsigned int position = index + 1;
  BNode* ptrNode = head_;

  //loop through nodes
  while(ptrNode){
    //find position
    if(position <= ptrNode->count)
      break;
    position -= ptrNode->count;
    ptrNode = ptrNode->next; // go to next node
  }
  return ptrNode->values[position-1];
}            

template <typename T, unsigned Size>
const T& BList<T, Size>::operator[](int index) const{
  // std::cout<<"[] rvalue\n";
  // for r-values
  if(index > stats_.ItemCount)
    throw BListException(BListException::E_BAD_INDEX, "invalid index!");

  unsigned int position = index + 1;
  BNode* ptrNode = head_;

  //loop through nodes
  while(ptrNode){
    //find position
    if(position <= ptrNode->count)
      break;
    position -= ptrNode->count;
    ptrNode = ptrNode->next; // go to next node
  }
  return ptrNode->values[position-1];
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

template <typename T, unsigned Size>
void BList<T, Size>::copy_to_(T* arrSrc, unsigned int size, T* arrDest){
  for(unsigned int i = 0; i<size ; i++){
    arrDest[i] = arrSrc[i];
  }
}

template <typename T, unsigned Size>
void BList<T, Size>::insertAt(T value, BNode* ptrNode, int insertPos){
  // std::cout<<"DEBUG: count  "<<ptrNode->count<<std::endl;
  if(ptrNode->count < Size){
    //insert and shift the rest up by one
    //shift current values up by one position
    unsigned int position = ptrNode->count;
    for(int i=position-1; i>=insertPos; i--){
      ptrNode->values[i+1] = ptrNode->values[i];
      #ifdef DEBUG_
        std::cout<<"DEBUG: shifting "<<ptrNode->values[i]<<std::endl;
      #endif
    }
    ptrNode->values[insertPos] = value; //insert value at front of node
    #ifdef DEBUG_
      std::cout<<"insert in "<< ptrNode->values[insertPos]<<std::endl;
    #endif
    ptrNode->count++; //update count
    stats_.ItemCount++;
    return;
  }
}

template <typename T, unsigned Size>
void BList<T, Size>::SplitNode(BNode* &ptrNode){
  //create two sub array and assign each with values
  T* arrLeft = new T[Size];
  T* arrRight = new T[Size];
  unsigned int mid = static_cast<unsigned int>(ptrNode->count/2 - 0.5);
  unsigned int arrLeftIndex = 0;
  unsigned int arrRightIndex = 0;
  for(unsigned int i=0; i<ptrNode->count; i++){
    if(i <= mid)
      arrLeft[arrLeftIndex++] = ptrNode->values[i];
    else if(i > mid)
      arrRight[arrRightIndex++] = ptrNode->values[i];
  }
  // // assign remaining slots in sub arrays to zero
  // while(arrLeftIndex<Size){
  //   arrLeft[arrLeftIndex++] = static_cast<T>(0);
  // }
  // while(arrRightIndex<Size){
  //   arrRight[arrRightIndex++] = static_cast<T>(0);
  // }


  //create two nodes, link them into BList
  //BNode* NewLeftNode = new BNode();
  BNode* NewRightNode = new BNode();

  BNode* PrevNode = ptrNode->prev;
  BNode* NextNode = ptrNode->next;
  if(PrevNode != nullptr){
    PrevNode->next = ptrNode;
    ptrNode->prev = PrevNode;
  }

  ptrNode->next = NewRightNode;
  NewRightNode->prev = ptrNode;
  
  if(NextNode != nullptr){
    NewRightNode->next = NextNode;
    NextNode->prev = NewRightNode;
  }

  ptrNode->count = arrLeftIndex;
  NewRightNode->count = arrRightIndex;
  // printf("DEBUG in split: %u, %u\n", arrLeftIndex, arrRightIndex);

  copy_to_(arrLeft, Size, ptrNode->values);
  copy_to_(arrRight, Size, NewRightNode->values);
  //printf("%p, %p\n", (void*)ptrNode, (void*)ptrNode->next);
  //printf("%p, %p, %p, %p\n", (void*)PrevNode, (void*)NewLeftNode, (void*)NewRightNode, (void*)NextNode);
  //delete splited node
  //delete [] ptrNode;
  //ptrNode = NewLeftNode;
  stats_.NodeCount++;
}

template <typename T, unsigned Size>
int BList<T, Size>::SearchFor(const T& value, BNode* node)const{
  T* array = node->values;
  int count = node->count;
  for(int i = 0; i < count; i++){
    if(value == array[i])
      return i;
  }
  return -1;
}