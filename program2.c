#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdbool.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef struct tree_node
{
    long long int data;
    struct tree_node * left;
    struct tree_node * right;
} node;

node * create_node(long long int data) {

    node * new_node = (node *) malloc (sizeof(node));
    if (new_node == NULL) {
        printf("\nmemory allocation has failed.\n");
        return new_node;
    }
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
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

bool insert_node(node * tree, long long int data) {

    node * new_node = create_node(data);

    if (is_tree_empty(tree)) {
        tree = new_node;
        printf("\nInsertion success\n");
        return true;
    }
    node * ptr = tree;
    node * parent_ptr = NULL;

    while(ptr != NULL) {
        parent_ptr = ptr;
        if (new_node->data == ptr->data) {
            printf("\nNode already present. Duplicate node cannot be inserted.\n");
            return false;
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
    return true;
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


int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    // Create binary tree
    node *root = create_node(1);
    root->left = create_node(2);
    root->right = create_node(3);
    root->left->left = create_node(4);
    root->left->right = create_node(5);
    root->right->left = create_node(6);
    root->right->right = create_node(7);

    insert_node(root, 15);

    node * found_or_not = search_node(root, 15);
    if (found_or_not == NULL) {
        printf("node not found\n");
    } else {
        printf("found at location: %p %lld\n", found_or_not, found_or_not->data);
    }

    // Create GTK window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Binary Tree");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create drawing area
    GtkWidget *darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), root);

    // Show all widgets
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}