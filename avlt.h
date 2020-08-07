/*avlt.h*/

//
// Threaded AVL tree
//

#pragma once

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

template<typename KeyT, typename ValueT>
class avlt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded; // true => Right is a thread, false => non-threaded
    int    Height;     // height of tree rooted at this node
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* internalState;

  void _copying(NODE* from){
  
      if(from == NULL){
          return;
      }
      else{
      
        insert_(from->Key, from->Value);
        _copying(from->Left);
        if(from->isThreaded == false){
        
            _copying(from->Right);
        }
        
      }
  }
  
  void _destruct(NODE* curr){
      
      if(curr == nullptr){
          return;
      }
      else{
          _destruct(curr->Left);
          if(!curr->isThreaded){
              _destruct(curr->Right);
          }
          delete curr;
          Size--;
      }
  }
  
  int _GetHeight(NODE* cur){
      
      if(cur == nullptr){
          return -1;
      }
      else{
          return cur->Height;
      }
  }

  void RightRotate(NODE* Parent, NODE* N){
  
      NODE* L = N->Left;
      NODE* A = L->Left;
      NODE* B;
      NODE* C;
      if(L->isThreaded == true)
          B = NULL;
      else
          B = L->Right;

      if(N->isThreaded == true)
          C = NULL;
      else
          C = N->Right;

      
      L->Right = N;
      L->isThreaded = false;
      N->Left = B;
      
      if(Parent == NULL){
          Root = L;
      }
      else if(Parent->Key < L->Key){
          Parent->Right = L;
      }
      else{
          Parent->Left = L;
      }
      
      N->Height = 1 + max(_GetHeight(B), _GetHeight(C));
      L->Height = 1 + max(_GetHeight(A), N->Height);
  }
  
  void LeftRotate(NODE* Parent, NODE* N){
  
      NODE* R = N->Right;
      NODE* A = N->Left;
      NODE* B = R->Left;
      NODE* C;
      if(R->isThreaded == true)
          C = NULL;
      else
          C = R->Right;
      
      R->Left = N;
      if(B == NULL){
          N->Right = R;
          N->isThreaded = true;
      }else{
          N->Right = B;
      }
      
      if(Parent == NULL){
          Root = R;
      }
      else if(Parent->Key < R->Key){
          Parent->Right = R;
      }
      else{
          Parent->Left = R;
      }
      
      N->Height = 1 + max(_GetHeight(A), _GetHeight(B));
      R->Height = 1 + max(_GetHeight(C), N->Height);
  }
  
  void Print(NODE* curr, ostream& output) const {
      if(curr == NULL){
          ;
      }
      else{
      
          Print(curr->Left,output);
          if(curr->isThreaded == true && curr->Right != NULL){
              output << "(" << curr->Key << "," << curr->Value << "," << curr->Height << "," << curr->Right->Key << ")" << endl;
              
          }
          else if(curr->isThreaded == false) {
              output << "(" << curr->Key << "," << curr->Value << "," << curr->Height << ")" << endl;
          }
          else if(curr->isThreaded == true && curr->Right == NULL)
              output << "(" << curr->Key << "," << curr->Value << "," << curr->Height << ")" << endl;
              
          if(curr->isThreaded != true)
              Print(curr->Right,output);
      }
  }
  
public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  avlt()
  {
    Root = nullptr;
    Size = 0;
    internalState = nullptr;
  }

  //
  // copy constructor
  //
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  //
  avlt (const avlt& other)
  {
    //
    // TODO
    //
    Size = 0;
    Root = NULL;
    internalState = NULL;
    _copying(other.Root);
    internalState = other.internalState;
  }

    //
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~avlt()
  {
    //
    // TODO
    //
    _destruct(Root);
    Root = NULL;
    Size = 0;
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  // NOTE: makes an exact copy of the "other" tree, such that making the
  // copy requires no rotations.
  //
  avlt& operator=(const avlt& other)
  {
    //
    // TODO:
    //
    if(other.Root == NULL){
        return *this;
    }
    _destruct(Root);
    Size = 0;
    Root = NULL;
    _copying(other.Root);
    Size = other.Size;
    
    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
    //
    // TODO
    //
    _destruct(Root);
    Root = NULL;
    Size = 0;
  }

  //
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1)
  //
  int size() const
  {
    return Size;
  }

  //
  // height:
  //
  // Returns the height of the tree, -1 if empty.
  //
  // Time complexity:  O(1)
  //
  int height() const
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
  }

  //
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) worst-case
  //
  bool search(KeyT key, ValueT& value) const
  {
    //
    // TODO
    //
    NODE* curr = Root;
    while(curr != NULL){
    
        if(key < curr->Key){
            curr = curr -> Left;
        }
        else if(key > curr->Key && curr->isThreaded!=true){
        
            curr = curr -> Right;
        }
        else if(key > curr->Key && curr->isThreaded==true){
        
            curr = nullptr;
        }
        else if(curr->Key == key){
        
            value = curr->Value;
            return true;
        }
    
    }
    
    return false;
  }

  //
  // range_search
  //
  // Searches the tree for all keys in the range [lower..upper], inclusive.
  // It is assumed that lower <= upper.  The keys are returned in a vector;
  // if no keys are found, then the returned vector is empty.
  //
  // Time complexity: O(lgN + M), where M is the # of keys in the range
  // [lower..upper], inclusive.
  //
  // NOTE: do not simply traverse the entire tree and select the keys
  // that fall within the range.  That would be O(N), and thus invalid.
  // Be smarter, you have the technology.
  //
  vector<KeyT> range_search(KeyT lower, KeyT upper)
  {
    vector<KeyT>  keys;
    
    //
    // TODO
    //
    
    NODE* cur = Root;
    NODE* prev = NULL;
    
    if(cur == NULL){
        
        return keys;
    }
    while(cur != NULL){
        
        if(lower < cur->Key){
            prev = cur;
            cur = cur -> Left;
        }
        else if(lower > cur->Key && cur->isThreaded!=true){
            prev = cur;
            cur = cur -> Right;
        }
        else if(lower > cur->Key && cur->isThreaded==true){
            prev = cur;
            cur = nullptr;
        }
        else if(cur->Key == lower){
            prev = cur;
            cur = nullptr;
        }
    
    }
    if(lower>prev->Key){
        
        prev = prev ->Right;
    }
        
    while(prev!= NULL && prev->Key < upper){
        
        keys.push_back(prev->Key);
        if(prev->isThreaded == true)
            prev = prev->Right;
        else{
            prev = prev ->Right;
            while(prev->Left != NULL){
                prev = prev->Left;
            }
        }
        
    }
    
    
    return keys;
  }

  //
  // insert_
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert_(KeyT key, ValueT value)
  {
    //
    // TODO
    //
    
    NODE* curr = Root;
    NODE* prev = NULL;
    stack<NODE*> nodes;
    
    while(curr != NULL){
        nodes.push(curr);
        if(key < curr->Key){
            prev = curr;
            curr = curr -> Left;
        }
        else if(key > curr->Key && curr->isThreaded!=true){
            prev = curr;
            curr = curr -> Right;
        }
        else if(key > curr->Key && curr->isThreaded==true){
            prev = curr;
            curr = nullptr;
        }
        else if(curr->Key == key){
            return;
        }
    
    }
    
    NODE* temp = new NODE;
    temp->isThreaded = true;
    temp->Key = key;
    temp->Value = value;
    temp->Height = 0;
    temp->Left = NULL;
    if(prev == NULL){
    
        this->Root = temp;
        temp->Right = NULL;
    }
    else if(key<prev->Key){
    
        prev -> Left = temp;
        temp -> Right = prev;
    }
    else if(key>prev->Key){
    
        temp->Right = prev -> Right;
        prev->Right = temp;
         prev->isThreaded = false;
        
    }
    
    Size++;

    while(!nodes.empty()){
    
        curr = nodes.top();
        nodes.pop();
        
        int HL,HR;
        
        if(curr->Left == nullptr){
            HL = -1;
        }else{
            HL = curr->Left->Height;
        }
        
        if(curr->Right == nullptr){
            HR = -1;
        }else if(curr->Right != nullptr && curr->isThreaded == true){
            HR = -1;
        }else{
            HR = curr->Right->Height;
        }
        
        curr->Height = 1 + max(HL,HR);
    }
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.  Rotations are performed
  // as necessary to keep the tree balanced according to AVL definition.
  //
  // Time complexity:  O(lgN) worst-case
  //
  void insert(KeyT key, ValueT value)
  {
    //
    // TODO
    //
    
    NODE* prev = nullptr;
    NODE* cur = Root;
    stack<NODE*> nodes;
    
    while(cur!= nullptr){
    
        if(key == cur->Key){
            return;
        }
        
        nodes.push(cur);
        if(key < cur->Key){
            prev = cur;
            cur = cur -> Left;
        }
        else if(key > cur->Key && cur->isThreaded!=true){
            prev = cur;
            cur = cur -> Right;
        }
        else if(key > cur->Key && cur->isThreaded==true){
            prev = cur;
            cur = nullptr;
        }
    }
    
    NODE* temp = new NODE;
    temp->isThreaded = true;
    temp->Key = key;
    temp->Value = value;
    temp->Left = NULL;
    temp->Height = 0;
    if(prev == NULL){
    
        this->Root = temp;
        temp->Right = NULL;
    }
    else if(key<prev->Key){
    
        prev -> Left = temp;
        temp -> Right = prev;
    }
    else if(key>prev->Key){
    
        temp->Right = prev -> Right;
        prev->Right = temp;
         prev->isThreaded = false;
        
    }
    
    Size++;
    
    while(!nodes.empty()){
    
        cur = nodes.top();
        nodes.pop();
        
        int HL,HR,newH;
        
        if(cur->Left == nullptr){
            HL = -1;
        }else{
            HL = cur->Left->Height;
        }
        
        if(cur->Right == nullptr){
            HR = -1;
        }else if(cur->Right != nullptr && cur->isThreaded == true){
            HR = -1;
        }else{
            HR = cur->Right->Height;
        }
        newH = 1 + max(HL,HR);
        
        if(newH == cur->Height){
            break;
        }
        else if((HL-HR == 0) || (HL-HR == 1) || (HL-HR == -1)){
            cur->Height = newH;
            continue;
        }else{
            NODE* parent;
            if(nodes.empty()){
                parent = NULL;
            }
            else{
                parent = nodes.top();
            }
            if(cur->Key>key){                   //cases 1-2
                if(key<cur->Left->Key){             //RR rotation
                    RightRotate(parent, cur);
                }
                else{                               //LR-RR rotation
                    LeftRotate(cur, cur->Left);
                    RightRotate(parent,cur);
                }
            }
            else{                               //cases 3-4
                if(key>cur->Right->Key){            //LR rotation
                    LeftRotate(parent,cur);
                }
                else{                               //RR-LR rotation
                    RightRotate(cur,cur->Right);
                    LeftRotate(parent,cur);
                }
            }
            break;
        }
        
    }
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) worst-case
  //
  ValueT operator[](KeyT key) const
  {
    //
    // TODO
    //
    
    NODE* curr = Root;
    while(curr != NULL){
    
        if(curr->Key == key){
            return curr->Value;
        }
        else if(key < curr->Key){
            curr = curr -> Left;
        }
        else if(key > curr->Key && curr->isThreaded!=true){
        
            curr = curr -> Right;
        }
        else if(key > curr->Key && curr->isThreaded==true){
        
            return ValueT{ };
        }
    
    }
    
    return ValueT{ };
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) worst-case
  //
  KeyT operator()(KeyT key) const
  {
    //
    // TODO
    //

    NODE* curr = Root;
    while(curr != NULL){
    
        if(curr->Key == key){
        
            if(curr->Right != NULL){
                return curr->Right->Key;
            }
            break;
        }
        else if(key < curr->Key){
            curr = curr -> Left;
        }
        else if(key > curr->Key && curr->isThreaded!=true){
        
            curr = curr -> Right;
        }
        else if(key > curr->Key && curr->isThreaded==true){
        
            return KeyT{ };
        }
    
    }

    return KeyT{ };
  }

  //
  // %
  //
  // Returns the height stored in the node that contains key; if key is
  // not found, -1 is returned.
  //
  // Example:  cout << tree%12345 << endl;
  //
  // Time complexity:  O(lgN) worst-case
  //
  int operator%(KeyT key) const
  {
    //
    // TODO
    //
    
    NODE* curr = Root;
    while(curr != NULL){
    
        if(curr->Key == key){
            return curr->Height;
        }
        else if(key < curr->Key){
            curr = curr -> Left;
        }
        else if(key > curr->Key && curr->isThreaded!=true){
        
            curr = curr -> Right;
        }
        else if(key > curr->Key && curr->isThreaded==true){
        
            return -1;
        }
    
    }
    
    return -1;
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) worst-case
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    //
    // TODO
    //
    if((Root == NULL)){
        return;
    }
    NODE* curr = Root;
    
    
    while(curr->Left != NULL){
    
        curr = curr -> Left;
        
    }
    internalState = curr;
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) worst-case
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    //
    // TODO
    //

    if(internalState == NULL){
        return false;
    }
    
    key = internalState->Key;
    if(internalState->isThreaded == true){
        internalState = internalState -> Right;
    } else{
        internalState = internalState->Right;
        while(internalState->Left != NULL){
            internalState = internalState->Left;
        }
    }

    return true;
  }

  //
  // dump
  //
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* AVLT ***********************" << endl;

    output << "** size: " << this->size() << endl;
    output << "** height: " << this->height() << endl;

    //
    // inorder traversal, with one output per line: either
    // (key,value,height) or (key,value,height,THREAD)
    //
    // (key,value,height) if the node is not threaded OR thread==nullptr
    // (key,value,height,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //

    //
    // TODO
    //
    if((Root == NULL)){
        output << "**************************************************" << endl;
        return;
    }
    
    Print(Root, output);
    output << "**************************************************" << endl;
  }
    
};


