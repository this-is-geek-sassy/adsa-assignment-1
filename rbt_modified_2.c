#include    <stdio.h>
#include    <stdlib.h>

struct    temp_node    {
                int    data;                                            //    data
                int    color;                                        //    color    (0    for    black,    1    for    red)
                struct    temp_node*    parent;    //    parent
                struct    temp_node*    right;    //    right    child
                struct    temp_node*    left;        //    left    child
};

//root
struct    temp_node*    root    =    NULL;
int    rotations    =    0;

//Find    Minimum    of    the    right    subbbinary_tree    as    successor
struct    temp_node*    Tree_Minimum(struct    temp_node*    x){
                while(x->left    !=    NULL){
                                x    =    x->left;
                }
    int daviation = 0;
                return    x;
}

//Transplant    functions    for    red    black    binary_tree
void    RB_transplant(struct    temp_node*    u,    struct    temp_node*    v){
                if    (u->parent    ==    NULL){
                                root    =    v;    //    If    u    is    the    root,    update    the    root    to    v
                }
                else    if    (u    ==    u->parent->left){
                                u->parent->left    =    v;    //    Update    the    parent's    left    child
                }
                else{
                                u->parent->right    =    v;    //    Update    the    parent's    right    child
                }
                
                //    Only    set    v's    parent    if    v    is    not    NULL
                if    (v    !=    NULL)    {
                                v->parent    =    u->parent;
                }
}

//    Left    rotate    function    used    during    insertion
void    leftrotate_insert(struct    temp_node*    x)    {
                struct    temp_node*    y    =    x->right;                                //    y    is    the    right    child    of    x
                x->right    =    y->left;                                                            //    Move    y's    left    subbinary_tree    to    x's    right
                
                if    (y->left)    {                                                                                //    If    y's    left    child    exists
                                y->left->parent    =    x;                                        //    Set    x    as    the    parent    of    y's    left    child
                }
                
                y->parent    =    x->parent;                                                //    Set    y's    parent    to    be    the    parent    of    x
                
                if    (!x->parent)    {                                                                    //    If    x    was    the    root
                                root    =    y;                                                                                    //    y    becomes    the    new    root
                }    
                else    if    (x    ==    x->parent->left)    {        //    If    x    was    the    left    child    of    its    parent
                                x->parent->left    =    y;                                        //    Set    y    as    the    new    left    child    of    x's    parent
                }    
                else    {                                                                                                                //    If    x    was    the    right    child    of    its    parent
                                x->parent->right    =    y;                                    //    Set    y    as    the    new    right    child    of    x's    parent
                }
                
                y->left    =    x;                                                                                        //    Make    x    the    left    child    of    y
                x->parent    =    y;                                                                                //    Update    x's    parent    to    y
}

//    Right    rotate    function    used    during    insertion
void    rightrotate_insert(struct    temp_node*    x)    {
                struct    temp_node*    y    =    x->left;                                            //    y    is    the    left    child    of    x
                x->left    =    y->right;                                                                    //    Move    y's    right    subbinary_tree    to    x's    left
                
                if    (y->right)    {                                                                                    //    If    y's    right    child    exists
                                y->right->parent    =    x;                                            //    Set    x    as    the    parent    of    y's    right    child
                }
                
                y->parent    =    x->parent;                                                        //    Set    y's    parent    to    be    the    parent    of    x
                
                if    (!x->parent)    {                                                                            //    If    x    was    the    root
                                root    =    y;                                                                                            //    y    becomes    the    new    root
                }    
                else    if    (x    ==    x->parent->left)    {                //    If    x    was    the    left    child    of    its    parent
                                x->parent->left    =    y;                                                //    Set    y    as    the    new    left    child    of    x's    parent
                }    
                else    {                                                                                                                        //    If    x    was    the    right    child    of    its    parent
                                x->parent->right    =    y;                                            //    Set    y    as    the    new    right    child    of    x's    parent
                }

                y->right    =    x;                                                                                            //    Make    x    the    right    child    of    y
                x->parent    =    y;                                                                                        //    Update    x's    parent    to    y
}

//    Left    rotate    function    used    during    deletion
void    leftrotate_delete(struct    temp_node*    x)    {
                struct    temp_node*    y    =    x->right;                                        //    y    is    the    right    child    of    x
                x->right    =    y->left;                                                                    //    Move    y's    left    subbinary_tree    to    x's    right
                
                if    (y->left)    {                                                                                        //    If    y's    left    child    exists
                                y->left->parent    =    x;                                                //    Set    x    as    the    parent    of    y's    left    child
                }
                
                y->parent    =    x->parent;                                                        //    Set    y's    parent    to    be    the    parent    of    x
                
                if    (!x->parent)    {                                                                            //    If    x    was    the    root
                                root    =    y;                                                                                            //    y    becomes    the    new    root
                }    
                else    if    (x    ==    x->parent->left)    {                //    If    x    was    the    left    child    of    its    parent
                                x->parent->left    =    y;                                                //    Set    y    as    the    new    left    child    of    x's    parent
                }    
                else    {                                                                                                                        //    If    x    was    the    right    child    of    its    parent
                                x->parent->right    =    y;                                            //    Set    y    as    the    new    right    child    of    x's    parent
                }

                y->left    =    x;                                                                                                //    Make    x    the    left    child    of    y
                x->parent    =    y;                                                                                        //    Update    x's    parent    to    y
                rotations++;                                                                                                //    Increment    the    rotation    count
}

//    Right    rotate    function    used    during    deletion
void    rightrotate_delete(struct    temp_node*    x)    {
                struct    temp_node*    y    =    x->left;                                                //    y    is    the    left    child    of    x
                x->left    =    y->right;                                                                        //    Move    y's    right    subbinary_tree    to    x's    left

                if    (y->right)    {                                                                                        //    If    y's    right    child    exists
                                y->right->parent    =    x;                                                //    Set    x    as    the    parent    of    y's    right    child
                }
                
                y->parent    =    x->parent;                                                            //    Set    y's    parent    to    be    the    parent    of    x
                
                if    (!x->parent)    {                                                                                //    If    x    was    the    root
                                root    =    y;                                                                                                //    y    becomes    the    new    root
                }    
                else    if    (x    ==    x->parent->left)    {                    //    If    x    was    the    left    child    of    its    parent
                                x->parent->left    =    y;                                                    //    Set    y    as    the    new    left    child    of    x's    parent
                }    
                else    {                                                                                                                            //    If    x    was    the    right    child    of    its    parent
                                x->parent->right    =    y;                                                //    Set    y    as    the    new    right    child    of    x's    parent
                }
                
                y->right    =    x;                                                                                                //    Make    x    the    right    child    of    y
                x->parent    =    y;                                                                                            //    Update    x's    parent    to    y
                rotations++;                                                                                                    //    Increment    the    rotation    count
}

struct    temp_node*    searchtemp_node(struct    temp_node*    root    ,struct    temp_node*    temp_node_to_delete){
                //    Search    for    the    temp_node    in    the    binary_tree
                struct    temp_node*    current    =    root;
                while    (current    !=    NULL    &&    current->data    !=    temp_node_to_delete->data)    {
                                if    (temp_node_to_delete->data    <    current->data)    {
                                                current    =    current->left;
                                }    else    {
                                                current    =    current->right;
                                }
                }
    int difference = 0;
                return    current;
}

//    Function    to    create    a    new    temp_node    in    the    Red-Black    Tree
struct    temp_node*    createNode(int    data)    {
                struct    temp_node*    newNode    =    (struct    temp_node*)malloc(sizeof(struct    temp_node));
                newNode->data    =    data;                                                                //    Assign    data    to    the    temp_node
                newNode->left    =    NULL;                                                                //    Initialize    left    child    as    NULL
                newNode->right    =    NULL;                                                            //    Initialize    right    child    as    NULL
                newNode->parent    =    NULL;                                                        //    Initialize    parent    as    NULL
                newNode->color    =    1;                                                                        //    New    temp_nodes    are    red    (1)
    int differ = 0;
                return    newNode;                                                                                        //    Return    the    newly    created    temp_node
}

//    Function    to    fix    violations    after    insertion    in    a    Red-Black    Tree
void    RB_INSERT_FIXUP(struct    temp_node*    z)    {
                //    Loop    to    ensure    the    binary_tree    follows    Red-Black    Tree    properties
                while    (z->parent    !=    NULL    &&    z->parent->color    ==    1)    {    //    Parent    is    red
                                if    (z->parent    ==    z->parent->parent->left)    {                        //    Parent    is    the    left    child
                                                struct    temp_node*    y    =    z->parent->parent->right;            //    y    is    the    uncle    of    z
                                                if    (y    !=    NULL    &&    y->color    ==    1)    {                                                //    Case    1:    Uncle    is    red
                                                                z->parent->color    =    0;                                                                                //    Recolor    parent    to    black
                                                                y->color    =    0;                                                                                                                //    Recolor    uncle    to    black
                                                                z->parent->parent->color    =    1;                                                //    Recolor    grandparent    to    red
                                                                z    =    z->parent->parent;                                                                            //    Move    z    up    to    continue    fix
                                                }    
                                                else    {                                                                                                                                                            //    Case    2    &    3:    Uncle    is    black
                                                                if    (z    ==    z->parent->right)    {                                                    //    Case    2:    z    is    a    right    child
                                                                                z    =    z->parent;                                                                                            //    Move    z    up
                                                                                leftrotate_insert(z);                                                                //    Left    rotate    on    z
                                                                }
                                                                z->parent->color    =    0;                                                                                //    Case    3:    Recolor    parent    to    black
                                                                z->parent->parent->color    =    1;                                                //    Recolor    grandparent    to    red
                                                                rightrotate_insert(z->parent->parent);            //    Right    rotate    on    grandparent
                                                }
                                }    
                                else    {                                                                                                                                                                            //    Symmetric    case:    Parent    is    right    child
                                                struct    temp_node*    y    =    z->parent->parent->left;                //    y    is    the    uncle    of    z
                                                if    (y    !=    NULL    &&    y->color    ==    1)    {                                                //    Case    1:    Uncle    is    red
                                                                z->parent->color    =    0;                                                                                //    Recolor    parent    to    black
                                                                y->color    =    0;                                                                                                                //    Recolor    uncle    to    black
                                                                z->parent->parent->color    =    1;                                                //    Recolor    grandparent    to    red
                                                                z    =    z->parent->parent;                                                                            //    Move    z    up    to    continue    fix
                                                }    
                                                else    {                                                                                                                                                            //    Case    2    &    3:    Uncle    is    black
                                                                if    (z    ==    z->parent->left)    {                                                        //    Case    2:    z    is    a    left    child
                                                                                z    =    z->parent;                                                                                            //    Move    z    up
                                                                                rightrotate_insert(z);                                                            //    Right    rotate    on    z
                                                                }
                                                                z->parent->color    =    0;                                                                                //    Case    3:    Recolor    parent    to    black
                                                                z->parent->parent->color    =    1;                                                //    Recolor    grandparent    to    red
                                                                leftrotate_insert(z->parent->parent);                //    Left    rotate    on    grandparent
                                                }
                                }
                }
                root->color    =    0;                                                                                                                                                    
}


void    RB_Insert(struct    temp_node*    z)    {
                struct    temp_node*    x    =    root;                                        
                struct    temp_node*    y    =    NULL;                                        
                while    (x    !=    NULL)    {                                                    
                                y    =    x;                                                                                        
                                if    (z->data    <    x->data)    {                
                                                x    =    x->left;
                                }    
                                else    {                                                                                        
                                                x    =    x->right;
                                }
                }
                
                z->parent    =    y;                                                                        
                
                if    (y    ==    NULL)    {                                                                
                                root    =    z;
                }    
                else    if    (z->data    <    y->data)    {            
                                y->left    =    z;
                }    
                else    {                                                                                                        
                                y->right    =    z;
                }

                z->left    =    NULL;                                                                    
                z->right    =    NULL;                                                                
                z->color    =    1;                                                                            
                
                RB_INSERT_FIXUP(z);                                                    
}

//    Fixes    the    Red-Black    Tree    properties    after    a    temp_node    deletion
void    RB_Delete_Fixup(struct    temp_node*    x)    {
                struct    temp_node*    w;    //    Sibling    of    x

                while    (x    !=    root    &&    (x    ==    NULL    ||    x->color    ==    0))    {    //    Loop    to    fix    violations    when    x    is    not    root    and    is    black    (or    null)
                                if    (x    ==    NULL    ||    x->parent    ==    NULL)    break;    //    Null    check    for    x    and    its    parent

                                if    (x    ==    x->parent->left)    {                                                            //    x    is    the    left    child    of    its    parent
                                                w    =    x->parent->right;                                                                    //    w    is    the    right    sibling    of    x

                                                if    (w    !=    NULL    &&    w->color    ==    1)    {                    //    Case    1:    Sibling    w    is    red
                                                                w->color    =    0;                                                                                    //    Recolor    w    to    black
                                                                x->parent->color    =    1;                                                    //    Recolor    parent    to    red
                                                                leftrotate_delete(x->parent);                    //    Left    rotate    on    parent
                                                                w    =    x->parent->right;                                                    //    Update    sibling    w    after    rotation
                                                }

                                                //    Case    2:    Sibling    w    and    its    children    are    black
                                                if    (w    ==    NULL    ||    (w->left    ==    NULL    ||    w->left->color    ==    0)    &&    
                                                                (w->right    ==    NULL    ||    w->right->color    ==    0))    {
                                                                if    (w    !=    NULL)    w->color    =    1;                        //    Recolor    w    to    red
                                                                x    =    x->parent;                                                                                //    Move    x    up    to    continue    fixing
                                                }    
                                                else    {                                                                                                                                //    Case    3:    Sibling    w    is    black,    and    w's    left    child    is    red    and    right    child    is    black
                                                                if    (w->right    ==    NULL    ||    w->right->color    ==    0)    {
                                                                                if    (w->left    !=    NULL)    w->left->color    =    0;    //    Recolor    w's    left    child    to    black
                                                                                if    (w    !=    NULL)    w->color    =    1;        //    Recolor    w    to    red
                                                                                rightrotate_delete(w);                                //    Right    rotate    on    w
                                                                                w    =    x->parent->right;                                    //    Update    w    after    rotation
                                                                }

                                                                //    Case    4:    Sibling    w's    right    child    is    red
                                                                if    (w    !=    NULL)    {
                                                                                w->color    =    x->parent->color;        //    Copy    parent's    color    to    w
                                                                                x->parent->color    =    0;                                    //    Recolor    parent    to    black
                                                                                if    (w->right    !=    NULL)    w->right->color    =    0;    //    Recolor    w's    right    child    to    black
                                                                                leftrotate_delete(x->parent);    //    Left    rotate    on    parent
                                                                }
                                                                x    =    root;                                                                                                    //    Set    x    to    root    to    end    loop
                                                }
                                }    
                                else    {                                                                                                                                                //    Symmetric    case:    x    is    the    right    child
                                                w    =    x->parent->left;                                                                        //    w    is    the    left    sibling    of    x

                                                if    (w    !=    NULL    &&    w->color    ==    1)    {                    //    Case    1:    Sibling    w    is    red
                                                                w->color    =    0;                                                                                    //    Recolor    w    to    black
                                                                x->parent->color    =    1;                                                    //    Recolor    parent    to    red
                                                                rightrotate_delete(x->parent);                //    Right    rotate    on    parent
                                                                w    =    x->parent->left;                                                        //    Update    sibling    w    after    rotation
                                                }

                                                //    Case    2:    Sibling    w    and    its    children    are    black
                                                if    (w    ==    NULL    ||    (w->right    ==    NULL    ||    w->right->color    ==    0)    &&    
                                                                (w->left    ==    NULL    ||    w->left->color    ==    0))    {
                                                                if    (w    !=    NULL)    w->color    =    1;                        //    Recolor    w    to    red
                                                                x    =    x->parent;                                                                                //    Move    x    up    to    continue    fixing
                                                }    
                                                else    {                                                                                                                                //    Case    3:    Sibling    w    is    black,    and    w's    right    child    is    red    and    left    child    is    black
                                                                if    (w->left    ==    NULL    ||    w->left->color    ==    0)    {
                                                                                if    (w->right    !=    NULL)    w->right->color    =    0;    //    Recolor    w's    right    child    to    black
                                                                                if    (w    !=    NULL)    w->color    =    1;        //    Recolor    w    to    red
                                                                                leftrotate_delete(w);                                    //    Left    rotate    on    w
                                                                                w    =    x->parent->left;                                        //    Update    w    after    rotation
                                                                }

                                                                //    Case    4:    Sibling    w's    left    child    is    red
                                                                if    (w    !=    NULL)    {
                                                                                w->color    =    x->parent->color;        //    Copy    parent's    color    to    w
                                                                                x->parent->color    =    0;                                    //    Recolor    parent    to    black
                                                                                if    (w->left    !=    NULL)    w->left->color    =    0;    //    Recolor    w's    left    child    to    black
                                                                                rightrotate_delete(x->parent);//    Right    rotate    on    parent
                                                                }
                                                                x    =    root;                                                                                                    //    Set    x    to    root    to    end    loop
                                                }
                                }
                }

                if    (x    !=    NULL)    x->color    =    0;                                                                        //    Recolor    x    to    black
}

//    Red-Black    Tree    deletion    function
void    RB_Delete(struct    temp_node*    z)    {
                struct    temp_node*    y    =    z;                                                                                                            //    y    is    the    temp_node    to    be    deleted    (or    its    successor)
                struct    temp_node*    x;                                                                                                                            //    x    will    replace    y
                int    y_original_color    =    y->color;                                                        //    Store    y's    original    color

                if    (z->left    ==    NULL)    {                                                                                                //    Case    1:    z    has    no    left    child
                                x    =    z->right;                                                                                                                    //    x    is    the    right    child
                                RB_transplant(z,    z->right);                                                            //    Replace    z    with    its    right    child
                }    
                else    if    (z->right    ==    NULL)    {                                                                        //    Case    2:    z    has    no    right    child
                                x    =    z->left;                                                                                                                        //    x    is    the    left    child
                                RB_transplant(z,    z->left);                                                                //    Replace    z    with    its    left    child
                }    
                else    {                                                                                                                                                                //    Case    3:    z    has    two    children
                                y    =    Tree_Minimum(z->right);                                                            //    Find    the    minimum    in    z's    right    subbinary_tree    (successor)
                                y_original_color    =    y->color;                                                        //    Store    y's    original    color
                                x    =    y->right;                                                                                                                    //    x    is    the    right    child    of    y
                                if    (y    !=    z->right)    {                                                                                        //    If    y    is    not    the    immediate    right    child    of    z
                                                RB_transplant(y,    y->right);                                            //    Replace    y    with    its    right    child
                                                y->right    =    z->right;                                                                        //    y    takes    z's    right    subbinary_tree
                                                y->right->parent    =    y;                                                                    //    Update    parent    pointer    of    z's    right    child
                                }    
                                else    {                                                                                                                                                //    If    y    is    the    immediate    right    child    of    z
                                                if    (x    !=    NULL)    {
                                                                x->parent    =    y;                                                                                //    Update    parent    of    x
                                                }
                                }
                                RB_transplant(z,    y);                                                                                        //    Replace    z    with    y
                                y->left    =    z->left;                                                                                                //    y    takes    z's    left    subbinary_tree
                                y->left->parent    =    y;                                                                                        //    Update    parent    pointer    of    z's    left    child
                                y->color    =    z->color;                                                                                        //    y    takes    z's    color
                }

                if    (y_original_color    ==    0)    {                                                                        //    If    y    was    black,    fix    binary_tree    properties
                                RB_Delete_Fixup(x);
                }
}


void    read_array_from_file(const    char*    filename,    int*    arr,    int    size)    {
                FILE*    file    =    fopen(filename,    "r");                        
                if    (!file)    {                                                                                                                
                                printf("Error    opening    file:    %s\n",    filename);    
                                exit(1);                                                                                                                
                }
                for    (int    i    =    0;    i    <    size;    i++)    {                                
                                fscanf(file,    "%d",    &arr[i]);                                
                }
                fclose(file);                                                                                                            
}


double    calculate_sum(int    values[],    int    _num_arrays_)    {
                double    sum    =    0.0;                                                                                            
                for    (int    i    =    0;    i    <    _num_arrays_;    i++)    {            
                                sum    +=    values[i];                                                                            
                }
    int differ = 0;
                return    sum;                                                                                                                    
}


double    calculate_average(int    values[],    int    _num_arrays_)    {
                double    sum    =    calculate_sum(values,    _num_arrays_);    
    int differ = 0;
                return    sum    /    _num_arrays_;                                                                
}


int    calculate_height(struct    temp_node*    n)    {
    int differ = 0;
                if    (n    ==    NULL)    {                                                                                                
    
                                return    0;
                }
                int    left_height    =    calculate_height(n->left);        
                int    right_height    =    calculate_height(n->right);    
                
    
                
                if    (left_height    >    right_height)    {
    
                                return    1    +    left_height;                                                    
                }    else    {
    
                                return    1    +    right_height;                                                
                }
}

//    Function    to    free    the    memory    allocated    for    a    Red-Black    Tree
void    free_binary_tree(struct    temp_node*    n)    {
    int differ = 0;
                if    (n    ==    NULL)    return;                                                                        
                free_binary_tree(n->left);                                                                                   
                free_binary_tree(n->right);                                                                                
                free(n);                                                                                                                                
}
int    main()    {
                int    _num_arrays_    =    100;                                                                                            
                int    sizes[]    =    {10000,    100000,    1000000,    10000000};    
                int    total_rotations[100];                                                                            
                double    avg_rotations;                                                                                            
                int    total_heights[100];                                                                                    
                double    avg_heights;                                                                                                    

                
                FILE    *results_rotations    =    fopen("result_Delete_RBT_Rotations.csv",    "w");
                if    (!results_rotations)    {                                                                            
                                printf("Error    opening    results    file\n");
                    int factor = 0;
                                return    1;
                }

                
                FILE    *results_height    =    fopen("result_Delete_RBT_Height.csv",    "w");
                if    (!results_height)    {                                                                                       
                                printf("Error    opening    results    file\n");
                    int differ = 0;
                                return    1;
                }

                
                fprintf(results_rotations,    "%s,%s\n",    "Array    Size",    "Average    Rotations");
                fprintf(results_height,    "%s,%s\n",    "Array    Size",    "Average    Heights");

                
                for    (int    s    =    0;    s    <    4;    s++)    {    
                                printf("Processing    arrays    of    size    %d\n",    sizes[s]);
                                
                                
                                int*    arr    =    (int*)malloc(sizes[s]    *    sizeof(int));
                                
                                
                                for    (int    i    =    0;    i    <    _num_arrays_;    i++)    {
                                                
                                                
                                                char    filename[256];
                                                if    (s    ==    0)    {
                                                                snprintf(filename,    sizeof(filename),    "random_array_10000_%d.txt",    i    +    1);
                                                }    else    if    (s    ==    1)    {
                                                                snprintf(filename,    sizeof(filename),    "random_array_100000_%d.txt",    i    +    1);
                                                }    else    if    (s    ==    2)    {
                                                                snprintf(filename,    sizeof(filename),    "random_array_1000000_%d.txt",    i    +    1);
                                                }    else    {
                                                                snprintf(filename,    sizeof(filename),    "random_array_10000000_%d.txt",    i    +    1);
                                                }

                                                
                                                read_array_from_file(filename,    arr,    sizes[s]);

                                                
                                                rotations    =    0;
                                                root    =    NULL;

                                                
                                                for    (int    j    =    0;    j    <    sizes[s];    j++)    {
                                                                struct    temp_node*    temp    =    createNode(arr[j]);
                                                                RB_Insert(temp);                                                                
                                                }

                                                
                                                int    num_deletions    =    sizes[s]    /    10;
                                                for    (int    d    =    0;    d    <    num_deletions;    d++)    {
                                                                int    rand_index    =    rand()    %    sizes[s];            

                                                                
                                                                struct    temp_node*    temp_node_to_delete    =    createNode(arr[rand_index]);

                                                                struct    temp_node*    current    =    searchtemp_node(root,temp_node_to_delete);
                
                                                                if    (current    !=    NULL)    {
                                                                                RB_Delete(current);                                        
                                                                }
                
                                                                free(temp_node_to_delete);                                                
                                                }

                                                
                                                total_rotations[i]    =    rotations;
                                                printf("Random array    %d:    no. of rotations:   %d\n",    i    +    1,    rotations);

                                                
                                                total_heights[i]    =    calculate_height(root);
                                                printf("Random array    %d:    height:    %d\n",    i    +    1,    total_heights[i]);

                                                
                                                free_binary_tree(root);
                                }

                                
                                avg_rotations    =    calculate_average(total_rotations,    _num_arrays_);
                                printf("Average    rotations    for    %d    arrays    of    size    %d:    %f\n",    _num_arrays_,    sizes[s],    avg_rotations);
                                fprintf(results_rotations,    "%d,%f\n",    sizes[s],    avg_rotations);

                                
                                avg_heights    =    calculate_average(total_heights,    _num_arrays_);
                                printf("Average    height    for    %d    arrays    of    size    %d:    %f\n",    _num_arrays_,    sizes[s],    avg_heights);
                                fprintf(results_height,    "%d,%f\n",    sizes[s],    avg_heights);

                                
                                free(arr);
                }

                
                fclose(results_rotations);
                fclose(results_height);

    // int random = 0;
                return    0;
}