   std::cout<<"BREAKPT - insert START - value: "<<value<<"\n";
  // State ret = FAIL;
  BNode* ptrNode = head_; //start at head node
  
  //search through every node
  for(int i=0; i<stats_.NodeCount; i++){
    //search through every data in node
    if(ptrNode->count == 0){
      //basecase
      ptrNode->values[0] = value; //insert value at front of node
    std::cout<<"BREAKPT -assign"<<value<<"\n";
      ptrNode->count++; //update count
    
      stats_.ItemCount++;
      return;
    }
    for(unsigned int j=0; j< ptrNode->count;){
      if(value < ptrNode->values[j]){
        //found insert position
        if(ptrNode->count < Size){
          //insert and shift the rest up by one
          //shift current values up by one position
          unsigned int position = ptrNode->count;
          int insertPos = j;
          for(int i=position-1; i>=insertPos; i--){
            ptrNode->values[i+1] = ptrNode->values[i];
          }
          printf("insertpos: %i", insertPos);
          ptrNode->values[insertPos] = value; //insert value at front of node
    std::cout<<"BREAKPT -assign"<<value<<"\n";
          ptrNode->count++; //update count
          stats_.ItemCount++;
          // ret = SUCCESS;
          return;
        }
        else if(Size > 2){
          //Split node then insert...
          //create two sub array and assign each with values
          T* arrLeft = new T[Size];
          T* arrRight = new T[Size];
          unsigned int mid = ptrNode->count/2;
          unsigned int arrLeftIndex = 0;
          unsigned int arrRightIndex = 0;
          for(unsigned int i=0; i<ptrNode->count; i++){
            if(i <= mid)
              arrLeft[arrLeftIndex++] = ptrNode->values[i];
            else if(i > mid)
              arrRight[arrRightIndex++] = ptrNode->values[i];
          }
          //create two nodes, link them into BList
          BNode* NewLeftNode = new BNode();
          BNode* NewRightNode = new BNode();
        std::cout<<"BREAKPT - new nodes created\n";
          BNode* PrevNode = ptrNode->prev;
          BNode* NextNode = ptrNode->next;
          PrevNode->next = NewLeftNode;
          NewLeftNode->prev = PrevNode;
          NewLeftNode->next = NewRightNode;
          NewRightNode->prev = NewLeftNode;
          NewRightNode->next = NextNode;
          NextNode->prev = NewRightNode;
          NewLeftNode->count = arrLeftIndex;
          NewRightNode->count = arrRightIndex;
          copy_to_(arrLeft, Size, NewLeftNode->values);
          copy_to_(arrRight, Size, NewRightNode->values);
          
          //delete splited node
          delete [] ptrNode;
          ptrNode = NewLeftNode;
          j = 0; //repeat loop
          continue;
        }
      }
      j++;
    }
    std::cout<<"BREAKPT - insert later - ptrNode: "<<ptrNode<<"\n";
    if(ptrNode == tail_){
      printf("at tail: %p == %p\n", (void*)ptrNode, (void*)tail_);
      push_back(value);
    }
    else
      ptrNode = ptrNode->next;
  }
  