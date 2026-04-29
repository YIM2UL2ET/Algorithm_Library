const int LOG = 18;
struct LCA {  // 1-index
  int dep[1 << LOG], par[1 << LOG][LOG]; vector<int> adj[1 << LOG];

  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void init(int cur = 1, int prv = 0, int depth = 1) {  // default: root == 1, O(nlogn)
    par[cur][0] = prv, dep[cur] = depth;
    for (int i = 1; i < LOG; i++) par[cur][i] = par[par[cur][i - 1]][i - 1];
    for (auto &nxt : adj[cur])
      if (nxt != prv) init(nxt, cur, depth + 1);
  }

  int get_lca(int u, int v) {  // O(logN)
    if (dep[u] < dep[v]) swap(u, v);
    for (int i = LOG - 1; i >= 0; i--)
      if (dep[u] - dep[v] >= (1 << i)) u = par[u][i];
    if (u == v) return u;
    for (int i = LOG - 1; i >= 0; i--)
      if (par[u][i] != par[v][i]) u = par[u][i], v = par[v][i];
    return par[u][0];
  }
}; 