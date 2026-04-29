struct Kosaraju {  // 0-index
  int N;  stack<int> st; vector<int> group; vector<vector<int>> adj, rvs_adj;
  
  Kosaraju(int N) : N(N), group(N), adj(N), rvs_adj(N) {}

  void add_edge(int u, int v) {
    adj[u].push_back(v);
    rvs_adj[v].push_back(u);
  }

  pair<int, vector<int>> strongly_connected_component() {
    int cnt = 0;
    fill(group.begin(), group.end(), 0);
    for (int i = 0; i < N; i++) dfs(i);
    fill(group.begin(), group.end(), 0);
    while (!st.empty()) {
      int cur = st.top();
      st.pop();
      if (!group[cur]) rvs_dfs(cur, ++cnt);
    }
    return make_pair(cnt, group);
  }
  
  void dfs(int cur) {
    if (group[cur]) return;
    group[cur] = 1;
    for (auto &nxt : adj[cur]) dfs(nxt);
    st.push(cur);
  }
  
  void rvs_dfs(int cur, int num) {
    if (group[cur]) return;
    group[cur] = num;
    for (auto &nxt : rvs_adj[cur]) rvs_dfs(nxt, num);
  }
};