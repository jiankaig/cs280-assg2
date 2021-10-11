/**
 * @file BList.cpp
 * @author Giam Jian Kai (jiankai.g\@digipen.edu)
 * @par email: jiankai.g\@digipen.edu
 * @par Digipend Login: jiankai.g
 * @par Course: CS280
 * @par Assignment #2
 * @brief 
 *    This file describes the interface to BList, BListException and BListStats
 *    This file contains the declaration of the following functions for
 *    the BList class:
 * 
 *    Public functions of BList include:
 *      Constructor
 *      Destructor
 *      operator=
 *      push_back
 *      push_front
 *      insert   
 *      remove
 *      remove_by_value
 *      find
 *      operator[]
 *      size
 *      clear
 *      nodesize
 *      GetHead
 *      GetStats
 *    Private methods include:
 *      copy_to_
 *      insertAt
 *      SearchFor
 *      SplitNode
 *      
 *    Hours spent on this assignment: 50
 *    Specific portions that gave you the most trouble: optimisation of free.
 * @version 0.1
 * @date 2021-10-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "BList.h"

/**
 * @fn BList<T, Size>::BList
 * @brief Construct a new BList<T, Size>::BList object
 * 
 * @tparam T templated data type to be stored
 * @tparam Size maximum size of data allowed to store in a node in BList
 */
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
}

/**
 * @fn BList<T, Size>::BList
 * @brief Copy Constructor of a new BList<T, Size>::BList object
 * 
 * @tparam T 
 * @tparam Size 
 * @param rhs reference to BList to be copied
 */
template <typename T, unsigned Size>
BList<T, Size>::BList(const BList &rhs){
  //init pointers
  BNode* rhsNodePtr = rhs.head_;
  BNode* lhsNodePtr = nullptr;
  BNode* prevNode = nullptr;

  //loop through rhs Blist
  while(rhsNodePtr){
    try{
      //create new node for an existing node in rhs Blist
      lhsNodePtr = new BNode();
    }
    catch (const std::exception& e){
      throw(BListException(BListException::E_NO_MEMORY, e.what()));
    }
    //update pointers
    lhsNodePtr->prev = prevNode;
    if(prevNode!=nullptr)
      prevNode->next = lhsNodePtr;

    //if first node, assign head_
      if(prevNode == nullptr)
        head_ = lhsNodePtr;

    //assign values
    lhsNodePtr->count = rhsNodePtr->count;
    for(unsigned int i = 0; i < lhsNodePtr->count ;i++){
      lhsNodePtr->values[i] = rhsNodePtr->values[i];
    }
    prevNode = lhsNodePtr;
    rhsNodePtr = rhsNodePtr->next;
  }

  //assign other stuff
  stats_.ArraySize = rhs.GetStats().ArraySize;
  stats_.ItemCount = rhs.GetStats().ItemCount;
  stats_.NodeCount = rhs.GetStats().NodeCount;
  stats_.NodeSize = rhs.GetStats().NodeSize;
  tail_ = lhsNodePtr;
} 

/**
 * @fn BList<T, Size>::~BList
 * @brief Destroy the BList<T, Size>::BList object
 * 
 * @tparam T 
 * @tparam Size 
 */
template <typename T, unsigned Size>
BList<T, Size>::~BList(){
  // destructor
  clear();
}                         

/**
 * @fn BList<T, Size>::operator=(const BList &rhs)
 * @brief assignment operator overload
 * 
 * @tparam T 
 * @tparam Size 
 * @param rhs 
 * @return BList<T, Size>&  returns a copy of BList
 */
template <typename T, unsigned Size>
BList<T, Size>& BList<T, Size>::operator=(const BList &rhs){
  if (this == &rhs)
    return *this;
  
  clear(); // delete existing object/Blist
  //init pointers
  BNode* rhsNodePtr = rhs.head_;
  BNode* lhsNodePtr = nullptr;
  BNode* prevNode = nullptr;

  //loop through rhs Blist
  while(rhsNodePtr){
    try{
      //create new node for an existing node in rhs Blist
      lhsNodePtr = new BNode();
    }
    catch (const std::exception& e){
      throw(BListException(BListException::E_NO_MEMORY, e.what()));
    }
    //update pointers
    lhsNodePtr->prev = prevNode;
    if(prevNode!=nullptr)
      prevNode->next = lhsNodePtr;

    //if first node, assign head_
      if(prevNode == nullptr)
        head_ = lhsNodePtr;

    //assign values
    lhsNodePtr->count = rhsNodePtr->count;
    for(unsigned int i = 0; i < lhsNodePtr->count ;i++){
      lhsNodePtr->values[i] = rhsNodePtr->values[i];
    }
    prevNode = lhsNodePtr;
    rhsNodePtr = rhsNodePtr->next;
  }
  //assign other stuff
  stats_.ArraySize = rhs.GetStats().ArraySize;
  stats_.ItemCount = rhs.GetStats().ItemCount;
  stats_.NodeCount = rhs.GetStats().NodeCount;
  stats_.NodeSize = rhs.GetStats().NodeSize;
  tail_ = lhsNodePtr;
  return *this;
}

/**
 * @fn BList<T, Size>::push_back
 * @brief Push back value to the end of last node, 
 *    arrays will be unsorted, if calling either of these
 * 
 * @tparam T 
 * @tparam Size 
 * @param value input value to push back into BList
 */
template <typename T, unsigned Size>
void BList<T, Size>::push_back(const T& value){
  State ret = FAIL;
  BNode* ptrNode = head_; //start at head node
  BNode* ptrPrevNode;
  int i=0;
  while(ret != SUCCESS){
    //if node got space, assign to node..
    if(ptrNode->count < static_cast<int>(Size)){
      ptrNode->values[ptrNode->count] = value; // assign value to front, based on count
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
        tail_ = ptrNode;
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

/**
 * @fn BList<T, Size>::push_front
 * @brief Push front value to the start of first/head node, 
 *    arrays will be unsorted, if calling either of these
 * 
 * @tparam T 
 * @tparam Size 
 * @param value input value to push front into BList
 */
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

/**
 * @fn BList<T, Size>::insert
 * @brief insert a value into BList, arrays will be sort if calling insert only.
 *    nodes/arrays required to be inserted into that are full,
 *     will be splitted into two equal halves.
 * 
 * @tparam T 
 * @tparam Size 
 * @param value input value to be inserted into BList
 */
template <typename T, unsigned Size>
void BList<T, Size>::insert(const T& value){
  BNode* ptrNode = head_; //start at head node

  //find position to insert, search through every node
  while(ptrNode){
    for(unsigned int j=0; j< Size; j++){
      // base case: first item just push front
      if(stats_.ItemCount == 0){
        push_front(value); //basecase
        return;
      }

      // split node when value < item and node is full
      if(value < ptrNode->values[j] && ptrNode->count == Size){
        //if previous node is full and checking value is at position[0], split previous node
        if(ptrNode->prev!=nullptr){
          if(ptrNode->prev->count == Size && j == 0 && Size != 1){
            SplitNode(ptrNode->prev); //special case of favouring left node
            ptrNode = ptrNode->prev; //loop back to preiovus node
            continue;
          }
          else
            SplitNode(ptrNode); //normal case of splitting
        }
        else
            SplitNode(ptrNode); //normal case of splitting
      }

      // insert(before item) when value less than item and not check unassigned data
      if(value < ptrNode->values[j] && j < ptrNode->count){
        // special case, where after split, node may be full
        if(Size == 1){
          insertAt(ptrNode->values[0], ptrNode->next, 0);
          ptrNode->values[0] = (T)0;
          ptrNode->count--;
          stats_.ItemCount--;
          insertAt(value, ptrNode, j);
          return;
        }
        else{
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
          return;
        }
        else{
          insertAt(value, ptrNode, ptrNode->count); //insert/push front since value > item
          return;
        }
      }

      //check next node[0], if smaller, insert here
      if(j >= ptrNode->count && value < ptrNode->next->values[0]){
        insertAt(value, ptrNode, j);
        return;
      }
      //else need to insert at next node..
    }
    ptrNode = ptrNode->next;
  }
}

/**
 * @fn BList<T, Size>::remove
 * @brief remove data by index
 * 
 * @tparam T 
 * @tparam Size 
 * @param index index of data starting from 0 at first node, 
 *    first element of array. 
 */
template <typename T, unsigned Size>
void BList<T, Size>::remove(int index){
  // check for valid index
  int maxCount = static_cast<int>(stats_.ItemCount + 1);
  
  if(index > maxCount || index < 0)
    throw BListException(BListException::E_BAD_INDEX, "Wrong index provided");
  
  size_t position = index + 1; // check if is within current node

  BNode* curNode = head_; // get node
  
  while (position > curNode->count)
  {
    position -= curNode->count;
    curNode = curNode->next;
  }
  /*loop to move item forward*/
  for (size_t i = position - 1; (i + 1) < curNode->count; ++i)
    curNode->values[i] = curNode->values[i + 1];
  
  stats_.ItemCount--; // decrement

  if (!(--curNode->count))
  {
    if (curNode->prev)
      curNode->prev->next = curNode->next; // remove from list
    else
      head_ = curNode->next;
    
    if (curNode->next)
      curNode->next->prev = curNode->prev; // remove from list
    else
      tail_ = curNode->prev;

    delete curNode; // deallocate

    stats_.NodeCount--; // decrement
  }

}

/**
 * @fn BList<T, Size>::remove_by_value
 * @brief remove data by searching for value
 * 
 * @tparam T 
 * @tparam Size 
 * @param value input value to remove from BList
 */
template <typename T, unsigned Size>
void BList<T, Size>::remove_by_value(const T& value){
  BNode* curNode = head_; // get node
  /*loop thru node*/
  while (curNode)
  {
    for (size_t i = 0; i < curNode->count; ++i)
    {
      if (value == curNode->values[i])
      {
        for (size_t j = i; (j + 1) < curNode->count; ++j)
          curNode->values[j] = curNode->values[j + 1];
        
        stats_.ItemCount--;
        BNode* temp = curNode->prev;
      
        if (!(--(curNode->count)))
        {
          // check if is within current node
          if (curNode->prev)
            curNode->prev->next = curNode->next; 
          else
            head_ = curNode->next;
          // check if is within current node
          if (curNode->next)
            curNode->next->prev = curNode->prev;
          else
            tail_ = curNode->prev;
      
          delete curNode; // deallocate
          stats_.NodeCount--; // decrement
        }
        curNode = temp;
        break;
      }
    }

    if (!curNode)
      curNode = head_;
    else
      curNode = curNode->next; // iterate
  }
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
  while (head_)
  {
    BNode* curNode = head_;
    head_ = head_->next; // get next
    delete curNode; // delete current
  }
  // set all to initial value
  head_ = tail_ = nullptr;
  stats_.NodeCount = stats_.ItemCount = 0;
}          

/**
 * @fn BList<T, Size>::GetStats
 * @brief getter function to access statiscal information
 * 
 * @tparam T 
 * @tparam Size 
 * @return BListStats returns statistical information
 */
template <typename T, unsigned Size>
BListStats BList<T, Size>::GetStats() const{
  return stats_;
}

/**
 * @fn BList<T, Size>::nodesize
 * @brief returns the sizeof a BNode
 * 
 * @tparam T 
 * @tparam Size 
 * @return size_t sizeof a BNode
 */
template <typename T, unsigned Size>
size_t BList<T, Size>::nodesize(void)
{
  return sizeof(BNode);
}

/**
 * @fn BList<T, Size>::GetHead
 * @brief getter function for head node
 * 
 * @tparam T 
 * @tparam Size 
 * @return const BNode* return pointer to head node of BList
 */
template <typename T, unsigned Size>
const typename BList<T, Size>::BNode* BList<T, Size>::GetHead() const
{
  return head_;
}

/**
 * @fn BList<T, Size>::copy_to_
 * @brief helper function to copy data from source 
 *    array to destination array
 * 
 * @tparam T 
 * @tparam Size 
 * @param arrSrc source array
 * @param size size of source array
 * @param arrDest destination array
 */
template <typename T, unsigned Size>
void BList<T, Size>::copy_to_(T* arrSrc, unsigned int size, T* arrDest){
  for(unsigned int i = 0; i<size ; i++){
    arrDest[i] = arrSrc[i];
  }
}

/**
 * @fn BList<T, Size>::insertAt
 * @brief insert value at choosen position of a node
 *    this function asumes that array in node is not full.
 * 
 * @tparam T 
 * @tparam Size 
 * @param value input value to be inserted
 * @param ptrNode input node to insert value to
 * @param insertPos input position in node to insert to
 */
template <typename T, unsigned Size>
void BList<T, Size>::insertAt(T value, BNode* ptrNode, int insertPos){
  if(ptrNode->count < Size){
    //insert and shift the rest up by one
    //shift current values up by one position
    unsigned int position = ptrNode->count;
    for(int i=position-1; i>=insertPos; i--){
      ptrNode->values[i+1] = ptrNode->values[i];
    }
    ptrNode->values[insertPos] = value; //insert value at front of node
    ptrNode->count++; //update count
    stats_.ItemCount++;
    return;
  }
}

/**
 * @fn BList<T, Size>::SearchFor
 * @brief helper function to search for value in node
 * 
 * @tparam T 
 * @tparam Size 
 * @param value input value to search for
 * @param node input node to search
 * @return int index of found value, if value isn't found then return -1
 */
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

/**
 * @fn BList<T, Size>::SplitNode
 * @brief helper function to split a node into two nodes
 *    with equal halves. 
 * 
 * @tparam T 
 * @tparam Size 
 * @param ptrNode input node to be splitted
 */
template <typename T, unsigned Size>
void BList<T, Size>::SplitNode(BNode* ptrNode){
  if(ptrNode == nullptr)
    return;
    
  //create two sub array and assign each with values
  T* arrLeft = new T[Size];
  T* arrRight = new T[Size];
  unsigned int mid = static_cast<unsigned int>(ptrNode->count/2 - 0.5);
  unsigned int arrLeftIndex = 0;
  unsigned int arrRightIndex = 0;
  for(unsigned int i=0; i<ptrNode->count; i++){
    if(i <= mid){
      arrLeft[arrLeftIndex++] = ptrNode->values[i];
    }
    else if(i > mid){
      arrRight[arrRightIndex++] = ptrNode->values[i];
    }
  }

  //create two nodes, link them into BList
  BNode* NewRightNode = new BNode();

  NewRightNode->next = ptrNode->next;
  NewRightNode->prev = ptrNode;
  if(ptrNode->next != nullptr){
    ptrNode->next->prev = NewRightNode;
  }
  ptrNode->next = NewRightNode;
  
  ptrNode->count = arrLeftIndex;
  NewRightNode->count = arrRightIndex;

  copy_to_(arrLeft, Size, ptrNode->values);
  copy_to_(arrRight, Size, NewRightNode->values);
  delete [] arrLeft;
  delete [] arrRight;
  stats_.NodeCount++;
}
