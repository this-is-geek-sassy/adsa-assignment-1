#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int total_rotations = 0;  // Global counter for rotations

// AVL Tree Node structure
struct AVLNode {
    int data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
};

// Utility function to get the height of a node
int height(struct AVLNode* node) {
    return (node == NULL) ? 0 : node->height;
}

// Create a new AVL node
struct AVLNode* create_AVLNode(int data) {
    struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    node->data = data;
    node->left = node->right = NULL;
    node->height = 1;  // New node is initially added at leaf
    return node;
}


// Right rotate subtree rooted with y
struct AVLNode* right_rotate(struct AVLNode* y) {
    total_rotations++;  // Increment rotation count
    struct AVLNode* x = y->left;
    struct AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x;  // Return new root
}

// Utility function to find the node with the smallest value in a tree
struct AVLNode* min_value_node(struct AVLNode* node) {
    struct AVLNode* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}


// Left rotate subtree rooted with x
struct AVLNode* left_rotate(struct AVLNode* x) {
    total_rotations++;  // Increment rotation count
    struct AVLNode* y = x->right;
    struct AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y;  // Return new root
}


// Get the balance factor of node
int get_balance(struct AVLNode* node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}


// Recursive function to insert a node into the AVL tree and balance the tree
struct AVLNode* AVL_insert(struct AVLNode* node, int data) {
    // Standard BST insert
    if (node == NULL) {
        return create_AVLNode(data);
    }
    if (data < node->data) {
        node->left = AVL_insert(node->left, data);
    } else if (data > node->data) {
        node->right = AVL_insert(node->right, data);
    } else {
        // Duplicate keys are not allowed in BST
        return node;
    }

    // Update height of this ancestor node
    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    // Get balance factor to check if node is unbalanced
    int balance = get_balance(node);

    // Left Left Case
    if (balance > 1 && data < node->left->data) {
        return right_rotate(node);
    }

    // Right Right Case
    if (balance < -1 && data > node->right->data) {
        return left_rotate(node);
    }

    // Left Right Case
    if (balance > 1 && data > node->left->data) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Right Left Case
    if (balance < -1 && data < node->right->data) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;  // Return the (unchanged) node pointer
}

// Recursive function to delete a node with given data from the AVL tree and balance the tree
struct AVLNode* AVL_delete(struct AVLNode* root, int data) {
    // Standard BST delete
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = AVL_delete(root->left, data);
    } else if (data > root->data) {
        root->right = AVL_delete(root->right, data);
    } else {
        // Node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            struct AVLNode* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                // One child case
                *root = *temp;
            }
            free(temp);
        } else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            struct AVLNode* temp = min_value_node(root->right);

            // Copy the inorder successor's data to this node
            root->data = temp->data;

            // Delete the inorder successor
            root->right = AVL_delete(root->right, temp->data);
        }
    }

    // If the tree had only one node, return
    if (root == NULL) {
        return root;
    }

    // Update height of this ancestor node
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));

    // Get balance factor to check if the node is unbalanced
    int balance = get_balance(root);

    // Left Left Case
    if (balance > 1 && get_balance(root->left) >= 0) {
        return right_rotate(root);
    }

    // Left Right Case
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    // Right Right Case
    if (balance < -1 && get_balance(root->right) <= 0) {
        return left_rotate(root);
    }

    // Right Left Case
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;  // Return the (possibly) balanced root
}



// Function to calculate tree height (to match structure of original code)
int calculate_height(struct AVLNode* node) {
    return height(node);  // Simply return the height stored in the node
}

// Function to free the AVL tree nodes
void free_tree(struct AVLNode* node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}


// Utility function to average values (similar to original code)
double average(int* values, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += values[i];
    }
    return (double)sum / count;
}

void read_file(const char* filename, int* arr, int size) {
    FILE* file = fopen(filename, "r");      // Open the file for reading
    if (!file) {                            // Check if file opened successfully
        printf("Error opening file: %s\n", filename); // Print error message
        exit(1);                            // Exit the program if file can't be opened
    }
    for (int i = 0; i < size; i++) {        // Loop through the array
        fscanf(file, "%d", &arr[i]);        // Read each integer from the file
    }
    fclose(file);                           // Close the file after reading
}


int main() {
    int num_of_samples = 100;                       // Number of array samples
    int sizes[4] = {10000, 100000, 1000000, 10000000};        

    int total_rotations_insert[num_of_samples];                // To store the total rotations for each array
    int total_rotations_delete[num_of_samples];                // To store the total rotations for each array
    int total_heights_insert[num_of_samples];                  // To store the height of the tree for each array
    int total_heights_delete[num_of_samples];                  // To store the height of the tree for each array

    // To calculate time for processing each sized array
    clock_t start, end;
    double total_insertion_time, total_deletion_time;
    double avg_insertion_time, avg_deletion_time;

    // Open the results files
    FILE* insert_output = fopen("result_Insert_AVL.csv", "a+");
    if (insert_output == NULL) {
        printf("An error occurred while opening the insert results file\n");
        return 1;
    }

    FILE* delete_rotations_output = fopen("result_Delete_AVL_Rotation.csv", "a+");
    if (!delete_rotations_output) {
        printf("An error occurred while opening the delete rotations file\n");
        return 1;
    }

    FILE* delete_height_output = fopen("result_Delete_AVL_Height.csv", "a+");
    if (!delete_height_output) {
        printf("An error occurred while opening the delete height file\n");
        return 1;
    }

    FILE* time_output = fopen("result_Times_AVL.csv", "a+");
    if (time_output == NULL) {
        printf("An error occurred while opening the time results file\n");
        return 1;
    }

    // Write the headers of the output files
    fprintf(delete_rotations_output, "%s, %s\n", "Array Size", "Average Rotations");
    fprintf(delete_height_output, "%s, %s\n", "Array Size", "Average Heights");
    fprintf(insert_output, "%s, %s, %s, %s\n", "Array Size", "Array Number", "Rotations", "Height");
    fprintf(time_output, "%s, %s, %s, %s, %s\n", "Array Size", "Total Insertion Time (seconds)", "Average Insertion Time (seconds)", "Total Deletion Time (seconds)", "Average Deletion Time (seconds)");

    // For all the array sizes
    for (int s = 0; s < 4; s++) {
        printf("Processing arrays of size %d:\n", sizes[s]);

        // Initialize total time for insertions and deletions
        total_insertion_time = 0;
        total_deletion_time = 0;

        // Dynamically allocate memory for array data
        int* data = (int*)malloc(sizes[s] * sizeof(int));

        // Loop over each array sample
        for (int i = 0; i < num_of_samples; i++) {
            // Safely format the filename with array size and index, ensuring no buffer overflow occurs
            char filename[256];
            snprintf(filename, sizeof(filename), "random_array_%d_%d.txt", sizes[s], i + 1);

            // Read the array data from the file
            read_file(filename, data, sizes[s]);

            // Reset rotation count and tree root before starting
            total_rotations = 0;
            struct AVLNode* root = NULL;

            // Start measuring the time for insertions
            start = clock();

            for (int j = 0; j < sizes[s]; j++) {
                root = AVL_insert(root, data[j]);
            }

            // End measuring the time for insertions
            end = clock();
            total_insertion_time += ((double)(end - start)) / CLOCKS_PER_SEC;  // Accumulate insertion time

            // Store the insertion rotation count and height for this array
            total_rotations_insert[i] = total_rotations;
            total_heights_insert[i] = calculate_height(root);  // Store the height after insertion


            // Output to the insertOutput file
            fprintf(insert_output, "random_array_%d_%d.txt, %d, %d\n", sizes[s], i + 1, total_rotations_insert[i], total_heights_insert[i]);

            // Reset rotation count before deletions
            total_rotations = 0;

            // Delete 1/10th of the array
            int num_deletions = sizes[s] / 10;

            // Start measuring the time for deletions
            start = clock();

            for (int d = 0; d < num_deletions; d++) {
                int rand_index = rand() % sizes[s];  // Select a random index for deletion
                root = AVL_delete(root, data[rand_index]);
            }

            // End measuring the time for deletions
            end = clock();
            total_deletion_time += ((double)(end - start)) / CLOCKS_PER_SEC;  // Accumulate deletion time

            // Store the deletion rotation count and height for this array
            total_rotations_delete[i] = total_rotations;
            total_heights_delete[i] = calculate_height(root);

            // Free the tree after processing
            free_tree(root);
            root = NULL;
        }

            // Calculate average rotations and heights for the current array size
        double avg_rotations = average(total_rotations_insert, num_of_samples);
        double avg_heights = average(total_heights_insert, num_of_samples);

        printf("Average insert rotations for %d arrays of size %d: %f\n", num_of_samples, sizes[s], avg_rotations);
        printf("Average insert height for %d arrays of size %d: %f\n", num_of_samples, sizes[s], avg_heights);

        // Calculate average rotations and heights for the current array size
        avg_rotations = average(total_rotations_delete, num_of_samples);
        avg_heights = average(total_heights_delete, num_of_samples);

        printf("Average delete rotations for %d arrays of size %d: %f\n", num_of_samples, sizes[s], avg_rotations);
        printf("Average delete height for %d arrays of size %d: %f\n", num_of_samples, sizes[s], avg_heights);

        // Output average rotations and heights to the corresponding files
        fprintf(delete_rotations_output, "%d, %f\n", sizes[s], avg_rotations);
        fprintf(delete_height_output, "%d, %f\n", sizes[s], avg_heights);

        // Calculate the average insertion and deletion times
        avg_insertion_time = total_insertion_time / num_of_samples;
        avg_deletion_time = total_deletion_time / num_of_samples;

        // Output time results to the time_output file
        fprintf(time_output, "%d, %f, %f, %f, %f\n", sizes[s], total_insertion_time, avg_insertion_time, total_deletion_time, avg_deletion_time);

        printf("Total insertion time for arrays of size %d: %f seconds\n", sizes[s], total_insertion_time);
        printf("Average insertion time for arrays of size %d: %f seconds\n", sizes[s], avg_insertion_time);
        printf("Total deletion time for arrays of size %d: %f seconds\n", sizes[s], total_deletion_time);
        printf("Average deletion time for arrays of size %d: %f seconds\n", sizes[s], avg_deletion_time);

        // Release dynamically allocated memory
        free(data);
        data = NULL;
        printf("\n");
    }

    // Close the files after writing all data
    fclose(delete_rotations_output);
    fclose(delete_height_output);
    fclose(insert_output);
    fclose(time_output);

    return 0;
}
