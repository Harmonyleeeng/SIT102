#ifndef DYNAMIC_ARRAY_HEADER
#define DYNAMIC_ARRAY_HEADER

#include <cstdlib>
#include <new>

/**
 * @brief A dynamic array struct that contains the size, capacity,
 *        and data pointer used to implement this dynamic structure.
 *
 * @tparam T the type of data to store in the dynamic array
 * @field data a pointer to the data in the dynamic array on the heap
 * @field size the number of elements used in the dynamic array
 * @field capacity the number of elements the dynamic array can hold
 */
template<typename T>
struct dynamic_array
{
  T *data;
  unsigned int size;
  unsigned int capacity;
};

/**
 * @brief Create a new dynamic array with the indicated initial capacity.
 *
 * @tparam T the type of data the array will store
 * @param capacity its initial capacity, with a default value of 50
 * @return dynamic_array<T>* a pointer to the new dynamic array
 */
template<typename T>
dynamic_array<T> *new_dynamic_array(unsigned int capacity = 50)
{
  dynamic_array<T> *result = (dynamic_array<T>*)malloc(sizeof(dynamic_array<T>));
  new(result) dynamic_array<T>();
  if (result == nullptr)
  {
    return result;
  }
  result->data = (T*)malloc(sizeof(T) * capacity);
  for (int i = 0; i < capacity; i++)
  {
      new(&result->data[i]) T();
  }

  result->size = 0;
  if(result->data == nullptr)
  {
    result->capacity = 0;
  }
  else
  {
    result->capacity = capacity;
  }
  return result;
}

/**
 * @brief Free the memory allocated to the dynamic array. Once freed
 *        the data in the array will no longer be accessible.
 *
 * @tparam T the data type of the dynamic array
 * @param array a pointer to the dynamic array to free
 */
template<typename T>
void delete_dynamic_array(dynamic_array<T> *array)
{
  if (!array) return;

  array->size = 0;

  for (int i = 0; i < array->capacity; i++)
  {
      array->data[i].~T();
  }

  array->capacity = 0;

  free(array->data);
  array->data = nullptr;

  free(array);
}

/**
 * @brief Resize the capacity of the dynamic array.
 *
 * If the new capacity is smaller than the current size, the size will be updated to match the new capacity.
 *
 * @tparam T the type of data in the dynamic array
 * @param array the dynamic array to grow
 * @param new_capacity the new capacity of the dynamic array
 * @returns true if this succeeded, or false if it could not reallocate memory
*/
template<typename T>
bool resize(dynamic_array<T> *array, unsigned int new_capacity)
{
  if (!array) return false;

  for(int i = array->capacity - 1; i >= (int)new_capacity; i--)
  {
    array->data[i].~T();
  }

  T* new_data = (T*)realloc(array->data, sizeof(T) * new_capacity);
  if (new_data == nullptr)
  {
    return false;
  }

  for(int i = array->capacity; i < new_capacity; i++)
  {
    new(&array->data[i]) T();
  }

  array->data = new_data;
  array->capacity = new_capacity;

  if (new_capacity < array->size)
  {
    array->size = new_capacity;
  }

  return true;
}

/**
 * @brief Add an element to the end of the dynamic array
 *
 * @tparam T the type of data in the dynamic array
 * @param value the value to add to the end of the dynamic array
 * @param array the dynamic array to add the value to
 */
template<typename T>
bool add(dynamic_array<T> *array, T value)
{
  if (!array) return false;

  if (array->size >= array->capacity && !resize(array, array->capacity * 2 + 1))
  {
    return false;
  }

  array->data[array->size] = value;
  array->size++;

  return true;
}
// Function to remove an element from the dynamic array
bool remove(dynamic_array<product_data> *array, int index)
{
    if (index < 0 || index >= array->size)
    {
        return false; // Index out of bounds
    }

    // Shift elements to the left to fill the gap
    for (int i = index; i < array->size - 1; ++i)
    {
        array->data[i] = array->data[i + 1];
    }

    --array->size; // Decrease the size
    return true;
}

/**
 * @brief Read and return the value of the indicated element from the dynamic array.
 *
 * If the index is out of bounds, the function will return the indicated default value.
 *
 * @tparam T the type of data in the dynamic array
 * @param array the dynamic array to remove the element from
 * @param index the index of the element to remove
 * @param default_value the value to return if the index is out of bounds
*/
template<typename T>
T get(const dynamic_array<T> *array, unsigned int index, T default_value)
{
  if (!array || index >= array->size)
  {
    return default_value;
  }

  return array->data[index];
}

/**
 * @brief Set the value of the indicated element from the dynamic array.
 *
 * If the index is out of bounds, the function will do nothing and return false.
 *
 * @tparam T the type of data in the dynamic array
 * @param array the dynamic array to set the element in
 * @param index the index of the element to change
 * @param value the value to set the element to
 * @returns true when the value is set, or false if this failed
*/
template<typename T>
bool set(dynamic_array<T> *array, unsigned int index, T value)
{
  if (!array || index >= array->size)
  {
    return false;
  }

  array->data[index] = value;
  return true;
}

#endif