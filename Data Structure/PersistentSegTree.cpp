const int SZ = 101010;  // Dynamic SegTree's Max Size
struct PersistentSegTree {
  struct Node {
    Node *left, *right; int val;
    Node() : left(nullptr), right(nullptr), val(0) {}
  };

  int n, cnt; Node *tree[SZ];

  PersistentSegTree(int n, int arr[]) : n(n), cnt(0) {
    fill(tree, tree + SZ, nullptr);
    tree[0] = new Node;
    build(tree[0], 1, n, arr);
  }

  void build(Node *nd, int s, int e, int arr[]) {
    if (s == e) {
      nd->val = arr[s];
    } else {
      int m = s + e >> 1;
      nd->left = new Node, nd->right = new Node;
      build(nd->left, s, m, arr), build(nd->right, m + 1, e, arr);
      nd->val = nd->left->val + nd->right->val;
    }
  }

  void add(int i, int v) {
    tree[++cnt] = new Node;
    update(tree[cnt - 1], tree[cnt], 1, n, i, v);
  }

  void update(Node *prv, Node *cur, int s, int e, int i, int v) {
    if (s == e) {
      cur->val = v;
    } else {
      int m = s + e >> 1;
      if (i <= m) {
        cur->left = new Node, cur->right = prv->right;
        update(prv->left, cur->left, s, m, i, v);
      } else {
        cur->left = prv->left, cur->right = new Node;
        update(prv->right, cur->right, m + 1, e, i, v);
      }
      cur->val = cur->left->val + cur->right->val;
    }
  }

  int query(Node *nd, int s, int e, int l, int r) {
    if (r < s || e < l) return 0;
    if (l <= s && e <= r) return nd->val;
    int m = s + e >> 1;
    return query(nd->left, s, m, l, r) + query(nd->right, m + 1, e, l, r);
  }
};