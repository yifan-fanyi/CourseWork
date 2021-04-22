/******************************************************************************
 *
 * NAME
 *    tsvq.h
 *    J. R. Goldschneider 2/94
 *
 * DESCRIPTION
 *    Macros used in the programs tsvq and tsvqe and prune and select.
 *
 *****************************************************************************/
#include <stdio.h>
#include <math.h>

#define DATATYPE         unsigned char
#define DISTTYPE         double
#define BOOLEAN          int
#define TRUE             1
#define FALSE            0
#define NAME_MAX         100

/* defaults for tsvq and tsvqe */
#define DEF_dim          4
#define DEF_mult_offset  .01
#define DEF_thresh       0.0
#define DEF_rate         0.0
#define DEF_min_vectors  1 /* the smallest value is 1 */

/* defaults for prune */
#define NODE_DATA_SIZE   4
#define DELTA_D          0
#define DELTA_R          1
#define LAMBDA           2
#define LAMBDA_MIN       3

/* defaults for voronoi_ts */
#define DEF_rows         512
#define DEF_cols         512

#define USAGE            "Usage:"
#define HOWTOUSE_TSVQ    "-t trainingsequence -c codebook -s statfile\n   -d dimension -r rate -m offset -h threshold -B"
#define HOWTOUSE_TSVQE   "-c codebook -i input -o output -R -D"
#define HOWTOUSE_PRUNE   "-c codebook -s statfile -o nested_subtree -E"
#define HOWTOUSE_SELECT  "-c codebook -s nested_subtree -n selection -o sub_codebook"
#define HOWTOUSE_VORONOI "-c codebook -o output -r rows -l columns"

#define NOTFOUND         "not found"
#define NOTOPTION        "unknown option"
#define NOMEMORY         "insufficient memory"
#define NOREAD           "error reading data"
#define NOWRITE          "error writing data"
#define NOTREE           "tree structure error"
#define NODATA           "insufficient data"
#define AND              "and"
#define ARESAME          "are identical"

#define ABORT_TSVQ       "codebook not generated"
#define ABORT_TSVQE      "image not encoded"
#define ABORT_PRUNE      "nested tree file not generated"
#define ABORT_SELECT     "sub-codebook file not generated"
#define ABORT_VORONOI    "voronoi diagram not generated"
#define HALT_TSVQ        "unable to extend codebook tree"
#define EMPTY_CELL       "empty cell may be added to codebook tree"
#define SMALL_THRESH     "use a smaller threshold"

#define DEF_APPEND_CDBK  ".cdbk"
#define DEF_APPEND_STAT  ".stat"
#define DEF_APPEND_TSVQ  ".tsvq"
#define DEF_APPEND_PRUNE ".nest"
#define DEF_APPEND_VOR   ".vor"

#define DEF_trsqname     ""
#define DEF_codebookname ""
#define DEF_statname     ""
#define DEF_inputname    ""
#define DEF_outputname   ""
#define DEF_subtreename  ""
#define DEF_ratename     "rate.dat"

/******************************************************************************
 *
 *    definitions of structures and function calls
 *
 *****************************************************************************/
DISTTYPE dist(DATATYPE *data,DISTTYPE *codeword);

typedef struct tree_struct
{
  struct tree_struct  *left_child;
  struct tree_struct  *right_child;
  struct tree_struct  *parent;
  DISTTYPE            *data;
  long                count;
  DISTTYPE            avmse;
  BOOLEAN             designed;
  int                 depth;
  DATATYPE            **trainseq;
} TreeNode;

typedef struct slope_struct
{
  struct slope_struct *previous;
  struct slope_struct *next;
  TreeNode            *node;
  DISTTYPE            slope;
} SlopeList;
void entropy_init(TreeNode *node);

void split_node();
void split_alternate();
char     *programname;
int      dim=4;
DISTTYPE mult_offset;
DISTTYPE thresh;

SlopeList *slist;

DISTTYPE minimum();

void find_maxdepth();
void normalize_avmse();

FILE *inputfile;
FILE *outputfile;
FILE *ratefile;

char  outputname[NAME_MAX];
char  ratename[NAME_MAX];

BOOLEAN printrate;

int   maxdepth;
BOOLEAN write_codebook(root,numnodes,codebookfile,codebookname)
     TreeNode *root;
     long     numnodes;
     FILE     *codebookfile;
     char     *codebookname;
{
  long      i,n;           /* counters */
  TreeNode *node;          /* node used to point to the tree */
  short    *tree_descrip;  /* array containing tree structure (in bytes) */

  /* allocate memory for tree description */
  if(!(tree_descrip = (short *) calloc(numnodes,sizeof(short)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }
    TreeNode *newnode();
    TreeNode *newchild(TreeNode *parentnode);
    void clear_count(TreeNode *node);
  /* create a node to search the tree */
  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }
  node = root;

  /* create the tree description array */
  n = 0; /* count the number of nodes */
  for (i = 0; i < numnodes; i++) {
    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal */
      if(node->right_child != NULL) {  /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
        return(FALSE);
      }
      tree_descrip[n] = 0; /* enter a terminal node in tree description */
      n++; /* increment count of nodes */

      /* find next node to examine */
      while ((node != root) && (node == node->parent->right_child)) {
        node = node->parent; /* continue with the leaf node's parent */
      }
      if (node == root) break;
      else {
        node = node->parent->right_child;
      }
    }

    else { /* node has a child, go to left child */
      if(node->right_child == NULL) { /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
        return(FALSE);
      }
      tree_descrip[n] = 1; /* enter a non-terminal node in tree description */
      n++; /* increment count of nodes */
      node = node->left_child;
    }
  }

  /* when done constructing the tree, node should be the root */
  if (node != root || n != numnodes) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
  }

  /* write numnodes, dim and tree_descrip */
  if ((fwrite((char *) &numnodes,sizeof(long),1,codebookfile) != 1) ||
      ferror(codebookfile) || feof(codebookfile)) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOWRITE);
    return(FALSE);
  }
  if ((fwrite((char *) &dim,sizeof(int),1,codebookfile) != 1) ||
      ferror(codebookfile) || feof(codebookfile)) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOWRITE);
    return(FALSE);
  }
  if (fwrite((char *) tree_descrip,sizeof(short), (int) numnodes,codebookfile)
      != numnodes || ferror(codebookfile) || feof(codebookfile)){
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOWRITE);
    return(FALSE);
  }

  /* //free allocated space */
  //free((char *) tree_descrip);

  /* now write the codewords */
  node = root;
  n = 0; /* count the number of nodes */
  for (i = 0; i < numnodes; i++) {

    /* write the node's data */
    if (fwrite((char *) node->data,sizeof(DISTTYPE),dim,codebookfile)!= dim ||
    ferror(codebookfile) || feof(codebookfile)){
      fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOWRITE);
      return(FALSE);
    }

    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal, find next node */
      if(node->right_child != NULL) {  /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
        return(FALSE);
      }
      n++; /* increment count of nodes */

      /* find the next node */
      while ((node != root) && (node == node->parent->right_child)) {
        node = node->parent; /* continue with the leaf node's parent */
      }
      if (node == root) break;
      else {
        node = node->parent->right_child;
      }
    }

    else { /* node has a child, go to left child */
      if(node->right_child == NULL) { /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
        return(FALSE);
      }
      n++; /* increment count of nodes */
      node = node->left_child;
    }
  }

  /* when done constructing the tree, node should be the root */
  if (node != root || n != numnodes) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
  }

  fflush(codebookfile);
  return(TRUE);
}

BOOLEAN write_stat(root,numnodes,statfile,statname)
     TreeNode *root;
     long     numnodes;
     FILE     *statfile;
     char     *statname;
{
  long      i,n;           /* counters */
  TreeNode *node;          /* node used to point to the tree */

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }
  node = root;

  n = 0; /* count the number of nodes */
  for (i = 0; i < numnodes; i++) {

    /* write the node's count */
    if (fwrite((char *) &(node->count),sizeof(long),1,statfile)!= 1 ||
    ferror(statfile) || feof(statfile)){
      fprintf(stderr,"%s: %s: %s\n",programname,statname,NOWRITE);
      return(FALSE);
    }
    if (fwrite((char *) &(node->avmse),sizeof(double),1,statfile)!= 1 ||
    ferror(statfile) || feof(statfile)){
      fprintf(stderr,"%s: %s: %s\n",programname,statname,NOWRITE);
      return(FALSE);
    }

    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal, find next node */
      if(node->right_child != NULL) {  /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,statname,NOTREE);
        return(FALSE);
      }
      n++; /* increment count of nodes */

      /* find the next node to use */
      while ((node != root) && (node == node->parent->right_child)) {
        node = node->parent; /* continue with the leaf node's parent */
      }
      if (node == root) break;
      else {
        node = node->parent->right_child;
      }
    }

    else { /* node has a child, go to left child */
      if(node->right_child == NULL) { /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,statname,NOTREE);
        return(FALSE);
      }
      n++; /* increment count of nodes */
      node = node->left_child;
    }
  }

  /* when done constructing the tree, node should be the root */
  if (node != root || n != numnodes) {
    fprintf(stderr,"%s: %s: %s\n",programname,statname,NOTREE);
    return(FALSE);
  }

  fflush(statfile);
  return(TRUE);
}

BOOLEAN write_nested_subtree(root,numnodes,subtreefile,subtreename)
     TreeNode *root;
     long     numnodes;
     FILE     *subtreefile;
     char     *subtreename;
{
  long      i,n;           /* counters */
  TreeNode *node;          /* node used to point to the tree */

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }
  node = root;

  n = 0; /* count the number of nodes */
  for (i = 0; i < numnodes; i++) {

    /* write the node's count */
    if (fwrite((char *) &(node->count),sizeof(long),1,subtreefile)!= 1 ||
    ferror(subtreefile) || feof(subtreefile)){
      fprintf(stderr,"%s: %s: %s\n",programname,subtreename,NOWRITE);
      return(FALSE);
    }

    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal, find next node */
      if(node->right_child != NULL) {  /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,subtreename,NOTREE);
        return(FALSE);
      }
      n++; /* increment count of nodes */

      /* find the next node to use */
      while ((node != root) && (node == node->parent->right_child)) {
        node = node->parent; /* continue with the leaf node's parent */
      }
      if (node == root) break;
      else {
        node = node->parent->right_child;
      }
    }

    else { /* node has a child, go to left child */
      if(node->right_child == NULL) { /* test tree fidelity */
        fprintf(stderr,"%s: %s: %s\n",programname,subtreename,NOTREE);
        return(FALSE);
      }
      n++; /* increment count of nodes */
      node = node->left_child;
    }
  }

  /* when done constructing the tree, node should be the root */
  if (node != root || n != numnodes) {
    fprintf(stderr,"%s: %s: %s\n",programname,subtreename,NOTREE);
    return(FALSE);
  }

  fflush(subtreefile);
  return(TRUE);
}


DISTTYPE image_encode(root,bits,maxbits,numpixels)
     TreeNode  *root;            /* codebook tree */
     long      *bits;            /* number of bits used to encode image */
     int       *maxbits;         /* longest codeword used */
     long      *numpixels;       /* number of pixels encoded */
{
  DISTTYPE   distortion;      /* distortion of encoded image */
  DISTTYPE   tempdistortion;  /* distortion of current encoded vector */
  int        tempbits;        /* length of codeword to encode current vector */
  DATATYPE   *datavector;     /* vector used to store data to be encoded */

  /* allocate memory for data vector */
  if (!(datavector = (DATATYPE *) calloc(dim, sizeof(DATATYPE)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(-1.0);
  }

  /* find the maximum depth of the tree */
  find_maxdepth(root);

  /* begin encoding */
  distortion = 0.0;
  *bits = 0;
  *maxbits = 0;
  *numpixels = 0;

  while(fread((char *) datavector,sizeof(DATATYPE),dim,inputfile) == dim &&
    !feof(inputfile) && !ferror(inputfile)) {

    if(!encode(datavector,root,&tempbits,&tempdistortion)) {
      return(-1.0);
    }

    *bits += tempbits;
    distortion += tempdistortion;
    if (tempbits > *maxbits) {
      *maxbits = tempbits;
    }
    *numpixels += dim;
  }

  normalize_avmse(root);

  //free((char *) datavector);
  return(distortion);
}

BOOLEAN encode(vector,node,bits,distortion)
     DATATYPE *vector;       /* vector to encode */
     TreeNode *node;         /* codebook tree */
     int      *bits;         /* length of the codeword */
     DISTTYPE *distortion;   /* distortion of encoded vector */

{
  int      i; /* counter */
  DISTTYPE tempdist;

  tempdist = dist(vector,node->data);
  node->count += 1;
  node->avmse += tempdist;

  if((node->left_child == NULL) || (node->right_child == NULL)) {
    *distortion = tempdist;
    *bits = node->depth;

    /* round the codeword */
    for (i = 0; i < dim; i++) { /* for double data take the floor out */
      vector[i] = (DATATYPE) ( floor ( node->data[i] + 0.5));
      /* vector[i] = (DATATYPE) node->data[i]; */
    }

    /* write the data to the output file */
    if (fwrite((char *) vector,sizeof(DATATYPE),dim,outputfile) != dim) {
      fprintf(stderr,"%s: %s: %s\n",programname,outputname,NOWRITE);
      return(FALSE);
    }

    if(printrate) { /* output the rate of the codeword */
      for (i = 0; i < dim; i++) {
    vector[i] = (DATATYPE)
      (floor( (float) 255.0 * node->depth / (float) maxdepth));
      }
      if (fwrite((char *) vector,sizeof(DATATYPE),dim,ratefile) != dim) {
    fprintf(stderr,"%s: %s: %s\n",programname,ratename,NOWRITE);
    return(FALSE);
      }
    }

    return(TRUE);
  }

  else {
    /* find the next node to go to */
    if(dist(vector,node->left_child->data) <
       dist(vector,node->right_child->data)) {
      return(encode(vector,node->left_child,bits,distortion));
    }
    else {
      return(encode(vector,node->right_child,bits,distortion));
    }
  }
}


DISTTYPE dist(data,codeword)
     DATATYPE *data;
     DISTTYPE *codeword;
{
  int      i;
  DISTTYPE distortion;

  distortion = 0.0;
  for (i = 0; i < dim; i++) {
    distortion +=
      (( (DISTTYPE) data[i]) - codeword[i]) *
    (( (DISTTYPE) data[i]) - codeword[i]);
  }

  return(distortion);
}

double empirical_entropy(root,numnodes)
     TreeNode *root;
     long     numnodes;
{
  TreeNode *node;
  long     i, n;
  double   entropy;
  long     numbervectors;

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(-1,0);
  }
  node = root;

  entropy = 0.0;
  numbervectors = 0;
  n = 0;
  for (i = 0; i < numnodes; i++) {
    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal */
      if(node->right_child != NULL) {  /* test tree fidelity */
        fprintf(stderr,"%s: %s\n",programname,NOTREE);
        return(-1.0);
      }

      n++;
      numbervectors += node->count;
      if (node->count > 0) {
    entropy += ( (double) (node->count)) * log( (double) node->count );
      }

      /* find the next node to examine */
      while ((node != root) && (node == node->parent->right_child)) {
        node = node->parent; /* continue with the leaf node's parent */
      }

      /* tree search complete */
      if (node == root) break;

      else {
        node = node->parent->right_child;
      }
    }

    else { /* node has a child, go to left child */

      /* test tree fidelity, a non-terminal node should have two childen */
      if(node->right_child == NULL) {
        fprintf(stderr,"%s: %s\n",programname,NOTREE);
        return(-1.0);
      }
      n++;
      node = node->left_child; /* search the next node of the tree */
    }
  }

  if (node != root || n != numnodes) {
    fprintf(stderr,"ccc%s: %s\n",programname,NOTREE);
    return(-1.0);
  }

  /* return the entropy */
  entropy = (log( (double) numbervectors) - (entropy/numbervectors)) / M_LN2;
  return(entropy);
}

void find_maxdepth(node)
     TreeNode *node;
{
  if (node->left_child == NULL) {
    if (node->depth > maxdepth) maxdepth = node->depth;
  }
  else {
    find_maxdepth(node->left_child);
    find_maxdepth(node->right_child);
  }
}

void normalize_avmse(node)
     TreeNode *node;
{
  if (node->count > 0) {
    node->avmse /= (DISTTYPE) node->count;
  }
  if (node->left_child != NULL) {
    normalize_avmse(node->left_child);
    normalize_avmse(node->right_child);
  }
}


void initialize_stat_tree(node)
     TreeNode *node; /* current node to initialize */
{
  /* the node is a leaf node */
  if (!(node->left_child)) {

    /* use the unnormalized squared error */
    node->avmse *= node->count;

    /* initialize leaf node values */
    node->data[DELTA_D] = 0.0;
    node->data[DELTA_R] = 0.0;
    node->data[LAMBDA] = HUGE;
    node->data[LAMBDA_MIN] = HUGE;

    /* use the node->count field to store the subtree_number */
    node->count = 0;
  }

  /* node is an internal node */
  else {
    initialize_stat_tree(node->left_child);
    initialize_stat_tree(node->right_child);

    /* use the unnormalized squared error */
    node->avmse *= node->count;

    /* initialize internal node */
    node->data[DELTA_D] =
      (node->right_child->avmse + node->left_child->avmse - node->avmse)
    + node->left_child->data[DELTA_D] + node->right_child->data[DELTA_D];
    node->data[DELTA_R] = (double) (node->count)
      + node->left_child->data[DELTA_R]+ node->right_child->data[DELTA_R];
    node->data[LAMBDA] = -1.0 *  node->data[DELTA_D] / node->data[DELTA_R];
    node->data[LAMBDA_MIN] = minimum(node->data[LAMBDA],
                     node->left_child->data[LAMBDA_MIN],
                     node->right_child->data[LAMBDA_MIN]);

    /* use the node->count field to store the subtree_number */
    node->count = 0;
  }
}

void entropy_init_stat_tree(node)
     TreeNode *node; /* current node to initialize */
{
  entropy_init(node);
  clear_count(node);
}

void entropy_init(node)
     TreeNode *node; /* current node to initialize */
{
  /* the node is a leaf node */
  if (!(node->left_child)) {

    /* use the unnormalized squared error */
    node->avmse *= node->count;

    /* initialize leaf node values */
    node->data[DELTA_D] = 0.0;
    node->data[DELTA_R] = 0.0;
    node->data[LAMBDA] = HUGE;
    node->data[LAMBDA_MIN] = HUGE;
  }

  /* node is an internal node */
  else {
    entropy_init(node->left_child);
    entropy_init(node->right_child);

    /* use the unnormalized squared error */
    node->avmse *= node->count;

    /* initialize internal node for entropy based pruning */
    node->data[DELTA_D] =
      (node->right_child->avmse + node->left_child->avmse - node->avmse)
    + node->left_child->data[DELTA_D] + node->right_child->data[DELTA_D];
    node->data[DELTA_R] = (node->count)*log((double) node->count)/M_LN2 -
      (node->left_child->count)*log((double) node->left_child->count)/M_LN2 -
    (node->right_child->count)*log((double) node->right_child->count)/M_LN2
      + node->left_child->data[DELTA_R]+ node->right_child->data[DELTA_R];
    node->data[LAMBDA] = -1.0 *  node->data[DELTA_D] / node->data[DELTA_R];
    node->data[LAMBDA_MIN] = minimum(node->data[LAMBDA],
                     node->left_child->data[LAMBDA_MIN],
                     node->right_child->data[LAMBDA_MIN]);
  }
}

void clear_count(node)
     TreeNode *node; /* root node */
{
  /* use the node->count field to store the subtree_number */
  node->count = 0;
  if(node->left_child) {
    clear_count(node->left_child);
    clear_count(node->right_child);
  }
}

TreeNode *find_min_slope(node)
     TreeNode *node; /* root node */
{
  /* if the minimum slope is infinity, there is a problem */
  if (node->data[LAMBDA_MIN] == HUGE) {
    fprintf(stderr,"%s: %s\n",programname, NOTREE);
    return(NULL);
  }

  do {
    /* minimum slope is on left side of tree */
    if (node->data[LAMBDA_MIN] == node->left_child->data[LAMBDA_MIN]) {
      node = node->left_child;
    }

    /* minimum slope is on right side of tree */
    else if (node->data[LAMBDA_MIN] == node->right_child->data[LAMBDA_MIN]) {
      node = node->right_child;
    }

    /* minimum slope is at this node */
    else {
      return(node);
    }
  } while (node->left_child != NULL);

  /* there is a problem if a terminal leaf is reached */
  fprintf(stderr,"%s: %s\n",programname, NOTREE);
  return(NULL);
}

void update_tree(node)
     TreeNode *node;
{
  DISTTYPE delta_d,delta_r;

  /* find correction factors */
  delta_d = node->data[DELTA_D];
  delta_r = node->data[DELTA_R];

  /* make node a leaf node */
  node->data[DELTA_D] = 0.0;
  node->data[DELTA_R] = 0.0;
  node->data[LAMBDA] = HUGE;
  node->data[LAMBDA_MIN] = HUGE;

  /* update the internal nodes including the root */
  while (node->parent != NULL) {
    node = node->parent;
    node->data[DELTA_D] -= delta_d;
    node->data[DELTA_R] -= delta_r;
    node->data[LAMBDA] = -1.0 * node->data[DELTA_D] / node->data[DELTA_R];
    node->data[LAMBDA_MIN] = minimum(node->data[LAMBDA],
                     node->left_child->data[LAMBDA_MIN],
                     node->right_child->data[LAMBDA_MIN]);
  }
}

DISTTYPE minimum(num1,num2,num3)
     DISTTYPE num1,num2,num3;
{
  if (num1 < num2) {
    if (num1 < num3) {
      return(num1);
    }
    else {
      return(num3);
    }
  }

  if (num2 < num3) {
    return(num2);
  }
  else {
    return(num3);
  }
}

void prune(node,subtree_number)
     TreeNode *node;
     long     subtree_number;
{
  if ((node->count > 0) && (node->count <= subtree_number)) {
    node->left_child = NULL;
    node->right_child = NULL;
  }
  if (node->left_child != NULL) {
    prune(node->left_child,subtree_number);
    prune(node->right_child,subtree_number);
  }
}

long count_nodes(root)
     TreeNode *root;
{
  TreeNode *node;
  long     n;

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(0);
  }
  node = root;

  /* count the number of nodes */
  n = 0;
  for ( ; ; ) {

    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal, find next node */
      if (node->right_child != NULL) { /* test tree fidelity */
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(0);
      }

      n++; /* increment count of nodes */
      while ((node != root) && (node == node->parent->right_child)) {
    node = node->parent; /* continue with the leaf node's parent */
      }
      /* test tree structure fidelity */
      if (node == root) break;
      else if(node->parent->right_child == NULL) {
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(0);
      }
      else {
    node = node->parent->right_child;
      }
    }

    else { /* node is not a terminal node, go to the left child */
      /* test tree structure fidelity */
      if((node->left_child == NULL) || (node->right_child == NULL)) {
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(FALSE);
      }
      n++; /* increment count of nodes */
      node = node->left_child;
    }
  }

  return(n);
}

long pruned_count(root)
     TreeNode *root;
{
  TreeNode *node;
  long     n;

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(0);
  }
  node = root;

  /* count the number of nodes just pruned */
  n = 0;
  for ( ; ; ) {

    /* determine node position and from this find the next node to use */

    /* node is terminal, find next node */
    if (node->data[LAMBDA_MIN] == HUGE) {
      n++; /* increment count of nodes */
      while ((node != root) && (node == node->parent->right_child)) {
    node = node->parent; /* continue with the leaf node's parent */
      }
      /* test tree structure fidelity */
      if (node == root) break;
      else if(node->parent->right_child == NULL) {
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(0);
      }
      else {
    node = node->parent->right_child;
      }
    }

    /* node is not a terminal node, go to the left child */
    else {
      /* test tree structure fidelity */
      if((node->left_child == NULL) || (node->right_child == NULL)) {
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(FALSE);
      }
      n++; /* increment count of nodes */
      node = node->left_child;
    }
  }

  return(n);
}
BOOLEAN construct_tree(root,numnodes,codebookfile,codebookname)
     TreeNode *root;
     long     numnodes;    /* number of nodes in the tree */
     FILE     *codebookfile;
     char     *codebookname;
{
  short    *tree_descrip;  /* array containing tree structure (in bytes) */
  long     i,n;            /* counters */
  TreeNode *node;          /* node used to build tree */

  /* allocate memory for the tree descriptor */
  if(!(tree_descrip = (short *) calloc(numnodes,sizeof(short)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* read in tree structure */
  if (fread((char *) tree_descrip,sizeof(short),(int) numnodes,codebookfile)
      != numnodes || ferror(codebookfile) || feof(codebookfile)) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOREAD);
    return(FALSE);
  }

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }
  node = root;

  n = 0; /* count the number of words read in */

  /* build the codebook given a node and its depth */
  for (i = 0; i < numnodes; i++) {

    /* read in node information */
    if((fread((char *) node->data,sizeof(DISTTYPE),dim,codebookfile) != dim) ||
    ferror(codebookfile) || feof(codebookfile)) {
      fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOREAD);
      return(FALSE);
    }

    /* determine node position and from this find the next node to use */
    if (tree_descrip[i] == 0) { /* node is terminal, find next node */
      n++; /* increment count of nodes */
      while ((node != root) && (node == node->parent->right_child)) {
    node = node->parent; /* continue with the leaf node's parent */
      }
      /* test tree structure fidelity */
      if (node == root) break;
      else if(node->parent->right_child != NULL) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
      }
      else {
    node = node->parent;
    if(!(node->right_child = newchild(node))) return(FALSE);
    node = node->right_child;
      }
    }

    else { /* node is not a terminal node, create the left child */
      /* test tree structure fidelity, this node should have no children */
      if((node->left_child != NULL) || (node->right_child != NULL)) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
      }
      n++; /* increment count of nodes */
      if(!(node->left_child = newchild(node))) return(FALSE);
      node = node->left_child;
    }
  }
  /* when done constructing the tree, node should be the root */
  if (node != root || n != numnodes) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
  }

  //free((char *) tree_descrip);
  return(TRUE);
}

BOOLEAN construct_stat_tree(root,numnodes,codebookfile,codebookname,
                statfile,statname)
     TreeNode *root;
     long     numnodes;    /* number of nodes in the tree */
     FILE     *codebookfile;
     char     *codebookname;
     FILE     *statfile;
     char     *statname;
{
  short    *tree_descrip;  /* array containing tree structure (in bytes) */
  long     i,n;            /* counters */
  TreeNode *node;          /* node used to build tree */

  /* allocate memory for the tree descriptor */
  if(!(tree_descrip = (short *) calloc(numnodes,sizeof(short)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* read in tree structure */
  if (fread((char *) tree_descrip,sizeof(short),(int) numnodes,codebookfile)
      != numnodes || ferror(codebookfile) || feof(codebookfile)) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOREAD);
    return(FALSE);
  }

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }
  node = root;

  n = 0; /* count the number of words read in */

  /* build the codebook given a node and its depth */
  for (i = 0; i < numnodes; i++) {

    /* read in node count and distortion information */
    if((fread((char *) &(node->count),sizeof(long),1,statfile) != 1) ||
    ferror(statfile) || feof(statfile)) {
      fprintf(stderr,"%s: %s: %s\n",programname,statname,NOREAD);
      return(FALSE);
    }
    if((fread((char *) &(node->avmse),sizeof(DISTTYPE),1,statfile) != 1) ||
    ferror(statfile) || feof(statfile)) {
      fprintf(stderr,"%s: %s: %s\n",programname,statname,NOREAD);
      return(FALSE);
    }

    /* determine node position and from this find the next node to use */
    if (tree_descrip[i] == 0) { /* node is terminal, find next node */
      n++; /* increment count of nodes */
      while ((node != root) && (node == node->parent->right_child)) {
    node = node->parent; /* continue with the leaf node's parent */
      }
      /* test tree structure fidelity */
      if (node == root) break;
      else if(node->parent->right_child != NULL) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
      }
      else {
    node = node->parent;
    if(!(node->right_child = newchild(node))) return(FALSE);
    node = node->right_child;
      }
    }

    else { /* node is not a terminal node, create the left child */
      /* test tree structure fidelity, this node should have no children */
      if((node->left_child != NULL) || (node->right_child != NULL)) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
      }
      n++; /* increment count of nodes */
      if(!(node->left_child = newchild(node))) return(FALSE);
      node = node->left_child;
    }
  }
  /* when done constructing the tree, node should be the root */
  if (node != root || n != numnodes) {
    fprintf(stderr,"%s: %s: %s\n",programname,codebookname,NOTREE);
    return(FALSE);
  }

  //free((char *) tree_descrip);
  return(TRUE);
}

BOOLEAN construct_subtree(root,numnodes,subtreefile,subtreename)
     TreeNode *root;
     long     numnodes;    /* number of nodes in the tree */
     FILE     *subtreefile;
     char     *subtreename;
{
  TreeNode *node;          /* node used to build tree */
  long     i,n;            /* counters */

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }
  node = root;

  n = 0; /* count the number of words read in */

  /* read in the subtree numbers */
  for (i = 0; i < numnodes; i++) {

    /* read in node count and distortion information */
    if((fread((char *) &(node->count),sizeof(long),1,subtreefile) != 1) ||
    ferror(subtreefile) || feof(subtreefile)) {
      fprintf(stderr,"%s: %s: %s\n",programname,subtreename,NOREAD);
      return(FALSE);
    }

    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal, find next node */
      n++; /* increment count of nodes */
      while ((node != root) && (node == node->parent->right_child)) {
    node = node->parent; /* continue with the leaf node's parent */
      }
      /* test tree structure fidelity */
      if (node == root) break;
      else if(node->parent->right_child == NULL) {
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(FALSE);
      }
      else {
    node = node->parent->right_child;
      }
    }

    else { /* node is not a terminal node, go to the left child */
      /* test tree structure fidelity */
      if((node->left_child == NULL) || (node->right_child == NULL)) {
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(FALSE);
      }
      n++; /* increment count of nodes */
      node = node->left_child;
    }
  }
  /* when done constructing the tree, node should be the root */
  if (node != root || n != numnodes) {
    fprintf(stderr,"%s: %s\n",programname,NOTREE);
    return(FALSE);
  }

  return(TRUE);
}

void update_rate(maxslopenode,bits,distortion)
     SlopeList *maxslopenode;
     long      *bits;
     DISTTYPE  *distortion;
{
  /* the distortion is the same as the old distortion less the distortion
     due to the node being replaced plus the distortion due to the
     two nodes just added */
  *distortion +=  maxslopenode->node->left_child->avmse *
    ((DISTTYPE) maxslopenode->node->left_child->count) +
      maxslopenode->node->right_child->avmse *
    ((DISTTYPE) maxslopenode->node->right_child->count) -
      maxslopenode->node->avmse * ((DISTTYPE) maxslopenode->node->count);

  /* when the very last nodes are added, the distortion is -0.0 */
  if (*distortion <= 0) *distortion = 0;

  /* the number of bits is the same as the old number plus count since
     this is just a replacement of one node by its two children */
  *bits += maxslopenode->node->count;
}

DISTTYPE slope(node)
     TreeNode *node;
{
  DISTTYPE distnode;
  DISTTYPE probleft = 0.0;
  DISTTYPE probright = 0.0;

  if(node->count < DEF_min_vectors) return( (DISTTYPE) 0.0 );

  probleft = ((DISTTYPE) (node->left_child->count)) /
    ((DISTTYPE) node->count);
  probright = ((DISTTYPE) (node->right_child->count)) /
    ((DISTTYPE) node->count);

  distnode = (node->avmse) -
    (probleft * (node->left_child->avmse)) -
      (probright * (node->right_child->avmse));

  /* slope is (change in distortion)/(change in rate) and (change in rate)
     is equal to count, so it is not necessary to multiply distnode by
     count */
  return(distnode);
}

BOOLEAN conditional_insert(node)
     TreeNode *node;

{
  if (slope(node) > 0) {
    return(forced_insert(node));
  }
  else {
    return(TRUE);
  }
}

BOOLEAN forced_insert(node)
     TreeNode *node;
{
  SlopeList *tempnode;

  if(!(tempnode = (SlopeList *) calloc(1,sizeof(SlopeList)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* insert the newest node at the beginning of the list */
  slist->next->previous = tempnode;
  tempnode->next=slist->next;
  slist->next = tempnode;
  tempnode->previous = slist;
  tempnode->slope = slope(node);
  tempnode->node = node;
  return(TRUE);
}

BOOLEAN initialize_slopelist()
{
  if(!(slist = (SlopeList *) calloc(1,sizeof(SlopeList)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* the slope of 0.0 ensures that there is always one entry in the list
     and that it will never be chosen */
  slist->previous = slist;
  slist->next = slist;
  slist->slope = 0.0;
  slist->node = NULL;
  return(TRUE);
}


void delete_slopelist(list_element)
     SlopeList *list_element;
{
  list_element->previous->next = list_element->next;
  list_element->next->previous = list_element->previous;
  //free((char *) list_element);
}

SlopeList *find_maxslope()
{
  SlopeList *i;
  SlopeList *currentbest;
  DISTTYPE max;

  if(!(i = (SlopeList *) calloc(1,sizeof(SlopeList))) ||
     !(currentbest = (SlopeList *) calloc(1,sizeof(SlopeList)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(NULL);
  }

  /* the first node is a dummy node */
  max = 0.0;
  currentbest = slist;

  /* search the list for the largest slope */
  for (i = slist->next; i != slist; i = i->next) {
    if (i->slope > max)    {
      max = i->slope;
      currentbest = i;
    }
  }

  return(currentbest);
}

SlopeList *find_oldest_entry()
{
  /* this should be used for a balanced tree, return the oldest entry */
  return(slist->previous);
}

TreeNode *newnode()
{
  TreeNode *n;

  if(!(n = (TreeNode *) calloc(1,sizeof(TreeNode)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(NULL);
  }

  n->left_child = NULL;
  n->right_child = NULL;
  n->parent = NULL;
  n->data = NULL;
  n->count = 0;
  n->avmse = 0.0;
  n->designed = FALSE;
  n->depth = 0;
  return(n);
}


TreeNode *newchild(parentnode)
     TreeNode *parentnode;
{
  TreeNode *n;

  if(!(n = (TreeNode *) calloc(1,sizeof(TreeNode)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(NULL);
  }

  n->left_child = NULL;
  n->right_child = NULL;
  n->parent = parentnode;
  if(!(n->data = (DISTTYPE *) calloc(dim,sizeof(DISTTYPE)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(NULL);
  }
  n->count = 0;
  n->avmse = 0.0;
  n->designed = FALSE;
  n->depth = parentnode->depth + 1;
  return(n);
}

TreeNode *create_root()
{
  TreeNode *n;

  if(!(n = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(NULL);
  }

  if(!(n->data = (DISTTYPE *) calloc(dim,sizeof(DISTTYPE)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(NULL);
  }

  return(n);
}

BOOLEAN initialize_tree(root,trsqfile,trsqname)
     TreeNode *root;       /* node to initialize */
     FILE     *trsqfile;   /* training sequence file */
     char     *trsqname;   /* training sequence name */
{
  DATATYPE *data_vector;  /* used to store one training vector */
  DISTTYPE distortion;    /* distortion of training vectors to centroid */
  long     count;         /* the number of training vectors */
  long     i,j;           /* counters */

  /* allocate memory for data_vector */
  if (!(data_vector = (DATATYPE *) calloc(dim,sizeof(DATATYPE)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* set the codeword to all zeros */
  for (i = 0; i < dim; i++) {
    root->data[i] = 0.0;
  }

  /* find the number of training vectors */
  rewind(trsqfile);
  for (count = 0; ; ) {
    if (fread(data_vector,sizeof(DATATYPE),dim,trsqfile) != dim ||
    feof(trsqfile) || ferror(trsqfile)) {
      break;
    }
      
      
     
    count++;
  }
 printf("%d",count);
  if (count == 0) {
    fprintf(stderr,"%s: %s: %s\n",programname,trsqname,NODATA);
    return(FALSE);
  }

  /* allocate memory for the training vector array */
  if (!(root->trainseq = (DATATYPE **) calloc(count,sizeof(DATATYPE *)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* read the training vectors, and compute centroid */
  rewind(trsqfile);
  for (i = 0; i < count;  i++) {

    /* read in the next training vector */
    if (fread((char *) data_vector,sizeof(DATATYPE),dim,trsqfile) != dim ||
    feof(trsqfile) || ferror(trsqfile)) {
      fprintf(stderr,"%s: %s: %s\n",programname,trsqname,NOREAD);
      return(FALSE);
    }

    /* allocate memory for the training vector */
    if(!(root->trainseq[i] = (DATATYPE *) calloc(dim,sizeof(DATATYPE)))){
      fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
      return(FALSE);
    }

    /* copy the training vector and add to the sum of training vectors */
    for (j = 0; j < dim; j++) {
      root->trainseq[i][j] = data_vector[j];
      root->data[j] += ((DISTTYPE) data_vector[j]);
    }
  }

  /* normalize the sum of training vectors */
  for (i = 0; i < dim; i++) {
    root->data[i] /= ((DISTTYPE) count);
  }

  root->count = count;
  root->designed = TRUE;

  /* compute the average distortion */
  distortion = 0.0;
  for (i = 0; i < count; i++) {
    distortion += dist(root->trainseq[i],root->data);
  }
  distortion /= ((DISTTYPE) root->count);
  root->avmse = distortion;

  /* release memory */

  return(TRUE);
}

BOOLEAN lloyd(node)
     TreeNode *node;
{
  long     leftcount, rightcount;
  DISTTYPE *leftcentroid, *rightcentroid;
  DISTTYPE leftdist, rightdist;
  DISTTYPE current_dist, past_dist;
  DISTTYPE current_leftdist, current_rightdist;
  long     i,j;
  BOOLEAN  alternate_flag = FALSE; /* avoid an infinite loop */

  /* allocate space centroids of the left and right nodes */
  if(!(leftcentroid = (DISTTYPE *) calloc(dim,sizeof(DISTTYPE))) ||
     !(rightcentroid = (DISTTYPE *) calloc(dim,sizeof(DISTTYPE)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* create the node's children */
  if(!(node->left_child = newchild(node)) ||
     !(node->right_child = newchild(node))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* initialize the codebook */
  split_node(node,leftcentroid,rightcentroid);

  /* initialize the current distortion as some enormous number */
  current_dist = HUGE;

  /* if the node's distortion is zero, leave, the slope will be zero.
     this is also the appropriate exit for a node with a zero count */
  if(node->avmse <= 0) return(TRUE);

  do{

    /* assign the codewords and clear the centroids, counts, and distortions */
    for (j = 0; j < dim; j++) {
      node->left_child->data[j] = leftcentroid[j];
      leftcentroid[j] = 0.0;
      node->right_child->data[j] = rightcentroid[j];
      rightcentroid[j] = 0.0;
    }
    leftcount = 0;
    rightcount = 0;
    leftdist = 0.0;
    rightdist = 0.0;

    /* do a lloyd iteration */
    for (i = 0; i < node->count; i++) {
      current_leftdist = dist(node->trainseq[i],node->left_child->data);
      current_rightdist = dist(node->trainseq[i],node->right_child->data);
      if (current_leftdist < current_rightdist) {
    for (j = 0; j < dim; j++) {
      leftcentroid[j] += ((DISTTYPE) node->trainseq[i][j]);
    }
    leftcount += 1;
    leftdist += current_leftdist;
      }
      else {
    for (j = 0; j < dim; j++) {
      rightcentroid[j] += ((DISTTYPE) node->trainseq[i][j]);
    }
    rightcount += 1;
    rightdist += current_rightdist;
      }
    }

    /* find average mse distortion */
    if (leftcount != 0)
      leftdist /= ((DISTTYPE) leftcount);
    if (rightcount != 0)
      rightdist /= ((DISTTYPE) rightcount);

    /* save old distortion, and find new distortion */
    past_dist = current_dist;
    current_dist = (leftdist * ((DISTTYPE) leftcount)/((DISTTYPE) node->count))
      + (rightdist * ((DISTTYPE) rightcount) / ((DISTTYPE) node->count));

    /* find the centroids for the next iteration */
    for (j = 0; j < dim; j++) {
      if (leftcount != 0)
    leftcentroid[j] /= ((DISTTYPE) leftcount);
      if (rightcount != 0)
    rightcentroid[j] /= ((DISTTYPE) rightcount);
    }

    /* if the distortion is 0, stop trying to split the node */
    if (current_dist == 0.0) break;

    /* if alternate_flag is true, a forced split has already been attempted,
       do not try it again since an infinite loop would occur */
    if ((leftcount == 0 || rightcount == 0) && alternate_flag) {
      /* do nothing, will exit when current_dist = past_dist */
    }

    /* else if the distortion is not 0 and the split was not successful,
       force a better split */
    else if (leftcount == 0 || rightcount == 0) {
      split_alternate(node,leftcentroid,rightcentroid);
      alternate_flag = TRUE;
      past_dist = HUGE;
    }

  }  while ( !(((past_dist - current_dist)/past_dist) <= thresh) );
  /* finished with iterations */

  /* save the avmse and count of the node's children */
  node->left_child->avmse = leftdist;
  node->right_child->avmse = rightdist;
  node->left_child->count = leftcount;
  node->right_child->count = rightcount;

  /* prepare to store the training sequence among the children */
  if (!(node->left_child->trainseq =
    (DATATYPE **)calloc(leftcount,sizeof(DATATYPE *))) ||
      !(node->right_child->trainseq =
    (DATATYPE **)calloc(rightcount,sizeof(DATATYPE *)))) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(FALSE);
  }

  /* divide the training set among the node's children */
  leftcount = 0;
  rightcount = 0;
  for (i = 0; i < node->count; i++) {
    if (dist(node->trainseq[i],node->left_child->data) <
    dist(node->trainseq[i],node->right_child->data)) {
      node->left_child->trainseq[leftcount] = node->trainseq[i];
      leftcount += 1;
    }
    else {
      node->right_child->trainseq[rightcount] = node->trainseq[i];
      rightcount += 1;
    }
  }

  /* When the convergence threshold thresh is greater than zero,the codeword
     of a given voronoi region is not the average of the training vectors in
     the voronoi region.  Hence it is possible that the Lloyd iteration may
     stop with a voronoi region that has one or more identical training
     vectors, but that the codeword may not be the same as the training
     vector.  If a tree that completely resolves the training data is to be
     grown, i.e. one leaf for each distinct codeword, then it may not be
     possible to create a tree with zero distortion.  This is solved by
     allowing any node with a non-zero distortion to split, even if
     the node's count is only one, since there is still a positive slope
     that represents the goodness of the split.  Allowing the node
     to split may cause the growing algorithm to add an empty cell to
     the tree.

     There may be another problem when the convergence threshold is greater
     than zero.  It is possible that a node with a non-zero avmse has failed
     to split and has a child node with the parent node's training vectors
     and a lower avmse. This would result in a positive slope which may cause
     the growing algorithm to add an empty cell to the tree. This
     would happen rarely and only if the threshold is too large.

     Since the option of a changing threshold is still allowed, a test
     is inserted here to check for an empty cell problem.  If an
     empty cell is found, the user is notified of the problem and the
     use of a smaller threshold is recommended. An empty cell problem
     does not cause the program to terminate. */

  if ((rightcount == 0 && node->left_child->avmse < node->avmse) ||
      (leftcount == 0 && node->right_child->avmse < node->avmse)) {
    fprintf(stderr,"%s: %s: %s\n",programname,EMPTY_CELL,SMALL_THRESH);
  }

  //free((char *) node->trainseq);
  //free((char *) leftcentroid);
  //free((char *) rightcentroid);
  return(TRUE);
}

void split_node(node,leftcentroid,rightcentroid)
     TreeNode *node;
     DISTTYPE *leftcentroid;
     DISTTYPE *rightcentroid;
{
  int i;

  for (i = 0; i < dim; i++) {
    leftcentroid[i] = node->data[i];
    rightcentroid[i] = (node->data[i]) * (1 + mult_offset);
  }
}

void split_alternate(node,leftcentroid,rightcentroid)
     TreeNode *node;
     DISTTYPE *leftcentroid;
     DISTTYPE *rightcentroid;
{
  long   i, j;
  double maxdist = 0.0;
  double tempdist;

  /* find the training vector that is the furthest from the centroid */
  j = 0;
  for (i = 0; i < node->count; i++) {
    if ((tempdist = dist(node->trainseq[i],node->data)) > maxdist) {
      j = i;
      maxdist = tempdist;
    }
  }

  /* copy new codewords */
  for (i = 0; i < dim; i++) {
    leftcentroid[i] = node->data[i];
    rightcentroid[i] = (DISTTYPE) node->trainseq[j][i];
  }
}

long tree_clean(root)
     TreeNode *root;
{
  TreeNode *node;
  long     n;

  if(!(node = newnode())) {
    fprintf(stderr,"%s: %s\n",programname,NOMEMORY);
    return(0);
  }
  node = root;

  /* count the number of designed nodes in the tree, and get rid of those
     nodes that are not designed */
  n = 0;
  for ( ; ; ) {
    /* determine node position and from this find the next node to use */
    if (node->left_child == NULL) { /* node is terminal */

      if(node->right_child != NULL) {  /* test tree fidelity */
        fprintf(stderr,"%s: %s\n",programname,NOTREE);
        return(0);
      }

      if(node->designed) n++; /* increment count of designed nodes */
      else { /* node is not designed, get rid of it and its sibling */
    node = node->parent;

    node->left_child = NULL;
    node->right_child = NULL;
      }

      /* find the next node to examine */
      while ((node != root) && (node == node->parent->right_child)) {
        node = node->parent; /* continue with the leaf node's parent */
      }

      /* tree search complete */
      if (node == root) break;

      else {
        node = node->parent->right_child;
      }
    }

    else { /* node has a child, go to left child */

      /* test tree fidelity, a non-terminal node should be designed and
         should have two childen */
      if(node->right_child == NULL || !(node->designed)) {
        fprintf(stderr,"%s: %s\n",programname,NOTREE);
        return(0);
      }
      n++; /* increment count of designed nodes */
      node = node->left_child; /* search the next node of the tree */
    }
  }

  /* return a count of the number of designed nodes */
  return(n);
}
