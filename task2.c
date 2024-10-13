#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product {
    int id;
    char name[100];
    float price;
    int quantity;
    struct Product* left;
    struct Product* right;
} Product;

Product* createProduct(int id, const char* name, float price, int quantity) {
    Product* newProduct = (Product*)malloc(sizeof(Product));
    newProduct->id = id;
    strcpy(newProduct->name, name);
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->left = newProduct->right = NULL;
    return newProduct;
}

Product* insert(Product* root, Product* product) {
    if (root == NULL) return product;
    
    if (strcmp(product->name, root->name) < 0)
        root->left = insert(root->left, product);
    else if (strcmp(product->name, root->name) > 0)
        root->right = insert(root->right, product);
    
    return root;
}

Product* search(Product* root, const char* name) {
    if (root == NULL || strcmp(name, root->name) == 0)
        return root;
    
    if (strcmp(name, root->name) < 0)
        return search(root->left, name);
    return search(root->right, name);
}

void updateProduct(Product* product, float price, int quantity) {
    product->price = price;
    product->quantity = quantity;
}

void inorderTraversal(Product* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%s - Price: $%.2f, Quantity: %d\n", root->name, root->price, root->quantity);
        inorderTraversal(root->right);
    }
}

void displayOutOfStock(Product* root) {
    if (root != NULL) {
        displayOutOfStock(root->left);
        if (root->quantity == 0)
            printf("%s is out of stock\n", root->name);
        displayOutOfStock(root->right);
    }
}

int main() {
    Product* root = NULL;
    int choice, id, quantity;
    char name[100];
    float price;

    while (1) {
        printf("\n1. Add product\n2. Search product\n3. Update product\n");
        printf("4. Display all products\n5. Display out of stock products\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter product ID: ");
                scanf("%d", &id);
                printf("Enter product name: ");
                scanf("%s", name);
                printf("Enter price: ");
                scanf("%f", &price);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                root = insert(root, createProduct(id, name, price, quantity));
                break;
            case 2:
                printf("Enter product name to search: ");
                scanf("%s", name);
                Product* found = search(root, name);
                if (found)
                    printf("Found: %s - Price: $%.2f, Quantity: %d\n", found->name, found->price, found->quantity);
                else
                    printf("Product not found.\n");
                break;
            case 3:
                printf("Enter product name to update: ");
                scanf("%s", name);
                found = search(root, name);
                if (found) {
                    printf("Enter new price: ");
                    scanf("%f", &price);
                    printf("Enter new quantity: ");
                    scanf("%d", &quantity);
                    updateProduct(found, price, quantity);
                    printf("Product updated.\n");
                } else {
                    printf("Product not found.\n");
                }
                break;
            case 4:
                inorderTraversal(root);
                break;
            case 5:
                displayOutOfStock(root);
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
