//
// Created by Patrick on 9/30/2024.

// QUEUE ------------------------------------------------------------------------
template <typename T>
class ABQ {
    T* data;
    int size, capacity;
    float scaleFactor = 2.0f;
public:
    ABQ();
    ABQ(int capacity);
    ABQ(const ABQ &d);
    ABQ& operator=(const ABQ& d);
    ~ABQ();

    void resize(int newCap);
    void enqueue(T value);
    T dequeue();
    T peek();
    int getSize();
    int getMaxCapacity();
    T* getData();

};
//Constructor and class basic defs

template <typename T>
ABQ <T>::ABQ() { //Default
    this->size = 0;
    this->capacity = 1;
    data = new T[1];
}

template <typename T>
ABQ <T>::ABQ(int capacity) { //Constructor with defined capacity
    this->size = 0;
    this->capacity = capacity;
    data = new T[capacity];
}

template <typename T>
ABQ <T>::ABQ(const ABQ& d) { //Copy constructor
    data = new T[d.capacity];
    this->size = d.size;
    this->capacity = d.capacity;
    for (int i = 0; i < this->size; i++) {
        data[i] = d.data[i];
    }
}

template <typename T>
ABQ<T>& ABQ<T>::operator=(const ABQ& d) { //Copy assignment operator
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

//Destructor
template <typename T>
ABQ <T>::~ABQ()  {
    delete[] data;
}

//Queue functionalities
template <typename T>
void ABQ<T>::resize(int newCap) {
    T* temp = new T[newCap];

    for (int i = 0; i < this->size; i++) {
        temp[i] = data[i];
    }

    delete[] data;
    data = temp;
    this->capacity = newCap;

}

template <typename T>
void ABQ<T>::enqueue(T value) {
  if(size == capacity) {
    resize((int)(size * scaleFactor));
  }
  data[size++] = value;
}

template <typename T>
T ABQ<T>::dequeue() {
  if(size == 0) {
    throw std::runtime_error("");
  }

  T top = data[0];

  for (int i = 1; i < size; ++i) {
      data[i - 1] = data[i];
  }
  size--;

  if (size < capacity / scaleFactor) {
      resize((int)(capacity / scaleFactor));
  }

  return top;

}
// T peek();
template <typename T>
T ABQ<T>::peek() {
    if (size > 0) {
    return data[size-1];
    } else {
        throw std::runtime_error("");
    }
}


// Getters
template <typename T>
int ABQ<T>::getSize() {
    return this->size;
}

template <typename T>
int ABQ<T>::getMaxCapacity() {
    return this->capacity;
}

template <typename T>
T *ABQ<T>::getData() {
    return data;
}