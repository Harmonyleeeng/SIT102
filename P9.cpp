#include "dynamic-array.hpp"
#include <cstdio>
#include <string.h>

// Define a structure to represent a product
typedef struct
{
    char name[50];
    double cost_price;
    double sale_price;
    int quantity_on_hand;
} product_data;

// Define the store_date structure with a dynamic array for products
typedef struct
{
    dynamic_array<product_data> *products; // Dynamic array of products
    double total_sales;
    double total_profit;
} store_date;

// Function prototypes
void initialize_store(store_date &store);
void add_product(store_date &store);
void delete_product(store_date &store);
void update_product(store_date &store);
void sell_product(store_date &store);
void print_status(const store_date &store);
void free_store(store_date &store);
int search_products(const store_date &store, const char *name, int matching_indices[], int &match_count);
void update_product_details(product_data &product);

// Function to initialize the store
void initialize_store(store_date &store)
{
    store.products = new_dynamic_array<product_data>(10); // Start with an initial capacity of 10
    store.total_sales = 0.0;
    store.total_profit = 0.0;
    printf("Store initialized with capacity for 10 products.\n");
}

// Function to add a new product
/**
 * @brief Adds a new product to the store's product list.
 * 
 * This function prompts the user to input details for a new product, including
 * the product name, cost price, sale price, and quantity on hand. The product
 * is then added to the store's product list. If memory allocation fails during
 * the addition, an error message is displayed.
 * 
 * @param store A reference to the store data structure where the product will be added.
 * 
 * @note The `fgets` function is used to read the product name, which increases
 *       memory safety by preventing buffer overflows. The newline character
 *       added by `fgets` is removed to ensure proper formatting.
 * 
 * @warning Ensure that the `store.products` structure is properly initialized
 *          before calling this function to avoid undefined behavior.
 */
void add_product(store_date &store)
{
    product_data new_product;
    printf("Enter product name: ");
    fgets(new_product.name, sizeof(new_product.name), stdin); 
    new_product.name[strcspn(new_product.name, "\n")] = '\0'; // Remove newline character
    // Or we can use:
    // string product_name = read_line();
    // strncpy(new_product.name, product_name.c_str(), sizeof(new_product.name) - 1);
    // new_product.name[sizeof(new_product.name) - 1] = '\0'; // Ensure null termination
    

    printf("Enter cost price: ");
    scanf("%lf", &new_product.cost_price);
    printf("Enter sale price: ");
    scanf("%lf", &new_product.sale_price);
    printf("Enter quantity on hand: ");
    scanf("%d", &new_product.quantity_on_hand);
    getchar(); // Consume the newline character left by scanf

    if (!add(store.products, new_product))
    {
        printf("Error: Failed to add product. Memory allocation failed.\n");
        return;
    }

    printf("Product added successfully. Current size: %d, capacity: %d\n",
           store.products->size, store.products->capacity);
}

// Function to search for matching products
int search_products(const store_date &store, const char *name, int matching_indices[], int &match_count)
{
    match_count = 0;

    for (unsigned int i = 0; i < store.products->size; i++)
    {
        product_data product = get(store.products, i, product_data{});
        if (strstr(product.name, name) != NULL) // Check if name contains the search text
        {
            matching_indices[match_count] = i;
            match_count++;
        }
    }

    return match_count > 0; // Return 1 if matches found, 0 otherwise
}

// Function to delete a product
void delete_product(store_date &store)
{
    char name[50];
    printf("Enter product name to search for: ");
    ///Doing this to avoid buffer overflow///
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int matching_indices[store.products->size];
    int match_count = 0;

    if (!search_products(store, name, matching_indices, match_count))
    {
        printf("No products found matching '%s'.\n", name);
        return;
    }

    // Display the list of matching products
    printf("Matching products:\n");
    for (int i = 0; i < match_count; i++)
    {
        int index = matching_indices[i];
        product_data product = get(store.products, index, product_data{});
        printf("%d. %s (Cost: %.2f, Sale: %.2f, Quantity: %d)\n",
               i + 1, product.name, product.cost_price, product.sale_price, product.quantity_on_hand);
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
    if (!remove(store.products, index_to_delete))
    {
        printf("Error: Failed to delete product. Index out of bounds.\n");
        return;
    }

    printf("Product deleted successfully. Current size: %d, capacity: %d\n",
           store.products->size, store.products->capacity);
}

// Function to update product details
void update_product(store_date &store)
{
    char name[50];
    printf("Enter product name to update: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int matching_indices[store.products->size];
    int match_count = 0;

    if (!search_products(store, name, matching_indices, match_count))
    {
        printf("No products found matching '%s'.\n", name);
        return;
    }

    // Display the list of matching products
    printf("Matching products:\n");
    for (int i = 0; i < match_count; i++)
    {
        int index = matching_indices[i];
        product_data product = get(store.products, index, product_data{});
        printf("%d. %s (Cost: %.2f, Sale: %.2f, Quantity: %d)\n",
               i + 1, product.name, product.cost_price, product.sale_price, product.quantity_on_hand);
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
    product_data &product = store.products->data[index_to_update];
    update_product_details(product);
}

// Function to update product details
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
void sell_product(store_date &store)
{
    char name[50];
    printf("Enter product name to sell: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    int matching_indices[store.products->size];
    int match_count = 0;

    if (!search_products(store, name, matching_indices, match_count))
    {
        printf("No products found matching '%s'.\n", name);
        return;
    }

    // Display the list of matching products
    printf("Matching products:\n");
    for (int i = 0; i < match_count; i++)
    {
        int index = matching_indices[i];
        product_data product = get(store.products, index, product_data{});
        printf("%d. %s (Cost: %.2f, Sale: %.2f, Quantity: %d)\n",
               i + 1, product.name, product.cost_price, product.sale_price, product.quantity_on_hand);
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
    product_data &product = store.products->data[index_to_sell];

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

    // Update the store's sales total and profit
    store.total_sales += sale_value;
    store.total_profit += profit;

    printf("Sold %d units of '%s'.\n", quantity_to_sell, product.name);
    printf("Sale value: %.2f, Profit: %.2f\n", sale_value, profit);
    printf("Updated quantity of '%s': %d\n", product.name, product.quantity_on_hand);
}

// Function to print the store status
void print_status(const store_date &store)
{
    printf("\nStore Status:\n");
    printf("Total sales: %.2f\n", store.total_sales);
    printf("Total profit: %.2f\n", store.total_profit);
    printf("Number of products: %d\n", store.products->size);

    double stock_value = 0.0;
    int low_stock_count = 0;

    for (unsigned int i = 0; i < store.products->size; i++)
    {
        product_data product = get(store.products, i, product_data{});
        stock_value += product.cost_price * product.quantity_on_hand;
        if (product.quantity_on_hand < 10)
        {
            low_stock_count++;
        }
    }

    printf("Value of stock on hand: %.2f\n", stock_value);
    printf("Number of products with low stock (less than 10): %d\n", low_stock_count);
    printf("Dynamic array capacity: %d\n", store.products->capacity);
}

// Function to free the store's dynamic array
void free_store(store_date &store)
{
    delete_dynamic_array(store.products);
    store.products = nullptr;
    printf("Store data freed successfully.\n");
}

// Main function
int main()
{
    store_date store;
    initialize_store(store);

    int choice;
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
            add_product(store);
            break;
        case 2:
            delete_product(store);
            break;
        case 3:
            update_product(store);
            break;
        case 4:
            sell_product(store);
            break;
        case 5:
            print_status(store);
            break;
        case 6:
            printf("Goodbye.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    free_store(store);
    return 0;
}
