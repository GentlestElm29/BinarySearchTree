// Yusuf Pisan pisan@uw.edu
// 15 Jan 2018

// BST class
// Creates a BST to store values
// Uses Node which holds the Data
// Uses templates to store any type of Data
// binarysearchtreee.cpp file is included at the bottom of the .h file
// binarysearchtreee.cpp is part of the template, cannot be compiled separately

#ifndef BST_HPP
#define BST_HPP

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

template <class T>
class BST
{
  // display BST tree in a human-readable format
  friend ostream &operator<<(ostream &Out, const BST &Bst)
  {
    Bst.printSideways(Out, Bst.Root);
    Out << endl;
    Bst.printVertical(Out, Bst.Root);
    return Out;
  }

private:
  // Node for BST
  struct Node
  {
    T Data;
    struct Node *Left;
    struct Node *Right;
  };

  // refer to data type "struct Node" as Node
  using Node = struct Node;

  // root of the tree
  Node *Root{nullptr};

  // height of a Node, nullptr is 0, Root is 1, static, no access to 'this'
  static int getHeight(const Node *N)
  {
   return 0; 
  }

  /**
   * print tree sideways with root on left
                6
            2
                5
        0
                4
            1
                3
   */
  static ostream &printSideways(ostream &Out, const Node *Curr, int Level = 0)
  {
    const static char SP = ' ';
    const static int ReadabilitySpaces = 4;
    if (!Curr)
      return Out;
    printSideways(Out, Curr->Right, ++Level);
    Out << setfill(SP) << setw(Level * ReadabilitySpaces) << SP;
    Out << Curr->Data << endl;
    printSideways(Out, Curr->Left, Level);
    return Out;
  }

  static ostream &centeredPrint(ostream &Out, int Space,
                                const string &Str, char FillChar = ' ')
  {
    auto StrL = static_cast<int>(Str.length());
    int Extra = (Space - StrL) / 2;
    if (Extra > 0)
    {
      Out << setfill(FillChar) << setw(Extra + StrL) << Str;
      Out << setfill(FillChar) << setw(Space - Extra - StrL) << FillChar;
    }
    else
    {
      Out << setfill(FillChar) << setw(Space) << Str;
    }
    return Out;
  }

  /**
   * print tree with the root at top
   *
      _____0______
   __1___      __2___
  3     4     5     6
   *
  **/
  static ostream &printTreeLevel(ostream &Out, queue<const Node *> &Q,
                                 int Width, int Depth, int MaxDepth)
  {
    const static char SP = ' ';
    const static char UND = '_';
    int Nodes = 0;
    int MaxN = pow(2, Depth - 1);
    int SpaceForEachItem = Width * pow(2, MaxDepth - 1) / MaxN; // NOLINT
    string
        Bigspace = string(static_cast<uint64_t>(SpaceForEachItem / 4), SP);
    while (Nodes++ < MaxN)
    {
      const Node *Tp = Q.front();
      Node *TpL = nullptr;
      Node *TpR = nullptr;
      Q.pop();
      string Label = "N";
      if (Tp)
      {
        stringstream Ss;
        Ss << Tp->Data;
        Label = Ss.str();
        TpL = Tp->Left;
        TpR = Tp->Right;
      }
      char Filler = Depth == MaxDepth ? SP : UND;
      if (Depth == MaxDepth)
      {
        centeredPrint(Out, SpaceForEachItem, Label, Filler);
      }
      else
      {
        Out << Bigspace;
        centeredPrint(Out, SpaceForEachItem / 2, Label, Filler);
        Out << Bigspace;
        Q.push(TpL);
        Q.push(TpR);
      }
    }
    Out << endl;
    return Out;
  }

  // helper function for displaying tree sideways, works recursively
  static ostream &printVertical(ostream &Out, Node *Curr)
  {
    const static int WIDTH = 6; // must be even
    if (!Curr)
      return Out << "[__]";
    // figure out the maximum depth which determines how wide the tree is
    int MaxDepth = getHeight(Curr);
    queue<const Node *> Q;
    Q.push(Curr);
    for (int Depth = 1; Depth <= MaxDepth; ++Depth)
    {
      printTreeLevel(Out, Q, WIDTH, Depth, MaxDepth);
    }
    return Out;
  }

  bool isIdentical(Node *Root1, Node *Root2) const
  {
    if (Root1 == nullptr && Root2 == nullptr)
    {
      return true;
    }

    if (Root1 != nullptr && Root2 != nullptr)
    {
      return (
          Root1->Data == Root2->Data &&
          isIdentical(Root1->Left, Root2->Left) &&
          isIdentical(Root1->Right, Root2->Right));
    }

    return false;
  }

  Node *newNode(const T &Item)
  {
    auto Temp = new Node;
    Temp->Data = Item;
    Temp->Left = nullptr;
    Temp->Right = nullptr;

    return Temp;
  }

  void inOrderHelper(Node *Root1, void Visit(const T &Item)) const
  {
    if (Root1 == nullptr)
    {
      return;
    }

    inOrderHelper(Root1->Left, Visit);
    Visit(Root1->Data);
    inOrderHelper(Root1->Right, Visit);
  }

  void preOrderHelper(Node *Root1, void Visit(const T &Item)) const
  {
    if (Root1 != nullptr)
    {
      Visit(Root1->Data);
      preOrderHelper(Root1->Left, Visit);
      preOrderHelper(Root1->Right, Visit);
    }
  }

  void postOrderHelper(Node *Root1, void Visit(const T &Item)) const
  {
    if (Root1 != nullptr)
    {
      postOrderHelper(Root1->Left, Visit);
      postOrderHelper(Root1->Right, Visit);
      Visit(Root1->Data);
    }
  }

  bool inTree(Node *Root1, const T &Item)
  {
    if (Root1 == nullptr)
    {
      return false;
    }

    if (Root1->Data == Item)
    {
      return true;
    }
    bool InLeft = InTree(Root1->Left, Item);

    bool InRight = InTree(Root1->Right, Item);

    return InLeft || InRight;
  }

  void copyTree(Node *CopyNode)
  {
    if (CopyNode != nullptr)
    {
      add(CopyNode->Data);
      copyTree(CopyNode->Left);
      copyTree(CopyNode->Right);
    }
  }

  void destroyTree(Node *Current)
  {
    if (Current != nullptr)
    {
      destroyTree(Current->Left);
      destroyTree(Current->Right);

      delete Current;
    }
  }

  Node *minimumNode(Node *Node1)
  {
    Node *Current = Node1;

    /* loop down to find the leftmost leaf */
    while (Current && Current->Left != nullptr)
    {
      Current = Current->Left;
    }
    return Current;
  }

  Node *deleteNode(Node *Root1, const T &Item)
  {
    if (Root1 == nullptr)
    {
      return Root1;
    }

    if (Item < Root1->Data)
    {
      Root1->Left = deleteNode(Root1->Left, Item);
    }

    else if (Item > Root1->Data)
      Root1->Right = deleteNode(Root1->Right, Item);

    else
    {
      // node with only one child or no child
      if (Root1->Left == nullptr)
      {
        Node *Temp = Root1->Right;
        free(Root1);
        return Temp;
      }

      if (Root1->Right == nullptr)
      {
        Node *Temp = Root1->Left;
        free(Root1);
        return Temp;
      }

      // node with two children: Get the inorder successor (smallest in the right subtree)
      Node *Temp = minimumNode(Root1->right);

      // Copy the inorder successor's content to this node
      Root1->Data = Temp->Data;

      // Delete the inorder successor
      Root1->Right = deleteNode(Root1->Right, Temp->Data);
    }
    return Root1;
  }

  int heightCounter(Node *Root1)
  {
    if (Root1 == nullptr)
    {
      return 0;
    }

    // find the height of each subtree
    int LeftSide = getHeight(Root1->Left);
    int RightSide = getHeight(Root1->Right);

    return 1 + max(LeftSide, RightSide);
  }

  unsigned int nodeCounter(Node *Root1) const
  {
    unsigned int Count = 1;
    if(Root1 == nullptr)
    {
      return 0;
    }

    if (Root1->Left != nullptr)
    {
      Count += nodeCounter(Root1->Left);
    }
    
    if (Root1->Right != nullptr)
    {
      Count += nodeCounter(Root1->Right);
    }

    return Count;
  }

  Node *balanceTree(Node *Root1)
  {
    // Store nodes of given BST in sorted order
    //vector<Node *> Nodes;
    //N is length of original array
    int ArrayLength = numberOfNodes();

    if (ArrayLength == 0)
    {
      return nullptr;
    }

    auto **Nodes = new Node *[ArrayLength];

    storeNodes(Root1, Nodes);

    // Constucts BST from nodes[]
    Node *NewRoot = buildTree(Nodes, 0, ArrayLength - 1);
    delete[] Nodes;

    return NewRoot;
  }

  Node *buildTree(Node **Nodes, int Initial, int End) //All assuming BST is sorted
  {
    //Base Case
    if (Initial > End)
    {
      return nullptr;
    }

    //Make middle element root
    int Mid = (Initial + End) / 2;
    Node *Root1 = Nodes[Mid];

    //Travel through Left then Right subtree
    Root1->Left = buildTree(Nodes, Initial, Mid - 1);
    Root1->Right = buildTree(Nodes, Mid + 1, End);

    return Root1;
  }

  void storeNodes(Node *Root1, Node **Nodes)
  {
    static int Counter = 0;
    //Base case
    if (Root1 == nullptr)
    {
      return;
    }

    //InOrder - Sorted case for BST
    storeNodes(Root1->Left, Nodes);
    Nodes[Counter] = Root1;
    Counter++;
    //Node* NewNode = newNode(Root->Data);
    //Nodes.push_back(Root1);
    storeNodes(Root1->Right, Nodes);
  }

  Node *convertArrayToBst(const T Arr[], int Start, int End)
  {
    if (Start > End)
      return nullptr;

    //make the root the mid of sorted array
    int Mid = (Start + End) / 2;
    Node *Root1 = newNode(Arr[Mid]);
    //construct left
    Root1->Left = sortedArrayToBST(Arr, Start, Mid - 1);
    //construct right
    Root1->Right = sortedArrayToBST(Arr, Mid + 1, End);

    return Root1;
  }

public:
  // constructor, empty tree
  BST()
  {
    Root = nullptr;
  }

  // constructor, tree with root
  explicit BST(const T &RootItem)
  {
    T const &NewItem = RootItem;
    Root = newNode(NewItem);
  }

  // given an array of length n
  // create a tree to have all items in that array
  // with the minimum height (i.e. rebalance)
  // Assignment specification
  // NOLINTNEXTLINE
  BST(const T Arr[], int N)
  {
    convertArrayToBST(Arr, 0, N);
    rebalance();
  }

  // copy constructor
  BST(const BST<T> &Bst)
  {
    Root = nullptr;
    copyTree(Bst.Root);
  }

  // destructor
  virtual ~BST()
  {
    Node *Current = Root;
    if (Current != nullptr)
    {
      destroyTree(Current);
    }

    Root = nullptr;
  }

  // true if no nodes in BST
  bool isEmpty() const
  {
    return Root == nullptr;
  }

  // 0 if empty, 1 if only root, otherwise
  // height of root is max height of subtrees + 1
  int getHeight() const
  {
    return heightCounter(Root);
  }

  // Number of nodes in BST
  int numberOfNodes() const
  {
    return nodeCounter(Root);
  }

  // add a new item, return true if successful
  bool add(const T &Item)
  {
    Node *Current = Root;

    Node *Parent = nullptr;

    if (Root == nullptr)
    {
      Root = newNode(Item);
      return true;
    }

    while (Current != nullptr)
    {
      Parent = Current;
      if (Item < Current->Data)
      {
        Current = Current->Left;
      }
      else
      {
        Current = Current->Right;
      }
    }

    if (Item < Parent->Data)
    {
      Parent->Left = newNode(Item);
      return true;
    }

    if(Item > Parent->Data)
    {
      Parent->Right = newNode(Item);
      return true;
    }

    return false;//Item == Parent->Data
  }

  // remove item, return true if successful
  bool remove(const T &Item)
  {
    return deleteNode(Root, Item);
  }

  // true if item is in BST
  bool contains(const T &Item) const
  {
    return inTree(Root, Item);
  }

  // inorder traversal: left-root-right
  // takes a function that takes a single parameter of type T
  void inOrderTraverse(void Visit(const T &Item)) const
  {
    inOrderHelper(Root, Visit);
  }

  // preorder traversal: root-left-right
  void preOrderTraverse(void Visit(const T &Item)) const
  {
    preOrderHelper(Root, Visit);
  }

  // postorder traversal: left-right-root
  void postOrderTraverse(void Visit(const T &Item)) const
  {
    postOrderHelper(Root, Visit);
  }

  // create dynamic array, copy all the items to the array
  // and then read the array to re-create this tree from scratch
  // so that resulting tree is balanced
  void rebalance()
  {
    Root = balanceTree(Root);
  }

  // delete all nodes in tree
  void clear()
  {
    destroyTree(Root);
  }

  // trees are equal if they have the same structure
  // AND the same item values at all the nodes
  bool operator==(const BST<T> &Other) const
  {
    return isIdentical(Root, Other.Root);
  }

  // not == to each other
  bool operator!=(const BST<T> &Other) const
  {
    return !(isIdentical(Root, Other.Root));
  }
};

#endif // BST_HPP
