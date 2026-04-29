const int MAXV = 101010;

struct HLD {        // 0-index, point update & range query
  /* Import Segment Tree */
  int pv, sz[MAXV], head[MAXV], in[MAXV], par[MAXV];
  vector<int> adj[MAXV];

  void AddEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void GetSize(int cur, int prv) {
    sz[cur] = 1, par[cur] = prv;
    for (auto &nxt : adj[cur]) {
      if (nxt == prv) continue;
      GetSize(nxt, cur);
      sz[cur] += sz[nxt];
      if (adj[cur].front() == prv || sz[nxt] > sz[adj[cur].front()])
        swap(cur, adj[cur].front());
    }
  }

  void Ordering(int cur, int prv) {
    in[cur] = ++pv;
    for (auto &nxt : adj[cur]) {
      if (nxt == prv) continue;
      head[nxt] = (adj[cur].front() == nxt ? head[cur] : nxt);
      Ordering(nxt, cur);
    }
  }

  void Init() {
    pv = head[0] = 0;
    memset(sz, 0, sizeof sz), memset(in, 0, sizeof in);
    for (int i = 0; i < MAXV; i++) if (!sz[i]) GetSize(i, i);
    for (int i = 0; i < MAXV; i++) if (!in[i]) Ordering(i, i);
  }

  int Query(int u, int v) {
    int res = 0;
    while (head[u] != head[v]) {
      if (sz[head[u]] > sz[head[v]]) swap(u, v);
      res += seg.Query(in[head[u]], in[u]);
      u = par[head[u]];
    }
    if (sz[u] < sz[v]) swap(u, v);
    res += seg.Query(in[u], in[v]);
    return res;
  }

  void Update(int i, int v) { seg.Update(in[i], v); }
};