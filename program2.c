#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef struct tree_node
{
    long long int data;
    struct tree_node * left;
    struct tree_node * right;
} node;

// Function to draw circles and lines for binary tree
void draw_node(cairo_t *cr, node *root, int x, int y, int dx, int depth) {
    if (root == NULL) return;

    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d", root->data);

    cairo_set_source_rgb(cr, 0, 0, 0);  // Set color to black
    cairo_arc(cr, x, y, 20, 0, 2 * M_PI);  // Draw circle
    cairo_stroke(cr);  // Stroke the circle
    
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


int main() {


}