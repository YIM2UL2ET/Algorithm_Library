const int ASZ = 26;
struct Trie {
  Trie *go[ASZ], *fail; bool output;
  Trie() : fail(nullptr), output(false) { fill(go, go + ASZ, nullptr); }
  ~Trie() { for (int i = 0; i < ASZ; i++) if (go[i]) delete go[i]; }

  void insert(const string &P) {
    Trie *cur = this;
    for (auto &ch : P) {
      Trie *&nxt = cur->go[ch - 'a'];
      if (!nxt) nxt = new Trie;
      cur = nxt;
    }
    cur->output = true;
  }

  void calc_failure_function() {
    queue<Trie *> Q;
    Q.push(this), this->fail = this;
    while (!Q.empty()) {
      Trie *cur = Q.front();
      Q.pop();
      for (int i = 0; i < ASZ; i++) {
        Trie *&nxt = cur->go[i];
        if (!nxt) continue;
        if (cur == this) {
          nxt->fail = this;
        } else {
          Trie *dst = cur->fail;
          while (dst != this && !dst->go[i]) dst = dst->fail;
          if (dst->go[i]) dst = dst->go[i];
          nxt->fail = dst;
        }
        nxt->output |= nxt->fail->output;
        Q.push(nxt);
      }
    }
  }

  bool ahocorasick(const string &T) {
    Trie *cur = this;
    for (auto &ch : T) {
      while (cur != this && !cur->go[ch - 'a']) cur = cur->fail;
      if (cur->go[ch - 'a']) cur = cur->go[ch - 'a'];
      if (cur->output) return true;
    }
    return false;
  }
};