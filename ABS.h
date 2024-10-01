//
// Created by Patrick on 9/28/2024.
//

using namespace std;
// STACK ------------------------------------------------------------------------
template <typename T>
class ABS {
    T* data;
    int size, capacity;
    float scaleFactor = 2.0f;
    void resize(int newCap);

    public:
    ABS();
    ABS(int capacity);
    ABS(const ABS& d);
    ABS& operator=(const ABS& d);
    ~ABS();


    void push(T value);
    T pop();
    T peek();
    int getSize();
    int getMaxCapacity();
    T* getData();
};

    //Constructor and class basic defs
template <typename T>
ABS <T>::ABS() { //Default
    this->size = 0;
    this->capacity = 1;
    data = new T[1];
}
template <typename T>
ABS <T>::ABS(int capacity) { //Constructor with defined capacity
    this->size = 0;
    this->capacity = capacity;
    data = new T[capacity];
}

template <typename T>
ABS <T>::ABS(const ABS& d) { //Copy constructor
    data = new T[d.capacity];
    this->size = d.size;
    this->capacity = d.capacity;
    for (int i = 0; i < this->size; i++) {
        data[i] = d.data[i];
    }
}

template <typename T>
ABS<T>& ABS<T>::operator=(const ABS& d) { //Copy assignment operator
    if (this != &d) {
        return *this;
    }
    delete[] data;
    capacity = d.capacity;
    size = d.size;
    data = new T[capacity];
    for (int i = 0; i < size; i++) {
        data[i] = d.data[i];
    }
    return *this;
}

template <typename T> //Destructor
ABS <T>::~ABS() {
    delete[] data;
}

    //Stack functionalities
template <typename T>
void ABS<T>::resize(int newCap) {
    T* temp = new T[newCap];

    for (int i = 0; i < this->size; i++) {
        temp[i] = data[i];
    }

    delete[] data;
    data = temp;
    this->capacity = newCap;

}

template <typename T>
void ABS<T>::push(T value) { //Push into the stack
    if (size == capacity) {
        resize((int)(capacity * scaleFactor)); // Resize if full to that scalefactor given
    }
    data[size++] = value;
}

template<typename T>
T ABS<T>::pop() {
    if (size > 0) {

    //LIFO
    T lastIn = data[--size];

    if (size < (capacity / scaleFactor)) {
        resize((int)(capacity / scaleFactor));
    }
    return lastIn;
    } else {
        throw std::runtime_error("");
    }
}

template<typename T>
T ABS<T>::peek() {
    if (size > 0) {
    return data[size-1];
    } else {
        throw std::runtime_error("");
    }
}

    // Getters
template<typename T>
int ABS<T>::getSize() {
    return this->size;
}

template<typename T>
int ABS<T>::getMaxCapacity() {
    return this->capacity;
}

template<typename T>
T *ABS<T>::getData() {
    return data;
}


// ------------------------------------------------------------------------


