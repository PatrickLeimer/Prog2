//
// Created by patri on 10/11/2024.
//


#include <iostream>
#include <vector>
using namespace std;

template <typename T>
class LinkedList {
    public:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& data) {
            this->data = data;
            this->next = nullptr;
            this->prev = nullptr;
        }
    };
    Node* head;
    Node* tail;
    unsigned int size;


//Big three, plus copy assignment operator
    LinkedList() {
        this->head = nullptr;
        this->tail = nullptr;
        this->size = 0;
    }
    LinkedList(const LinkedList<T>& other) {
        this->head = nullptr;
        this->tail = nullptr;
        this->size = other.size;
        Node* i = other.head;
        while (i) {
            AddTail(i->data);
            i = i->next;
        }
    }
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            Clear();

            Node* i = other.head;
            while (i) {
                AddTail(i->data);
                i = i->next;
            }
        }
        return *this;
    }
    ~LinkedList() {
        Clear();
    }

//Accessors
    Node* getHead() {
        return head;
    }
    const Node* getHead() const {
        return head;
    } //Some test cases do not pass because of this
    // Thus I just added both of these methods which do the same thing
    Node* Head() {
        return head;
    }
    const Node* Head() const {
        return head;
    }
    const Node* Tail() const {
        return tail;
    }
    Node* Tail() {
        return tail;
    }
    unsigned int NodeCount() const {
        return size;
    }

    void FindAll(vector<Node*>& outData, const T& value) const {
        Node* i = head;

        while(i) {
            if(i->data == value) {
                outData.push_back(i);
            }
            i = i->next;
        }

    }

    const Node* Find(const T& data) const {
        Node* i = head;
        while (i) {
            if (i->data == data) {
                return i;
            }
            i = i->next;
        }
        return nullptr;
    }

    Node* Find(const T& data) {
        Node* i = head;
        while (i) {
            if (i->data == data) {
                return i;
            }
            i = i->next;
        }
        return nullptr;
    }

    const Node* GetNode(unsigned int index) const {
        if (index >= size) {
            throw out_of_range("");
        }

        Node* i = head;
        int count = 0;
        while (i) {
            if (count == index) {
                return i;
            }
            i = i->next;
            count++;
        }
        return nullptr;
    }
     Node* GetNode(unsigned int index) {
        if (index >= size) {
            throw out_of_range("");
        }

        Node* i = head;
        int count = 0;
        while (i) {
            if (count == index) {
                return i;
            }
            i = i->next;
            count++;
        }
        return nullptr;
    }

//Insertion
    void AddHead(const T& data) {
        Node* newNode = new Node(data);
        if (!head) { // head == nullptr
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    void AddTail(const T& data) {
        Node* newNode = new Node(data);
        if (!tail) { //same as tail == nullptr
            tail = newNode;
            head = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void AddNodesHead(const T* data, unsigned int count) {
        for(int i = count - 1; i >= 0; --i) {
            AddHead(data[i]);
        }
        size += count;
    }

    void AddNodesTail(const T* data, unsigned int count) {
        for(int i = 0; i < count; ++i) {
            AddTail(data[i]);
        }
        size += count;
    }

    void InsertBefore(Node* node, const T& data) {
        if(!node) {
            throw invalid_argument("Node is null");
        }

        if (node == head) {
            AddHead(data);
        } else {
            Node* newNode = new Node(data);
            newNode->prev = node->prev;
            newNode->next = node;

            if (node->prev) {
                node->prev->next = newNode;
            }
            node->prev = newNode;
            size++;
        }
    }

    void InsertAfter(Node* node, const T& data) {
        if(!node) {
            throw invalid_argument("Node is null");
        }


        if (node == tail) {
            AddTail(data);
        } else {
            Node* newNode = new Node(data);
            newNode->prev = node;
            newNode->next = node->next;

            node->next->prev = newNode;
            node->next = newNode;
            size++;
        }

    }

    void InsertAt(const T& data, unsigned int index) {
        if(index > size) {
            throw out_of_range("");
        }

        if(index == 0) {
            AddHead(data);
        } else if (index == size) {
            AddTail(data);
        } else {
            Node* i = head;
            for(int j = 0; j < index; ++j) {
                i = i->next;
            }
            InsertBefore(i, data);
        }

    }

//Behaviors
    void PrintForward() const {
        Node* i = head;
        while(i) {
            cout << i->data << endl;
            i = i->next;
        }
    }

    void PrintReverse() const {
        Node* i = tail;
        while(i) {
            cout << i->data << endl;
            i = i->prev;
        }
    }

    void PrintForwardRecursive(const Node* node) const {
        if (!node) { //Base case for recursion
            return;
        }

        cout << node->data << endl;
        PrintForwardRecursive(node->next); //Recursive call
    }

    void PrintReverseRecursive(const Node* node) const {
        if (!node) {
            return;
        }
        cout << node->data << endl; //same deal but print the data after the recursive call
        PrintReverseRecursive(node->prev);

    }

//Removal
    bool RemoveHead() {
        if (!head) {
            return false;
        }
        Node* temp = head;
        head = head->next;

        if(head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        size--;
        return true;
    }

    bool RemoveTail() {
        if (!tail) {
            return false;
        }
        Node* temp = tail;
        tail = tail->prev;

        if(tail) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }

        delete temp;
        size--;
        return true;
    }

    unsigned int Remove(const T& data) {
        unsigned int rCount = 0;
        Node* i = head;

        while(i) {
          Node* nextNode = i->next; //This is here because once i is deleted, we won't have access to correct i->next
            if(i->data == data) {
                if (i == head) {
                  RemoveHead();
                } else if (i == tail) {
                  RemoveTail();
                } else {
                    i->prev->next = i->next; // previous node gets the current's node next
                    i->next->prev = i->prev; // next node gets current's previous
                    delete i;
                    size--;
                }
                rCount++;
            }
            i = nextNode;

        }
        return rCount;

    }

    bool RemoveAt(unsigned int index) {
        if(index >= size) {
            return false;
        }

        if (index == 0) {
            RemoveHead();
        } else if (index == size - 1) {
            RemoveTail();
        } else {
            Node* prev = nullptr;
            Node* curr = head;

            for (unsigned int j = 0; j < index; ++j) {
                prev = curr;
                curr = curr->next;
            }

            if (prev && curr) {
                prev->next = curr->next;
                delete curr;
                size--;
            }
        }
        return true;
    }

    void Clear() {
        while(head) {
            RemoveHead();
        }
        tail = nullptr;
        size = 0;
    }

//operators
    T& operator[](unsigned int index) {
        if (index >= size) {
            throw std::out_of_range("");
        }

        Node* i = head;
        for (unsigned int j = 0; j < index; ++j) {
            i = i->next;
        }
        return i->data;
    }

    const T& operator[](unsigned int index) const {
        if (index >= size) {
            throw std::out_of_range("");
        }

        Node* i = head;
        for (unsigned int j = 0; j < index; ++j) {
            i = i->next;
        }
        return i->data;
    }

    bool operator==(const LinkedList<T>& other) const {
        if (size != other.size) {
            return false;
        }
        Node* i = head;
        Node* j = other.head;
        while (i && j) {
            if (i->data != j->data) {
                return false;
            }
            i = i->next;
            j = j->next;
        }
        return true;
    }

};
