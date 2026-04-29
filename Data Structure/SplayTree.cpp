const int SZ = 101010;  // ptr[] size
// T 구현시 field, T(), T(argument), friend T operator+ (const T &t1, const T &t2) {}
template <typename T>  // 1-index, 사용시 [0],[N+1] dummy data 필요
struct SplayTree {
  struct Node {
    Node *l, *r, *p; T val, sum; int key, sz; bool flip;
    Node() { l = r = p = nullptr, sz = 1, flip = false; }
    ~Node() { for (Node* n : {l, r, p}) if (n != nullptr) delete n; }
    bool IsRoot() { return !p || (this != p->l && this != p->r); }
    bool IsLeft() { return !IsRoot() && this == p->l; }
    bool IsRight() { return !IsRoot() && this == p->r; }
    friend int GetSize(const Node* x) { return x ? x->sz : 0; }
    friend T GetSum(const Node* x) { return x ? x->sum : T(); }
    void Rotate() {
      p->Prop(); Prop();
      if (IsLeft()) { if (r) r->p = p; p->l = r; r = p; } 
      else { if (l) l->p = p; p->r = l; l = p; }
      if (!p->IsRoot()) (p->IsLeft() ? p->p->l : p->p->r) = this;
      Node* temp = p; p = temp->p; temp->p = this;
      temp->Update(); Update();
    }
    void Prop() {  // lazy propagation, 필요시 별개의 lazy 구현하기
      Update(val);
      if (flip) swap(l, r);  // if(lazy != ID) { val = composition(val, lazy); Update(val) };
      for (Node* c : {l, r}) if (c) c->flip ^= flip;  // c->lazy = push(c->lazy, lazy) };
      flip = 0; // lazy = ID;
    }
    void Update() { sz = 1 + GetSize(l) + GetSize(r);  }// sum = val + GetSum(l) + GetSum(r);
    void Update(const T& v) { val = v; Update(); }
  };
  Node *rt = nullptr, *ptr[SZ]{nullptr};
  Node* Splay(Node* x, Node* g = nullptr) {
    for (g || (rt = x); x->p != g; x->Rotate()) {
      if (!x->p->IsRoot()) x->p->p->Prop();
      x->p->Prop(); x->Prop();
      if (x->p->p != g) (x->IsLeft() ^ x->p->IsLeft() ? x : x->p)->Rotate();
    }
    x->Prop(); return x;
  }
  Node* Gather(int s, int e) {  // [s, e] 구간 모으기
    Node* temp = Kth(e + 1);
    return Splay(temp, Kth(s - 1))->l;
  }
  Node* Flip(int s, int e) {  // [s, e] 구간 뒤집기
    Node* x = Gather(s, e); x->flip ^= 1; return x;
  }
  Node* Shift(int s, int e, int k) {  // [s, e]를 k만큼 오른쪽으로 시프트
    int len = e - s + 1;
    if (k >= 0) { k = k % len; if (k) Flip(s, e), Flip(s, s + k - 1), Flip(s + k, e); }
    else { k = (-k) % len; if (k) Flip(s, e), Flip(s, e - k), Flip(e - k + 1, e); }
    return Gather(s, e);
  }
  Node* Kth(int k) {  // k번째 노드 (1~N번)
    for (Node* x = rt;; x = x->r) {
      for (; x->Prop(), x->l && x->l->sz > k; x = x->l);
      if (x->l) k -= x->l->sz;
      if (!k--) return Splay(x);
    }
  }
  int Index(int k) { Splay(ptr[k]); return rt->l->sz; }  // k번째 key 값의 인덱스 (1~N)
  void Build( int sz, T arr[]) {  // ptr[i]->key < ptr[i+1]->key / arr[0],arr[N+1]은 dummy data
    ptr[sz + 1] = new Node;
    ptr[sz + 1]->key = sz + 1, ptr[sz + 1]->Update(T());
    for (int i = sz; i >= 0; i--) {
      ptr[i] = new Node;
      ptr[i]->r = ptr[i + 1], ptr[i + 1]->p = ptr[i];
      ptr[i]->key = i, ptr[i]->Update(arr[i]);
    }
    rt = ptr[0];
  }
};