#include <cstdio>
#include "dynamic-array.hpp"

int main()
{
  // Create a dynamic array of int
  // and initialise it to a new dynamic array of 10 elements
  dynamic_array<int> *array = new_dynamic_array<int>(10);

  // Print the size and capacity of the array
  printf("size: %d, capacity: %d\n", array->size, array->capacity);

  // Add 15 values to the array
  for(int i = 0; i < 15; i++)
  {
    add(array, i);
  }

  // Reprint the size and capacity of the array after adding
  printf("size: %d, capacity: %d\n", array->size, array->capacity);

  // Print and update the values in the array, using the get and set functions
  for(int i = 0; i < array->size; i++)
  {
    printf("array[%d] =  %d\n", i, get(array, i, -1));
    set(array, i, i * 2);
  }

  // Attempt to access an element out of bounds using get
  printf("array[99] = %d\n", get(array, 99, -1));

  // Attempt to access an element out of bounds using set
  if (set(array, 99, 99))
  {
    printf("array[99] = %d\n", get(array, 99, -1));
  }
  else
  {
    printf("Failed to set array[99]\n");
  }

  printf("Before resize - size: %d, capacity: %d\n", array->size, array->capacity);
  // Change the size of the array
  resize(array, 5);

  printf("After resize - size: %d, capacity: %d\n", array->size, array->capacity);

  for(int i = 0; i < 20; i++)
  {
    printf("array[%d] =  %d\n", i, get(array, i, -1));
  }

  // Free the array and ensure we do not have a dangling pointer
  delete_dynamic_array(array);
  array = nullptr;

  return 0;
}


//Task 9: Product Database
#include "splashkit.h"
#include <string.h>
#include <stdio.h>

#define MAX_PRODUCTS 100

// Define a structure to represent a product
typedef struct
{
    char name[50];
    double cost_price;
    double sale_price;
    int quantity_on_hand;
} product_data;

// Define the store_date structure
typedef struct
{
    product_data products[MAX_PRODUCTS];
    int num_products;
    double total_sales;
    double total_profit;
} store_date; // change to store_date

// Function prototypes
void add_product(store_date &store_date);
void delete_product(store_date &store_date);
void update_product(store_date &store_date);
void sell_product(store_date &store_date);
void print_status(const store_date &store_date);
int search_products(const store_date &store_date, const char *name, int matching_indices[], int &match_count);
void update_product_details(product_data &product);

int main()
{
    store_date store_date = {{}, 0, 0.0, 0.0};

    int choice;
    string confirm_quit;
    do
    {
        // Display menu
        printf("\nProduct Database Menu:\n");
        printf("1. Add new product\n");
        printf("2. Delete product\n");
        printf("3. Update product\n");
        printf("4. Sell product\n");
        printf("5. Print status\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left by scanf

        switch (choice)
        {
        case 1:
            add_product(store_date);
            break;
        case 2:
            delete_product(store_date);
            break;
        case 3:
            update_product(store_date);
            break;
        case 4:
            sell_product(store_date);
            break;
        case 5:
            print_status(store_date);
            break;
        case 6:
            printf("Goodbye.\n");

            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
    return 0;
}

// Function to add a new product
void add_product(store_date &store_date)
{
    if (store_date.num_products >= MAX_PRODUCTS)
    {
        printf("Error: Cannot add more products. Maximum limit reached.\n");
        return;
    }

    product_data new_product;
    printf("Enter product name: ");
    fgets(new_product.name, sizeof(new_product.name), stdin);
    new_product.name[strcspn(new_product.name, "\n")] = '\0'; // Remove newline character

    printf("Enter cost price: ");
    scanf("%lf", &new_product.cost_price);
    printf("Enter sale price: ");
    scanf("%lf", &new_product.sale_price);
    printf("Enter quantity on hand: ");
    scanf("%d", &new_product.quantity_on_hand);

    store_date.products[store_date.num_products] = new_product;
    store_date.num_products++;
    printf("Product added successfully.\n");
}

// Function to search for matching products
int search_products(const store_date &store_date, const char *name, int matching_indices[], int &match_count)
{
    match_count = 0;

    for (int i = 0; i < store_date.num_products; i++)
    {
        if (strstr(store_date.products[i].name, name) != NULL) // Check if name contains the search text
        {
            matching_indices[match_count] = i;
            match_count++;
        }
    }

    return match_count > 0; // Return 1 if matches found, 0 otherwise
}

// Function to delete a product
void delete_product(store_date &store_date)
{
    char name[50];
    printf("Enter product name to search for: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int matching_indices[MAX_PRODUCTS];
    int match_count = 0;

    if (search_products(store_date, name, matching_indices, match_count) == -1)
    {
        printf("No products found matching '%s'.\n", name);
        return;
    }

    // Display the list of matching products
    printf("Matching products:\n");
    for (int i = 0; i < match_count; i++)
    {
        int index = matching_indices[i];
        printf("%d. %s (Cost: %.2f, Sale: %.2f, Quantity: %d)\n",
               i + 1,
               store_date.products[index].name,
               store_date.products[index].cost_price,
               store_date.products[index].sale_price,
               store_date.products[index].quantity_on_hand);
    }

    // Ask the user to select a product
    int choice;
    printf("Enter the number of the product to delete (1-%d): ", match_count);
    scanf("%d", &choice);
    getchar(); // Consume the newline character left by scanf

    if (choice < 1 || choice > match_count)
    {
        printf("Invalid choice. No changes made.\n");
        return;
    }

    // Remove the selected product
    int index_to_delete = matching_indices[choice - 1];
    for (int i = index_to_delete; i < store_date.num_products - 1; i++)
    {
        store_date.products[i] = store_date.products[i + 1];
    }
    store_date.num_products--;

    printf("Product deleted successfully.\n");
}

// Function to update product details
void update_product(store_date &store_date)
{
    char name[50];
    printf("Enter product name to update: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int matching_indices[MAX_PRODUCTS];
    int match_count = 0;

    if (search_products(store_date, name, matching_indices, match_count) == -1)
    {
        printf("No products found matching '%s'.\n", name);
        return;
    }

    // Display the list of matching products
    printf("Matching products:\n");
    for (int i = 0; i < match_count; i++)
    {
        int index = matching_indices[i];
        printf("%d. %s (Cost: %.2f, Sale: %.2f, Quantity: %d)\n",
               i + 1,
               store_date.products[index].name,
               store_date.products[index].cost_price,
               store_date.products[index].sale_price,
               store_date.products[index].quantity_on_hand);
    }

    // Ask the user to select a product
    int choice;
    printf("Enter the number of the product to update (1-%d): ", match_count);
    scanf("%d", &choice);
    getchar(); // Consume the newline character left by scanf

    if (choice < 1 || choice > match_count)
    {
        printf("Invalid choice. No changes made.\n");
        return;
    }

    // Get the selected product
    int index_to_update = matching_indices[choice - 1];
    update_product_details(store_date.products[index_to_update]);
}

void update_product_details(product_data &product)
{
    int choice;
    do
    {
        printf("\nProduct details:\n");
        printf("Name: %s\n", product.name);
        printf("Cost Price: %.2f\n", product.cost_price);
        printf("Sale Price: %.2f\n", product.sale_price);
        printf("Quantity on Hand: %d\n", product.quantity_on_hand);

        printf("\nSelect an option to update:\n");
        printf("1. Update name\n");
        printf("2. Update cost price\n");
        printf("3. Update sale price\n");
        printf("4. Update quantity on hand\n");
        printf("5. Quit update\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left by scanf

        switch (choice)
        {
        case 1:
            printf("Enter new name: ");
            fgets(product.name, sizeof(product.name), stdin);
            product.name[strcspn(product.name, "\n")] = '\0'; // Remove newline character
            break;
        case 2:
            printf("Enter new cost price: ");
            scanf("%lf", &product.cost_price);
            getchar(); // Consume the newline character left by scanf
            break;
        case 3:
            printf("Enter new sale price: ");
            scanf("%lf", &product.sale_price);
            getchar(); // Consume the newline character left by scanf
            break;
        case 4:
            printf("Enter new quantity on hand: ");
            scanf("%d", &product.quantity_on_hand);
            getchar(); // Consume the newline character left by scanf
            break;
        case 5:
            printf("Exiting update menu.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

// Function to sell a product
void sell_product(store_date &store_date)
{
    char name[50];
    printf("Enter product name to sell: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int matching_indices[MAX_PRODUCTS];
    int match_count = 0;

    if (search_products(store_date, name, matching_indices, match_count) == -1)
    {
        printf("No products found matching '%s'.\n", name);
        return;
    }

    // Display the list of matching products
    printf("Matching products:\n");
    for (int i = 0; i < match_count; i++)
    {
        int index = matching_indices[i];
        printf("%d. %s (Cost: %.2f, Sale: %.2f, Quantity: %d)\n",
               i + 1,
               store_date.products[index].name,
               store_date.products[index].cost_price,
               store_date.products[index].sale_price,
               store_date.products[index].quantity_on_hand);
    }

    // Ask the user to select a product
    int choice;
    printf("Enter the number of the product to sell (1-%d): ", match_count);
    scanf("%d", &choice);
    getchar(); // Consume the newline character left by scanf

    if (choice < 1 || choice > match_count)
    {
        printf("Invalid choice. No changes made.\n");
        return;
    }

    // Get the selected product
    int index_to_sell = matching_indices[choice - 1];
    product_data &product = store_date.products[index_to_sell];

    // Ask the user how many units to sell
    int quantity_to_sell;
    printf("Enter the number of units to sell from '%s' (Current quantity: %d): ",
           product.name, product.quantity_on_hand);
    scanf("%d", &quantity_to_sell);

    if (quantity_to_sell <= 0)
    {
        printf("Invalid quantity. No changes made.\n");
        return;
    }

    if (quantity_to_sell > product.quantity_on_hand)
    {
        printf("Error: Cannot sell more units than available. No changes made.\n");
        return;
    }

    // Update the quantity on hand
    product.quantity_on_hand -= quantity_to_sell;

    // Calculate the sale value and profit
    double sale_value = product.sale_price * quantity_to_sell;
    double profit = sale_value - (product.cost_price * quantity_to_sell);

    // Update the store_date's sales total and profit
    store_date.total_sales += sale_value;
    store_date.total_profit += profit;

    printf("Sold %d units of '%s'.\n", quantity_to_sell, product.name);
    printf("Sale value: %.2f, Profit: %.2f\n", sale_value, profit);
    printf("Updated quantity of '%s': %d\n", product.name, product.quantity_on_hand);
}

// Function to print the store_date status
void print_status(const store_date &store_date)
{
    printf("store_date Status:\n");
    printf("Total sales: %.2f\n", store_date.total_sales);
    printf("Total profit: %.2f\n", store_date.total_profit);
    printf("Number of products: %d\n", store_date.num_products);

    double stock_value = 0.0;
    int low_stock_count = 0;

    for (int i = 0; i < store_date.num_products; i++)
    {
        stock_value += store_date.products[i].cost_price * store_date.products[i].quantity_on_hand;
        if (store_date.products[i].quantity_on_hand < 10)
        {
            low_stock_count++;
        }
    }

    printf("Value of stock on hand: %.2f\n", stock_value);
    printf("Number of products with low stock (less than 10): %d\n", low_stock_count);
}


