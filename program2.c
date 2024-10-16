#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdbool.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <gobject/gsignal.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include <time.h>


typedef struct tree_node
{
    long long unsigned int data;
    long long int height;
    struct tree_node * left;
    struct tree_node * right;
    struct tree_node * parent;
} node;

// Global zoom factor
double zoom_factor = 1.0;

// Function to calculate the absolute height of the BST
int height(node * root) {
    if (root == NULL) {
        return -1; // Return -1 for the height of an empty tree
    }
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Function to calculate the AVL height of the BST
long long int avl_height(node * root) {
    if (root == NULL) {
        return -1; // Return -1 for the height of an empty tree
    }
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return leftHeight - rightHeight;
}

node * create_node(long long unsigned int data) {

    node * new_node = (node *) malloc (sizeof(node));
    if (new_node == NULL) {
        printf("\nmemory allocation has failed.\n");
        return new_node;
    }
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->height = 0;
    return new_node;
}

// Function to draw circles and lines for binary tree
void draw_node(cairo_t *cr, node *root, int x, int y, int dx, int depth) {
    if (root == NULL) return;

    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%lld", root->data);

    // Set color to white and fill the circle
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // White color (R, G, B all set to 1.0)
    cairo_arc(cr, x, y, 20, 0, 2 * M_PI);     // Draw circle
    cairo_fill_preserve(cr);                  // Fill the circle but keep the path for stroking

    // Set color to black and stroke the circle
    cairo_set_source_rgb(cr, 0, 0, 0);        // Black color
    cairo_stroke(cr);
    
    // Draw text
    cairo_move_to(cr, x - 10, y + 5);
    cairo_show_text(cr, buffer);
    
    // Draw left child
    if (root->left) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x - dx, y + depth);
        cairo_stroke(cr);
        draw_node(cr, root->left, x - dx, y + depth, dx / 2, depth);
    }
    
    // Draw right child
    if (root->right) {
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x + dx, y + depth);
        cairo_stroke(cr);
        draw_node(cr, root->right, x + dx, y + depth, dx / 2, depth);
    }
}

// GTK drawing callback
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    node *root = (node *)data;  // The root of the binary tree
    draw_node(cr, root, 400, 50, 200, 100);  // Starting coordinates and gap
    return FALSE;
}

node * create_empty_tree() {

    return NULL;
}

bool is_tree_empty(node * tree) {

    if (tree == NULL)
        return true;
    else
        return false;
}

node * left_left_rotation(node * grandfather) {

    printf("ll rotation called on node %lld\n", grandfather->data);

    node * father = grandfather->left;
    node * child = father->left;

    grandfather->left = father->right;

    if (father->right)
        father->right->parent = grandfather; // NULL check safety

    father->right = grandfather;
    father->parent = grandfather->parent;

    if (grandfather->parent) {
        if (grandfather->parent->left == grandfather) {
            grandfather->parent->left = father;
        } else {
            grandfather->parent->right = father;
        }
    }
    grandfather->parent = father;

    grandfather->height = avl_height(grandfather);
    father->height = avl_height(father);

    printf("ll rotation ended, new position holding by node: %lld\n", father->data);

    return father;
}

node * right_right_rotation (node * grandfather) {

    printf("rr rotation called on node %lld\n", grandfather->data);

    node * father = grandfather->right;
    node * child = father->right;

    grandfather->right = father->left;

    if (father->left) {
        father->left->parent = grandfather; // NULL check safety
    }

    father->left = grandfather;
    father->parent = grandfather->parent;

    if (grandfather->parent) {
        if (grandfather->parent->left == grandfather) {
            grandfather->parent->left = father;
        } else {
            grandfather->parent->right = father;
        }
    }
    grandfather->parent = father;

    grandfather->height = avl_height(grandfather);
    father->height = avl_height(father);

    printf("rr rotation ended, new position holding by node: %lld\n", father->data);

    return father;
}

node * right_left_rotation(node * grandfather) {

    printf("rl rotation called on node %lld\n", grandfather->data);

    node * father = grandfather->right;
    node * child = father->left;

    node * new_father = left_left_rotation(father);
    node * new_grandfather = right_right_rotation(grandfather);

    printf("rl rotation ended, new position is holding by node: %lld\n", new_father->data);
    return new_father;
}

node * left_right_rotation(node * grandfather) {

    printf("lr rotation called on node %lld\n", grandfather->data);

    node * father = grandfather->left;
    node * child = father->right;

    node * new_father = right_right_rotation(father);
    node * new_grandfather = left_left_rotation(grandfather);

    printf("lr rotation ended, new position is holding by node: %lld\n", new_father->data);
    return new_father;
}


// Special Left rotate;  function taken from: www.programiz.com/dsa/avl-tree
node *leftRotate(node * x) {
    printf("left rotate function called on %lld\n", x->data);
    node * y = x->right;
    node * T2 = y->left;

    y->left = x;
    y->parent = x->parent;
    x->right = T2;

    if (x->parent) {
        if (x->parent->left == x) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
    }
    if (T2)
        T2->parent = x;
    x->parent = y;

    x->height = avl_height(x);
    y->height = avl_height(y);

    return y;
}

// Special Right rotate;  function taken from: www.programiz.com/dsa/avl-tree
node *rightRotate(node * y) {
    printf("right rotate function called on %lld\n", y->data);
    node * x = y->left;
    node * T2 = x->right;

    x->right = y;
    x->parent = y->parent;
    y->left = T2;

    if (y->parent) {
        if (y->parent->left == y) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    }

    if (T2) 
        T2->parent = y;
    y->parent = x;

    y->height = avl_height(y);
    x->height = avl_height(x);

    return x;
}

node * insert_node(node * tree, long long unsigned int data) {

    node * new_node = create_node(data);

    if (is_tree_empty(tree)) {
        tree = new_node;
        printf("\nInsertion success. %lld inserted on empty tree\n", new_node->data);
        return tree;
    }
    node * ptr = tree;
    node * parent_ptr = NULL;

    while(ptr != NULL) {
        parent_ptr = ptr;
        if (new_node->data == ptr->data) {
            printf("\nNode already present. Duplicate node cannot be inserted.\n");
            return tree;
        }
        else if (new_node->data < ptr->data) {
            ptr = ptr->left;
        } else {
            ptr = ptr->right;
        }
    }
    if (new_node->data < parent_ptr->data) {
        parent_ptr->left = new_node;
    } else {
        parent_ptr->right = new_node;
    }
    new_node->parent = parent_ptr;

    if (parent_ptr->parent == NULL) {
        printf("\nInsertion success. %lld inserted.\n", new_node->data);
        parent_ptr->height = avl_height(parent_ptr);
        return parent_ptr;
    }
    
    node * grandparent_ptr = parent_ptr->parent;
    parent_ptr->height = avl_height(parent_ptr);
    grandparent_ptr->height = avl_height(grandparent_ptr);

    while(grandparent_ptr != tree) {
        grandparent_ptr->height = avl_height(grandparent_ptr);
        if (abs(grandparent_ptr->height) >= 2) {
            break;
        }
        new_node = parent_ptr;
        parent_ptr = grandparent_ptr;
        grandparent_ptr = grandparent_ptr->parent;
    }
    grandparent_ptr->height = avl_height(grandparent_ptr);
    
    if (abs(grandparent_ptr->height) >= 2) {
        printf("%lld->height: %lld\n", grandparent_ptr->data, grandparent_ptr->height);

        if (new_node->data < grandparent_ptr->data && new_node-> data < parent_ptr->data) {
            node * father = left_left_rotation(grandparent_ptr);
            printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
        }
        else if (new_node->data > grandparent_ptr->data && new_node-> data > parent_ptr->data)
        {
            node * father = right_right_rotation(grandparent_ptr);
            printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
        }
        else if (new_node->data > grandparent_ptr->data && new_node-> data < parent_ptr->data) {
            node * father = right_left_rotation(grandparent_ptr);
            printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
        }
        else if (new_node->data < grandparent_ptr->data && new_node-> data > parent_ptr->data) {
            node * father = left_right_rotation(grandparent_ptr);
            printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
        }
        
    }
    while (tree->parent != NULL)
    {
        tree = tree->parent;
    }
    
    return tree;
}

node * search_node(node * tree, long long int data){

    node * ptr = tree;
    node * parent_ptr = NULL;

    while(ptr != NULL) {
        parent_ptr = ptr;
        if (data == ptr->data) {
            printf("\nNode found...\n");
            return ptr;
        }
        else if (data < ptr->data) {
            ptr = ptr->left;
        } else {
            ptr = ptr->right;
        }
    }
    return NULL;  // node not found in the tree
}

int determie_orientation(node * node_to_be_deleted) {

    if ((node_to_be_deleted->left == NULL) && (node_to_be_deleted->right == NULL))
        return -1;  // leaf node
    else if ((node_to_be_deleted->left == NULL) && (node_to_be_deleted->right != NULL))
        return 0;  // only left_null
    else if ((node_to_be_deleted->left != NULL) && (node_to_be_deleted->right == NULL))
        return 1;  // only right_null
    else
        return 2;   // degree 2 node
}

bool is_singleton_tree(node * tree) {

    if (tree == NULL)
        return false;
    
    if (tree->left==NULL && tree->right==NULL)
        return true;
    
    return false;
}

node * find_max(node * root) {

    if (root == NULL) {
        return NULL; // If tree is empty, return NULL
    }

    node * current = root;

    // Traverse to the rightmost node
    while (current->right != NULL) {
        current = current->right;
    }
    printf("Max node is having key: %lld\n", current->data);
    return current; // The rightmost node contains the maximum value
}

void fix_up(node * tree, node * grandparent_ptr, node * parent_ptr, node * new_node) {

    printf("%lld->height: %lld\n", grandparent_ptr->data, grandparent_ptr->height);

    if (new_node->data < grandparent_ptr->data && new_node-> data < parent_ptr->data) {
        node * father = left_left_rotation(grandparent_ptr);
        printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
    }
    else if (new_node->data > grandparent_ptr->data && new_node-> data > parent_ptr->data)
    {
        node * father = right_right_rotation(grandparent_ptr);
        printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
    }
    else if (new_node->data > grandparent_ptr->data && new_node-> data < parent_ptr->data) {
        node * father = right_left_rotation(grandparent_ptr);
        printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
    }
    else if (new_node->data < grandparent_ptr->data && new_node-> data > parent_ptr->data) {
        node * father = left_right_rotation(grandparent_ptr);
        printf("new avl height of father of %lld is: %lld\n", new_node->data, avl_height(father));
    }
    

}

node * finish_deletion(node * tree, node * node_to_be_deleted, int o_val) {

    if (o_val == -1) {  // leaf node
        node * father = node_to_be_deleted->parent, * new_root = NULL;
        int right_rotation = 0, left_rotation = 0;

        if (father->left == node_to_be_deleted) {
            father->left = NULL;
            left_rotation = 1;
        } else {
            father->right = NULL;
            right_rotation = 1;
        }
        if (father->parent == NULL) {
            father->height = avl_height(father);
        }
        if (left_rotation && abs(father->height) >= 2)
        {
            new_root = leftRotate(father);
        }
        else if (right_rotation && abs(father->height) >= 2)
        {
            new_root = rightRotate(father);
        }

        if (new_root->parent != NULL) {
            node * grandfather = new_root->parent;
            grandfather->height = avl_height(grandfather);

            while(grandfather != tree) {
                grandfather->height = avl_height(grandfather);
                if (abs(grandfather->height) >= 2) {
                    fix_up(tree, grandfather, father, new_root);
                }
                new_root = father;
                father = grandfather;
                grandfather = grandfather->parent;
            }
            grandfather->height = avl_height(grandfather);
            if (abs(grandfather->height) >= 2) {
                fix_up(tree, grandfather, father, new_root);
            }
        }

        if (new_root)
            return new_root;
        return father;
    }
}

node * delete_node(node * tree, long long int data) {

    // search the node first
    node * node_to_be_deleted = search_node(tree, data);
    if (node_to_be_deleted == NULL) {
        printf("the node to be deleted was already deleted, hence cannot be faound.\n");
        return NULL;
    }

    // determine is_leaf or left_null or right_null or degree_2 node
    int val = determie_orientation(node_to_be_deleted);

    if (is_singleton_tree(tree))
    {
        printf("only node of the tree (%lld) is now getting delted. Tree will be empty now.\n", node_to_be_deleted->data);
        free(node_to_be_deleted);
        tree = NULL;
    }
    else if (val == -1 && !is_singleton_tree(tree)) {
        node * parent_ptr = node_to_be_deleted->parent;
        // if (parent_ptr->left == node_to_be_deleted) {
        //     parent_ptr->left = NULL;
        //     parent_ptr->height = avl_height(parent_ptr);
        //     if (abs(parent_ptr->height) >= 2)
        //         leftRotate(parent_ptr);
        // } else {
        //     parent_ptr->right = NULL;
        //     parent_ptr->height = avl_height(parent_ptr);
        //     if (abs(parent_ptr->height) >= 2)
        //         rightRotate(parent_ptr);
        // }
        finish_deletion(tree, node_to_be_deleted, val);
        printf("leaf node deletion (%lld) success.\n", node_to_be_deleted->data);
        
        free(node_to_be_deleted);
    }
    else if(val == 0){
        // the left subtree is blank
        node * parent_ptr = node_to_be_deleted->parent;
        if (parent_ptr && (parent_ptr->left == node_to_be_deleted)) {
            parent_ptr->left = node_to_be_deleted->right;

            parent_ptr->height = avl_height(parent_ptr);
            if (parent_ptr->height >= 2) {
                left_right_rotation(parent_ptr);
            }
        } else if (parent_ptr) {
            parent_ptr->right = node_to_be_deleted->right;

            parent_ptr->height = avl_height(parent_ptr);
            if (parent_ptr->height >= 2) {
                right_right_rotation(parent_ptr);
            }
        }
        node_to_be_deleted->right->parent = parent_ptr;
        free(node_to_be_deleted);
    }
    else if (val == 1) {
        // the right subtree is blank
        node * parent_ptr = node_to_be_deleted->parent;
        if (parent_ptr->left == node_to_be_deleted) {
            parent_ptr->left = node_to_be_deleted->left;
        } else {
            parent_ptr->right = node_to_be_deleted->left;
        }
        node_to_be_deleted->left->parent = parent_ptr;
        free(node_to_be_deleted);
    }
    else if (val == 2) {
        // internal node having degree 2
        node * parent_ptr = node_to_be_deleted->parent;
        node * inorder_predecessor = find_max(node_to_be_deleted->left);

        // a very tricky step after a nice observation
        if (inorder_predecessor->parent->right == inorder_predecessor) {
            inorder_predecessor->parent->right = inorder_predecessor->left;
            // inorder_predecessor->left->parent = inorder_predecessor->parent;
        } else
        {
            inorder_predecessor->parent->left = inorder_predecessor->left;
            // inorder_predecessor->left->parent = inorder_predecessor->parent;
        }
        if (inorder_predecessor->left != NULL)
            inorder_predecessor->left->parent = inorder_predecessor->parent;

        // replacing node_to_be_deleted with its inorder_predecessor
        inorder_predecessor->left = node_to_be_deleted->left;
        inorder_predecessor->right = node_to_be_deleted->right;
        inorder_predecessor->parent = node_to_be_deleted->parent;

        if (node_to_be_deleted->parent->left == node_to_be_deleted) {
            node_to_be_deleted->parent->left = inorder_predecessor;
        } else {
            node_to_be_deleted->parent->right = inorder_predecessor;
        }
        printf("the degree 2 node %lld is ready to be deleted. Its inorder predecessor %lld is replacing the node.\n",
                node_to_be_deleted->data, inorder_predecessor->data);
        free(node_to_be_deleted);
    }

    while (tree->parent != NULL)
    {
        tree = tree->parent;
    }

    return tree;
}

// Scroll event handler to zoom in/out
gboolean on_scroll_event(GtkWidget *widget, GdkEventScroll *event, gpointer data) {
    if (event->direction == GDK_SCROLL_UP) {
        zoom_factor *= 1.1;  // Zoom in
    } else if (event->direction == GDK_SCROLL_DOWN) {
        zoom_factor /= 1.1;  // Zoom out
    }

    gtk_widget_queue_draw(widget);  // Request redraw with updated zoom factor
    return TRUE;
}


node * insert_and_redraw(GtkWidget *widget, gpointer data) {
    node *root = (node *)data;
    long long unsigned int new_value;  // Example value, you can take input from a user
    printf("\nEnter the new value to enter into the tree\n");
    scanf("%lld", &new_value);
    root = insert_node(root, new_value);

    gtk_widget_queue_draw(widget);  // Request redraw of the drawing area

    return root;
}

node * delete_and_redraw(GtkWidget *widget, gpointer data) {
    node *root = (node *)data;
    long long int delete_value;  // Example value to delete
    printf("\nEnter the new value to delete from the tree\n");
    scanf("%lld", &delete_value);
    root = delete_node(root, delete_value);

    gtk_widget_queue_draw(widget);  // Request redraw of the drawing area

    return root;
}



int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    int i;
    int sizes[] = {pow(10,1), pow(10,2), pow(10,3), pow(10,4)};
    const gsl_rng_type * T;
    gsl_rng * r;
    size_t N;

    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    unsigned int seed = time(0);

    int any_number = rand_r(&seed) % 10;
    printf("%d %d \n", any_number, sizes[any_number]);

    // N = sizes[any_number];
    N = 30;

    gsl_permutation * p = gsl_permutation_alloc(N);
    gsl_permutation_init(p);

    double * collection = (double *)malloc(sizeof(double) * 100);

    FILE * record = fopen("record2.txt", "a+");
    if (record == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    gsl_ran_shuffle (r, p->data, N, sizeof(size_t));

    for (i=0; i<any_number; i++) {
        gsl_ran_shuffle (r, p->data, N, sizeof(size_t));
    }
    gsl_permutation_fprintf(record, p, " %u");

    // Create binary tree
    node * root = create_empty_tree();

    printf("size of size_t is %lu\n", sizeof(size_t));
    for (i=0; i<N; i++) {
        root = insert_node(root, p->data[i]);
    }

    // root = insert_node(root, 6);
    // root = insert_node(root, 14);
    // root = insert_node(root, 6);
    
    // root = insert_node(root,20);
    // // insert_node(root,6);
    // // insert_node(root,3);
    // root = insert_node(root, 1);
    // root = delete_node(root, 20);
    // insert_node(root, 21);

    // insert_node(root, 15);

    // node * found_or_not = search_node(root, 15);
    // if (found_or_not == NULL) {
    //     printf("node not found\n");
    // } else {
    //     printf("found at location: %p %lld\n", found_or_not, found_or_not->data);
    // }

    // printf("parent of node 4 is: %lld\n", search_node(root, 4)->parent->data);
    // printf("parent of node 15 is: %lld\n", search_node(root, 15)->parent->data);

    // Create GTK window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Binary Tree");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create layout container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create drawing area
    GtkWidget *darea = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(vbox), darea, TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), root);

    // Create button to insert and redraw tree
    GtkWidget *insert_button = gtk_button_new_with_label("Insert Node");
    g_signal_connect(insert_button, "clicked", G_CALLBACK(insert_and_redraw), root);
    gtk_box_pack_start(GTK_BOX(vbox), insert_button, FALSE, FALSE, 0);

    // Create button to delete and redraw tree
    GtkWidget *delete_button = gtk_button_new_with_label("Delete Node");
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_and_redraw), root);
    gtk_box_pack_start(GTK_BOX(vbox), delete_button, FALSE, FALSE, 0);

    // Connect scroll event for zooming
    g_signal_connect(G_OBJECT(darea), "scroll-event", G_CALLBACK(on_scroll_event), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    gtk_main();

    
    printf("Height of the BST is: %d\n", height(root));

    return 0;
}