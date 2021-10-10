struct BNode;
struct NextNode;
#include <iostream>;
int main(){
std::cout<<"NewRightNode created at "<<NewRightNode<<std::endl;

  BNode* NextNode = ptrNode->next;
  // BNode* CurNode = ptrNode;
  // BNode* NextNode = const_cast<const BNode*>(ptrNode->next);
  if(NextNode!=nullptr)
    std::cout<<"NextNode with "<<NextNode->values[0]<<" at "<<NextNode<<std::endl;

  ptrNode->next = NewRightNode;
  NewRightNode->prev = ptrNode;
  NewRightNode->next = ptrNode->next;
  
  if(ptrNode->next != nullptr){
    std::cout<<"BREAKPT: ptrNode at "<<ptrNode<<"\tNextNode->prev "<<NextNode->prev<<" with "<<NextNode->prev->values[0]<<std::endl;
    NextNode->prev = NewRightNode;
  }
  // ptrNode = CurNode;
  if(NextNode!=nullptr)
    std::cout<<"BREAKPT: ptrNode at "<<ptrNode<<"\tNextNode->prev "<<NextNode->prev<<" with "<<NextNode->prev->values[0]<<std::endl;



  std::cout<<"Before: ptrNode->count: "<<ptrNode->count<<std::endl;
  
  
  
  
  
  
  return 0;
}


  ptrNode->next = NewRightNode;
  NewRightNode->prev = ptrNode;
  NewRightNode->next = ptrNode->next;
  
  if(ptrNode->next != nullptr){
    std::cout<<"BREAKPT: ptrNode at "<<ptrNode<<"\tNextNode->prev "<<NextNode->prev<<" with "<<NextNode->prev->values[0]<<std::endl;
    NextNode->prev = NewRightNode;
  }

  