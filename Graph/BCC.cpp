const int SZ = 101010;
struct BCC {  // 0-index, O(N + M)
  using Func = function<void(int&, int, int)>;
  int n, cnt, in[SZ]; stack<pii> stk; vector<int> adj[SZ];
  BCC(int n) : n(n), cnt(0) {}

  inline void push(vector<pii>& compo, stack<pii>& stk) {
    compo.push_back(stk.top()); stk.pop();
  }

  void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }

  int dfs(int cur, int prv, Func& func) {
    int cmin = in[cur] = ++cnt;
    for (const auto& nxt : adj[cur])
      if (nxt != prv) func(cmin, cur, nxt);
    return cmin;
  }

  vector<vector<pii>> get_bcc() {
    vector<vector<pii>> compo;
    Func func = [&](int& cmin, int cur, int nxt) -> void {
      if (in[cur] > in[nxt]) stk.emplace(cur, nxt);
      if (in[nxt] > 0) {
        cmin = min(cmin, in[nxt]);
      } else {
        int nmin = dfs(nxt, cur, func);
        cmin = min(cmin, nmin);
        if (nmin >= in[cur]) {
          compo.push_back({});
          while (stk.top() != make_pair(cur, nxt)) push(compo.back(), stk);
          push(compo.back(), stk);
        }
      }
    };
    memset(in, 0, sizeof in), cnt = 0;
    for (int i = 0; i < n; i++)
      if (in[i] == 0) dfs(i, -1, func);
    return compo;
  }

  vector<pii> articulation_bridge() {
    vector<pii> bridge;
    Func func = [&](int& cmin, int cur, int nxt) -> void {
      if (in[nxt] > 0) {
        cmin = min(cmin, in[nxt]);
      } else {
        int nmin = dfs(nxt, cur, func);
        cmin = min(cmin, nmin);
        if (nmin > in[cur]) bridge.emplace_back(cur, nxt);
      }
    };
    memset(in, 0, sizeof in), cnt = 0;
    for (int i = 0; i < n; i++)
      if (in[i] == 0) dfs(i, -1, func);
    return bridge;
  }

  vector<int> articulation_point() {
    vector<int> point;
    int degree[SZ], is_root[SZ], atc_p[SZ];
    Func func = [&](int& cmin, int cur, int nxt) -> void {
      if (in[nxt] > 0) {
        cmin = min(cmin, in[nxt]);
      } else {
        int nmin = dfs(nxt, cur, func);
        cmin = min(cmin, nmin), degree[cur]++;
        if (nmin >= in[cur]) atc_p[cur] = true;
      }
    };
    cnt = 0;
    for (int i = 0; i < n; i++) atc_p[i] = degree[i] = in[i] = is_root[i] = 0;
    for (int i = 0; i < n; i++)
      if (in[i] == 0) is_root[i] = true, dfs(i, -1, func);
    for (int i = 0; i < n; i++)
      if (atc_p[i] && (!is_root[i] || degree[i] >= 2)) point.push_back(i);
    return point;
  }
};